#if !defined(AFX_SYSSETDLG_H__201F4B18_07E8_43DB_8BE8_772325D67D3A__INCLUDED_)
#define AFX_SYSSETDLG_H__201F4B18_07E8_43DB_8BE8_772325D67D3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSetDlg.h : header file
//

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSysSetDlg dialog


class CSysSetDlg : public CDialogBase
{
	enum { BTN_COUNT = 8 };
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	virtual void OnExit();
	void ReleaseGdi();

// Construction
public:
	CSysSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysSetDlg)
	enum { IDD = IDD_SYSSET_DIALOG };

	CBtnOfMainMenu	m_BtnTimeSet;
	CBtnOfMainMenu	m_Btn12;
	CBtnOfMainMenu	m_BtnTimeZone;
	CBtnOfMainMenu	m_BtnClock;

	CBtnOfMainMenu	m_BtnSkin;
	CBtnOfMainMenu	m_BtnAdjust;
	CBtnOfMainMenu	m_BtnLanguage;

	CBtnOfMainMenu	m_BtnForword;
	CBtnOfMainMenu  m_BtnBack;
	CBtnOfMainMenu	m_BtnBkLight;
	CBtnOfMainMenu  m_BtnBT;	

	CBtnOfMainMenu	m_BtnDefault;
	CBtnOfMainMenu  m_BtnCalibr;

	CBtnOfMainMenu	m_BtnExit;
	

	//}}AFX_DATA
	static enLanguage S_CurLanguage;
	static BOOL S_bSpare;
	
protected:

	ItemInfo       m_Item[BTN_COUNT];
	CString		m_stTitle;
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont	bigfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

private:
	void OnSyssetKeyVol();
	void OnBtnIPSet();

	void OnBtnPustToTalk();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSysInfo();
	afx_msg void OnBtnTimeSet();
	afx_msg void OnBtnLanguageSet();
	afx_msg void OnBtnSkinSet();
	afx_msg void OnBtnDefaultSet();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnSysset12();
	afx_msg void OnBtnSyssetTimezone();
	afx_msg void OnBtnBrigtness();
	afx_msg void OnBtnSyssetFor();
	afx_msg void OnBtnSyssetBack();
	afx_msg void OnBtnSyssetBt();
	afx_msg void OnBtnSyssetBkLight();
	afx_msg void OnBtnSyasetCalibr();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETDLG_H__201F4B18_07E8_43DB_8BE8_772325D67D3A__INCLUDED_)
