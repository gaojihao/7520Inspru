/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\msg\msg_gsm_ack.h

 $Reversion History:

        (1) Created Liuhl@2008-03-10 01:29 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  MSG_GSM_ACK_H
#define  MSG_GSM_ACK_H

#include "gsm_ack_interface.h"
#include "msg_ipc.h"

class msg_gsm_ack : public gsm_ack
{
public:
	msg_gsm_ack();
	~msg_gsm_ack();

public:
	//新的短消息来后的ACK
	bool  ack_new_msg( const void* p_param );
	bool  ack_new_call(const void* p_param );
	bool  ack_disconnect(int nReason );
	bool  ack_connect(const char* p_num);
	void  ack_gsm_signal(int n_level);
	void  ack_gsm_net(const char* p_num);

	void   set_ipc( msg_ipc* p_ipc )    { m_ipc = p_ipc; }
	void   set_calling( bool b_call )   { m_is_calling = b_call; }
	bool   is_calling()                 { return m_is_calling;   }   
protected:
	char      m_call_num[32];
	bool      m_is_calling;
	msg_ipc*  m_ipc;
};



#endif //MSG_GSM_ACK_H