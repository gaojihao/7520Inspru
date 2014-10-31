#if !defined(AFX_VOLUMEDLG_H__DD460AE0_433A_495E_9377_4B7AE88D2009__INCLUDED_)
#define AFX_VOLUMEDLG_H__DD460AE0_433A_495E_9377_4B7AE88D2009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeDlg.h : header file
//

#include "DialogBase.h"
/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg dialog
#define Timer_EVENT  0x00012

class CVolumeDlg : public CDialogBase
{
public:

	static int m_VolLevel;

	//±‰∆§∑Ù
	BOOL OnSkinChange();

	CExStatic m_StcVolume;
protected:

	ItemInfo       m_Item[2];
	CString		m_stTitle;
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	void OnExit();

// Construction
public:
	CVolumeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolumeDlg)
	enum { IDD = IDD_VOLUME_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVolumeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEDLG_H__DD460AE0_433A_495E_9377_4B7AE88D2009__INCLUDED_)
