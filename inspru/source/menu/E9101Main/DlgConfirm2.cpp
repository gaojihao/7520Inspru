// DlgConfirm2.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgConfirm2.h"
#include "../../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog
BOOL CDlgConfirm2::s_bOk = FALSE;
CString	CDlgConfirm2::m_stTitle = _T("");

CDlgConfirm2::CDlgConfirm2(DWORD dwTimeElapse /*= 5*/, CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgConfirm2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfirm2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwTimeElapse = dwTimeElapse;	//自动关闭窗口时间
}


void CDlgConfirm2::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirm2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfirm2, CDialogBase)
	//{{AFX_MSG_MAP(CDlgConfirm2)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm2 message handlers

BOOL CDlgConfirm2::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();

	MoveWindow(0 , 0, 494, 274);
	CenterWindow(GetDesktopWindow());
	SetTimer(TID_CLOSE_WINDOW, m_dwTimeElapse*1000, NULL);

	return TRUE;
}

BOOL  CDlgConfirm2::OnLanguageChange()
{
	return TRUE;
}

BOOL  CDlgConfirm2::OnSkinChange()
{
	return TRUE;
}

void CDlgConfirm2::OnExit()
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

void CDlgConfirm2::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
             // lpszFacename
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

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_CONFIRM2_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_CONFIRM2_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    
 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}


void CDlgConfirm2::InitControl()
{
	for(int i=0;i<3;i++)
		m_Item[i].nState = BTN_STATE_NORMAL;

	//确定
	m_Item[0].rect.left		= 0;
	m_Item[0].rect.top		= 0;
	m_Item[0].rect.right	= 0;
	m_Item[0].rect.bottom	= 0;
	//取消
	m_Item[1].rect.left		= 0;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 0;
	m_Item[1].rect.bottom	= 0;
//EXIT
	m_Item[2].rect.left		= 433;
	m_Item[2].rect.top		= 0;
	m_Item[2].rect.right	= 494;
	m_Item[2].rect.bottom	= 53;
}

void CDlgConfirm2::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgConfirm2::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgConfirm2::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	//绘制背景
	for(int i=0;i<3;i++)
	{
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	DrawBtnText(&memDC,CRect(0,80,494,274),m_stTitle,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_CENTER|DT_WORDBREAK);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgConfirm2::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i =0;i<3;i++)
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

void CDlgConfirm2::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i =0;i<3;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;

			switch(i)
			{
			case 0:
				s_bOk = TRUE;
				OnExit();
				break;
			case 1:
				s_bOk = FALSE;
				OnExit();
				break;
			case 2:
				CDlgConfirm2::s_bOk = FALSE;
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

LRESULT CDlgConfirm2::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			CDlgConfirm2::s_bOk = FALSE;
			OnExit();
		}
 	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgConfirm2::OnTimer(UINT_PTR nIDEvent)
{
	if(TID_CLOSE_WINDOW == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnExit();
	}
	CDialogBase::OnTimer(nIDEvent);
}
