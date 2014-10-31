#if !defined(AFX_BRIGHTNESSSETDLG_H__3708A28C_6923_4D2F_8024_B77DA9B3C6A0__INCLUDED_)
#define AFX_BRIGHTNESSSETDLG_H__3708A28C_6923_4D2F_8024_B77DA9B3C6A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrightnessSetDlg.h : header file
//

#include "DialogBase.h"
/////////////////////////////////////////////////////////////////////////////
// CBrightnessSetDlg dialog

class CBrightnessSetDlg : public CDialogBase
{
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	virtual void OnExit();

	//亮度
	void OnAdjustBrightness(int nDifference);
	//背光时间
	void OnSpareleft();
	void OnSpareright();

	//设置数值
	static void SetValue();
	static void SetbkliteTimeOuts();

	//更新按钮状态
	void UpdateBnStatus();

protected:
	ItemInfo   m_Item[7];
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP    m_stBtnPressBMP;
	HBITMAP    m_stBtnDisableBMP;
	
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

	int		m_nSpareTimeLevel;	//自动关背光时间,或不关闭背光 0~5

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

public:
	static int		m_nBrightness;		//亮度等级1~10

public:
	CBrightnessSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrightnessSetDlg)
	enum { IDD = IDD_BRIGHTNESS_SET_DIALOG };
	CExButton	m_BtnLightHigh;
	CExButton	m_BtnLightMid;
	CExButton	m_BtnLightLow;
	CExButton	m_BtnExit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrightnessSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrightnessSetDlg)
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

#endif // !defined(AFX_BRIGHTNESSSETDLG_H__3708A28C_6923_4D2F_8024_B77DA9B3C6A0__INCLUDED_)
