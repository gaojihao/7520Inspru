#if !defined(AFX_DLGCONFIRM_H__717A5043_708C_492A_AEBB_F3220DBE550F__INCLUDED_)
#define AFX_DLGCONFIRM_H__717A5043_708C_492A_AEBB_F3220DBE550F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog

class CDlgConfirm : public CDialogBase
{
// Construction
	enum TID
	{
		TID_TIME_OUT	= 1000,
		TID_EXIT,
	};
public:

	static BOOL s_bOk;
	static	CString		m_stTitle;

	BOOL OnLanguageChange();
	BOOL OnSkinChange();
	virtual void OnExit();
protected:

	ItemInfo       m_Item[3];


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
	CDlgConfirm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgConfirm)
	enum { IDD = IDD_DIALOG_CONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfirm)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIRM_H__717A5043_708C_492A_AEBB_F3220DBE550F__INCLUDED_)
