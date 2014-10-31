// E9101Main.h : main header file for the E9101MAIN application
//

#if !defined(AFX_E9101MAIN_H__54F6B8B8_5D4A_4E01_8394_13E0B02C1D6A__INCLUDED_)
#define AFX_E9101MAIN_H__54F6B8B8_5D4A_4E01_8394_13E0B02C1D6A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "E9101HardInterface.h"
#include <afxmt.h>
//#include "../DriveLib/midware.h"
#include "SysConfig.h"
#include "LogFile.h"
#include "CDirsRef.h"

#include "../GSM/GSMLogic.h"

#include "../FileMapping/FileMapping.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../DriveLib/Location.h"

#define WM_VOL_CHANGE_STYLE				WM_USER+10382
#define WM_FM_CHANGE					WM_USER+10383

#define WM_LCD_CLOSE 					WM_USER + 18		//	��ҪLCD�ر�ʱ�����������̷߳���
#define WM_CHILD_EXIT  					WM_USER + 11		//	�˳�ʱ�����������̷߳���
#define WM_POWER_OFF					WM_USER + 12		//	��Ҫ��Դ�ر�ʱ�����������̷߳���
#define WM_POWER_EVENT					WM_USER + 13	
#define WM_LANGUAGE_CHANGE				WM_USER + 14		//  ���Ըı���Ϣ
#define WM_SKIN_CHANGE					WM_USER + 14		//  Ƥ���ı���Ϣ

#define WM_POWER_LOW_DLG_DESTROY		WM_USER + 15  //  ��ʾ�Ի���������Ϣ

#define WM_COMMAND_TIANZE				(WM_USER+0x1000)		//ͬ�������ͨѶ����Ϣ
#define WM_COMMAND_PHOTO				(WM_USER+0x1001)		//ͬ����ͷ����ͨѶ����Ϣ
#define WM_CLOSE_COMMAND				(WM_USER+0x1002)		//�ر�����ͷ
#define	WM_CAR_REVERSE					(WM_USER+0x322)			//������Ϣ
#define HW_VOLUME       				(WM_USER+0x324)			//��������
#define WM_MENU			       			(WM_USER+0x325)			//�������ϵĵ������������ȵİ���
#define WM_BACKLIGHT					(WM_USER+0x328)
#define WM_ADJUST_BRIGHTNESS_VOLUMN		(WM_USER+0x1002)		//�������Ⱥ��������̵���Ϣ��WPARAM��1����  2����  LPARAM�����������ȵȼ�
#define WM_GET_SIGNAL_FROM_TIANZE		(WM_USER+0x1003)		//������̻�ȡ�ź���
#define MSGID_SHOW_VOLUMN				(WM_USER+0x1004)		//����������Ϣ

#define WM_FEED_DOG								(WM_USER+0x3001)	//ι����Ϣ
#define WPARAM_DOG_FOR_MENU						1					//WPARAM��������ؽ���Menu.exe�Ƿ�dead
#define WPARAM_DOG_FOR_GSM_AT					2					//WPARAM��������ؽ���GSM_AT.exe�Ƿ�dead
#define WPARAM_DOG_FOR_WIRELESSMANAGER			3					//WPARAM��������ؽ���WirelessManager.exe�Ƿ�dead
#define WPARAM_DOG_FOR_NETWORK					4					//WPARAM�������������״̬

#define WM_ACC_ON_OFF					(WM_USER+0x320)			//ACC֪ͨ��wParam��0  ACC����  1  ACC�ϵ�

#define  SOUND_KEYCLICK			_T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")
#define  SOUND_KEYCLICK1		_T("\\Flashdrv Storage\\menu\\Style\\KeySound1.wav\0")
#define  SOUND_KEYCLICK2		_T("\\Flashdrv Storage\\menu\\Style\\KeySound2.wav\0")
#define  SOUND_KEYCLICK3		_T("\\Flashdrv Storage\\menu\\Style\\KeySound3.wav\0")
#define  SOUND_RING				_T("\\Flashdrv Storage\\menu\\Style\\ring.wav\0")

#define PATH_SD					_T("\\Storage Card")
#define PATH_UDISK				_T("\\USB DISK")
#define WND_NAME_GSM_AT			_T("WND_GSM_AT")
#define DLG_VOLUMN				_T("DLG_VOLUMN")

extern DWORD  g_dwSpareTime;


void PlayKeySound( );

extern PROCESS_INFORMATION		g_sProcessInfo;			//����������Ϣ

extern	CString g_csBlueToothName;
extern	CString g_csBlueToothIconName;

extern	CString g_csPicName;
extern	CString g_csPicIconName;

extern	CString g_csMusicName;
extern	CString g_csMusicIconName;

extern	CString g_csMovieName;
extern	CString g_csMovieIconName;

extern	CString g_csNaviName;
extern	CString g_csNaviIconName;

extern	CString g_csVolumeName;
extern	CString g_csVolumeIconName;

extern  CString g_csTxtName;
extern  CString g_csTxtIconName;

extern BOOL g_bFmInit ;

extern CRect g_retTitile;; 

extern enKeySound g_enKeySnd;

typedef enum enBtnState
{
	BTN_STATE_NORMAL,
	BTN_STATE_DOWN,
	BTN_STATE_DISABLE,
}enBtnState;

class ItemInfo
{
public:
	CRect rect;
	CString chChar;
	enBtnState nState;
};

/////////////////////////////////////////////////////////////////////////////
// CE9101MainApp:
// See E9101Main.cpp for the implementation of this class
//
#define TIMER_START_SPARE_MODE 0x6110



#define USE_GSM_MUX         1

//Harry@20081105
extern CGSMLogic   g_GsmLogic;
extern const POS_INFO*	m_pPosInfo;


class CE9101MainApp : public CWinApp
{
public:
	CE9101MainApp();

	static UINT s_SpareTimer;
	static void BtInitThread(LPVOID lParam);
	static DWORD THDAppExit(LPVOID lpParameter);

// Overrides
	// ClassWizard enerated virtual function overrides
	//{{AFX_VIRTUAL(CE9101MainApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	HANDLE m_hOneInstance;

// Implementation

	//{{AFX_MSG(CE9101MainApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWinThread* m_pPowerThread;
	CEvent m_ExitEvent;

	CWinThread* m_pProcessEventThread;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E9101MAIN_H__54F6B8B8_5D4A_4E01_8394_13E0B02C1D6A__INCLUDED_)
// E9101Main.h : main header file for the E9101MAIN application
//


