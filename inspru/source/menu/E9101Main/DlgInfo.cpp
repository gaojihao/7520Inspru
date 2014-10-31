// DlgInfo.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgInfo.h"
#include "DlgGpsStar.h"
#include "E9101MainDlg.h"
#include "VolumeDlg.h"
#include "..\BlueToothPhone\HBCP.h"
#include "..\BlueToothPhone\Command.h"
#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//WirelessManager.exe窗口句柄
#define	WND_NAME_WIRELESS_MANAGER					_T("WND_WIRELESS_MANAGER")
#define	GNSS_PROCESS_TEXT							_T("WND_GNSSTERMINAL")
#define WND_BDCONVERT_PROCESS						_T("Wnd_BDConvert_Process")

CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfo)
	DDX_Control(pDX, IDC_BUTTON_INFO_EXIT, m_BtnExit);
	DDX_Control(pDX, IDC_BTN_INFO_GPSINFO, m_BtnGps);
	DDX_Control(pDX, IDC_BTN_CALIBRA, m_BtnCarlibra);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialogBase)
	//{{AFX_MSG_MAP(CDlgInfo)
	//ON_BN_CLICKED(IDC_BUTTON_INFO_EXIT, OnButtonInfoExit)
	ON_BN_CLICKED(IDC_BTN_INFO_GPSINFO, OnBtnInfoGpsinfo)
	ON_BN_CLICKED(IDC_BTN_CALIBRA, OnBtnCalibra)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo message handlers

LRESULT CDlgInfo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if( message == WM_CREATE_SET )
 	{
 		if( wParam == 0x06 )
 		{
 			//OnButtonInfoExit();
 		}
	}	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

BOOL CDlgInfo::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_BTN_INFO);
	m_stTitle = _T("系统信息");

	str.LoadString(RES_BUTTON_SCREEN);
//	m_BtnCarlibra.SetWindowText(str);
	//m_Item[0].chChar = str;

	str.LoadString(RES_BTN_DEFAULT_SET);
//	m_BtnGps.SetWindowText(str);
	//m_Item[1].chChar = str;

	//m_Item[2].chChar = _T("WinCE");


	return TRUE;
}
	//变皮肤
BOOL CDlgInfo::OnSkinChange()
{
	return TRUE;
}


BOOL CDlgInfo::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

//	TCHAR	szOSVersion[64] = {0};

//	SystemParametersInfo(SPI_GETPLATFORMTYPE, sizeof(szOSVersion), szOSVersion, 0);

	m_ItemSysInfo[3].chChar = _T("KP7520");
	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();

	if(m_IniFile.Load( _T("\\Flashdrv Storage\\Version.ini") ))
	{
 		m_IniFile.GetString(_T("VERSION"),_T("RAM"),m_ItemSysInfo[1].chChar);
 		m_IniFile.GetString(_T("VERSION"),_T("SWVER"),m_ItemSysInfo[3].chChar);
 		m_IniFile.GetString(_T("VERSION"),_T("IMEI"),m_ItemSysInfo[4].chChar);
		m_ItemSysInfo[4].chChar = _T("IMEI: ") + m_ItemSysInfo[4].chChar;
	}
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInfo::OnBtnInfoGpsinfo() 
{
	CSysConfig::Instance().ResetDefault();

	g_bFmInit = TRUE;
	CSysConfig::Instance().SetIsSpareMode( FALSE );
	CE9101MainDlg* pMainDlg = (CE9101MainDlg*)(AfxGetApp()->m_pMainWnd);
	::PostMessage( pMainDlg->m_hWnd,WM_FM_CHANGE , 0 , 0 );


	g_enKeySnd = CSysConfig::Instance().GetKeySoundType();

	//::waveOutSetVolume(0, 0xFFFFFFFF );

	OnInitConfig();
	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
	ReleaseGdi();
	InitGdi();
	::PostMessage( HWND_BROADCAST, WM_SKIN_CHANGE, (WPARAM)ID_SKIN1, 0  );
}

void CDlgInfo::OnBtnCalibra() 
{
	::DeleteFile(_T("\\Flashdrv Storage\\calibr.sav"));

	POINT pnt;
	HWND hWnd;

	BOOL bProcess = CreateProcess(_T("\\windows\\tchcalibrldr.exe"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, 
		NULL, &g_sProcessInfo);
	if(bProcess)
		CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);

	Sleep(1000);
	pnt.x = 160;
	pnt.y = 120;

	hWnd = ::WindowFromPoint(pnt);	
}

