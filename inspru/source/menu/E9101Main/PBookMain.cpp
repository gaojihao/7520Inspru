// PBookMain.cpp : implementation file
//


#include "stdafx.h"
#include "e9101main.h"
#include "PBookMain.h"
#include "../GSM/GSMLogic.h"
#include "../ExTool/ex_basics.h"
#include "DlgConfirm.h"
#include "SMSInfo.h"
#include "DlgPhoneEdit.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../SQLite3/CppSQLite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPBookMain dialog

#define   TID_GSM_READ_PBOOK     5001

CPBookMain::CPBookMain(CWnd* pParent /*=NULL*/)
	: CDialogBase(CPBookMain::IDD, pParent)
{
	m_nPageIndex	= 0;
	m_nPageCount	= 1;
	m_nRecordCount	= 0;

	m_IsGetItem = FALSE;
	m_strSel = _T("");
}


void CPBookMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CPBookMain, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CPBookMain::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitControl();
	InitGdi();
	
	OnLanguageChange();
	OnSkinChange();

	ShowPage( m_nPageIndex );

	m_bOperation = TRUE;
	return TRUE;
}

//变换语言和单位
BOOL CPBookMain::OnLanguageChange()//变皮肤
{

	CResString str;
	
	str.LoadString(RES_BLUETOOTH_PHONEBOOK);
	m_csTitile = str;
		
	str.LoadString(RES_SMS_READ_SIM_INFO);
	m_strReadTip = str;
	m_strName[0] = str;
	m_ItemState[0] = 0;

	m_Item[0].chChar = _T("");
	m_Item[1].chChar = _T("");
	m_Item[2].chChar = _T("");
	m_Item[3].chChar = _T("");
	
	return TRUE;
}

BOOL CPBookMain::OnSkinChange()
{
	return TRUE;
}


void CPBookMain::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_GSM_READ_PBOOK )
	{
		KillTimer( TID_GSM_READ_PBOOK );
		g_GsmLogic.GSMReadSIMPBook();
	}
	CDialogBase::OnTimer(nIDEvent);
}

void CPBookMain::InitGdi()
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
			ANTIALIASED_QUALITY,       // nQuality
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

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_LIST_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BG_LIST_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	pSkin = g_pResMng->RequestBmp(SKIN_SMS_FLAG, true);
	m_stSmsFlagBmp = pSkin->hBitmap;

	m_pold_bk_normal_bmp = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp  = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_sms_bmp       = bk_sms_flag.SelectObject(CBitmap::FromHandle(m_stSmsFlagBmp));

	CDC dcTemp;
	dcTemp.CreateCompatibleDC( m_pDC );
	pSkin = g_pResMng->RequestBmp(SKIN_BTN_DRAFT_ADD, true);
	HBITMAP TmpBmp = pSkin->hBitmap;
	CBitmap* pOld = dcTemp.SelectObject( CBitmap::FromHandle(TmpBmp) );
	
	CRect rt(m_Item[4].rect);
	bk_normal_dc.BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&dcTemp,0,0,SRCCOPY);
	bk_press_dc.BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&dcTemp,0,rt.Height(),SRCCOPY);

	dcTemp.SelectObject( pOld );
	::DeleteObject( TmpBmp );
	TmpBmp = NULL;
	dcTemp.DeleteDC();
	
	::ReleaseDC(m_hWnd, hDC);
}


void CPBookMain::ReleaseGdi()
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

