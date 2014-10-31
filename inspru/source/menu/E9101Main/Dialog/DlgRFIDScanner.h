// PWDInputDlg.h : header file
//

#if !defined(AFX_CDlgRFIDScanner_H__CVPOIUYTREWSDFGHJKNBVCXZJH8T__INCLUDED_)
#define AFX_CDlgRFIDScanner_H__CVPOIUYTREWSDFGHJKNBVCXZJH8T__INCLUDED_

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

#include <vector>

using std::vector;

class CDlgRFIDScanner : public CDialogBase
{
	typedef struct tagRFID
	{
		char szRFID[12];
	} RFID_INFO;
	enum { 
		enBtnCount		= 5, 
		enCountColumn	= 5,		//显示的RFID号列数
		enCountRow		= 10,		//显示的RFID号行数
	};
public:
	CDlgRFIDScanner(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRFIDScanner)
	enum { IDD = IDD_DLG_PWDINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRFIDScanner)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	void OnExit();
	void OnInput(int number);
	bool CheckPWD();
	
	void OnBtnChangePage(int nDiffer);//改变页数
	void OnBtnSend();
	void OnBtnClear();
	void OnTest();

	bool ConnectCardReader(int n_port, int n_baud);
	static DWORD THDICCardWatch(LPVOID lpThreadParameter);
	static DWORD THDReadCard(LPVOID lpThreadParameter);

	void parse_icdata(char* pBuffer, int nLen);
	bool check_cardexist(char* pBuffer, int nLen);
	bool check_cardnumber(char* pBuffer, int nLen);

	void SendCMD(int n);

	void DrawRFID(CDC* pmemDC);

	static void GetRFID(PVOID pCWnd, char* pBuffer, int nLen);

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

	CString		m_strPWDInput;		//PWD.
	CString		m_strPWDSHow;		//PWD->****.
	CIniFile	m_IniFile;
	CString     m_strFilePWD;
	ItemInfo	m_item[enBtnCount];

	CString		m_csCardNO;		//卡号
	BOOL		m_bIsGetICNO;	//是否获取到了卡号
	//char	    m_cRFID[10][256];	//RFID码
	//int			m_nRFIDCount;		//当前数组里面储存的RFID码个数
	CString		m_csState;		//状态

	vector<RFID_INFO>	m_vtRFID;
	typedef vector<RFID_INFO>::iterator	ITER_RFID;
	int			m_nPageIndex;	//当前页序号
	int			m_nPageCount;	//总共页数

	BOOL		m_bTHDIC;	//是否结束运行
	HANDLE		m_hTHDICCardWatch;
	//st_IC_Card	m_stICCard;		//卡号密码
	
	

    enum		enArea{
					PWDINPUT,
					NOTHING
				};
	//enArea		enSel;

	// Generated message map functions
	//{{AFX_MSG(CDlgRFIDScanner)
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

#endif // !defined(AFX_CDlgRFIDScannerScanner_H__CVPOIUYTREWSDFGHJKNBVCXZJH8T__INCLUDED_)
