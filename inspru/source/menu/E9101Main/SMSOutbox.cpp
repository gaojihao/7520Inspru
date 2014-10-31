// SMSOutbox.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "SMSOutbox.h"
#include "../GSM/GSMLogic.h"
#include "../ExTool/ex_basics.h"
#include "DlgConfirm.h"
#include "SMSInfo.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../SQLite3/CppSQLite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSOutbox dialog


CSMSOutbox::CSMSOutbox(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSMSOutbox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTotalPage = 0;
	m_nCurPage = 1;
}


void CSMSOutbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSOutbox, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CSMSOutbox::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();	
	OnLanguageChange();
	OnSkinChange();

	ShowPage( m_nCurPage );

// #if 0
// 	if ( g_GsmLogic.GetSIMState() && !g_GsmLogic.GSMIsReadSIM() )
// 	{
// 		SetTimer( TID_GSM_READ_SIM, 50, NULL );
// 	}else{
// 		m_nCurPage = 1;
// 		if ( !g_GsmLogic.m_inbox.is_open() )
// 			g_GsmLogic.m_inbox.load_data( SMS_PATH_INBOX );
// 		ShowPage( m_nCurPage );
// 		
// 	}
// #endif
// 
// #if 1
// 		m_nCurPage = 1;
// 		if ( !g_GsmLogic.m_inbox.is_open() )
// 			g_GsmLogic.m_inbox.load_data( SMS_PATH_INBOX );
// 		ShowPage( m_nCurPage );
// #endif 
	
	return TRUE;
}

BOOL CSMSOutbox::OnLanguageChange()
{

	CResString str;
	
	str.LoadString(RES_BUTTON_SMS_OUTOBX);
	m_csTitile = str;
	
	m_Item[0].chChar = _T("");
	m_Item[1].chChar = _T("");
	m_Item[2].chChar = _T("");
	m_Item[3].chChar = _T("");


	return TRUE;
}

BOOL CSMSOutbox::OnSkinChange()
{
	return TRUE;
}

void CSMSOutbox::InitGdi()
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
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_LIST_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BG_LIST_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;
	
	m_pold_bk_normal_bmp = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp  = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}


void CSMSOutbox::ReleaseGdi()
{

	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject( m_stBtnPressBMP );
	DeleteObject( m_stBtnNormalBMP );
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CSMSOutbox::OnExit()
{
	ReleaseGdi();
	g_GsmLogic.m_outbox.save_data( SMS_PATH_OUTBOX );
	CDialogBase::EndDialog(0);
}


void CSMSOutbox::InitControl()
{
	int i;
 	for ( i =0;  i < 4; i++)
 	{
 		m_Item[i].nState = BTN_STATE_NORMAL;
 	}

	for ( i =0;  i < elist_count; i++)
		m_list[i].nState = BTN_STATE_NORMAL;


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

	//短信列表
	for(i=0; i<elist_count; i++)
	{
		m_list[i].rect.left		= 30;
		m_list[i].rect.top		= 86+52*i-5;
		m_list[i].rect.right	= 769+5;
		m_list[i].rect.bottom	= 85+52*i+40+5;
	}
}

void CSMSOutbox::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CSMSOutbox::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CSMSOutbox::OnPaint() 
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

	for( i=0; i<elist_count; i++ )
	{
		DrawItems( &memDC,m_list[i].rect,m_list[i].nState);
		DrawBtnText( &memDC,m_list[i].rect, m_list[i].chChar, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	}

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CSMSOutbox::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CSMSOutbox::OnLButtonUp(UINT nFlags, CPoint point) 
{
// 	// TODO: Add your message handler code here and/or call default
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


void  CSMSOutbox::OnBtnPageUp()
{
	if (m_nCurPage <= 1)
	{
		m_nCurPage = 1;
	}
	else
	{
		m_nCurPage--;
	}

	ShowPage(m_nCurPage);
	Invalidate();
}

void  CSMSOutbox::OnBtnPageDown()
{
// 	m_nCurPage++;
// 	int nMaxNum = SMSMaxNum();
// 	int MaxPage = 1 + nMaxNum / elist_count;
// 	if ( nMaxNum > 0 )
// 		MaxPage = (nMaxNum - 1) / elist_count + 1;
// 	if ( m_nCurPage > MaxPage ) 
// 		m_nCurPage = MaxPage;
	
	if (m_nCurPage >= m_nTotalPage)
	{
		m_nCurPage = m_nTotalPage;
	}
	else
	{
		m_nCurPage++;
	}

	ShowPage(m_nCurPage);
	Invalidate();
}


void  CSMSOutbox::OnBtnDelAll()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_ALL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	
	if( CDlgConfirm::s_bOk )
	{
		// 		logic_data* pInbox = &( g_GsmLogic.m_inbox );
		// 		pInbox->remove_all();
		// 		g_GsmLogic.GSMDelSIMSMSAll();

		CppSQLite3DB db;
		db.open(PATH_SQLITE_DB_808);	//打开数据库

		char szSqlBuffer[512];
		memset(szSqlBuffer, 0, sizeof(szSqlBuffer));

		UINT Tag = 2;
		sprintf(szSqlBuffer, "DELETE FROM sms WHERE sms_type = '%d';", Tag);
		db.execDML(szSqlBuffer);
		db.close();
	}

	m_nCurPage = 1;
	ShowPage( m_nCurPage );
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void    CSMSOutbox::UserClickItem(int nItem )
{
	int nID = 0;
	nID = m_nUID[nItem];

	CSMSInfo dlg;
	dlg.SetReplyBtn(false);
	dlg.SetLogicData(nID);
	dlg.DoModal();

	ShowPage( m_nCurPage );
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void    CSMSOutbox::ShowPage(int nPage)
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	CppSQLite3Table t;
	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	CppSQLite3Query q;
	
	//查询记录总数量
	t = db.getTable("select * from sms where sms_type = 2;");
	m_nRecordCount  = t.numRows();
	UINT m_nTag = 2;

	//在数据库中查询第nPageIndex页的SDCount条数据
	sprintf(szSqlBuffer, "SELECT * FROM sms where UID and sms_type = %d ORDER BY read_status, sms_datatime DESC LIMIT %d, %d;", m_nTag, (nPage-1)*elist_count, elist_count);
	q = db.execQuery(szSqlBuffer);

	//计算总页数
	if(m_nRecordCount > 0)
		m_nTotalPage = (m_nRecordCount-1)/elist_count+1;
	else
		m_nTotalPage = 1;

	for(int j=0; j<elist_count; j++)
	{
		m_list[j].chChar = _T("");
	}

	for( int i=0; i<elist_count; i++ )
	{
		if ( !q.eof() )	//数据行
		{
			m_nUID[i] = q.getIntField("UID");
			//m_ItemState[i] = q.getIntField("read_status");

			const char* content = q.fieldValue("sms_content");
			m_list[i].chChar = content;
			m_list[i].nState = BTN_STATE_NORMAL;
			q.nextRow();
		}
		else			//空白行
		{
			m_ItemState[i]	= 4;
			m_list[i].chChar	= _T("");
			m_list[i].nState	= BTN_STATE_DISABLE;
		}
	}

	//释放statement
	q.finalize();
	db.close();	//关闭数据库
}
	
