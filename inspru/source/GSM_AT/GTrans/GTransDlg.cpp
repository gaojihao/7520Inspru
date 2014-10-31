// GTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GTrans.h"
#include "GTransDlg.h"
#include "./gprs/GPRSLogic.h"
#include "./gprs/gsm_reset.h"
#include "IniFile.h"
#include "./gprs/ex_file.h"
#include "./gprs/rectrl_api.h"
#include "./gprs/gsm_parse.h"

#include "./gprs/ex_file.h"
#include "LogTool/LogTool.h"

#include <map>
#include <utility>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGTransDlg dialog
#define  TID_GSM_INIT							0x1001
#define  TID_CONNECT_IP					0x1003
#define  TID_LOGIN_SVR						0x1004
#define  TID_ACK_SETTIME					0x1005
#define  TID_ACK_SETNET					 0x1006
#define  TID_ACK_NOTICE						 0x1007
#define  TID_ACK_SPEEDALERT         0x1008
#define  TID_DETECT_GSM					 0x1009
#define  TID_SYSTEM_STARTUP         0x3003
#define  TID_STATUS_REPORT			 0x3007   //向窗口发送消息线程


CGTransDlg* g_hMainUI = 0;
extern BOOL   g_bReadPhoneBooking;
void   ShowTip(CString strTip);
CIniFile iniFile;

CString g_csLogoFileName;
char	g_szLogoFileName[128];

msg_ipc*        g_pIPC;		//
char			g_szAPN[16];	//CMNET/CMWAP


#define   _GPRS_ENABLE           1
CGTransDlg::CGTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGTransDlg::IDD, pParent)
{
	memset(m_stHWNDSet, NULL, sizeof(m_stHWNDSet));
	memset(&m_stIniFile, NULL, sizeof(m_stIniFile));	

	m_hNaviHwnd = NULL;
	g_hMainUI = this;
	m_bNoticeRst = FALSE;
	m_nLogCount = 0;
	m_nDebugLog = TRUE;
	m_nDebugWin = TRUE;
	m_nResetForSim = 0;

	m_bStartListen = FALSE; //不是在监听
}

void CGTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGTransDlg)
	DDX_Control(pDX, IDC_LIST_LOG, m_ListLog);
}

BEGIN_MESSAGE_MAP(CGTransDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_GSM_POWERON, OnGsmPoweron)
	ON_BN_CLICKED(IDC_GSM_POWEROFF, OnGsmPoweroff)
	ON_BN_CLICKED(IDC_GPS_VOICE, OnGpsVoice)
	ON_BN_CLICKED(IDC_GPS_Dial, OnGPSDial)
	ON_BN_CLICKED(IDC_BTN_HANGUP, OnBtnHangup)
	ON_BN_CLICKED(IDC_BTN_PLAYV, OnBtnSendGPRS)
	ON_BN_CLICKED(IDC_BTN_SAVELOG, OnBtnSavelog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGTransDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText( WND_NAME_GSM_AT );

	//打开GSM模块串口，启动串口监听线程
	SetTimer( TID_SYSTEM_STARTUP, 8 * 1000, NULL );

	m_ipc.init_target(NULL, NULL);
	m_GsmMsgCtrl.set_ipc( &m_ipc );
	g_pIPC = &m_ipc;

	m_nDebugWin = 0;
	gsm_module::set_notify( GetSafeHwnd(), MSG_GSM_RESET_OVER );
	FmtLogInfo( _T("准备启动监控程序......") );

#ifdef DEBUG
	m_nDebugWin = 1;
	m_nDebugLog = 1;
#endif

	if ( m_nDebugWin == 0 )
		MoveWindow(0,0,0,0);
	else
		::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, 0,0,800,480,SWP_NOMOVE | SWP_NOSIZE );

	CGPRSLogic::Instance()->GSMSetCalling( FALSE );

	::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, 0,0,800,480,SWP_NOMOVE | SWP_NOSIZE );

	return TRUE;
}

void CGTransDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_SYSTEM_STARTUP )
	{
		KillTimer( nIDEvent );
		if ( CGPRSLogic::Instance()->InitWathing( GetSafeHwnd(), &m_ipc, 7 ) )
		{
			CLogTool::Instance()->WriteLogFile(_T("InitWathing success"));
			SetTimer( TID_GSM_INIT, 50, NULL );
		}
		else 
		{
			CLogTool::Instance()->WriteLogFile(_T("InitWathing failure"));
		}
	}

	//初始化GSM模块和网络
	else if ( nIDEvent == TID_GSM_INIT )
	{
		KillTimer( nIDEvent );
		CGPRSLogic::Instance()->SetState( eGState_InitGSM );
		CLogTool::Instance()->WriteLogFile(_T("TID_GSM_INIT StartConfig"));
		CGPRSLogic::Instance()->StartConfig();

		SetTimer(TID_DETECT_GSM, 120*1000, NULL);
	}
	else if (TID_DETECT_GSM == nIDEvent)
	{
		CLogTool::Instance()->WriteLogFile(_T("TID_DETECT_GSM StartConfig"));
		bool bOK = CGPRSLogic::Instance()->DetectGSM();

		if (bOK)
		{
			CLogTool::Instance()->WriteLogFile(_T("TID_DETECT_GSM Report to WirelessManager"));
			HWND hWnd = ::FindWindow(NULL, WIRELESS_MANAGER_TITLE);
			if(hWnd != NULL)
				::PostMessage(hWnd, MSG_RAS_CONNECT, 8, 0);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CGTransDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if ( message == WM_COPYDATA )
	 {
		m_GsmMsgCtrl.on_message(message, wParam, lParam );
	 }
	else if ( message == UMSG_GSM_CMD )
	 {
		 if ( wParam == GSM_CMD_DIAL )
		{
			CGPRSLogic::Instance()->GSMSetCalling(TRUE);
			m_GsmMsgCtrl.on_message(message, wParam, lParam );
		}
		else if (GSM_CMD_SENDMSG ==wParam)
		{
			 m_GsmMsgCtrl.on_message(message, wParam, lParam );
		}
		else if (GSM_CMD_DIAL_EXT == wParam ||GSM_CMD_SET_VOL == wParam)
		{
			m_GsmMsgCtrl.on_message(message, wParam, lParam );
		}
		else if (wParam == GSM_CMD_LISTEN)
		{
			CGPRSLogic::Instance()->GSMSetCalling(TRUE);
			m_GsmMsgCtrl.on_message(message, wParam, lParam );
		}
		else if ( wParam == GSM_SIM_PBOOK_READ )
		{
			gsm_api_handup();
			Sleep(1000);
			g_bReadPhoneBooking = FALSE;
		}
		else if ( wParam == GSM_CMD_HANGUP )	//电话打进->挂断电话
		{
			CGPRSLogic::Instance()->GSMSetCalling( FALSE );
			gsm_api_handup();
			gsm_module::switch_voice2gps();
		}

		else if ( wParam == GSM_CMD_ACCEPTED )					//电话打进->接听电话
		{
			m_GsmMsgCtrl.on_message(message, wParam, lParam );
		}
	}

	//从GSM模块串口发过来的消息
	else if ( message == MSG_GSM_NOTIFY )
	{
		m_MsgCtrl.OnMsgCtrl( wParam, lParam );
		OnLogicGsmMsg( wParam, lParam );
		return TRUE;
	}
	else if ( message == MSG_GSM_RESET_OVER )
	{
		if ( 1 == wParam )
		{
			//reset模块后重新打开串口配置模块
			CLogTool::Instance()->WriteLogFile(_T("MSG_GSM_RESET_OVER"));
			KillTimer( TID_SYSTEM_STARTUP );//TID_GSM_INIT
			SetTimer( TID_SYSTEM_STARTUP, 50, NULL );
		}
		else
		{
			if ( CGPRSLogic::Instance()->GSMIsCalling() )
			{
				CGPRSLogic::Instance()->SetState( eGState_Error );
				FmtLogInfo( _T("GPRS模块重新启动::系统正在拨号中....!") );
			}
		}

		return TRUE;
	}
	else if (WM_GSM_RESET == message)
	{
		//WirelessManager.exe -> GSM_AT.exe reset GSM模块
		//启动reset线程
		KillTimer(TID_DETECT_GSM);
		CLogTool::Instance()->WriteLogFile(_T("WM_GSM_RESET == message start reset_hd"));
		gsm_module::reset_hd();
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CGTransDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void  CGTransDlg::ReportGSMState()
{
	int nValue = CGPRSLogic::Instance()->m_gsm_state.sim_card_state;
	m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_SIM_STATE, nValue );

	nValue = CGPRSLogic::Instance()->m_gsm_state.gsm_signal;
	m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_SIGNAL, nValue );

	nValue = CGPRSLogic::Instance()->m_gsm_state.gsm_network_sel;
	m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_QUERY_NET, nValue );

	nValue = CGPRSLogic::Instance()->m_gsm_state.m_sim_sms;
	m_ipc.post_msg( UMSG_GSM_CMD, eGSM_READ_SIM_SMS_RST, nValue );

	nValue = CGPRSLogic::Instance()->m_gsm_state.m_sim_pbook;
	m_ipc.post_msg( UMSG_GSM_CMD, eGSM_READ_SIM_PBOOK_RST, nValue );
}
//////////////////////////////////////////////////////////////////////////



