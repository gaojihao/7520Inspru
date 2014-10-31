
#include "StdAfx.h"
#include "gsm_ctrl.h"
#include "ex_debug.h"
#include "gsm_at.h"
#include "gsm_api.h"
#include "msg_data.h"
#include "../WirelessManager.h"
//extern void   ShowTip(CString strTip);
//连接状态
extern ConnectStatus	g_enConnectStatus;

gsm_ctrl::gsm_ctrl()
{
	memset(m_parse_buf, 0, sizeof(m_parse_buf) );
	memset(m_read_buf, 0, sizeof(m_read_buf) );
	m_read_count = 0;
//	m_parse_ptr = 0;
}

gsm_ctrl::~gsm_ctrl()
{
	memset(m_parse_buf, 0, sizeof(m_parse_buf) );
	memset(m_read_buf, 0, sizeof(m_read_buf) );
	m_read_count = 0;
	m_serial.close();
}

bool   gsm_ctrl::connect_gsm(int n_port, int n_baud)
{
	return m_serial.open(n_port, n_baud );
}

//////////////////////////////////////////////////////////////////////////

bool   gsm_ctrl::shutdown_gsm()
{
	return m_serial.close();
}

//////////////////////////////////////////////////////////////////////////

bool  gsm_ctrl::send_msg( const char* p_msg, unsigned n_size )
{	
	int i;
	TRACE0("\n********Send GSM data start:***************************\n");
	for(i=0; i<n_size; i++)
	{
		TRACE1("%c", p_msg[i]);
	}
	TRACE0("\n--------16进制数据-------------------------------------\n");
	for(i=0; i<n_size; i++)
	{
		TRACE1("%02X ", p_msg[i]);
		if( (i+1)%20 == 0 && (i+1) != n_size )
			TRACE0("\n");
	}
	TRACE0("\n********GSM data end***********************************\n");
	
	return m_serial.write_data( p_msg, n_size );
}

//////////////////////////////////////////////////////////////////////////

bool   gsm_ctrl::check_ack( const char* p_ack)
{
	bool b ;//= (strstr( m_ack_buf, p_ack ) != 0);
	b = ( strstr( msg_data::instance()->ack_data(), p_ack ) != 0 );
	return b;
}

char*  gsm_ctrl::ipd_ack()
{
	return  msg_data::instance()->ipd_data();
}


int    gsm_ctrl::ipd_len()
{
	return msg_data::instance()->ipd_len();

}


char*  gsm_ctrl::ack_buf() //     { return 0; }
{
	return  msg_data::instance()->ack_data();
}

int    gsm_ctrl::ack_len()//     { return m_read_count;}
{
	return msg_data::instance()->ack_len();
}
//////////////////////////////////////////////////////////////////////////

void   gsm_ctrl::empty_data(bool b_ack /*= true*/ )
{
	if ( b_ack )
	{
		msg_data::instance()->empty_ack();
	}else
	{
		;
	}
}
//////////////////////////////////////////////////////////////////////////

bool   gsm_ctrl::send_gsm_cmd( unsigned n_type )
{
	const char * p_cmd = gsm_api_cmd( n_type );
	if ( p_cmd )
		return send_msg( p_cmd, strlen(p_cmd) );
	else
		return false;
}

int gsm_ctrl::read_empy_from_gsm_com( )
{
	int i;
	int  offset = 0;
	int  n_read = 0;
	for(;;)
	{
		if ( offset >= max_buf )
		{
			EXDBG_TRACE0("receive buffer has overlap.....\n");
			break;
		}
		n_read = m_serial.read_data( &m_read_buf[offset], 2 );


		if ( n_read > 0 )
		{
			offset += n_read;
		}
		else
		{
			break;
		}
	}
	m_read_count = offset;
	if ( m_read_count > 0 )
	{
		TRACE0("\n########Receive GSM data start:########################\n");
		for(i=0; i<m_read_count; i++)
		{
			TRACE1("%c", m_read_buf[i]);
		}
		TRACE0("\n--------16进制数据-------------------------------------\n");
		for(i=0; i<m_read_count; i++)
		{
			TRACE1("%02X ", m_read_buf[i]);
			if( (i+1)%20 == 0 && (i+1) != m_read_count )
				TRACE0("\n");
		}
		TRACE0("\n########GSM data end###################################\n");
		
		memcpy( m_parse_buf, m_read_buf, m_read_count );
		m_parse_buf[m_read_count] = '\0';
		//ex_trace_msg( m_parse_buf );
		parse_data(); 
	}

	return offset;
}

