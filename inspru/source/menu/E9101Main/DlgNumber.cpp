// DlgPair.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgNumber.h"
#include "E9101MainDlg.h"
#include "HBCP.h"
#include "command.h"
#include "../imm/InputShl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_PhoneEdit;
/////////////////////////////////////////////////////////////////////////////
// CDlgPair dialog

CDlgNumber::CDlgNumber(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgNumber::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNumber)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNumber)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNumber, CDialogBase)
	//{{AFX_MSG_MAP(CDlgPair)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPair message handlers

BOOL CDlgNumber::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	MoveWindow(0, 237, 468, 272);
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL  CDlgNumber::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_BTN_BT_PAIR);
	m_stTitle = str;

	str.LoadString(RES_BTN_BT_PAIRINFO);
	m_stInfo = str;

	return TRUE;
}

BOOL  CDlgNumber::OnSkinChange()
{
	return TRUE;
}

void CDlgNumber::OnExit()
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

	bitmap.DeleteObject();
	memDC.DeleteDC();


	CDialogBase::EndDialog(0);
}

void CDlgNumber::InitGdi()
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

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_NUMBER_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_BG_NUMBER_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    
 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}


void CDlgNumber::InitControl()
{

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

void CDlgNumber::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgNumber::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgNumber::OnPaint() 
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

//    	for(int jj=0;jj<13;jj++)
//      	{
//       		Rect = m_Item[jj].rect;
//       		memDC.FillSolidRect(&Rect, RGB(0, 255-jj*20, 0));
//		}

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CDlgNumber::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	for(int i=0;i<13;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				//PlayKeySound();
				m_Item[i].nState = BTN_STATE_DOWN;
				if( i == 0)
				{
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\1.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\1.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				}
				else if( i == 1) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\2.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\2.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 2) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\3.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\3.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 3) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\4.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\4.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 4) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\5.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\5.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 5) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\6.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\6.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 6) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\7.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\7.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 7)
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\8.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\8.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 8) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\9.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\9.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 9) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\0.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\0.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();

				else if(  10 <= i <= 12) 
				{
					PlayKeySound();
				}

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

void CDlgNumber::OnLButtonUp(UINT nFlags, CPoint point) 
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
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 1, 0);
					break;
				case 1:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 2, 0);
					break;
				case 2:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 3, 0);
					break;
				case 3:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 4, 0);
					break;
				case 4:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 5, 0);
					break;
				case 5:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 6, 0);
					break;
				case 6:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 7, 0);
					break;
				case 7:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 8, 0);
					break;
				case 8:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 9, 0);
					break;
				case 9:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 0, 0);				
					break;
				case 10:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 10, 0);
					break;	
				case 11:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 11, 0);
					OnExit();
					break;	
				case 12:
					::SendMessage(g_PhoneEdit, WM_IMM_MESSAGE, 12, 0);
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


void CDlgNumber::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialogBase::OnTimer(nIDEvent);
}