void  CGTransDlg::OnLogicGsmMsg( unsigned nType, unsigned nParam )
{
	switch ( nType )
	{
		//the result of config GSM module
	case eGSM_CONFIG_RST:
		break;

		// check the SIM Card result
	case eGSM_SIM_CARD_RST:
		{
			if ( FALSE == nParam )
			{
				m_nResetForSim++;		
			}
			else
			{
				m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_SIM_STATE, 1 );
				CGPRSLogic::Instance()->m_gsm_state.sim_card_state = 1;
			}
		}
		break;

		// GSM signal quality
	case eGSM_SIGNAL_VALUE:
		{
			CGPRSLogic::Instance()->m_gsm_state.gsm_signal = nParam;
			m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_SIGNAL, nParam );
		}
		break;

		// query GSM network provider
	case eGSM_QUERY_GSM_ISP:
		{
			CGPRSLogic::Instance()->m_gsm_state.gsm_network_sel = nParam;
			m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_QUERY_NET, nParam );
			//EXDBG_TRACE1("Current network provider is : %d\n", nParam );
		}
		break;
	case eGSM_REGISTER_GSM_RST:
		{	
			ReportGSMState();
			if ( nParam == FALSE )
			{
				ShowTip( _T("Reseting GSM::[REGISTER_GSM]......") );
				KillTimer( TID_GSM_INIT );
			}			
		}
		break;
		// register GPRS result
	case eGSM_REGISTER_GPRS_RST:
		{
			if ( nParam == FALSE )
			{
				CGPRSLogic::Instance()->SetState( eGState_Error );
			}
		}
		break;
	
	case eGSM_DIAL_CONNECT_RST:
		{
			if ( nParam == 1 )			//对方接通电话
			{
				CGPRSLogic::Instance()->GSMSetCalling( TRUE );
				m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT,1 );
				m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT,1 );
				HWND hWnd = ::FindWindow(NULL, WIRELESS_MANAGER_TITLE);
				if(hWnd)
				{
					::PostMessage(hWnd, UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT, 1);
				}
			}
			else if ( nParam == 0 )		//对方挂断电话
			{
				CGPRSLogic::Instance()->GSMSetCalling( FALSE );
				m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_LINE_DISCONNECT,0 );
				m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_LINE_DISCONNECT,0 );
				m_bStartListen = FALSE;
			}
		}
		break;
	case eGSM_NEW_CALLING_RST:			//有来电
		{

			CGPRSLogic::Instance()->GSMSetCalling( TRUE );

			char* p_num = (char*)nParam;
			cex_file file;
			if ( !file.is_exist(PATH_NEW_CALL_INCOMING) )
				file.create( PATH_NEW_CALL_INCOMING );

			char sz_buf[64];
			memset( sz_buf, 0, sizeof(sz_buf) );
			int  n_size = strlen( p_num );
			if ( n_size >= 64 ) n_size = 63;
			memcpy( sz_buf, p_num, n_size );


			if ( file.open(PATH_NEW_CALL_INCOMING, cex_file::m_write) )
			{
				file.seek2begin();
				file.write( sz_buf, n_size + 1 );
				file.flush();
				file.close();
			}


			if ( m_ipc.init_target(0,0)  )
			{
				m_ipc.post_msg( UMSG_GSM_CMD, GSM_ACK_NEW_CALLING,  0 );
			}
			else
			{
				PostMessage(UMSG_GSM_CMD,GSM_CMD_HANGUP,0);
			}

		}
		break;
	case eGSM_NEW_SMS_COMING:
		{
			m_GsmMsgCtrl.read_sim_sms( nParam );
		}
		break;
	case eGSM_READ_NEW_SMS_CONTENT:
		m_GsmMsgCtrl.Get_sms_content( nParam );
		break;
	case eGSM_READ_SIM_SMS_RST:
		{
			CGPRSLogic::Instance()->m_gsm_state.m_sim_sms = nParam;
			m_GsmMsgCtrl.set_sim_sms( nParam );
		}
   
		break;
	case eGSM_READ_SIM_PBOOK_RST:
		{
			m_GsmMsgCtrl.set_sim_pbook( nParam );
			CGPRSLogic::Instance()->m_gsm_state.m_sim_pbook = nParam;
		}
		break;

	default:
		break;
	}
}


