// SysSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "E9101Main.h"
#include "SysSetDlg.h"
#include "LanguageSetDlg.h"
#include "SkinSetDlg.h"
#include "SysConfig.h"
#include "E9101MainDlg.h"
#include "BrightnessSetDlg.h"
#include "TimeSetDlg.h"
//#include "../DriveLib/ak4642api.h"
//#include "DlgVirtion.h"
#include "DlgInfo.h"
#include "DlgGpsStar.h"
#include "DlgSyesetKeyVol.h"
#include "NaviSet.h"

#include "Dialog/DlgVehicleInfo.h"
#include "Dialog/DlgPushToTalkSetting.h"
#include "../../MutiLanguage/CGDICommon.h"
#include "E9101MainDlg.h"
#include "DlgConfirm2.h"

#include "HBCP.h"

#include "PWDInputDlg.h"
#include "NumSetDlg.h"
#include "IPSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSetDlg dialog

enLanguage CSysSetDlg::S_CurLanguage = ID_ENGLISH;

extern unsigned char g_power_status;

CSysSetDlg::CSysSetDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSysSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysSetDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



void CSysSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSetDlg)
	DDX_Control(pDX, IDC_BTN_TIME_SET, m_BtnTimeSet);
	DDX_Control(pDX, IDC_BTN_SKIN_SET, m_BtnSkin);
	DDX_Control(pDX, IDC_BTN_LIGHTENESS_SET, m_BtnAdjust);
	DDX_Control(pDX, IDC_BTN_LANGUAGE_SET, m_BtnLanguage);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BtnExit);
	DDX_Control(pDX, IDC_BTN_DEFAULT_SET, m_BtnDefault);
	
	DDX_Control(pDX, IDC_BTN_SYSSET_12, m_Btn12);
	DDX_Control(pDX, IDC_BTN_SYSSET_TIMEZONE, m_BtnTimeZone);
	DDX_Control(pDX, IDC_BTN_SYSSET_CLOCK, m_BtnClock);
	DDX_Control(pDX, IDC_BTN_SYSSET_FOR, m_BtnForword);
	DDX_Control(pDX, IDC_BTN_SYSSET_BACK, m_BtnBack);
	DDX_Control(pDX, IDC_BTN_SYSSET_BK_LIGHT, m_BtnBkLight);
	DDX_Control(pDX, IDC_BTN_SYSSET_BT, m_BtnBT);
	DDX_Control(pDX, IDC_BTN_SYASET_CALIBR, m_BtnCalibr);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSetDlg, CDialogBase)
//{{AFX_MSG_MAP(CSysSetDlg)
ON_BN_CLICKED(IDC_BTN_ADJUST, OnBtnSysInfo)
ON_BN_CLICKED(IDC_BTN_TIME_SET, OnBtnTimeSet)
ON_BN_CLICKED(IDC_BTN_LANGUAGE_SET, OnBtnLanguageSet)
ON_BN_CLICKED(IDC_BTN_SKIN_SET, OnBtnSkinSet)
ON_BN_CLICKED(IDC_BTN_DEFAULT_SET, OnBtnDefaultSet)
ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
ON_BN_CLICKED(IDC_BTN_SYSSET_12, OnBtnSysset12)
ON_BN_CLICKED(IDC_BTN_SYSSET_TIMEZONE, OnBtnSyssetTimezone)
ON_BN_CLICKED(IDC_BTN_SYSSET_FOR, OnBtnSyssetFor)
ON_BN_CLICKED(IDC_BTN_SYSSET_BACK, OnBtnSyssetBack)
ON_BN_CLICKED(IDC_BTN_SYSSET_BT, OnBtnSyssetBt)
ON_BN_CLICKED(IDC_BTN_SYSSET_BK_LIGHT, OnBtnSyssetBkLight)
ON_BN_CLICKED(IDC_BTN_SYASET_CALIBR, OnBtnSyasetCalibr)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSysSetDlg::OnLanguageChange()
{
	m_stTitle = _T("系统设置");
	m_Item[0].chChar = _T("路径设置");
	m_Item[1].chChar = _T("背光设置");
	m_Item[2].chChar = _T("按键声音");
	m_Item[3].chChar = _T("一键通设置");
	m_Item[4].chChar = _T("星历信息");
	m_Item[5].chChar = _T("IP设置");
	m_Item[6].chChar = _T("系统信息");
	m_Item[7].chChar = _T("");
	return TRUE;
}

