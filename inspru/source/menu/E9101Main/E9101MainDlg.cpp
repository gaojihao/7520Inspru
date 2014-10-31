// E9101MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "E9101Main.h"
#include "E9101MainDlg.h"
#include "CIniFile.h"

#include "SysSetDlg.h"
#include "SysConfig.h"

#include "SMSMain.h"
#include "GSMDial.h"
#include "SMSInbox.h"

#include "DlgDiaIn.h"
#include "PhoneBookMng.h"

#include "GSMCalling.h"
#include "GSMDialMain.h"

#include "DataType.h"
#include "PWDInputDlg.h"

#include "../E9101Main/Dialog/DlgTextInfo.h"
#include "../Dialog/DlgReportState.h"
#include "Dialog/DlgPushToTalk.h"
#include "Dialog/DlgVehicleInfo.h"
#include "Dialog/DlgICCard.h"
#include "Dialog/DlgMediaMain.h"
#include "Dialog/DlgDiaodu.h"
#include "Dialog/DlgDriveInfo.h"
#include "Dialog/DlgRFIDCheck.h"

#include "Dialog/DlgDriveEconomySetting.h"
#include "Dialog/DlgIMEI.h"
#include "Dialog/DlgPhone.h"
#include "Dialog/DlgTextInfoList.h"

#include "DlgSyesetKeyVol.h"
#include "BrightnessSetDlg.h"

#include "../../SQLite3/CppSQLite3.h"

#include "DlgConfirm2.h"

#include "../SCom/scom_rfid.h"
#include "../Msg/MsgQueue.h"
#include "../Msg/MsgProc.h"
//#include "../DriveLib/IOControl.h"


extern void TTSSpeaking(CString csText);
//拍照进程窗口名称
#define WNDTITLE_CAMERA_VIDEO				_T("CAMERA_VIDEO_20110411")

//menu进程窗口名称
#define WND_NAME_MENU						_T("WND_MAIN_MENU")

#define VK_RI_P 0xE1
#define VK_RI_S 0xE8

#define  TID_HANGUP_GSM   3013
#define  TID_COUNT_SEC    3014
#define IOCTRL_SYSTEM_RESET		        (3038)
#define MSG_OPEN_GUIDER4U			(WM_USER+0x378)//打开隐藏的导航软件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HWND hMainWnd = NULL;
static BOOL g_AV_IN = FALSE;
UINT CE9101MainDlg::S_5Minute = 0;
int CE9101MainDlg::m_nPic = 0;
BOOL CE9101MainDlg::m_bPhone = FALSE;	//默认不需要电话短信功能
BOOL g_bCamera = FALSE;

//menu.exe同GNSSTerminal.exe进程之间交互的消息ID
UINT	g_nWM_MENU_GNSSTERMINAL	= RegisterWindowMessage(_T("MENU_GNSSTERMINAL"));
UINT	g_nWM_CLOSE_COMMAND	= RegisterWindowMessage(_T("CLOSE_COMMAND"));

HWND m_MainWnd;

CString g_csBlueToothName;
CString g_csBlueToothIconName;

CString g_csPicName;
CString g_csPicIconName;

CString g_csMusicName;
CString g_csMusicIconName;

CString g_csMovieName;
CString g_csMovieIconName;

CString g_csNaviName;
CString g_csNaviIconName;

CString g_csVolumeName;
CString g_csVolumeIconName;

CString g_csTxtName;
CString g_csTxtIconName;

BOOL g_bFmInit ;

extern HWND g_hDialIn;
//extern HWND g_hDialOut;

enKeySound g_enKeySnd;

extern BOOL g_bLight;
// add by 陈国乐 2007-07-24
#define BLUETOOT_DIAL_IN_MSG  _T("{E286F1E5-CB1D-42d9-B000-6DDCC36D5A07}-BLUETOOTH_DIAL_IN")
UINT g_nDialIn  = RegisterWindowMessage(BLUETOOT_DIAL_IN_MSG);

// add by 陈国乐 2007-08-06
#define POWER_LOW_TO_MP4JP_MSG  _T("{6A45E7B5-98EB-44b7-BEC9-68AECAF32E74}-POWER_LOW_TO_MP4")
UINT g_nPowerLow = RegisterWindowMessage(POWER_LOW_TO_MP4JP_MSG);
// add by 陈国乐2007-08-14
#define HIDE_MAINWND          _T("{D8128951-0C88-456c-AB9F-629BD3C202E4}-HIDE_MAINWND")
UINT g_nHideMainWnd = RegisterWindowMessage(HIDE_MAINWND);

extern unsigned char g_power_status;
UINT g_PhoneDialOut = 1;//0:init 1:phone call out 2:gps call out
BOOL CE9101MainDlg::g_Bluetooth_state = FALSE;

BOOL  g_bAppStatus = FALSE;

wchar_t g_chVol[4];
int	g_nAmplifierVol = 0;


BOOL CE9101MainDlg::m_bACC_ON = FALSE;	//启动时默认ACC关
BOOL CE9101MainDlg::m_bCanStartNavi	= FALSE;

//extern BOOL g_PowerOn;
//#define CHANGEMODE 100
#define TID_FOR_START_INIT					0x1099
#define TID_CLOSE_MSGBOX					0x1100
#define	TID_START_NAVI_DELAY				0x1102


void OnKeyNavi(CString strPosInfo)
{
#define MSG_WND_NAME L"Guider_MsgWnd"	// 消息接收窗口名称
	HWND hwnd = ::FindWindow(NULL, MSG_WND_NAME);
	if (NULL == hwnd)
	{
		TCHAR bCmdline[100];
		LPCWSTR srCmdLine = bCmdline;

		BOOL bProcessState = CreateProcess( CSysConfig::Instance().GetNaviPath(), NULL, NULL, NULL, 
			FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);	
		if(bProcessState)
			CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);

		for(int i = 0; i < 1 * 60; i++ )
		{
			Sleep(1000);
			hwnd = ::FindWindow(NULL, MSG_WND_NAME);
			if ( hwnd ) 
				break;
		}
	}

	if (hwnd)
	{
		wchar_t buf[256] = {0};
		wsprintf(buf, L"SET_DESTINATION,%s",  strPosInfo );
		COPYDATASTRUCT cpd;
		cpd.dwData = 0;
		cpd.cbData = wcslen(buf)*sizeof(wchar_t) + 2;
		cpd.lpData = (PVOID)buf;

		::SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cpd);
	}
}

