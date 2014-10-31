#include "stdafx.h"
#include <windows.h>
#include "ex_basics.h"
#include "ex_debug.h"
#include "TCHAR.h"
#include "ex_file.h"
//#include "srv_gps.h"

cex_file g_log;
void  ex_trace_scrn(int x, int y, char* _msg)
{
#ifdef _WIN32
		HDC hDC = GetDC(NULL);
		RECT  rtScreen;
		LOGFONT   lf;
		HFONT font, hOld;
		SetRectEmpty(&rtScreen);
		
		SetTextColor(hDC, 0x00ff0000);
		SetBkColor(hDC, 0x00FF00FF);


		lf.lfHeight=0;   
		lf.lfWidth=0;   
		lf.lfOrientation=0;   
		lf.lfItalic=FALSE;   
		lf.lfWeight=600;   
		lf.lfStrikeOut=FALSE;   
		lf.lfQuality=DRAFT_QUALITY;   
		lf.lfUnderline=FALSE;   
		lf.lfEscapement=0;   
		lf.lfCharSet=GB2312_CHARSET;   
		lf.lfOutPrecision=0;   
		lf.lfClipPrecision=CLIP_STROKE_PRECIS;   
		lf.lfPitchAndFamily=VARIABLE_PITCH|FF_MODERN;   
		_tcscpy(lf.lfFaceName,TEXT("ËÎÌו_GB2312") );   

		font = CreateFontIndirect(&lf);
		hOld = (HFONT)SelectObject(hDC, font );

	#if defined(UNICODE) || defined(_UNICODE)
		TCHAR uni_msg[128];
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

		SelectObject(hDC, hOld );
		::DeleteObject( font );
	ReleaseDC(NULL, hDC);
#endif
}

void  ex_trace_msg(char* msg)
{
	//ex_trace_scrn(50,1,msg);
#if 0
	if ( !g_log.is_open() )
	{
		char* p_path = "\\Flashdrv Storage\\gsm_log.txt";
		if ( !cex_file::is_exist( p_path ) )
			cex_file::create( p_path );
		g_log.open( p_path, cex_file::m_write );
		g_log.seek2end();
	}else
	{
		g_log.write(msg, strlen(msg) );
	}
#endif

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

//////////////////////////////////////////////////////////////////////////
void      ex_trace_time()
{
	//static n_count = 0;
	SYSTEMTIME sys_time;
	::GetLocalTime( &sys_time );
	char sz_buf[128];

//	sprintf( sz_buf,"(%d-%d)%02d:%02d:%02d\0",
//		++n_count, gps_info()->loc_state, sys_time.wHour,sys_time.wMinute, sys_time.wSecond );
	ex_trace_msg( sz_buf);
}