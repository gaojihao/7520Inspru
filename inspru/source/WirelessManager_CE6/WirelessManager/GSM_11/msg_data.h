
/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gps\msg_data.h

 $Reversion History:

        (1) Created   Liuhl@2009-04-01 19:32 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  MSG_DATA_H
#define  MSG_DATA_H

#include "ex_basics.h"

class  msg_data
{
public:
	static msg_data*  instance();
	~msg_data();


	void   set_ack_buf( char* p_buf, int n_buf_size);
	char*  ipd_data()   { return m_ipd_buf; }
	int    ipd_len()    { return m_ipd_len; }
	char*  ack_data()   { return m_ack_buf; }
	int    ack_len()    { return m_ack_len; }

	void   empty_ack()  { m_ack_buf[0] ='\0';}
	void   empty_ipd();
protected:
	msg_data();
	static msg_data*  _instance;

protected:
	char   m_ack_buf[256];
	char   m_ipd_buf[Ex_1KB * 2];
	int    m_ipd_len;
	int    m_ack_len;
};



#endif //MSG_MID_H