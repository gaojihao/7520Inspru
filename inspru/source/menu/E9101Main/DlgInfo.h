#if !defined(AFX_DLGINFO_H__BC214842_2981_46D2_9B2D_C18B4C0FC7D0__INCLUDED_)
#define AFX_DLGINFO_H__BC214842_2981_46D2_9B2D_C18B4C0FC7D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInfo.h : header file
//

#include "DialogBase.h"
#include "BtnOfMainMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog

class CDlgInfo : public CDialogBase
{
	enum TID
	{
		TID_EXIT_PROCESS = 100,
		TID_SHOW_GSM_AT = 0x111
	};
public:
		//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	virtual void OnExit();

	CExButton	m_BtnExit;
	CExButton	m_BtnGps;
	CExButton	m_BtnCarlibra;

	void OnInitConfig();
public:

	ItemInfo       m_Item[4];
	CIniFile m_IniFile;
	static ItemInfo       m_ItemNum;

	ItemInfo       m_ItemSysInfo[6];

	CString		m_stTitle;
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	void ReleaseGdi();
	void OnBtnWince();
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);


// Construction
public:
	CDlgInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInfo)
	enum { IDD = IDD_DIALOG_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInfoExit();
	afx_msg void OnBtnInfoGpsinfo();
	afx_msg void OnBtnCalibra();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINFO_H__BC214842_2981_46D2_9B2D_C18B4C0FC7D0__INCLUDED_)
