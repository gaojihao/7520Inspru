
#include "StdAfx.h"
#include "scom_api.h"
#include "../ExTool/ex_basics.h"
#include "serial_win32.h"
#include "../ExTool/ex_debug.h"
#include "../ExTool/ex_file.h"

#define  MAX_RECV_BUF         ( 1024 * 4 )

unsigned long    g_scom_thrid  = 0;
HANDLE           g_scom_handle = 0;
scom_ctrl         g_ctrl;      // scom message control send & read
bool             g_scom_third_running = true;
//sec_lock         g_lock_send;

scom_ctrl* scom_api_ctrl()
{
	return &g_ctrl;
}

bool scom_api_start(unsigned n_com, unsigned n_baud)
{
	bool b_suc = g_ctrl.connect_scom( n_com, n_baud );
	if ( b_suc )
	{
		if ( g_scom_handle == 0 )
		{
			g_scom_third_running = true;
			g_scom_handle = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)watching_scom,
				NULL,
				0,
				&g_scom_thrid );
		}
	}
	return b_suc;
}

void    scom_api_close()
{
	if ( g_scom_handle )
	{
		g_scom_third_running = false;
		if ( WAIT_TIMEOUT == WaitForSingleObject( g_scom_handle, 200) )
		{
			EXDBG_TRACE0(" Wating watching scom thread failed..\n");
			TerminateThread( g_scom_handle, 0 );
		}
	}
	g_ctrl.shutdown_scom();
	g_scom_third_running = false;
	g_scom_handle = 0;
}

void  watching_scom( void  *a_param )
{
	unsigned  offset = 0;
	char*     ptr_str = 0;
	int       n_count = 0;
	while ( g_scom_third_running )
	{
		g_ctrl.wait_com_rx();
		Sleep(10);
		g_ctrl.reset_com_rx();
		g_ctrl.read_empy_from_scom_com();
	}
}
