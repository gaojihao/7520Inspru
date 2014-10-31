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

#define WM_COMMAND_TIANZE			WM_USER+0x1000	//ͬ�������ͨѶ����Ϣ
#define WM_GSM_RESET				WM_USER+0x1001	//WirelessManager.exe -> GSM_AT.exe reset GSMģ��
#define WM_ACC_ON_OFF				(WM_USER+0x320)			//ACC֪ͨ��wParam��0  ACC����  1  ACC�ϵ�
#define WM_SIMCARD_REQUEST			(WM_USER+0x322)
#define MSG_SINGAL_VALUE			(WM_USER+0x2227)
//����״̬: ��¼������:	0 ����  1 ��ͨ  RAS����:  2 �Ͽ�    3 ����
#define WPARAM_SERVER_LOGIN			10	

#define VALUE_LOW_GSM_SIGNAL		10	//���ź�ֵ
#define LOW_SIGNAL_COUNT			3	//���ź�ֵ�������
#define WND_NAME_GSM_AT				_T("WND_GSM_AT")

//����״̬
ConnectStatus	g_enConnectStatus;
//��g_enConnectStatusΪCONNECT_STATUS_DISCONNECTEDʱ������Ͽ��ĸ���ԭ��
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
	//m_bGSMReset			= FALSE;		//True��gsmģ������reset��False��gsm��������
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


	GetDlgItem( IDC_BTN_DIAL)->SetWindowText( _T("����") );
	
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
	GetDlgItem(IDC_BTN_DIAL)->EnableWindow(FALSE);	//���ð�ť״̬
	if(RASCS_Disconnected == m_RasState)
	{
		GetDlgItem( IDC_BTN_DIAL)->SetWindowText( _T("�Ҷ�...") );
		RasControl(enRAS_Dial);
	}
	else
	{
		GetDlgItem( IDC_BTN_DIAL)->SetWindowText( _T("����") );
		RasControl(enRAS_Hangup);
	}
}

DWORD CWirelessManagerDlg::DialRasNetwork()
{
	
	//����֮ǰ���ж��ϴβ��ŵľ���Ƿ����ͷ�
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
	RASDIALPARAMS   RasDialParams;   //RAS�ṹ������������RAS�Ĳ���   
	//���濪ʼ��ʼ�����Ų���
	memset((char*)&RasDialParams, 0, sizeof(RasDialParams));
	RasDialParams.dwSize   =   sizeof(RASDIALPARAMS);   //�ṹ��С   
	wcscpy(RasDialParams.szEntryName, /*_T("cmnet")*/ENTRY_NAME);		//���������н����ĵ绰����
	
   	wcscpy(RasDialParams.szPhoneNumber , _T(""));	//*99***#
    wcscpy(RasDialParams.szCallbackNumber,_T(""));   //�ز����룬����ʱ��ΪNULL   
    wcscpy(RasDialParams.szUserName,_T(""));   //�û���   
    wcscpy(RasDialParams.szPassword,_T(""));   //�û�����
	wcscpy(RasDialParams.szDomain,_T(""));   //�û�Ȩ����֤��

	RASDIALPARAMS ras_para_test;
	memset(&ras_para_test, NULL, sizeof(RASDIALPARAMS));

	int nRasResult = RasDial(NULL, NULL, &RasDialParams, 0xFFFFFFFF, m_hWnd, &m_hRasConn);
	g_bIsDialing = FALSE;
	
	return nRasResult;
}

void CWirelessManagerDlg::RasControl(DIAL_STYPE enStype)
{
	m_enDialStype = enStype;	//���沦������
	switch(enStype)
	{
	case DIAL_TYPE_NONE:
		break;
	case enRAS_Dial://��ǰ����״̬Ϊ�Ͽ���
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
		//ǿ�ƶϿ�ras�ɹ�
		else if(m_enDialStype == enRAS_Hangup)
		{
			//�ص�������ʱ��
			KillTimer(TID_DISCONNECT);
			CLogTool::Instance()->WriteLogFile("TID_DIAL: receive disconnect: m_enDialStype == enRAS_Hangup");
		}
	}
	else if(RASCS_Connected == rasState)
	{
		m_dwReDialCnt = 0;	//���Ӻ󣬸�λ�ز�������
		CLogTool::Instance()->WriteLogFile("TID_DIAL: receive connected");
		GetDlgItem(IDC_BTN_DIAL)->EnableWindow(TRUE);	//���ð�ť״̬
	}
}

