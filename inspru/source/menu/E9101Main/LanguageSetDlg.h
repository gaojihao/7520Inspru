#if !defined(AFX_LANGUAGESETDLG_H__9406C023_F771_47FF_9864_AEAE2E314E89__INCLUDED_)
#define AFX_LANGUAGESETDLG_H__9406C023_F771_47FF_9864_AEAE2E314E89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LanguageSetDlg.h : header file
//
#include "DialogBase.h"
/////////////////////////////////////////////////////////////////////////////
// CLanguageSetDlg dialog

class CLanguageSetDlg : public CDialogBase
{
public:
	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	enLanguage m_eLan;
	//记录语言链表位置
	LONG m_ListInitPos;

// Construction

	void OnExit();
	void ReleaseGdi();

	void OnLeft();
	void OnRight();

	void OnSkinRight();
	void OnSkinLeft();

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


public:
	CLanguageSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLanguageSetDlg)
	enum { IDD = IDD_LANGUAGESET_DIALOG };
	CExButton	m_BtnCancel;
	CExButton	m_BtnF;
	CExButton	m_BtnB;
	//}}AFX_DATA

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLanguageSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLanguageSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnF();
	afx_msg void OnBtnB();
	afx_msg void OnBtnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANGUAGESETDLG_H__9406C023_F771_47FF_9864_AEAE2E314E89__INCLUDED_)