//变皮肤
BOOL CSysSetDlg::OnSkinChange()
{
	// 	SkinSetDlgStyle(KDS_CTRL_FKTEXT|KDS_BTN_AUTOBMP);
	// 	SkinSetTitlePos(CPoint(200,6));
	// 	SkinSetResID(bmp_bg_mainmenu);
	// 
	// 
	// 	m_Btn12.SetBtnResID(bmp_file_SysSet_Time_12_24_SetBtn);
	// 	m_BtnTimeZone.SetBtnResID(bmp_file_SysSet_TimeZone);
	// 	m_BtnClock.SetBtnResID(bmp_file_SysSet_Clock);
	// 
	// 	m_BtnTimeSet.SetBtnResID(bmp_file_SysSet_TimeSetBtn);
	// 	m_BtnSkin.SetBtnResID(bmp_file_SysSet_Skin);
	// 	m_BtnAdjust.SetBtnResID(bmp_file_SysSet_info);
	// 	m_BtnCalibr.SetBtnResID(bmp_file_SysSet_Calibrate);
	// 	m_BtnLanguage.SetBtnResID(bmp_file_SysSet_Language);
	// 	m_BtnExit.SetBtnResID(bmp_file_back);
	// 	m_BtnDefault.SetBtnResID(bmp_file_SysSet_Default);
	// 	m_BtnForword.SetBtnResID(bmp_file_SysSet_foword);
	// 
	// 	m_BtnBkLight.SetBtnResID(bmp_file_SysSet_close_screen);
	// 	m_BtnBT.SetBtnResID(bmp_file_SysSet_bluetooth);
	// 	m_BtnBack.SetBtnResID(bmp_file_SysSet_back);
	// 
	// 
	// 	m_Btn12.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnTimeZone.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnClock.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnAdjust.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnCalibr.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnTimeSet.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnLanguage.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnSkin.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnDefault.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnForword.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnBkLight.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	// 	m_BtnBT.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
	//	m_BtnBack.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
		
	return TRUE;
}

BOOL CSysSetDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();
	
	
	// 	m_BtnTimeSet.SetWindowPos(&CWnd::wndTop,0,34,0,0,NULL);
	// 	m_BtnTimeSet.SettextLeft(80);
	// 	m_Btn12.SetWindowPos(&CWnd::wndTop,120,34,0,0,NULL);
	// 	m_Btn12.SettextLeft(80);
	// 	m_BtnTimeZone.SetWindowPos(&CWnd::wndTop,240,34,0,0,NULL);
	// 	m_BtnTimeZone.SettextLeft(80);
	// 	m_BtnClock.SetWindowPos(&CWnd::wndTop,360,34,0,0,NULL);
	// 	m_BtnClock.SettextLeft(80);
	// 
	// 	
	// 	m_BtnBT.SetWindowPos(&CWnd::wndTop,0,150,0,0,NULL);
	// 	m_BtnLanguage.SetWindowPos(&CWnd::wndTop,120,150,0,0,NULL);	
	// 	m_BtnSkin.SetWindowPos(&CWnd::wndTop,240,150,0,0,NULL);
	// 	m_BtnForword.SetWindowPos(&CWnd::wndTop,360,150,0,0,NULL);
	// 
	// 
	// 	m_BtnAdjust.SetWindowPos(&CWnd::wndTop,360,34,0,0,NULL);
	// 	m_BtnAdjust.ShowWindow(SW_HIDE);
	// 	m_BtnBkLight.SetWindowPos(&CWnd::wndTop,0,150,0,0,NULL);
	// 	m_BtnBkLight.ShowWindow(SW_HIDE);
	// 	m_BtnDefault.SetWindowPos(&CWnd::wndTop,120,150,0,0,NULL);
	// 	m_BtnDefault.ShowWindow(SW_HIDE);
	// 	m_BtnCalibr.SetWindowPos(&CWnd::wndTop,240,150,0,0,NULL);
	// 	m_BtnCalibr.ShowWindow(SW_HIDE);
	// 	m_BtnBack.SetWindowPos(&CWnd::wndTop,360,150,0,0,NULL);
	// 	m_BtnBack.ShowWindow(SW_HIDE);
	// 
	//	m_BtnExit.SetWindowPos(&CWnd::wndTop,0,0,0,0,NULL);
	
	
	InitGdi();
	InitControl();
	OnSkinChange();
	OnLanguageChange();
	
	
	
	return TRUE;
}


