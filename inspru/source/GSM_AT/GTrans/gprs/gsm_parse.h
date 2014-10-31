// gsm_parse.h: interface for the gsm_parse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GSM_PARSE_H__597D198B_BF2B_4DEC_B27A_A98136EB4E17__INCLUDED_)
#define AFX_GSM_PARSE_H__597D198B_BF2B_4DEC_B27A_A98136EB4E17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GSM/gsm_msg_def.h"
#include "../GSM/msg_ipc.h"

class gsm_parse  
{
public:


	char          m_call_num[32];



public:
	gsm_parse();
	virtual ~gsm_parse();

	void   set_notify( HWND h_wnd, msg_ipc* p_ipc )   { m_notify_wnd = h_wnd; m_ipc = p_ipc; }
	void   set_cmd( unsigned n_cmd )  { m_cmd = n_cmd;        }
	void   parse( const char* p_buf, int n_size );
 
public:
	void   msg_report2ui(unsigned n_process, unsigned n_rst, bool b_sync = false );
	void   msg_reset_gprs();
	void   msg_report_pos(bool b_state, int n_count);
	void   msg_notify(unsigned int n_type, unsigned int n_param, bool b_sync = false);

protected:
	void  parse_ack(const char* p_buf, int n_len);
	void  parse_sim(const char* p_buf);
	void  parse_register_gprs(const char* p_buf);
	
	int   parse_gsm_signal( const char* p_buf );
	void  parse_gsm_dial_connect( const char* p_buf );
protected:
	unsigned  m_cmd;
	HWND      m_notify_wnd;
	msg_ipc*  m_ipc;
};

#endif // !defined(AFX_GSM_PARSE_H__597D198B_BF2B_4DEC_B27A_A98136EB4E17__INCLUDED_)
