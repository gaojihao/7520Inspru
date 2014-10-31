
// DlgEventList.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgEventList.h"
#include "../DlgConfirm.h"
#include "DlgTextInfo.h"
#include "../E9101MainDlg.h"
#include "DlgMsgBox.h"
#include "../Msg/MsgQueue.h"

#include "../../GSM/diaodu_data.h"
#include "../../ExTool/ex_basics.h"
#include "../../MutiLanguage/CGDICommon.h"
#include "../../SQLite3/CppSQLite3.h"
#include "../E9101Main/DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT	g_nWM_MENU_GNSSTERMINAL;

//是否有新的中心信息
BOOL CDlgEventList::m_bSMSCenter = FALSE;
BOOL CDlgEventList::m_bIsOpen = FALSE;
UINT CDlgEventList::WM_DlgEventInfo_Refresh = RegisterWindowMessage(_T("WM_DlgEventInfo_Refresh"));

CDlgEventList::CDlgEventList(BOOL bIsModelDlg /*= TRUE*/, CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgEventList::IDD, pParent)
{
	m_nPageIndex	= 0;
	m_nPageCount	= 1;
	m_nRecordCount	= 0;

	m_bIsModelDlg	= bIsModelDlg;	//当前窗口是否为模态窗口
}

void CDlgEventList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfoMenu)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgEventList, CDialogBase)
	//{{AFX_MSG_MAP(CDlgInfoMenu)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgEventList::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	SetWindowText(_T("DlgEventInfo_WINDOW_NAME"));

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	m_bIsOpen = TRUE;

	ShowPage( m_nPageIndex );

	CSysConfig::Instance().SetLastDialogID(1);

	ChangeReadStatus();
	//UpdateData();

	return TRUE;
}
//变换语言和单位
BOOL CDlgEventList::OnLanguageChange()//变皮肤
{
	CResString str;

	m_csTitile = _T("事件/点播菜单");
	
	m_Item[0].chChar = _T("");
	m_Item[1].chChar = _T("");
	m_Item[2].chChar = _T("");
	m_Item[3].chChar = _T("");

	return TRUE;
}

BOOL CDlgEventList::OnSkinChange()
{
	return TRUE;
}

void CDlgEventList::InitGdi()
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
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_LIST_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BG_LIST_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 

	m_pold_bk_normal_bmp = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp  = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgEventList::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject( m_stBtnPressBMP );
	DeleteObject( m_stBtnNormalBMP );
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgEventList::OnExit()
{
	m_bIsOpen = FALSE;

	ReleaseGdi();
	//退出时,保存文件并且清空内存数据
	//CSMSCenter_data::Instance()->save_data(true);

	if(m_bIsModelDlg)
		CDialogBase::EndDialog(0);		//模态对话框
	else
		CDialogBase::DestroyWindow();	//非模态对话框

	CSysConfig::Instance().SetLastDialogID(0);
}

void CDlgEventList::InitControl()
{
	int i;
 	for ( i =0;  i < BTN_COUNT; i++)
 	{
 		m_Item[i].nState = BTN_STATE_NORMAL;
 	}

	for ( i =0;  i < elist_count; i++)
 	{
		m_list[i].nState = BTN_STATE_NORMAL;
 	}

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
		m_list[i].rect.left		= 30-5;
		m_list[i].rect.top		= 86+52*i-5;
		m_list[i].rect.right	= 769+5;
		m_list[i].rect.bottom	= 85+52*i+40+5;
	}
}

void CDlgEventList::DrawItems(CDC* pDC,const CRect &rt, enBtnState state)
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

