// SMSDraft.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgAnswerList.h"
#include "../DlgConfirm.h"
#include "DlgAnswer.h"
#include "../E9101MainDlg.h"

#include "../../GSM/diaodu_data.h"
#include "../../ExTool/ex_basics.h"
#include "../../MutiLanguage/CGDICommon.h"

#include "../../SQLite3/CppSQLite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void TTSSpeaking(CString csText);
//�Ƿ����µ�������Ϣ
BOOL CDlgAnswerList::m_bSMSCenter = FALSE;
BOOL CDlgAnswerList::m_bIsOpen = FALSE;
UINT CDlgAnswerList::WM_DlgAnswerList_Refresh = RegisterWindowMessage(_T("WM_DlgAnswerList_Refresh"));

CDlgAnswerList::CDlgAnswerList(int nIndexCurr, CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgAnswerList::IDD, pParent)
{
	m_nPageIndex	= 0;
	m_nPageCount	= 1;
	m_nRecordCount	= 0;
	m_nQuestionIndex= nIndexCurr;
}

void CDlgAnswerList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgAnswerList, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgAnswerList::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	SetWindowText(_T("DlgAnswerList_WINDOW_NAME"));

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
//�任���Ժ͵�λ
BOOL CDlgAnswerList::OnLanguageChange()//��Ƥ��
{
	CResString str;

	m_csTitile = _T("���б�");
	
	m_Item[0].chChar = _T("");
	m_Item[1].chChar = _T("");
	m_Item[2].chChar = _T("");
	m_Item[3].chChar = _T("");

	return TRUE;
}

BOOL CDlgAnswerList::OnSkinChange()
{
	return TRUE;
}

void CDlgAnswerList::InitGdi()
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

void CDlgAnswerList::ReleaseGdi()
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

void CDlgAnswerList::OnExit()
{
	m_bIsOpen = FALSE;

	ReleaseGdi();

	CDialogBase::EndDialog(0);		//ģ̬�Ի���

	CSysConfig::Instance().SetLastDialogID(0);
}

void CDlgAnswerList::InitControl()
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

	//���
	m_Item[0].rect.left		= 368;
	m_Item[0].rect.top		= 396;
	m_Item[0].rect.right	= 512;
	m_Item[0].rect.bottom	= 480;
	//��һҳ
	m_Item[1].rect.left		= 514;
	m_Item[1].rect.top		= 396;
	m_Item[1].rect.right	= 655;
	m_Item[1].rect.bottom	= 480;
	//��һҳ
	m_Item[2].rect.left		= 657;
	m_Item[2].rect.top		= 396;
	m_Item[2].rect.right	= 800;
	m_Item[2].rect.bottom	= 480;
	//Exit
	m_Item[3].rect.left		= 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;

	//�����б�
	for(i=0; i<elist_count; i++)
	{
		m_list[i].rect.left		= 30-5;
		m_list[i].rect.top		= 86+52*i-5;
		m_list[i].rect.right	= 769+5;
		m_list[i].rect.bottom	= 85+52*i+40+5;
	}
}

void CDlgAnswerList::DrawItems(CDC* pDC,const CRect &rt, enBtnState state)
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