void CSysSetDlg::OnBtnSysInfo() 
{
	CDlgInfo dlg;
	dlg.DoModal();
}

void CSysSetDlg::OnBtnTimeSet() 
{
	// TODO: Add your control notification handler code here
	
	CTimeSetDlg dlg;
	dlg.DoModal();
	
}

void CSysSetDlg::OnBtnLanguageSet() 
{
	// TODO: Add your control notification handler code here
	// 	static BOOL g_Skin = TRUE;
	// 	if( g_Skin )
	// 	{
	// 		CSysConfig::Instance().SetSkinType( ID_SKIN2 );
	// 		g_Skin = FALSE;
	// 	}
	// 	else
	// 	{
	// 		CSysConfig::Instance().SetSkinType( ID_SKIN1 );
	// 		g_Skin = TRUE;
	//	}
	CLanguageSetDlg dlg;
	dlg.DoModal();
}
//设置系统时间
void CSysSetDlg::OnBtnSkinSet() 
{
	// TODO: Add your control notification handler code here
	
//	CSkinSetDlg dlg;
//	dlg.DoModal();
	
}



void CSysSetDlg::OnBtnDefaultSet() 
{
	if( !CE9101MainDlg::m_bCanStartNavi )
	{
		CDlgConfirm2::m_stTitle = _T("GPS模块初始化中,请稍后再启动!");
		CDlgConfirm2 dlg(3);
		dlg.DoModal();
		return;
	}
	DlgGpsStar dlg;
	dlg.DoModal();
}

void CSysSetDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	OnExit();
}




void CSysSetDlg::OnBtnSysset12() 
{	
}

void CSysSetDlg::OnBtnSyssetTimezone() 
{	
}

void CSysSetDlg::OnBtnBrigtness() 
{
	CBrightnessSetDlg dlg;
	dlg.DoModal();
}

LRESULT CSysSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WM_SKIN_CHANGE:
		{
			ReleaseGdi();
			InitGdi();
		}
		break;
	default:
		break;
	}
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CSysSetDlg::OnBtnSyssetFor() 
{
}

void CSysSetDlg::OnBtnSyssetBack()
{
}

void CSysSetDlg::OnBtnSyssetBt()
{
	CResString str;
	CE9101MainDlg* pMainDlg = (CE9101MainDlg*)(AfxGetApp()->m_pMainWnd);
	
	if( CSysConfig::Instance().GetBtSwitch() )
	{
		str.LoadString(RES_BTN_BT_CLOSE);	
		CSysConfig::Instance().SetBtSwitch( false );
		//ioc_btpwr( 0 );
		
		//		pMainDlg->m_stcBlueTooth.SkinSetStcID(STC_BMP, bmp_BlueToothstate_Disconnected, bmp_bg_mainmenu);
	}
	else
	{
		str.LoadString(RES_BTN_BT_OPEN);
		CSysConfig::Instance().SetBtSwitch( true );
		
		//ioc_btpwr( 1 );
		//HBCP_POWER_ON_OFF();
		//Sleep(2000);
		//if (g_power_status == HBCP_STATUS_POWER_OFF)
		//{
		//	HBCP_POWER_ON_OFF();
		//}
		
		//		pMainDlg->m_stcBlueTooth.SkinSetStcID(STC_BMP, bmp_BlueToothState_Connecting, bmp_bg_mainmenu);
	}	
	
	CE9101MainDlg::g_Bluetooth_state = FALSE;
	
	m_BtnBT.SetWindowText(str);
	m_BtnBT.Invalidate();
}

