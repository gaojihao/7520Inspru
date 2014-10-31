// WndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OKJ.h"
#include "WndDlg.h"
#include "midware.h"
#include "OKJDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern COKJDlg *pDlg;
/////////////////////////////////////////////////////////////////////////////
// WndDlg dialog


WndDlg::WndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WndDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(WndDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void WndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WndDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(WndDlg, CDialog)
	//{{AFX_MSG_MAP(WndDlg)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WndDlg message handlers

void WndDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT nrect = {0};

	nrect.left = 133;
	nrect.top = 72;
	nrect.right = 133+647;
	nrect.bottom = 72+391;	
	CamSetRect(&nrect);

	CameraCtrl(TRUE);
	MoveWindow(0,0,0,0,0);

	pDlg->UpdateWindow();
	CWnd* pwndChild = pDlg->GetWindow(GW_CHILD); 
	while (pwndChild)
	{      
		pwndChild->UpdateWindow();
		pwndChild = pwndChild->GetNextWindow();
	} 

	
	CDialog::OnLButtonUp(nFlags, point);
}
