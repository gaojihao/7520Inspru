
#include "StdAfx.h"
#include "gsm_msg_def.h"
#include "msg_sms.h"
#include "../GPRS/gsm_api.h"
#include "../GPRS/ex_file.h"

#define   GSMSVR_CLS_NAME       _T("Dialog")
#define   GSMSVR_WND_NAME       _T("WND_MAIN_MENU")


msg_sms::msg_sms()
{
	m_nSMSNum = 0;
	m_nPbookNum = 0;
	m_ipc = 0;
	m_nSMSIndex	= 0;
//	memset( m_msg_buf, 0, sizeof(m_msg_buf) );
//	memset( m_phone_num, 0, sizeof(m_phone_num) );
	memset( &m_sms_context, 0, sizeof(m_sms_context) );
	memset( &m_pbook_item, 0, sizeof(m_pbook_item) );
}

//////////////////////////////////////////////////////////////////////////

msg_sms::~msg_sms()
{
}

void  msg_sms::on_message( unsigned n_type, unsigned n_param )
{
	switch ( n_type )
	{
	case GSM_CMD_SENDMSG:    cmd_gsm_sendmsg();   break;
	case GSM_SIM_PBOOK_READ: cmd_gsm_pbook_read(); break;
	case GSM_SIM_PBOOK_DEL:  cmd_gsm_pbook_del(n_param); break;
	case GSM_CMD_ADD_SIM_PBOOK: cmd_gsm_pbook_write(); break;
	default: 
		break;
	}
}

void   msg_sms::cmd_gsm_sendmsg()
{
	cex_file file;
	if ( !file.is_exist(PATH_SEND_SMS_TEXT) )
		file.create( PATH_SEND_SMS_TEXT );

	char sz_sms[512];
	char sz_number[32];
	memset( sz_sms, 0, sizeof(sz_sms) );
	memset( sz_number, 0, sizeof(sz_number) );
	
	if ( file.open( PATH_SEND_SMS_TEXT, cex_file::m_read ) )
	{
		file.seek2begin();
		file.read( sz_sms, sizeof(sz_sms) );
		file.close();
	}

	if ( file.open( PATH_NEW_CALL_OUT, cex_file::m_read) )
	{
		file.seek2begin();
		file.read( sz_number, sizeof(sz_number) );
		file.close();
	}
	bool b_rst = gsm_api_sms_send( sz_number, sz_sms );
	if ( m_ipc )
		m_ipc->post_msg(UMSG_GSM_CMD, GSM_ACK_MSG_RESULT, b_rst );
}


void   msg_sms::cmd_gsm_pbook_write()
{
	gsm_api_decode_mode("UCS2");
	gsm_api_pbook_write( &m_pbook_item );
	gsm_api_decode_mode("IRA");
}
//////////////////////////////////////////////////////////////////////////

void   msg_sms::set_phone_num(char* p_num, int n_len)
{
/*
	if( n_len == -1 ) n_len = strlen(p_num);
	if ( n_len >= sizeof(m_phone_num) )
		n_len = sizeof(m_phone_num) - 1;
	
	memset(m_phone_num, 0, sizeof(m_phone_num) );
	memcpy(m_phone_num, p_num, n_len );
*/
 }


void   msg_sms::set_msg_buf(char* p_msg, int n_len  )
{
/*
	if( n_len == -1 ) n_len = strlen(p_msg);
	if ( n_len >= sizeof(m_msg_buf) )
		n_len = sizeof(m_msg_buf) - 1;
	
	memset(m_msg_buf, 0, sizeof(m_msg_buf) );
	memcpy(m_msg_buf, p_msg, n_len );
	EXDBG_TRACE1("sms_buf:%s\n", m_msg_buf);
*/
}

//////////////////////////////////////////////////////////////////////////
//检测是否reset系统
bool check_resetsys(SM_PARAM& sms_context)
{
	return true;
}

