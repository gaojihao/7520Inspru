// GTrans.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CGTransApp:
// See GTrans.cpp for the implementation of this class
//

class CGTransApp : public CWinApp
{
public:
	CGTransApp();
	
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	HANDLE m_hOneInstance;

	DECLARE_MESSAGE_MAP()
};

extern CGTransApp theApp;