void   CGTransDlg::FmtLogInfo(CString strLog)
{
	if ( m_nDebugLog == 0 ) return;

	if ( strLog.IsEmpty() )
	{
		int x = 2;
		x = 3;
	}
	
	CString str;
	SYSTEMTIME sysTime;
	memset( &sysTime, 0, sizeof(sysTime) );
	::GetLocalTime( &sysTime );
	
	str.Format(_T("(%d)%02d:%02d:%02d->%s"),
		m_nLogCount++,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		strLog );

	if ( m_nLogCount % 100 == 0  )
	{
		m_ListLog.ResetContent();
	}
	m_ListLog.InsertString( 0, str );
}

void CGTransDlg::OnBtnExit() 
{
	OnDestroy();
}


void CGTransDlg::OnDestroy() 
{
	CGPRSLogic::Instance()->UnInit();
	CDialog::OnDestroy();	
}


void   ShowTip(CString strTip)
{
	g_hMainUI->FmtLogInfo( strTip );
}


void  CGTransDlg::InitCtrl()
{
	MoveWindow(0, 0, 800, 480);
	CenterWindow(GetDesktopWindow());
}

CString     CGTransDlg::GenerateFilePath(CString strFileName)
{
	TCHAR tchDir = '\\';
	CString strTemp, strIniPathName;
	GetModuleFileName(NULL, strTemp.GetBuffer(MAX_PATH), MAX_PATH);
	strTemp.ReleaseBuffer();
	
	int PathPos = strTemp.ReverseFind(tchDir) + 1;
	
	strIniPathName = strTemp.Left(PathPos);
	strIniPathName += strFileName;
	return strIniPathName;
}

void  CGTransDlg::NotifyNaviRst(int nType, int nResult)
{
	if ( m_hNaviHwnd )
		::SendMessage( m_hNaviHwnd, MSG_GPRS2NAVI, nType, nResult );
}


void CGTransDlg::OnGsmPoweron() 
{

}

void CGTransDlg::OnGsmPoweroff() 
{
}
extern bool    gsm_api_sms_send(const char* p_phone_number, const char* p_sms_msg);
void CGTransDlg::OnGpsVoice()
{
	return;
}

void CGTransDlg::OnGPSDial()
{
	
}

void CGTransDlg::OnBtnHangup() 
{

}

void CGTransDlg::OnBtnSendGPRS() 
{//对GSM模块reset
	gsm_module::reset_hd();
} 

void CGTransDlg::OnBtnSavelog() 
{
	m_ListLog.ResetContent();	
}