void   gsm_ctrl::parse_data()
{
	//if ( m_parse_ptr )
	//	m_parse_ptr->parse( m_parse_buf, m_read_count );
	msg_data* p_data =  msg_data::instance();

	p_data->empty_ipd();

	//提取"+IPD"信息
	msg_data::instance()->set_ack_buf( m_parse_buf, m_read_count );

	//解析GSM-AT指令
	if ( p_data->ack_len() > 0 )
		parse_gsm_signal( p_data->ack_data() );
/*
	//解析GPRS数据，以"+IPD"开头
	if ( p_data->ipd_len() )
	{
		char* ptr = p_data->ipd_data();
		int n = parse_null( p_data->ipd_data(), p_data->ipd_len() );
		if ( n > 0 && n < p_data->ipd_len() )
			parse_null( ptr + n, p_data->ipd_len() - n );
	}*/
}

int gsm_ctrl::parse_gsm_signal(const char* p_buf)
{
	int n_signal = 0;
	int nSignaleThreshold = 4;
	int nCountThreshold = 5;
	static int	nLowSignalCount		= 0;	//信号连续在4以下的次数
	static int	nHighSignalCount	= 0;	//信号连续在4以下的次数
	static bool	bCurrConnectState	= true;	//true:	ras连接	false: ras断开
	char* p_ptr = strstr(p_buf, "\r\n+CSQ:");
	if ( p_ptr )
	{
		p_ptr += 7;
		if ( p_ptr )
		{
			n_signal = atoi( p_ptr );
			if ( n_signal == 99 ) 
				n_signal = 0;
			//msg_notify(eGSM_SIGNAL_VALUE,n_signal, true);
			//g_pstShareMem->nSignal = n_signal;
			TRACE1("Signal: %d\n", n_signal);

			{
				HWND hWnd = ::FindWindow(NULL, WND_NAME_WIRELESS_MANAGER);
				if(hWnd != NULL)
					::PostMessage(hWnd, WM_GSM_SIGNAL, n_signal, 0);
			}

			//信号控制ras连接模块
			if(n_signal < nSignaleThreshold)
				nLowSignalCount++;
			else
				nLowSignalCount = 0;

			if(g_enConnectStatus != CONNECT_STATUS_DISCONNECTED)
				bCurrConnectState = true;

			//信号持续低(5次小于4)，断开ras连接
			if(bCurrConnectState)//发过一次  断开后就没必要发送了
			{
				if(nLowSignalCount >= nCountThreshold)
				{
					HWND hWnd = ::FindWindow(NULL, WND_NAME_WIRELESS_MANAGER);
					if(hWnd != NULL)
					{
						::PostMessage(hWnd, MSG_RAS_CONNECT, 0, 0);
						bCurrConnectState = false;
					}
				}
			}

			if(!bCurrConnectState)
			{
				if(n_signal >= nSignaleThreshold)
					nHighSignalCount++;
				else
					nHighSignalCount = 0;
				
				//信号持续低->恢复连接(5次4以上)
				if(nHighSignalCount >= nCountThreshold)
				{
					HWND hWnd = ::FindWindow(NULL, WND_NAME_WIRELESS_MANAGER);
					if(hWnd != NULL)
					{
						::PostMessage(hWnd, MSG_RAS_CONNECT, 1, 0);
						bCurrConnectState = true;
					}
				}
			}
		}
	}

	return n_signal;
}
