
/*++
  ====+ (C) Copyright 2006-2007.  AnyCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\cmn\cmn_debug.h

 $Abstract:
      
		The main debug function declare.

 $Reversion History:

        (1) Created by HarriLiu  on 21:23:00  01/30/2007 

 $Notes:
		
	    Increment this if a change has a global effects.
--*/

#ifndef  CMN_DEBUG_INC
#define  CMN_DEBUG_INC

#define EX_DEBUG

#ifdef __cplusplus
extern "C"{
#endif 

#include <stdio.h>
#include <stdarg.h>

void      ex_trace_msg(char* msg);
void      ex_trace_scrn(int x, int y, char* _msg);
void      ex_trace_time();

#ifdef  EX_DEBUG

	#define EXDBG_TRACE0(var)       \
{ \
		ex_trace_msg(var); \
} \

	#define EXDBG_TRACE1(var,s1)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(__sz_buf) ); \
		sprintf(__sz_buf, var, s1 ); \
		ex_trace_msg( __sz_buf ); \
} \

	#define EXDBG_TRACE2(var,s1,s2)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(__sz_buf) ); \
		sprintf(__sz_buf, var, s1,s2 ); \
		ex_trace_msg( __sz_buf ); \
} \

	#define EXDBG_TRACE3(var,s1,s2,s3)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(__sz_buf) ); \
		sprintf(__sz_buf, var, s1,s2,s3 ); \
		ex_trace_msg( __sz_buf ); \
} \

	#define EXDBG_TRACE4(var,s1,s2,s3,s4)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(__sz_buf) ); \
		sprintf(__sz_buf, var, s1,s2,s3,s4 ); \
		ex_trace_msg( __sz_buf ); \
} \

	#define EXDBG_TRACE5(var,s1,s2,s3,s4,s5)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(__sz_buf) ); \
		sprintf(__sz_buf, var, s1,s2,s3,s4,s5 ); \
		ex_trace_msg( __sz_buf ); \
} \

	#define EXDBG_TRACE6(var,s1,s2,s3,s4,s5,s6)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(sz_buf) ); \
		sprintf(__sz_buf, var, s1,s2,s3,s4,s5,s6); \
		ex_trace_msg( __sz_buf ); \
} \	

	#define EXDBG_TRACE7(var,s1,s2,s3,s4,s5,s6,s7)     \
{ \
		char  __sz_buf[2046];	\
		memset(&__sz_buf[0], 0, sizeof(__sz_buf) ); \
		sprintf(__sz_buf, var, s1,s2,s3,s4,s5,s6,s7); \
		ex_trace_msg( __sz_buf ); \
} \	

#else
	#define EXDBG_TRACE0(var)
	#define EXDBG_TRACE1(var,s1)
	#define EXDBG_TRACE2(var,s1,s2)
	#define EXDBG_TRACE3(var,s1,s2,s3)
	#define EXDBG_TRACE4(var,s1,s2,s3,s4)
	#define EXDBG_TRACE5(var,s1,s2,s3,s4,s5)
	#define EXDBG_TRACE6(var,s1,s2,s3,s4,s5,s6)
	#define EXDBG_TRACE7(var,s1,s2,s3,s4,s5,s6,s7)
      
#endif


#ifdef  EX_SCREEN
	#define EX_IN_SCREEN(x,y,_msg)  ex_trace_scrn(x,y,_msg)
#else
	#define EX_IN_SCREEN(x,y,_msg)
#endif



#ifdef __cplusplus
}
#endif 

#endif  //CMN_DEBUG_INC
