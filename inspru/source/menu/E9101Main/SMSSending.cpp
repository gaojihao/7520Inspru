// SMSSending.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "SMSSending.h"
#include "../GSM/GSMLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSSending dialog

#define   TID_SENDING_LIST     1010
#define   TID_SENDING_ACTION   1011

CSMSSending::CSMSSending(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSMSSending::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSSending)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hPrevWnd = NULL;
}


void CSMSSending::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSSending)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSSending, CDialogBase)
	//{{AFX_MSG_MAP(CSMSSending)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CSMSSending::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	MoveWindow(0,0,494,274);
	CenterWindow(GetDesktopWindow());

	b_HasResult = false;
	SetTimer( TID_SENDING_LIST,   300, NULL );
	SetTimer( TID_SENDING_ACTION, 100, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CSMSSending::SetMsgParam(CString& pStrPhone, CString& strMsg, BOOL bSaved /* = FALSE */)
{
	m_strPhone = pStrPhone;
	m_strSMSMsg = strMsg;
	m_TipItem.chChar = m_strPhone;
	m_bSaveMsg = bSaved;
	
	if ( !g_GsmLogic.m_pbook.is_open() )
		g_GsmLogic.m_pbook.load_data( PBOOK_PATH_DEVICE );
	
	g_GsmLogic.SchPBook( pStrPhone, m_strUserName );
}
//变换语言和单位
BOOL CSMSSending::OnLanguageChange()//变皮肤
{

	CResString str;
	str.LoadString(RES_BUTTON_SMS_SENDING_TITLE);
	m_csTitile = str;
	
	return TRUE;
}

BOOL CSMSSending::OnSkinChange()
{
	
	return TRUE;
}

void CSMSSending::InitGdi()
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
	bk_list_dc.CreateCompatibleDC( m_pDC );
	//bk_press_dc.CreateCompatibleDC(m_pDC);

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_SENDING,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_BG_SENDING_LIST, true);
	m_stBmpList = pSkin->hBitmap;
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject( CBitmap::FromHandle(m_stBtnNormalBMP) );
	m_pold_sending_bmp    = bk_list_dc.SelectObject( CBitmap::FromHandle(m_stBmpList) );
	//m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CSMSSending::ReleaseGdi()
{

//	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_list_dc.SelectObject( m_pold_sending_bmp );
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	
//	DeleteObject(m_stBtnPressBMP);
	DeleteObject( m_stBmpList );
	DeleteObject(m_stBtnNormalBMP);
	
//	bk_press_dc.DeleteDC();
	bk_list_dc.DeleteDC();
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CSMSSending::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}


void CSMSSending::InitControl()
{
	// 上x:151 
    //  y:71  
	// 下x:329 y:104
	m_TipItem.rect.left		= 0;
	m_TipItem.rect.top		= 185;
	m_TipItem.rect.right	= 494;
	m_TipItem.rect.bottom	= 220;

	//Exit
	m_ExitItem.rect.top = 0;
	m_ExitItem.rect.left = 430;
	m_ExitItem.rect.bottom = 40;
	m_ExitItem.rect.right = 480;

	//发送短信动态效果
	m_TipList.rect.left		= 175;
	m_TipList.rect.top		= 98;
	m_TipList.rect.right	= 175+210;
	m_TipList.rect.bottom	= 98+55;

	m_nCurIdx = 0;
	m_nListWidth[0] = 0;
	m_nListWidth[1] = 42;
	m_nListWidth[2] = 42*2;
	m_nListWidth[3] = 42*3;
	m_nListWidth[4] = 42*4;
	m_nListWidth[5] = 42*5;
}

void CSMSSending::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CSMSSending::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CSMSSending::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	DrawBtnText(&memDC,CRect(0,0,494,60),m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	// draw user name
	DrawBtnText(&memDC,m_TipItem.rect,m_strUserName,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	//draw dialing number
	CRect rtTemp(m_TipItem.rect);
	rtTemp.OffsetRect(0, 28 );
	DrawBtnText(&memDC,rtTemp,m_TipItem.chChar,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);

	memDC.BitBlt(m_TipList.rect.left, m_TipList.rect.top, m_nListWidth[m_nCurIdx%6], m_TipList.rect.Height(),
		&bk_list_dc, 0, 0, SRCCOPY );
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CSMSSending::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CSMSSending::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ( b_HasResult )
	{
		OnExit();
	} 

	//为了解决发送过程中退出的BUGs
/*	
	else if(PtInRect(m_ExitItem.rect,point))
	{
		if(m_TipItem.nState == BTN_STATE_DOWN)
			m_TipItem.nState = BTN_STATE_NORMAL;
		OnExit();
	}
*/	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}


void CSMSSending::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_SENDING_LIST )
	{
		if ( m_hPrevWnd != NULL )
		{
			m_hPrevWnd->OnExit();
			m_hPrevWnd = NULL;
		}

		m_nCurIdx++;
		if ( m_nCurIdx >= 100 )
		{
			KillTimer( nIDEvent );
			SendMsgResult( FALSE );
		}
		Invalidate( FALSE );
	}else if ( nIDEvent == TID_SENDING_ACTION )
	{
		KillTimer( nIDEvent );
		g_GsmLogic.SetPhoneNum( m_strPhone );
		g_GsmLogic.SetMsgBuf( m_strSMSMsg );//发短信字符
		g_GsmLogic.GSMSendSMS();
	}
	CDialogBase::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CSMSSending::SendMsgResult(BOOL bSuc)
{
	b_HasResult = true;
	KillTimer( TID_SENDING_LIST );
	if ( bSuc )
	{
		CResString str;
		str.LoadString(RES_BUTTON_SMS_SENDED_OK);
		m_TipItem.chChar = str;
		m_nCurIdx = 5;
		if ( m_bSaveMsg )
			g_GsmLogic.UserSendMsg(m_strPhone, m_strSMSMsg );	//UserSaveMsg
	}
	else
	{
		CResString str;
		str.LoadString(RES_BUTTON_SMS_SENDED_FAILED);
		m_TipItem.chChar = str; 
		m_nCurIdx = 0;
		if ( m_bSaveMsg )
			g_GsmLogic.UserSaveMsg( m_strSMSMsg );
	}	
}

//////////////////////////////////////////////////////////////////////////
BOOL CSMSSending::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
		if ( pMsg->wParam ==  GSM_ACK_MSG_RESULT )
		{
			KillTimer( TID_SENDING_LIST );
			m_nCurIdx = 6;
			SendMsgResult( pMsg->lParam );
			Invalidate(FALSE);
		}
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}

void CSMSSending::SetCallPhone( CString& pStrPhone )
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
		m_nTotalIdx = 1500;
	}

	if ( !g_GsmLogic.m_pbook.is_open() )
			g_GsmLogic.m_pbook.load_data( PBOOK_PATH_DEVICE );

	g_GsmLogic.SchPBook( pStrPhone, m_strUserName );
}