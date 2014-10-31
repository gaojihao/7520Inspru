// GNSSTerminalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GNSSTerminal.h"
#include "GNSSTerminalDlg.h"
#include "./DataParser/DataPack.h"
#include "./Tools/LogTool.h"
#include "./Lib/IniManager.h"
#include "./MsgProc/MsgProc.h"
#include "./GPSData/GPSData.h"
#include "./SocketManager/SocketManager.h"
#include "./DataParser/ComManager.h"
#include "./TerminalConfig/TerminalConfig.h"
#include "DataParser/DataParser.h"
#include "Lib/unzip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGNSSTerminalDlg dialog
#define	GNSS_PROCESS_TEXT		_T("WND_GNSSTERMINAL")
extern void TTSSpeaking(CString csText);


CGNSSTerminalDlg::CGNSSTerminalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGNSSTerminalDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitSocket = FALSE;
}

CGNSSTerminalDlg::~CGNSSTerminalDlg()
{
//	ColseCom();
}

void CGNSSTerminalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGNSSTerminalDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CGNSSTerminalDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGNSSTerminalDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGNSSTerminalDlg message handlers

BOOL CGNSSTerminalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

#ifdef DEBUG
	MoveWindow(0,0,480,272);
#else
	MoveWindow(0,0,0,0);
#endif

	CTerminalConfig::Instance()->LoadConfigFile( PATH_CONFIG_INI );



	if (0xFFFFFFFF != GetFileAttributes(PATH_GNSS_BAK))
	{
		::DeleteFile(PATH_GNSS_BAK);
	}
	if (0xFFFFFFFF != GetFileAttributes(PATH_UPGRADE_EXE))
	{
		::DeleteFile(PATH_UPGRADE_EXE);
	}
	if (0xFFFFFFFF != GetFileAttributes(PATH_UPGRADE_ZIP))
	{
		::DeleteFile(PATH_UPGRADE_ZIP);
	}
	
	SetWindowText(GNSS_PROCESS_TEXT);


	CMsgProc::Instance()->SetMainWnd(GetSafeHwnd(), this);


	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCKProcess, this, 0, NULL);

	
	CDataPack::Instance()->m_hMutex = CreateMutex(NULL, FALSE, NULL);

	CMsgProc::Instance()->BeginHeartBeat();

//	readroutefile();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

DWORD CGNSSTerminalDlg::StartCKProcess(LPVOID lpParameter)
{
	CGNSSTerminalDlg* pDlg = (CGNSSTerminalDlg*)lpParameter;
	Sleep(2000);
	CComManager::Instance()->InitCom();

	return 0;
}



