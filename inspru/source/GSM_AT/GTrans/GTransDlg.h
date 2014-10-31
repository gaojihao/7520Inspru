// GTransDlg.h : header file
//

#pragma once

// CGTransDlg dialog
#include "resource.h"
#include "./gprs/GPRSLogic.h"
#include "UIMsgCtrl.h"
#include "./GSM/msg_gsm.h"
#include "./GSM/msg_ipc.h"

#define WM_COMMAND_TIANZE			WM_USER+0x1000			//ͬ�������ͨѶ����Ϣ
#define WM_GSM_RESET				WM_USER+0x1001			//WirelessManager.exe -> GSM_AT.exe reset GSMģ��
#define MSG_RAS_CONNECT				(WM_USER+0x2007)		//RAS����:wParam 1 ����    0�Ͽ�����
#define WM_ACC_ON_OFF				(WM_USER+0x320)			//ACC֪ͨ��wParam��0  ACC����  1  ACC�ϵ�
#define WPARAM_CALL					7						//ͨ����Ϣ   LPARAM:1 ��ʼͨ��   0 ����ͨ��

#define	WIRELESS_MANAGER_TITLE		_T("WND_WIRELESS_MANAGER")//WirelessManager.exe������������ras����


#define WPARAM_SERVER_LOGIN			10	//��������¼״̬	LPARAM:1 ��ͨ   0 ����

class CGTransDlg : public CDialog
{
typedef struct
{
	DWORD		dwProcID;		//����ID
	HWND		hStatusWnd;		//Ŀ�Ĵ��ھ��
} stHWNDSet;
//�����ļ��ṹ
typedef struct
{
	int		nGSMPort;		//GSMģ�鴮��
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

	//���յ���������֪ͨ
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
