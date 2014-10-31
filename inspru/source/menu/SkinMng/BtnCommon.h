#if !defined(AFX_BTNCOMMON_H__44C12F32_CD47_439D_8BAA_3732CEF6B904__INCLUDED_)
#define AFX_BTNCOMMON_H__44C12F32_CD47_439D_8BAA_3732CEF6B904__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BtnCommon.h : header file
//
#include "ExButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBtnCommon window

class CBtnCommon : public CExButton
{
// Construction
public:
	CBtnCommon();

// Attributes
public:

// Operations
public:
	virtual BOOL OnSkinChange();
	virtual BOOL OnLanguageChange();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtnCommon)
	protected:
	virtual void PreSubclassWindow();
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBtnCommon();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBtnCommon)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTNCOMMON_H__44C12F32_CD47_439D_8BAA_3732CEF6B904__INCLUDED_)
