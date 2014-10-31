// GSMDialMain.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "GSMDialMain.h"
#include "GSMDial.h"
#include "PhoneBook.h"
#include "PhoneLog.h"
#include "PBookMain.h"
#include "GSMNetSet.h"
#include "SMSMain.h"

#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GSMDialMain dialog



GSMDialMain::GSMDialMain(CWnd* pParent /*=NULL*/)
	: CDialogBase(GSMDialMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(GSMDialMain)
	//}}AFX_DATA_INIT
}




void GSMDialMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GSMDialMain)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GSMDialMain, CDialogBase)
	//{{AFX_MSG_MAP(GSMDialMain)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GSMDialMain message handlers

BOOL GSMDialMain::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	SetTimer(103, 1000, NULL);
	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();

	return TRUE;
}

BOOL GSMDialMain::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_BUTTON_DIAL);
	m_csTitile = str;

	str.LoadString(RES_BTN_BT_DIAL);
	m_Item[0].chChar = str;

	str.LoadString(RES_BTN_BT_PHONEBOOK);
	m_Item[1].chChar = str;

	str.LoadString(RES_BTN_BT_DIALLOG);
	m_Item[2].chChar = str;

	str.LoadString(RES_DIAL_NET_SET);
	m_Item[3].chChar = str;

	return TRUE;
}

//变皮肤
BOOL GSMDialMain::OnSkinChange()
{
	return TRUE;
}

void GSMDialMain::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_GSM_DIAL_MAIN_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_GSM_DIAL_MAIN_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void GSMDialMain::InitControl()
{
	for (int i=0;  i < 5; i++)
		m_Item[i].nState = BTN_STATE_NORMAL;

	//
	m_Item[0].rect.left		= 190;
	m_Item[0].rect.top		= 145;
	m_Item[0].rect.right	= 325;
	m_Item[0].rect.bottom	= 275;
	//
	m_Item[1].rect.left		= 335;
	m_Item[1].rect.top		= 145;
	m_Item[1].rect.right	= 466;
	m_Item[1].rect.bottom	= 275;
	//
	m_Item[2].rect.left		= 478;
	m_Item[2].rect.top		= 145;
	m_Item[2].rect.right	= 611;
	m_Item[2].rect.bottom	= 275;
	//
	m_Item[3].rect.left		= 0;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 0;
	m_Item[3].rect.bottom	= 0;

	//Exit.
	m_Item[4].rect.left		= 740;
	m_Item[4].rect.top		= 0;
	m_Item[4].rect.right	= 800;
	m_Item[4].rect.bottom	= 60;
}

void GSMDialMain::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void GSMDialMain::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void GSMDialMain::OnExit()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(0);
}

void GSMDialMain::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;


	for(int i=0;i<5;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.bottom += 30;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_BOTTOM|DT_CENTER);
	}

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void GSMDialMain::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0;i<5;i++)
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

	//短信
	if(PtInRect(&CRect(700, 400, 800, 480),point))
	{
		//OnBtnSMS();
	}
	
	Invalidate();
	CDialogBase::OnLButtonDown(nFlags, point);
}

void GSMDialMain::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<5;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:	OnDialout();break;
			case 1:	OnPhoneBook();break;
			case 2:	OnPhoneLog();break;
			case 3: GSMNetSet(); break;
			case 4: OnExit(); 	break;
			default: break;
			}
		}
	}

	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void GSMDialMain::OnPhoneBook()
{
	CPBookMain dlg;//电话薄
 	dlg.DoModal();		
}
void GSMDialMain::OnDialout()
{
	CGSMDial dlg;//打电话
	dlg.DoModal();
}

void GSMDialMain::OnPhoneLog()
{
 	CPhoneLog dlg;//通话记录
	dlg.SetGSMMain(this);
	dlg.DoModal();
}

void GSMDialMain::GSMNetSet()
{
//	CGSMNetSet dlg;
//	dlg.DoModal();
}

void GSMDialMain::OnBtnSMS()
{
	CSMSMain dlg;
	dlg.DoModal();
}

void GSMDialMain::OnTimer(UINT nIDEvent) 
{

	CDialogBase::OnTimer(nIDEvent);
}

LRESULT GSMDialMain::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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


void GSMDialMain::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();
	bk_normal_dc.DeleteDC();

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

BOOL GSMDialMain::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == 0x9876 )
	{
		OnDialout();
		return TRUE;
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}
