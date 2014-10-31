
#include "diaodu_data.h"

CSMSCenter_data CSMSCenter_data::m_instance;
bool CSMSCenter_data::m_is_open = false;
const char* CSMSCenter_data::m_path = PATH_SMS_CENTER;
CSMSCenter_data* CSMSCenter_data::Instance()
{
	return &m_instance;
}

CSMSCenter_data::CSMSCenter_data()
{
	m_list = 0;
	m_count = 0;
	m_max = 0;
	m_is_change = false;
}

CSMSCenter_data::~CSMSCenter_data()
{
	if ( m_list ) 
		delete [] m_list;
	m_list = 0;
	m_count = 0;
	m_is_change = false;
	m_is_open = false;
}

bool  CSMSCenter_data::load_data()
{
	if ( m_list ) 
		delete [] m_list;
	m_list = 0;
	m_count = 0;

	cex_file file;

	// 1 如果打开当前文件失败，则尝试打开备份文件 
	if ( !cex_file::is_exist( m_path) )
	{
		char back_path[256];
		sprintf(back_path,"%s.bak\0",m_path);
		// 如果备份文件存在，则拷贝备份文件到正式文件
		if ( cex_file::is_exist( back_path) )
		{
			cex_file::rename(back_path, m_path);
		}
		else
		{
			; // This is should not be happened.
		}
	}
	
	if ( !file.open( m_path, cex_file::m_read ) )
		return false;

	m_count = file.size() / sizeof(st_SMSCenter) ;
	m_max = m_count + reserve_number;
	m_list = new st_SMSCenter[ m_max ];
	if ( m_list == 0 ) 
		return false;

	file.read( m_list, file.size() );
	file.close();

	m_is_open = true;
	return true;
}

bool   CSMSCenter_data::save_data(bool bClear)
{
	if ( m_path == 0 ) return false;

	if ( m_is_change )
	{
		char back_path[256];
		// 1 删除备份文件
		sprintf(back_path,"%s.bak\0", m_path);
		cex_file::remove( back_path );
		
		// 2 当前内容写人备份文件
		cex_file file;
		if ( !file.open( back_path, cex_file::m_readwrite) )
			return false;
		file.write( m_list, m_count*sizeof(st_SMSCenter) );
		file.close();
		
		// 3 删除当前文件
		cex_file::remove( m_path );
		
		// 4 将备份文件改名为当前文件
		cex_file::rename(back_path, m_path);
	}

	//需要将内存清空
	if(bClear)
	{
		if ( m_list ) 
			delete [] m_list;
		m_list = 0;
		m_count = 0;
		m_is_open = false;
	}
	m_is_change = false;
	return true;
}

bool   CSMSCenter_data::delete_record( int n_idx )
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		if ( n_idx < m_count-1 )
		{
			for( int x = n_idx; x < m_count-1; x++ )
			{
				memcpy( &m_list[x], &m_list[x+1], sizeof(st_SMSCenter) );
			}
		}
		//m_list[m_count-1].m_type = 0;
		m_count--;
		m_is_change = true;
		return true;
	}
	return false;
}

bool   CSMSCenter_data::add_recourd(st_SMSCenter* p_msg )
{
	if ( p_msg == 0 ) return false;
	if ( m_count >= m_max ) return false;
	memcpy(&m_list[m_count++], p_msg, sizeof(st_SMSCenter) );
	m_is_change = true; 
	return true;
}

int  CSMSCenter_data::max_number()
{
	return m_count;
}

st_SMSCenter*   CSMSCenter_data::get_recourd( int n_idx )
{
	if ( n_idx < m_count && n_idx >= 0 )
		return &m_list[n_idx];
	return 0;
}

bool  CSMSCenter_data::read_sms(int n_idx)
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		//m_list[n_idx].m_type = 1;
		//m_is_change = true;
		return true;
	}
	else
		return false;
}

bool  CSMSCenter_data::add_2file_end(st_SMSCenter* p_msg)
{
	if ( p_msg == 0 )
		return false;
	if( m_is_open )
		return false;

	cex_file file;
	if ( !file.open( m_path, cex_file::m_write ) )
		return false;

	file.seek2end();
	file.write( p_msg, sizeof(st_SMSCenter) );
	file.close();
	return true;
}



//////////////////////////////////////////////////////////////////////////

CSMSTask_data CSMSTask_data::m_instance;
bool CSMSTask_data::m_is_open = false;
const char* CSMSTask_data::m_path = PATH_SMS_TASK_DING;
CSMSTask_data* CSMSTask_data::Instance()
{
	return &m_instance;
}

CSMSTask_data::CSMSTask_data()
{
	m_list = 0;
	m_count = 0;
	m_max = 0;
	m_is_change = false;
}

CSMSTask_data::~CSMSTask_data()
{
	if ( m_list ) 
		delete [] m_list;
	m_list = 0;
	m_count = 0;
	m_is_change = false;
	m_is_open = false;
}

