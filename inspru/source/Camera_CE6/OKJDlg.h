// OKJDlg.h : header file
//
#if !defined(AFX_OKJDLG_H__21CFC74C_974B_49E6_9F9E_E0F8DAF00BEF__INCLUDED_)
#define AFX_OKJDLG_H__21CFC74C_974B_49E6_9F9E_E0F8DAF00BEF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/*****************************************************/

class COKJDlg : public CDialog
{

public:
	COKJDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COKJDlg)
	enum { IDD = IDD_OKJ_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CStatic	m_stcVideo;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COKJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON		m_hIcon;
	void OnExit();
	void	OnCmdCOPYDATA(WPARAM wParam, LPARAM lParam );
	
	// Generated message map functions
	//{{AFX_MSG(COKJDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCam1();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnStaticVideo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OKJDLG_H__21CFC74C_974B_49E6_9F9E_E0F8DAF00BEF__INCLUDED_)
