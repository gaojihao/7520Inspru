
#include "StdAfx.h"
#include "gsm_msg_def.h"
#include "msg_sms.h"
#include "ex_debug.h"
#include "gsm_api.h"


#define   GSMSVR_CLS_NAME       ("Dialog")
#define   GSMSVR_WND_NAME       ("HarrySMS1027")

msg_sms::msg_sms()
{
	m_nSMSNum = 0;
	m_nPbookNum = 0;
	memset( m_msg_buf, 0, sizeof(m_msg_buf) );
}

//////////////////////////////////////////////////////////////////////////

msg_sms::~msg_sms()
{
}


void   msg_sms::cmd_gsm_sendmsg()
{
//	bool b_rst = gsm_api_sms_send( m_phone_num, m_msg_buf );
//	m_ipc.post_msg(UMSG_GSM_CMD, GSM_ACK_MSG_RESULT, b_rst );
}


//////////////////////////////////////////////////////////////////////////

void   msg_sms::set_msg_buf(char* p_msg, int n_len  )
{
	if( n_len == -1 ) n_len = strlen(p_msg);
	if ( n_len >= sizeof(m_msg_buf) )
		n_len = sizeof(m_msg_buf) - 1;
	
	memset(m_msg_buf, 0, sizeof(m_msg_buf) );
	memcpy(m_msg_buf, p_msg, n_len );
	EXDBG_TRACE1("sms_buf:%s\n", m_msg_buf);
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_read_sim_sms( )
{
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_del_sim_sms(int nIdx)
{
	gsm_api_sms_delete( nIdx );
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_pbook_read()
{
#if 0
	int nUsed = LOWORD(m_nPbookNum);
	int nMax = HIWORD(m_nPbookNum);

	if ( m_nPbookNum == 0 )
	{
		gsm_api_watching_pause();
		if ( gms_api_pbook_sim_count(nMax, nUsed) )
		{
			m_nPbookNum = (nMax << 16 ) | ((nUsed) & 0xFFFF);
			m_ipc.send_msg( UMSG_GSM_CMD, GSM_ACK_PBOOK_STATE, m_nPbookNum);
		}
		gsm_api_watching_resume();

		Sleep(100);
	}
	


	if ( nUsed > 0 )
	{
		gsm_api_watching_pause();
		gsm_api_decode_mode("UCS2");

		PBOOK_ITEM *p_list = new PBOOK_ITEM[nUsed];
		memset(p_list, 0, sizeof(PBOOK_ITEM) * nUsed);

		int idx = 0;
		for( int i = 1; i <= nMax; i++ )
		{
			if ( gsm_api_phook_sim_read(i, &p_list[idx]) )
			{
				idx++;
				if ( idx >= nUsed )
					break;
			}

			#if 0		
				char sz_temp[64];
				memset( sz_temp, 0, sizeof(sz_temp) );
				sprintf( sz_temp, "pbook_read:%d\n",i);
				ex_trace_scrn(100,10, sz_temp);
			#endif
		}

		gsm_api_decode_mode("IRA");

		gsm_api_watching_resume();
		m_ipc.past_big_data( GSM_DCOPY_SIM_PHONEBOOK, (const char*)p_list, sizeof(PBOOK_ITEM) * nUsed );
		delete []p_list;
	}else
	{
		m_ipc.past_big_data( GSM_DCOPY_SIM_PHONEBOOK, NULL, 0 );
	}
#endif
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::cmd_gsm_pbook_del(int nIdx)
{
#if 0
	gsm_api_pbook_del( nIdx );
#endif
}

//////////////////////////////////////////////////////////////////////////

void   msg_sms::set_sim_pbook(PBOOK_ITEM* p_item)
{
#if 0
	if ( p_item )
	{
		gsm_api_watching_pause();
		gsm_api_decode_mode("UCS2");
		gsm_api_pbook_write( p_item );
		gsm_api_decode_mode("IRA");
		gsm_api_watching_resume();
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
