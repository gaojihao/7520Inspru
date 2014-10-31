// WirelessManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WirelessManager.h"
#include "WirelessManagerDlg.h"

#include "Tools/LogTool.h"

#include <Ipexport.h>
#include <Icmpapi.h>
#include <winsock.h>
#include <iphlpapi.h>
#include <iptypes.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_COMMAND_TIANZE			WM_USER+0x1000	//同天泽进程通讯的消息
#define WM_GSM_RESET				WM_USER+0x1001	//WirelessManager.exe -> GSM_AT.exe reset GSM模块
#define WM_ACC_ON_OFF				(WM_USER+0x320)			//ACC通知：wParam：0  ACC掉电  1  ACC上电
#define WM_SIMCARD_REQUEST			(WM_USER+0x322)
#define MSG_SINGAL_VALUE			(WM_USER+0x2227)
//网络状态: 登录服务器:	0 掉线  1 接通  RAS拨号:  2 断开    3 连接
#define WPARAM_SERVER_LOGIN			10	

#define VALUE_LOW_GSM_SIGNAL		10	//低信号值
#define LOW_SIGNAL_COUNT			3	//低信号值计算次数
#define WND_NAME_GSM_AT				_T("WND_GSM_AT")

//连接状态
ConnectStatus	g_enConnectStatus;
//当g_enConnectStatus为CONNECT_STATUS_DISCONNECTED时，代表断开的附加原因
ExtraStatus		g_enExtraStatus;

CWirelessManagerDlg::CWirelessManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWirelessManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RasState		= RASCS_Disconnected;

	memset(m_stHWNDSet, NULL, sizeof(m_stHWNDSet));
	g_enConnectStatus	= CONNECT_STATUS_UNKNOWN;
	g_enExtraStatus		= UN_KNOW;
	m_hRasConn			= NULL;
	//m_bGSMReset			= FALSE;		//True：gsm模块正在reset，False：gsm正常工作
	m_dwReDialCnt		= 0;
	m_bDialing          = false;
}

void CWirelessManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWirelessManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_DIAL, OnBtnDial)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STATUS, OnBtnStatus)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PING, &CWirelessManagerDlg::OnBnClickedBtnPing)
END_MESSAGE_MAP()


// CWirelessManagerDlg message handlers

BOOL CWirelessManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	GetDlgItem( IDC_BTN_DIAL)->SetWindowText( _T("拨号") );
	
	SetWindowText( WND_NAME_WIRELESS_MANAGER );

#ifndef _DEBUG
	//MoveWindow(0, 0, 0, 0);
#endif
	MoveWindow(0, 0, 0, 0);
	CreateThread(NULL, 0, CWirelessManagerDlg::THDStartUp, this, 0, NULL);
	

	//LoadConfig();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWirelessManagerDlg::OnBtnDial()
{
	GetDlgItem(IDC_BTN_DIAL)->EnableWindow(FALSE);	//设置按钮状态
	if(RASCS_Disconnected == m_RasState)
	{
		GetDlgItem( IDC_BTN_DIAL)->SetWindowText( _T("挂断...") );
		RasControl(enRAS_Dial);
	}
	else
	{
		GetDlgItem( IDC_BTN_DIAL)->SetWindowText( _T("拨号") );
		RasControl(enRAS_Hangup);
	}
}

DWORD CWirelessManagerDlg::DialRasNetwork()
{
	
	//拨号之前先判断上次拨号的句柄是否已释放
	if (m_hRasConn != NULL)
	{
		RasHangUp(m_hRasConn);
		m_hRasConn = NULL;
	}


	if (m_dwReDialCnt > 5)
	{
		RasControl(enRAS_Hangup);
		m_dwReDialCnt = 0;
		return 0;
	}
	m_dwReDialCnt++;

	g_bIsDialing = TRUE;
	RASDIALPARAMS   RasDialParams;   //RAS结构包含用来建立RAS的参数   
	//下面开始初始化拨号参数
	memset((char*)&RasDialParams, 0, sizeof(RasDialParams));
	RasDialParams.dwSize   =   sizeof(RASDIALPARAMS);   //结构大小   
	wcscpy(RasDialParams.szEntryName, /*_T("cmnet")*/ENTRY_NAME);		//拨号网络中建立的电话簿名
	
   	wcscpy(RasDialParams.szPhoneNumber , _T(""));	//*99***#
    wcscpy(RasDialParams.szCallbackNumber,_T(""));   //回拨号码，不用时置为NULL   
    wcscpy(RasDialParams.szUserName,_T(""));   //用户名   
    wcscpy(RasDialParams.szPassword,_T(""));   //用户口令
	wcscpy(RasDialParams.szDomain,_T(""));   //用户权限验证域

	RASDIALPARAMS ras_para_test;
	memset(&ras_para_test, NULL, sizeof(RASDIALPARAMS));

	int nRasResult = RasDial(NULL, NULL, &RasDialParams, 0xFFFFFFFF, m_hWnd, &m_hRasConn);
	g_bIsDialing = FALSE;
	
	return nRasResult;
}

