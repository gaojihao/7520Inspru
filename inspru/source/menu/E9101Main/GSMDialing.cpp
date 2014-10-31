// GSMDialing.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "GSMDialing.h"
#include "PhoneBookMng.h"
#include "DialLog.h"
#include "PhoneLogMng.h"
#include "../ExTool/ex_basics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGSMDialing dialog

#define   TID_DIALING_LIST          2010
#define   TID_DIALING_ACTION        2011

CGSMDialing::CGSMDialing(CWnd* pParent /*=NULL*/)
	: CDialogBase(CGSMDialing::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGSMDialing)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTHDDialingList = FALSE;
}


void CGSMDialing::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGSMDialing)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGSMDialing, CDialogBase)
	//{{AFX_MSG_MAP(CGSMDialing)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers


BOOL CGSMDialing::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	MoveWindow(153, 93, 494, 274);

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	
	OnSkinChange();

	b_DialResult = FALSE;
	b_HasResult = FALSE;

	m_waiting_count = 0; // 100
	//SetTimer( TID_DIALING_LIST,   300, NULL ); 

	m_bTHDDialingList = TRUE;
	m_hTHDDialingList = CreateThread(NULL, 0, THDDialingList, this, 0, NULL);

	SetTimer( TID_DIALING_ACTION, 500, NULL );

	return TRUE;
}

void CGSMDialing::SetCallPhone( CString& pStrPhone )
{
	m_strPhone = pStrPhone;
	m_TipItem.chChar = pStrPhone;
	m_strUserName = _T("");
	if ( m_strPhone == _T("110") ||
		m_strPhone == _T("112") ||
		m_strPhone == _T("119") ||
		m_strPhone == _T("120") )
	{
		m_nTotalIdx = 0xFFFFFFFF-1;
	}else
	{
		m_nTotalIdx = 500;
	}

	if ( !g_GsmLogic.m_pbook.is_open() )
			g_GsmLogic.m_pbook.load_data( PBOOK_PATH_DEVICE );

	g_GsmLogic.SchPBook( pStrPhone, m_strUserName );
}

//变换语言和单位
BOOL CGSMDialing::OnLanguageChange()//变皮肤
{
	CResString str;
	str.LoadString(RES_BUTTON_SMS_DIALING_TITLE);
	m_csTitile = "";
	
	return TRUE;
}

BOOL CGSMDialing::OnSkinChange()
{
	return TRUE;
}

void CGSMDialing::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap( m_pDC,rc.Width(),rc.Height() );
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
	bk_list_dc.CreateCompatibleDC( m_pDC );

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_GSM_DIALING,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_DIALING_LIST, true);
	m_stBmpList = pSkin->hBitmap;
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject( CBitmap::FromHandle(m_stBtnNormalBMP) );
	m_pold_sending_bmp    = bk_list_dc.SelectObject( CBitmap::FromHandle(m_stBmpList) );
	
	::ReleaseDC(m_hWnd, hDC);
}


