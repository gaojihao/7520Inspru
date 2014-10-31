
#include "StdAfx.h"
#include "gsm_msg_def.h"
#include "msg_dial.h"
#include "../GPRS/gsm_api.h"
#include "../GPRS/gsm_reset.h"
#include "../GPRS/ex_file.h"
#define	WIRELESS_MANAGER_TITLE		_T("WND_WIRELESS_MANAGER")//WirelessManager.exe进程主窗口名ras拨号


msg_dial::msg_dial()
{
	m_ipc = 0;
}

msg_dial::~msg_dial()
{

}

void  msg_dial::on_message( unsigned n_type, unsigned n_param )
{
	switch ( n_type )
	{
	case GSM_CMD_SET_VOL:    cmd_gsm_set_vol( n_param ); break;
	case GSM_CMD_DIAL   :    cmd_gsm_dial();             break;
	case GSM_CMD_LISTEN:     cmd_dial_listen();          break;
	case GSM_CMD_DIAL_EXT:   cmd_gsm_dial_ext();         break;
	case GSM_CMD_HANGUP:     cmd_gsm_hangup();           break;
	case GSM_CMD_ACCEPTED:   cmd_gsm_accepted();         break;
	default: 
		break;
	}

}



void msg_dial::cmd_gsm_set_vol(unsigned n_level)
{
	n_level += 8;
	gsm_api_set_vol( n_level % 101 );
}



void   msg_dial::cmd_gsm_dial()
{
	cex_file  file;
	int nRst = 0;
	if ( file.open(PATH_NEW_CALL_OUT,cex_file::m_readwrite) )
	{
		char sz_num[64];
		memset( sz_num, 0, sizeof(sz_num) );
		file.read( sz_num, 64 );
		file.close();
		gsm_module::switch_voice2gsm();
		nRst = gsm_api_call_start( sz_num );
	}
}

void msg_dial::cmd_dial_listen()
{
	cex_file  file;
	int nRst = 0;
	if ( file.open(PATH_NEW_CALL_OUT,cex_file::m_readwrite) )
	{
		char sz_num[64];
		memset( sz_num, 0, sizeof(sz_num) );
		file.read( sz_num, 64 );
		file.close();
		nRst = gsm_api_call_start( sz_num );
	}
}

void   msg_dial::cmd_gsm_dial_ext()
{
	cex_file  file;
	if ( file.open( PATH_NEW_CALL_OUT_EX, cex_file::m_readwrite ) )
	{
		char sz_num[64];
		memset( sz_num, 0, sizeof(sz_num) );
		file.read( sz_num, 64 );
		file.close();
		gsm_api_call_ext( sz_num );
	}
}

void  msg_dial::cmd_gsm_hangup()
{
	HWND hWnd;
	hWnd = ::FindWindow(NULL, WIRELESS_MANAGER_TITLE);   //挂断
	if(hWnd)
	{
		::PostMessage(hWnd, UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT, 0);
	}
	gsm_api_handup();
	Sleep(20);
}

void msg_dial::cmd_gsm_accepted()
{
	HWND hWnd;
	hWnd = ::FindWindow(NULL, WIRELESS_MANAGER_TITLE);   //本机接听
	if(hWnd)
	{
		::PostMessage(hWnd, UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT, 1);
	}
	gsm_module::switch_voice2gsm();
	gsm_api_incoming_accept();
	Sleep( 20 );
}

void   msg_dial::set_phone_num(char* p_num, int n_len)
{
	
}

void   msg_dial::set_phone_ext(char* p_num, int n_len)
{	
}
