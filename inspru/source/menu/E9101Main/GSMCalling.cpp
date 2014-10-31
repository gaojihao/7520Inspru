// GSMCalling.cpp : implementation file

#include "stdafx.h"
#include "e9101main.h"
#include "GSMCalling.h"
#include "DlgDialNumber.h"
#include "../imm/InputShl.h"
#include "PhoneLog.h"
#include "PhoneLogMng.h"
#include "PhoneBookMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGSMCalling dialog
extern CDlgDialNumber* g_DialNum;;
extern UINT g_nDialIn;
#define TIMER_RINGIND  3001
CString CGSMCalling::m_strName = _T("");
CString CGSMCalling::m_strCityr = _T("");
BOOL  CGSMCalling::m_bIsAcc = FALSE;
BOOL  CGSMCalling::m_is_accepted = FALSE;

#define   GSM_NEW_CALLING              (WM_USER+0x9234)


CGSMCalling::CGSMCalling(CWnd* pParent /*=NULL*/)
	: CDialogBase(CGSMCalling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGSMCalling)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bExit = FALSE;
	m_EnablePress = TRUE;
	m_is_accepted = FALSE;
	m_strCallingNum = _T("");
	m_strExtNo = _T("");
}

BEGIN_MESSAGE_MAP(CGSMCalling, CDialogBase)
	//{{AFX_MSG_MAP(CGSMCalling)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnButtonAccept)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, OnButtonReject)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_VOLUP, OnBtnVolup)
	ON_BN_CLICKED(IDC_BTN_VOLDOWN, OnBtnVoldown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IMM_DIAL_NUMBER_MESSAGE, OnDialNum)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGSMCalling message handlers
void CGSMCalling::OnButtonAccept() 
{
	if ( !m_is_accepted )
	{
		m_nCurIdx = 6;
		m_Item[1].nState = BTN_STATE_NORMAL;
		KillTimer( TIMER_RINGIND );
		m_is_accepted = TRUE;
		g_GsmLogic.GSMAccepted();

		// 保存通话记录
		CDialLog plog;
		PhoneLogMng::Instance().LoadAll();
		plog.m_csNum = m_strCallingNum;
		plog.m_iLogFlag = 2;//call accept
		PhoneLogMng::Instance().AddLog(&plog); 
		PhoneLogMng::Instance().Write();
		Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL CGSMCalling::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	InitGdi();

	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW );
	CUiMng::Instance().Detach(this);
	 m_is_accepted = FALSE;
	 m_DialExtNum = FALSE;
	InitControl();
	OnLanguageChange();
	OnSkinChange();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, DLG_WIDTH, DLG_HIGHT, SWP_SHOWWINDOW);

	SetWindowPos(&wndTopMost, 153, 93, 494, 274, SWP_SHOWWINDOW );
	SetTimer( TIMER_RINGIND, 250, 0 );
	m_nCurIdx = 0;

	::PostMessage(HWND_BROADCAST, GSM_NEW_CALLING, 1, 0 );
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void  CGSMCalling::SetCallingStr(CString strNum)
{
	m_strCallingNum = strNum;
	m_strUserName = _T("");
	if ( !g_GsmLogic.m_pbook.is_open() )
		g_GsmLogic.m_pbook.load_data( PBOOK_PATH_DEVICE );
	g_GsmLogic.SchPBook( m_strCallingNum, m_strUserName );
}

void CGSMCalling::OnButtonReject() 
{
	if (m_strCallingNum.GetLength() > 2)
	{
		// 保存通话记录
		CDialLog plog;
		PhoneLogMng::Instance().LoadAll();
		plog.m_csNum = m_strCallingNum;
		plog.m_iLogFlag = 3;//call accept
		PhoneLogMng::Instance().AddLog(&plog); 
		PhoneLogMng::Instance().Write();
	}
	OnBtnExit();
}

void CGSMCalling::OnBtnExit() 
{
	g_GsmLogic.GSMHangup();
	::PostMessage(HWND_BROADCAST, g_nDialIn, 0, 0 );
	OnExit();
}

BOOL CGSMCalling::OnLanguageChange()
{
	CResString str;
	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
	m_stTitle = str;
	return TRUE;
}

BOOL CGSMCalling::OnSkinChange()
{
	return TRUE;
}

void CGSMCalling::OnBtnVolup() 
{
	if ( !m_is_accepted ) return;

	UINT nVol = CSysConfig::Instance().GetGSMVol();
	if ( nVol < 11 )
	{
		nVol++;
		CSysConfig::Instance().SetGSMVol( nVol );
		g_GsmLogic.GSMSetVol( nVol * 8 + 1  );
		Invalidate( FALSE );
	}
}

void CGSMCalling::OnBtnVoldown() 
{	
	if ( !m_is_accepted ) return;
	UINT nVol = CSysConfig::Instance().GetGSMVol();
	if ( nVol > 0 )
	{
		nVol--;
		CSysConfig::Instance().SetGSMVol( nVol );
		g_GsmLogic.GSMSetVol( nVol * 8 + 1  );
		Invalidate( FALSE );
	}
}

