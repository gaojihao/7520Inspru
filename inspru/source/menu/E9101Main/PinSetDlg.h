// PinSetDlg.h : header file
//

#if !defined(AFX_PINSETDLG_H__929D284E_3B80_4242_B4C8_tghfgsadgD08__INCLUDED_)
#define AFX_PINSETDLG_H__929D284E_3B80_4242_B4C8_tghfgsadgD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SysConfig.h"
#include "DialogBase.h"
#include "MaskCtrl.h"
#include "BtnOfMainMenu.h"
#include "resource.h"
#include "e9101main.h"
#include "PWDInputDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog
//
//typedef enum enBtnState
//{
//	BTN_STATE_NORMAL,
//	BTN_STATE_DOWN,
//	BTN_STATE_DISABLE,
// }enBtnState;
//
//class ItemInfo
//{
//public:
//	CRect rect;
//	CString chChar;
//	enBtnState nState;
// };

class CPinSetDlg : public CDialogBase
{
// Construction
public:
	CPinSetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPinSetDlg)
	enum { IDD = IDD_DLG_PINSET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPinSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	void OnExit();
	void OnInput(int number);
	void OnSave();
	void ChangeObject();
	bool CheckPWD();

// Implementation
protected:
	HICON		m_hIcon;

	CDC*		m_pDC;
	CDC			memDC;
	CBitmap		bitmap;
	CFont		smallfont;
	CFont		bigfont;

	CDC			bk_normal_dc;
	CDC			bk_press_dc;

	CBitmap*	m_pold_bk_normal_bmp;
	CBitmap*	m_pold_bk_press_bmp;

	HBITMAP		m_stBtnNormalBMP;
	HBITMAP		m_stBtnPressBMP;

	ItemInfo	m_item[19];

	CIniFile	m_IniFile;
	
	CString		m_strOldPWD;		//文件读取旧密码
	CString		m_strOldPWDInput;	//输入的旧密码
	CString		m_strNewPWD;		//新密码
	CString		m_strConfirmPWD;	//确认密码.
	CString		m_strPWDShow;	    //密码****.

    enum		enArea{
					OLDPWD,
					NEWPWD,
					CFMPWD,
					NOTHING
				};

	enObject	enOBJ;
	enArea		enSel;

	// Generated message map functions
	//{{AFX_MSG(CPinSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PinSetDlg_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_)
