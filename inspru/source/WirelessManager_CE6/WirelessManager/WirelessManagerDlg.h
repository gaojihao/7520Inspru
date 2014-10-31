// WirelessManagerDlg.h : header file
//

#pragma once

#include "Ras.h"
#include "Raserror.h"
#include "MsgDefine.h"


#define  UMSG_GSM_CMD                  ( WM_USER + 0x4001 ) 
#define  GSM_ACK_SINGLE_RESULT         13  
#define  GSM_ACK_DIAL_RESULT           15   //拨打电话后的状态ACK, 1:接通； 0:挂断

#define MODEM			_T("modem")
#define ENTRY_NAME		_T("GprsModem")

enum SIM_CardStatus{
	SIM_NO,
	SIM_YES,
};

enum GSM_PowerStatus{
	GSM_PowerOFF,
	GSM_PowerON,
};

enum GSM_SignalStatus{
	GSM_SignalLow,
	GSM_SignalHigh,
};

//连接状态
extern ConnectStatus	g_enConnectStatus;
//当g_enConnectStatus为CONNECT_STATUS_DISCONNECTED时，代表断开的附加原因
extern ExtraStatus		g_enExtraStatus;
static BOOL		g_bIsDialing;		//是否在拨号


class CWirelessManagerDlg : public CDialog
{
// Construction
public:
	typedef struct
	{
		DWORD		dwProcID;		//进程ID
		HWND		hStatusWnd;		//目的窗口句柄
	} stHWNDSet;
	enum
	{
		TID_START_GSM_MODUAL = 1001,
		TID_GSM_INIT,
		TID_AUTO_CONNECT,
		TID_FORCE_CHECK_GPRS,
		TID_CHECK_RAS_CONNECTION,	//被请求连接RAS后，定时检查是否连接成功
		TID_DISCONNECT,			//超时：断开
		TID_DIAL,
	};
	enum DIAL_STYPE
	{
		DIAL_TYPE_NONE,
		enRAS_Dial,			//拨号
		enRAS_Hangup,		//挂断
		enRAS_ReDial,		//重拨
	};
	CWirelessManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WIRELESSMANAGER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	private:
		HRASCONN		m_hRasConn;			//RAS句柄
		RASCONNSTATE	m_RasState;			//当前RAS状态
		stHWNDSet		m_stHWNDSet[32];
		CString			m_csIPSet[5];		//IP地址
		DIAL_STYPE		m_enDialStype;		//拨号类型
		DWORD			m_dwReDialCnt;		//Ras重拨次数，超过5次则Reset GSM模块
		bool            m_bDialing;

		DWORD	DialRasNetwork();		//连接Ras网络
		void	RasControl(DIAL_STYPE enStype);		//所有拨号类型
		void	ParseRasDialEvent( WPARAM, LPARAM );
		void	UpdateRASState( RASCONNSTATE state );
		void	PostMsg2WND(ConnectStatus enStatus);
		void	PostStatus();
		void	PostSignal(UCHAR cSignal);
		void	SendState2Menu(int nType);

public:

	static  DWORD THDStartUp(LPVOID lpParameter);
		
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDial();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStatus();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPing();
};
