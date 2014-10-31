
#include "StdAfx.h"
#include "msg_cunion.h"


msg_mid::msg_mid(char* sz_buf, int n_size ) : 
    msg_base(sz_buf, n_size)
{
}

msg_mid::~msg_mid()
{

}

//////////////////////////////////////////////////////////////////////////
	
bool  msg_mid::build_head()
{
	if ( m_max_len > 10  && m_data != 0)
	{
		//1¡¢ÃüÁîÍ·
		m_data[OFFSET_HEAD] = 0xFF;
		m_data[OFFSET_MSGLEN] = 0x00;
		m_data[OFFSET_MSGLEN+1] = 10;
	}
	return true;
}

bool  msg_mid::build_tail()
{
	m_data[m_msg_len-1] = 0xFF;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool  msg_mid::build_length()
{
	int nLen = m_msg_len - 4;
	m_data[OFFSET_MSGLEN] = ( nLen >> 8 ) & 0xFF;
	m_data[OFFSET_MSGLEN+1] = ( nLen ) & 0xFF;
	return true;
}

bool  msg_mid::build_payload(int msg_type, const char* p_msg, int n_len)
{
	m_msg_len = n_len + 6;
	if ( m_max_len < m_max_len )
		return false;
	m_data[OFFSET_DEVICE_TYPE] = 0x01;//GPS Device
	m_data[OFFSET_MSGID] = (char)msg_type;
	memcpy(&m_data[OFFSET_PAYLOAD], p_msg, n_len );

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool  msg_mid::build_msg()
{
	build_head();
	build_length();
	calc_xor();
	build_tail();
	return true;
}
