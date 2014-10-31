
#include "logic_data.h"
#include "../ExTool/ex_file.h"

logic_data::logic_data()
{
	m_list = 0;
	m_count = 0;
	m_max = 0;
	m_is_change = false;
	m_is_open = false;
}

logic_data::~logic_data()
{
	if ( m_list ) delete [] m_list;
	m_list = 0;
	m_count = 0;
	m_is_change = false;
	m_is_open = false;
}

//////////////////////////////////////////////////////////////////////////

bool  logic_data::load_data( const char* p_path )
{
	if ( m_list ) delete [] m_list;
	m_list = 0;
	m_count = 0;

	cex_file file;

	// 1 如果打开当前文件失败，则尝试打开备份文件 
	if ( !cex_file::is_exist( p_path) )
	{
		char back_path[256];
		sprintf(back_path,"%s.bak\0",p_path);
		// 如果备份文件存在，则拷贝备份文件到正式文件
		if ( cex_file::is_exist( back_path) )
		{
			cex_file::rename(back_path, p_path);
		}else
		{
			; // This is should not be happened.
		}
	}
	
	if ( !file.open( p_path, cex_file::m_read ) )
		return false;

	m_count = file.size() / sizeof(st_sms) ;
	m_max = m_count + reserve_number;
	m_list = new st_sms[ m_max ];
	if ( m_list == 0 ) return false;

	file.read( m_list, file.size() );
	file.close();

	m_is_open = true;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool   logic_data::save_data( const char* p_path )
{	
	if ( p_path == 0 ) return false;

	if ( m_is_change )
	{
		char back_path[256];
		// 1 删除备份文件
		sprintf(back_path,"%s.bak\0",p_path);
		cex_file::remove( back_path );
		
		// 2 当前内容写人备份文件
		cex_file file;
		if ( !file.open( back_path, cex_file::m_readwrite) )
			return false;
		file.write( m_list, m_count*sizeof(st_sms) );
		file.close();
		
		// 3 删除当前文件
		cex_file::remove( p_path );
		
		// 4 将备份文件改名为当前文件
		cex_file::rename(back_path, p_path);
	}

	if ( m_list ) delete [] m_list;
	m_list = 0;
	m_count = 0;
	m_is_open = false;
	m_is_change = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool   logic_data::delte_record( int n_idx )
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		if ( n_idx < m_count-1 )
		{
			for( int x = n_idx; x < m_count-1; x++ )
			{
				memcpy( &m_list[x], &m_list[x+1], sizeof(st_sms) );
			}
		}
		m_list[m_count-1].m_type = 0;
		m_count--;
		m_is_change = true;
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////

bool   logic_data::add_recourd(st_sms* p_msg )
{
	if ( p_msg == 0 ) return false;
	if ( m_count >= m_max ) return false;
	memcpy(&m_list[m_count++], p_msg, sizeof(st_sms) );
	m_is_change = true; 
	return true;
}

//////////////////////////////////////////////////////////////////////////

int  logic_data::max_number()
{
	return m_count;
}

//////////////////////////////////////////////////////////////////////////

st_sms*   logic_data::get_recourd( int n_idx )
{	
	if ( n_idx < m_count && n_idx >= 0 )
		return &m_list[n_idx];
	return 0;
}


//////////////////////////////////////////////////////////////////////////

bool  logic_data::read_sms(int n_idx)
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		m_list[n_idx].m_type = 1;
		//m_is_change = true;
		return true;
	}else
		return false;
}


//////////////////////////////////////////////////////////////////////////

bool  logic_data::add_2file(const char* p_path, st_sms* p_msg)
{
	if ( p_msg == 0 ) return false;

	//需要尝试打开备份文件吗？
	
	cex_file file;
	if ( !file.open( p_path, cex_file::m_write ) )
		return false;

	file.seek2end();
	file.write( p_msg, sizeof(st_sms) );
	file.close();
	return true;
}





















//////////////////////////////////////////////////////////////////////////