CE9101MainDlg::CE9101MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CE9101MainDlg::IDD, pParent)
{
	m_PowerState = BATTERY_FAILED;
	m_nPic = 0;
	PowerTip = FALSE;
	m_nTimerCount = 0;
	m_bEnableInput = FALSE;

	//网络状态: 登录服务器:	0 掉线  1 接通  RAS拨号:  2 断开    3 连接
	m_nRasState		= 2;
	m_nLoginServer	= 0;
}

void CE9101MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CE9101MainDlg)
	DDX_Control(pDX, IDC_STC_BLUETOOTH, m_stcBlueTooth);
	DDX_Control(pDX, IDC_STC_BATTERY, m_stcBattery);
	DDX_Control(pDX, IDC_STA_TIME, m_stcTime);
	DDX_Control(pDX, IDC_STATIC_SETTING, m_stSet);
	DDX_Control(pDX, IDC_STATIC_ADJUST, m_stAdjust);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CE9101MainDlg, CDialogBase)
	//{{AFX_MSG_MAP(CE9101MainDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TIP, OnTip)
	ON_MESSAGE(WM_CHILD_EXIT, OnChildExit)
	ON_MESSAGE(WM_POWER_EVENT, OnPowerEvent)
	ON_MESSAGE(WM_STORAGE_EVENT, OnSdCardMessage)
	ON_MESSAGE(WM_HBCPSTATE, OnBlueToothStateTrans)
//	ON_MESSAGE(WM_HBCPCALLIN, OnBlueToothDialIn)
	ON_MESSAGE(WM_HBCPCALLER_NUMBER, OnBlueToothCallNumber)
	ON_MESSAGE(WM_POWER_LOW_DLG_DESTROY, OnPowerLowDlgDestroy)
	ON_REGISTERED_MESSAGE(g_nHideMainWnd, OnHideWindow)
	ON_MESSAGE(WM_HBCPOUTGOINGCALL, OnOutGoingCall)
	ON_MESSAGE(WM_HBCPOFFHOOK, OnOffHookCall)
	ON_MESSAGE(WM_HBCPCONNECTED, OnBlueToothConnected)
	ON_MESSAGE(WM_HBCPRINGIND, OnRingInd)
	ON_MESSAGE(WM_HBCPACTIVECALL, OnActiveCall)
//	ON_MESSAGE(WM_HBCPPAIRLISTCOMPLETE, OnPairComplete)
END_MESSAGE_MAP()

BOOL CE9101MainDlg::OnInitDialog()
{
	m_nTimerCount = 0;
	CDialogBase::OnInitDialog();
	m_bEnableInput = FALSE;

	//将主窗口句柄和类对象指针设置到消息处理类
	CMsgProc::Instance()->SetMainWnd(GetSafeHwnd(), this);

	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	CenterWindow(GetDesktopWindow());	// center to the hpc screen
	SetWindowText(WND_NAME_MENU);

	bVol = FALSE;
	g_Bluetooth_state = FALSE;

	hMainWnd = GetSafeHwnd();
	
	bTimeOver = TRUE;
	SetTimer(TIMER_CURTIME, 1000, NULL);//刷新当前时间
	
	m_MainWnd = AfxGetMainWnd()->m_hWnd;
	
	m_ItemFm.nState = BTN_STATE_NORMAL;
	m_ItemGSM.nState = BTN_STATE_NORMAL;

	InitGdi();
	InitControl();
	//变皮肤
	OnSkinChange();
	//变换语言
	OnLanguageChange();



	SetTimer( TID_FOR_START_INIT,50, NULL );
	//开机15秒内不能启动导航软件,解决启动虚拟串口工具初始化时间延迟的bug
	SetTimer( TID_START_NAVI_DELAY, 15*1000, NULL);

	m_pPosInfo = GetSharePosInfo();
	//CIniFile	IniFile;
	////加载文件失败
	//if( !IniFile.Load( PATH_CONFIG_INI ) )
	//{
	//	AfxMessageBox(_T("加载config.ini文件失败！"));
	//	return FALSE;
	//}

	//
	//CString strAuthenCode = _T("");

	//IniFile.GetString(_T("Set"), _T("AuthenCode"), strAuthenCode);

	//if (strAuthenCode.GetLength() == 0)
	//{	
	//	//THDOpenRegDialog(this);
	//}
	//
	return TRUE;
}

void  CE9101MainDlg::LoadGSMNetType()
{
	CResString str;
	int nType = g_GsmLogic.GetGSMNetType();
	if ( nType == 0 )
		str.LoadString( RES_GSM_TYPE_SEARCHING );
	else if ( nType == 1 )
		str.LoadString( RES_GSM_TYPE_CMCC );
	else if ( nType == 2 )
		str.LoadString( RES_GSM_TYPE_CUC );
	else if ( nType == 3 )
		str = _T("中国联通3G");
	
	m_strNetTip = str;
}

