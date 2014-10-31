
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
}
