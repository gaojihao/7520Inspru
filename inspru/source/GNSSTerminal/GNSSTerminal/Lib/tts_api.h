
/*++
  ====+ (C) Copyright 2006.  Target Corp.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $tts_src\tts_voice.h

 $Abstract:
      
		This public header file defines a class for TTS.

 $Reversion History:

        (1) LiuHL  Create file on 12/28/2006

 $Notes:
		
	
--*/

#ifndef  _TTS_API_INC
#define  _TTS_API_INC

#include <windows.h>


#ifdef TTS_EXPORTS
#define TTS_API __declspec(dllexport)
#else
#define TTS_API __declspec(dllimport)
#endif


TTS_API HANDLE  tts_open();
TTS_API void    tts_close(HANDLE t_handle);
TTS_API bool    tts_init(HANDLE t_handle, const char* res_path);
TTS_API void    tts_speaking(HANDLE t_handle, const  unsigned short* str_voice, int n_len );
TTS_API void    tts_man_role(HANDLE t_handle, bool b_man);
TTS_API bool    tts_is_speaking(HANDLE t_handle);
TTS_API void    tts_cancle(HANDLE t_handle);


#endif //_TTS_API_INC