void CGSMDialing::ReleaseGdi()
{
	bk_list_dc.SelectObject( m_pold_sending_bmp );
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject( m_stBmpList );
	DeleteObject(m_stBtnNormalBMP);

	bk_list_dc.DeleteDC();
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CGSMDialing::OnExit()
{
	KillTimer( TID_DIALING_ACTION );
	//KillTimer( TID_DIALING_LIST );
	m_bTHDDialingList = FALSE;
	if(m_hTHDDialingList != NULL)
		TerminateThread(m_hTHDDialingList,0);
	
	ReleaseGdi();
	g_GsmLogic.m_pbook.save_data( PBOOK_PATH_DEVICE );
	if ( b_DialResult )
		CDialogBase::EndDialog( IDOK );
	else
		CDialogBase::EndDialog( IDCANCEL );
}

//////////////////////////////////////////////////////////////////////////
void CGSMDialing::InitControl()
{//name
	m_TipItem.rect.left		= 0;
	m_TipItem.rect.top		= 0;
	m_TipItem.rect.right	= 494;
	m_TipItem.rect.bottom	= 33;
//Exit
	m_ExitItem.rect.left	= 162;
	m_ExitItem.rect.top		= 169;
	m_ExitItem.rect.right	= 332;
	m_ExitItem.rect.bottom	= 273;
//<<<<...<<<<
	m_TipList.rect.left		= 206;
	m_TipList.rect.top		= 94;
	m_TipList.rect.right	= 206+150;
	m_TipList.rect.bottom	= 94+25;

	m_nCurIdx = 0;
	m_nListWidth[0] = 0;
	m_nListWidth[1] = 30;
	m_nListWidth[2] = 60;
	m_nListWidth[3] = 90;
	m_nListWidth[4] = 120;
}

void CGSMDialing::DrawItems(CDC* pDC, const CRect &rt, enBtnState state)
{
	ASSERT(pDC);
	int x = 0;
	int y = 0;
	switch(state)
	{
	case BTN_STATE_NORMAL:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
//	case BTN_STATE_DOWN:
//		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
//		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CGSMDialing::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CGSMDialing::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	//DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_TOP|DT_LEFT);

	// draw user name
	CString str;	//名字和号码
	str.Format(_T("%s  %s"), m_strUserName, m_TipItem.chChar);
	DrawBtnText(&memDC,m_TipItem.rect,str,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	//draw dialing number
	//CRect rtTemp(m_TipItem.rect);
	//rtTemp.OffsetRect(0, 28 );
	//DrawBtnText(&memDC,rtTemp,m_TipItem.chChar,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);

	memDC.BitBlt(m_TipList.rect.left, m_TipList.rect.top, (m_nCurIdx%6)*30/*m_nListWidth[m_nCurIdx%5]*/, m_TipList.rect.Height(),
		&bk_list_dc, 0, 0, SRCCOPY );
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CGSMDialing::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(PtInRect(m_ExitItem.rect,point))
	{
		if(m_TipItem.nState == BTN_STATE_NORMAL)
		{
			m_TipItem.nState = BTN_STATE_DOWN;
			PlayKeySound();
		}
	}
	
	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CGSMDialing::OnLButtonUp(UINT nFlags, CPoint point) 
{
 	if ( b_HasResult )
	{
 		OnExit();
 	} else if(PtInRect(m_ExitItem.rect,point))
	{
		if( m_TipItem.nState == BTN_STATE_DOWN )
			m_TipItem.nState = BTN_STATE_NORMAL;
		OnExit();
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CGSMDialing::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_DIALING_LIST )
	{
		m_nCurIdx++;
		m_waiting_count++;
		if ( m_waiting_count >= m_nTotalIdx )
		{

			KillTimer( TID_DIALING_LIST );
			SendDialResult( FALSE );
		}
		Invalidate( FALSE );
	}else if ( nIDEvent == TID_DIALING_ACTION )
	{
		KillTimer( nIDEvent );
		// 保存通话记录
		CDialLog plog;
		PhoneLogMng::Instance().LoadAll();
		plog.m_csNum = m_strPhone;
		plog.m_iLogFlag = 1;//call out
		//PhoneLogMng::Instance().Delete(&plog);
		PhoneLogMng::Instance().AddLog(&plog); 
		PhoneLogMng::Instance().Write();

		Sleep(100);
		if ( m_strPhone == _T("110") ||
			 m_strPhone == _T("119") ||
			 m_strPhone == _T("120") )
			 m_strPhone = _T("112");
		g_GsmLogic.SetPhoneNum( m_strPhone );
		g_GsmLogic.GSMDial();
	}

	CDialogBase::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CGSMDialing::SendDialResult(BOOL bSuc)
{
	//打电话的结果是成功，还是失败
	if ( bSuc )
	{
		//表示已经接通，退出当前UI
		b_DialResult = bSuc;
		OnExit();
	}else
	{
		b_HasResult = TRUE;
		CResString str;
		str.LoadString(RES_BUTTON_SMS_DIALING_FAILED);
		m_TipItem.chChar = str; 
		m_nCurIdx = 0;
		g_GsmLogic.GSMHangup();
	}
	Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
BOOL CGSMDialing::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
		if ( pMsg->wParam ==  GSM_ACK_DIAL_RESULT )
		{
			SendDialResult( pMsg->lParam );
			Invalidate(FALSE);
		}else if ( pMsg->wParam == GSM_ACK_LINE_DISCONNECT ||
			       pMsg->wParam == GSM_ACK_LINE_NOCARRIER )
		{
			KillTimer( TID_DIALING_ACTION );
			//KillTimer( TID_DIALING_LIST );
			m_bTHDDialingList = FALSE;
			SendDialResult( FALSE );
		}
	}

	else if( pMsg->message == WM_CREATE_SET )
	{
		if( pMsg->wParam == 0x06 )
		{
			OnExit();
		}
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}

DWORD CGSMDialing::THDDialingList( LPVOID lpParameter )
{
	CGSMDialing* pDlg = (CGSMDialing*)lpParameter;
	while(pDlg->m_bTHDDialingList)
	{
		pDlg->m_nCurIdx++;
		pDlg->m_waiting_count++;
		if ( pDlg->m_waiting_count >= pDlg->m_nTotalIdx )
		{
			//KillTimer( TID_DIALING_LIST );
			pDlg->m_bTHDDialingList = FALSE;
			//pDlg->SendDialResult( FALSE );
			return 0;
		}
		pDlg->Invalidate( FALSE );
		Sleep(300);
	}

	return 1;
}
