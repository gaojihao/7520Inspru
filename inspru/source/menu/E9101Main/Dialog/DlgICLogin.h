// PWDInputDlg.h : header file
//

#if !defined(AFX_CDlgICLogin_H__929DFWEFSDSRHEARGFSDFFSDFASD8__INCLUDED_)
#define AFX_CDlgICLogin_H__929DFWEFSDSRHEARGFSDFFSDFASD8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SysConfig.h"
#include "DialogBase.h"
#include "MaskCtrl.h"
#include "BtnOfMainMenu.h"
#include "resource.h"
#include "e9101main.h"

#include "../../SCom/serial_win32.h"
#include "../../gsm/diaodu_data.h"

class CDlgICLogin : public CDialogBase
{
public:
	CDlgICLogin(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgICLogin)
	enum { IDD = IDD_DLG_PWDINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgICLogin)
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
	bool CheckPWD();

	bool ConnectCardReader(int n_port, int n_baud);
	static DWORD THDICCardWatch(LPVOID lpThreadParameter);
	static DWORD THDReadCard(LPVOID lpThreadParameter);

	void parse_icdata(char* pBuffer, int nLen);
	bool check_cardexist(char* pBuffer, int nLen);
	bool check_cardnumber(char* pBuffer, int nLen);

	void SendCMD(int n);

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

	ItemInfo	m_item[18];

	CString		m_strPWDInput;		//PWD.
	CString		m_strPWDSHow;		//PWD->****.
	CIniFile	m_IniFile;
	CString     m_strFilePWD;

	CString		m_csCardNO;		//卡号
	BOOL		m_bIsGetICNO;	//是否获取到了卡号
	st_IC_Card	m_stICCard;		//卡号密码

	BOOL		m_bTHDIC;	//是否结束运行
	HANDLE		m_hTHDICCardWatch;
	HANDLE		m_hTHDReadCard;
	

    enum		enArea{
					PWDINPUT,
					NOTHING
				};
	enArea		enSel;

	// Generated message map functions
	//{{AFX_MSG(CDlgICLogin)
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

#endif // !defined(AFX_CDlgICLogin_H__929DFWEFSDSRHEARGFSDFFSDFASD8__INCLUDED_)
