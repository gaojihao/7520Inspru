/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\rectrl_api.h

 $Reversion History:

        (1) Created Liuhl@2009-10-27 05:30 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  RECTRL_API_H
#define  RECTRL_API_H

#include "ex_basics.h"
#include "gsm_ctrl.h"
//////////////////////////////////////////////////////////////////////////

// GSM initialize function 

typedef struct _tagrectrl{
	unsigned char    dev_addr;
	unsigned char    dev_chanle;
	unsigned char    dev_state;  //00: 表示断开 01:表示闭合
}st_rectrl;
bool      rectrl_api_start(unsigned n_com, unsigned n_baud);
void      rectrl_api_close();
//下发是否授权可以用来打开车门
void      rectrl_api_enable(bool b_open_dev); 



#endif //RECTRL_API_H