
#ifndef   _MACRODEF_H
#define   _MACRODEF_H

/*++
  ====+ (C) Copyright 2006.  eCompass Corp.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $public\Macrodef.h

 $Abstract:
      
		This public header file specifies all structures, macro, global
		
	  variables and function prototypes ,which presents functions to 
	
  	 be used  in project EurpNav in the WINCE platform.

 $Reversion History:

         If this module does work properly, it is created by LiuHL, if not,

	  i don't know who writes these codes. 

        (1) LiuHL  Create file on 04/28/2006

 $Notes:
		
	    Increment this if a change has a global effects.
--*/





//
//  Defines Macros
//

#define   TIME_ID_START          ( 0x1000 )
#define   TIME_WATCHING_SYSTEM   (TIME_ID_START + 1)



//
//  Define Message
//
// The message was fully processed in PreTranslateMessage and 
// should not be processed further.
#define    RTN_PROCESS_END      TRUE
// The message should be processed in the normal way.
#define    RTN_PROCESS_CTU      FALSE


#define    MSG_EURP_START        ( WM_USER + 0x1000  )
#define    MSG_SYS_BEGIN         ( MSG_EURP_START + 1  )
// The initialization has performanced over.
#define		MSG_SYS_INITOVER		( MSG_SYS_BEGIN + 1 )
#define		MSG_LOCATION_DATA		( MSG_SYS_BEGIN + 2 ) // location message.
#define		WM_CAL_START			( MSG_SYS_BEGIN + 3 ) // start calculation message.
#define		WM_CAL_OVER				( MSG_SYS_BEGIN + 4 ) // end calculation message.
#define		MSG_SYS_END				( MSG_EURP_START + 99 )
#define		MSG_SYS_LOADMAPOVER		( MSG_SYS_BEGIN + 5 )  //FINISHED LOAD MAP,SEND THIS MESSAGE TO DIALOGUE OF "startup picpure"
#define		WM_CLOSE_DIFFTOWN		( MSG_SYS_BEGIN + 6 )


#define		WM_GETDISINFO	WM_USER + 2324
#define		WM_SELCHANGE	WM_USER + 2323

#define		WM_STORAGE_EVENT        	0x219//SD¿¨²å°ÎÏûÏ¢


//
//  define structures
//


#endif  //_MACRODEF_H