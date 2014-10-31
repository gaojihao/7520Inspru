
#include "StdAfx.h"
#include "gsm_reset.h"
#include "GPRSLogic.h"
#include <Winbase.h>
#include "../GTransDlg.h"
#include "../lib/IOControl.h"
#include "../LogTool/LogTool.h"

int  g_notify_msg = 0;
HWND g_notify_wnd = NULL;
HANDLE  g_thrd_handle = 0;

extern void   ShowTip(CString strTip);
void	gsm_reseting( void *a_param );
BOOL	LoadDriver(TCHAR *szDevKey, TCHAR *szFileName);
void	ReLoadMuxDriver(void);

void gsm_module::set_notify(HWND h_wnd, int n_msg )
{
	g_notify_wnd = h_wnd;
	g_notify_msg = n_msg;
}

//////////////////////////////////////////////////////////////////////////

void gsm_module::switch_voice2gsm()
{
	int n_val = 0;
	n_val = IO_ChangeAmp(FALSE);	
	if ( n_val == 0 )
		n_val = IO_ChangeAmp(FALSE);
	if ( n_val == 1 )
		ShowTip( _T("GSM Voice=1...\n") );
	else
		ShowTip( _T("GSM Voice=0...\n") );
}

//////////////////////////////////////////////////////////////////////////

void gsm_module::switch_voice2gps()
{
	int n_val = 0;
	n_val = IO_ChangeAmp(TRUE);
	if ( n_val == 0 )
		IO_ChangeAmp(TRUE);
	if ( n_val == 1 )
		ShowTip( _T("GPRS Voice=1...\n") );
	else
		ShowTip( _T("GPRS Voice=0...\n") );
}

void  gsm_reseting( void *a_param )
{
	//1.通讯状态进入reset模式
	CGPRSLogic::Instance()->ResetGSM();	
	//3.关闭GSM串口
	CGPRSLogic::Instance()->CloseGSM();	

	ShowTip(_T("GSM模块断电......"));

	IO_OpenGprsPower(FALSE);
	ShowTip(_T("GSM模块准备上电中......"));

	Sleep( 3 * 1000 );
	IO_OpenGprsPower(TRUE);
	Sleep( 3 * 1000 );


	CLogTool::Instance()->WriteLogFile(_T("GPRS Power Reset"));
	//MUX需要加载两次
	ReLoadMuxDriver();
	Sleep( 1 * 1000 );
	ReLoadMuxDriver();
	ShowTip(_T("MUX 加载完成"));
	::PostMessage(g_notify_wnd, g_notify_msg, 1, 0 );

	g_thrd_handle = 0;
}

BOOL LoadDriver(TCHAR *szDevKey, TCHAR *szFileName)
{
	HANDLE hdevice;
	DEVMGR_DEVICE_INFORMATION DeviceInfo = {sizeof(DEVMGR_DEVICE_INFORMATION)};

	hdevice = CreateFile(szFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hdevice != INVALID_HANDLE_VALUE)
	{
		if(!GetDeviceInformationByFileHandle(hdevice, &DeviceInfo))
			return FALSE;
		CloseHandle(hdevice);
		if(!DeactivateDevice(DeviceInfo.hDevice))
			return FALSE;
	}

	hdevice = ActivateDeviceEx(szDevKey, NULL, 0, NULL);
	if(hdevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	return TRUE;
}

void ReLoadMuxDriver(void) 
{
	TCHAR *sDevKey;
	TCHAR *sFileName;
	int i;

	sDevKey = _T("Drivers\\BuiltIn\\GSM0710_1");
	sFileName = _T("COM0:");
	for(i=0;i<5;i++)
	{
		if(LoadDriver(sDevKey, sFileName))
			break;
	}

	sDevKey = _T("Drivers\\BuiltIn\\GSM0710_2");
	sFileName = _T("COM7:");
	for(i=0;i<5;i++)
	{
		if(LoadDriver(sDevKey, sFileName))
			break;
	}

	return;
}

void gsm_module::reset_hd()
{
	if ( g_thrd_handle != 0 )
	{
		return;
	}

	g_thrd_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)gsm_reseting, NULL, 0, 0 );
}