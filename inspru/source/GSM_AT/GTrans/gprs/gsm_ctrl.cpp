
#include "StdAfx.h"
#include "gsm_ctrl.h"
#include "gsm_at.h"
#include "gsm_api.h"
#include "msg_data.h"
extern void   ShowTip(CString strTip);


gsm_ctrl::gsm_ctrl()
{
	memset(m_parse_buf, 0, sizeof(m_parse_buf) );
	memset(m_read_buf, 0, sizeof(m_read_buf) );
	m_read_count = 0;
	m_parse_ptr = 0;
}

//////////////////////////////////////////////////////////////////////////

gsm_ctrl::~gsm_ctrl()
{
	memset(m_parse_buf, 0, sizeof(m_parse_buf) );
	memset(m_read_buf, 0, sizeof(m_read_buf) );
	m_read_count = 0;
	m_serial.close();
}

//////////////////////////////////////////////////////////////////////////

bool   gsm_ctrl::connect_gsm(int n_port, int n_baud)
{
	return m_serial.open(n_port, n_baud );
}

//////////////////////////////////////////////////////////////////////////

bool   gsm_ctrl::shutdown_gsm()
{
	return m_serial.close();
}

//////////////////////////////////////////////////////////////////////////
bool  gsm_ctrl::send_msg( const char* p_msg, unsigned n_size )
{
	int i;
	TRACE0("\n********Send GSM data***************************\n");
	for(i=0; i<n_size; i++)
	{
		TRACE1("%c", (UCHAR)p_msg[i]);
	}
	
	return m_serial.write_data( p_msg, n_size );
}

//////////////////////////////////////////////////////////////////////////

bool   gsm_ctrl::check_ack( const char* p_ack)
{
	bool b ;//= (strstr( m_ack_buf, p_ack ) != 0);
	b = ( strstr( msg_data::instance()->ack_data(), p_ack ) != 0 );
	return b;
}

char*  gsm_ctrl::ipd_ack()
{
	return  msg_data::instance()->ipd_data();
}


int    gsm_ctrl::ipd_len()
{
	return msg_data::instance()->ipd_len();

}


char*  gsm_ctrl::ack_buf() //     { return 0; }
{
	return  msg_data::instance()->ack_data();
}

int    gsm_ctrl::ack_len()//     { return m_read_count;}
{
	return msg_data::instance()->ack_len();
}
//////////////////////////////////////////////////////////////////////////

void   gsm_ctrl::empty_data(bool b_ack /*= true*/ )
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

bool   gsm_ctrl::send_gsm_cmd( unsigned n_type )
{
	const char * p_cmd = gsm_api_cmd( n_type );
	if ( p_cmd )
		return send_msg( p_cmd, strlen(p_cmd) );
	else
		return false;
}


//////////////////////////////////////////////////////////////////////////

int gsm_ctrl::read_empy_from_gsm_com( )
{
	int  offset = 0;
	int  n_read = 0;
	int  i = 0;
	memset(m_read_buf, 0, sizeof(m_read_buf));
	for(;;)
	{
		if ( offset >= max_buf )
		{
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
		memcpy( m_parse_buf, m_read_buf, m_read_count );
		m_parse_buf[m_read_count] = '\0';
		parse_data(); 
		TRACE0("\n########Receive GSM data start:########################\n");
		for(i=0; i<m_read_count; i++)
		{
			TRACE1("%c", m_read_buf[i]);
		}
		TRACE0("\n########Receive GSM data end:########################\n");
	}

	return offset;
}

//////////////////////////////////////////////////////////////////////////

void   gsm_ctrl::parse_data()
{
	if ( m_parse_ptr )
		m_parse_ptr->parse( m_parse_buf, m_read_count );
}
