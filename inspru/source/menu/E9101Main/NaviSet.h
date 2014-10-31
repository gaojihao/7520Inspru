#if !defined(AFX_NAVISET_H__4DC6C84F_9E34_498A_A4D9_3B3C34FEA546__INCLUDED_)
#define AFX_NAVISET_H__4DC6C84F_9E34_498A_A4D9_3B3C34FEA546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NaviSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNaviSet dialog

class CNaviSet : public CDialogBase
{
public:
	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	virtual void OnExit();

protected:

	CString m_csAutoStart;
	CString m_csAppPath;
	CString m_csNaviPath;

	ItemInfo m_NaviPath;

	int m_curBro;

	ItemInfo        m_ItemSig;
	ItemInfo        m_ItemApp[2];

	ItemInfo        m_Item[2];
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnSignSel;

	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;

	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC	bk_sigsel_dc;


	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*	m_pold_bk_signel_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);


// Construction
public:
	CNaviSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNaviSet)
	enum { IDD = IDD_DIALOG_NAVISET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNaviSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNaviSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVISET_H__4DC6C84F_9E34_498A_A4D9_3B3C34FEA546__INCLUDED_)
