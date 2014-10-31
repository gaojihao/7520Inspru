// DlgPair.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgDialNumber.h"
#include "E9101MainDlg.h"
#include "HBCP.h"
#include "command.h"
#include "../imm/InputShl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hDialIn;
extern CDlgDialNumber* g_DialNum;
/////////////////////////////////////////////////////////////////////////////
// CDlgPair dialog

CDlgDialNumber::CDlgDialNumber(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgDialNumber::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDialNumber)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDialNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDialNumber)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDialNumber, CDialogBase)
	//{{AFX_MSG_MAP(CDlgDialNumber)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPair message handlers

BOOL CDlgDialNumber::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	MoveWindow(0, 237, 468, 272);
	//SetActiveWindow();
	//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 237, 480, 272, SWP_SHOWWINDOW);	
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL  CDlgDialNumber::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_BTN_BT_PAIR);
	m_stTitle = str;

	str.LoadString(RES_BTN_BT_PAIRINFO);
	m_stInfo = str;

	return TRUE;
}

BOOL  CDlgDialNumber::OnSkinChange()
{
	return TRUE;
}

void CDlgDialNumber::OnExit()
{
//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

//	DeleteObject(m_stBtnDisBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

//	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::DestroyWindow();
	g_DialNum = NULL;
	//CDialogBase::EndDialog(0);
}

void CDlgDialNumber::InitGdi()
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
// 		smallfont.CreateFont(
// 		-18,					    // nHeight
// 		0,                         // nWidth
// 		0,                         // nEscapement
// 		0,                         // nOrientation
// 		FW_SEMIBOLD,                 // nWeight
// 		FALSE,                     // bItalic
// 		FALSE,                     // bUnderline
// 		0,                         // cStrikeOut
// 		ANSI_CHARSET,              // nCharSet
// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 		ANTIALIASED_QUALITY,           // nQuality
// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 		_T("ËÎÌו\0"));             // lpszFacename
// 
// 		bigfont.CreateFont(
// 		45,					    // nHeight
// 		20,                         // nWidth
// 		0,                         // nEscapement
// 		0,                         // nOrientation
// 		FW_SEMIBOLD,                 // nWeight
// 		FALSE,                     // bItalic
// 		FALSE,                     // bUnderline
// 		0,                         // cStrikeOut
// 		ANSI_CHARSET,              // nCharSet
// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 		ANTIALIASED_QUALITY,           // nQuality
// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 		_T("ËÎÌו\0")); 
// 
// 		midfont.CreateFont(
// 		32,					    // nHeight
// 		12,                         // nWidth
// 		0,                         // nEscapement
// 		0,                         // nOrientation
// 		FW_SEMIBOLD,                 // nWeight
// 		FALSE,                     // bItalic
// 		FALSE,                     // bUnderline
// 		0,                         // cStrikeOut
// 		ANSI_CHARSET,              // nCharSet
// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 		ANTIALIASED_QUALITY,           // nQuality
// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 		_T("ËÎÌו\0")); 

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_DIAL_NUMBER_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_BG_DIAL_NUMBER_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    
 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}


void CDlgDialNumber::InitControl()
{

// 	m_Item.nState = BTN_STATE_NORMAL;
// 
// 	m_Item.rect.top = 0;
// 	m_Item.rect.left = 430;
// 	m_Item.rect.bottom = 40;
//	m_Item.rect.right = 480;
	for (int i =0;  i < 13; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
		RECT rt;
		rt.left  = i*36;
		rt.top   = 0;
		rt.right = rt.left + 36;
		rt.bottom = rt.top + 36;
		m_Item[i].rect = rt;
		
	}

}

void CDlgDialNumber::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgDialNumber::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgDialNumber::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<13;i++)
	{
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

//	DrawBtnText(&memDC,g_retTitile,m_stTitle,midfont,RGB(255,255,255),DT_LEFT|DT_TOP);

// 	Rect.top = 190;
// 	Rect.left = 15;
// 	Rect.bottom = 215;
// 	Rect.right = 465;
// 	DrawBtnText(&memDC,Rect,m_stInfo,smallfont,RGB(255,255,255));
// 
// 	Rect.top = 80;
// 	Rect.left = 200;
// 	Rect.bottom = 120;
// 	Rect.right = 320;
// 
//	DrawBtnText(&memDC,Rect,m_stTimer,bigfont,RGB(255,255,255),DT_VCENTER | DT_TOP);

//    	for(int jj=0;jj<13;jj++)
//      	{
//       		Rect = m_Item[jj].rect;
//       		memDC.FillSolidRect(&Rect, RGB(0, 255-jj*10, 0));
//		}

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CDlgDialNumber::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	for(int i=0;i<13;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				PlayKeySound();
				m_Item[i].nState = BTN_STATE_DOWN;

			}
	}
//	if(PtInRect(CRect(0, 0, 80, 40),point))
//	{
//		EndDialog(0);
		//PlayKeySound();
		//OnBtnTxtlistBack();
//	}

	Invalidate();
	SetCapture();
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgDialNumber::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<13;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
				switch(i)
				{
				case 0:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 1, 0);
					break;
				case 1:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 2, 0);
					break;
				case 2:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 3, 0);
					break;
				case 3:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 4, 0);
					break;
				case 4:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 5, 0);
					break;
				case 5:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 6, 0);
					break;
				case 6:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 7, 0);
					break;
				case 7:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 8, 0);
					break;
				case 8:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 9, 0);
					break;
				case 9:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 0, 0);				
					break;
				case 10:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 10, 0);
					break;	
				case 11:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 11, 0);
					break;	
				case 12:
					::SendMessage(g_hDialIn, WM_IMM_DIAL_NUMBER_MESSAGE, 12, 0);
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


void CDlgDialNumber::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialogBase::OnTimer(nIDEvent);
}

void CDlgDialNumber::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogBase::PostNcDestroy();
	delete this;
}

