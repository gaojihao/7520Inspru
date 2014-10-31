// NaviSet.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "NaviSet.h"
#include "DlgNaviBro.h"

#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNaviSet dialog


CNaviSet::CNaviSet(CWnd* pParent /*=NULL*/)
	: CDialogBase(CNaviSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNaviSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNaviSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNaviSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNaviSet, CDialogBase)
	//{{AFX_MSG_MAP(CNaviSet)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNaviSet message handlers

BOOL CNaviSet::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_curBro = 0;

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNaviSet::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_BUTTON_NAVIAUTO);
	m_csAutoStart = str;

	str.LoadString(RES_BTN_NAVI_SET);
	m_csTitile = str;

	str.LoadString(RES_STA_NAVI_PATH);
	m_csNaviPath = str;
	
	str.LoadString(RES_STA_APP_PATH);
	m_csAppPath = str;

	return TRUE;
}

//变皮肤
BOOL CNaviSet::OnSkinChange()
{
	return TRUE;
}

void CNaviSet::InitGdi()
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
//	bk_press_dc.CreateCompatibleDC(m_pDC);
//	bk_sigsel_dc.CreateCompatibleDC(m_pDC);

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_NAVISET_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_BG_NAVISET_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 

//	pSkin = g_pResMng->RequestBmp(BMP_BG_SIGSEL_N, true);
//	m_stBtnSignSel = pSkin->hBitmap; 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
//	m_pold_bk_signel_bmp   = bk_sigsel_dc.SelectObject(CBitmap::FromHandle(m_stBtnSignSel));

	::ReleaseDC(m_hWnd, hDC);
}

void CNaviSet::OnExit()
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

	CDialogBase::EndDialog(0);
}

void CNaviSet::InitControl()
{	
	int i = 0;
	for ( i=0;  i < 2; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	m_ItemApp[1].nState = BTN_STATE_NORMAL;

	if( TRUE == CSysConfig::Instance().GetAutoNavi() )
		m_ItemSig.nState = BTN_STATE_DOWN;
	else
		m_ItemSig.nState = BTN_STATE_NORMAL;

	m_NaviPath.nState = BTN_STATE_DISABLE;

	//choose mark
	m_ItemSig.rect.left   = 189;
 	m_ItemSig.rect.top    = 96;
 	m_ItemSig.rect.right  = 271;
 	m_ItemSig.rect.bottom = 179;

	//Choose the directory of Navi.
 	m_Item[0].rect.left   = 586;
 	m_Item[0].rect.top    = 226;
 	m_Item[0].rect.right  = 728;
 	m_Item[0].rect.bottom = 293;

	//Choose the directory of application.
	m_ItemApp[1].rect.left   = 586;
 	m_ItemApp[1].rect.top    = 331;
 	m_ItemApp[1].rect.right  = 728;
 	m_ItemApp[1].rect.bottom = 393;

	//Close button.
	m_Item[1].rect.left		= 740;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 800;
	m_Item[1].rect.bottom	= 60;

	//Show the path of Navi.
	m_NaviPath.rect.left   = 86;
	m_NaviPath.rect.top    = 241;
	m_NaviPath.rect.right  = 573;
	m_NaviPath.rect.bottom = 278;

	//Show the path of application.
	m_ItemApp[0].rect.left   = 86;
 	m_ItemApp[0].rect.top    = 344;
 	m_ItemApp[0].rect.right  = 573;
 	m_ItemApp[0].rect.bottom = 382;

	m_NaviPath.chChar = CSysConfig::Instance().GetNaviPath();
	m_ItemApp[0].chChar  = CSysConfig::Instance().GetAppPath();
}

void CNaviSet::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CNaviSet::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CNaviSet::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<2;i++)
	{
		//绘制背景
		//推出button和Navi的选择button
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}
	//绘制应用程序的选择button
	str   = m_ItemApp[1].chChar;
	Rect  = m_ItemApp[1].rect;
	state = m_ItemApp[1].nState;
	DrawItems(&memDC,Rect,state);

	//Navi的开机启动选项
	Rect = m_ItemSig.rect;
	if( BTN_STATE_DOWN == m_ItemSig.nState )
		DrawItems(&memDC,Rect,BTN_STATE_DOWN);
	else 
		DrawItems(&memDC,Rect,BTN_STATE_NORMAL);

	//Navi的路径显示
	str  = m_NaviPath.chChar;
	Rect = m_NaviPath.rect;
	DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT | DT_VCENTER);
	//Navi的路径显示
	str  = m_ItemApp[0].chChar;
	Rect = m_ItemApp[0].rect;
	DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT | DT_VCENTER);

	//标题栏显示
	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_CENTER | DT_VCENTER);

	//文字说明
	//Navi auto start
	Rect       = m_ItemSig.rect;
	Rect.left  = m_ItemSig.rect.right + 15;
	Rect.right = m_ItemSig.rect.right + 450;
	DrawBtnText(&memDC,Rect,m_csAutoStart,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT | DT_VCENTER );
	//Navi path select
	Rect        = m_NaviPath.rect;
	Rect.top    = m_NaviPath.rect.top - 40;
	Rect.bottom = m_NaviPath.rect.top - 15;
	//Rect.left   = m_NaviPath.rect.left + 70;
	DrawBtnText(&memDC,Rect,m_csNaviPath,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT | DT_TOP );
	//application path select
	Rect        = m_ItemApp[0].rect;
	Rect.top    = m_ItemApp[0].rect.top - 40;
	Rect.bottom = m_ItemApp[0].rect.top - 15;
	//Rect.left   = m_ItemApp[0].rect.left + 70;
	DrawBtnText(&memDC,Rect,m_csAppPath,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT | DT_TOP );

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CNaviSet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<2;i++)
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

	//Choose the directory of appliction 
	if(PtInRect(&m_ItemApp[1].rect,point))
	{
		if(m_ItemApp[1].nState == BTN_STATE_NORMAL)
		{
			m_ItemApp[1].nState = BTN_STATE_DOWN;
			PlayKeySound();
		}
	}
	if(PtInRect(&m_ItemSig.rect,point))
	{
			PlayKeySound();
	}

	Invalidate();
	SetCapture();

	CDialogBase::OnLButtonDown(nFlags, point);
}

void CNaviSet::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<2;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:
				{
					CDlgNaviBro dlg; //TRUE: Navi
					dlg.SetType( true );
					dlg.DoModal();
					m_NaviPath.chChar = CSysConfig::Instance().GetNaviPath();
				}
				break;
			case 1:
				OnExit();
				break;
			default:
				break;
			}					
		}		
	}

	//Choose the directory of appliction 
	if(m_ItemApp[1].nState==BTN_STATE_DOWN)
	{
		m_ItemApp[1].nState = BTN_STATE_NORMAL;
		CDlgNaviBro dlg; //TRUE: appliction
		dlg.SetType( false );
		dlg.DoModal();
		m_ItemApp[0].chChar = CSysConfig::Instance().GetAppPath();
	}

	//Navi auto start
	if(PtInRect(&m_ItemSig.rect,point))
	{
		if( m_ItemSig.nState == BTN_STATE_DOWN )
		{
			m_ItemSig.nState = BTN_STATE_NORMAL;
			CSysConfig::Instance().SetAutoNavi(false);
		}
		else if( m_ItemSig.nState == BTN_STATE_NORMAL )
		{
			m_ItemSig.nState = BTN_STATE_DOWN;
			CSysConfig::Instance().SetAutoNavi(true);
		}
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}





LRESULT CNaviSet::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnExit();
		}
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
