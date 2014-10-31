// MaskCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "MaskCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 45
/////////////////////////////////////////////////////////////////////////////
// CMaskCtrl

CMaskCtrl::CMaskCtrl()
{

}

CMaskCtrl::~CMaskCtrl()
{
	m_BackDC.DeleteDC();
	m_BackBmp.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMaskCtrl, CStatic)
	//{{AFX_MSG_MAP(CMaskCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskCtrl message handlers

void CMaskCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Initial();
	CStatic::PreSubclassWindow();
}

void CMaskCtrl::Initial()
{
	ASSERT(IsWindow(GetSafeHwnd()));

	if(m_BackDC.GetSafeHdc())
	{
		m_BackDC.DeleteDC();
	}

	///Create background DC and buffer DC
	CDC *pDC=GetDC();
	m_BackDC.	CreateCompatibleDC(pDC);
	m_BackBmp.CreateCompatibleBitmap(pDC,SCREEN_WIDTH,SCREEN_HEIGHT);
	ReleaseDC(pDC);

	CRect  rtScreen(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	MoveWindow(&rtScreen,FALSE);

}

void CMaskCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	// TODO: Add your message handler code here
	CBitmap* pOldBackBmp = m_BackDC.SelectObject(&m_BackBmp);
	dc.BitBlt(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		&m_BackDC,0,0,SRCCOPY);
	m_BackDC.SelectObject(pOldBackBmp);;
	// Do not call CStatic::OnPaint() for painting messages
}

void CMaskCtrl::Active(BOOL bActive)
{
	if(bActive)
	{
		CBitmap* pOldBackBmp = m_BackDC.SelectObject(&m_BackBmp);

		HDC hDC = ::GetDC(NULL);	
		::BitBlt(m_BackDC.GetSafeHdc(),0,0,SCREEN_WIDTH,SCREEN_HEIGHT,hDC,0,0,SRCCOPY);
		::ReleaseDC(NULL, hDC);

		m_BackDC.SelectObject(pOldBackBmp);

		BringWindowToTop();
		ShowWindow(SW_SHOW);
		UpdateWindow();
	}
	else
	{
		UpdateWindow();
		ShowWindow(SW_HIDE);	
	}
}
