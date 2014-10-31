/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\msg_dial.h

 $Reversion History:

        (1) Created Liuhl@2008-03-10 01:29 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  MSG_DIAL_H
#define  MSG_DIAL_H

#include "msg_ipc.h"

class msg_dial
{
public:
	msg_dial();
	~msg_dial();

	enum { gsm_none = 0, gsm_incoming, gsm_calling };
	enum { max_call_num = 64 , max_ext_num = 64 };

	void  set_ipc(msg_ipc* p_ipc)   { m_ipc = p_ipc; }
	void  on_message(unsigned n_type, unsigned n_param);
	void  set_phone_num(char* p_num, int n_len);
	void  set_phone_ext(char* p_num, int n_len);

protected:
	void cmd_gsm_set_vol(unsigned n_level);
	void cmd_gsm_dial();
	void cmd_dial_listen();
	void cmd_gsm_dial_ext();
	void cmd_gsm_hangup();
	void cmd_gsm_accepted();


protected:
	msg_ipc*   m_ipc;
	//char  m_phone_num[max_call_num];
	//char  m_phone_ext[max_ext_num];
};

#endif //MSG_DIAL_H