#if !defined(AFX_EXBUTTON_H__F7CE1B61_0601_4A17_A7D7_3900A0935F9D__INCLUDED_)
#define AFX_EXBUTTON_H__F7CE1B61_0601_4A17_A7D7_3900A0935F9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExButton.h : header file
//
#include "SkinPub.h"
class  IResMng;

class CExButton : public CButton
{
// Construction
public:
	CExButton();

public:
	CExButton&     SetBtnCol(BYTE NorCol, BYTE SelCol, BYTE DisCol);
	CExButton&     SetBtnResID(unsigned nResID);
	CExButton&     SetBtnStyle(DWORD dwStyle);
	CExButton&     SetBtnFontID(BYTE nFontID);
//	CExButton&     SetBtnTitle(CString& strText);
	CExButton&     GetBtnCol(BYTE& NorCol, BYTE& SelCol, BYTE& DisCol);
	CExButton&     GetBtnStyle(DWORD& dwStyle);

	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	BTN_DRAW_ITEM    m_BtnDrawItem;
	static IResMng*  s_pResMng;
	CString          m_strTitle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXBUTTON_H__F7CE1B61_0601_4A17_A7D7_3900A0935F9D__INCLUDED_)