void CE9101MainDlg::OnInitConfig()
{
	//读取子进程路径
	ChileProcessInfo ProcessInfo;
	g_enKeySnd = CSysConfig::Instance().GetKeySoundType();

	if (!CSysConfig::Instance().GetNaviInfo(ProcessInfo))
	{
	}
	else
	{
		g_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	if (!CSysConfig::Instance().GetMoviesInfo(ProcessInfo))
	{
		//GetDlgItem(IDC_BTN_MOVIE)->EnableWindow(FALSE);
	}
	else
	{
		g_csMovieName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csMovieIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if (!CSysConfig::Instance().GetMusicInfo(ProcessInfo))
	{
		//GetDlgItem(IDC_BTN_MUSIC)->EnableWindow(FALSE);
	}
	else
	{
		g_csMusicName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csMusicIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if (!CSysConfig::Instance().GetBlueToothInfo(ProcessInfo))
	{
	}
	else
	{
		g_csBlueToothName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csBlueToothIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if (!CSysConfig::Instance().GetPictureInfo(ProcessInfo))
	{
		
	}
	else
	{
		g_csPicName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csPicIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if (CSysConfig::Instance().GetVolumeInfo(ProcessInfo))
	{
		g_csVolumeName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csVolumeIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	
	if (!CSysConfig::Instance().GetTxtInfo(ProcessInfo))
	{
		//GetDlgItem(IDC_BTN_EBOOK_WORDPAD)->EnableWindow(FALSE);
	}
	else
	{
		g_csTxtName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csTxtIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}

	CDlgDriveEconomySetting::m_bIsGearSwitch = CSysConfig::Instance().GetGearSwitch();
 		
	//读取公用配置,恢复状态
	TCHAR tchDir = '\\';
	CString strTemp;
	GetModuleFileName(NULL, strTemp.GetBuffer(MAX_PATH), MAX_PATH);
	strTemp.ReleaseBuffer();
	int PathPos = strTemp.ReverseFind(tchDir) + 1;
	CString strExePath = strTemp.Left(PathPos);
	
	CString LangPath = _T("");
	CString SkinPath = _T("");
	LangPath = strExePath + CSysConfig::Instance().GetLanguagePath();
	SkinPath = strExePath + CSysConfig::Instance().GetSkinPath();
	enLanguage LanguageId = ID_ENGLISH;
	LanguageId	=  CSysConfig::Instance().GetLanguage();
	enSkinType SkinId = ID_SKIN1;
	SkinId = CSysConfig::Instance().GetSkinType();
	if (LangPath.IsEmpty() || SkinPath.IsEmpty())
	{
		CSysConfig::Instance().ResetDefault();
	}
	
	CUiMng::Instance().SetLanguagePath(LangPath);
	CUiMng::Instance().SetLanguage(LanguageId);
	CUiMng::Instance().SetTimeUnit(CSysConfig::Instance().GetTimeUnit());
	
	CUiMng::Instance().SetSkinPath(SkinPath);
	CUiMng::Instance().SetSkinType(SkinId);
	
	//设置时区
	int iTimeZone = CSysConfig::Instance().GetTimeZone();	
	CUnitAndFormatSwitch::SetTimeZone(iTimeZone);
	//电话短信功能开关
	m_bPhone = CSysConfig::Instance().GetPhone();
	
	g_bFmInit = TRUE;

	return;
}

	//变换语言
BOOL CE9101MainDlg::OnLanguageChange()
{
	CResString str;

	m_csTitle = _T("");
	m_Item[0].chChar = _T("汽车导航");
	m_Item[1].chChar = _T("一键通");
	m_Item[2].chChar = _T("电话");
	m_Item[3].chChar = _T("文本信息");
	m_Item[4].chChar = _T("摄像头");
	m_Item[5].chChar = _T("视频播放");
	m_Item[6].chChar = _T("音乐播放");
	m_Item[7].chChar = _T("图片浏览");
	m_Item[8].chChar = _T("书籍阅读");
	m_Item[9].chChar = _T("系统设置");

	LoadGSMNetType();

	return TRUE;
}

//变皮肤
BOOL CE9101MainDlg::OnSkinChange()
{
	return TRUE;
}

VOID CE9101MainDlg::OnBtnSysset() 
{
	CSysSetDlg SysDlg;
	int nResponse = SysDlg.DoModal();
}
//调度信息
VOID CE9101MainDlg::OnBtnDiaoDu()
{
	CDlgTextInfoList dlg;
	CDlgTextInfoList::m_iInfoType  = 0;
	dlg.DoModal();
}

//驾驶信息
VOID CE9101MainDlg::OnBtnBook()
{
	CDlgMediaMain dlg;
	dlg.OnBtnText();
}

void CE9101MainDlg::OnTimer(UINT nIDEvent)
{
	if( TID_FOR_START_INIT == nIDEvent )
	{
		KillTimer(TID_FOR_START_INIT);
	
		Sleep( 1500 );

		g_GsmLogic.InitFindSrv( GetSafeHwnd() );
		m_ResumeGPRS = false;
		g_GsmLogic.GSMInit();
		g_GsmLogic.GSMReFindHwnd();
		m_bEnableInput = TRUE;

		{
			UINT nLev = CSysConfig::Instance().GetGSMVol();
			if ( nLev > 11 ) nLev = 11;
			if ( nLev < 0 ) nLev = 0;
			if ( nLev )
				g_GsmLogic.GSMSetVol( nLev * 8 + 1  );
		}

		if ( CSysConfig::Instance().GetAutoNavi() )
		{
			TCHAR bCmdline[100];
			LPCWSTR srCmdLine = bCmdline;

			BOOL bProcessState = CreateProcess(CSysConfig::Instance().GetNaviPath(), NULL, NULL, NULL, 
						FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);

			if(bProcessState)
			{
				CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
			}
		}
	}
	else if (100 == nIDEvent)
	{
		KillTimer(100);
		bTimeOver = TRUE;
	}
	else if (TIMER_POWER_STATE == nIDEvent)
	{
		KillTimer(TIMER_POWER_STATE);
		::PlaySound( _T("\\Flashdrv Storage\\menu\\Style\\BatteryLow.wav\0"), NULL, SND_ASYNC | SND_NODEFAULT );
		PowerTip = FALSE;
	}
	else if (nIDEvent == TIMER_CURTIME)
	{
		if ( m_nTimerCount < 120 )
			Invalidate();
	}
	else if (nIDEvent == TID_CLOSE_MSGBOX)
	{
		KillTimer(TID_CLOSE_MSGBOX);
		HWND hwnd = ::FindWindow(NULL, _T("MENU"));
		if(hwnd)
		{
			::SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}
	else if(TID_START_NAVI_DELAY == nIDEvent)
	{
		KillTimer(nIDEvent);
		m_bCanStartNavi = TRUE;	//开机后15秒才能启动导航软件

		//一体机才需要启动GSM_AT.exe(GSM模块通讯用)， WirelessManager.exe(RAS拨号用)
		if(1)
		{
			CreateProcess(_T("\\Flashdrv Storage\\Menu\\GSM_AT.exe"), NULL, NULL, NULL, 
				FALSE, 0, NULL, NULL, NULL, NULL);
			Sleep(2000);

			CreateProcess(_T("\\Flashdrv Storage\\Menu\\WirelessManager.exe"), NULL, NULL, NULL, 
				FALSE, 0, NULL, NULL, NULL, NULL);
		}

		CreateProcess(_T("\\Flashdrv Storage\\Menu\\GNSSTerminal.exe"), NULL, NULL, NULL, 
			FALSE, 0, NULL, NULL, NULL, NULL);
	}

	CDialogBase::OnTimer(nIDEvent);
}

void CE9101MainDlg::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	bk_gsm_dc.SelectObject( m_pold_bk_gsm_bmp );
	
	DeleteObject( m_stGSMSignalBMP );
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	DeleteObject(m_stIconFlagBMP);

	bk_gsm_dc.DeleteDC();		
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CE9101MainDlg::OnExit()
{
	ReleaseGdi();
	KillTimer(TIMER_CURTIME);
	CDialogBase::EndDialog(0);
}


void  CE9101MainDlg::DelayTime(int nSecMil)
{
	MSG  msg;
	SetTimer(100, nSecMil, NULL);
	while (!bTimeOver)
	{
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message >0 && WM_CREATE_SET)
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
}

LRESULT CE9101MainDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_CHILD_EXIT)
	{
		SetTimer(TIMER_CURTIME, 1000, NULL);
	}
	else if ( message == UMSG_GSM_CMD )
	{
		OnCmdGSM( wParam, lParam );
	}
	else if ( message == WM_COPYDATA )
	{
		OnCmdCOPYDATA( wParam, lParam );
	}
	else if(WM_COMMAND_TIANZE == message)
	{
		OnCmdTianze( wParam, lParam );
	}
	else if (g_nWM_MENU_GNSSTERMINAL == message)
	{
		if (MSG_SVR_CALL_BACK == wParam)
		{
			CMsgProc::Instance()->OnCALL_BACK(lParam);
		}
	}

	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

VOID CE9101MainDlg::OnCmdGSM(WPARAM wParam, LPARAM lParam)
{
	if ( wParam == GSM_ACK_NEW_CALLING )
	{
		if( !m_bPhone )
		{
			g_GsmLogic.GSMHangup();
			::PostMessage(HWND_BROADCAST, g_nDialIn, 0, 0 );
			return;
		}

		::PostMessage(HWND_BROADCAST, g_nDialIn, 1, 0 );
		::PostMessage(HWND_BROADCAST, g_nPowerLow, 0, 0);
		::PostMessage(HWND_BROADCAST, g_nDialIn, 1, 0 );
		::PostMessage(HWND_BROADCAST, g_nPowerLow, 0, 0);
		
		Sleep( 100 );
		
		//有新电话来了
		CGSMCalling dlg;
		dlg.SetCallingStr( g_GsmLogic.GetCallingPhone() );
		dlg.DoModal();
		::PostMessage(HWND_BROADCAST, g_nDialIn, 0, 0 );
		::PostMessage(HWND_BROADCAST, g_nDialIn, 0, 0 );
	}
	else
	{
		g_GsmLogic.InitFindSrv( GetSafeHwnd() );
		g_GsmLogic.onGsmAckCmd( wParam, lParam );
		if ( wParam == GSM_ACK_QUERY_NET )
		{
			LoadGSMNetType();
		}
	}
	Invalidate(FALSE);
}

VOID  CE9101MainDlg::OnCmdCOPYDATA(WPARAM wParam, LPARAM lParam)
{
	Sleep(500);
	int nRestul = g_GsmLogic.onGsmDataCopy( wParam, lParam );
	if ( nRestul == 999 )
	{
		ResetOS();
	}
	Invalidate();
}

VOID CE9101MainDlg::OnCmdTianze(WPARAM wParam, LPARAM lParam)
{
	if(WPARAM_SERVER_LOGIN == wParam)
	{//网络状态: 登录服务器:	0 掉线  1 接通  RAS拨号:  2 断开    3 连接
		switch(lParam)
		{
		case 0:	m_nLoginServer = 0;		break;
		case 1:	m_nLoginServer = 1;		break;
		case 2:	m_nRasState = 2;		break;
		case 3:	m_nRasState = 3;		break;
		}
	}
}

//服务中心下发巡检指令
VOID CE9101MainDlg::CheckRFID(LPARAM lParam)
{
	
}

DWORD CE9101MainDlg::THDOpenRegDialog( LPVOID lpParameter )
{
	CE9101MainDlg* pDlg = (CE9101MainDlg*)lpParameter;
	CDlgRFIDCheck* pdlg = new CDlgRFIDCheck(FALSE,pDlg);
	pdlg->Create(CDlgRFIDCheck::IDD, pDlg);
	pdlg->ShowWindow(SW_SHOW);
	return 1;
}


//接收服务器下发拍照指令
VOID CE9101MainDlg::PicSnap(WPARAM wParam, LPARAM lParam)
{
	//检查参数，并且回复
	HWND hWnd;

	short Pixel = (short)(lParam & 0x0000FFFF);		//分辨率1,2,3
	short Cameral = (short)(lParam >> 16);			//选择摄像头1，2，3，4,  (0为4个摄像头都拍)
	//if(Cameral == 0)
	//	return;
	if(Pixel < 1 || Pixel > 3 || Cameral < 0 || Cameral > 4)
	{//失败
		CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, WPARAM_SNAP,0);
		return;
	}

	BOOL bProcess = CreateProcess( PATH_CAMERA_VIDEO, 
		_T("0"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
	Sleep(200);
	//给摄像头进程发送拍照指令
	hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
	if(NULL != hWnd)
		::PostMessage(hWnd, WM_COMMAND_PHOTO, WPARAM_SNAP, lParam);
}

VOID CE9101MainDlg::ResetOS()
{
	HANDLE h_handle = NULL;

	h_handle = CreateFile(TEXT("MID0:"), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h_handle == INVALID_HANDLE_VALUE)
		return;

	if(!DeviceIoControl(h_handle, IOCTRL_SYSTEM_RESET, NULL, 0, NULL, 0, NULL, NULL))
		return;

	CloseHandle(h_handle);
	h_handle = NULL;
}

VOID CE9101MainDlg::CancelTask()
{
	//char			m_taskID[10];		//任务编号
	//unsigned char	ucTaskStyle;		//任务类型：0：定期定点任务		1：紧急救援任务
	//unsigned char	ucOperateStyle;		//操作类型：0：取消				1：终止
	/*
	cex_file file;
	if ( !file.open( PATH_TZ_BUFFER_CABCLE_CUT_TASK, cex_file::m_read ) )
		return;
	T_CancleTask task_cancel;
	memset(&task_cancel, NULL, sizeof(T_CancleTask));
	file.read( &task_cancel, sizeof(T_CancleTask) );
	file.close();
	//定期定点任务
	if(task_cancel.ucTaskStyle == 0)
	{
		g_pTaskDing->CancelTask(task_cancel.m_taskID, sizeof(task_cancel.m_taskID));
		g_pTaskDing->SaveData();
	}
	//紧急救援任务
	else if(task_cancel.ucTaskStyle == 1)
	{
		CSMSTask_data::Instance()->cancel_task(task_cancel.m_taskID, sizeof(task_cancel.m_taskID));
	}
	*/
}

VOID CE9101MainDlg::OnCmdCarReverse(WPARAM wParam, LPARAM lParam)
{
	static bool bIsCarReverse = false;
	if (1 == wParam && !bIsCarReverse)
	{//开始倒车
		bIsCarReverse = true;

		HWND hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
		if(NULL != hWnd)
		{
			::PostMessage(hWnd, WM_COMMAND_PHOTO, WPARAM_CAR_REVERSE, 1);
		}
		else
		{
			BOOL bProcess = CreateProcess( PATH_CAMERA_VIDEO, 
				_T("2"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
		}
	}
	else if (0 == wParam && bIsCarReverse)
	{//结束倒车
		bIsCarReverse = false;
		
		HWND hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
		if(NULL != hWnd)
		{
			::PostMessage(hWnd, WM_COMMAND_PHOTO, WPARAM_CAR_REVERSE, 0);
		}
	}
	/*
	if (1 == wParam)
	{
		if (m_bIsCARReverseOpen == FALSE)
		{
			InputSelect(0x7, 0xE8);
			CRect rect; 
			rect.SetRect(0,0,800,480);
			m_stcVideo.MoveWindow(rect);
			CamSetRect(&rect);
			CameraCtrl(TRUE);
			
			m_bIsCARReverse = TRUE;
			m_bIsCARReverseOpen = TRUE;
		}
	}
	else if (0 == wParam)
	{
		if (m_bIsCARReverse == TRUE)
		{
			CRect rect; 
			rect.SetRect(0,0,0,0);
			m_stcVideo.MoveWindow(rect);
			CamSetRect(&rect);
			CameraCtrl(FALSE);
			m_bIsCARReverse = FALSE;
			m_bIsCARReverseOpen = FALSE;
			POINT point = {400,240};
			CWnd *cwnd = WindowFromPoint(point);
			cwnd->Invalidate();
		}
	}*/
}

VOID CE9101MainDlg::WriteSMSCenter()
{
	::PlaySound( _T("\\Flashdrv Storage\\menu\\Style\\SMS_Center_REC.wav\0"), 
		NULL, SND_ASYNC | SND_NODEFAULT );
	CDlgTextInfoList::m_bSMSCenter	= TRUE;			//是否有中心信息
	::PostMessage(HWND_BROADCAST, WM_REFRESH_PAGE, 0, 0);
	return;
}

VOID CE9101MainDlg::WriteSMSTask()
{
	cex_file file;
	if ( !file.open( PATH_TZ_BUFFER_SMS_TASK, cex_file::m_read ) )
		return;
	if( file.get_file_size() != sizeof(st_SMSTask) )
	{
		file.close();
		return;
	}
	st_SMSTask stSMSTask;
	memset(&stSMSTask, NULL, sizeof(st_SMSTask));
	file.read( &stSMSTask, sizeof(st_SMSTask) );
	file.close();
	
	if( CSMSTask_data::Instance()->is_open() )
	{
		CSMSTask_data::Instance()->add_recourd( &stSMSTask );
		//文件数据已装载到内存,则不需要清空内存数据
		CSMSTask_data::Instance()->save_data( false );
	}
	else
	{
		CSMSTask_data::Instance()->add_2file_end( &stSMSTask );
	}
}



LRESULT CE9101MainDlg::OnTip(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

LRESULT CE9101MainDlg::OnChildExit(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOW);
	return 1L;
}

LRESULT CE9101MainDlg::OnSdCardMessage(WPARAM wParam, LPARAM lParam)
{
	return 1L;
}

LRESULT CE9101MainDlg::OnPowerEvent(WPARAM wParam, LPARAM lParam)
{
	static int Percent = 100;
	
	m_PowerState =(POWERSTAT_E)wParam;
	
	switch (m_PowerState)
	{
	case BATTERY_CHARGING:
		{
			m_PowerState = BATTERY_CHARGING;
			m_stcBattery.ShowWindow(SW_SHOW);
			S_5Minute = 0;
			Percent = 100;
		}
		break;
	case BATTERY_FULL:
		{
			m_PowerState = BATTERY_FULL;
			m_stcBattery.SkinSetBatterNum(battery_none);
 			m_stcBattery.ShowWindow(SW_SHOW);
			S_5Minute = 0;
			break;
		}
	case BATTERY_ONLINE:
		m_PowerState = BATTERY_ONLINE;
		m_nPercent =(int)lParam;
		m_nPic = m_nPercent/20;
		{
			//电量0时可以使主页面电量显示回跳
			if (m_nPercent != 0)
			{
				if (m_nPercent > Percent)
				{
					m_nPercent = Percent;
				}
				else
				{
					Percent = m_nPercent;
				}
			}
			switch (m_nPercent)
			{
			case 0:
				{
					m_stcBattery.SkinSetBatterNum(battery_empy);
					m_stcBattery.ShowWindow(SW_SHOW);
					S_5Minute++;
					if( S_5Minute > 40 )
					{				
						if (!PowerTip)
						{
							PowerTip = TRUE;
							SetTimer(TIMER_POWER_STATE, 10000, NULL);
						}
						S_5Minute = 0;
					}
					break;
				}
			case 20:
				m_stcBattery.SkinSetBatterNum(battery_20);
				S_5Minute = 0;
				//PowerTip = TRUE;
				m_stcBattery.ShowWindow(SW_SHOW);
				break;
			case 40:
				m_stcBattery.SkinSetBatterNum(battery_40);
				S_5Minute = 0;
				//PowerTip = TRUE;
				m_stcBattery.ShowWindow(SW_SHOW);
				break;
			case 60:
				m_stcBattery.SkinSetBatterNum(battery_60);
				S_5Minute = 0;
				//PowerTip = TRUE;
				 m_stcBattery.ShowWindow(SW_SHOW);
				break;
			case 80:
				m_stcBattery.SkinSetBatterNum(battery_80);
				S_5Minute = 0;
				//PowerTip = TRUE;
				m_stcBattery.ShowWindow(SW_SHOW);
				break;
			case 100:
				m_stcBattery.SkinSetBatterNum(battery_100);
				S_5Minute = 0;
				//PowerTip = TRUE;
				m_stcBattery.ShowWindow(SW_SHOW);
				break;
			default:
				m_stcBattery.SkinSetBatterNum(battery_20);
				S_5Minute = 0;
				//PowerTip = TRUE;
				m_stcBattery.ShowWindow(SW_SHOW);
				break;
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return 1L;
}
//////////////////////////////////////////////////////////////////////////

void CE9101MainDlg::OnBtnGPRS()
{
}

unsigned long CALLBACK watch_proce(void* p_param)
{
	return 1;
}

VOID CE9101MainDlg::OnBtnDial()
{
	if( !m_bPhone )//电话短信功能开关
	{
		CDlgConfirm2::m_stTitle = _T("无权限使用该功能!");
		CDlgConfirm2 dlg;
		dlg.DoModal();
		return;
	}
	
 	if( 0 == g_GsmLogic.GetGSMNetType() )
 	{
 		SetTimer(TID_CLOSE_MSGBOX, 3*1000, NULL);
 		MessageBox(_T("系统正在查找网络......"), _T("MENU"), MB_OK);
 		return;
 	}
	CDlgPhone dlg;
	dlg.DoModal();
	//dlg.DoModal();
	//return;

}

LRESULT CE9101MainDlg::OnBlueToothStateTrans(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

//接收拨入电话号码(蓝牙模块发送)
LRESULT CE9101MainDlg::OnBlueToothCallNumber(WPARAM wParam, LPARAM lParam)
{
	CString str;
	char ansi_str[64];
	int i = 0;
	for (i = 0; i < lParam; i++)
		ansi_str[i] = *(unsigned char *)wParam++;
	ansi_str[lParam] = '\0';
	
	i = 0;
	while (isdigit(ansi_str[i++]));
	
	if (isdigit(ansi_str[0]))
	{
		CDlgDiaIn::m_strNumber.Empty();
		TCHAR wide_str[64];
//		mbstowcs(wide_str, (const char*)ansi_str, --i);
		MultiByteToWideChar(CP_ACP, 0, (char *)ansi_str, --i, wide_str, 2 * i);
		CDlgDiaIn::m_strNumber.Format(_T("%s"), wide_str);
		CDlgDiaIn::m_strNumber = CDlgDiaIn::m_strNumber.Left(lParam);

		CPhoneBookFile *pPhoneBook; 
		PhoneBookMng::Instance().Load();

		UINT Count = PhoneBookMng::Instance().GetCount();
	
		for(UINT i=0; i<Count; i++)
		{
			pPhoneBook = PhoneBookMng::Instance().GetPhoneBookItem(i);

			if(pPhoneBook->m_csNum == CDlgDiaIn::m_strNumber)
			{
				CDlgDiaIn::m_strName = pPhoneBook->m_csName;
			}
		}
	}
	return TRUE;
}
//电源低电量报警模块析构时发送给主窗体的消息
LRESULT CE9101MainDlg::OnPowerLowDlgDestroy(WPARAM wParam, LPARAM lParam)
{
//	s_pTipDlg = NULL;
	return 0;
}

///////////////////////////////////////////////////////////////////////
//
// 函数名       : CE9101MainDlg::OnHideWindow
// 功能描述     : 当播放MP3，MP4和浏览图片时，发送该消息隐藏主窗体
// 参数         : WPARAM wParam
// 参数         : LPARAM lParam
// 返回值       : LRESULT 
//
///////////////////////////////////////////////////////////////////////
LRESULT CE9101MainDlg::OnHideWindow(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CE9101MainDlg::OnOutGoingCall(WPARAM wParam, LPARAM lParam)
{
	if(g_PhoneDialOut != 2)
	{
		::PostMessage(HWND_BROADCAST, g_nDialIn, 1, 0);
 		if(g_AV_IN)
			return 0;			
			CDlgDiaIn dlg;
			CDlgDiaIn::m_strNumber = _T("***********");
			CDlgDiaIn::m_strName = _T("");
			CDlgDiaIn::m_strCityr = _T("");
			CDlgDiaIn::m_bIsAcc = TRUE;
			dlg.DoModal();
	}

	return 0;
}

LRESULT CE9101MainDlg::OnOffHookCall(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(g_hDialIn,WM_HBCPOFFHOOK, 0, 0);
	return 0;
}

LRESULT CE9101MainDlg::OnActiveCall(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(g_hDialIn,WM_HBCPACTIVECALL, 0, 0);
	return 0;
}

LRESULT CE9101MainDlg::OnBlueToothConnected(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	::PostMessage(g_hDialIn,BLUETOOTH_STATE,1,0);
	g_Bluetooth_state = TRUE;
	return 0;
}

LRESULT CE9101MainDlg::OnRingInd(WPARAM wParam, LPARAM lParam) 
{
	::PostMessage(g_hDialIn, WM_HBCPRINGIND, 0, 0);
	return 0;
}

void CE9101MainDlg::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC );
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	bk_gsm_dc.CreateCompatibleDC( m_pDC );

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_MENU_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_BG_MENU_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;  
	
	pSkin = g_pResMng->RequestBmp(SKIN_GSM_SIGNAL, true);
	m_stGSMSignalBMP = pSkin->hBitmap;

	m_pold_bk_normal_bmp = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp  = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_bk_gsm_bmp    = bk_gsm_dc.SelectObject(CBitmap::FromHandle(m_stGSMSignalBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CE9101MainDlg::InitControl()
{
	int i;
	for (i =0;  i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	//10个按钮
	for(i=0; i<BTN_COUNT; i++)
	{
		//第一行
		if(i<5)
		{
			m_Item[i].rect.left   = 57+i*143;
			m_Item[i].rect.top    = 86;
			m_Item[i].rect.right  = 57+i*143 + 123;
			m_Item[i].rect.bottom = 207;
		}
		//第二行
		else
		{
			m_Item[i].rect.left   = 57+(i-5)*143;
			m_Item[i].rect.top    = 266;
			m_Item[i].rect.right  = 57+(i-5)*143+123;
			m_Item[i].rect.bottom = 390;
		}
	}
	m_MenuRect = g_retTitile;
	
	if( CSysConfig::Instance().GetLanguage() == ID_RUSSIA )
	{
		m_MenuRect.left -= 1;
		
		m_ItemFm.rect.top = 0;
		m_ItemFm.rect.left = 251;
		m_ItemFm.rect.bottom = 33;
		m_ItemFm.rect.right = 278;
	}
	else
	{
		if( ID_SKIN1 == CUiMng::Instance().GetCurSkin() )
		{
			m_MenuRect.left -= 1;
			
			m_ItemFm.rect.top = 0;
			m_ItemFm.rect.left = 122;
			m_ItemFm.rect.bottom = 33;
			m_ItemFm.rect.right = 152;
		}
		else if( ID_SKIN2 == CUiMng::Instance().GetCurSkin() )
		{
		}
	}
	m_MenuRect.left += 8;
	
	m_ItemGSM.rect.top    = 0;
	m_ItemGSM.rect.left   = 650;
	m_ItemGSM.rect.bottom = 64;
	m_ItemGSM.rect.right  = m_ItemGSM.rect.left + 64;
	
	m_ItemSMS.rect.top    = 0;
	m_ItemSMS.rect.left   = 550;
	m_ItemSMS.rect.bottom = 64;
	m_ItemSMS.rect.right  = m_ItemSMS.rect.left + 64;
	
	m_ItemFm.rect.top = 0;
	m_ItemFm.rect.left = 160;
	m_ItemFm.rect.bottom = 33;
	m_ItemFm.rect.right = 186;
}

void CE9101MainDlg::DrawItems(CDC* pDC, const CRect &rt, enBtnState state)
{
	ASSERT(pDC);
	int x = 0;
	int y = 0;
	switch(state)
	{
	case BTN_STATE_NORMAL:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DOWN:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DISABLE:
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CE9101MainDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
{
	COLORREF  nOldRgb;
	nOldRgb=pDC->SetTextColor( col );
	int      nOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont* def_font = pDC->SelectObject(&font);

	pDC->DrawText(str, &rt, Format );

	pDC->SelectObject(def_font);
	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(nOldRgb);
}

void CE9101MainDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	//6 buttons
	for(int i=0;i<BTN_COUNT;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.bottom += 25;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont());
	}

	DrawItems(&memDC,m_ItemFm.rect,m_ItemFm.nState);

	if (m_pPosInfo->rmc_data.pos_state.cFix)
	{
		m_csTitle = _T("GPS已定位");
	}
	else
	{
		m_csTitle = _T("GPS未定位");
	}
	//主菜单
	DrawBtnText(&memDC,m_MenuRect,m_csTitle,CGDICommon::Instance()->bigfont(),RGB(255,255,255),DT_LEFT | DT_VCENTER);

	//一体机才需要显示信号
	if( 1 )
	{
		//显示移动运营商
		Rect = m_MenuRect;
		Rect.left =  240;
		Rect.right = 460;
		DrawBtnText(&memDC,Rect, m_strNetTip, CGDICommon::Instance()->bigfont(), RGB(255,255,255), DT_LEFT | DT_VCENTER );

		//绘制GSM信号
		UINT nIdx = g_GsmLogic.GetSMSSignal();
		if ( nIdx == sqr_count ) //没有接收到信号图片，则显示无信号
		{
			nIdx = 1;
			memDC.BitBlt( 
				m_ItemGSM.rect.left, 
				m_ItemGSM.rect.top,
				m_ItemGSM.rect.Width(),
				m_ItemGSM.rect.Height(),
				&bk_gsm_dc,
				nIdx * m_ItemGSM.rect.Width(), 0, SRCCOPY );
		}
		else if ( nIdx > sqr_no_sim && nIdx < sqr_count )
		{
			nIdx = nIdx + 1;
			memDC.BitBlt(
				m_ItemGSM.rect.left, 
				m_ItemGSM.rect.top, 
				m_ItemGSM.rect.Width(),
				m_ItemGSM.rect.Height(),
				&bk_gsm_dc,
				nIdx * m_ItemGSM.rect.Width(), 0, SRCCOPY );
		}

		//显示未读短信
		if(g_GsmLogic.m_iUnRdMsg || CDlgTextInfoList::m_bSMSCenter)			//是否有新的中心信息
		{
			memDC.BitBlt(
				m_ItemSMS.rect.left, 
				m_ItemSMS.rect.top, 
				m_ItemSMS.rect.Width(),
				m_ItemSMS.rect.Height(),
				&bk_gsm_dc,
				7 * m_ItemGSM.rect.Width(), 0, SRCCOPY );
		}

		// 如果没有SIM卡，则显示没有SIM的图标，否则不显示
		if ( g_GsmLogic.GetSMSSignal() != sqr_no_sim && !g_GsmLogic.GetSIMState()  )
		{
			CRect rt(m_ItemGSM.rect);
			rt.OffsetRect( m_ItemGSM.rect.Width(), 0 );
			//memDC.BitBlt( rt.left, rt.top, rt.Width(), rt.Height(), &bk_gsm_dc, 0, 0, SRCCOPY );
		}
		//显示RAS拨号和服务器登录状态
		int nOffset = 8;
		if(1 /*g_pstShareMem != NULL*/ )
		{
			if( m_nRasState == 2 )		//RAS未连接GPRS网络
				nOffset = 8;
			else if( m_nRasState == 3 )	//RAS连接成功
			{//网络状态: 登录服务器:	0 掉线  1 接通  RAS拨号:  2 断开    3 连接
				if( m_nLoginServer == 1 )//1 接通
					nOffset = 10;
				else if( m_nLoginServer == 0 )	//0 掉线
					nOffset = 9;
			}

			g_GsmLogic.SetLoginState(nOffset);
		}
		memDC.BitBlt( 150, 0, m_ItemGSM.rect.Width(), m_ItemGSM.rect.Height(),
			&bk_gsm_dc, nOffset * m_ItemGSM.rect.Width(), 0, SRCCOPY );
	}

	if ( m_nTimerCount < 120 )
	{
		if ( g_GsmLogic.IsSMSHasMaxOnSIM() )
		{
			CResString str;
			str.LoadString(RES_SMS_SIM_MAX);
			CRect rt(134,10,480,40);
			DrawBtnText(&memDC,rt,str,CGDICommon::Instance()->smallfont(),RGB(255,0,0),DT_LEFT | DT_TOP);
		}
	}
	//绘制时间和日期
	DrawDataTime(&memDC);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	memDC.SelectObject(pOldBmp);
}

VOID CE9101MainDlg::DrawDataTime(CDC* pmemDC)
{
	CString str;
	SYSTEMTIME systime;
	if(g_pstShareMem != NULL 
		/*&& g_pstShareMem->st_GPS.m_location*/	//如果GPS定位,则用GPS时间,没有定位则使用本地时间
		&& g_pstShareMem->st_GPS.m_sys_time.wYear != 0			//还未解析到gps数据
		&& g_pstShareMem->st_GPS.m_sys_time.wYear != 1601)		//已知bug
		memcpy(&systime, &g_pstShareMem->st_GPS.m_sys_time, sizeof(SYSTEMTIME));
	else
		GetLocalTime(&systime);
	
	//GetLocalTime(&systime);
	//日期
	str.Format(_T("%d/%02d/%02d"), systime.wYear, systime.wMonth, systime.wDay);
	DrawBtnText(pmemDC, CRect(445, 0, 550, 31), str, 
		CGDICommon::Instance()->smallfont(), RGB(255,255,255), DT_LEFT|DT_BOTTOM);
	//时间
	str.Format(_T("%02d:%02d:%02d"), systime.wHour, systime.wMinute, systime.wSecond);
	DrawBtnText(pmemDC, CRect(445, 32, 550, 63), str, 
		CGDICommon::Instance()->smallfont(), RGB(255,255,255), DT_LEFT|DT_TOP);
}

void CE9101MainDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ( !m_bEnableInput ) 
		return;

	for(int i = 0; i < BTN_COUNT; i++ )
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			PlayKeySound();
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
			}
		}
	}

	Invalidate();
	SetCapture();
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CE9101MainDlg::OnBtnSMS()
{
	CSMSMain dlg;
	dlg.DoModal();
}

void  CE9101MainDlg::OnBtnPicture()
{
	CDlgMediaMain dlg;
	dlg.OnBtnPicture();
}

//状态上传
void  CE9101MainDlg::OnBtnReportState()
{
	SHELLEXECUTEINFO ShExecInfo;
	ZeroMemory(&ShExecInfo,sizeof(ShExecInfo));
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShExecInfo.hwnd = NULL; 
	ShExecInfo.lpVerb = _T("open"); 
	ShExecInfo.lpFile = _T("http://www.baidu.com");
	ShExecInfo.lpParameters = NULL; 
	ShExecInfo.lpDirectory = NULL; 
	ShExecInfo.nShow = SW_SHOW; 
	ShExecInfo.hInstApp = NULL; 
	ShellExecuteEx(&ShExecInfo); 
}

#include "SMSSending.h"
void CE9101MainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( !m_bEnableInput )
		return;
	for(int i=0;i<BTN_COUNT;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:		//汽车导航
				OnBtnNavi();
				break;
			case 1:		//一键通
				OnBtnPushToTalk();
				break;
			case 2:		//电话
				OnBtnDial();
				break;
			case 3:		//调度信息
				OnBtnDiaoDu();
				break;
			case 4:		//摄像头
				OnBtnCamera();
				break;
			case 5:		//视频播放
//				OnBtnVideo();
				break;
			case 6:		//音乐播放
//				OnBtnMP3();
				break;
			case 7:		//图片浏览
//				OnBtnPicture();
				break;
			case 8:		//书籍阅读
//				OnBtnBook();
				break;
			case 9:		//系统设置
				OnBtnSysset();
				break;
			default:
				break;
			}
		}
	}

	//短信图标
	if(PtInRect(&m_ItemSMS.rect, point))
	{
		if( CDlgTextInfoList::m_bSMSCenter )		//是否有中心信息
		{
			CDlgTextInfoList dlg;
			dlg.DoModal();

			goto stop;
		}
		else if(g_GsmLogic.m_iUnRdMsg > 0)	//未读短信
		{
			OnBtnNewSms();
			goto stop;
		}
	}
stop:
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

VOID CE9101MainDlg::OnBtnNavi() 
{

#define MSG_WND_NAME L"Guider_MsgWnd"	// 消息接收窗口名称

	HWND hwnd = ::FindWindow(NULL, MSG_WND_NAME);
	if (hwnd != NULL)
	{
		::SendMessage(hwnd,MSG_OPEN_GUIDER4U, NULL, NULL);
		return;
	}


	CString strNaviPath = CSysConfig::Instance().GetNaviPath();
	if( 0xFFFFFFFF == GetFileAttributes(strNaviPath) )
	{
		CDlgConfirm2::m_stTitle = _T("导航路径不存在，请重新设置!");
		CDlgConfirm2 dlg(3);
		dlg.DoModal();
		return;
	}
	if( !m_bCanStartNavi )
	{
		CDlgConfirm2::m_stTitle = _T("GPS模块初始化中,请稍后再启动!");
		CDlgConfirm2 dlg(3);
		dlg.DoModal();
		return;
	}

    g_bAppStatus = CreateProcess(strNaviPath, NULL, NULL, NULL, 
            FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);

 	if(g_bAppStatus)
	{
		CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
	}
}

VOID CE9101MainDlg::OnBtnPushToTalk()
{
	if( CSysConfig::Instance().GetPushToTalk() )
	{
		if( 0 == g_GsmLogic.GetGSMNetType() )
		{
			CDlgConfirm2::m_stTitle = _T("系统正在查找网络......");
			CDlgConfirm2 dlg(3);		  
			dlg.DoModal();
			return;
		}

		CString csNum;
		csNum = CSysConfig::Instance().GetCenterNum();
		if(csNum.IsEmpty())
		{
			return;
		}
		CGSMDial dlg;
		CGSMDial::m_UserNum = csNum;
		CGSMDial::m_bAutoDial = TRUE;
		dlg.DoModal();
	}
	else
	{
		CDlgPushToTalk dlg;
		dlg.DoModal();
	}
}

VOID CE9101MainDlg::OnBtnVideo()
{
	CDlgMediaMain dlg;
	dlg.OnBtnMovie();
}

VOID CE9101MainDlg::OnBtnMP3()
{
	CDlgMediaMain dlg;
	dlg.OnBtnMusic();
}

VOID CE9101MainDlg::OnBtnCamera()
{
	if (g_bAppStatus)
		return;
	CString strTemp;
	GetModuleFileName(NULL,strTemp.GetBuffer(60),60);
	strTemp.ReleaseBuffer();
	strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
	CString strPath = strTemp + _T("\\Video.exe");
	g_bAppStatus = CreateProcess(PATH_CAMERA_VIDEO, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);
	
	g_bCamera = TRUE;

	if(g_bAppStatus)
	{
		CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
	}
}

void CE9101MainDlg::OnBtnNewSms()
{
	CSMSInbox dlg;
	dlg.DoModal();
}

//中心拨号
void CE9101MainDlg::OnBtnCenterDial()
{
	if( 0 == g_GsmLogic.GetGSMNetType() )
	{
		CDlgConfirm2::m_stTitle = _T("系统正在查找网络......");
		CDlgConfirm2 dlg(3);		  
		dlg.DoModal();
		return;
	}
}

VOID CE9101MainDlg::CheckSMSReadStatus()
{
	int nUnReadSMS = 0;	//未读短信数量
	const char* pszSQL;
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	
	//查询中心信息未读短信总数量
	pszSQL = "select count(*) from sms_center where read_status = 0 and sms_datatime >= (SELECT datetime('now','localtime', '-3 day'));";
	nUnReadSMS = db.execScalar(pszSQL);
	if(nUnReadSMS > 0)
		CDlgTextInfoList::m_bSMSCenter	= TRUE;

	db.close();
}