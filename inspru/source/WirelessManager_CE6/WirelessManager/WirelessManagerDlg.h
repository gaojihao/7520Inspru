// WirelessManagerDlg.h : header file
//

#pragma once

#include "Ras.h"
#include "Raserror.h"
#include "MsgDefine.h"


#define  UMSG_GSM_CMD                  ( WM_USER + 0x4001 ) 
#define  GSM_ACK_SINGLE_RESULT         13  
#define  GSM_ACK_DIAL_RESULT           15   //����绰���״̬ACK, 1:��ͨ�� 0:�Ҷ�

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

//����״̬
extern ConnectStatus	g_enConnectStatus;
//��g_enConnectStatusΪCONNECT_STATUS_DISCONNECTEDʱ������Ͽ��ĸ���ԭ��
extern ExtraStatus		g_enExtraStatus;
static BOOL		g_bIsDialing;		//�Ƿ��ڲ���


class CWirelessManagerDlg : public CDialog
{
// Construction
public:
	typedef struct
	{
		DWORD		dwProcID;		//����ID
		HWND		hStatusWnd;		//Ŀ�Ĵ��ھ��
	} stHWNDSet;
	enum
	{
		TID_START_GSM_MODUAL = 1001,
		TID_GSM_INIT,
		TID_AUTO_CONNECT,
		TID_FORCE_CHECK_GPRS,
		TID_CHECK_RAS_CONNECTION,	//����������RAS�󣬶�ʱ����Ƿ����ӳɹ�
		TID_DISCONNECT,			//��ʱ���Ͽ�
		TID_DIAL,
	};
	enum DIAL_STYPE
	{
		DIAL_TYPE_NONE,
		enRAS_Dial,			//����
		enRAS_Hangup,		//�Ҷ�
		enRAS_ReDial,		//�ز�
	};
	CWirelessManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WIRELESSMANAGER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	private:
		HRASCONN		m_hRasConn;			//RAS���
		RASCONNSTATE	m_RasState;			//��ǰRAS״̬
		stHWNDSet		m_stHWNDSet[32];
		CString			m_csIPSet[5];		//IP��ַ
		DIAL_STYPE		m_enDialStype;		//��������
		DWORD			m_dwReDialCnt;		//Ras�ز�����������5����Reset GSMģ��
		bool            m_bDialing;

		DWORD	DialRasNetwork();		//����Ras����
		void	RasControl(DIAL_STYPE enStype);		//���в�������
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
