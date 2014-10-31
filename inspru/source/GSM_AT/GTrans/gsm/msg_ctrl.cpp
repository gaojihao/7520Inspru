
#include "stdafx.h"
#include "msg_ctrl.h"
#include "ex_basics.h"
#include "ex_debug.h"
#include "gsm_msg_def.h"
#include <windows.h>

#define TID_FOR_START_GSM          1011
#define TID_FOR_CHECK_SIGNAL_INIT  1012
#define TID_FOR_CHECK_SIGNAL       1013



msg_ctrl::msg_ctrl()
{
	m_check_line = 0;
}

//////////////////////////////////////////////////////////////////////////

msg_ctrl::~msg_ctrl() 
{ 


}

//////////////////////////////////////////////////////////////////////////

void  msg_ctrl::init_msg_ctrl(  HWND h_wnd )
{
#if 0
	m_gsm.set_owner( h_wnd );
	::SetTimer(h_wnd,TID_FOR_START_GSM, 100, NULL);
	m_check_line = 0; 
#endif
}

//////////////////////////////////////////////////////////////////////////

bool  msg_ctrl::on_message( unsigned int u_msg, unsigned int w_param, long l_param )
{
 	bool b_has_done = true;
 	switch( u_msg )
	{
	case WM_TIMER: on_timer( w_param ); break;
 	case WM_COPYDATA:
		{
			COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)(l_param);
 			on_data_copy(pcds->dwData, pcds->lpData, pcds->cbData );
		}
		break;
	case UMSG_GSM_CMD:
//		m_gsm.on_message(u_msg, w_param, l_param );
		break;
	 case WM_DESTROY:
		 PostQuitMessage( 0 );
	 	b_has_done = false;
	 	break;
	 default:
	 	b_has_done = false;
	 	break;
	}
 	return b_has_done;
}

//////////////////////////////////////////////////////////////////////////

void  msg_ctrl::on_timer( unsigned timer_id )
{
#if 0
	if ( timer_id == TID_FOR_START_GSM )
	{
		KillTimer( m_gsm.get_main_wnd(), timer_id );
		m_gsm.on_gsm_cmd( GSM_CMD_OPEN, 0 );
		::SetTimer(m_gsm.get_main_wnd(),TID_FOR_CHECK_SIGNAL_INIT, 60 * 1000, NULL );

	}
	else if ( timer_id == TID_FOR_CHECK_SIGNAL )
	{
		m_gsm.on_gsm_cmd( GSM_CMD_CHECK_SIGNAL, 0 );
	}else if ( timer_id == TID_FOR_CHECK_SIGNAL_INIT )
	{
		if ( m_check_line++ > 10 )
		{
 			KillTimer( m_gsm.get_main_wnd(),timer_id );
 			::SetTimer( m_gsm.get_main_wnd(),GSM_CMD_CHECK_SIGNAL, 50 * 1000, NULL );
 		}
		m_gsm.on_gsm_cmd( GSM_CMD_CHECK_SIGNAL, 0 );		
	}
#endif
}

//////////////////////////////////////////////////////////////////////////

void  msg_ctrl::on_data_copy( unsigned int n_type, void* p_data, int n_len )
{
#if 0
	switch ( n_type )
	{
	case GSM_DCOPY_PHONE: 
		m_gsm.set_phone_num( (char*)p_data, n_len ); 
		break;
	case GSM_DCOPY_PHONE_EXT:
		m_gsm.set_phone_ext( (char*)p_data, n_len );
		break;
	case GSM_DCOPY_MSG:
		m_gsm.set_msg_buf( (char*)p_data, n_len ); 
		break;
	case GSM_DCOPY_PBOOK:
		m_gsm.set_sim_pbook( (PBOOK_ITEM*)p_data );
		break;
	}
#endif
}

//////////////////////////////////////////////////////////////////////////

void msg_ctrl::on_test_gsm()
{
}

//////////////////////////////////////////////////////////////////////////
void  msg_ctrl::on_test_memory()
{
	MEMORYSTATUS ms;
	memset(&ms, 0,  sizeof(ms) );
	::GlobalMemoryStatus(&ms);
	char  sz_buf[128];
	sprintf(sz_buf,"%d__%d K\0", ms.dwAvailPhys/11024, ms.dwAvailPhys/11024);
	
	ex_trace_scrn( 10, 10, sz_buf );
}
