
#include "StdAfx.h"
#include "ex_basics.h"
#include "serial_win32.h"
#include "rectrl_api.h"

#define  MAX_RECTRL_BUF         ( 1024 * 2 )
unsigned char     g_rectrl_buf[MAX_RECTRL_BUF];

extern HWND g_notify_wnd;
bool               g_rectrl_running = false;
unsigned long      g_rectrl_thrid  = 0;
HANDLE             g_rectrl_handle = 0;
serial_win32       g_rectrl_win32;
st_rectrl          g_st_rectrl;

void  watching_rectrl( void  *a_param );
int   read_empty_rectrl_com( unsigned char* ptr, int n_ptr_size );
void  parse_rectrl_data(unsigned char* p_ptr, int n_length );
//////////////////////////////////////////////////////////////////////////
bool  rectrl_api_start(unsigned n_com, unsigned n_baud)
{
	bool b_suc = g_rectrl_win32.open( n_com, n_baud );
	if ( b_suc )
	{
		if ( g_rectrl_handle == 0 )
		{
			g_rectrl_running = true;
			g_rectrl_handle = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)watching_rectrl,
				NULL,
				0,
				&g_rectrl_thrid );
		}
	}
	return b_suc;
}


void    rectrl_api_close()
{
	if ( g_rectrl_handle )
	{
		g_rectrl_running = false;
		if ( WAIT_TIMEOUT == WaitForSingleObject( g_rectrl_handle, 200) )
		{
			TerminateThread( g_rectrl_handle, 0 );
		}
	}
}



void  watching_rectrl( void  *a_param )
{
	unsigned  offset = 0;
	char*     ptr_str = 0;
	int       n_count = 0;
	while ( g_rectrl_running )
	{
		g_rectrl_win32.wait_event();
		Sleep(10);
		g_rectrl_win32.set_event(EV_RXCHAR);
		
		//n_count = g_rectrl_win32.read_data(g_rectrl_buf, MAX_RECTRL_BUF );
		n_count = read_empty_rectrl_com( g_rectrl_buf, MAX_RECTRL_BUF );
		if ( n_count >= 0 && n_count < MAX_RECTRL_BUF )
		{
			
			parse_rectrl_data( g_rectrl_buf, n_count );
		}
	} // while ( true )
}

int   read_empty_rectrl_com(unsigned char* ptr, int n_ptr_size)
{
	int  offset = 0;
	int  n_read = 0;
	for(;;)
	{
		if ( offset >= n_ptr_size )
		{
			break;
		}
		n_read = g_rectrl_win32.read_data( (char*)&ptr[offset], 2 );
		
#if 0
		if ( n_read > 0 )
		{
			char szBuf[64];
			memcpy( szBuf, &ptr[offset], n_read );
			szBuf[n_read] = '\0';
			ex_trace_msg( szBuf );
		}
#endif
		
		
		if ( n_read > 0 )
		{
			offset += n_read;
		}
		else
		{
			break;
		}
	}

	return offset;
}
void  parse_rectrl_data(unsigned char* p_ptr, int n_length )
{
	if ( p_ptr == 0 || n_length < 6 ) return;
	if ( p_ptr[0] != 0x78 ) return;

	g_st_rectrl.dev_addr = p_ptr[1];

	//个数
	int n_num = p_ptr[2];
	if ( n_num != 1 ) return;

	g_st_rectrl.dev_chanle = p_ptr[3];
	if ( p_ptr[5] != 0x72 ) return;
	g_st_rectrl.dev_state = p_ptr[4];
	if ( g_notify_wnd )
		::PostMessage(g_notify_wnd, MSG_GSM_NOTIFY,eGSM_RECTRL_OPEN, (long)&g_st_rectrl);	
}
// 0x78 0x01 01 01 00 0x72
//      0x02

//下发
void      rectrl_api_enable(bool b_open_dev)
{
	unsigned char sz_cmd[32];

	sz_cmd[0] = 0x78;
	sz_cmd[1] = 0xFF;
	sz_cmd[2] = 0x01; //表示只有一个
	sz_cmd[3] = 0x03;/*g_st_rectrl.dev_chanle + 1*/;
	sz_cmd[4] = b_open_dev;
	sz_cmd[5] = 0x72;

	g_rectrl_win32.write_data( (const char*)sz_cmd, 6 );
}