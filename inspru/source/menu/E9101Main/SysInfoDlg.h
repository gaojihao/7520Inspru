#if !defined(AFX_SYSINFODLG_H__5839FF85_51B0_49BD_AF90_D9D74289AEBC__INCLUDED_)
#define AFX_SYSINFODLG_H__5839FF85_51B0_49BD_AF90_D9D74289AEBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysInfoDlg.h : header file
//

#include "DialogBase.h"


/////////////////////////////////////////////////////////////////////////////
// CSysInfoDlg dialog

class CSysInfoDlg : public CDialogBase
{
// Construction
public:
	CSysInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysInfoDlg)
	enum { IDD = IDD_SYSINFO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysInfoDlg)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSINFODLG_H__5839FF85_51B0_49BD_AF90_D9D74289AEBC__INCLUDED_)
