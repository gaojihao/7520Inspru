// NumSetDlg.h : header file
//

#if !defined(AFX_NUMBSETDLG_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_)
#define AFX_NUMBSETDLG_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SysConfig.h"
#include "DialogBase.h"
#include "MaskCtrl.h"
#include "BtnOfMainMenu.h"
#include "resource.h"
#include "e9101main.h"

/////////////////////////////////////////////////////////////////////////////
// CNumSetDlg dialog
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

class CNumSetDlg : public CDialogBase
{
// Construction
public:
	CNumSetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNumSetDlg)
	enum { IDD = IDD_DIALOG_NUMB_SET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumSetDlg)
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
	void PWDSetting();

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

	CString		m_strCenterNum;
	CString		m_strHospitalNum;
	CString		m_strFireNum;
	CString		m_strAlarmNum;

	CString			m_csTitile;

    enum		enArea{
					CENTERNUM,
					HOSPITALNUM,
					FIRENUM,
					ALARMNUM,
					NOTHING
				};

	enArea		enSel;

	// Generated message map functions
	//{{AFX_MSG(CNumSetDlg)
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

#endif // !defined(AFX_NumSetDlg_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_)
