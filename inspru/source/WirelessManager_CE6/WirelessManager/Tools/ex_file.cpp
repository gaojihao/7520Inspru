
#include "StdAfx.h"
#include "ex_file.h"
#include "ex_debug.h"

//////////////////////////////////////////////////////////////////////////

bool cex_file::rename(const char* old_name, const char* new_name )
{
#if defined(UNICODE) || defined(_UNICODE)
	wchar_t sz_old[Ex_MAXPATH];
	wchar_t sz_new[Ex_MAXPATH];
	memset(sz_old, 0, sizeof(sz_old));
	memset(sz_new, 0, sizeof(sz_new));

	ex_char2uni(old_name, sz_old, Ex_MIDOF(sz_old) );
	ex_char2uni(new_name, sz_new, Ex_MIDOF(sz_new) );
	if ( ::MoveFileW( sz_old, sz_new ) )
		return true;
	else
		return false;	
#else
	if ( ::MoveFileA( old_name, new_name ) )
		return true;
	else
		return false;	
#endif

}

//////////////////////////////////////////////////////////////////////////
bool cex_file::remove(const char* file_name)
{
#if defined(UNICODE) || defined(_UNICODE)
	wchar_t sz_name[Ex_MAXPATH];
	memset(sz_name, 0, sizeof(sz_name));
	ex_char2uni(file_name, sz_name, Ex_MIDOF(sz_name) );
	if ( ::DeleteFileW( sz_name ) )
		return true;
	else
		return false;
#else
	if ( ::DeleteFileA( file_name ) )
		return true;
	else
		return false;
#endif
	
}

//////////////////////////////////////////////////////////////////////////

bool cex_file::copyfile(const char* src_file, const char* new_file, bool is_overwrite)
{
#if defined(UNICODE) || defined(_UNICODE)
	wchar_t sz_old[Ex_MAXPATH];
	wchar_t sz_new[Ex_MAXPATH];

	ex_char2uni(src_file, sz_old, Ex_MIDOF(sz_old) );
	ex_char2uni(new_file, sz_new, Ex_MIDOF(sz_new) );
	if ( ::CopyFileW(sz_old, sz_new, is_overwrite) )
		return true;
	else
		return false;
	
#else
	if ( ::CopyFileA(src_file, new_file, is_overwrite) )
		return true;
	else
		return false;
#endif
}

//////////////////////////////////////////////////////////////////////////