void CWirelessManagerDlg::UpdateRASState( RASCONNSTATE state )
{
	//���ó�ʱ��ʱ��
	KillTimer(TID_DISCONNECT);
	SetTimer(TID_DISCONNECT, 20*1000, NULL);	//���ò��ų�ʱ��ʱ��
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
		m_dwReDialCnt = 0;	//���Ӻ󣬸�λ�ز�������
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
	//���Ӽ�������
	else if(MSG_ADDLISTENWND == message)
	{
		//�����ӵļ������ڷ��͵�ǰ״̬��Ϣ
		ExtraStatus enExtraStatus = UN_KNOW;
		if(g_enConnectStatus == CONNECT_STATUS_DISCONNECTED)
			enExtraStatus = g_enExtraStatus;
		::PostMessage( (HWND)lParam, WM_CONNECT_STATUS, g_enConnectStatus, enExtraStatus );

		int i=0;
		//�������еĽ��̣�������ڣ������øý�����Ӧ�Ĵ��ھ��
		while( 0 != m_stHWNDSet[i].dwProcID && i<32 )
		{
			if( wParam == m_stHWNDSet[i].dwProcID )
			{
				//���³ɹ�
				m_stHWNDSet[i].hStatusWnd = (HWND)lParam;
				return 1;
			}
			i++;
		}
		if(i >= 32)
			return 0;
		//�������ID�����ڣ�������һ��
		m_stHWNDSet[i].dwProcID		= (DWORD)wParam;
		m_stHWNDSet[i].hStatusWnd	= (HWND)lParam;
		return 1;
	}
	//ɾ����������
	else if(MSG_REMOVELISTENWND == message)
	{
		int i=0;
		//�������еĽ���ID��������ͬ����ɾ��
		while( 0 != m_stHWNDSet[i].dwProcID && i<32 )
		{
			if( wParam == m_stHWNDSet[i].dwProcID )
			{
				int j=i+1;
				//��iλ���Ժ��Ԫ����ǰ��λ
				while( 0 != m_stHWNDSet[j].dwProcID && j<32 )
				{
					m_stHWNDSet[j-1].dwProcID	= m_stHWNDSet[j].dwProcID;
					m_stHWNDSet[j-1].hStatusWnd	= m_stHWNDSet[j].hStatusWnd;
					j++;
				}
				//������Ԫ��ɾ��
				m_stHWNDSet[j-1].dwProcID	= 0;
				m_stHWNDSet[j-1].hStatusWnd	= 0;
				return 1;
			}
			i++;
		}
		//û��������Ҫ��ɾ���Ľ���ID
		return 0;
	}
	//��ȡ��ǰ����״̬
	else if(MSG_GETCONNECTSTATUS == message)
	{
		return g_enConnectStatus;
	}
	//GSM_AT.exe���͵���Ϣ
	else if(MSG_RAS_CONNECT == message)
	{
		//�ҵ�sim��������ras����
		if(wParam == 8)
		{
			//ģ���ϵ���ҵ�sim�Ϳ��Կ�ʼras����
			CLogTool::Instance()->WriteLogFile(_T("recv message"));

			if(m_RasState != RASCS_Connected)
			{
				m_dwReDialCnt = 0;
				CLogTool::Instance()->WriteLogFile(_T("start enRAS_Dial"));
				RasControl(enRAS_Dial);	//����ras
			}

			Invalidate(FALSE);
		}
	}
	//���������������ras����Ͽ�����������ras
	else if(MSG_RAS_RECONNECT == message && 2 == wParam)
	{
		if(m_RasState == RASCS_Disconnected)
		{
			RasControl(enRAS_Dial);		//����ras
		}
		else if(m_RasState == RASCS_Connected)
		{
			RasControl(enRAS_ReDial);	//����
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
		//����������RAS�󣬶�ʱ����Ƿ����ӳɹ�
		KillTimer(nIDEvent);
		if(g_enConnectStatus != CONNECT_STATUS_CONNECTED)
		{//���20�����δ���ӳɹ�������GSM_AT.exe���̷�������reset GSMģ��
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
		RasControl(enRAS_Dial);	//�Ͽ��ɹ����������
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
	//��ǰ״̬��Ϊ�Ͽ�ʱ������g_enExtraStatus
	if(g_enConnectStatus == CONNECT_STATUS_DISCONNECTED)
		enExtraStatus = g_enExtraStatus;

	int i=0;
	//�������еĽ���ID����ÿһ�����̴��ڷ�����Ϣ
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
	//{//����״̬: RAS����:  2 �Ͽ�    3 ����
	//	::PostMessage( hWnd, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, nType );
	//}

	::PostMessage( HWND_BROADCAST, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, nType );

	//hWnd = ::FindWindow(NULL, GNSS_PROCESS_TEXT);
	//if(hWnd != NULL)
	//{//����״̬: RAS����:  2 �Ͽ�    3 ����
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