void CWirelessManagerDlg::RasControl(DIAL_STYPE enStype)
{
	m_enDialStype = enStype;	//保存拨号类型
	switch(enStype)
	{
	case DIAL_TYPE_NONE:
		break;
	case enRAS_Dial://当前拨号状态为断开，
		if( m_RasState == RASCS_Disconnected)
		{
			DialRasNetwork();			
		}
		break;
	case enRAS_Hangup:
		{
			RasHangUp(m_hRasConn);
			m_hRasConn = NULL;
			KillTimer(TID_DISCONNECT);
			m_RasState = RASCS_Disconnected;

			HWND hwnd = ::FindWindow(NULL,WND_NAME_GSM_AT);
			::PostMessage(hwnd, WM_GSM_RESET, 0, 0);	
		}
		break;

	case enRAS_ReDial:
		if(m_RasState != RASCS_Disconnected)
		{
			RasHangUp(m_hRasConn);
			m_hRasConn = NULL;
			KillTimer(TID_DISCONNECT);
		}
		DialRasNetwork();

		break;
	}
}

void CWirelessManagerDlg::ParseRasDialEvent( WPARAM wParam, LPARAM lParam)
{
	RASCONNSTATE rasState = (RASCONNSTATE)wParam;
	m_RasState = rasState;
	UpdateRASState( rasState );
	
	if( RASCS_Disconnected == rasState )
	{
		if(m_enDialStype == enRAS_Dial)
		{
			KillTimer(TID_DIAL);
			SetTimer(TID_DIAL, 15*1000, NULL);
			CLogTool::Instance()->WriteLogFile("TID_DIAL: receive disconnect: m_enDialStype == enRAS_Dial");
		}
		else if(m_enDialStype == enRAS_ReDial)
		{
			KillTimer(TID_DIAL);
			SetTimer(TID_DIAL, 15*1000, NULL);
			CLogTool::Instance()->WriteLogFile("TID_DIAL: receive disconnect: m_enDialStype == enRAS_ReDial");
		}
		//强制断开ras成功
		else if(m_enDialStype == enRAS_Hangup)
		{
			//关掉重连定时器
			KillTimer(TID_DISCONNECT);
			CLogTool::Instance()->WriteLogFile("TID_DIAL: receive disconnect: m_enDialStype == enRAS_Hangup");
		}
	}
	else if(RASCS_Connected == rasState)
	{
		m_dwReDialCnt = 0;	//连接后，复位重拨计数器
		CLogTool::Instance()->WriteLogFile("TID_DIAL: receive connected");
		GetDlgItem(IDC_BTN_DIAL)->EnableWindow(TRUE);	//设置按钮状态
	}
}

void CWirelessManagerDlg::UpdateRASState( RASCONNSTATE state )
{
	//设置超时定时器
	KillTimer(TID_DISCONNECT);
	SetTimer(TID_DISCONNECT, 20*1000, NULL);	//设置拨号超时定时器
	CString szRASState;

	switch(state)
	{
    case RASCS_OpenPort:
		szRASState = _T("Opening port...");
		PostMsg2WND(CONNECT_STATUS_WAITINGCONNECTION);
		break;
    case RASCS_PortOpened:
		szRASState = _T("Port opened.");
		break;
    case RASCS_ConnectDevice:
		szRASState = _T("Connecting Device...");
		break;
    case RASCS_DeviceConnected:
		szRASState = _T("Device connected.");
		break;
    case RASCS_AllDevicesConnected:
		szRASState = _T("All devices connected.");
		break;
    case RASCS_Authenticate:
		szRASState = _T("Authenticating...");
		break;
    case RASCS_AuthNotify:
		szRASState = _T("Authentication notify.");
		break;
    case RASCS_AuthRetry:
		szRASState = _T("Authentication retrying...");
		break;
    case RASCS_AuthCallback:
		szRASState = _T("Authentication callback.");
		break;
    case RASCS_AuthChangePassword:
		szRASState = _T("Requested to change the password ");
		break;
    case RASCS_AuthProject:
		szRASState = _T("The projection phase is starting.");
		break;
    case RASCS_AuthLinkSpeed:
		szRASState = _T("The link-speed calculation phase is starting.");
		break;
    case RASCS_AuthAck:
		szRASState = _T("An authentication request is being acknowledged.");
		break;
    case RASCS_ReAuthenticate:
		szRASState = _T("Reauthentication (after callback) is starting.");
		break;
    case RASCS_Authenticated:
		szRASState = _T("Authentication completed.");
		break;
    case RASCS_PrepareForCallback:
		szRASState = _T("About to disconnect in preparation for callback.");
		break;
    case RASCS_WaitForModemReset:
		szRASState = _T("Wait for the modem reset.");
		break;
    case RASCS_WaitForCallback:
		szRASState = _T("Waiting for an incoming call from the remote access server.");
		break;
    case RASCS_Projected:
		szRASState = _T("Projection result data is available");
		break;
    case RASCS_Interactive:
		szRASState = _T("Interactive.");
		break;
    case RASCS_RetryAuthentication:
		szRASState = _T("Retry authentication.");
		break;
    case RASCS_CallbackSetByCaller:
		szRASState = _T("Callback set by caller.");
		break;
    case RASCS_PasswordExpired:
		szRASState = _T("Password expired.");
		break;
	case RASCS_Connected:
		KillTimer(TID_DISCONNECT);
		szRASState = _T("Connected.");
		PostMsg2WND(CONNECT_STATUS_CONNECTED);
		SendState2Menu(3);
		m_dwReDialCnt = 0;	//连接后，复位重拨计数器
		break;
    case RASCS_Disconnected:
		szRASState = _T("Disconnected.");
		PostMsg2WND(CONNECT_STATUS_DISCONNECTED);
		SendState2Menu(2);
		break;
	default:
		szRASState = _T("Unknown state.");
		break;
	}
	GetDlgItem(IDC_STATIC_RAS_STATE)->SetWindowText(szRASState);

	Invalidate(FALSE);
}

