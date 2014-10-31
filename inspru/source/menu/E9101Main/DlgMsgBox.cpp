// BrightnessSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgMsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CDlgMsgBox::m_strTextInfo;
UINT CDlgMsgBox::WM_DlgMsgBox_Refresh = RegisterWindowMessage(_T("WM_DlgMsgBox_Refresh"));

CDlgMsgBox::CDlgMsgBox(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgMsgBox::IDD, pParent)
{
}

void CDlgMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMsgBox, CDialogBase)
	//{{AFX_MSG_MAP(CBrightnessSetDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgMsgBox::OnInitDialog()
{
	CDialogBase::OnInitDialog();
	MoveWindow(0, 0, 494, 274);
	CenterWindow();

	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();

	SetWindowText(_T("DlgMsgBox_WINDOW_NAME"));

	SetTimer(TID_TIME_OUT, 8*1000, NULL);

	return TRUE;
}

BOOL CDlgMsgBox::OnLanguageChange()
{
	CResString str;

	return TRUE;
}

//变皮肤
BOOL CDlgMsgBox::OnSkinChange()
{
	return TRUE;
}

void CDlgMsgBox::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

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

void CDlgMsgBox::InitControl()
{
	for (int i=0;  i < CTR_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	//提示信息
	m_Item[0].rect.top		= 0;
	m_Item[0].rect.left		= 0;
	m_Item[0].rect.bottom	= 0;
	m_Item[0].rect.right	= 0;

	//退出按钮
	m_Item[1].rect.left		= 433;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 494;
	m_Item[1].rect.bottom	= 53;
}

void CDlgMsgBox::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgMsgBox::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgMsgBox::OnExit()
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

	CDialogBase::EndDialog(0);
}

void CDlgMsgBox::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	//提示信息
	//DrawBtnText(&memDC, m_Item[0].rect, m_strTextInfo, bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER|DT_WORDBREAK);
	DrawBtnText(&memDC,CRect(0,0,494,274),m_strTextInfo,CGDICommon::Instance()->bigbigfont(),RGB(255,255,255),DT_VCENTER|DT_CENTER);
	//退出按钮
	DrawItems(&memDC, m_Item[1].rect, m_Item[1].nState);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgMsgBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=1;i<CTR_COUNT;i++)
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
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgMsgBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0; i<CTR_COUNT; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:
			case 1:
				OnExit();
				break;
			default:
				break;
			}
		}
	}

	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CDlgMsgBox::OnTimer(UINT nIDEvent) 
{
	if(TID_TIME_OUT == nIDEvent)
	{
		m_strTextInfo	= _T("发送失败!");
		KillTimer(nIDEvent);
		Invalidate();
		SetTimer(TID_EXIT, 2*1000, NULL);
	}
	else if(TID_EXIT == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnExit();
	}

	CDialogBase::OnTimer(nIDEvent);
}

LRESULT CDlgMsgBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_DlgMsgBox_Refresh)
	{
		KillTimer(TID_TIME_OUT);
		Invalidate();
		SetTimer(TID_EXIT, 2*1000, NULL);
	}

	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgMsgBox::ReleaseGdi()
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

void CDlgMsgBox::SetTextInfo(CString strInfo)
{
	m_strTextInfo = strInfo;
}