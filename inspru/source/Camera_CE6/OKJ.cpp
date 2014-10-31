// OKJ.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OKJ.h"
#include "OKJDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COKJApp

BEGIN_MESSAGE_MAP(COKJApp, CWinApp)
	//{{AFX_MSG_MAP(COKJApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COKJApp construction

COKJApp::COKJApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COKJApp object

COKJApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COKJApp initialization

BOOL COKJApp::InitInstance()
{
	//只启动一个进程
	HANDLE hOneInstance=CreateMutex(NULL,FALSE,_T("CAMERA_ONE_INSTANCE"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return FALSE;

	COKJDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