void CSysSetDlg::OnBtnSyssetBkLight() 
{
	// TODO: Add your control notification handler code here
	CResString str;
	
	if( CSysConfig::Instance().GetAutoBkLight() )
	{
		str.LoadString(RES_BTN_SRC_CLOSE);
		CSysConfig::Instance().SetAutoBkLight( false );
		::KillTimer(0, CE9101MainApp::s_SpareTimer);
	}
	else
	{
		str.LoadString(RES_BTN_SRC_OPEN);
		CSysConfig::Instance().SetAutoBkLight( true );
	}
	m_BtnBkLight.SetWindowText(str);
	m_BtnBkLight.Invalidate();
}

void CSysSetDlg::OnBtnSyasetCalibr() 
{
	// TODO: Add your control notification handler code here
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

void CSysSetDlg::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//
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
		_T("宋体\0"));             // lpszFacename
	
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_SET_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_BG_SET_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	::ReleaseDC(m_hWnd, hDC);
}

void CSysSetDlg::ReleaseGdi()
{
	//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	//	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	//	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CSysSetDlg::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}


void CSysSetDlg::InitControl()
{
	int i;
	for (i =0;  i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	//路径设置
	m_Item[0].rect.left		= 126;
	m_Item[0].rect.top		= 87;
	m_Item[0].rect.right	= 246;
	m_Item[0].rect.bottom	= 208;
	//背光设置
	m_Item[1].rect.left		= 267;
	m_Item[1].rect.top		= 87;
	m_Item[1].rect.right	= 389;
	m_Item[1].rect.bottom	= 208;
	//按键声音
	m_Item[2].rect.left		= 408;
	m_Item[2].rect.top		= 87;
	m_Item[2].rect.right	= 533;
	m_Item[2].rect.bottom	= 208;
	//一键通设置
	m_Item[3].rect.left		= 550;
	m_Item[3].rect.top		= 87;
	m_Item[3].rect.right	= 677;
	m_Item[3].rect.bottom	= 208;
	//星历信息
	m_Item[4].rect.left		= 194;
	m_Item[4].rect.top		= 263;
	m_Item[4].rect.right	= 319;
	m_Item[4].rect.bottom	= 388;
	//IP设置
	m_Item[5].rect.left		= 339;
	m_Item[5].rect.top		= 263;
	m_Item[5].rect.right	= 462;
	m_Item[5].rect.bottom	= 388;
	//系统信息
	m_Item[6].rect.left		= 481;
	m_Item[6].rect.top		= 263;
	m_Item[6].rect.right	= 608;
	m_Item[6].rect.bottom	= 388;

	//Exit.
	m_Item[7].rect.left		= 740;
	m_Item[7].rect.top		= 0;
	m_Item[7].rect.right	= 800;
	m_Item[7].rect.bottom	= 60;
}

void CSysSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CSysSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CSysSetDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	
	for(int i=0;i<BTN_COUNT;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.bottom += 24;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}	
	
	DrawBtnText(&memDC,g_retTitile,m_stTitle,CGDICommon::Instance()->bigbigfont(),RGB(255,255,255),DT_CENTER | DT_VCENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);	
}

void CSysSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<BTN_COUNT;i++)
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
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CSysSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<BTN_COUNT;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:
				{
					CNaviSet dlg;	//Navi Path.
					dlg.DoModal();
				}
				break;
			case 1:
				OnBtnBrigtness();	//Back light.
				break;
			case 2:
				OnSyssetKeyVol();	//Key volumn.
				break;
			case 3:
				OnBtnPustToTalk();	//一键通设置
				break;
			case 4:
				OnBtnDefaultSet();	//Star info.
				break;
			case 5:
				OnBtnIPSet();		//IP set.
				break;
			case 6:
				OnBtnSysInfo();		//Syetem info.
				break;
			case 7:
				OnExit();			//Exit.
				break;
			default:
				break;
			}
		}
		
	}
	
	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CSysSetDlg::OnBtnPustToTalk()
{
	CDlgPushToTalkSetting dlg;
	dlg.DoModal();
}

void CSysSetDlg::OnSyssetKeyVol()
{
	CDlgSyesetKeyVol dlg;
	dlg.DoModal();
}

void CSysSetDlg::OnBtnIPSet()
{
	CPWDInputDlg dlg;
	dlg.SetCheckObject(IPSETDLG);
	dlg.DoModal();
}