void   msg_sms::cmd_gsm_read_sim_sms( int nIdx )
{
	m_nSMSIndex	= nIdx;
	int n = gsm_api_sms_read(m_nSMSIndex, 1, &m_sms_context);
	Sleep(1000);
//	if ( n  )
//	{
		gsm_api_sms_delete(6);

		//发送至收件箱
		if ( m_ipc )
			m_ipc->past_data( GSM_DCOPY_ACK_MSG, (const char*)(&m_sms_context), sizeof(SM_PARAM) );	
//	}
}

void   msg_sms::cmd_gsm_get_sms_content( int nSMS )
{
	gsm_api_sms_delete( 6 );
	if ( m_ipc )
		m_ipc->past_data( GSM_DCOPY_ACK_MSG, (const char*)nSMS, sizeof(SM_PARAM) );	
}
//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_del_sim_sms(int nIdx)
{
	gsm_api_sms_delete( 6 );
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_pbook_read()
{

	int nUsed = LOWORD(m_nPbookNum);
	int nMax = HIWORD(m_nPbookNum);
	//是否在读取电话号码薄

	if ( m_nPbookNum == 0 )
	{
		if ( gms_api_pbook_sim_count(nMax, nUsed) )
		{
			m_nPbookNum = (nMax << 16 ) | ((nUsed) & 0xFFFF);
			if ( m_ipc )
				m_ipc->send_msg( UMSG_GSM_CMD, GSM_ACK_PBOOK_STATE, m_nPbookNum);
		}
	}
	
	if ( nUsed > 0 )
	{
		PBOOK_ITEM *p_list = new PBOOK_ITEM[nUsed];
		memset(p_list, 0, sizeof(PBOOK_ITEM) * nUsed);
		int idx = 0;


		gsm_api_decode_mode("UCS2");
		for( int i = 1; i <= nMax; i++ )
		{
			if ( gsm_api_phook_sim_read( i, &p_list[idx]) )
			{
				idx++;
				if ( idx >= nUsed )
					break;
				if ( m_ipc )
					m_ipc->post_msg( UMSG_GSM_CMD, GSM_SIM_PBOOK_READ_PROCESS, MAKELONG(idx+1,nUsed) );
				Sleep( 150 );
			}
			else
			{
				Sleep(250);
				if ( gsm_api_phook_sim_read( i, &p_list[idx]) )
				{
					idx++;
					if ( idx >= nUsed )
						break;
					if ( m_ipc )
						m_ipc->post_msg( UMSG_GSM_CMD, GSM_SIM_PBOOK_READ_PROCESS, MAKELONG(idx+1,nUsed) );
				}else
				{
					break;
				}
			}
		}
		gsm_api_decode_mode("IRA");


		if ( m_ipc )
		{
			m_ipc->send_msg( UMSG_GSM_CMD, GSM_ACK_PBOOK_STATE, m_nPbookNum);
			m_ipc->past_big_data( GSM_DCOPY_SIM_PHONEBOOK, (const char*)p_list, sizeof(PBOOK_ITEM) * nUsed );
		}
		delete []p_list;
	}else
	{
		if ( m_ipc )
		{
			m_ipc->send_msg( UMSG_GSM_CMD, GSM_ACK_PBOOK_STATE, m_nPbookNum);
			m_ipc->past_big_data( GSM_DCOPY_SIM_PHONEBOOK, NULL, 0 );
		}
	}

}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_pbook_del(int nIdx)
{
	gsm_api_pbook_del( nIdx );
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::add_sim_pbook(PBOOK_ITEM* p_item)
{
	if ( p_item )
	{
		memcpy( &m_pbook_item, p_item, sizeof(m_pbook_item) );
/*
		gsm_api_decode_mode("UCS2");
		gsm_api_pbook_write( p_item );
		gsm_api_decode_mode("IRA");
*/
	}
}

//////////////////////////////////////////////////////////////////////////
