// SysInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "SysInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysInfoDlg dialog
TCHAR MyInfo[] = { 0x49, 0x20 , 0x61, 0x6d, 0x20 ,0x6c, 0x75, 0x68, 0x61, 0x69, 0x20, 0x21, 0x0a,0x0d,
	0x6c,0x75,0x68,0x61,0x69,0x40,0x31,0x32,0x36,0x2e,0x63,0x6f,0x6d,0x0a,0x0d,
	0x4d,0x53,0x4e,0x20,0x3a,0x20,0x6c,0x75,0x68,0x61,0x69,0x5f,0x63,0x6e,0x40,0x68,0x6f,0x74,0x6d,0x61,0x69,0x6c,0x2e,0x63,0x6f,0x6d,0x0a,0x0d,
	0x51,0x51,0x20,0x3a,0x20,0x31,0x30,0x36,0x36,0x30,0x33,0x37,0x36,0x00 };

CSysInfoDlg::CSysInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSysInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSysInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysInfoDlg, CDialogBase)
	//{{AFX_MSG_MAP(CSysInfoDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysInfoDlg message handlers

typedef struct PLATFORMVERSION_S {
 WCHAR NBOOT_VERSION[16];
 WCHAR EBOOT_VERSION[16];
 WCHAR NKBIN_VERSION[16];
 WCHAR SOFTWARE_VERSION[16];
 WCHAR HWBOARD_VERSION[16];
 WCHAR BUILD_DATE[32];
 WCHAR PAD[400];
} PLATFORMVERSION_T, *PPLATFORMVERSION_T;


void CSysInfoDlg::OnPaint() 
{
	// TODO: Add your message handler code here
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect(&rt);

	CBrush brush(RGB(0,125,125));
	dc.FillRect(&rt,&brush);
	brush.DeleteObject();	
	SetTextColor( dc.GetSafeHdc(), RGB(0,0,0));
	::SetBkMode(dc.GetSafeHdc(),TRANSPARENT);

    PLATFORMVERSION_T *platver;
    TCHAR g_szPlatform[512];
 
	SystemParametersInfo(SPI_GETPLATFORMTYPE, sizeof(g_szPlatform), g_szPlatform, FALSE);
	platver = (PLATFORMVERSION_T*) g_szPlatform;


	CString str;

	str = _T("Version Info");
	dc.DrawText(str, CRect(120,10,320,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );

	str = platver->NBOOT_VERSION;
	dc.DrawText(str, CRect(10,30,160,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );

	str = platver->EBOOT_VERSION;
	dc.DrawText(str, CRect(10,50,160,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );

	str = platver->NKBIN_VERSION;
	dc.DrawText(str, CRect(10,70,160,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );

	str = platver->SOFTWARE_VERSION;
	dc.DrawText(str, CRect(10,90,160,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );

	str = platver->HWBOARD_VERSION;
	dc.DrawText(str, CRect(10,110,160,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );

	str = platver->BUILD_DATE;
	dc.DrawText(str, CRect(10,310,160,240) , DT_TOP | DT_LEFT | DT_SINGLELINE );
	
	// Do not call CDialog::OnPaint() for painting messages
}

	DWORD Tick3 = 0 ;
	DWORD Tick4 = 0;

LRESULT CSysInfoDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( WM_LBUTTONDOWN == message )
	{
		CRect rt(300, 200, 320, 240);
		CPoint pt;
		pt.x = LOWORD( lParam );
		pt.y = HIWORD( lParam );

		if( rt.PtInRect( pt ) )
		{
			Tick3 = ::GetTickCount();
		}
		else
		{
			EndDialog( 0 );
		}
	}
	else if(WM_LBUTTONUP == message)
	{
		CRect rt(300, 200, 320, 240);
		CPoint pt;
		pt.x = LOWORD( lParam );
		pt.y = HIWORD( lParam );

		if( rt.PtInRect( pt ) )
		{
			Tick4 = ::GetTickCount() - Tick3;

			if( Tick4 > 5000 )
			{

		 		MessageBox(MyInfo);
			}
		}
	}
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