LRESULT CGNSSTerminalDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_COMMAND_TIANZE == message)
	{
		if (WPARAM_SERVER_LOGIN == wParam)
		{
			if (2 == lParam && m_bInitSocket) //断开
			{
				CMsgProc::Instance()->SetLoginState(FALSE);
				CMsgProc::Instance()->DisConnectSocket();
				m_bInitSocket = FALSE;
			}
			else if (3 == lParam)//连接
			{
				if ( !m_bInitSocket)
				{
					m_bInitSocket = TRUE;
					
					for (int i = 0; i < 5; i++)
					{
						Sleep(1000);
						if (CMsgProc::Instance()->ConnectSocket())
						{
							return 1;
						}
						Sleep(1000);
					}
					::PostMessage(HWND_BROADCAST, MSG_RAS_RECONNECT, 2, 0); //重连ras

				}
			}
		}
		else if (MSG_TMN_TEXT_SMS == wParam)
		{
			CDataPack::Instance()->PackTextSMS(lParam);
		}
	}
	else if (HW_ACCDET == message)
	{
		OnsysPowerChange(wParam);
		CDataPack::Instance()->OutsideEvmDetect((BYTE)wParam);
	}
	else if (HW_PowerOff == message)
	{
		CDataPack::Instance()->SystemPowerOff(wParam);
	}
	else if (WM_SERVER == message && 10054 == wParam)
	{
		CMsgProc::Instance()->ReConnectServer();
	}
	else if ( message == WM_COPYDATA )
	{
		OnCmdCOPYDATA( wParam, lParam );
	}
	else if (WM_SERVER == message && MSG_UPGRADE_WPARAM == wParam)
	{
		if (lParam == 1)//下载升级文件成功
		{
			ReplaceUpgradeFile();
		}
		else //下载升级文件失败
		{
			KillTimer(1005);
			SetTimer(1005, 3000, NULL);
		}
	}
	else
	{
		CMsgProc::Instance()->MsgProc(message, wParam, lParam);

	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CGNSSTerminalDlg::ReplaceUpgradeFile()
{
	HZIP hZip = OpenZip(PATH_UPGRADE_ZIP,NULL);

	if (0 == hZip)
	{
		DeleteFile(PATH_UPGRADE_ZIP);
		CDataPack::Instance()->Pack_Upgrade_ACK(0x01);
		return FALSE;
	}

	SetUnzipBaseDir(hZip, _T("\\Flashdrv Storage"));

	ZIPENTRY ze; 
	ZeroMemory(&ze, sizeof(ZIPENTRY));
	ZRESULT zRet = GetZipItem(hZip, -1, &ze); 
	int numitems = ze.index;
	for (int i=0; i<numitems; i++)
	{ 
		ZRESULT zRet = GetZipItem(hZip, i, &ze);
		ZRESULT zReturn = UnzipItem(hZip, i, ze.name);
		//解压失败，删除已解压的文件
		if (ZR_OK != zReturn)
		{
			DeleteFile(PATH_UPGRADE_EXE);
			CloseZip(hZip);
			CDataPack::Instance()->Pack_Upgrade_ACK(0x01);
			return FALSE;
		}
	}
	CloseZip(hZip);

	CDataPack::Instance()->Pack_Upgrade_ACK(0x00);
	//重命名源文件
	CFile::Rename(PATH_GNSS, PATH_GNSS_BAK);
	

	BOOL bOK = CopyFile(PATH_UPGRADE_EXE, PATH_GNSS, FALSE);

	if (bOK)
	{
		DeleteFile(PATH_UPGRADE_EXE);
		Sleep(500);
		DeleteFile(PATH_UPGRADE_ZIP);
	}

	return TRUE;

	
}

void CGNSSTerminalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (1005 == nIDEvent)
	{
		KillTimer(nIDEvent);
		CDataParser::Instance()->FtpConnect();
	}

	CDialog::OnTimer(nIDEvent);
}

void CGNSSTerminalDlg::OnCmdCOPYDATA(WPARAM wParam, LPARAM lParam )
{
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)(lParam);
	if ( pcds->dwData == GSM_DCOPY_ACK_MSG )
	{
		if ( pcds->cbData == sizeof(SM_PARAM) )
		{
			SM_PARAM  *p_sm = (SM_PARAM*)pcds->lpData;

			WORD		wFieldLen = 0;
			CIniFile	IniFile;
			CHAR		szTemp[512];
			CString		strTemp;
			memset(szTemp, NULL, sizeof(szTemp));
			//加载文件失败
			IniFile.Load( PATH_CONFIG_INI );

			PCSTR pcField	= ScanField((PCSTR)p_sm->TP_UD, 1, wFieldLen, ':');

			pcField	= ScanField(pcField, 0, wFieldLen, ';');
			if (pcField == NULL)
			{
				return;
			}

			strTemp = _T("");
			IniFile.SetString(_T("Set"), _T("AuthenCode"), strTemp);
			

			if (wFieldLen > 6)
			{
				memset(szTemp, NULL, sizeof(szTemp));
				strncpy(szTemp, pcField, wFieldLen);
				strTemp	= szTemp;
				IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_IP"), strTemp);
			}

			pcField	= ScanField(pcField, 1, wFieldLen, ';');
			if (pcField == NULL)
			{
				return;
			}

			if (wFieldLen > 2)
			{
				memset(szTemp, NULL, sizeof(szTemp));
				strncpy(szTemp, pcField, wFieldLen);
				WORD wport	= atoi(szTemp);
				IniFile.SetInt(_T("TerminalParam"), _T("SVR_TCP_PORT"), wport);
			}


			pcField	= ScanField(pcField, 1, wFieldLen, ';');
			if (pcField == NULL)
			{
				return;
			}
			

			pcField	= ScanField(pcField, 1, wFieldLen, ';');
			if (pcField == NULL)
			{
				return;
			}
			if (wFieldLen > 1)
			{
				memset(szTemp, NULL, sizeof(szTemp));
				strncpy(szTemp, pcField, wFieldLen);
				strTemp	= szTemp;
				IniFile.SetString(_T("Set"), _T("PhoneNum"), strTemp);
			}
			
			Sleep(500);

			IniFile.Flush();
//			IniFile.Release();

			Sleep(500);

			CTerminalConfig::Instance()->LoadConfigFile( PATH_CONFIG_INI );
			Sleep(200);
			CMsgProc::Instance()->ReConnectServer();

		}
	}
}