LRESULT CWirelessManagerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_RASDIALEVENT ) 
	{
		ParseRasDialEvent( wParam, lParam );
		return TRUE;
	}
	else if (UMSG_GSM_CMD == message)
	{
		if (GSM_ACK_DIAL_RESULT == wParam)
		{
			m_bDialing = (bool)lParam;
		}
	}
	//增加监听窗口
	else if(MSG_ADDLISTENWND == message)
	{
		//给增加的监听窗口发送当前状态信息
		ExtraStatus enExtraStatus = UN_KNOW;
		if(g_enConnectStatus == CONNECT_STATUS_DISCONNECTED)
			enExtraStatus = g_enExtraStatus;
		::PostMessage( (HWND)lParam, WM_CONNECT_STATUS, g_enConnectStatus, enExtraStatus );

		int i=0;
		//搜索已有的进程，如果存在，则重置该进程相应的窗口句柄
		while( 0 != m_stHWNDSet[i].dwProcID && i<32 )
		{
			if( wParam == m_stHWNDSet[i].dwProcID )
			{
				//更新成功
				m_stHWNDSet[i].hStatusWnd = (HWND)lParam;
				return 1;
			}
			i++;
		}
		if(i >= 32)
			return 0;
		//如果进程ID不存在，则新增一个
		m_stHWNDSet[i].dwProcID		= (DWORD)wParam;
		m_stHWNDSet[i].hStatusWnd	= (HWND)lParam;
		return 1;
	}
	//删除监听窗口
	else if(MSG_REMOVELISTENWND == message)
	{
		int i=0;
		//遍历已有的进程ID，若有相同，则删除
		while( 0 != m_stHWNDSet[i].dwProcID && i<32 )
		{
			if( wParam == m_stHWNDSet[i].dwProcID )
			{
				int j=i+1;
				//将i位置以后的元素往前移位
				while( 0 != m_stHWNDSet[j].dwProcID && j<32 )
				{
					m_stHWNDSet[j-1].dwProcID	= m_stHWNDSet[j].dwProcID;
					m_stHWNDSet[j-1].hStatusWnd	= m_stHWNDSet[j].hStatusWnd;
					j++;
				}
				//将最后的元素删除
				m_stHWNDSet[j-1].dwProcID	= 0;
				m_stHWNDSet[j-1].hStatusWnd	= 0;
				return 1;
			}
			i++;
		}
		//没有搜索到要求删除的进程ID
		return 0;
	}
	//获取当前网络状态
	else if(MSG_GETCONNECTSTATUS == message)
	{
		return g_enConnectStatus;
	}
	//GSM_AT.exe发送的消息
	else if(MSG_RAS_CONNECT == message)
	{
		//找到sim卡后请求ras拨号
		if(wParam == 8)
		{
			//模块上电后，找到sim就可以开始ras拨号
			CLogTool::Instance()->WriteLogFile(_T("recv message"));

			if(m_RasState != RASCS_Connected)
			{
				m_dwReDialCnt = 0;
				CLogTool::Instance()->WriteLogFile(_T("start enRAS_Dial"));
				RasControl(enRAS_Dial);	//连接ras
			}

			Invalidate(FALSE);
		}
	}
	//天泽进程请求，连接ras，或断开后重新连接ras
	else if(MSG_RAS_RECONNECT == message && 2 == wParam)
	{
		if(m_RasState == RASCS_Disconnected)
		{
			RasControl(enRAS_Dial);		//连接ras
		}
		else if(m_RasState == RASCS_Connected)
		{
			RasControl(enRAS_ReDial);	//重连
		}
	}

