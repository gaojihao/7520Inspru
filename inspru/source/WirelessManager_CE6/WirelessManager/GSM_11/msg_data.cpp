
#include "StdAfx.h"
#include "msg_data.h"


msg_data*  msg_data::_instance = 0;
msg_data*  msg_data::instance()
{
	if ( _instance == 0 )
		_instance = new msg_data;
	return _instance;
}

msg_data::msg_data()
{
	memset(m_ack_buf, 0, sizeof(m_ack_buf) );
	memset(m_ipd_buf, 0, sizeof(m_ipd_buf) );
	m_ipd_len = 0;
	m_ack_len = 0;
}


msg_data::~msg_data()
{
}

void   msg_data::empty_ipd()
{
	m_ipd_len = 0;
	memset( m_ipd_buf, 0, sizeof(m_ipd_buf) );
	//char   m_ack_buf[256];
	//char   m_ipd_buf[Ex_1KB * 2];
}

void   msg_data::set_ack_buf( char* p_buf, int n_buf_size)
{
	char* p_str = strstr( p_buf, "+IPD");
	int   n_val32 = 0;
	int   n_ack_len = n_buf_size;
	m_ipd_len = 0;

	if ( p_str )
	{
		char* p_ipd_ptr = p_str + 4;
		n_val32= atoi( p_ipd_ptr );
		p_ipd_ptr = strstr( p_ipd_ptr, ":");
		if ( n_val32 > sizeof(m_ipd_buf) )
			n_val32 = sizeof(m_ipd_buf) - 1;
		if ( n_val32 < 0 ) n_val32 = 0;

		if ( p_ipd_ptr )
		{
			p_ipd_ptr++;
			memcpy( &m_ipd_buf[m_ipd_len], p_ipd_ptr, n_val32 );
			m_ipd_len = n_val32;
			p_ipd_ptr += n_val32;
			n_ack_len -= p_ipd_ptr - p_str;
		}
		
		if ( n_buf_size > p_ipd_ptr - p_buf )
		{
			n_val32 = p_ipd_ptr - p_buf;
			memcpy( p_str, p_ipd_ptr, n_val32 );
			p_str[n_val32] = '\0';
		}else
		{
			p_str[0] = '\0';
		}
	}
	

	if ( n_ack_len > 0 )
	{
		memcpy( m_ack_buf, p_buf, n_ack_len );
		m_ack_buf[n_ack_len] ='\0';
		m_ack_len = n_ack_len;
	}
	else
	{
		m_ack_len = 0;
		m_ack_buf[0] = '\0';
	}

}
