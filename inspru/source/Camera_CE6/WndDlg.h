#if !defined(AFX_WNDDLG_H__C5E1A68F_7B1B_4572_9310_6CC647CC13BC__INCLUDED_)
#define AFX_WNDDLG_H__C5E1A68F_7B1B_4572_9310_6CC647CC13BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WndDlg dialog

class WndDlg : public CDialog
{
// Construction
public:
	WndDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(WndDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WndDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(WndDlg)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDDLG_H__C5E1A68F_7B1B_4572_9310_6CC647CC13BC__INCLUDED_)
