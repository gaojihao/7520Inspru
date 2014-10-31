#if !defined(AFX_DLGSYESETKEYVOL_H__5182BA7D_009D_4FC6_967C_63978FE3AED7__INCLUDED_)
#define AFX_DLGSYESETKEYVOL_H__5182BA7D_009D_4FC6_967C_63978FE3AED7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSyesetKeyVol.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDlgSyesetKeyVol dialog

class CDlgSyesetKeyVol : public CDialogBase
{
public:
	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	//声音类型
	void OnLeft();
	void OnRight();

	//音量
	void OnAdjustVolumn(int nDifference);
	//设置数值
	static void SetValue();
	//更新按钮状态
	void UpdateBnStatus();

	void OnExit();
public:
	static int     m_nVolLev;	//音量等级

protected:

	ItemInfo        m_Item[7];
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
	
	void SetKeyType();

// Construction
public:
	CDlgSyesetKeyVol(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSyesetKeyVol)
	enum { IDD = IDD_DIALOG_SYSSET_KEYVOL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSyesetKeyVol)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSyesetKeyVol)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYESETKEYVOL_H__5182BA7D_009D_4FC6_967C_63978FE3AED7__INCLUDED_)
