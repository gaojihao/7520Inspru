// SMSNew.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgReportState.h"
#include "E9101MainDlg.h"
#include "../SQLite3/CppSQLite3.h"
#include "../Msg/MsgQueue.h"
#include "../DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog
UINT CDlgReportState::WM_DlgReportState_Refresh = RegisterWindowMessage(_T("DlgReportState_Refresh"));

BOOL CDlgReportState::m_bIsOpen = FALSE;	//窗口是否已经打开

CDlgReportState::CDlgReportState(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgReportState::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgReportState::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReportState, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDlgReportState::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	SetWindowText(_T("DlgReportState_Refresh"));

	MoveWindow(0, 0, 494, 274);
	CenterWindow(GetDesktopWindow());
	//SetForegroundWindow();
	//BringWindowToTop();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	m_bIsOpen	= TRUE;
	SetTimer(TID_SET_TOP, 500, NULL);
	SetTimer(TID_TIME_OUT, 8*1000, NULL);

	return TRUE;
}
//变换语言和单位
BOOL CDlgReportState::OnLanguageChange()//变皮肤
{
	CResString str;
	
	m_csTitile = _T("状态发送中，请稍候...");
	
	return TRUE;
}

BOOL CDlgReportState::OnSkinChange()
{
	return TRUE;
}


void CDlgReportState::InitGdi()
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
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_CONFIRM2_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_BG_CONFIRM2_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgReportState::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgReportState::ReportState(WPARAM wParam)
{
	if(wParam != WPARAM_BTN_2 && wParam != WPARAM_BTN_3 && wParam != WPARAM_BTN_4 && wParam != WPARAM_BTN_5)
		return;

	//写入数据库
// 	CppSQLite3DB db;
// 	db.open(PATH_SQLITE_DB_808);	//打开数据库
// 	char szSql[512];
// 	memset(szSql, 0, sizeof(szSql));
	if(WPARAM_BTN_2 == wParam)
	{
		m_csTitile	= _T("\"到库\"") + m_csTitile;
		//sprintf(szSql, "INSERT INTO text_info(text_type, flag, text_content) VALUES(%d, %d, '出勤');", 2, 1);
	}
	else if(WPARAM_BTN_3 == wParam)
	{
		m_csTitile	= _T("\"离库\"") + m_csTitile;
		//sprintf(szSql, "INSERT INTO text_info(text_type, flag, text_content) VALUES(%d, %d, '待命');", 2, 1);
	}
	else if(WPARAM_BTN_4 == wParam)
	{
		m_csTitile	= _T("\"到站\"") + m_csTitile;
		//sprintf(szSql, "INSERT INTO text_info(text_type, flag, text_content) VALUES(%d, %d, '待命');", 2, 1);
	}
	else if(WPARAM_BTN_5 == wParam)
	{
		m_csTitile	= _T("\"离站\"") + m_csTitile;
		//sprintf(szSql, "INSERT INTO text_info(text_type, flag, text_content) VALUES(%d, %d, '待命');", 2, 1);
	}

	//db.execDML(szSql);
	//DWORD dwLastUID	= (DWORD)db.lastRowId();
	//db.close();

	//CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, enMSG_ID::MSG_TMN_TEXT_SMS, dwLastUID);
}

void CDlgReportState::OnExit()
{
	m_bIsOpen	= FALSE;
	ReleaseGdi();
	Close();
}

//窗口是否已经打开
BOOL CDlgReportState::IsOpen()
{
	return m_bIsOpen;
}

void CDlgReportState::InitControl()
{
	for (int i =0;  i < enCtrCount; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	//出勤
	m_Item[0].rect.left		= 0;
	m_Item[0].rect.top		= 0;
	m_Item[0].rect.right	= 0;
	m_Item[0].rect.bottom	= 0;
	//m_Item[0].chChar		= _T("出勤");
	//待命
	m_Item[1].rect.left		= 0;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 0;
	m_Item[1].rect.bottom	= 0;
	//m_Item[1].chChar		= _T("待命");
	//Exit
	m_Item[2].rect.left		= 433;
	m_Item[2].rect.top		= 0;
	m_Item[2].rect.right	= 494;
	m_Item[2].rect.bottom	= 53;
}

void CDlgReportState::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgReportState::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgReportState::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	
	for(int i=0;i<enCtrCount;i++)
	{
		//绘制背景
		str		= m_Item[i].chChar;
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}
	
	DrawBtnText(&memDC,CRect(0,0,494,274),m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgReportState::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<enCtrCount;i++)
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
	
	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgReportState::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<enCtrCount;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				//ReportState(i);
				break;
			case 1:
				//ReportState(i);
				break;
			case 2:
				//OnExit();
				SetTimer(TID_CLOSE, 1000, NULL);
				break;
			default:
				break;
			}
		}
	}
	
	Invalidate();
	//ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CDlgReportState::PostNcDestroy()
{
	CDialogBase::PostNcDestroy();
	delete this;
}

void CDlgReportState::OnTimer(UINT_PTR nIDEvent)
{
	if(TID_CLOSE == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnExit();
	}
	else if(TID_CLOSE_TIMEOUT == nIDEvent)
	{
		KillTimer(nIDEvent);
		//m_csTitile	= _T("发送超时，请重试!");
		//m_csTitile	= _T("发送成功!");
		Invalidate();
		SetTimer(TID_CLOSE, 1000, NULL);
	}
	else if(TID_SET_TOP == nIDEvent)
	{
		KillTimer(nIDEvent);
		SetWindowPos(&wndTopMost, 0, 0, 494, 274, SWP_SHOWWINDOW);
		CenterWindow(GetDesktopWindow());
		//SetTimer(TID_CLOSE_TIMEOUT, 5000, NULL);
	}
	else if (TID_TIME_OUT)
	{
		KillTimer(nIDEvent);
		m_csTitile	= _T("发送超时，请重试!");
		Invalidate();
		SetTimer(TID_CLOSE, 1000, NULL);
	}
	CDialogBase::OnTimer(nIDEvent);
}

LRESULT CDlgReportState::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_DlgReportState_Refresh == message)
	{
		// 		if (MSG_SVR_COMMON_ACK == wParam)
		// 		{
		// 			unCOMMON_ACK_RESULT* common_result = (unCOMMON_ACK_RESULT*)(lParam);
		// 
		// 			if (common_result->ack_result.wMsgID == MSG_TMN_EVENT_REPORT)
		// 			{
		if (lParam == 0)
		{
			KillTimer(TID_TIME_OUT);
			m_csTitile	= _T("发送成功!");
			SetTimer(TID_CLOSE_TIMEOUT, 2000, NULL);
		}
		else
		{
			KillTimer(TID_TIME_OUT);
			m_csTitile	= _T("发送失败!");
			SetTimer(TID_CLOSE_TIMEOUT, 2000, NULL);
		}
		//			}
		//		}
	}

	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