void CPBookMain::OnExit()
{
	ReleaseGdi();
	g_GsmLogic.m_pbook.save_data( PBOOK_PATH_DEVICE );
	
	if ( m_IsGetItem )
		CDialogBase::EndDialog(IDOK);
	else
		CDialogBase::EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////

void CPBookMain::InitControl()
{
	int i;
 	for ( i =0;  i < 5; i++)
 	{
 		m_Item[i].nState = BTN_STATE_NORMAL;
 	}

	for ( i =0;  i < elist_count; i++)
	{
		m_list[i].nState = BTN_STATE_NORMAL;
		m_strPhone[i] = _T(""); 
		m_strName[i] = _T("");
	}


	//增加
	m_Item[0].rect.left		= 222;
	m_Item[0].rect.top		= 396;
	m_Item[0].rect.right	= 366;
	m_Item[0].rect.bottom	= 480;
	//清空
	m_Item[0].rect.left		= 368;
	m_Item[0].rect.top		= 396;
	m_Item[0].rect.right	= 512;
	m_Item[0].rect.bottom	= 480;
	//上一页
	m_Item[1].rect.left		= 514;
	m_Item[1].rect.top		= 396;
	m_Item[1].rect.right	= 655;
	m_Item[1].rect.bottom	= 480;
	//下一页
	m_Item[2].rect.left		= 657;
	m_Item[2].rect.top		= 396;
	m_Item[2].rect.right	= 800;
	m_Item[2].rect.bottom	= 480;
	//Exit
	m_Item[3].rect.left		= 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;

	//Add
	m_Item[4].rect.left		= 222;
	m_Item[4].rect.top		= 396;
	m_Item[4].rect.right	= 366;
	m_Item[4].rect.bottom	= 480;

	for(i=0; i<elist_count; i++)
	{
		m_list[i].rect.left		= 30-5;
		m_list[i].rect.top		= 86+52*i-5;
		m_list[i].rect.right	= 769+5;
		m_list[i].rect.bottom	= 85+52*i+40+5;
	}
}

void CPBookMain::DrawItems(CDC* pDC, const CRect &rt, enBtnState state)
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

void CPBookMain::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CPBookMain::OnPaint() 
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
	for(i=0;i<5;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	PSKINBMP_ST pSkin = g_pResMng->RequestBmp(SKIN_BTN_DRAFT_ADD, true);

	for( i = 0; i < elist_count; i++ )
	{
		DrawItems( &memDC,m_list[i].rect,m_list[i].nState);

		//if ( m_ItemState[i] == 1 ) //已读
		//	memDC.BitBlt(m_list[i].rect.left, m_list[i].rect.top, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
		//	         &bk_sms_flag, 0, pSkin->nHeightPerCell , SRCCOPY );
		//else 
		if ( m_ItemState[i] == 3 ) //SIM卡上SMS
			memDC.BitBlt(m_list[i].rect.left, m_list[i].rect.top, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			         &bk_sms_flag, 0,  pSkin->nHeightPerCell * 2, SRCCOPY );

		CRect rt(m_list[i].rect);
		if ( m_ItemState[i] == 3 )
			rt.left += pSkin->nWidthPerCell;
		rt.left += 5;
		DrawBtnText( &memDC,rt, m_strName[i], smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
		rt.left += rt.Width()/2 - 2;
		rt.right -= 5;
		DrawBtnText( &memDC,rt, m_strPhone[i], smallfont,RGB(255,255,255),DT_VCENTER|DT_RIGHT);
	}

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CPBookMain::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	
	if ( m_bOperation )
	{
		for( i = 0; i < 5; i++ )
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
	}

	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CPBookMain::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int i=0;
	if ( m_bOperation )
	{
		for(i=0;i<5;i++)
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
				case 4: OnBtnAdd(); break;
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
	}

	CDialogBase::OnLButtonUp(nFlags, point);
}


void  CPBookMain::OnBtnPageUp()
{
	if(m_nPageIndex <= 0)	//已到达第一页
	{
		m_nPageIndex = 0;
		return;
	}
	m_nPageIndex--;
	ShowPage( m_nPageIndex );
	Invalidate();
}

void  CPBookMain::OnBtnPageDown()
{
	if(m_nPageIndex >= m_nPageCount-1)//已到达最后一页
	{
		m_nPageIndex = m_nPageCount-1;
		return;
	}
	m_nPageIndex++;

	ShowPage( m_nPageIndex );
	Invalidate();
}

void  CPBookMain::OnBtnDelAll()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_ALL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	
	if( CDlgConfirm::s_bOk )
	{
// 		logic_pbook* pInbox = &( g_GsmLogic.m_pbook );
// 		pInbox->remove_all();
// 		g_GsmLogic.GSMDelSIMPBookAll();
// 		m_nCurPage = 1;
// 		ShowPage( m_nCurPage );
		// 		Invalidate();
		CppSQLite3DB db;
		db.open(PATH_SQLITE_DB_808);
		db.execDML( "DELETE FROM phone_book;" );
		db.close();

		m_nPageIndex	= 0;
		m_nPageCount	= 1;
		m_nRecordCount	= 0;

		ShowPage( m_nPageIndex );
	}
}

//////////////////////////////////////////////////////////////////////////

void  CPBookMain::OnBtnAdd()
{
	CDlgPhoneEdit dlg;
	dlg.SetPrevHwnd(this);
	CDlgPhoneEdit::m_PhoneName = _T("");
	CDlgPhoneEdit::m_PhoneNum = _T("");
	CDlgPhoneEdit::m_PhonePos = 0;
	CDlgPhoneEdit::m_AddPhone = TRUE;
	dlg.DoModal();

// 	int nMaxRd = SMSMaxNum();
// 	if ( m_nCurPage >= nMaxRd / elist_count + 1 )
// 	{
// 		if ( nMaxRd > 0 )
// 			m_nCurPage = ( nMaxRd - 1 ) / elist_count + 1;
// 		else
// 			m_nCurPage = ( nMaxRd ) / elist_count + 1;
// 	}

	ShowPage( m_nPageIndex );
}

void    CPBookMain::UserClickItem(int nItem )
{
// 	int  nSleIdx = nItem + m_nPageIndex * elist_count;
// 	if ( nSleIdx >= m_nRecordCount )
// 		return;
// 
// 	//CDlgSMSCenterInfo dlg(nSleIdx);
// 	//dlg.DoModal();
// 
// 	ShowPage( m_nPageIndex );
// 	Invalidate();
// 	return;

	//int nMaxRd = SMSMaxNum();
	//logic_pbook* pInbox = &(g_GsmLogic.m_pbook);
	int  nRecordIndex = nItem + m_nPageIndex * elist_count;
	if ( nRecordIndex >= m_nRecordCount )
		return;

	CDlgPhoneEdit dlg;
	dlg.SetPrevHwnd( this );
	CDlgPhoneEdit::m_PhoneName =m_strName[nItem];
	CDlgPhoneEdit::m_PhoneNum = m_strPhone[nItem];
	CDlgPhoneEdit::m_AddPhone = FALSE;
	dlg.SetEditType( FALSE, nRecordIndex );

// 	if ( nRecordIndex < pInbox->max_number() )
// 	{
// 		nRecordIndex = pInbox->max_number() - nRecordIndex - 1;
// 		dlg.SetEditType( FALSE, nRecordIndex );
// 	}else
// 	{
// 		int nUsed = g_GsmLogic.GSMGetPBookUsed();
// 		nRecordIndex = nItem + (m_nCurPage - 1) * elist_count;
// 		nRecordIndex = nRecordIndex - pInbox->max_number();
// 		if ( nRecordIndex < nUsed )
// 		{
// 			PBOOK_ITEM* Item = &(g_GsmLogic.GSMGetSIMPBookList()[nRecordIndex]);
// 			dlg.SetEditType( TRUE, Item->nIdx );
// 		}
// 	}

	dlg.DoModal();
	ShowPage( m_nPageIndex );

// 	nRecordIndex = SMSMaxNum();
// 	if ( m_nCurPage >= nRecordIndex / elist_count + 1 )
// 	{
// 		if ( nRecordIndex > 0 )
// 			m_nCurPage = ( nRecordIndex - 1 ) / elist_count + 1;
// 		else
// 			m_nCurPage = ( nRecordIndex ) / elist_count + 1;
// 	}
// 	ShowPage( m_nCurPage );
// 	Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/*
void    CPBookMain::ShowPage(int nPage)
{
	logic_pbook* pInbox = &(g_GsmLogic.m_pbook);
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
		m_strName[i] = _T("");
		m_strPhone[i] = _T("");
		m_list[i].nState = BTN_STATE_DISABLE;
	}
	
	for( i = 0; i < elist_count; i++ )
	{
		if ( nStartIdx >= nMaxRd )
			break;
		if ( nStartIdx < pInbox->max_number() )
		{
			PBOOK_ITEM* pSMS = pInbox->get_recourd( pInbox->max_number() - nStartIdx - 1 );
			if ( pSMS )
			{
				TCHAR szChar[64];
				memset(szChar, 0, sizeof(szChar) );
				ex_char2uni(pSMS->phone, szChar, sizeof(szChar)/sizeof(TCHAR) );
				m_strPhone[i].Format(_T("%s"), szChar);
				
				memset(szChar, 0, sizeof(szChar) );
				ex_char2uni(pSMS->name, szChar, sizeof(szChar)/sizeof(TCHAR) );
				m_strName[i].Format(_T("%s"), szChar);

				m_list[i].nState = BTN_STATE_NORMAL;
				m_ItemState[i] = 1;
			}
		}else
		{
			PBOOK_ITEM* pList = &(g_GsmLogic.GSMGetSIMPBookList()[nStartIdx-pInbox->max_number()]);
			if ( pList )
			{
				TCHAR szChar[64];
				memset(szChar, 0, sizeof(szChar) );
				ex_char2uni(pList->phone, szChar, sizeof(szChar)/sizeof(TCHAR) );
				m_strPhone[i].Format(_T("%s"), szChar);
				
				memset(szChar, 0, sizeof(szChar) );
				ex_char2uni(pList->name, szChar, sizeof(szChar)/sizeof(TCHAR) );
				m_strName[i].Format(_T("%s"), szChar);
				m_list[i].nState = BTN_STATE_NORMAL;
				m_ItemState[i] = 3;
			}
		}
		nStartIdx++;
	}

}*/

void CPBookMain::ShowPage(int nPageIndex)
{
	CString str;
	int i = 0;
	int nStartIndex = 0;
	int nOffset = 0;

	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	//查询记录总数量
	m_nRecordCount = db.execScalar("select count(*) from phone_book;");
	//计算总页数
	if(m_nRecordCount > 0)
		m_nPageCount = (m_nRecordCount-1)/elist_count+1;
	else
		m_nPageCount = 1;

	//在数据库中查询第nPageIndex页的elist_count条数据
	char szSqlBuffer[512];
	sprintf(szSqlBuffer, "SELECT * FROM phone_book ORDER BY name LIMIT %d, %d;", nPageIndex*elist_count, elist_count);
	CppSQLite3Query q = db.execQuery(szSqlBuffer);

	for( i = 0; i < elist_count; i++ )
	{
		if ( !q.eof() )	//数据行
		{
			m_strName[i]		= q.fieldValue("name");
			m_strPhone[i]		= q.fieldValue("phone_number");
			m_list[i].nState	= BTN_STATE_NORMAL;
			m_ItemState[i]		= 1;

			q.nextRow();
		}
		else			//空白行
		{
			m_strName[i]		= _T("");
			m_strPhone[i]		= _T("");
			m_list[i].nState	= BTN_STATE_DISABLE;
			m_ItemState[i]		= 1;
		}
	}
	//释放statement
	q.finalize();

	db.close();	//关闭数据库
	return;
}

BOOL CPBookMain::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
		if ( pMsg->wParam ==  GSM_SIM_PBOOK_READ )
		{
			m_nPageIndex = 0;
			ShowPage( m_nPageIndex );
			Invalidate();
// 			m_bOperation = TRUE;
// 			m_nCurPage = 1;
// 			if ( !g_GsmLogic.m_pbook.is_open() )
// 				g_GsmLogic.m_pbook.load_data( PBOOK_PATH_DEVICE );
// 			ShowPage( m_nCurPage );
// 			Invalidate();
		}else if ( pMsg->wParam == GSM_SIM_PBOOK_READ_PROCESS )
		{
			int nTotal = HIWORD( pMsg->lParam );
			int nCur = LOWORD( pMsg->lParam );
			CString str;
			str.Format( _T("(%d%%) %s"), 100 * (nCur+1) / (nTotal+1), m_strReadTip );
			m_strName[0] = str;
			m_ItemState[0] = 0;
			Invalidate();
		}
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

int   CPBookMain::SMSMaxNum()
{
	logic_pbook* pbook = &( g_GsmLogic.m_pbook );
	int nMax = pbook->max_number();
	/*
	if ( g_GsmLogic.GSMGetSIMPBookList() != 0  )
		nMax += g_GsmLogic.GSMGetPBookUsed();
	*/
	 return nMax;
}

