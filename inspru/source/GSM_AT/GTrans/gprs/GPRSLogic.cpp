// GPRSLogic.cpp: implementation of the CGPRSLogic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPRSLogic.h"
#include "gsm_api.h"
#include "gsm_ctrl.h"
#include "../UIMsgCtrl.h"
#include "../resource.h"
#include "gsm_reset.h"
#include "rectrl_api.h"
#include "../GTransDlg.h"
#include "../LogTool/LogTool.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern gsm_ctrl			g_ctrl;      // GSM message control send & read

BOOL  g_bSimReset = FALSE;
char    g_szIP[32];
char    g_szType[4];
int     g_nPort = 7605;
void    ThreadConfigGSM( void *pParam );

//主对话框窗口句柄
extern HWND	g_hWndMainDlg;	

CGPRSLogic CGPRSLogic::m_instance;

CGPRSLogic* CGPRSLogic::Instance()
{
	return &m_instance;
}


CGPRSLogic::CGPRSLogic()
{
	memset( g_szIP, 0, sizeof(g_szIP) );
	memset( g_szType, 0, sizeof(g_szType) );
	g_nPort = 0;
	m_state = eGState_NULL;
	m_ipc = 0;
	m_gsm_state.gsm_signal = 0;
	m_gsm_state.sim_card_state = gsm_sim_null;
	m_gsm_state.gsm_network_sel = gsm_net_null;
	m_gsm_state.gsm_is_dialing = false;
	m_gsm_state.m_sim_sms = 0;
	m_gsm_state.m_sim_pbook = 0;
	m_hThrdConfigGSM = NULL;
	m_bdialing       = false;
}

//////////////////////////////////////////////////////////////////////////

CGPRSLogic::~CGPRSLogic( )
{
	//gsm_api_close();
}
//////////////////////////////////////////////////////////////////////////

void   CGPRSLogic::SetServerInfo(const char* pIP, int nPort,const char* IpType, int nSlice /*= 10*/ )
{
	g_nPort = nPort;
	int nLen = strlen( pIP );
	if ( nLen > sizeof(g_szIP)-1  )
		nLen = sizeof(g_szIP) - 1;
	memset( g_szIP, 0, sizeof(g_szIP) );
	memcpy( g_szIP, pIP, nLen );

	memset( g_szType, 0, sizeof(g_szType) );
	memcpy( g_szType, IpType, 3 );
}


void   CGPRSLogic::UnInit()
{
	gsm_api_close();
}

void   CGPRSLogic::GSMSetCalling(BOOL bCall)
{
	m_gsm_state.gsm_is_dialing = bCall; 
	m_bdialing = bCall;
}

//////////////////////////////////////////////////////////////////////////

BOOL   CGPRSLogic::InitWathing( HWND hNotifyWnd , msg_ipc* p_ipc, int nGSMPort)
{
	g_ctrl.set_parse_ptr( &m_parse);
	m_parse.set_notify( hNotifyWnd, p_ipc );
	m_ipc = p_ipc;

	Sleep(1000);

	if ( !gsm_api_start(nGSMPort,115200 ) )//COM7
	{
		AfxMessageBox( _T("Open GSM COM7, baudrate 115200 failure!") );
		return FALSE;
	}

	return TRUE;
}

char*  CGPRSLogic::GCMD_IMEI()
{
	return gsm_api_imei();
}

void    CGPRSLogic::GCDM_QuerySignal()
{
	gsm_api_signal();
}


void    CGPRSLogic::GCMD_ShutConnect()
{
	
}


BOOL    CGPRSLogic::GPRS_Send(const char* p_data, int n_size )
{
	return TRUE;
}


char*   CGPRSLogic::GPRS_Buf()
{
	return g_ctrl.ipd_ack();
}

int    CGPRSLogic::GPRS_BufSize()
{
	return g_ctrl.ipd_len();
}


void     CGPRSLogic::CloseGSM()
{
	//停止ReportGPS数据
	m_state = eGState_NULL; 
	//关闭GSM的所有功能
	gsm_api_close();
}

void CGPRSLogic::ResetGSM()
{
	m_gsm_state.gsm_signal = 0;
	m_gsm_state.sim_card_state = gsm_sim_null;
	m_gsm_state.gsm_network_sel = gsm_net_null;
	m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_SIM_STATE, m_gsm_state.sim_card_state );
	m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_SIGNAL, m_gsm_state.gsm_signal );
	m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_QUERY_NET, m_gsm_state.gsm_network_sel );
	//通知menu.exe网络状态改变
	m_ipc->post_msg( WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, 2 );	//RAS拨号:  2 断开    3 连接

	::PostMessage( HWND_BROADCAST,0xC020, 0x06, 0 );
}

bool CGPRSLogic::DetectGSM()
{
		int nValue = gsm_api_query_net_isp();
		g_ctrl.empty_data();

		if (nValue > 0)
		{
			return true;
		}

		return false;
}


BOOL   CGPRSLogic::StartConfig()
{
	unsigned long nThrdID = 0;
	m_hThrdConfigGSM = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadConfigGSM,
		&m_parse,
		0,
		&nThrdID );

	return ( m_hThrdConfigGSM != INVALID_HANDLE_VALUE );
}

//////////////////////////////////////////////////////////////////////////


void ThreadConfigGSM( void *pParam )
{
	gsm_parse* pNotify = (gsm_parse*)pParam;
	bool bSuc = FALSE;
	int  i = 0;

	// Config GSM param
	gsm_api_config( true );
	g_ctrl.empty_data();
	pNotify->msg_report2ui( eGSM_CONFIG_RST, bSuc );
	Sleep( 50 );

	// query GSM signal quality
	for( i = 0; i < 5; i++ )
	{
		int nValue = gsm_api_query_signal();
		CGPRSLogic::Instance()->m_gsm_state.gsm_signal = nValue;
		pNotify->msg_report2ui( eGSM_SIGNAL_VALUE, nValue );
		break;
	}
	g_ctrl.empty_data();
	Sleep( 100 );

	// check SIM card
	for( i = 0; i < 5; i++ )
	{
		bSuc = gsm_api_check_sim();
		if(bSuc)
			break;
	}
	pNotify->msg_report2ui( eGSM_SIM_CARD_RST, bSuc );
	CGPRSLogic::Instance()->m_gsm_state.sim_card_state = bSuc;
	g_ctrl.empty_data();
	Sleep( 10 );

	pNotify->msg_report2ui( eGSM_REGISTER_GSM_RST, bSuc );

	int nValue = 0;
	// query GSM ISP
	for( i = 0; i < 5; i++ )
	{
		nValue = gsm_api_query_net_isp();
		CGPRSLogic::Instance()->m_gsm_state.gsm_network_sel = nValue;
		if ( nValue > 0 )
		{
			pNotify->msg_report2ui(eGSM_QUERY_GSM_ISP, nValue );
			break;
		}
	}
	g_ctrl.empty_data();
	Sleep( 10 );


	gsm_api_config2();

	if(nValue > 0)
	{
		//找到sim卡后
		//通知WirelessManager.exe进程，让其开始ras拨号
		CLogTool::Instance()->WriteLogFile(_T("Report to WirelessManager"));
		HWND hWnd = ::FindWindow(NULL, WIRELESS_MANAGER_TITLE);
		if(hWnd != NULL)
			::PostMessage(hWnd, MSG_RAS_CONNECT, 8, 0);
	}

	return;
}
