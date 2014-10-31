
#include "msg_gsm_ack.h"
#include "gsm_api.h"
#include "gsm_msg_def.h"
#include "ak4642api.h"
#include "ex_debug.h"
#include "msg_gsm.h"


msg_gsm_ack::msg_gsm_ack()
{
	m_ipc = 0;
	m_is_calling = false;
	memset( m_call_num, 0, sizeof(m_call_num) );
}

//////////////////////////////////////////////////////////////////////////

msg_gsm_ack::~msg_gsm_ack()
{
	m_ipc = 0;
}

//////////////////////////////////////////////////////////////////////////

bool msg_gsm_ack::ack_new_msg( const void* p_param )
{
	if ( m_ipc == 0 || p_param == 0 )
		return false;
	EXDBG_TRACE0("ACK_NEW_SMS MESSAGE...\n");
	SM_PARAM* pMsg = (SM_PARAM*)p_param;
	m_ipc->past_data( GSM_DCOPY_ACK_MSG, (const char*)p_param, sizeof(SM_PARAM) );	
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool  msg_gsm_ack::ack_disconnect(int nReason )
{
	EXDBG_TRACE0("ACK_DISCONNECT...\n");
	set_calling( false );
	if ( m_ipc )
		m_ipc->post_msg( UMSG_GSM_CMD, nReason, 0 );
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool  msg_gsm_ack::ack_connect(const char* p_num)
{
	EXDBG_TRACE0("ACK_CONNECT...\n");
	set_calling( true );
	if ( m_ipc )
	{
		m_ipc->send_msg( UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT,1 );
		m_ipc->post_msg(  UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT,1 );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool  msg_gsm_ack::ack_new_call( const void* p_param )
{
	return false;

	if ( !is_calling() )
	{
		EXDBG_TRACE0("ACK_NEW_CALLING...\n");
		const char* pMsg = (const char*)p_param;
		memset( m_call_num , 0, sizeof(m_call_num) );
		for( int i = 0; i < sizeof(m_call_num); i++ )
		{
			if ( pMsg[i] == '\0' || pMsg[i] == '\"' )
				break;
			m_call_num[i] = pMsg[i];
		}
		set_calling( true );
		m_ipc->past_data( GSM_DCOPY_ACK_PHONE, m_call_num, -1 );
		msg_gsm::switch_voice2gsm();
		if ( m_ipc )
			m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_NEW_CALLING,  0 );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

void  msg_gsm_ack::ack_gsm_signal( int n_level )
{
	if ( m_ipc )
		m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_SIGNAL, n_level );
}

//////////////////////////////////////////////////////////////////////////

void  msg_gsm_ack::ack_gsm_net( const char* p_num )
{
	if( strstr(p_num, "CHINA MOBILE") != 0 )
	{
		if ( m_ipc )
			m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_QUERY_NET, 1 );
	}
	else if ( strstr(p_num, "CHINA UNICOM") != 0 )
	{
		if ( m_ipc )
			m_ipc->post_msg( UMSG_GSM_CMD, GSM_ACK_QUERY_NET, 2);
	}
}