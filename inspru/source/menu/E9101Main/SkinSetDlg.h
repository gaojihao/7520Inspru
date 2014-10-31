#if !defined(AFX_SKINSETDLG_H__AB89FC11_A2EF_4DA5_8946_981743EA0B7B__INCLUDED_)
#define AFX_SKINSETDLG_H__AB89FC11_A2EF_4DA5_8946_981743EA0B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinSetDlg.h : header file
//

#include "DialogBase.h"
#include "CUiMng.h"
#include "BtnOfMainMenu.h"
/////////////////////////////////////////////////////////////////////////////
// CSkinSetDlg dialog

#define FM_ADJUST_STEP 0.5

void ns73InitThread(LPVOID);
void ReMoveZero(CString &str);

class CSkinSetDlg : public CDialogBase
{
public:

	static BOOL g_bFmOpen;

	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	enSkinType m_enSkinType;

	virtual void OnExit();
	
// Construction
	void OnLeft();
	void OnRight();

protected:

	ItemInfo     m_Item[7];
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnDisableBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;

	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_disable_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_disable_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);


public:
	CSkinSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkinSetDlg)
	enum { IDD = IDD_SKIN_DIALOG };
	CBtnOfMainMenu	m_FreqSub;
	CBtnOfMainMenu	m_FreqAdd;
	CExStatic	m_Static;  
	CBtnOfMainMenu	m_BtnSwith;


	CExButton	m_BtnOk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinSetDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkinSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSwitch();
	afx_msg void OnBtnSkinsetOk();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnSub();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINSETDLG_H__AB89FC11_A2EF_4DA5_8946_981743EA0B7B__INCLUDED_)
