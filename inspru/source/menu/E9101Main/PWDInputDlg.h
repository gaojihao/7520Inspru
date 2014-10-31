// PWDInputDlg.h : header file
//

#if !defined(AFX_PWDINPUTDLG_H__929D284E_3B80_3256_B4C8_A93B5917DD08__INCLUDED_)
#define AFX_PWDINPUTDLG_H__929D284E_3B80_3256_B4C8_A93B5917DD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SysConfig.h"
#include "DialogBase.h"
#include "MaskCtrl.h"
#include "BtnOfMainMenu.h"
#include "resource.h"
#include "e9101main.h"

enum enObject{
	IPSETDLG,
	DIALDLG
};

class CPWDInputDlg : public CDialogBase
{
public:
	CPWDInputDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPWDInputDlg)
	enum { IDD = IDD_DLG_PWDINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPWDInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	void OnExit();
	void OnInput(int number);
	void OnSave();
	bool CheckPWD();

// Implementation
public:
	void SetCheckObject(enObject enTemp);
protected:
	HICON		m_hIcon;

	CDC*		m_pDC;
	CDC			memDC;
	CBitmap		bitmap;
	CFont		smallfont;
	CFont		bigfont;

	CDC			bk_normal_dc;
	CDC			bk_press_dc;

	CBitmap*	m_pold_bk_normal_bmp;
	CBitmap*	m_pold_bk_press_bmp;

	HBITMAP		m_stBtnNormalBMP;
	HBITMAP		m_stBtnPressBMP;

	ItemInfo	m_item[18];

	CString		m_strPWDInput;		//PWD.
	CString		m_strPWDSHow;		//PWD->****.
	CIniFile	m_IniFile;
	CString     m_strFilePWD;

    enum		enArea{
					PWDINPUT,
					NOTHING
				};
	enObject	enOBJ;
	enArea		enSel;

	// Generated message map functions
	//{{AFX_MSG(CPWDInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PWDInputDlg_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_)
