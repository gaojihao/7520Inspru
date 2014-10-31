/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\gsm_reset.h

 $Reversion History:

        (1) Created Liuhl@2009-03-23 10:45 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  GSM_RESET_H
#define  GSM_RESET_H

class gsm_module
{
public:
	static void		set_notify(HWND h_wnd, int n_msg );
	static void		reset_hd();
	static void		switch_voice2gsm();
 	static void		switch_voice2gps();
};

#endif //GSM_RESET_H