
/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\ex_gui\msg_ctrl.h

 $Reversion History:

        (1) Created Liuhl@2007-12-14 13:50

 $Notes:
		

--*/

#ifndef  MSG_CTRL_H
#define  MSG_CTRL_H


#define   PNDMAIN_CLS_NAME    "Dialog"
#define   PNDMAIN_WND_NAME    "HarrySMS1027"

#include "ex_basics.h"
//#include "msg_ipc.h"
#include "msg_gsm.h"
#include <windows.h>

class msg_ctrl
{
public:
	msg_ctrl();
	~msg_ctrl();

public:
	void  init_msg_ctrl( HWND h_wnd );
	bool  on_message( unsigned int u_msg, unsigned int w_param, long l_param );
	void  on_data_copy(unsigned int n_type, void* p_data, int n_len);
	void  on_timer( unsigned n_id );


protected:
	void  on_test_gsm();
	void  on_test_memory();
	
public:
	int        m_check_line;
	//msg_gsm    m_gsm;
};


#endif  // MSG_CTRL_H
