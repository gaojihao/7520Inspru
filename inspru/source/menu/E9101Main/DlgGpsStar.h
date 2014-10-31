#if !defined(AFX_DLGGPSSTAR_H__0E720272_A3B1_4C11_A87F_BC450783127C__INCLUDED_)
#define AFX_DLGGPSSTAR_H__0E720272_A3B1_4C11_A87F_BC450783127C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGpsStar.h : header file
//

#include "Dialogbase.h"
#include "BtnOfMainMenu.h"

#include "../GSM/AlmanacDiagramCtrl.h"
#include "SatelliteDiagramCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// DlgGpsStar dialog

class DlgGpsStar : public CDialogBase
{
public:

	virtual void UserRefresh();
	
	CBtnOfMainMenu	m_BtnExit;

	CAlmanacDiagramCtrl	m_stcAlmanac;
	CSatelliteDiagramCtrl	m_stcSatellite;

	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	virtual void OnExit();
private:

	void HandleStardata();

	HBRUSH		m_hbkBrush; // set background of all controls to be white.
	HBRUSH		m_hbkGpsState;
	UINT		m_nMsgIDPosRefresh;		//模块位置数据更新消息ID


	CFont	m_GpsStateFont;

// Construction
public:
	DlgGpsStar(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(DlgGpsStar)
	enum { IDD = IDD_DIALOG_GPS_STAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGpsStar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	ItemInfo       m_Item;
	CString		m_stTitle;
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont  bigfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGpsStar)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGpsstarExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGPSSTAR_H__0E720272_A3B1_4C11_A87F_BC450783127C__INCLUDED_)
