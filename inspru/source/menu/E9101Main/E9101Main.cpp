// E9101Main.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "E9101Main.h"
#include "E9101MainDlg.h"
#include "SysConfig.h"

#include "ExButton.h"
#include "HBCP.h"
#include "../DriveLib/tts_api.h"
#include "../DriveLib/IOControl.h"

#include "Winbase.h"

#include "../GSM/GSMLogic.h"

#include "DlgSyesetKeyVol.h"
#include "BrightnessSetDlg.h"

#include "Dialog/DlgVehicleInfo.h"
#include "Dialog/DlgDriveEconomySetting.h"

#include "../SQLite3/CppSQLite3.h"
#include "../Msg/MsgQueue.h"


#pragma comment(lib,"../DriveLib/IOControl")

#define UNIQE_NAME _T("{menu-1.01}")

#define BATTERY_FLAG_COMPLETE 0x10

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BOOL g_bLight;
extern HWND hMainWnd;
extern BOOL  g_bAppStatus;
extern int  g_nAmplifierVol;
extern BOOL g_bCamera;

CRect g_retTitile;

extern unsigned char g_power_status;

UINT CE9101MainApp::s_SpareTimer = 0;


BEGIN_MESSAGE_MAP(CE9101MainApp, CWinApp)
	//{{AFX_MSG_MAP(CE9101MainApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

#define	PATH_TTS_DATA		"\\Flashdrv Storage\\Menu\\stubsvr.dat"

CGSMLogic		g_GsmLogic;
const POS_INFO*	m_pPosInfo;
//VEHICLE_INFO	g_stVehicle;					//��can���߻�ȡ��������Ϣ
HANDLE			g_hTTS;							//tts���
BOOL			g_bLight = FALSE;				//�Ƿ��Զ��رձ���
DWORD			g_dwSpareTime = 30*1000;		//�Զ��ر���ʱ��
PROCESS_INFORMATION		g_sProcessInfo;			//����������Ϣ



//����Ӧ�ó���ļ�������,�˳�ʱ�ָ�����
DWORD CE9101MainApp::THDAppExit(LPVOID lpParameter)
{
	//�����������̲��رձ���
	//g_bLight = FALSE;	
	
	DWORD  dwSig = WaitForSingleObject(g_sProcessInfo.hProcess, INFINITE );
	switch( dwSig )
	{
	case WAIT_OBJECT_0:
		{
			HWND hwd = ::FindWindow( _T("HHTaskBar"), NULL );
			if( hwd != NULL )
				::ShowWindow( hwd, SW_HIDE );

			g_bAppStatus = FALSE;
			g_bCamera = FALSE;
		}
		break;
	case WAIT_TIMEOUT:
		g_bCamera = FALSE;
		break;
	default:
		break;
	}
	return 1;
}

void TTSSpeaking(CString csText)
{
	if(g_hTTS == NULL)
		return;
	
	const unsigned short* pszText = (unsigned short*)csText.GetBuffer(csText.GetLength());
	if ( tts_is_speaking(g_hTTS) )
	{
		tts_cancle(g_hTTS);
	}
	tts_speaking(g_hTTS, pszText, csText.GetLength()+1 );
}

CE9101MainApp::CE9101MainApp()
	: CWinApp(),
	m_ExitEvent(FALSE,TRUE,NULL,NULL)
{
	m_pPowerThread = NULL;
}

CE9101MainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CE9101MainApp initialization
// �õ�Ӧ�ó���·��
CString GetExePath()
{
	TCHAR tchDir = '\\';
	CString strTemp, strIniPathName;
	GetModuleFileName(NULL, strTemp.GetBuffer(MAX_PATH), MAX_PATH);
	strTemp.ReleaseBuffer();
                               
	int PathPos = strTemp.ReverseFind(tchDir) + 1;

	strIniPathName = strTemp.Left(PathPos);

	return strIniPathName;
}

CString GetExeIniName()
{
	CString strIniName;
	TCHAR tchDir = '\\';
	CString strTemp;
	GetModuleFileName(NULL, strTemp.GetBuffer(MAX_PATH), MAX_PATH);
	strTemp.ReleaseBuffer();

	int PathPos = strTemp.ReverseFind(tchDir) + 1;
	strTemp.Delete(0,PathPos);
	
	strIniName = strTemp;
	TCHAR tchDot = '.';
	int DotPos = strTemp.Find(tchDot);
	strIniName  = strTemp.Left(DotPos);
	strIniName += _T(".ini");

	return strIniName;
}

