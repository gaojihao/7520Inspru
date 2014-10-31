#if !defined(AFX_MASKCTRL_H__76A2E2C2_5C55_45B1_A466_29B789DF0D5A__INCLUDED_)
#define AFX_MASKCTRL_H__76A2E2C2_5C55_45B1_A466_29B789DF0D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaskCtrl.h : header file


/////////////////////////////////////////////////////////////////////////////
// CMaskCtrl window

class CMaskCtrl : public CStatic
{
// Construction
public:
	CMaskCtrl();
	virtual ~CMaskCtrl();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaskCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Active (BOOL bActive);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMaskCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	CDC				m_BackDC;
	CBitmap			m_BackBmp;
	void Initial();	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASKCTRL_H__76A2E2C2_5C55_45B1_A466_29B789DF0D5A__INCLUDED_)
