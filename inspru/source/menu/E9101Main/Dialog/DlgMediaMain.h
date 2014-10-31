#if !defined(AFX_MEDIAMAIN_H__BB3B70F5_EBF6_465B_87C6_133F7B09EC6E__INCLUDED_)
#define AFX_MEDIAMAIN_H__BB3B70F5_EBF6_465B_87C6_133F7B09EC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MediaMain.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMediaMain dialog
extern BOOL g_bLight;
extern DWORD  g_dwSpareTime;

class CDlgMediaMain : public CDialogBase
{
	enum { BTN_COUNT = 7 };
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	VOID	OnBtnMovie();
	VOID	OnBtnMusic();
	VOID	OnBtnPicture();
	VOID	OnBtnText();

	
	void OnExit();
	void ReleaseGdi();
	
// Construction
public:
	CDlgMediaMain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMediaMain)
	enum { IDD = IDD_MEDIA_MAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMediaMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	ItemInfo    m_Item[BTN_COUNT];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	bool	m_bBtnPress;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
protected:

	VOID	OnBtnGame();
	VOID	OnBtnFM();
	
	// Generated message map functions
	//{{AFX_MSG(CDlgMediaMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIAMAIN_H__BB3B70F5_EBF6_465B_87C6_133F7B09EC6E__INCLUDED_)
