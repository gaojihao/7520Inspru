#if !defined(AFX_PHONEBOOK_H__12F72D4C_B598_4A08_AC91_DD900B615292__INCLUDED_)
#define AFX_PHONEBOOK_H__12F72D4C_B598_4A08_AC91_DD900B615292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneBook.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"

#include "PhoneBookFile.h"

#include "PhoneBookMng.h"

//#include "BlueToothDialOut.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneBook dialog
/*
class CPhoneBook : public CDialogBase
{

public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	CBtnOfMainMenu	m_BtnExit;
	
	CBtnOfMainMenu	m_BtnPhoneBook1;
	CBtnOfMainMenu	m_BtnPhoneBook2;
	CBtnOfMainMenu	m_BtnPhoneBook3;
	CBtnOfMainMenu	m_BtnPhoneBook4;
	CBtnOfMainMenu	m_BtnPhoneBook5;

	CBtnOfMainMenu	m_BtnLog;
	CBtnOfMainMenu	m_BtnDia;
	CBtnOfMainMenu	m_BtnEdit;
	CBtnOfMainMenu	m_BtnPageUp;
	CBtnOfMainMenu	m_BtnPageDown;

	CPhoneBookFile m_PhoneBook;

	CObArray* m_ObArrary;
	UINT m_Pos;
	
// Construction
public:
	CPhoneBook(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhoneBook)
	enum { IDD = IDD_DIALOG_PHONEBOOK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneBook)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhoneBook)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
*/
class CPhoneBook : public CDialogBase
{
	public:
	CFile m_IniFile;
	ULONGLONG m_iPos;
	BOOL ReadIniFile( CString path );
	BOOL ReadOneString(char* pIn, ULONGLONG iInLen, char*& pOut, ULONGLONG &iOutLen);
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	virtual void OnExit();

	CBtnOfMainMenu	m_BtnExit;
	
	CBtnOfMainMenu	m_BtnPhoneLog1;
	CBtnOfMainMenu	m_BtnPhoneLog2;
	CBtnOfMainMenu	m_BtnPhoneLog3;
	CBtnOfMainMenu	m_BtnPhoneLog4;
	CBtnOfMainMenu	m_BtnPhoneLog5;

	CBtnOfMainMenu	m_BtnDelAll;

	CBtnOfMainMenu	m_BtnPageUp;
	CBtnOfMainMenu	m_BtnPageDown;

	CObArray* m_ObArrary;
	UINT m_Pos;
	int m_TotalPage;

	CString m_stTitle;

	int m_CurPage;
	BOOL m_PhoneNUll;
	UINT m_CurListPos;
// Construction
public:
	CPhoneBook(CWnd* pParent = NULL);   // standard constructor

	BOOL   m_IsGetItem;
	CString m_strSel;
// Dialog Data
	//{{AFX_DATA(CPhoneLog)
	enum { IDD = IDD_DIALOG_PHONEBOOK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneLog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	int   GetMaxRecord();

	// Generated message map functions
	//{{AFX_MSG(CPhoneLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnDelAll();
	afx_msg void OnBtnItem1();
	afx_msg void OnBtnItem2();
	afx_msg void OnBtnItem3();
	afx_msg void OnBtnItem4();
	afx_msg void OnBtnItem5();
	afx_msg void OnBtnPageup();
	afx_msg void OnBtnPagedown();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnIMMMsg(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	ItemInfo     m_Item[12];
	
	HBITMAP      m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnDisableBMP;
	CString ItemStrName[5];
	CString ItemStrNum[5];
	CString tempItemStr;
	UINT CurPhoneLog;
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont  midfont;
	CFont bigfont;

	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_dis_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_disable_bmp;

	void OnStaticList(UINT nId);
	void InitGdi();
	void OnOutBook();
	void OnInBook();
	void OnBtnEditPhoneName();
	void OnAddPhone();

	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONEBOOK_H__12F72D4C_B598_4A08_AC91_DD900B615292__INCLUDED_)
