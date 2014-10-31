#include "stdafx.h"
#include <windows.h>
#include "ex_basics.h"
#include "ex_debug.h"

void  ex_trace_scrn(int x, int y, char* _msg)
{
#ifdef _WIN32
		HDC hDC = GetDC(NULL);
		RECT  rtScreen;
		SetRectEmpty(&rtScreen);
		
		SetTextColor(hDC, 0x00ff0000);
		SetBkColor(hDC, 0x00FF00FF);

	
	#if defined(UNICODE) || defined(_UNICODE)
		wchar_t uni_msg[128];
		memset(uni_msg, 0, sizeof(uni_msg));
		ex_char2uni(_msg, uni_msg, Ex_MIDOF(uni_msg) );

		DrawTextW(hDC, uni_msg, wcslen(uni_msg) , &rtScreen, DT_CALCRECT );
		OffsetRect(&rtScreen, x, y );
		DrawTextW(hDC, uni_msg, wcslen(uni_msg) , &rtScreen, DT_LEFT|DT_TOP );	

	#else
		DrawTextA(hDC, _msg, strlen(_msg) , &rtScreen, DT_CALCRECT );
		OffsetRect(&rtScreen, x, y );
		DrawTextA(hDC, _msg, strlen(_msg) , &rtScreen, DT_LEFT|DT_TOP );	
	#endif

	ReleaseDC(NULL, hDC);
#endif
}

void  ex_trace_msg(char* msg)
{

#ifdef _WIN32
		
	#if defined(UNICODE) || defined(_UNICODE)
		wchar_t uni_msg[128];
		memset(uni_msg, 0, sizeof(uni_msg));
		ex_char2uni(msg, uni_msg, Ex_MIDOF(uni_msg) );
		OutputDebugStringW( uni_msg );
	#else
		OutputDebugStringA( msg );
	#endif

#endif

}