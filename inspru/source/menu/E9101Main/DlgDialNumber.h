#if !defined(AFX_DLGPAIR_H__925EB066_0CF31111_48DF_81800010_6E4346036C00__INCLUDED_)
#define AFX_DLGPAIR_H__925EB066_0CF31111_48DF_81800010_6E4346036C00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgNumber.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNumber dialog

class CDlgDialNumber : public CDialogBase
{
// Construction
public:

	CString m_stTimer;
	BOOL OnLanguageChange();
	BOOL OnSkinChange();
	virtual void OnExit();
protected:

	ItemInfo       m_Item[13];
	CString		m_stTitle;
	CString     m_stInfo;

	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;

	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	CFont midfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;


	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);


public:
	CDlgDialNumber(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPair)
	enum { IDD = IDD_DIALOG_DIAL_NUMBER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDialNumber)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDialNumber)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAIR_H__925EB066_0CF3_48DF_8180_6E4346036C00__INCLUDED_)