//////////////////////////////////////////////////////////////////////////

BOOL CE9101MainApp::InitInstance()
{
	m_hOneInstance=CreateMutex(NULL,FALSE,UNIQE_NAME);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}

	//��gpsת��
	Open(4,9600, MODE_SGL_GPS);

	EnableConvertGPS(9);

	
	CString strExePath = GetExePath();
	CString strExeIniPath = GetExeIniName();

	CSysConfig::Instance().SetConfigFilePath(strExePath);
	CSysConfig::Instance().SetConfigFileName(strExeIniPath);
	if(!CSysConfig::Instance().Load())
	{
 		CSysConfig::Instance().ResetDefault();
	}

	CBrightnessSetDlg::SetbkliteTimeOuts();


	//����
	CDlgSyesetKeyVol::m_nVolLev = CSysConfig::Instance().GetSysVol();
	if ( CDlgSyesetKeyVol::m_nVolLev <= 0)	//��Χ���
		CDlgSyesetKeyVol::m_nVolLev = 0;
	else if ( CDlgSyesetKeyVol::m_nVolLev >= 10 ) 
		CDlgSyesetKeyVol::m_nVolLev = 10;
	DWORD dwVol = 0xFFFF * CDlgSyesetKeyVol::m_nVolLev / 10;	//ת��Ϊ������ֵ
    dwVol = MAKELONG( dwVol, dwVol );
	::waveOutSetVolume(0, dwVol );

	//����
	CBrightnessSetDlg::m_nBrightness = (int)(CSysConfig::Instance().GetBrightness());
	if(CBrightnessSetDlg::m_nBrightness < 1)			
		CBrightnessSetDlg::m_nBrightness = 1;
	else if(CBrightnessSetDlg::m_nBrightness > 7)	
		CBrightnessSetDlg::m_nBrightness = 7;
	IO_SetLcdBackLight(CBrightnessSetDlg::m_nBrightness);

	//��ʼ������λ��
	g_retTitile.left	= 0;
	g_retTitile.top		= 0;
	g_retTitile.right	= 800;
	g_retTitile.bottom	= 62;

	//��ʼ������
	CE9101MainDlg::OnInitConfig();
	g_hTTS = tts_open();
	tts_init(g_hTTS, PATH_TTS_DATA);
	tts_man_role(g_hTTS, false);


	//���˵�
	CE9101MainDlg dlg2;
	theApp.m_pMainWnd = &dlg2;
	dlg2.DoModal();
	tts_close(g_hTTS);

	return FALSE;
}

int CE9101MainApp::ExitInstance() 
{
	///<���˳��ź�
	m_ExitEvent.SetEvent();
	Sleep(200);

	Close();

	///<�ȴ���Դ����߳��˳�
	if (NULL != m_pPowerThread)
	{
		while(::WaitForSingleObject(m_pPowerThread->m_hThread,0)!=WAIT_OBJECT_0)
		{
			Sleep(200);
		}
		delete m_pPowerThread;
		m_pPowerThread = NULL;
	}

	return CWinApp::ExitInstance();
}

BOOL CE9101MainApp::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
 	case WM_LCD_CLOSE:
		{
 			break;
		}
	case WM_KEYDOWN:
		{
			if(pMsg->lParam>>16 == LC_KEY_PWR_SHORT)
			{
			}
			if(pMsg->lParam>>16 == LC_KEY_PWR_LONG)
			{
			}

			break;
		}
	case WM_LBUTTONDOWN:
		{
			break;
		}
	case WM_POWER_OFF:
		{
			break;
		}
	case WM_CHILD_EXIT:
		{
		}

	default:
		break;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}


void PlayKeySound(  )
{
	if( g_enKeySnd == KeySnd_none )
		return;
	else if ( g_enKeySnd == KeySnd_1 )
	{
		::PlaySound(SOUND_KEYCLICK, NULL, SND_ASYNC | SND_NODEFAULT );
	}
	else if ( g_enKeySnd == KeySnd_2 )
	{
		::PlaySound(SOUND_KEYCLICK1, NULL, SND_ASYNC | SND_NODEFAULT );
	}
	else if ( g_enKeySnd == KeySnd_3 )
	{
		::PlaySound(SOUND_KEYCLICK2, NULL, SND_ASYNC | SND_NODEFAULT );
	}
	else if ( g_enKeySnd == KeySnd_4 )
	{
		::PlaySound(SOUND_KEYCLICK3, NULL, SND_ASYNC | SND_NODEFAULT );
	}
	
}

