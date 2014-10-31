#if !defined(AFX_PHONELOG_H__24700E4E_6569_423E_B307_764BF3AA7CC4__INCLUDED_)
#define AFX_PHONELOG_H__24700E4E_6569_423E_B307_764BF3AA7CC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneLog.h : header file
//

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "PhoneLogMng.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneLog dialog

enum enLogType
{
	PLACEDCALL = 1,
	RECEIVEDCALL,
	MISSEDCALL
};

class CPhoneLog : public CDialogBase
{
	public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	virtual void OnExit();
	void ShowPage(int nPageIndex); //显示内容

	CBtnOfMainMenu	m_BtnExit;	
	CBtnOfMainMenu	m_BtnPhoneLog1;
	CBtnOfMainMenu	m_BtnPhoneLog2;
	CBtnOfMainMenu	m_BtnPhoneLog3;
	CBtnOfMainMenu	m_BtnPhoneLog4;
	CBtnOfMainMenu	m_BtnPhoneLog5;
	CBtnOfMainMenu	m_BtnDelAll;
	CBtnOfMainMenu	m_BtnPageUp;
	CBtnOfMainMenu	m_BtnPageDown;

	CObArray*		m_ObArrary;
	UINT			m_Pos;
	int				m_TotalPage;
	int				m_CurPage;
	BOOL			m_PhoneNUll;
	CString			m_stTitle;
	static enLogType enLog; //1:已拨 2:已接 3:未接
	int				m_nRecordCount;		//数据库中记录总数
	UINT			m_nTag;
	int				m_nTotalPage;				//页总数
// Construction
public:
	CPhoneLog(CWnd* pParent = NULL);   // standard constructor
	void   SetGSMMain(CDialogBase* pDlg) { m_pGSMMain = pDlg;}

// Dialog Data
	//{{AFX_DATA(CPhoneLog)
	enum { IDD = IDD_DIALOG_PHONELOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDialogBase*  m_pGSMMain;

	// Generated message map functions
	//{{AFX_MSG(CPhoneLog)
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	ItemInfo	m_Item[12];
	HBITMAP		m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnDisableBMP;
	CString		ItemStr[5];
	CString		ItemStrCity[5];
	CString		m_strName[5];
	CString		m_strTime[5];
	CString		m_strTimeLen[5];
	UINT		CurPhoneLog;
	CDC*		m_pDC;
	CDC			memDC;
	CBitmap		bitmap;
	CFont		smallfont;
	CFont		bigfont;

	CString		m_csTitile;
	
	CDC			bk_normal_dc;
	CDC			bk_press_dc;
	CDC			bk_disable_dc;
	CBitmap*	m_pold_bk_normal_bmp;
	CBitmap*	m_pold_bk_press_bmp;
	CBitmap*	m_pold_bk_disable_bmp;

	void UpdateUserName();
	void OnBtnDelLogState(int state);
	void OnStaticList(UINT nId);
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONELOG_H__24700E4E_6569_423E_B307_764BF3AA7CC4__INCLUDED_)