void CDlgEventList::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgEventList::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
	int i;

	for(i=0;i<BTN_COUNT;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	//绘制列表
	DrawList(&memDC);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgEventList::DrawList(CDC* pmemDC)
{
	/*
	CString str;
	//计算最大页数
	int nMaxPage;
	if ( g_pSMSCenter->GetRecordCount() > 0 )
		nMaxPage = ( g_pSMSCenter->GetRecordCount() - 1 ) / elist_count + 1;
	else
		nMaxPage = 1;

	if ( m_nCurPage > nMaxPage ) 
		return;

	int nStartIdx =  ( m_nCurPage - 1 ) * elist_count;
	
	for( int i = 0; i < elist_count; i++ )
	{
		if ( nStartIdx+i >= g_pSMSCenter->GetRecordCount() )
			break;
		st_SMSCenter* pSMS = g_pSMSCenter->GetRecord( nStartIdx + i );
		if ( pSMS )
		{
			DrawItems( pmemDC, m_list[i].rect, m_list[i].nState);
			//内容
			str = pSMS->m_Text.m_infoContext;
			str = "  " + str;
			DrawBtnText( pmemDC, m_list[i].rect, str, 
				CGDICommon::Instance()->smallfont(), RGB(255,255,255),DT_VCENTER|DT_LEFT);
		}
	}*/
	int i = 0;
	CString str;
	for( i = 0; i < elist_count; i++ )
	{
		str = _T("  ") + m_list[i].chChar;
		DrawBtnText( pmemDC, m_list[i].rect, str, 
			CGDICommon::Instance()->smallfont(), RGB(255,255,255),DT_VCENTER|DT_LEFT);
	}
}

void CDlgEventList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i = 0;
	for( i = 0; i < BTN_COUNT; i++ )
	{
		if(PtInRect(&m_Item[i].rect, point))
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

void CDlgEventList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int i=0;
	for( i=0; i<BTN_COUNT; i++)
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
			//m_list[i].nState = BTN_STATE_NORMAL;
			//UserClickItem( i );
			
			//CResString str;
			//str.LoadString( RES_BUTTON_DELETE_ALL );
			CString str = _T("确定发送该条事件信息?");
			CDlgConfirm::m_stTitle = str;
			CDlgConfirm dlg;
			dlg.DoModal();

			if( CDlgConfirm::s_bOk )
			{
				UINT nID = m_nEvent_ID[i];
				CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, enMSG_ID::MSG_TMN_EVENT_REPORT, nID);

				CDlgMsgBox	dlgBox;
				CDlgMsgBox::SetTextInfo(_T("发送中..."));
				dlgBox.DoModal();

				m_nPageIndex	= 0;
				m_nPageCount	= 1;
				m_nRecordCount	= 0;

				ShowPage( m_nPageIndex );
			}
		}
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void  CDlgEventList::OnBtnPageUp()
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

void  CDlgEventList::OnBtnPageDown()
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

void  CDlgEventList::OnBtnDelAll()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_ALL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	
	if( CDlgConfirm::s_bOk )
	{
		CppSQLite3DB db;
		db.open(PATH_SQLITE_DB_808);
		db.execDML( "DELETE FROM event_info;" );
		db.close();

		m_nPageIndex	= 0;
		m_nPageCount	= 1;
		m_nRecordCount	= 0;

		ShowPage( m_nPageIndex );
	}
}

void CDlgEventList::UserClickItem(int nItem )
{
	int  nSleIdx = nItem + m_nPageIndex * elist_count;
	if ( nSleIdx >= m_nRecordCount )
		return;

	CDlgTextInfo dlg(nSleIdx);
	dlg.DoModal();

	ShowPage( m_nPageIndex );
	Invalidate();
}

void CDlgEventList::ShowPage(int nPageIndex)
{
	CString str;
	int i = 0;
	int nStartIndex = 0;
	int nOffset = 0;

	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	
	//查询记录总数量
	m_nRecordCount = db.execScalar("SELECT count(*) FROM event_info;");
	//计算总页数
	if(m_nRecordCount > 0)
		m_nPageCount = (m_nRecordCount-1)/elist_count+1;
	else
		m_nPageCount = 1;

	//在数据库中查询第nPageIndex页的elist_count条数据
	char szSqlBuffer[512];
	sprintf(szSqlBuffer, "SELECT * FROM event_info ORDER BY event_ID DESC LIMIT %d, %d;", nPageIndex*elist_count, elist_count);
	CppSQLite3Query q = db.execQuery(szSqlBuffer);

	for( i = 0; i < elist_count; i++ )
	{
		if ( !q.eof() )	//数据行
		{
			m_nEvent_ID[i]		= q.getIntField("event_ID");
			m_list[i].chChar	= q.fieldValue("event_content");
			m_list[i].nState	= BTN_STATE_NORMAL;
			q.nextRow();
		}
		else			//空白行
		{
			m_ItemState[i]		= 0;
			m_list[i].chChar	= _T("");
			m_list[i].nState	= BTN_STATE_DISABLE;
		}
	}
	//释放statement
	q.finalize();

	db.close();	//关闭数据库
	return;
}

LRESULT CDlgEventList::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_REFRESH_PAGE == message)
	{
		m_nPageIndex = 0;
		ShowPage( m_nPageIndex );
		Invalidate();
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgEventList::ChangeReadStatus()
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	int nUnReadSMS = 0;	//未读短信数量
	const char* pszSQL;
	//查询中心信息未读短信总数量
	pszSQL = "select count(*) from text_info where read_status = 0;";
	nUnReadSMS = db.execScalar(pszSQL);
	if(nUnReadSMS > 0)
	{
		//更新状态:未读->已读
		pszSQL = "update text_info set read_status = 1;";
        db.execDML(pszSQL);
	}
	db.close();
	m_bSMSCenter = FALSE;//将新信息提示关闭
}

void CDlgEventList::PostNcDestroy()
{
	CDialogBase::PostNcDestroy();

	if( !m_bIsModelDlg )	//非模态对话框下才能删除对话框对象
		delete this;
}

//窗口是否已经打开
BOOL CDlgEventList::IsOpen()
{
	return m_bIsOpen;
}