void CDlgInfo::OnInitConfig()
{

	//读取子进程路径
	ChileProcessInfo ProcessInfo;

	if(!CSysConfig::Instance().GetNaviInfo(ProcessInfo))
	{
	}
	else
	{
		g_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}

	if(!CSysConfig::Instance().GetMoviesInfo(ProcessInfo))
	{
	}
	else
	{
		g_csMovieName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csMovieIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if(!CSysConfig::Instance().GetMusicInfo(ProcessInfo))
	{
	}
	else
	{
		g_csMusicName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csMusicIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if(!CSysConfig::Instance().GetBlueToothInfo(ProcessInfo))
	{
	}
	else
	{
		g_csBlueToothName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csBlueToothIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if(!CSysConfig::Instance().GetPictureInfo(ProcessInfo))
	{
	}
	else
	{
		g_csPicName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csPicIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}
	
	if( !CSysConfig::Instance().GetVolumeInfo(ProcessInfo) )
	{
	}
	else
	{
		g_csVolumeName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
		g_csVolumeIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
	}

//	CSysConfig::Instance().GetIsSpareMode();
	
	//读取公用配置,恢复状态

	TCHAR tchDir = '\\';
	CString strTemp;
	GetModuleFileName(NULL, strTemp.GetBuffer(MAX_PATH), MAX_PATH);
	strTemp.ReleaseBuffer();
	int PathPos = strTemp.ReverseFind(tchDir) + 1;
	CString strExePath = strTemp.Left(PathPos);

	CString LangPath = strExePath + CSysConfig::Instance().GetLanguagePath();
	CUiMng::Instance().SetLanguagePath(LangPath);
	CUiMng::Instance().SetLanguage(CSysConfig::Instance().GetLanguage());
	CUnitAndFormatSwitch::SetCurTimeUnit(CSysConfig::Instance().GetTimeUnit());

	CString SkinPath = strExePath + CSysConfig::Instance().GetSkinPath();
	CUiMng::Instance().SetSkinPath(SkinPath);
	CUiMng::Instance().SetSkinType(CSysConfig::Instance().GetSkinType());


	return;
}

void CDlgInfo::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
	{
  		smallfont.CreateFont(
		-12,					    // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0")); 


	}
	else
	{
  		smallfont.CreateFont(
		-18,					    // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0"));             // lpszFacename
	}

		bigfont.CreateFont(
		32,					    // nHeight
		12,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0"));  

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_SYSINFO_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_SYSINFO_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgInfo::ReleaseGdi()
{

	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgInfo::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CDlgInfo::InitControl()
{
	for (int i =0;  i < 4; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	for (int j =0;  j < 6; j++)
	{
		m_ItemSysInfo[j].nState = BTN_STATE_DISABLE;
	}

	//cpu
	m_ItemSysInfo[0].rect.top = 84;
	m_ItemSysInfo[0].rect.left = 45;
	m_ItemSysInfo[0].rect.bottom = 94+50;
	m_ItemSysInfo[0].rect.right = 760;
	m_ItemSysInfo[0].chChar = _T("CPU Type: Samsung A8 1GHz");


	//RAM Size
	m_ItemSysInfo[1].rect.top = 84+50;
	m_ItemSysInfo[1].rect.left = 45;
	m_ItemSysInfo[1].rect.bottom = 94+50*2;
	m_ItemSysInfo[1].rect.right = 760;
	m_ItemSysInfo[1].chChar = _T("RAM Size: 128MB");

	//ROM Size
	m_ItemSysInfo[2].rect.top = 84+50*2;
	m_ItemSysInfo[2].rect.left = 45;
	m_ItemSysInfo[2].rect.bottom = 94+50*3;
	m_ItemSysInfo[2].rect.right = 760;
	m_ItemSysInfo[2].chChar = _T("ROM Size: 4GB");



	//软件版本
	m_ItemSysInfo[3].rect.top = 84+50*3;
	m_ItemSysInfo[3].rect.left = 45;
	m_ItemSysInfo[3].rect.bottom = 94+50*4;
	m_ItemSysInfo[3].rect.right = 760;
	m_ItemSysInfo[3].chChar = _T("Sofeware version: v2.0.0.02.2");

	//IMEI
	m_ItemSysInfo[4].rect.top = 84+50*4;
	m_ItemSysInfo[4].rect.left = 45;
	m_ItemSysInfo[4].rect.bottom = 94+50*5;
	m_ItemSysInfo[4].rect.right = 760;
	m_ItemSysInfo[4].chChar = _T("ID: 8D56C09D00001AA1");

	//设备类型
	m_ItemSysInfo[5].rect.top = 84+50*5;
	m_ItemSysInfo[5].rect.left = 45;
	m_ItemSysInfo[5].rect.bottom = 94+50*6;
	m_ItemSysInfo[5].rect.right = 760;
	m_ItemSysInfo[5].chChar = _T("DeviceType: KP7520");

	//GPS串口
	//m_ItemSysInfo[4].rect.top = 94+60*4;
	//m_ItemSysInfo[4].rect.left = 45;
	//m_ItemSysInfo[4].rect.bottom = 94+60*5;
	//m_ItemSysInfo[4].rect.right = 760;
	//m_ItemSysInfo[4].chChar = _T("");	//GPS COM: COM6,9600 

	//屏幕校准
	m_Item[0].rect.left		= 368;
	m_Item[0].rect.top		= 396;
	m_Item[0].rect.right	= 513;
	m_Item[0].rect.bottom	= 480;
	m_Item[0].chChar		= _T("屏幕校准");
	//恢复默认设置
	m_Item[1].rect.left		= 514;
	m_Item[1].rect.top		= 396;
	m_Item[1].rect.right	= 655;
	m_Item[1].rect.bottom	= 480;
	m_Item[1].chChar		= _T("恢复默认");
	//WinCE
	m_Item[2].rect.left		= 656;
	m_Item[2].rect.top		= 396;
	m_Item[2].rect.right	= 800;
	m_Item[2].rect.bottom	= 480;
	m_Item[2].chChar		= _T("WinCE");
	//Exit
	m_Item[3].rect.left		= 740;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;
}

void CDlgInfo::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgInfo::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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
void CDlgInfo::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<4;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.top = Rect.top ;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),RGB(255,255,255));
	}

	for(int j=0;j<6;j++)
	{
		//绘制背景
		str = m_ItemSysInfo[j].chChar;
		Rect = m_ItemSysInfo[j].rect;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),RGB(255,255,255), DT_VCENTER | DT_LEFT );
	}

	DrawBtnText(&memDC,g_retTitile,m_stTitle,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER | DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CDlgInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	KillTimer( TID_SHOW_GSM_AT );
	KillTimer( TID_EXIT_PROCESS );
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<4;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			
				switch( i )
					{
 					case 0:
 						OnBtnCalibra();
 						break;
 					case 1:
 						//OnBtnInfoGpsinfo();
 						break;
 					case 2:
 						//OnBtnWince();
						CreateProcess(_T("\\Flashdrv Storage\\Menu\\DebugTool.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
 						break;
 					case 3:
 						OnExit();
 	 				default:
						break;
					}
			}		
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}


void CDlgInfo::OnBtnWince()
{
	// TODO: Add your control notification handler code here
	CreateProcess(_T("\\Windows\\explorer.exe"), NULL, NULL, NULL, 
		FALSE, 0, NULL, NULL, NULL, NULL);
	
	HWND hWnd;
	//关闭GSM_AT.EXE进程
	hWnd = ::FindWindow(NULL, WND_NAME_GSM_AT);
	if(hWnd)
	{
		::PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
	//关闭WirelessManager.exe进程
	hWnd = ::FindWindow(NULL, WND_NAME_WIRELESS_MANAGER);
	if(hWnd)
	{
		::PostMessage(hWnd, WM_DESTROY, 0, 0);
	}

	//关闭GNSSTerminal.exe进程
	hWnd = ::FindWindow(NULL, GNSS_PROCESS_TEXT);
	if(hWnd)
	{
		::PostMessage(hWnd, WM_DESTROY, 0, 0);
	}

	//关闭BDConvert.exe进程  
	hWnd = ::FindWindow(NULL, WND_BDCONVERT_PROCESS);
	if(hWnd)
	{
		::PostMessage(hWnd, WM_DESTROY, 0, 0);
	}

	ExitProcess(0);
}

void CDlgInfo::OnTimer(UINT nIDEvent) 
{
	if ( TID_SHOW_GSM_AT == nIDEvent )
	{
		KillTimer( nIDEvent );
		//HWND hWnd = ::FindWindow(NULL, GPRS_WND_TITLE);
		//::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,800,480, SWP_SHOWWINDOW );
		HWND hWnd = ::FindWindow(NULL, _T("WND_GSM_AT"));
		if(hWnd != NULL)
		{
			static bool bShowWnd = false;
			if( bShowWnd )
			{
				::SetWindowPos( hWnd, HWND_BOTTOM, 0,0,0,0, SWP_HIDEWINDOW );
				bShowWnd = false;
			}
			else
			{
				::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,480,272, SWP_SHOWWINDOW );
				bShowWnd = true;
			}
		}
		else
		{
			CreateProcess(_T("\\Flashdrv Storage\\Menu\\GNSSTerminal.exe"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, NULL, NULL);
		}
	}
	else if( TID_EXIT_PROCESS == nIDEvent )
	{
		KillTimer( nIDEvent );
		OnBtnWince();
	}
	CDialogBase::OnTimer(nIDEvent);
}

void CDlgInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(PtInRect(&CRect(0,0,80,80),point))		//左上角
		SetTimer( TID_SHOW_GSM_AT, 3 * 1000, NULL );
	if(PtInRect(&CRect(0,400,80,480),point))	//左下角
		SetTimer( TID_EXIT_PROCESS, 3 * 1000, NULL );

	
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<4;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}
	
	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}
