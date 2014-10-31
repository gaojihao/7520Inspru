
#include "StdAfx.h"
#include "scom_ctrl.h"
#include "../ExTool/ex_debug.h"
#include "scom_api.h"
//#include "msg_data.h"

scom_ctrl::scom_ctrl()
{
	memset(m_parse_buf, 0, sizeof(m_parse_buf) );
	memset(m_read_buf, 0, sizeof(m_read_buf) );
	m_read_count = 0;
	m_parse_ptr = 0;
}

//////////////////////////////////////////////////////////////////////////

scom_ctrl::~scom_ctrl()
{
	memset(m_parse_buf, 0, sizeof(m_parse_buf) );
	memset(m_read_buf, 0, sizeof(m_read_buf) );
	m_read_count = 0;
	m_serial.close();
}

//////////////////////////////////////////////////////////////////////////

bool   scom_ctrl::connect_scom(int n_port, int n_baud)
{
	return m_serial.open(n_port, n_baud );
}

//////////////////////////////////////////////////////////////////////////

bool   scom_ctrl::shutdown_scom()
{
	return m_serial.close();
}

//////////////////////////////////////////////////////////////////////////

bool  scom_ctrl::send_msg( const char* p_msg, unsigned int n_size )
{	
	unsigned int i=0;
	TRACE0("\n********Send scom data start:***************************\n");
	for(i=0; i<n_size; i++)
	{
		TRACE1("%c", (UCHAR)p_msg[i]);
	}
	TRACE0("\n--------16进制数据-------------------------------------\n");
	for(i=0; i<n_size; i++)
	{
		TRACE1("%02X ", (UCHAR)p_msg[i]);
		if( (i+1)%20 == 0 && (i+1) != n_size )
			TRACE0("\n");
	}
	TRACE0("\n********scom data end***********************************\n");
	
	return m_serial.write_data( p_msg, n_size );
}

//////////////////////////////////////////////////////////////////////////
/*
bool   scom_ctrl::check_ack( const char* p_ack)
{
	bool b ;//= (strstr( m_ack_buf, p_ack ) != 0);
	b = ( strstr( msg_data::instance()->ack_data(), p_ack ) != 0 );
	return b;
}

char*  scom_ctrl::ipd_ack()
{
	return  msg_data::instance()->ipd_data();
}

int scom_ctrl::ipd_len()
{
	return msg_data::instance()->ipd_len();
}

char*  scom_ctrl::ack_buf() //     { return 0; }
{
	return  msg_data::instance()->ack_data();
}

int    scom_ctrl::ack_len()//     { return m_read_count;}
{
	return msg_data::instance()->ack_len();
}
//////////////////////////////////////////////////////////////////////////

void   scom_ctrl::empty_data(bool b_ack )
{
	if ( b_ack )
	{
		msg_data::instance()->empty_ack();
	}else
	{
		;
	}
}
//////////////////////////////////////////////////////////////////////////

bool   scom_ctrl::send_scom_cmd( unsigned n_type )
{
	const char * p_cmd = scom_api_cmd( n_type );
	if ( p_cmd )
		return send_msg( p_cmd, strlen(p_cmd) );
	else
		return false;
}
*/

//////////////////////////////////////////////////////////////////////////

int scom_ctrl::read_empy_from_scom_com()
{
	int  offset = 0;
	int  n_read = 0;
	int i=0;
	for(;;)
	{
		if ( offset >= max_buf )
		{
			EXDBG_TRACE0("receive buffer has overlap.....\n");
			break;
		}
		n_read = m_serial.read_data( &m_read_buf[offset], 2 );

		if ( n_read > 0 )
		{
			offset += n_read;
		}
		else
		{
			break;
		}
	}
	m_read_count = offset;
	if ( m_read_count > 0 )
	{
		TRACE0("\n>>>>>>>>>>Receive Serial COM Data:>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		for(i=0; i<m_read_count; i++)
		{
			TRACE1("%c", m_read_buf[i]);
		}
		TRACE0("\n--------16进制数据-------------------------------------\n");
		for(i=0; i<m_read_count; i++)
		{
			TRACE1("%02X ", (unsigned char)m_read_buf[i]);
			if( (i+1)%20 == 0 && (i+1) != m_read_count )
				TRACE0("\n");
		}
		TRACE0("\n>>>>>>>>>>end>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

		memcpy( m_parse_buf, m_read_buf, m_read_count );
		m_parse_buf[m_read_count] = '\0';
		//ex_trace_msg( m_parse_buf );
		//parse_data();
	}

	return offset;
}
/*
void   scom_ctrl::parse_data()
{
	if ( m_parse_ptr )
		m_parse_ptr->parse( m_parse_buf, m_read_count );
}
*/