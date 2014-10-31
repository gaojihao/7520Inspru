
/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gps\msg_mid.h

 $Reversion History:

        (1) Created   Liuhl@2009-01-21 09:53 AM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  MSG_MID_H
#define  MSG_MID_H

#include "msg_base.h"

#define  OFFSET_HEAD          0
#define  OFFSET_MSGLEN        1
#define  OFFSET_DEVICE_TYPE   3
#define  OFFSET_MSGID         4
#define  OFFSET_PAYLOAD       5
#define  OFFSET_SVR_ACK_RST   26 


class  msg_mid : public msg_base
{
public:
	msg_mid(char* sz_buf, int n_size );
	~msg_mid();

	virtual  bool  build_head();
	virtual  bool  build_tail();
	virtual  bool  build_payload(int msg_type, const char* p_msg, int n_len);
	virtual  bool  build_length();
	virtual  bool  build_msg();
protected:
	virtual  bool  calc_xor(){ return true;}
};



#endif //MSG_MID_H