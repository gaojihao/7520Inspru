#if !defined(AFX_BTNOFMAINMENU_H__820EFE13_4AD4_4F68_A6CD_E7FCCF080C1F__INCLUDED_)
#define AFX_BTNOFMAINMENU_H__820EFE13_4AD4_4F68_A6CD_E7FCCF080C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BtnOfMainMenu.h : header file
//
#include "ExButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBtnOfMainMenu window

class CBtnOfMainMenu : public CExButton
{
// Construction
public:
	CBtnOfMainMenu();

// Attributes
public:

// Operations
public:

	
	void SettextLeft(UINT iLeft);


	UINT m_iLeft;

	virtual BOOL OnSkinChange();
	virtual BOOL OnLanguageChange();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtnOfMainMenu)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBtnOfMainMenu();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBtnOfMainMenu)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTNOFMAINMENU_H__820EFE13_4AD4_4F68_A6CD_E7FCCF080C1F__INCLUDED_)
