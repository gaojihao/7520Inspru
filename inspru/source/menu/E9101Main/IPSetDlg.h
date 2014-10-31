// IPSetDlg.h : header file
//

#if !defined(AFX_IPSETDLG_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_)
#define AFX_IPSETDLG_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SysConfig.h"
#include "DialogBase.h"
#include "MaskCtrl.h"
#include "BtnOfMainMenu.h"
#include "resource.h"
#include "e9101main.h"

#include "../GSM/diaodu_data.h"

class CIPSetDlg : public CDialogBase
{
// Construction
public:
	CIPSetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIPSetDlg)
	enum { IDD = IDD_DIALOG_IPSET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	void OnExit();
	void OnInput(int number);
	BOOL CheckIP();
	void OnSave();

	void ChangeDeviceID();
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

	ItemInfo	m_item[20];
	CIniFile	m_IniFile;

	CString		m_strIP;			//IP.
	CString		m_strPort;		//Port.
	CString		m_strDeviceID;			//LocalNum.
	CString		m_strProtocol;
	st_IP_Port	m_stIP_PORT;

	CString			m_csTitile;
	
    enum		enArea{
					IP,
					PORT,
					DEVICEID,
					NOTHING
				};

	enArea		enSel;

	// Generated message map functions
	//{{AFX_MSG(CIPSetDlg)
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

#endif // !defined(AFX_IPSetDlg_H__929D284E_3B80_4242_B4C8_A93B5917DD08__INCLUDED_)
