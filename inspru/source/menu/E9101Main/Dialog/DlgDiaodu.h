#if !defined(AFX_DLGDIAODU_H64YHRTHERGSFWE2_DF04FD5__INCLUDED_)
#define AFX_DLGDIAODU_H64YHRTHERGSFWE2_DF04FD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgDiaoDu : public CDialogBase
{
	enum { BTN_COUNT = 6 };
public:
	//�任���Ժ͵�λ
	virtual BOOL OnLanguageChange();
	//��Ƥ��
	virtual BOOL OnSkinChange();

	virtual void OnExit();

protected:
	ItemInfo        m_Item[BTN_COUNT];
	HBITMAP			m_stBtnNormalBMP;
	HBITMAP			m_stBtnPressBMP;
	HBITMAP			m_stBtnDisableBMP;
	
	CDC*			m_pDC;
	CDC				memDC;
	CBitmap			bitmap;
	CFont			smallfont;
	CFont			bigfont;

	CString			m_csTitile;

	CDC				bk_normal_dc;
	CDC				bk_press_dc;
	CDC				bk_disable_dc;
	CBitmap*		m_pold_bk_normal_bmp;
	CBitmap*		m_pold_bk_press_bmp;
	CBitmap*		m_pold_bk_disable_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

private:
	VOID OnTextInfo();
	VOID OnQuestion();
	VOID OnInfoMenu();
	VOID OnInfoServer();
	VOID OnMenu();

public:
	CDlgDiaoDu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDiaoDu)
	enum { IDD = IDD_DLG_DIAODU };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDiaoDu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDiaoDu)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
	
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDIAODU_H64YHRTHERGSFWE2_DF04FD5__INCLUDED_)