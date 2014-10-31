// GTransDlg.h : header file
//

#pragma once

// CGTransDlg dialog
#include "resource.h"
#include "./gprs/GPRSLogic.h"
#include "UIMsgCtrl.h"
#include "./GSM/msg_gsm.h"
#include "./GSM/msg_ipc.h"

#define WM_COMMAND_TIANZE			WM_USER+0x1000			//同天泽进程通讯的消息
#define WM_GSM_RESET				WM_USER+0x1001			//WirelessManager.exe -> GSM_AT.exe reset GSM模块
#define MSG_RAS_CONNECT				(WM_USER+0x2007)		//RAS拨号:wParam 1 连接    0断开连接
#define WM_ACC_ON_OFF				(WM_USER+0x320)			//ACC通知：wParam：0  ACC掉电  1  ACC上电
#define WPARAM_CALL					7						//通话消息   LPARAM:1 开始通话   0 结束通话

#define	WIRELESS_MANAGER_TITLE		_T("WND_WIRELESS_MANAGER")//WirelessManager.exe进程主窗口名ras拨号


#define WPARAM_SERVER_LOGIN			10	//服务器登录状态	LPARAM:1 接通   0 掉线

class CGTransDlg : public CDialog
{
typedef struct
{
	DWORD		dwProcID;		//进程ID
	HWND		hStatusWnd;		//目的窗口句柄
} stHWNDSet;
//配置文件结构
typedef struct
{
	int		nGSMPort;		//GSM模块串口
} stIniFile;

// Construction
public:
	CGTransDlg(CWnd* pParent = NULL);	// standard constructor
	void     FmtLogInfo(CString strLog);
	void     InitCtrl();
	CString  GenerateFilePath(CString strFileName);
	void     NotifyNaviRst(int nType, int nResult);

// Dialog Data
	//{{AFX_DATA(CGTransDlg)
	enum { IDD = IDD_GTRANS_DIALOG };
	CListBox	m_ListLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTransDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	void  OnLogicGsmMsg(unsigned nType, unsigned nParam);

	//接收到服务器的通知
	void  OnMsgSvrNotice(unsigned nState);

	void  PackMsgConnect();

protected:
	void	ReportGSMState();
	
// Implementation
protected:

	int   m_nDebugWin;
	int   m_nDebugLog;

	HWND  m_hNaviHwnd;
	BOOL  m_bNoticeRst;
	CUIMsgCtrl   m_MsgCtrl;
	msg_gsm      m_GsmMsgCtrl;
	msg_ipc      m_ipc;
	// Generated message map functions
	//{{AFX_MSG(CGTransDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnExit();
	afx_msg void OnDestroy();
	afx_msg void OnGsmPoweron();
	afx_msg void OnGsmPoweroff();
	afx_msg void OnGpsVoice();
	afx_msg void OnGPSDial();
	afx_msg void OnBtnHangup();
	afx_msg void OnBtnSendGPRS();
	afx_msg void OnBtnSavelog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	int  m_nLogCount;
	int  m_nResetForSim;
	BOOL   m_bStartListen;

	stHWNDSet		m_stHWNDSet[32];
public:
	stIniFile		m_stIniFile;
};
