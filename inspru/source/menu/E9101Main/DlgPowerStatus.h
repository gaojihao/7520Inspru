#if !defined(AFX_DLGPOWERSTATUS_H__B4363E19_D9D1_48FA_AD0B_4B19D251453B__INCLUDED_)
#define AFX_DLGPOWERSTATUS_H__B4363E19_D9D1_48FA_AD0B_4B19D251453B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPowerStatus.h : header file
//
#include "DialogBase.h"
#include "ExStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPowerStatus dialog

class CDlgPowerStatus : public CDialogBase
{
// Construction
public:
	static int m_nPic;
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	void ShowPowerStatus();

	void HidePowerStatus();
	CDlgPowerStatus(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgPowerStatus)
	enum { IDD = IDD_DIALOG_POWERSTATUS };
	CExStatic	m_stcPowerStatus;
	BOOL        m_bVisible;
	//}}AFX_DATA
private:
	POWERSTAT_E m_PowerState;
	int m_nPercent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPowerStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPowerStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnPowerStatus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOWERSTATUS_H__B4363E19_D9D1_48FA_AD0B_4B19D251453B__INCLUDED_)
