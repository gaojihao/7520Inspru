// MediaMain.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"

#include "DlgMediaMain.h"
#include "DlgGame.h"

#include "../../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMediaMain dialog
extern CString g_csPicName;
extern CString g_csPicIconName;

extern CString g_csMusicName;
extern CString g_csMusicIconName;

extern CString g_csMovieName;
extern CString g_csMovieIconName;

extern CString g_csTxtName;
extern CString g_csTxtIconName;
extern BOOL	g_bAppStatus;

CDlgMediaMain::CDlgMediaMain(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgMediaMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMediaMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMediaMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMediaMain, CDialogBase)
	//{{AFX_MSG_MAP(CDlgMediaMain)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMediaMain message handlers

BOOL CDlgMediaMain::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	SetWindowText(TEXT("Menu_Entertainment"));

	InitGdi();
	InitControl();

	m_bBtnPress = false;
	
	OnLanguageChange();
	OnSkinChange();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//变换语言和单位
BOOL CDlgMediaMain::OnLanguageChange()//变皮肤
{
	CResString str;
	
	str.LoadString(RES_BUTTON_MEDIA);
	m_csTitile = str;
	
	str.LoadString(RES_BUTTON_MOVIE);
	m_Item[0].chChar = str;
	
	str.LoadString(RES_BUTTON_MUSIC);
	m_Item[1].chChar = str;
	
	str.LoadString(RES_BUTTON_PICTURE);
	m_Item[2].chChar = str;
	
	str.LoadString(RES_BUTTON_EBOOK);
	m_Item[3].chChar = str;
	
	//Game.
	str.LoadString(RES_BUTTON_GAMES);
	m_Item[4].chChar = str;

	m_Item[5].chChar = _T("收音机");

	return TRUE;
}

BOOL CDlgMediaMain::OnSkinChange()
{
	// 	SkinSetDlgStyle(KDS_CTRL_FKTEXT|KDS_BTN_AUTOBMP);
	// 	SkinSetTitlePos(CPoint(200,6));
	// 	SkinSetResID(bmp_bg_language);                                                                                                                                                                                                                                                                                           
	// 
	// 	m_Btn12.SetBtnResID(bmp_file_SetChild_12);
	// 	m_Btn24.SetBtnResID(bmp_file_SetChild_24);
	// 
	// 	enTimeUnit timeFmt = CSysConfig::Instance().GetTimeUnit();
	// 	if(timeFmt == ID_12_TYPE)
	//	{
	
	//		m_Btn24.EnableWindow(TRUE);
	//		m_Btn12.EnableWindow(FALSE);
	
	//	}	
	//	else if(timeFmt == ID_24_TYPE)
	//	{
	//		m_Btn24.EnableWindow(FALSE);
	//		m_Btn12.EnableWindow(TRUE);
	//	}
	
	//	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
	
	
	
	// 	m_BtnExit.SetBtnStyle(KDS_BTN_AUTOBMP |KDS_TEXT_VCENTER | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT)
	//		.SetBtnResID(bmp_file_back);
	
	return TRUE;
}

void CDlgMediaMain::InitGdi()
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
		_T("宋体\0")); 
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_MEDIA_MAIN_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_MEDIA_MAIN_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgMediaMain::ReleaseGdi()
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

void CDlgMediaMain::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CDlgMediaMain::InitControl()
{
	int i;
 	for ( i=0;  i < BTN_COUNT; i++)
 	{
 		m_Item[i].nState = BTN_STATE_NORMAL;
 	}
	//视频播放
	m_Item[0].rect.left		= 197;
	m_Item[0].rect.top		= 87;
	m_Item[0].rect.right	= 318;
	m_Item[0].rect.bottom	= 206;
	//音乐播放
	m_Item[1].rect.left		= 337;
	m_Item[1].rect.top		= 87;
	m_Item[1].rect.right	= 463;
	m_Item[1].rect.bottom	= 206;
	//图片浏览
	m_Item[2].rect.left		= 479;
	m_Item[2].rect.top		= 87;
	m_Item[2].rect.right	= 608;
	m_Item[2].rect.bottom	= 206;
	//书籍阅读
	m_Item[3].rect.left		= 263;
	m_Item[3].rect.top		= 264;
	m_Item[3].rect.right	= 392;
	m_Item[3].rect.bottom	= 391;
	//游戏
	m_Item[4].rect.left		= 405;
	m_Item[4].rect.top		= 264;
	m_Item[4].rect.right	= 539;
	m_Item[4].rect.bottom	= 391;
	//收音机
	m_Item[5].rect.left		= 0;
	m_Item[5].rect.top		= 0;
	m_Item[5].rect.right	= 0;
	m_Item[5].rect.bottom	= 0;

	//Exit.
	m_Item[6].rect.left		= 740;
	m_Item[6].rect.top		= 0;
	m_Item[6].rect.right	= 800;
	m_Item[6].rect.bottom	= 60;
}

