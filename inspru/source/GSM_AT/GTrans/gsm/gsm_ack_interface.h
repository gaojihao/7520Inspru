/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gps\gsm_ack_interface.h

 $Reversion History:

        (1) Created   Liuhl@2008-11-10 11:09 AM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  GSM_ACK_INTERFACE_H
#define  GSM_ACK_INTERFACE_H

class  gsm_ack
{
public:
	//新的短消息来后的ACK
	bool virtual  ack_new_msg( const void* p_param ) = 0;
	bool virtual  ack_new_call(const void* p_param ) = 0;
	bool virtual  ack_disconnect( int nReason ) = 0;
	bool virtual  ack_connect(const char* p_num) = 0;
	void virtual  ack_gsm_signal(int n_level) = 0;
	void virtual  ack_gsm_net(const char* p_num) = 0;
};

#endif //GSM_ACK_INTERFACE_H