void CDlgAnswerList::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgAnswerList::OnPaint() 
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
	{	//���Ʊ���
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	//�����б�
	DrawList(&memDC);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgAnswerList::DrawList(CDC* pmemDC)
{
	/*
	CString str;
	//�������ҳ��
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
			//����
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

void CDlgAnswerList::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CDlgAnswerList::OnLButtonUp(UINT nFlags, CPoint point) 
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
			m_list[i].nState = BTN_STATE_NORMAL;
			UserClickItem( i );
		}
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void  CDlgAnswerList::OnBtnPageUp()
{
	if(m_nPageIndex <= 0)	//�ѵ����һҳ
	{
		m_nPageIndex = 0;
		return;
	}
	m_nPageIndex--;
	ShowPage( m_nPageIndex );
	Invalidate();
}

void  CDlgAnswerList::OnBtnPageDown()
{
	if(m_nPageIndex >= m_nPageCount-1)//�ѵ������һҳ
	{
		m_nPageIndex = m_nPageCount-1;
		return;
	}
	m_nPageIndex++;
	
	ShowPage( m_nPageIndex );
	Invalidate();
}

void  CDlgAnswerList::OnBtnDelAll()
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
		db.execDML( "DELETE FROM answer;" );
		db.close();

		m_nPageIndex	= 0;
		m_nPageCount	= 1;
		m_nRecordCount	= 0;

		ShowPage( m_nPageIndex );
	}
}

void    CDlgAnswerList::UserClickItem(int nItem )
{
// 	int  nSleIdx = nItem + m_nPageIndex * elist_count;
// 	if ( nSleIdx >= m_nRecordCount )
// 		return;

	CDlgAnswer dlg(m_nQuestionIndex, m_nAnswerID[nItem]);
	dlg.DoModal();
 
	ShowPage( m_nPageIndex );
	Invalidate();
}

void    CDlgAnswerList::ShowPage(int nPageIndex)
{
	CString str = _T("");
	int i = 0;
	int nStartIndex = 0;
	int nOffset = 0;
	
	CppSQLite3Table t;
	CppSQLite3Query q;
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//�����ݿ�
	char szSqlBuffer[1024];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	//��ѯ��¼������
// 	sprintf(szSqlBuffer, "SELECT COUNT(*) FROM answer \
// 						 WHERE UID IN (SELECT UID FROM question ORDER BY question_datatime DESC LIMIT %d, 1) \
// 						 ORDER BY answer_ID;", 
// 						 m_nQuestionIndex);
// 	m_nRecordCount = db.execScalar( szSqlBuffer );

	sprintf(szSqlBuffer, "SELECT * FROM answer where UID = '%d';", m_nQuestionIndex);
	t = db.getTable(szSqlBuffer);
	m_nRecordCount = t.numRows();
	
	//������ҳ��
	if(m_nRecordCount > 0)
		m_nPageCount = (m_nRecordCount-1)/elist_count+1;
	else
		m_nPageCount = 1;

	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	sprintf(szSqlBuffer, "SELECT * FROM question where UID = '%d';", m_nQuestionIndex);
	q = db.execQuery(szSqlBuffer);
	int nFlag = q.getIntField("flag");
	const char* ccText = q.fieldValue("question_content");
	str = ccText;

	//�����ݿ��в�ѯ��nPageIndexҳ��elist_count������
// 	sprintf(szSqlBuffer, "SELECT * FROM answer \
// 						 WHERE UID IN (SELECT UID FROM question ORDER BY question_datatime DESC LIMIT %d, 1) \
// 						 ORDER BY answer_ID LIMIT %d, %d;", 
// 						 m_nQuestionIndex, nPageIndex*elist_count, elist_count);
// 	q = db.execQuery(szSqlBuffer);

	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	sprintf(szSqlBuffer, "SELECT * FROM answer where UID = '%d' LIMIT %d, %d;", 
				m_nQuestionIndex, (nPageIndex)*elist_count, elist_count);
	q = db.execQuery(szSqlBuffer);

	for( i = 0; i < elist_count; i++ )
	{
		if ( !q.eof() )	//������
		{
			m_nAnswerID[i]		= q.getIntField("answer_ID");
			m_list[i].chChar	= q.fieldValue("answer_content");
			m_list[i].nState	= BTN_STATE_NORMAL;
			q.nextRow();
		}
		else			//�հ���
		{
			m_ItemState[i]		= 0;
			m_list[i].chChar	= _T("");
			m_list[i].nState	= BTN_STATE_DISABLE;
		}
	}
	//�ͷ�statement
	q.finalize();
	db.close();	//�ر����ݿ�

	if(nFlag)
		TTSSpeaking( str );

	return;
}

LRESULT CDlgAnswerList::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_DlgAnswerList_Refresh == message)
	{
		m_nPageIndex = 0;
		m_nQuestionIndex = lParam;
		ShowPage( m_nPageIndex );
		Invalidate();
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgAnswerList::ChangeReadStatus()
{
	return;

	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//�����ݿ�
	int nUnReadSMS = 0;	//δ����������
	const char* pszSQL;
	//��ѯ������Ϣδ������������
	pszSQL = "select count(*) from text_info where read_status = 0;";
	nUnReadSMS = db.execScalar(pszSQL);
	if(nUnReadSMS > 0)
	{
		//����״̬:δ��->�Ѷ�
		pszSQL = "update text_info set read_status = 1;";
        db.execDML(pszSQL);
	}
	db.close();
	m_bSMSCenter = FALSE;//������Ϣ��ʾ�ر�
}

//�����Ƿ��Ѿ���
BOOL CDlgAnswerList::IsOpen()
{
	return m_bIsOpen;
}