void CGSMCalling::OnExit()
{
	m_strCallingNum = _T("");
	m_strExtNo = _T("");
	m_is_accepted = FALSE;
	g_GsmLogic.GSMHangup();
	KillTimer( TIMER_RINGIND );
	m_DialExtNum = FALSE;
	if(g_DialNum != NULL)
	{
		g_DialNum->OnExit();
		m_DialExtNum = TRUE;
	}

	bk_list_dc.SelectObject( m_pold_sending_bmp );
	//bk_dis_dc.SelectObject(m_pold_bk_dis_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	//DeleteObject(m_stBtnDisBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	DeleteObject( m_stBmpList );

	//bk_dis_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	bk_list_dc.DeleteDC();

	midfont.DeleteObject();
	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
	//m_ipc->send_msg( UMSG_GSM_CMD, GSM_ACK_NEW_CALLING,  0 );
	EndDialog(0);
	::PostMessage(HWND_BROADCAST, GSM_NEW_CALLING, 0, 0 );
}


void CGSMCalling::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  
		CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
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

		midfont.CreateFont(
		25,					    // nHeight
		9,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0")); 

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	//bk_dis_dc.CreateCompatibleDC(m_pDC);
	bk_vol_dc.CreateCompatibleDC( m_pDC );
	bk_list_dc.CreateCompatibleDC( m_pDC );
	

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_CALLING_LIST,true);
	m_stBmpList   = pSkin->hBitmap;

	pSkin  = g_pResMng->RequestBmp(BMP_BG_DIAL_IN_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_BG_DIAL_IN_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 

	//pSkin = g_pResMng->RequestBmp(BMP_BG_DIAL_IN_D, true);
	//m_stBtnDisBMP = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_VOL_LIST, true);
	m_stVolBmp = pSkin->hBitmap;  

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject( CBitmap::FromHandle(m_stBtnNormalBMP) );
	m_pold_bk_press_bmp   = bk_press_dc.SelectObject( CBitmap::FromHandle(m_stBtnPressBMP) );
	//m_pold_bk_dis_bmp     = bk_dis_dc.SelectObject( CBitmap::FromHandle(m_stBtnDisBMP) );
	m_pold_bk_vol_bmp     = bk_vol_dc.SelectObject( CBitmap::FromHandle(m_stVolBmp) );
	m_pold_sending_bmp    = bk_list_dc.SelectObject( CBitmap::FromHandle(m_stBmpList) );
	::ReleaseDC(m_hWnd, hDC);
}

void CGSMCalling::InitControl()
{
	for (int i =0;  i < 6; i++)
		m_Item[i].nState = BTN_STATE_NORMAL;
	//Accept
	m_Item[0].rect.left   = 83;
	m_Item[0].rect.top    = 168;
	m_Item[0].rect.right  = 246;
	m_Item[0].rect.bottom = 274;
	//Reject
	m_Item[1].rect.left   = 247;
	m_Item[1].rect.top    = 168;
	m_Item[1].rect.right  = 410;
	m_Item[1].rect.bottom = 274;
	//+
	m_Item[2].rect.left   = 411;
	m_Item[2].rect.top    = 168;
	m_Item[2].rect.right  = 494;
	m_Item[2].rect.bottom = 274;
	//-
	m_Item[3].rect.left   = 0;
	m_Item[3].rect.top    = 168;
	m_Item[3].rect.right  = 82;
	m_Item[3].rect.bottom = 274;

	//Exit.
	m_Item[4].rect.top    = 0;
	m_Item[4].rect.left   = 0;
	m_Item[4].rect.bottom = 0;
	m_Item[4].rect.right  = 0;

	m_Item[5].rect.top    = 0;
	m_Item[5].rect.left   = 0;
	m_Item[5].rect.bottom = 0;
	m_Item[5].rect.right  = 0;

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

void CGSMCalling::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_dis_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CGSMCalling::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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
//////////////////////////////////////////////////////////////////////////

void CGSMCalling::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for( int i = 0; i < 6; i++ )
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.top = Rect.top + 30;
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255));
	}

	DrawBtnText(&memDC,g_retTitile,m_stTitle,bigfont,RGB(255,255,255),DT_LEFT|DT_VCENTER);
