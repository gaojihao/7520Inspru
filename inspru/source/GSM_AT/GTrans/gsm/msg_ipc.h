/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\logic\msg_ipc.h

 $Reversion History:

        (1) Created Liuhl@2008-01-16 11:21

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  PROC_IPC_H
#define  PROC_IPC_H

#include "../GPRS/ex_basics.h"

#define  NAME_GSM_UI_CLASS       _T("Dialog")
#define  NAME_GSM_UI_TITLE       _T("WND_MAIN_MENU")




class msg_ipc
{
public:
	msg_ipc();
	~msg_ipc();

	bool   init_target( TCHAR* cls_name, TCHAR* win_name );
	void   set_onwer(HWND h_owner) { m_owner = h_owner; }
	void   post_msg(unsigned n_msg,unsigned int w_param, long l_param);
	void   send_msg(unsigned n_msg,unsigned int w_param, long l_param);
	void   past_data(unsigned n_type, const char* p_data, int n_len = -1 );
	void   past_big_data(unsigned n_type, const char* p_data, int n_len = -1 );
protected:
	HWND   m_hwnd_svr;
	HWND   m_owner;

};

#endif //PROC_IPC_H