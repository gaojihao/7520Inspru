
#ifndef  SCOM_CTRL_H
#define  SCOM_CTRL_H

#include "../ExTool/ex_basics.h"
#include "serial_win32.h"
#include "scom_parse.h"
class  scom_ctrl
{
public:
	scom_ctrl();
	~scom_ctrl();

	enum{ max_buf = Ex_1KB };

public:
	void   set_parse_ptr(scom_parse* p_parse) { m_parse_ptr = p_parse; }
	bool   connect_scom(int n_port, int n_baud);
	bool   shutdown_scom();

	bool   send_msg(const char* p_msg, unsigned int n_size );
	bool   send_scom_cmd( unsigned n_type );

	void   wait_com_rx()  { m_serial.wait_event(); }
	void   reset_com_rx() { m_serial.set_event(EV_RXCHAR); }

	char*  ipd_ack();
	int    ipd_len();
	char*  ack_buf();//     { return 0; }
	int    ack_len();//     { return m_read_count;}
	bool   check_ack ( const char* p_ack );
	
public:
	int    read_empy_from_scom_com();
	void   empty_data( bool b_ack = true );

protected:
	//void   parse_data();

protected:
	scom_parse*     m_parse_ptr;
	//sec_lock       m_cs;
	serial_win32   m_serial;
	char           m_parse_buf[max_buf];
	char           m_read_buf[max_buf];
	int            m_read_count;
};

#endif //SCOM_CTRL_H