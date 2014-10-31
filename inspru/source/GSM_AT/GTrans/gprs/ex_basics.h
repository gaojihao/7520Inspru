/*++
  ====+ (C) Copyright 2007-2008.  AnyCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $Ex\inc\ex_basics.h

 $Abstract:
      

 $Reversion History:

        (1) created Liuhl@ 13:50 2007-12-10

 $Notes:
		
	    Increment this if a change has a global effects.
--*/

#ifndef  EX_BASICS_H
#define  EX_BASICS_H

#include "ex_config.h"

#ifdef EXOS_WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdarg.h>

//// ****** some fixes for MSVC++ ******
#if defined(_MSC_VER)
#pragma warning(disable : 4786)      // identifier was truncated...
#endif


// define macros for all projects

// define all const variables for all project
#define Ex_PI                (3.1415926535)
#define Ex_PIPI              (180.0)
#define Ex_1KB               (1024)
#define Ex_1MB               (1024*1024)
#define Ex_MAXPATH           (64)


#ifndef NULL
	#define NULL             ((void*)0)
#endif //NULL

#ifndef TRUE
	#define TURE             1
#endif //TRUE

#ifndef FALSE
	#define FALSE            0
#endif // FALSE


// Determine number of elements in an array (not bytes)
#define Ex_MIDOF(array)             (sizeof(array)/sizeof(array[0]))
#define Ex_MID(low,num,high)        (((low) <= (num)) && ((num) <= (high)))
#define Ex_MIN(x,y)                 (((x) < (y)) ? (x) : (y))
#define Ex_MAX(x,y)                 (((x) > (y)) ? (x) : (y))
#define Ex_ABS(x)                   (((x) >= 0) ? (x) : (-(x)))
#define Ex_IROUND(v)                (int(v))
#define Ex_UROUND(v)                (unsigned(v))
#define Ex_DEG2RAD(deg)             ( (deg) * Ex_PI / Ex_PIPI )
#define Ex_RAD2DEG(rad)             ( (rad) * Ex_PIPI / Ex_PI )
#define ExX_SCREEN(x)               ( x & 0xFFFF )
#define ExY_SCREEN(y)               ( y >> 16 )



// define the macros to declare variables, function, 
// function-ptr for other modules
#define Ex_AL_VAR(type, name)             extern type name
#define Ex_AL_ARRAY(type, name)           extern type name[]
#define Ex_AL_FUNC(type, name, args)      extern type name args
#define Ex_AL_METHOD(type, name, args)    type (*name) args
#define Ex_AL_FUNCPTR(type, name, args)   extern type (*name) args



// typedef all variable type
// redefine all 
#ifndef Ex_INT8
	#define Ex_INT8      signed char
#endif //Ex_INT8

#ifndef Ex_INT8U 
	#define Ex_INT8U     unsigned char
#endif //Ex_INT8U

#ifndef Ex_INT16
	#define Ex_INT16     signed short
#endif //Ex_INT16

#ifndef Ex_INT16U 
	#define Ex_INT16U    unsigned short
#endif //Ex_INT16U

#ifndef Ex_INT32
	#define Ex_INT32     signed int
#endif // Ex_INT32

#ifndef Ex_INT32U 
	#define Ex_INT32U    unsigned 
#endif // Ex_INT32U

#ifndef Ex_INT64
	#if defined(_MSC_VER)
		#define Ex_INT64 signed __int64
	#else
		#define Ex_INT64 signed long long
	#endif //defined(_MSC_VER)
#endif //Ex_INT64

#ifndef Ex_INT64U
	#if defined(_MSC_VER)
		#define Ex_INT64U unsigned __int64
	#else
		#define Ex_INT64U unsigned long long
	#endif //defined(_MSC_VER)
#endif // Ex_INT64U

typedef Ex_INT8    int8;
typedef Ex_INT8U   int8u;
typedef Ex_INT16   int16;
typedef Ex_INT16U  int16u;
typedef Ex_INT32   int32;
typedef Ex_INT32U  int32u;
typedef Ex_INT64   int64;
typedef Ex_INT64U  int64u;

#if defined(_MSC_VER)
	#define Ex_INLINE  __forceinline
#else 
	#define Ex_INLINE  inline
#endif //defined(_MSC_VER)


// define all basics structure and function
struct row_info8u
{
	int x1, x2;
	int8u* ptr;
	row_info8u() {}
	row_info8u(int x1_, int x2_, int8u* ptr_) : x1(x1_), x2(x2_), ptr(ptr_) {}
};

typedef struct const_row_info8u
{
	int x1, x2;
	const int8u* ptr;
	const_row_info8u() {}
	const_row_info8u(int x1_, int x2_, const int8u* ptr_) : 
	    x1(x1_), x2(x2_), ptr(ptr_) { }
}const_row_info;


typedef       row_info8u   row_info;
typedef const_row_info8u   const_row_info;



// define the structure of 'point_t'
typedef struct point_i
{
	int m_x,m_y;
	point_i();
	point_i(int x, int y);
	void offset(int x, int y);
}point_i;

// define the structure of 'rect_i'
typedef struct rect_i 
{
	rect_i();
	rect_i(int x1_, int y1_, int x2_, int y2_);
	void          init(int x1_, int y1_, int x2_, int y2_);
	void          init(const rect_i& r_ );
	void          offset(int x, int y);
    bool          clip(const rect_i& r_ );
	const rect_i& normalize();

	int        width()  const       { return (m_x2 - m_x1); };
	int        height() const       { return (m_y2 - m_y1); };
	point_i    left_top() const     { return point_i(m_x1, m_y1); };
	point_i    right_bottom() const { return point_i(m_x2, m_y2); };
	bool       is_valid() const     { return ( m_x1 <= m_x2 && m_y1 <= m_y2) ; };
	bool       hit_test(int x, int y) const { return (x >= m_x1 && x <= m_x2 && y >= m_y1 && y <= m_y2); };

	int m_x1;  // left
	int m_y1;  // top
	int m_x2;  // right
	int m_y2;  // bottom
}rect_i;

class sec_lock
{
private:
	CRITICAL_SECTION m_cs;
public:
	sec_lock();
	~sec_lock();
	void  enter();
	void  leave();
};


rect_i intersect_rectangles(const rect_i& r1, const rect_i& r2);
rect_i unite_rectangles(const rect_i& r1, const rect_i& r2);


// global function implemention
void          ex_char2uni(const char* sz_src, wchar_t* sz_rst, unsigned rst_size );
void          ex_uni2char(const wchar_t* sz_src, char* sz_rst, unsigned rst_size );
unsigned int  ex_scrn_size(); // see ExX_SCREEN, ExY_SCREEN macro
void          ex_start_time();
double        ex_elapsed_time();




#endif //EX_BASICS_H
