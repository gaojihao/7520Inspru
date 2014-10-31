// GTrans.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GTrans.h"
#include "GTransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "./Lib/IOControl.lib")

BEGIN_MESSAGE_MAP(CGTransApp, CWinApp)
END_MESSAGE_MAP()


// CGTransApp construction
CGTransApp::CGTransApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGTransApp object
CGTransApp theApp;

// CGTransApp initialization

#define UNIQE_NAME _T("{GPRSSvr200904241706")
BOOL CGTransApp::InitInstance()
{
	m_hOneInstance=CreateMutex( NULL, FALSE, UNIQE_NAME );
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CGTransDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CGTransApp::ExitInstance() 
{
	if ( m_hOneInstance )
		CloseHandle( m_hOneInstance );
	m_hOneInstance = 0;
	return CWinApp::ExitInstance();
}