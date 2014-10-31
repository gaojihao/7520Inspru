// GNSSTerminal.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif


// CGNSSTerminalApp:
// See GNSSTerminal.cpp for the implementation of this class
//

class CGNSSTerminalApp : public CWinApp
{
public:
	CGNSSTerminalApp();
	
// Overrides
public:
	virtual BOOL InitInstance();
	void InitSQLite();
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGNSSTerminalApp theApp;
