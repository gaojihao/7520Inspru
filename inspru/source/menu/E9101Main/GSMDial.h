#if !defined(AFX_GSMDIAL_H__91472D29_6592_4C94_A528_A1EBF7059E1F__INCLUDED_)
#define AFX_GSMDIAL_H__91472D29_6592_4C94_A528_A1EBF7059E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSMDial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGSMDial dialog

#include "DialogBase.h"
#include "BtnOfMainMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CGSMDial dialog
enum BlueToothState{UnInit, Initialized, Pair, DialIn, AcceptCall,DialOut};


class CGSMDial : public CDialogBase
{
	enum EN_TID{
		TID_CLOSE	= 1000,
		TID_SET_TOP,
	};
public:
	
	BOOL m_bDialOut; 
	static BOOL	m_bIsOpen;	//窗口是否已经打开

	static BOOL	IsOpen();

	//变换语言和单位
	virtual BOOL OnLanguageChange();
	virtual BOOL OnSkinChange();

	void OnServerDiaOut(CString strphone);

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

// Construction
public:
	CGSMDial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGSMDial)
	enum { IDD = IDD_GSM_DIAL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGSMDial)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnExit();
	virtual void PostNcDestroy();

// Implementation
public:
	static CString    m_UserNum;    //用户电话号码
	static BOOL       m_bAutoDial;  //是否启动导航

	ItemInfo       m_ItemNum;
	ItemInfo       m_Item[19];
	CString        m_Info;
	CString		   m_stTitle;
	CString        m_strExtNo;
	HBITMAP        m_stBtnNormalBMP;
	HBITMAP        m_stBtnPressBMP;
	HBITMAP        m_stBtnDisableBMP;
	HBITMAP        m_stVolBmp;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	
	CDC  bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_vol_dc;
	CDC  bk_disable_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_disable_bmp;
	CBitmap*  m_pold_bk_vol_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

	//Dial
protected:
	BOOL   m_bIsDialing;
	BOOL   m_bIMeShow;
	UINT   m_nZeroTick;
	UINT   m_DialCount;
	void   AddPhoneNum(TCHAR chNum, BOOL bAdd = TRUE );
	void   LoadIMEFromText();
	void   DelAllInput();
	

	
protected:
	// Generated message map functions
	//{{AFX_MSG(CGSMDial)
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
	afx_msg void OnBTNHangup();
	afx_msg void OnBtnReset();
	afx_msg void OnStaticNum();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnVolUp();
	afx_msg void OnBtnVolDown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSMDIAL_H__91472D29_6592_4C94_A528_A1EBF7059E1F__INCLUDED_)
