// WirelessManager.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CWirelessManagerApp:
// See WirelessManager.cpp for the implementation of this class
//
//WirelessManager.exe窗口句柄
#define	WND_NAME_WIRELESS_MANAGER					_T("WND_WIRELESS_MANAGER")
//GSM_AT.exe进程窗口句柄
#define WND_NAME_GSM_AT								_T("WND_GSM_AT")
//menu.exe主窗口名称
#define WND_NAME_MENU								_T("WND_MAIN_MENU")
//GNSSTERMINAL主窗口名称
#define	GNSS_PROCESS_TEXT							_T("WND_GNSSTERMINAL")

class CWirelessManagerApp : public CWinApp
{
public:
	CWirelessManagerApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWirelessManagerApp theApp;