FN_END:
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CWirelessManagerDlg::OnClose() 
{
	if (m_hRasConn != NULL)
	{
		RasHangUp(m_hRasConn);
		m_hRasConn = NULL;
	}
	
	CDialog::OnClose();
}

void CWirelessManagerDlg::OnOK() 
{
	if (m_hRasConn != NULL)
	{
		RasHangUp(m_hRasConn);
		m_hRasConn = NULL;
	}
	
	CDialog::OnOK();
}

void CWirelessManagerDlg::OnTimer(UINT nIDEvent) 
{
	if(TID_FORCE_CHECK_GPRS == nIDEvent)
	{
		//KillTimer(nIDEvent);
	}
	else if(TID_CHECK_RAS_CONNECTION == nIDEvent)
	{
		//被请求连接RAS后，定时检查是否连接成功
		KillTimer(nIDEvent);
		if(g_enConnectStatus != CONNECT_STATUS_CONNECTED)
		{//如果20秒后仍未连接成功，则向GSM_AT.exe进程发送请求reset GSM模块
			//HWND hWnd = ::FindWindow(NULL, WND_NAME_GSM_AT);
			//if(NULL != hWnd)
			//	::PostMessage(hWnd, WM_GSM_RESET, 0, 0);
		}
	}
	else if(TID_DISCONNECT == nIDEvent)
	{
		KillTimer(nIDEvent);
		//OnBtnDial();
		RasControl(enRAS_ReDial);//enRAS_Hangup
	}
	else if(TID_DIAL == nIDEvent)
	{
		CLogTool::Instance()->WriteLogFile("Timer: TID_DIAL");

		KillTimer(nIDEvent);
		RasControl(enRAS_Dial);	//断开成功后继续拨号
	}

	CDialog::OnTimer(nIDEvent);
}

void CWirelessManagerDlg::PostMsg2WND(ConnectStatus enStatus)
{
	if(enStatus == g_enConnectStatus)
		return;
	g_enConnectStatus = enStatus;
	PostStatus();
}

void CWirelessManagerDlg::PostStatus()
{
	ExtraStatus enExtraStatus = UN_KNOW;
	//当前状态不为断开时，重置g_enExtraStatus
	if(g_enConnectStatus == CONNECT_STATUS_DISCONNECTED)
		enExtraStatus = g_enExtraStatus;

	int i=0;
	//遍历已有的进程ID，向每一个进程窗口发送消息
	while( 0 != m_stHWNDSet[i].dwProcID && i<32 )
	{
		if( 0 != m_stHWNDSet[i].hStatusWnd )
		{
			::PostMessage(
				m_stHWNDSet[i].hStatusWnd, 
				WM_CONNECT_STATUS, 
				g_enConnectStatus, 
				enExtraStatus);
			TRACE3("\nPostMessage: HWND-%d, Status-%d, ExtraStatus-%d\n", 
				m_stHWNDSet[i].hStatusWnd, g_enConnectStatus, g_enExtraStatus);
			Sleep(50);
		}
		i++;
	}
}


DWORD CWirelessManagerDlg::THDStartUp(LPVOID lpParameter)
{
	CWirelessManagerDlg* pDlg = (CWirelessManagerDlg*)lpParameter;

	Sleep(1000);

	HWND hwnd = ::FindWindow(NULL,WND_NAME_GSM_AT);
	if (NULL != hwnd)
	{
		::PostMessage(hwnd, WM_SIMCARD_REQUEST, 0, 0);
	}

	return 1;
}

void CWirelessManagerDlg::SendState2Menu(int nType)
{
	//HWND hWnd = ::FindWindow(NULL, WND_NAME_MENU);
	//if(hWnd != NULL)
	//{//网络状态: RAS拨号:  2 断开    3 连接
	//	::PostMessage( hWnd, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, nType );
	//}

	::PostMessage( HWND_BROADCAST, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, nType );

	//hWnd = ::FindWindow(NULL, GNSS_PROCESS_TEXT);
	//if(hWnd != NULL)
	//{//网络状态: RAS拨号:  2 断开    3 连接
	//	::PostMessage( hWnd, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, nType );
	//}
}

void CWirelessManagerDlg::OnBnClickedBtnPing()
{
	return;
}

void CWirelessManagerDlg::OnBtnStatus() 
{
}


