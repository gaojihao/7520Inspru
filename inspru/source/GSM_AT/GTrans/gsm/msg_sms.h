/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\msg_sms.h

 $Reversion History:

        (1) Created Liuhl@2008-03-10 01:29 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  MSG_SMS_H
#define  MSG_SMS_H

#include "gsm_msg_def.h"
#include "msg_ipc.h"
#include "../GPRS/gsm_api.h"

class msg_sms
{
public:
	msg_sms();
	~msg_sms();
	enum  { max_call_num = 64, max_sms_size = 141 };
	void   on_message(unsigned n_type, unsigned n_param);

	void   set_ipc(msg_ipc* p_ipc)  { m_ipc = p_ipc; }
	void   set_msg_buf(char* p_msg, int n_len  );
	void   set_phone_num(char* p_num, int n_len);
	void   add_sim_pbook(PBOOK_ITEM* p_item);
	void   cmd_gsm_read_sim_sms( int nIdx );
	void   cmd_gsm_get_sms_content( int nSMS );

	void  set_sim_sms( unsigned n_param )    { m_nSMSNum = n_param;   }
	void  set_sim_pbook( unsigned n_param )  { m_nPbookNum = n_param; }
protected:

	void   cmd_gsm_sendmsg();
	void   cmd_gsm_del_sim_sms(int nIdx);
	void   cmd_gsm_pbook_read();
	void   cmd_gsm_pbook_del(int nIdx);
	void   cmd_gsm_pbook_write();
	
protected:
	msg_ipc*        m_ipc;
	UINT            m_nSMSNum;       // (LOWORD() = USED ,HIWORD() == MAX_NUM )
	UINT            m_nPbookNum;
	//char            m_phone_num[max_call_num];
	//char            m_msg_buf[max_sms_size];//79
	SM_PARAM        m_sms_context;
	PBOOK_ITEM      m_pbook_item;
	UINT			m_nSMSIndex;	

};


#endif //MSG_SMS_H