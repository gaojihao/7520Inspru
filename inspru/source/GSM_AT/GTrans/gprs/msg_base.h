/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gps\msg_base.h

 $Reversion History:

        (1) Created   Liuhl@2009-01-21 09:53 AM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  MSG_BASE_H
#define  MSG_BASE_H

#include <windows.h>

class  msg_base
{
public:
	enum { max_msg_len = 2048};
	msg_base(char* sz_buf, int n_size ) { m_data = (unsigned char*)sz_buf; m_max_len = n_size; m_msg_len = 0; }
	virtual  char* buf() { return (char*)m_data;}
	virtual  int   len() { return m_msg_len;}
	virtual  void  set_buf( char* p_buf, int n_siz) { m_data = (unsigned char*)p_buf; m_max_len = n_siz; }

	virtual  bool  build_head() = 0;
	virtual  bool  build_tail() = 0;
	virtual  bool  build_payload(int msg_type, const char* p_msg, int n_len) = 0;
	virtual  bool  build_length() = 0;
	virtual  bool  build_msg() = 0;
protected:
	virtual  bool  calc_xor() = 0;

protected:
	int  m_msg_len;
	int  m_max_len;
	unsigned char *m_data;
};



#endif //MSG_BASE_H