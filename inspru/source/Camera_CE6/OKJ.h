// OKJ.h : main header file for the OKJ application
//

#if !defined(AFX_OKJ_H__A6757E6F_B929_466B_A8A7_D19872C1F6C7__INCLUDED_)
#define AFX_OKJ_H__A6757E6F_B929_466B_A8A7_D19872C1F6C7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COKJApp:
// See OKJ.cpp for the implementation of this class
//


#define  SOUND_KEYCLICK   _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")
#define  SOUND_KEYCLICK1   _T("\\Flashdrv Storage\\menu\\Style\\KeySound1.wav\0")
#define  SOUND_KEYCLICK2   _T("\\Flashdrv Storage\\menu\\Style\\KeySound2.wav\0")
#define  SOUND_KEYCLICK3   _T("\\Flashdrv Storage\\menu\\Style\\KeySound3.wav\0")



enum enKeySound
{
	KeySnd_none = 0,
	KeySnd_1,
	KeySnd_2,
	KeySnd_3,
	KeySnd_4,
};


class COKJApp : public CWinApp
{
public:
	COKJApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COKJApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COKJApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OKJ_H__A6757E6F_B929_466B_A8A7_D19872C1F6C7__INCLUDED_)