//	memDC.BitBlt( m_TipList.rect.left + m_nListWidth[6] - m_nListWidth[m_nCurIdx%7], 
//		          m_TipList.rect.top, 
//				  (m_nCurIdx%6)*30, 
//				  m_TipList.rect.Height(),
// 		&bk_list_dc, m_nListWidth[6] - m_nListWidth[m_nCurIdx%7], 0, SRCCOPY );

	memDC.BitBlt(m_TipList.rect.left+150-(m_nCurIdx%6)*30,
		m_TipList.rect.top, 
		(m_nCurIdx%6)*30/*m_nListWidth[m_nCurIdx%5]*/, 
		m_TipList.rect.Height(),
		&bk_list_dc, 0, 0, SRCCOPY );

	//if(m_strCallingNum.IsEmpty() != TRUE)
	/*{
		Rect.top = 0;
		Rect.bottom = 30;
		Rect.left = 0;
		Rect.right = 420;
		DrawBtnText(&memDC,Rect,m_strUserName,bigfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
		Rect.OffsetRect(0, 32);
		DrawBtnText(&memDC,Rect,m_strCallingNum + m_strExtNo,bigfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}*/
	//名字和号码
	str.Format(_T("%s  %s %s"), m_strUserName, m_strCallingNum, m_strExtNo);
	DrawBtnText(&memDC,CRect(0,0,494,33),str,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);

	// Draw volume
    //X1：416 Y1：74 X2：452 Y2：216
	UINT nLev = CSysConfig::Instance().GetGSMVol();
	if ( nLev > 11 ) 
		nLev = 11;
	if ( nLev < 0 ) 
		nLev = 0;
		//X1：416 Y1：179 //X2：451 Y2：215

	//memDC.BitBlt(417-60, 179-60 - 6 * nLev, 34, 6 * nLev, &bk_vol_dc, 0, (11-nLev)*6 ,SRCCOPY);
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	memDC.SelectObject(pOldBmp);
}

//////////////////////////////////////////////////////////////////////////

void CGSMCalling::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_DialExtNum = FALSE;
	if( g_DialNum != NULL )
	{
		g_DialNum->OnExit();
		m_DialExtNum = TRUE;
	}

	for(int i=0;i<6;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			PlayKeySound();
			if(m_Item[i].nState == BTN_STATE_NORMAL)
				m_Item[i].nState = BTN_STATE_DOWN;
		}
	}
	Invalidate();
	SetCapture();
	CDialogBase::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGSMCalling::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<6;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:
				OnButtonAccept();
				break;
			case 1:
				OnButtonReject();
				break;
			case 2:
				OnBtnVolup();
				break;
			case 3:
				OnBtnVoldown();
				break;
			case 4:
				OnBtnExit();
				break;
			case 5:
				OnOpenNumber();
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

//////////////////////////////////////////////////////////////////////////

void CGSMCalling::OnOpenNumber()
{
	if(g_DialNum == NULL &&! m_DialExtNum )
	{
		g_DialNum = new CDlgDialNumber;
		g_DialNum->Create(IDD_DIALOG_DIAL_NUMBER, this);
	}
}

LRESULT CGSMCalling::OnDialNum(WPARAM wParam , LPARAM lParam)
{
	switch(wParam)
	{
		case 0:
			m_strExtNo+=_T("0");
			Invalidate();
			break;
		case 1:
			m_strExtNo+=_T("1");
			Invalidate();
			break;
		case 2:
			m_strExtNo+=_T("2");
			Invalidate();
			break;
		case 3:
			m_strExtNo+=_T("3");
			Invalidate();
			break;
		case 4:
			m_strExtNo+=_T("4");
			Invalidate();
			break;
		case 5:
			m_strExtNo+=_T("5");
			Invalidate();
			break;
		case 6:
			m_strExtNo+=_T("6");
			Invalidate();
			break;
		case 7:
			m_strExtNo+=_T("7");
			Invalidate();
			break;
		case 8:
			m_strExtNo+=_T("8");
			Invalidate();
			break;
		case 9:
			m_strExtNo+=_T("9");
			Invalidate();
			break;
		case 10:
			m_strExtNo+=_T("#");
			Invalidate();
			break;
		case 11:
			m_strExtNo+=_T("*");
			Invalidate();
			break;
		case 12:
			m_strExtNo = m_strExtNo.Left(m_strExtNo.GetLength()-1);
			Invalidate();
			break;
	}
	return 1;
}


BOOL CGSMCalling::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
		if ( pMsg->wParam == GSM_ACK_LINE_DISCONNECT ||
			pMsg->wParam == GSM_ACK_LINE_NOCARRIER )
		{
			OnButtonReject();
		}
	}

	else if( pMsg->message == WM_CREATE_SET )
	{
		if( pMsg->wParam == 0x06 )
		{
			OnBtnExit();
		}
	};

	return CDialogBase::PreTranslateMessage(pMsg);
}

void CGSMCalling::OnTimer(UINT nIDEvent) 
{
	if ( TIMER_RINGIND == nIDEvent )
	{
		m_nCurIdx++;
		if ( m_nCurIdx % 8 == 0 )
			::PlaySound(SOUND_RING, NULL, SND_ASYNC | SND_NODEFAULT );

		if ( m_nCurIdx > 45 * 4 )
		{
			m_nCurIdx = 0;
			KillTimer( TIMER_RINGIND );
			OnButtonReject();
		}
		Invalidate();
	}
	CDialogBase::OnTimer(nIDEvent);
}