void CDlgMediaMain::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CDlgMediaMain::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgMediaMain::OnPaint() 
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
		Rect.bottom += 25;
		DrawBtnText(&memDC,Rect,str, CGDICommon::Instance()->smallfont(),RGB(255,255,255),DT_BOTTOM|DT_CENTER);
	}
	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgMediaMain::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bBtnPress)
		return;
	
	for(int i=0;i<BTN_COUNT;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			m_bBtnPress = true;
			SetTimer(1234, 3 * 1000, 0);

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

void CDlgMediaMain::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<BTN_COUNT;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:	
				OnBtnMovie();
				break;
			case 1:	
				OnBtnMusic();
				break;
			case 2:	
				OnBtnPicture();
				break;
			case 3: 
				OnBtnText();
				break;
			case 4: 
				OnBtnGame();
				break;
			case 5: 
				OnBtnFM();
				break;
			case 6: 
				OnExit();
				break;
			default:
				break;
			}
		}
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}


VOID  CDlgMediaMain::OnBtnMovie()
{
	DWORD dwMainThreadId = GetCurrentThreadId();
	
	TCHAR bCmdline[100];
	BOOL m_bIsSpare = CSysConfig::Instance().GetIsSpareMode();
	DWORD IsSpare =(DWORD)m_bIsSpare;
	wsprintf(bCmdline, _T("%lu %d"), dwMainThreadId, IsSpare);
	LPCWSTR srCmdLine = bCmdline;
	
	g_bAppStatus = CreateProcess(g_csMovieName, srCmdLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);
		
 	if(g_bAppStatus)
	{
		CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
	}
}

VOID  CDlgMediaMain::OnBtnMusic()
{
	DWORD dwMainThreadId = GetCurrentThreadId();
	
	TCHAR bCmdline[100];
	BOOL m_bIsSpare = CSysConfig::Instance().GetIsSpareMode();
	DWORD IsSpare =(DWORD)m_bIsSpare;
	wsprintf(bCmdline, _T("%lu %d"), dwMainThreadId, IsSpare);
	LPCWSTR srCmdLine = bCmdline;
	
	CreateProcess(g_csMusicName, srCmdLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);
}

VOID  CDlgMediaMain::OnBtnPicture()
{
	DWORD dwMainThreadId = GetCurrentThreadId();
	
	TCHAR bCmdline[100];
	BOOL m_bIsSpare = CSysConfig::Instance().GetIsSpareMode();
	DWORD IsSpare =(DWORD)m_bIsSpare;
	wsprintf(bCmdline, _T("%lu %d"), dwMainThreadId, IsSpare);
	LPCWSTR srCmdLine = bCmdline;
	
    CreateProcess(g_csPicName, srCmdLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);
}

VOID  CDlgMediaMain::OnBtnText()
{
	DWORD dwMainThreadId = GetCurrentThreadId();
	
	TCHAR bCmdline[100];
	BOOL m_bIsSpare = CSysConfig::Instance().GetIsSpareMode();
	DWORD IsSpare =(DWORD)m_bIsSpare;
	wsprintf(bCmdline, _T("%lu %d"), dwMainThreadId, IsSpare);
	LPCWSTR srCmdLine = bCmdline;
	
    CreateProcess(g_csTxtName, srCmdLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);
}

VOID CDlgMediaMain::OnBtnGame()
{
	g_bAppStatus = CreateProcess(_T("\\Flashdrv Storage\\Game\\tanke\\TANK.EXE"), 
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);

	if(g_bAppStatus)
	{
		CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
	}
	return;

	CDlgGame dlg;
	dlg.DoModal();
}

VOID CDlgMediaMain::OnBtnFM()
{
	return;
}

LRESULT CDlgMediaMain::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgMediaMain::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(1234 == nIDEvent)
	{
		KillTimer(1234);
		m_bBtnPress = false;
	}
	
	CDialogBase::OnTimer(nIDEvent);
}
