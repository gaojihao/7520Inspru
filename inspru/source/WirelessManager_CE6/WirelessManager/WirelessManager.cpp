// WirelessManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WirelessManager.h"
#include "WirelessManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UNIQE_NAME _T("{WIRELESSMANAGER_20110211")

BEGIN_MESSAGE_MAP(CWirelessManagerApp, CWinApp)
END_MESSAGE_MAP()

CWirelessManagerApp::CWirelessManagerApp()
	: CWinApp()
{
}

CWirelessManagerApp theApp;

BOOL CWirelessManagerApp::InitInstance()
{
	CreateMutex( NULL, FALSE, UNIQE_NAME );
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}

	TCHAR	szOSVersion[64] = {0};
	SystemParametersInfo(SPI_GETOEMINFO, sizeof(szOSVersion), szOSVersion, 0);
	if( _tcscmp(szOSVersion, _T("Samsung Electronics")) != 0 )
		return FALSE;

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CWirelessManagerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