bool  CSMSTask_data::load_data()
{
	if ( m_list ) 
		delete [] m_list;
	m_list = 0;
	m_count = 0;

	cex_file file;

	// 1 如果打开当前文件失败，则尝试打开备份文件 
	if ( !cex_file::is_exist( m_path) )
	{
		char back_path[256];
		sprintf(back_path,"%s.bak\0",m_path);
		// 如果备份文件存在，则拷贝备份文件到正式文件
		if ( cex_file::is_exist( back_path) )
		{
			cex_file::rename(back_path, m_path);
		}
		else
		{
			; // This is should not be happened.
		}
	}
	
	if ( !file.open( m_path, cex_file::m_read ) )
		return false;

	m_count = file.size() / sizeof(st_SMSTask) ;
	m_max = m_count + reserve_number;
	m_list = new st_SMSTask[ m_max ];
	if ( m_list == 0 ) 
		return false;

	file.read( m_list, file.size() );
	file.close();

	m_is_open = true;
	return true;
}

bool   CSMSTask_data::save_data(bool bClear)
{
	if ( m_path == 0 ) return false;

	if ( m_is_change )
	{
		char back_path[256];
		// 1 删除备份文件
		sprintf(back_path,"%s.bak\0", m_path);
		cex_file::remove( back_path );
		
		// 2 当前内容写人备份文件
		cex_file file;
		if ( !file.open( back_path, cex_file::m_readwrite) )
			return false;
		file.write( m_list, m_count*sizeof(st_SMSTask) );
		file.close();
		
		// 3 删除当前文件
		cex_file::remove( m_path );
		
		// 4 将备份文件改名为当前文件
		cex_file::rename(back_path, m_path);
	}
	if(bClear)
	{
		if ( m_list ) 
			delete [] m_list;
		m_list = 0;
		m_count = 0;
		m_is_open = false;
	}
	m_is_change = false;
	return true;
}

bool   CSMSTask_data::delete_record( int n_idx )
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		if ( n_idx < m_count-1 )
		{
			for( int x = n_idx; x < m_count-1; x++ )
			{
				memcpy( &m_list[x], &m_list[x+1], sizeof(st_SMSTask) );
			}
		}
		//m_list[m_count-1].m_type = 0;
		m_count--;
		m_is_change = true;
		return true;
	}
	return false;
}

//未开始则取消，执行中则终止
bool CSMSTask_data::cancel_task(char* pTaskID, int nLen)
{
	int i;
	//文件已打开
	if( m_is_open )
	{
		//修改
		for(i=0; i<m_count; i++)
		{
			if( memcmp(m_list[i].m_taskID, pTaskID, nLen ) == 0 )
			{
				//未开始则取消，执行中则终止
				//状态：0未开始   1执行中    2已完成    3取消     4终止
				if( 0 == m_list[i].m_status )
				{
					m_list[i].m_status = 3;
				}
				else if( 1 == m_list[i].m_status )
				{
					m_list[i].m_status = 4;
				}
				m_is_change = true;
			}
		}
		//文件状态为已打开,保存但不清空内存
		save_data(false);
	}
	//未打开，打开->修改->保存
	else
	{
		//打开
		if( !load_data() )
			return false;
		//修改
		for(i=0; i<m_count; i++)
		{
			if( memcmp(m_list[i].m_taskID, pTaskID, nLen ) == 0 )
			{
				//未开始则取消，执行中则终止
				//状态：0未开始   1执行中    2已完成    3取消     4终止
				if( 0 == m_list[i].m_status )
				{
					m_list[i].m_status = 3;
				}
				else if( 1 == m_list[i].m_status )
				{
					m_list[i].m_status = 4;
				}
				m_is_change = true;
			}
		}
		//保存并清空内存
		save_data(true);
	}

	return true;
}

bool   CSMSTask_data::add_recourd(st_SMSTask* p_msg )
{
	if ( p_msg == 0 ) return false;
	if ( m_count >= m_max ) return false;
	memcpy(&m_list[m_count++], p_msg, sizeof(st_SMSTask) );
	m_is_change = true; 
	return true;
}

int  CSMSTask_data::max_number()
{
	return m_count;
}

st_SMSTask* CSMSTask_data::get_recourd( int n_idx )
{
	if ( n_idx < m_count && n_idx >= 0 )
		return &m_list[n_idx];
	return 0;
}

bool  CSMSTask_data::read_sms(int n_idx)
{
	if ( n_idx < m_count && n_idx >= 0 )
	{
		//m_list[n_idx].m_type = 1;
		//m_is_change = true;
		return true;
	}
	else
		return false;
}

bool  CSMSTask_data::add_2file_end(st_SMSTask* p_msg)
{
	if ( p_msg == 0 )
		return false;
	if( m_is_open )
		return false;

	cex_file file;
	if ( !file.open( m_path, cex_file::m_write ) )
		return false;

	file.seek2end();
	file.write( p_msg, sizeof(st_SMSTask) );
	file.close();
	return true;
}
