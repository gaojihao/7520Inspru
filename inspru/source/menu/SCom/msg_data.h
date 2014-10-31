
#ifndef  MSG_DATA_H
#define  MSG_DATA_H

#include "ex_basics.h"

class  msg_data
{
public:
	static msg_data*  instance();
	~msg_data();

	void   set_ack_buf( char* p_buf, int n_buf_size);
	char*  buffer_data()   { return m_ipd_buf; }
	int    buffer_len()    { return m_ipd_len; }

	void   empty_ack()  { m_ack_buf[0] ='\0';}
	void   empty_ipd();
protected:
	msg_data();
	static msg_data*  _instance;

protected:
	char   m_ack_buf[256];
	char   m_ipd_buf[Ex_1KB];
	int    m_ipd_len;
	int    m_ack_len;
};
#endif //MSG_MID_H