logic_pbook::logic_pbook()
{
	m_list = 0;
	m_count = 0;
	m_max = 0;
	m_is_change = false;
	m_is_open = false;
}

logic_pbook::~logic_pbook()
{
	if ( m_list ) delete [] m_list;
	m_list = 0;
	m_count = 0;
	m_is_change = false;
	m_is_open = false;
}

//////////////////////////////////////////////////////////////////////////

bool  logic_pbook::load_data( const char* p_path )
{
	if ( m_list ) delete [] m_list;
	m_list = 0;
	m_count = 0;

	cex_file file;

	// 1 如果打开当前文件失败，则尝试打开备份文件 
	if ( !cex_file::is_exist( p_path) )
	{
		char back_path[256];
		sprintf(back_path,"%s.bak\0",p_path);
		// 如果备份文件存在，则拷贝备份文件到正式文件
		if ( cex_file::is_exist( back_path) )
		{
			cex_file::rename(back_path, p_path);
		}else
		{
			; // This is should not be happened.
		}
	}
	
	if ( !file.open( p_path, cex_file::m_read ) )
		return false;

	m_count = file.size() / sizeof(PBOOK_ITEM) ;
	m_max = m_count + reserve_number;
	m_list = new PBOOK_ITEM[ m_max ];
	if ( m_list == 0 ) return false;

	file.read( m_list, file.size() );
	file.close();

	m_is_open = true;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool   logic_pbook::save_data( const char* p_path )
{	
	if ( p_path == 0 ) return false;

	if ( m_is_change )
	{
		char back_path[256];
		// 1 删除备份文件
		sprintf(back_path,"%s.bak\0",p_path);
		cex_file::remove( back_path );
		
		// 2 当前内容写人备份文件
		cex_file file;
		if ( !file.open( back_path, cex_file::m_readwrite) )
			return false;
		file.write( m_list, m_count*sizeof(PBOOK_ITEM) );
		file.close();
		
		// 3 删除当前文件
		cex_file::remove( p_path );
		
		// 4 将备份文件改名为当前文件
		cex_file::rename(back_path, p_path);
	}

	if ( m_list ) delete [] m_list;
	m_list = 0;
	m_count = 0;
	m_is_open = false;
	m_is_change = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool   logic_pbook::delte_record( int n_idx )
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		if ( n_idx < m_count-1 )
		{
			for( int x = n_idx; x < m_count-1; x++ )
			{
				memcpy( &m_list[x], &m_list[x+1], sizeof(PBOOK_ITEM) );
			}
		}
		m_list[m_count-1].state = 0;
		m_count--;
		m_is_change = true;
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////

bool   logic_pbook::add_recourd(PBOOK_ITEM* p_msg )
{
	if ( p_msg == 0 ) return false;
	if ( m_count >= m_max ) return false;
	memcpy(&m_list[m_count++], p_msg, sizeof(PBOOK_ITEM) );
	m_is_change = true; 
	return true;
}

//////////////////////////////////////////////////////////////////////////

int  logic_pbook::max_number()
{
	return m_count;
}

//////////////////////////////////////////////////////////////////////////

PBOOK_ITEM*   logic_pbook::get_recourd( int n_idx )
{	
	if ( n_idx < m_count && n_idx >= 0 )
		return &m_list[n_idx];
	return 0;
}

//////////////////////////////////////////////////////////////////////////

PBOOK_ITEM* logic_pbook::sch_item(const char* p_phone)
{
	for( int i = 0; i < m_count; i++ )
	{
		if ( strcmp(m_list[i].phone, p_phone) == 0 )
			return &m_list[i];
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

bool  logic_pbook::add_2file(const char* p_path, PBOOK_ITEM* p_msg)
{
	if ( p_msg == 0 ) return false;

	//需要尝试打开备份文件吗？
	
	cex_file file;
	if ( !file.open( p_path, cex_file::m_write ) )
		return false;

	file.seek2end();
	file.write( p_msg, sizeof(st_sms) );
	file.close();
	return true;
}
