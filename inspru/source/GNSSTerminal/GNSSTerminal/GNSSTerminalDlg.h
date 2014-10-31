// GNSSTerminalDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "./Lib/Location.h"
#include "SocketManager/SocketManager.h"


// CGNSSTerminalDlg dialog
class CGNSSTerminalDlg : public CDialog
{
// Construction
public:
	CGNSSTerminalDlg(CWnd* pParent = NULL);	// standard constructor
	~CGNSSTerminalDlg();

// Dialog Data
	enum { IDD = IDD_GNSSTERMINAL_DIALOG };
	

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	BOOL	ReplaceUpgradeFile();

	void	OnsysPowerChange(WPARAM wparam);
	static DWORD THDStartTTS(LPVOID lpvoid);

	void	OnCmdCOPYDATA(WPARAM wParam, LPARAM lParam );

	BOOL	ConnectSocket();
	static DWORD    StartCKProcess(LPVOID lpParameter);
	BOOL   m_bInitSocket;
	PCSTR		ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator = ',');

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	void readroutefile();

	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
