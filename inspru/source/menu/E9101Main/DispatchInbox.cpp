// SMSInbox.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DispatchInbox.h"
#include "../GSM/GSMLogic.h"
#include "../ExTool/ex_basics.h"
#include "DlgConfirm.h"
#include "SMSInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSInbox dialog

#define   TID_GSM_READ_SIM     4001

CDispatchInbox::CDispatchInbox(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDispatchInbox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDispatchInbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispatchInbox, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDispatchInbox::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	SetWindowPos(&wndTopMost, 0, 0, 800, 480, SWP_SHOWWINDOW);

#if 0
	if ( g_GsmLogic.GetSIMState() && !g_GsmLogic.GSMIsReadSIM() )
	{
		SetTimer( TID_GSM_READ_SIM, 50, NULL );
	}else{
		m_nCurPage = 1;
		if ( !g_GsmLogic.m_dispatch.is_open() )
			g_GsmLogic.m_dispatch.load_data( SMS_PATH_DISPATCH_INFO );
		ShowPage( m_nCurPage );
	}
#endif

#if 1
		m_nCurPage = 1;
		if ( !g_GsmLogic.m_dispatch.is_open() )
			g_GsmLogic.m_dispatch.load_data( SMS_PATH_DISPATCH_INFO );
		ShowPage( m_nCurPage );
#endif 
	
	return TRUE;
}
//变换语言和单位
BOOL CDispatchInbox::OnLanguageChange()//变皮肤
{

	CResString str;
	
	str.LoadString(RES_BUTTON_SMS_INBOX);
	m_csTitile = str;
		
	str.LoadString(RES_SMS_READ_SIM_INFO);
	m_list[0].chChar = str;
	m_ItemState[0] = 1;

	m_Item[0].chChar = _T("");
	m_Item[1].chChar = _T("");
	m_Item[2].chChar = _T("");
	m_Item[3].chChar = _T("");


	return TRUE;
}

BOOL CDispatchInbox::OnSkinChange()
{
	return TRUE;
}


void CDispatchInbox::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_GSM_READ_SIM )
	{
		KillTimer( TID_GSM_READ_SIM );
		g_GsmLogic.GSMReadSIMSMS();
	}
	CDialogBase::OnTimer(nIDEvent);
}

void CDispatchInbox::InitGdi()
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
	bk_sms_flag.CreateCompatibleDC( m_pDC );

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_SMS_LIST_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_BG_SMS_LIST_N, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	pSkin = g_pResMng->RequestBmp(SKIN_SMS_FLAG, true);
	m_stSmsFlagBmp = pSkin->hBitmap; 

	m_pold_bk_normal_bmp = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp  = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_sms_bmp       = bk_sms_flag.SelectObject(CBitmap::FromHandle(m_stSmsFlagBmp));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDispatchInbox::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	bk_sms_flag.SelectObject( m_pold_sms_bmp );
	
	DeleteObject( m_stBtnPressBMP );
	DeleteObject( m_stBtnNormalBMP );
	DeleteObject( m_stSmsFlagBmp );
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	bk_sms_flag.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CDispatchInbox::OnExit()
{
	g_GsmLogic.GetUnRdMsg();		//count the number of unreaded sms;
	ReleaseGdi();
	g_GsmLogic.m_dispatch.save_data( SMS_PATH_DISPATCH_INFO );
	CDialogBase::EndDialog(0);
}

void CDispatchInbox::InitControl()
{
	int i;
 	for ( i =0;  i < 4; i++)
 	{
 		m_Item[i].nState = BTN_STATE_NORMAL;
 	}

	for ( i =0;  i < elist_count; i++)
		m_list[i].nState = BTN_STATE_NORMAL;

//2 上x:228y:231 下x:304y:268
	m_Item[0].rect.top = 405;
	m_Item[0].rect.left = 377;
	m_Item[0].rect.bottom = 475;
	m_Item[0].rect.right = 510;
	
	//3 上x:312y:231 下x:388y:268
	m_Item[1].rect.top = 405;
	m_Item[1].rect.left = 516;
	m_Item[1].rect.bottom = 475;
	m_Item[1].rect.right = 648;
	
	//4 上x:396y:231 下x:472y:268
	m_Item[2].rect.top = 405;
	m_Item[2].rect.left = 656;
	m_Item[2].rect.bottom = 475;
	m_Item[2].rect.right = 787;

	//Exit
	m_Item[3].rect.top = 0;
	m_Item[3].rect.left = 740;
	m_Item[3].rect.bottom = 61;
	m_Item[3].rect.right = 800;

	m_list[0].rect.left = 0;
	m_list[0].rect.top = 75;
	m_list[0].rect.right = 800;
	m_list[0].rect.bottom = 139;
		
	m_list[1].rect.left = 0;
	m_list[1].rect.top = 140;
	m_list[1].rect.right = 800;
	m_list[1].rect.bottom = 203;
		
	m_list[2].rect.left = 0;
	m_list[2].rect.top = 204;
	m_list[2].rect.right = 800;
	m_list[2].rect.bottom = 268;
		
	m_list[3].rect.left = 0;
	m_list[3].rect.top = 269;
	m_list[3].rect.right = 800;
	m_list[3].rect.bottom = 333;

	m_list[4].rect.left = 0;
	m_list[4].rect.top = 334;
	m_list[4].rect.right = 800;
	m_list[4].rect.bottom = 397;
}

