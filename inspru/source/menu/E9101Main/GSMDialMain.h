#if !defined(AFX_GSMDIALMAIN_H__FBD026A3_65AD_4D04_AC5D_4B0560883B43__INCLUDED_)
#define AFX_GSMDIALMAIN_H__FBD026A3_65AD_4D04_AC5D_4B0560883B43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSMDialMain.h : header file

#include "DialogBase.h"

class GSMDialMain : public CDialogBase
{
public:
	
	
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	virtual void OnExit();
	
	// Construction
	
	void OnPhoneBook();
	void OnDialout();
	void OnPhoneLog();
	void GSMNetSet();

	void OnBtnSMS();

	void ReleaseGdi();
	
protected:
	
	ItemInfo    m_Item[5];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	
	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_disable_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
	
public:
	GSMDialMain(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(GSMDialMain)
	enum { IDD = IDD_GSM_DIAL_MAIN };
	//	CExButton	m_BtnLightHigh;
	//	CExButton	m_BtnLightMid;
	//	CExButton	m_BtnLightLow;
	//	CExButton	m_BtnExit;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GSMDialMain)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GSMDialMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSMDIALMAIN_H__FBD026A3_65AD_4D04_AC5D_4B0560883B43__INCLUDED_)
