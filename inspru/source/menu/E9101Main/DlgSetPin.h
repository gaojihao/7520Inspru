#if !defined(AFX_DLGDIAL_H__8291C050_73A9_49CA_BFC9_111111111111AC79AD78D1D4__INCLUDED_)
#define AFX_DLGDIAL_H__8291C050_73A9_49CA_BFC9_111111111111AC79AD78D1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDial.h : header file
//

#include "DialogBase.h"
#include "BtnOfMainMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDial dialog
//enum BlueToothState{UnInit, Initialized, Pair, DialIn, AcceptCall,DialOut};


class CDlgSetPinCode : public CDialogBase
{
	enum{ BTN_COUNT = 16 };
public:
	BOOL m_bDialOut; 

	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	CBtnOfMainMenu	m_BtnExit;

	CBtnOfMainMenu	m_Btn0;
	CBtnOfMainMenu	m_Btn1;
	CBtnOfMainMenu	m_Btn2;
	CBtnOfMainMenu	m_Btn3;
	CBtnOfMainMenu	m_Btn4;
	CBtnOfMainMenu	m_Btn5;
	CBtnOfMainMenu	m_Btn6;
	CBtnOfMainMenu	m_Btn7;
	CBtnOfMainMenu	m_Btn8;
	CBtnOfMainMenu	m_Btn9;
	CBtnOfMainMenu	m_Btnshar;
	CBtnOfMainMenu	m_Btnstar;

	CBtnOfMainMenu	m_BtnOut;
	CBtnOfMainMenu	m_BtnIn;
	CBtnOfMainMenu	m_BtnBack;

	CBtnOfMainMenu	m_BtnVolUp;
	CBtnOfMainMenu	m_BtnVolDown;

	CExStatic m_StcNum;

	BOOL m_IsNUm;
	BOOL  m_pin_type;

// Construction
public:
	CDlgSetPinCode(CWnd* pParent = NULL);   // standard constructor

	void SetType( bool b_pin_type = TRUE ) { m_pin_type = b_pin_type; }
// Dialog Data
	//{{AFX_DATA(CDlgDial)
	enum { IDD = IDD_DIALOG_DIA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual void OnExit() ;

// Implementation
public:

	ItemInfo       m_Item[BTN_COUNT];

	CString        m_strNum;
	ItemInfo       m_ItemNum;

	CString		m_stTitle;
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

	void  LoadPookBook();
protected:

	int    m_nRstState;

	// Generated message map functions
	//{{AFX_MSG(CDlgDial)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();
	afx_msg void OnBtn3();
	afx_msg void OnBtn4();
	afx_msg void OnBtn5();
	afx_msg void OnBtn6();
	afx_msg void OnBtn7();
	afx_msg void OnBtn8();
	afx_msg void OnBtn9();
	afx_msg void OnBtn10();
	afx_msg void OnBtn0();
	afx_msg void OnBtn11();
	afx_msg void OnBTNDiaOut();
	afx_msg void OnBtnBack();
	afx_msg void OnBTNDiaIn();
	afx_msg void OnBtnReset();
	afx_msg void OnStaticNum();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnVolUp();
	afx_msg void OnBtnVolDown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnBuleToothoffHook(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnBlueToothDialIn(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDIAL_H__8291C050_73A9_49CA_BFC9_AC79AD78D1D4__INCLUDED_)
