#include "stdafx.h"
#include <windows.h>
#include "ex_basics.h"

rect_i::rect_i() : 
	  m_x1(0), m_y1(0), m_x2(0), m_y2(0) {};

rect_i::rect_i(int x1_, int y1_, int x2_, int y2_) : 
	      m_x1(x1_), m_y1(y1_), m_x2(x2_), m_y2(y2_) {};

void  rect_i::init(int x1_, int y1_, int x2_, int y2_)
{
	m_x1 = x1_;
	m_y1 = y1_;
	m_x2 = x2_; 
	m_y2 = y2_; 
}

void   rect_i::init(const rect_i& r_ )
{
	m_x1 = r_.m_x1; 
	m_y1 = r_.m_y1; 
	m_x2 = r_.m_x2; 
	m_y2 = r_.m_y2; 
}

bool  rect_i::clip(const rect_i& r_ )
{
	if(m_x2 > r_.m_x2) m_x2 = r_.m_x2;
	if(m_y2 > r_.m_y2) m_y2 = r_.m_y2;
	if(m_x1 < r_.m_x1) m_x1 = r_.m_x1;
	if(m_y1 < r_.m_y1) m_y1 = r_.m_y1;
	return m_x1 <= m_x2 && m_y1 <= m_y2;
}

const rect_i& rect_i::normalize()
{
	int t;
	if(m_x1 > m_x2) { t = m_x1; m_x1 = m_x2; m_x2 = t; }
	if(m_y1 > m_y2) { t = m_y1; m_y1 = m_y2; m_y2 = t; }
	return *this;
}

void  rect_i::offset(int x, int y)
{
	m_x1 -= x;
	m_x2 -= x;
	m_y1 -= y;
	m_y2 -= y;
}

//////////////////////////////////////////////////////////////////////////
rect_i intersect_rectangles(const rect_i& r1, const rect_i& r2)
{
	rect_i r = r1;
	
	// First process x2,y2 because the other order 
	// results in Internal Compiler Error under 
	// Microsoft Visual C++ .NET 2003 69462-335-0000007-18038 in 
	// case of "Maximize Speed" optimization option.
	//-----------------
	if(r.m_x2 > r2.m_x2) r.m_x2 = r2.m_x2; 
	if(r.m_y2 > r2.m_y2) r.m_y2 = r2.m_y2;
	if(r.m_x1 < r2.m_x1) r.m_x1 = r2.m_x1;
	if(r.m_y1 < r2.m_y1) r.m_y1 = r2.m_y1;
	
	return r;
}

//////////////////////////////////////////////////////////////////////////

rect_i unite_rectangles(const rect_i& r1, const rect_i& r2)
{
	rect_i r = r1;
	if(r.m_x2 < r2.m_x2) r.m_x2 = r2.m_x2;
	if(r.m_y2 < r2.m_y2) r.m_y2 = r2.m_y2;
	if(r.m_x1 > r2.m_x1) r.m_x1 = r2.m_x1;
	if(r.m_y1 > r2.m_y1) r.m_y1 = r2.m_y1;
	return r;
}



void    ex_char2uni(LPCSTR sz_src, LPWSTR sz_rst, unsigned rst_size )
{
	//MultiByteToWideChar(935,MB_PRECOMPOSED, szSrc, -1, szRst, nRstSize );
	int rst = MultiByteToWideChar(CP_ACP,NULL, sz_src, -1, sz_rst, rst_size );
	if ( rst == 0 )
		rst = MultiByteToWideChar(936,NULL, sz_src, -1, sz_rst, rst_size );
}


void    ex_uni2char(LPCWSTR sz_src, LPSTR sz_rst, unsigned rst_size )
{
	int rst = WideCharToMultiByte(CP_ACP,NULL, sz_src, -1, sz_rst, rst_size, NULL,FALSE );
	if ( rst == 0 )
		 rst = WideCharToMultiByte(936,NULL, sz_src, -1, sz_rst, rst_size, NULL,FALSE );
}

unsigned int  ex_scrn_size()
{
	unsigned  x,y;
#ifdef UNDER_CE
	x = ::GetSystemMetrics( SM_CXSCREEN );
	y = ::GetSystemMetrics( SM_CYSCREEN );
#else
	x = 480;
	y = 272;
#endif
	return unsigned int( x | y<<16 );
}


sec_lock::sec_lock()
{ 
	InitializeCriticalSection(&m_cs);
}
sec_lock::~sec_lock()
{
	DeleteCriticalSection( &m_cs );
}
void sec_lock::enter()
{ 
	return;
	EnterCriticalSection( &m_cs );
}
void sec_lock::leave()
{ 
	return;
	LeaveCriticalSection( &m_cs );
}



static  LARGE_INTEGER g_freq;
static  LARGE_INTEGER g_start;
void    ex_start_timer()
{
	if ( g_freq.QuadPart == 0 )
	{
		::QueryPerformanceFrequency( &g_freq );
		::QueryPerformanceCounter( &g_start );
	}
	::QueryPerformanceCounter( &g_start );
}

double  ex_elapsed_time()
{
	LARGE_INTEGER stop;
    ::QueryPerformanceCounter(&stop);
    return double(stop.QuadPart - 
                      g_start.QuadPart) * 1000.0 / 
                      double(g_freq.QuadPart);
}