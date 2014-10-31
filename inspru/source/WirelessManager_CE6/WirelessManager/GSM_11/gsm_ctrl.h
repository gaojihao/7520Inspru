/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\gsm_ctrl.h

 $Reversion History:

        (1) Created Liuhl@2008-03-05 05:30 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  GSM_CTRL_H
#define  GSM_CTRL_H

#include "ex_basics.h"
#include "serial_win32.h"
//#include "gsm_parse.h"
class  gsm_ctrl
{
public:
	gsm_ctrl();
	~gsm_ctrl();

	enum{ max_buf = 2 * Ex_1KB };

public:
	//void   set_parse_ptr(gsm_parse* p_parse) { m_parse_ptr = p_parse; }
	bool   connect_gsm(int n_port, int n_baud);
	bool   shutdown_gsm();

	bool   send_msg(const char* p_msg, unsigned n_size );
	bool   send_gsm_cmd( unsigned n_type );

	void   wait_com_rx()  { m_serial.wait_event(); }
	void   reset_com_rx() { m_serial.set_event(EV_RXCHAR); }

	char*  ipd_ack();
	int    ipd_len();
	char*  ack_buf();//     { return 0; }
	int    ack_len();//     { return m_read_count;}
	bool   check_ack ( const char* p_ack );
	
public:
	int    read_empy_from_gsm_com();
	void   empty_data( bool b_ack = true );

protected:
	void   parse_data();
	int		parse_gsm_signal(const char* p_buf);

protected:
	//gsm_parse*     m_parse_ptr;
	sec_lock       m_cs;
	serial_win32   m_serial;
	char           m_parse_buf[max_buf];
	char           m_read_buf[max_buf];
	int            m_read_count;
};

#endif //GSM_CTRL_H