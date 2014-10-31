
/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\msg_gsm.h

 $Reversion History:

        (1) Created Liuhl@2007-12-14 13:50

 $Notes:
		

--*/

#ifndef  MSG_GSM_H
#define  MSG_GSM_H


#include "msg_sms.h"
#include "msg_dial.h"


class msg_gsm
{
public:
	msg_gsm();
	~msg_gsm();

	void   set_ipc(msg_ipc* p_ipc);

public:
	bool  on_message( unsigned int u_msg, unsigned int w_param, long l_param );
	void  on_data_copy(unsigned int n_type, void* p_data, int n_len);
	void  read_sim_sms(int n_idx );
	void  Get_sms_content(int nSMS );
	void  set_sim_sms( unsigned n_param )   { m_sms.set_sim_sms(n_param);   }
	void  set_sim_pbook( unsigned n_param ) { m_sms.set_sim_pbook(n_param); }
	void  on_timer( unsigned n_id );



protected:
	void  on_test_gsm();
	void  on_test_memory();
	
	msg_sms   m_sms;
	msg_dial  m_dial;

protected:
	int   m_check_line;

};


#endif  // MSG_GSM_H