bool cex_file::is_exist(const char* file_path )
{
	bool b_exist = false;

#if defined(UNICODE) || defined(_UNICODE)
	wchar_t sz_uni[Ex_MAXPATH];
	memset(sz_uni, 0, sizeof(sz_uni));
	ex_char2uni(file_path, sz_uni, Ex_MIDOF(sz_uni) );
	HANDLE tmp_file = ::CreateFile( sz_uni, GENERIC_READ, FILE_SHARE_READ,0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
#else
	HANDLE tmp_file = ::CreateFile( file_path, GENERIC_READ, FILE_SHARE_READ,0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
#endif
	if ( tmp_file != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( tmp_file );
		b_exist = true;
	}
	return b_exist;
}


//////////////////////////////////////////////////////////////////////////

bool cex_file::create( const char* file_path )
{
#if defined(UNICODE) || defined(_UNICODE)
	wchar_t sz_name[Ex_MAXPATH];
	memset(sz_name, 0, sizeof(sz_name));
	ex_char2uni(file_path, sz_name, Ex_MIDOF(sz_name) );
	HANDLE hfile = ::CreateFile( sz_name, GENERIC_READ, FILE_SHARE_READ,0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if (  hfile != INVALID_HANDLE_VALUE  )
	{
		return (::CloseHandle( hfile ) != 0);
	}
#else
	HANDLE hfile = ::CreateFile( file_path, GENERIC_READ, FILE_SHARE_READ,0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if ( hfile != INVALID_HANDLE_VALUE )
	{
		if ( ::CloseHandle( hfile ) )
			return true;
		else
			return false;
	}else
	{
		return false;
	}
#endif
	return false;
}

//////////////////////////////////////////////////////////////////////////

cex_file::cex_file()
{
	m_hfile = (unsigned int)cex_file::file_null;
}

cex_file::~cex_file()
{
	close();
}

bool  cex_file::open(const char* file_path, unsigned flag )
{
	if ( m_hfile != (unsigned int)file_null )
		close();


	unsigned long dw_access = 0;
	DWORD dwAccess = 0;
	switch ( flag & 3 )
	{
	case m_read:
		dw_access = GENERIC_READ;
		break;
	case m_write:
		dw_access = GENERIC_WRITE;
		break;
	case m_readwrite:
		dw_access = GENERIC_READ|GENERIC_WRITE;
		break;
	default:
		;//EXDBG_TRACE0("ERR:: invalid share mode \n");
		break;
	}

#if defined(UNICODE) || defined(_UNICODE)
	wchar_t sz_uni[Ex_MAXPATH];
	memset(sz_uni, 0, sizeof(sz_uni));
	ex_char2uni(file_path, sz_uni, Ex_MIDOF(sz_uni) );
	m_hfile = (unsigned int)::CreateFile( sz_uni, dw_access, FILE_SHARE_READ,0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
#else
	m_hfile = (unsigned int)::CreateFile( file_path, dw_access, FILE_SHARE_READ,0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
#endif
	
	return ( m_hfile != (unsigned int)file_null);
}

//////////////////////////////////////////////////////////////////////////

unsigned   cex_file::read(void* p_buf, unsigned n_count )
{
	if ( m_hfile == (unsigned int)file_null || p_buf == 0)
	{
		//EXDBG_TRACE0("ERR:  cex_file::read has error:: m_hfile = null || p_buf = 0! \n");
		return false;
	}

	if ( n_count == 0 )
	{
		//EXDBG_TRACE0("ERR:  cex_file::read has error:: n_size = 0! \n");
		return false;
	}
	
	unsigned long dw_read = 0;
	if (::ReadFile((HANDLE)m_hfile, p_buf, n_count, &dw_read,0 ))
	{
		//EXDBG_TRACE1("OK :  read %d bytes from file \n",dw_read );		
		return dw_read;
	}
	else
	{
		//EXDBG_TRACE0("ERR:  read file failed! \n");		
		return 0;
	}
}

bool  cex_file::write( const void* p_buf, unsigned n_size )
{
	if ( m_hfile == (unsigned int)file_null || p_buf == 0)
	{
		//EXDBG_TRACE0(" cex_file::write has error:: m_hfile = null || p_buf = 0! \n");
		return false;
	}
	if ( n_size == 0 )
	{
		//EXDBG_TRACE0(" cex_file::write has error:: n_size = 0! \n");
		return false;
	}
	
	unsigned long dw_write = 0;
	if ( ::WriteFile( (HANDLE)m_hfile, p_buf, n_size, &dw_write,0 ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
	
long cex_file::seek(unsigned off_set, unsigned from )
{
	long dw_new = ::SetFilePointer( (HANDLE)m_hfile, off_set, 0, from );
	return dw_new;
}

long cex_file::seek2end()
{
	return seek( 0, cex_file::end );
}

long cex_file::seek2begin()
{
	return seek( 0, cex_file::begin );
}
//////////////////////////////////////////////////////////////////////////

void  cex_file::close()
{
	::CloseHandle( (HANDLE)m_hfile );
	m_hfile = file_null;
}

//////////////////////////////////////////////////////////////////////////

void  cex_file::flush()
{
	if (m_hfile == ( unsigned int )file_null)
		return;
	::FlushFileBuffers( (HANDLE)m_hfile );
}

//////////////////////////////////////////////////////////////////////////
unsigned   cex_file::size()
{
	long dw_len, dw_cur;
	dw_cur = seek(0, current);
	dw_len =  seek2end();
	if ( dw_cur != seek(dw_cur, cex_file::begin) )
	{
		EXDBG_TRACE0("ERR: cex_file::size has error. \n");
	}
	
	return dw_len;
}

