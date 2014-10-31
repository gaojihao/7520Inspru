#if !defined(AFX_COM_SETTING_DLG_H__JRTEGWHNJUMYFTJYUTRIYUTNAERGWEJGHJJHA__INCLUDED_)
#define AFX_COM_SETTING_DLG_H__JRTEGWHNJUMYFTJYUTRIYUTNAERGWEJGHJJHA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgComSetting.h : header file
//

#include "DialogBase.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgComSetting dialog

class CDlgComSetting : public CDialogBase
{
	enum{ BTN_COUNT = 10, };
	enum{ 
		COM_FRONT	= 2,
		COM_BACK	= 7,
	};
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	virtual void OnExit();

	//ICCard
	void OnAdjustICCard(int nDifference);
	//扫描枪
	void OnAdjustScanner(int nDifference);
	//RFID
	void OnAdjustRFID(int nDifference);

	//更新按钮状态
	void UpdateBnStatus();

protected:
	ItemInfo	m_Item[BTN_COUNT];
	HBITMAP		m_stBtnNormalBMP;
	HBITMAP		m_stBtnPressBMP;
	//HBITMAP    m_stBtnDisableBMP;
	
	CDC*		m_pDC;
	CDC			memDC;
	CBitmap		bitmap;
	CFont		smallfont;
	CFont		bigfont;

	CString		m_csTitile;
	
	CDC			bk_normal_dc;
	CDC			bk_press_dc;
	//CDC  bk_disable_dc;
	CBitmap*	m_pold_bk_normal_bmp;
	CBitmap*	m_pold_bk_press_bmp;
	//CBitmap*  m_pold_bk_disable_bmp;

	int		m_nComICCard;
	int		m_nComScanner;
	int		m_nComRFID;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

public:
	CDlgComSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgComSetting)
	enum { IDD = IDD_BRIGHTNESS_SET_DIALOG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgComSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgComSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COM_SETTING_DLG_H__JRTEGWHNJUMYFTJYUTRIYUTNAERGWEJGHJJHA__INCLUDED_)
