
#include "stdafx.h"
#include "msg_gsm.h"
#include "../GPRS/ex_basics.h"
#include "gsm_msg_def.h"
#include <windows.h>

#define TID_FOR_START_GSM          1011
#define TID_FOR_CHECK_SIGNAL_INIT  1012
#define TID_FOR_CHECK_SIGNAL       1013


msg_gsm::msg_gsm()
{
	m_check_line = 0;
}

//////////////////////////////////////////////////////////////////////////

msg_gsm::~msg_gsm() 
{ 
}

//////////////////////////////////////////////////////////////////////////

void  msg_gsm::set_ipc(msg_ipc* p_ipc)
{
	m_sms.set_ipc( p_ipc );
	m_dial.set_ipc( p_ipc );
}

//////////////////////////////////////////////////////////////////////////

bool  msg_gsm::on_message( unsigned int u_msg, unsigned int w_param, long l_param )
{
	bool b_has_done = true;
	switch( u_msg )
	{
	case WM_COPYDATA:
		{
			COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)(l_param);
			on_data_copy(pcds->dwData, pcds->lpData, pcds->cbData );
		}
		break;
	case UMSG_GSM_CMD:
		m_dial.on_message( w_param, l_param );
		m_sms.on_message( w_param, l_param );
		break;
	default:
		break;
	}
	return true;
}


void  msg_gsm::read_sim_sms(int n_idx )
{
	m_sms.cmd_gsm_read_sim_sms( n_idx );
}

void  msg_gsm::Get_sms_content(int nSMS )
{
	m_sms.cmd_gsm_get_sms_content( nSMS );
}

void  msg_gsm::on_data_copy( unsigned int n_type, void* p_data, int n_len )
{
	switch ( n_type )
	{
	case GSM_DCOPY_PHONE: 
		m_dial.set_phone_num( (char*)p_data, n_len );
		m_sms.set_phone_num( (char*)p_data, n_len );
		break;
	case GSM_DCOPY_PHONE_EXT:
		m_dial.set_phone_ext( (char*)p_data, n_len );
		break;
	case GSM_DCOPY_MSG:
		m_sms.set_msg_buf( (char*)p_data, n_len ); 
		break;
	case GSM_DCOPY_PBOOK:
		m_sms.add_sim_pbook( (PBOOK_ITEM*)p_data );
		break;
	}
}


//////////////////////////////////////////////////////////////////////////

void  msg_gsm::on_timer( unsigned timer_id )
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


//////////////////////////////////////////////////////////////////////////

void msg_gsm::on_test_gsm()
{
}

//////////////////////////////////////////////////////////////////////////
void  msg_gsm::on_test_memory()
{
	MEMORYSTATUS ms;
	memset(&ms, 0,  sizeof(ms) );
	::GlobalMemoryStatus(&ms);
	char  sz_buf[128];
	sprintf(sz_buf,"%d__%d K\0", ms.dwAvailPhys/11024, ms.dwAvailPhys/11024);
}
