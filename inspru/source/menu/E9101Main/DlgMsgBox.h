#if !defined(AFX_DLG_MSG_BOX_H__34564562HGJJ456456RI9JSIFO9SD8_INCLUDED_)
#define AFX_DLG_MSG_BOX_H__34564562HGJJ456456RI9JSIFO9SD8_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "DialogBase.h"

class CDlgMsgBox : public CDialogBase
{
	enum {CTR_COUNT	= 2};
	enum TID
	{
		TID_TIME_OUT	= 1000,
		TID_EXIT,
	};
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	virtual void OnExit();
	void ReleaseGdi();

	static void SetTextInfo(CString strInfo);

public:
	static UINT		WM_DlgMsgBox_Refresh;

protected:
	ItemInfo	m_Item[CTR_COUNT];
	HBITMAP		m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnDisableBMP;

	static CString	m_strTextInfo;	//需要提示的信息

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

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

public:
	CDlgMsgBox(CWnd* pParent = NULL);

	enum { IDD = IDD_DIALOG_MSGBOX };

	// Overrides
	//{{AFX_VIRTUAL(DlgMsgBox)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgMsgBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DLG_MSG_BOX_H__34564562HGJJ456456RI9JSIFO9SD8_INCLUDED_)
