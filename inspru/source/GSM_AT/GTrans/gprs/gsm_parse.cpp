// gsm_parse.cpp: implementation of the gsm_parse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "gsm_parse.h"
#include "msg_data.h"
#include "gsm_api.h"
#include "rectrl_api.h"
#include "GPRSLogic.h"
#include "../LogTool/LogTool.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
st_sms	g_stSMS;

extern msg_ipc*        g_pIPC;
//是否在读取电话号码薄
BOOL   g_bReadPhoneBooking = FALSE;

//发送调度信息的线程
DWORD THDSendMSG(LPVOID lpParameter)
{
	st_sms stSMS;
	memcpy(&stSMS, &g_stSMS, sizeof(st_sms));
	g_pIPC->past_data( GSM_DCOPY_DIPATCH_MSG, (char*)&stSMS, sizeof(st_sms));

	return 0;
}

gsm_parse::gsm_parse()
{
	m_notify_wnd = NULL;
	m_cmd = eCMD_NULL;
	m_ipc = NULL;
	memset( m_call_num, 0, sizeof(m_call_num) );
}

gsm_parse::~gsm_parse()
{

}

void   gsm_parse::msg_notify(unsigned int n_type, unsigned int n_param, bool b_sync /* =false */)
{
	if ( b_sync )
		::SendMessage( m_notify_wnd, MSG_GSM_NOTIFY, n_type, n_param );
	else
		::PostMessage( m_notify_wnd, MSG_GSM_NOTIFY, n_type, n_param );
}

//////////////////////////////////////////////////////////////////////////

void   gsm_parse::parse(const char* p_buf, int n_size )
{
	msg_data* p_data =  msg_data::instance();

	p_data->empty_ipd();

	//提取"+IPD"信息
	msg_data::instance()->set_ack_buf( (char*)p_buf, n_size );

	//解析GSM-AT指令
	if ( p_data->ack_len() > 0 )
		parse_ack( p_data->ack_data(), p_data->ack_len() );

	return;
}



void  gsm_parse::parse_ack( const char* p_buf , int n_len)
{
	//CLogTool::Instance()->WriteLogFile(p_buf,n_len);
	//parse_gsm_signal(p_buf );
	parse_gsm_dial_connect( p_buf );
}


//////////////////////////////////////////////////////////////////////////

void  gsm_parse::parse_sim(const char* p_buf)
{
	char* p_ack = "\r\n+CPIN: READY";
	if ( strstr( p_buf, p_ack ) != 0 )
		msg_notify(eGSM_SIM_CARD,1,false);
	else
		msg_notify(eGSM_SIM_CARD,0,false);
}

//////////////////////////////////////////////////////////////////////////


void  gsm_parse::parse_register_gprs(const char* p_buf)
{
	if ( strstr( p_buf, "\r\n+CGATT: 1") != 0 )
		msg_notify(eGSM_REGISTER_GPRS,1);
	else if ( strstr( p_buf, "\r\n+CGATT: 0") != 0 )
		msg_notify(eGSM_REGISTER_GSM,1);
	else
		msg_notify(eGSM_REGISTER_GSM,0);
}

//////////////////////////////////////////////////////////////////////////

void   gsm_parse::msg_report2ui(unsigned n_process, unsigned n_rst, bool b_sync /* = false*/ )
{
	HWND hwnd = FindWindow(NULL,_T("WND_MAIN_MENU"));
	::PostMessage( m_notify_wnd, UMSG_GSM_CMD, n_process, n_rst );
	msg_notify( n_process, n_rst, b_sync );
}
//////////////////////////////////////////////////////////////////////////

void gsm_parse::msg_report_pos(bool b_state, int n_count)
{
	unsigned int n_param = (n_count & 0xFFFF);
	if ( b_state ) n_param |= 0x10000;
	msg_notify( eGSM_REPORT_POS, n_param );
}

void   gsm_parse::msg_reset_gprs()
{
	msg_notify( eGSM_RESET_GPRS, 0 );
}



void  gsm_parse::parse_gsm_dial_connect( const char* p_buf )
{
	const char* p_str = 0;

	// GSM calling state
	if( strstr( p_buf, "\r\nBUSY") ||       //对方忙
		strstr( p_buf, "\r\nNO CARRIER") || //对方无应答
		strstr( p_buf, "\r\nNO ANSWER")  || //对方没有反映
		strstr( p_buf, "\r\nNO DIALTON") ||
		strstr( p_buf, "\r\nMISSED_CALL"))
	{
		msg_notify(eGSM_DIAL_CONNECT_RST,0, TRUE);
	}

	// new message 
	p_str = strstr(p_buf, "\r\n+CMTI:");
	if ( p_str )
	{
		p_str = strstr(p_str, ",");
		if ( p_str )
		{
			p_str ++;
			int n_idx = atoi(p_str);
			msg_notify( eGSM_NEW_SMS_COMING, n_idx, FALSE );
		}
	}

	p_str = strstr(p_buf, "\r\n++CSQN");
	if (p_str)
	{
		int n_signal = 0;
		if(p_str)
			p_str = strstr( p_str, ":");
		if ( p_str )
		{
			p_str += 1;
			if ( p_str )
			{
				n_signal = atoi( p_str );
				if ( n_signal == 99 ) 
					n_signal = 0;
				msg_notify(eGSM_SIGNAL_VALUE,n_signal, true);
			}
		}
	}



	// The call is through
	p_str = strstr(p_buf, "\r\n+COLP:");
	if (  p_str )
	{
		msg_notify( eGSM_DIAL_CONNECT_RST, 1, TRUE );
	}


	// +CLIP: "13564821116", 129, "", ,"", 0
	p_str = strstr( p_buf, "\r\n+CLIP:");
	if ( p_str )
	{
		if(  !CGPRSLogic::Instance()->m_bdialing )
		{
			p_str = strstr(p_str,"\"");
			if ( p_str )
			{
				p_str++;
				memset( m_call_num , 0, sizeof(m_call_num) );
				for( int i = 0; i < sizeof(m_call_num); i++ )
				{
					if ( p_str[i] == '\0' || p_str[i] == '\"' )
						break;
					m_call_num[i] = p_str[i];
				}

				msg_notify(eGSM_NEW_CALLING_RST, (unsigned int)m_call_num, TRUE );
				CGPRSLogic::Instance()->m_bdialing = true;
			}
		}

	}
}

int gsm_parse::parse_gsm_signal(const char* p_buf)
{
	int n_signal = 0;
	//+CSQN: 27,0
	const char* p_ptr = strstr(p_buf, "\r\n+CSQ");
	if(p_ptr)
		p_ptr = strstr( p_ptr, ":");
	if ( p_ptr )
	{
		p_ptr += 1;
		if ( p_ptr )
		{
			n_signal = atoi( p_ptr );
			if ( n_signal == 99 ) 
				n_signal = 0;
			msg_notify(eGSM_SIGNAL_VALUE,n_signal, true);
		}
	}
	else
		return 0;
	return n_signal;
}