PCSTR CGNSSTerminalDlg::ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator)
{
	PCSTR pcFieldBegin = pcBuffer;

	while( nFieldNO-- > 0 )
	{
		while ( pcFieldBegin && *pcFieldBegin++ != cSeparator && *pcFieldBegin != NULL )
			;
	}
	//搜索的节为空
	if(NULL == *pcFieldBegin)
	{
		wLen = 0;
		return NULL;
	}

	//节的最后一个字符指针的后一位
	PCSTR pcFieldEnd = pcFieldBegin;
	while( pcFieldEnd && *pcFieldEnd != cSeparator && *pcFieldEnd != NULL )
	{
		pcFieldEnd++;
	}

	//计算节的长度
	wLen = pcFieldEnd - pcFieldBegin;

	return pcFieldBegin;
}
void CGNSSTerminalDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMsgProc::Instance()->ConnectSocket();
}

void CGNSSTerminalDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMsgProc::Instance()->DisConnectSocket();
}

void CGNSSTerminalDlg::readroutefile()
{
	if (::GetFileAttributes(PATH_ROUTE1_TXT) == 0xFFFFFFFF)
	{
		return;
	}
//保存路径数据
	CFile file;
	file.Open(PATH_ROUTE1_TXT, CFile::modeRead);

	DWORD dwlen = file.GetLength();

	BYTE *buffer = new BYTE[dwlen+1];

	WORD ulen = file.Read(buffer,dwlen);


	

	CLogTool::Instance()->WriteLogFile(_T("route date:"),buffer,ulen);
//	CLogTool::Instance()->WriteLogFile(_T("Save BlindData:"), cMsgBody, wOffset);

	file.Close();
	delete[] buffer;


}

void CGNSSTerminalDlg::OnsysPowerChange(WPARAM wparam)
{
	switch(wparam)//00:断电 01:电压正常
	{
	case 0x00:
		{
			if (g_alarm_state.unAlarm.alarm_flag.cNoExternalPower == 0)
			{
				CreateThread(NULL, 0, THDStartTTS, this, 0, NULL);
			}
		}
		break;
	case 0x01:
		break;
	default:
		break;
	}
}

DWORD CGNSSTerminalDlg::THDStartTTS(LPVOID lpvoid)
{
	CGNSSTerminalDlg* pDlg = (CGNSSTerminalDlg*)lpvoid;

	for (int i = 0; i < 5; i++)
	{
		if (g_alarm_state.unAlarm.alarm_flag.cNoExternalPower == 0)
		{
			return 0;
		}
		TTSSpeaking(_T("主电源断开，请打开备用电源"));
		Sleep(8*1000);
	}
	return 0;
}