void CDispatchInbox::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDispatchInbox::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDispatchInbox::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	int i=0;
	for(i=0;i<4;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}


	PSKINBMP_ST pSkin = g_pResMng->RequestBmp(SKIN_SMS_FLAG, true);

	for( i = 0; i < elist_count; i++ )
	{
		DrawItems( &memDC,m_list[i].rect,m_list[i].nState);

		if ( m_ItemState[i] == 1 ) //已读
			memDC.BitBlt(m_list[i].rect.left, m_list[i].rect.top, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			         &bk_sms_flag, 0, pSkin->nHeightPerCell , SRCCOPY );

		else if ( m_ItemState[i] == 2 ) //未读
			memDC.BitBlt(m_list[i].rect.left, m_list[i].rect.top, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			         &bk_sms_flag, 0, 0, SRCCOPY );
		else if ( m_ItemState[i] == 3 ) //SIM卡上SMS
			memDC.BitBlt(m_list[i].rect.left, m_list[i].rect.top, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			         &bk_sms_flag, 0,  pSkin->nHeightPerCell * 2, SRCCOPY );

		CRect rt(m_list[i].rect);
		rt.left += pSkin->nWidthPerCell;
		DrawBtnText( &memDC,rt, m_list[i].chChar, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	}


	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDispatchInbox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	for( i = 0; i < 4; i++ )
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

	for( i = 0; i < elist_count; i++ )
	{
		if(PtInRect(&m_list[i].rect,point))
		{
			if(m_list[i].nState == BTN_STATE_NORMAL)
			{
				m_list[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}
	

	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDispatchInbox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int i=0;
	for(i=0;i<4;i++)
	{
		if(m_Item[i].nState == BTN_STATE_DOWN )
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:	OnBtnDelAll();break;
			case 1:	OnBtnPageUp();break;
			case 2:	OnBtnPageDown();break;
			case 3: OnExit();break;
			default:
				break;
			}
		}
	}

	for(i=0;i<elist_count;i++)
	{
		if(m_list[i].nState == BTN_STATE_DOWN )
		{
			m_list[i].nState = BTN_STATE_NORMAL;
			UserClickItem( i );
		}
	}

	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}


void  CDispatchInbox::OnBtnPageUp()
{
	m_nCurPage--;
	if ( m_nCurPage < 1 )
		m_nCurPage = 1;
	ShowPage( m_nCurPage );
	Invalidate();
}

void  CDispatchInbox::OnBtnPageDown()
{
	m_nCurPage++;
	int nMaxNum = SMSMaxNum();
	int MaxPage = 1 + nMaxNum / elist_count;
	if ( nMaxNum > 0 )
		MaxPage = (nMaxNum - 1) / elist_count + 1;
	if ( m_nCurPage > MaxPage ) 
		m_nCurPage = MaxPage;
	
	ShowPage( m_nCurPage );
	Invalidate();
}


void  CDispatchInbox::OnBtnDelAll()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_ALL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	
	if( CDlgConfirm::s_bOk )
	{
		logic_data* pInbox = &( g_GsmLogic.m_dispatch );
		pInbox->remove_all();
		g_GsmLogic.GSMDelSIMSMSAll();
		m_nCurPage = 1;
		ShowPage( m_nCurPage );
		Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////

void    CDispatchInbox::UserClickItem(int nItem )
{
#if 1
	int nMaxRd = SMSMaxNum();
	logic_data* pInbox = &(g_GsmLogic.m_dispatch);
	int  nSleIdx = nItem + (m_nCurPage - 1) * elist_count;
	if ( nSleIdx >= nMaxRd )
		return;
	nSleIdx = nMaxRd - nSleIdx - 1;
	if ( nSleIdx < pInbox->max_number() )
	{
		pInbox->get_recourd( nSleIdx )->m_type = 1;
		if ( nItem < elist_count) 	
			m_ItemState[nItem] = 1;
		pInbox->user_change(true);
	}

	
	CSMSInfo dlg;
	dlg.SetReplyBtn(true);
	dlg.SetLogicData(pInbox, nItem + (m_nCurPage - 1) * elist_count, TRUE  );
	dlg.DoModal();
	
	if ( m_nCurPage > nMaxRd / elist_count + 1 )
	{
		if ( nMaxRd > 0 )
			m_nCurPage = ( nMaxRd - 1 ) / elist_count + 1;
		else
			m_nCurPage = ( nMaxRd ) / elist_count + 1;
	}
	ShowPage( m_nCurPage );
	Invalidate();
#endif


//没有显示手机SMS
#if 0
	logic_data* pInbox = &(g_GsmLogic.m_dispatch);
	int  nSleIdx = nItem + (m_nCurPage - 1) * elist_count;
	if ( nSleIdx >= pInbox->max_number() )
		return;
	nSleIdx = pInbox->max_number() - nSleIdx - 1;
	pInbox->get_recourd( nSleIdx )->m_type = 1;
	if ( nItem < elist_count) 	
		m_ItemState[nItem] = 1;

	pInbox->user_change(true);
	CSMSInfo dlg;
	dlg.SetReplyBtn(true);
	dlg.SetLogicData(pInbox, nItem + (m_nCurPage - 1) * elist_count, TRUE  );
	dlg.DoModal();

	if ( m_nCurPage >= pInbox->max_number() / elist_count + 1 )
	{
		if ( pInbox->max_number() > 0 )
			m_nCurPage = ( pInbox->max_number() - 1 ) / elist_count + 1;
		else
			m_nCurPage = ( pInbox->max_number() ) / elist_count + 1;
	}
	ShowPage( m_nCurPage );
	Invalidate();
#endif

}

//////////////////////////////////////////////////////////////////////////

void    CDispatchInbox::ShowPage(int nPage)
{

#if 1
	logic_data* pInbox = &(g_GsmLogic.m_dispatch);
	int nMaxRd = SMSMaxNum();
	
	int MaxPage = 1 + nMaxRd / elist_count;
	if ( nMaxRd > 0 )
		MaxPage = ( nMaxRd - 1 ) / elist_count + 1;
	
	if ( nPage > MaxPage ) return;
	
	int nStartIdx =  ( nPage - 1 ) * elist_count;
	int i = 0;
	for( i = 0; i < elist_count; i++ )
	{
		m_ItemState[i] = 0;
		m_list[i].chChar = _T("");
		m_list[i].nState = BTN_STATE_DISABLE;
	}
	
	for( i = 0; i < elist_count; i++ )
	{
		if ( nStartIdx >= nMaxRd )
			break;
		if ( nStartIdx < pInbox->max_number() )
		{
			st_sms* pSMS = pInbox->get_recourd( pInbox->max_number() - nStartIdx - 1 );
			if ( pSMS )
			{
				TCHAR szChar[160];
				memset(szChar, 0, sizeof(szChar) );
				ex_char2uni(pSMS->m_context, szChar, 160 );
				m_list[i].chChar.Format( _T("%s"), szChar );
				m_list[i].nState = BTN_STATE_NORMAL;
				m_ItemState[i] = pSMS->m_type;
			}
		}else
		{
			SM_PARAM* pList = &(g_GsmLogic.GSMGetList()[nStartIdx-pInbox->max_number()]);
			if ( pList )
			{
				TCHAR szChar[160];
				memset(szChar, 0, sizeof(szChar) );
				ex_char2uni(pList->TP_UD, szChar, 160 );
				m_list[i].chChar.Format( _T("%s"), szChar );
				m_list[i].nState = BTN_STATE_NORMAL;
				m_ItemState[i] = 3;
			}
		}
		nStartIdx++;
	}
#endif

//没有显示手机SMS
#if 0 
	logic_data* pInbox = &(g_GsmLogic.m_dispatch);

	int MaxPage = 1 + pInbox->max_number() / elist_count;
	if ( pInbox->max_number() > 0 )
		MaxPage = ( pInbox->max_number() - 1 ) / elist_count + 1;

	if ( nPage > MaxPage ) return;

	int nStartIdx =  ( nPage - 1 ) * elist_count;
	for( int i = 0; i < elist_count; i++ )
	{
		m_ItemState[i] = 0;
		m_list[i].chChar = _T("");
		m_list[i].nState = BTN_STATE_DISABLE;
	}

	for( i = 0; i < elist_count; i++ )
	{
		if ( nStartIdx >= pInbox->max_number() )
			break;
		st_sms* pSMS = pInbox->get_recourd( pInbox->max_number() - nStartIdx - 1 );
		if ( pSMS )
		{
			TCHAR szChar[160];
			memset(szChar, 0, sizeof(szChar) );
			ex_char2uni(pSMS->m_number, szChar, 160 );
			m_list[i].chChar.Format( _T("%s"), szChar );
			m_list[i].nState = BTN_STATE_NORMAL;
			m_ItemState[i] = pSMS->m_type;
		}
		nStartIdx++;
	}
#endif

}

BOOL CDispatchInbox::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
		if ( pMsg->wParam ==  GSM_SIM_SMS_READ )
		{
			m_nCurPage = 1;
			if ( !g_GsmLogic.m_dispatch.is_open() )
				g_GsmLogic.m_dispatch.load_data( SMS_PATH_DISPATCH_INFO );
			ShowPage( m_nCurPage );
			Invalidate();
		}
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

int   CDispatchInbox::SMSMaxNum()
{
	logic_data* pInbox = &(g_GsmLogic.m_dispatch);
	int nMax = pInbox->max_number();
	nMax += g_GsmLogic.GSMGetSIMUsed();
	return nMax;
}