
#include "StdAfx.h"
#include "gsm_api.h"
#include "ex_basics.h"
#include "serial_win32.h"
#include "ex_debug.h"
#include "gsm_at.h"
#include "ex_file.h"
#include "../FileMapping/FileMapping.h"

#define  PATH_IMEI            "\\Flashdrv Storage\\imei.txt"
#define  MAX_RECV_BUF         ( 1024 * 4 )

//extern void   ShowTip( CString strTip );
//serial_win32     g_gsm_serial;    // com serial
unsigned long    g_gsm_thrid  = 0;
HANDLE           g_gsm_handle = 0;
gsm_ctrl         g_ctrl;      // GSM message control send & read
bool             g_gsm_third_running = true;
char             g_gsm_imei[16];
sec_lock         g_lock_send;
/*extern*/ char		g_szAPN[16] = "CMNET";	//CMNET/CMWAP

void  watching_gsm( void  *a_param );
void  __gsm_gprs_send(const char* p_data, int n_size);

/*
bool   gsm_api_sms_load_sms_param(SM_PARAM* p_src, 
								  const char* str_phone_number, 
								  const char* str_struct_info );

int   gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);*/
int   gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
int   gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
int   gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
int   gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
int   gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength);
bool  sms_is_only_assic(const char* str_sms, int n_len);
int   sms_ucs2_length(const char* str_sms, int n_len);
//int   gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);
int   gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
int   gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength);
/////////////////////////////////////////////////////////////////////

gsm_ctrl* gsm_api_ctrl()
{
	return &g_ctrl;
}
//////////////////////////////////////////////////////////////////////////
bool  gsm_api_start(unsigned n_com, unsigned n_baud)
{
	bool b_suc = g_ctrl.connect_gsm( n_com, n_baud );
	if ( b_suc )
	{
		if ( g_gsm_handle == 0 )
		{
			g_gsm_third_running = true;
			g_gsm_handle = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)watching_gsm,
				NULL,
				0,
				&g_gsm_thrid );
		}
	}
	return b_suc;
}


void    gsm_api_close()
{
	if ( g_gsm_handle )
	{
		g_gsm_third_running = false;
		char* p_buf = "ATE0\r";
		g_ctrl.send_msg( p_buf, strlen(p_buf) );
		if ( WAIT_TIMEOUT == WaitForSingleObject( g_gsm_handle, 200) )
		{
			EXDBG_TRACE0(" Wating watching gsm thread failed..\n");
			TerminateThread( g_gsm_handle, 0 );
		}
	}
	g_ctrl.shutdown_gsm();
	g_gsm_third_running = false;
	g_gsm_handle = 0;
}



void  watching_gsm( void  *a_param )
{
	unsigned  offset = 0;
	char*     ptr_str = 0;
	int       n_count = 0;
	while ( g_gsm_third_running )
	{
		//EXDBG_TRACE0("Wating GSM DATA.........\n");
		g_ctrl.wait_com_rx();
		Sleep(10);
		g_ctrl.reset_com_rx();
		g_ctrl.read_empy_from_gsm_com();
		
	} // while ( true )

	EXDBG_TRACE0("watching gsm has exit!\n");
}

//////////////////////////////////////////////////////////////////////////

bool  gsm_api_config(bool b_check_gsn )
{
	bool  b_suc = false;
	char* gsm_cmd[] = {
		//"AT+CGMR\r",               //��ѯģ��İ汾
		"ATE0\r",                  //�رջ�������

#ifdef DEBUG
		"AT+CMEE=2\r",             //URC�Ĵ�����ʾ
		"AT+CGMR\r",               //��ѯģ��İ汾
#endif
		"AT+GSN\r",                //��ѯGSMģ��IMEI
	};
	
	char* p_cmd;
	int  n_num = sizeof(gsm_cmd)/sizeof(gsm_cmd[0]);
	if ( !b_check_gsn ) 
		n_num --;

	Sleep( 500 );
	for( int i = 0; i < n_num; i++ )
	{
		p_cmd = gsm_cmd[i];
		g_ctrl.send_msg( p_cmd, strlen(p_cmd) );
		for( int j = 0; j < 10; j++ )
		{
			Sleep( 100 * j );
			b_suc = g_ctrl.check_ack( "OK" );
			if ( b_suc )
			{
				if ( i != n_num - 1 )
					g_ctrl.empty_data();
				break;
			}
		}
		
		if ( !b_suc && i == 0 )
		{
			EXDBG_TRACE1("CMD:[%s] ack error!\n", p_cmd );
			return false;
		}
	}
	
	if ( b_check_gsn && g_ctrl.check_ack("OK") )
	{
		char* p_ack = strstr( g_ctrl.ack_buf(),"\r\n");
		if ( p_ack )
		{
			p_ack += 2;
			memset( g_gsm_imei, 0, sizeof(g_gsm_imei) );
			memcpy( g_gsm_imei, p_ack, 15 );
		}
	}
	g_ctrl.empty_data();

	char szTemp[32];
	memset(szTemp, NULL, sizeof(szTemp));
	bool bRtn = gsm_api_get_imsi(szTemp);
	if(bRtn)
	{
		//memset(g_pstShareMem->szIMSI, NULL, sizeof(g_pstShareMem->szIMSI));
		//ex_char2uni(szTemp, g_pstShareMem->szIMSI, sizeof(szTemp));
	}
	g_ctrl.empty_data();
	Sleep(1000);
	memset(szTemp, NULL, sizeof(szTemp));
	bRtn = gsm_api_get_imei(szTemp);
	if(bRtn)
	{
		//memset(g_pstShareMem->szIMEI, NULL, sizeof(g_pstShareMem->szIMEI));
		//ex_char2uni(szTemp, g_pstShareMem->szIMEI, sizeof(szTemp));
	}
	
	g_ctrl.empty_data();
	
	return true;
}

bool   gsm_api_config2()
{
	bool  b_suc = false;
	char* gsm_cmd[] =
	{
            "AT+CIPHEAD=1\r",          //AT+CIPHEAD   
			"AT+COLP=1\r",             //�Է�ժ��֪ͨ
			"AT+CLIP=1\r",             //����֪ͨ
			"AT+CNMI=2,1,0,0,0\r",     //����Ϣ֪ͨ,�洢��SIM����
			"AT+CMGF=0\r",             //PDUģʽ
			"AT+CIMI\r",
			"AT+CIPFLP=0\r",
			"AT+CRSL=95\r",            //����������������
			"AT+CMIC=0,12\r",
			"AT+ECHO=3000,5,10,0\r",
			"AT+EXUNSOL=\"SQ\",1\r",
	};
	
	char* p_cmd;
	int  n_num = sizeof(gsm_cmd)/sizeof(gsm_cmd[0]);
	
	Sleep( 500 );
	for( int i = 0; i < n_num; i++ )
	{
		p_cmd = gsm_cmd[i];
		g_ctrl.send_msg( p_cmd, strlen(p_cmd) );
		Sleep( 20 );
		for( int j = 0; j < 10; j++ )
		{
			Sleep( 30 * j );
			b_suc = g_ctrl.check_ack( "OK" );
			if ( b_suc )
			{
				if ( i != n_num - 1 )
					g_ctrl.empty_data();
				break;
			}
		}
#if 0
		CString str = g_ctrl.ack_buf();
//		ShowTip( str );
#endif
		
		if ( !b_suc && i == 0 )
		{
			EXDBG_TRACE1("CMD:[%s] ack error!\n", p_cmd );
			return false;
		}
	}
	

	g_ctrl.empty_data();
	
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool   gsm_api_check_sim()
{
	char* p_cmd = "AT+CPIN?\r\0";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 300 * i );
		if ( g_ctrl.check_ack("\r\n+CPIN: READY") )
			return true;
	}
	return false;
}


int       gsm_api_query_signal()
{
	char* p_cmd = "AT+CSQ\r";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );

	int n_value = 0;
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		char* p_ptr = strstr( g_ctrl.ack_buf(), "\r\n+CSQ:");
		if ( p_ptr )
		{
			p_ptr += 7;
			if ( p_ptr )
			{
				n_value = atoi( p_ptr );
				if ( n_value == 99 ) 
					n_value = 0;
			}
		}
	}

	//g_pstShareMem->nSignal = n_value;

	return n_value;
}

bool      gsm_api_register_gsm()
{
	bool  b_suc = false;
	char* p_cmd = "AT+CREG?\r";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );
	for( int i = 0; i < 15; i++ )
	{
		Sleep( 400 * i );
		//EXDBG_TRACE1("--->register_gsm_ack:%s\n", g_ctrl.ack_buf() );	
		if ( g_ctrl.check_ack("\r\n+CREG: 0,1") ||
			g_ctrl.check_ack("\r\n+CREG: 0,5") )
		{
			b_suc = true;
			break;
		}
		else if ( g_ctrl.check_ack("\r\n+CREG: 0,2") )
		{
			b_suc = false;
			break;
		}			
	}
	g_ctrl.empty_data();
	return b_suc;
}

//////////////////////////////////////////////////////////////////////////
/*
int       gsm_api_query_net_isp()
{
	char* p_str = "AT+COPS?\r";
	for( int i = 0; i < 15; i++ )
	{
		g_ctrl.send_msg( p_str, strlen(p_str) );
		if( strstr(g_ctrl.ack_buf(), "CHINA MOBILE") != 0 )
		{
			return gsm_net_china_mobile;
		}
		else if ( strstr(g_ctrl.ack_buf(), "CHINA UNICOM") != 0 )
		{
			return gsm_net_china_unicom;
		}
		Sleep( 2000 );
	}
	return gsm_net_null;
}
*/

bool      gsm_api_register_gprs()
{
	int i;
	bool  b_suc = false;
	//Indicates the state of GPRS attachment
	//0: detached 1:attached
	char* p_cmd = "AT+CGATT=1\r";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );
	Sleep( 100 );
	for( i = 0; i < 5; i++ )
	{
		Sleep( 260  );
		if ( g_ctrl.check_ack("\r\nOK") )
		{
			break;
		}
	}


	b_suc = false;
	p_cmd = "AT+CGATT?\r";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );
	Sleep( 100 );
	for( i = 0; i < 5; i++ )
	{
		Sleep( 400 * i );
		if ( g_ctrl.check_ack("\r\n+CGATT: 1") )
		{
			b_suc = true;
			break;
		}

		else if ( g_ctrl.check_ack("\r\n+CGATT: 0") )
		{
			b_suc = false;
			break;
		}
	}

	if ( b_suc )
	{
		//AT+CSTT               // ����APN���û���������
		//AT+CIICR              // �����ƶ�����
		//AT+CIFSR              // ��ñ���IP
		//10.79.87.248

		char szCMD[64];
		g_szAPN[strlen(g_szAPN)] = '\"';
		g_szAPN[strlen(g_szAPN)] = '\r';
		sprintf(szCMD, "AT+CSTT=\"%s", g_szAPN);
		//char* szCMD = "AT+CSTT=\"CMNET\"\r";
		gsm_api_scmd( szCMD, strlen(szCMD) );
		Sleep( 100 );
		for( i = 0; i < 10; i++ )
		{
			Sleep( 400 * i );
			if ( g_ctrl.check_ack("OK") || g_ctrl.check_ack("ERROR") )
			{
				break;
			}
		}
		g_ctrl.empty_data();

		char* p_cmd = "AT+CIICR\r";
		gsm_api_scmd( p_cmd, strlen(p_cmd) );
		Sleep( 100 );
		//@@@ -- ʱ��������ܸ���һ��
		for( i = 0; i < 10; i++ )
		{
			Sleep( 500 * i );
			if ( g_ctrl.check_ack("OK") )
			{
				break;
			}
		}
		g_ctrl.empty_data();

		p_cmd = "AT+CIFSR\r";
		gsm_api_scmd( p_cmd, strlen(p_cmd) );
		Sleep( 100 );
		for( i = 0; i < 10; i++ )
		{
			Sleep( 400 * i );
			if ( g_ctrl.check_ack("OK") )
			{
				break;
			}
		}
		g_ctrl.empty_data();
	}
	return b_suc;
}



int      gprs_api_connect_ip(const char* sz_type, const char* sz_ip, unsigned n_port)
{
	char  sz_buf[128];

	memset(sz_buf, 0, sizeof(sz_buf));

	//AT+CIPSTART="TCP","116.228.221.51",7015      
	sprintf( sz_buf, "AT+CIPSTART=\"%s\",\"%s\",\"%d\"\r",
		sz_type, /*"122.224.175.197"*/sz_ip, n_port );
	gsm_api_scmd( sz_buf, strlen(sz_buf) );


	bool b_suc = false;
	for( int i = 0; i < 20; i++ )
	{
		Sleep( 200 * i );
		//EXDBG_TRACE1("--->connect_ip_ack:%s\n", g_ctrl.ack_buf() );
		if ( g_ctrl.check_ack("CONNECT OK") )
		{
			b_suc = true;
			break;
		}
		
		else if ( g_ctrl.check_ack("ALREADY CONNECT") ||
			      g_ctrl.check_ack("ERROR"))
		{
			b_suc = false;
			break;
		}
	}
	//g_ctrl.empty_data();
	return b_suc;
}


//////////////////////////////////////////////////////////////////////////


void  gprs_api_shutdown( )
{
	int i;
	char* p_cmd = "AT+CIPCLOSE\r";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );
	for( i = 0; i < 10; i++ )
	{
		Sleep( 200 * i );
		if ( g_ctrl.check_ack( "\r\nSHUT OK") )
			break;
		else if ( g_ctrl.check_ack("ERROR") )
			break;
	}

	p_cmd = "AT+CIPSHUT\r";
	gsm_api_scmd( p_cmd, strlen(p_cmd) );
	for( i = 0; i < 10; i++ )
	{
		Sleep( 200 * i );
		if ( g_ctrl.check_ack( "\r\nSHUT OK") )
			break;
		else if ( g_ctrl.check_ack("ERROR") )
			break;
	}
	return;
}



//////////////////////////////////////////////////////////////////////////

char*     gsm_api_imei()
{
	return g_gsm_imei;
}

//////////////////////////////////////////////////////////////////////////

void  __gsm_gprs_send(const char* p_data, int n_size)
{
	g_ctrl.send_msg( p_data, n_size );
}




void  gsm_api_scmd(const char* p_cmd, int n_len )
{
	g_lock_send.enter();
	__gsm_gprs_send( p_cmd, n_len );
	g_lock_send.leave();
}

//////////////////////////////////////////////////////////////////////////

bool  gprs_send_data(const char* sz_data, int n_len)
{
	if ( n_len < 1 ) return true;
	
	char  sz_cmd[32];
	bool  b_suc = false;
	int   i = 0;
	memset( sz_cmd, 0, sizeof(sz_cmd) );
	//prepare for send the data number.
	sprintf( sz_cmd, "AT+CIPSEND=%d\r", n_len );

	g_lock_send.enter( );
	__gsm_gprs_send( sz_cmd, strlen(sz_cmd) );
	for( i = 0; i < 10; i++ )
	{
		Sleep( 90 * i );
		b_suc = g_ctrl.check_ack(">");
		if ( b_suc ) 
			break;

		if ( g_ctrl.check_ack("ERROR") /*|| g_ctrl.check_ack("0,CLOSED")*/ )
		{
			b_suc = false;
			break;
		}
	}

	if ( b_suc )
	{
		b_suc = false;
		Sleep(50);
		// Send GPRS data
		__gsm_gprs_send( sz_data, n_len );
		unsigned long n_start = ::GetTickCount();
		while( true )
		{
			if ( ::GetTickCount() - n_start > 59 * 1000 )
			{
				b_suc = false;
				EXDBG_TRACE1("\nGPRS data send over time : %d \n", ::GetTickCount() - n_start );
				break;
			}
			Sleep( 500 );
			//EXDBG_TRACE1("--->gprs_send:%s\n", g_ctrl.buf() );
			b_suc = g_ctrl.check_ack( "SEND OK" );
			if ( b_suc )
			{
				EXDBG_TRACE1("\nGPRS data Send OK : %d \n", ::GetTickCount() - n_start );
				break;
			}

			if ( g_ctrl.check_ack("SEND FAIL") || 
				 g_ctrl.check_ack("CLOSED")  ||
				 g_ctrl.check_ack("CONNECT FAIL") )
			{
				EXDBG_TRACE1("\nGPRS data Send Fail time: %d \n", ::GetTickCount() - n_start );
				b_suc = false;
				break;
			}
		} // while( true )
	} // b_suc
	

	g_lock_send.leave();

	return b_suc;
}

void gsm_api_signal()
{
	char* sz_cmd = "AT+CSQ\r";
	g_lock_send.enter( );
	__gsm_gprs_send( sz_cmd, strlen(sz_cmd) );
	Sleep( 50 );
	g_lock_send.leave();
}

void gsm_api_set_vol( int n_value )
{
	if ( n_value >= 70 )
		n_value = 70;
	char sz_buf[32];
	memset( sz_buf, 0, sizeof(sz_buf) );
	sprintf(sz_buf,"AT+CLVL=%d\r", n_value);

#if 1
	//CString str = sz_buf;
//	ShowTip( sz_buf );
#endif

	g_lock_send.enter( );
	__gsm_gprs_send( sz_buf, strlen(sz_buf) );
	for( int i = 0; i < 5; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\nOK") )
			break;
	}

	/*
	memset( sz_buf, 0, sizeof(sz_buf) );
	sprintf(sz_buf,"AT+CRSL=%d\r", n_value);
	g_ctrl.empty_data(true);
	__gsm_gprs_send( sz_buf, strlen(sz_buf) );
	for( i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\nOK") )
			break;
	}
	*/
	g_lock_send.leave();
}


int    gsm_api_call_start(const char* p_number )
{
	char sz_buf[128];
	char* p_cmd = "AT+COLP=1\r";
	int i  = 0;
	int nRst = 0;
	memset( sz_buf, 0, sizeof(sz_buf) );
	sprintf(sz_buf,"ATD%s;\r", p_number);

	g_lock_send.enter(); 
	__gsm_gprs_send( sz_buf, strlen(sz_buf) );
	for( i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\nOK") )
		{
			nRst = 1;
			break;
		}
		else if ( g_ctrl.check_ack("+CME ERROR: invalid characters in dial string"))
		{
			nRst = 0;
			break;
		}
	}
	g_lock_send.leave();
	
	return nRst;
}



void    gsm_api_call_ext(const char* sz_ext)
{
	char  sz_cmd[64+4];
	memset( sz_cmd, 0, sizeof(sz_cmd) );
	
	int n_len = strlen( sz_ext );
	if ( n_len > 32 )
		n_len = 32;
	sz_cmd[0] = '"';
	for( int i = 0; i < n_len; i++)
	{
		sz_cmd[2*i+1] = sz_ext[i];
		sz_cmd[2*i+2] = ',';
	}
	sz_cmd[2*n_len]= '"';
	
	char  sz_buf[128];
	memset(sz_buf, 0, sizeof(sz_buf));
	sprintf(sz_buf, "AT+VTS=%s\r", sz_cmd);
	g_lock_send.enter( );
	__gsm_gprs_send( sz_buf, strlen(sz_buf) );
	Sleep( 10 );
	g_lock_send.leave();
}

void    gsm_api_handup()
{
	char * p_cmd = "ATH\r";
	g_lock_send.enter( );
	__gsm_gprs_send( p_cmd, strlen(p_cmd) );
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\nOK") )
			break;
	}
	g_lock_send.leave();
}


void    gsm_api_incoming_accept()
{
	char * p_cmd = "ATA\r";
	g_lock_send.enter( );
	__gsm_gprs_send( p_cmd, strlen(p_cmd) );
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\nOK") )
			break;
	}
	g_lock_send.leave();
}



//ɾ��һ������
bool   gsm_api_sms_delete(int idx_sms)
{
	char   sz_msg[32];
	sprintf( sz_msg, "AT+CMGD=%d\r", idx_sms );
	__gsm_gprs_send( sz_msg, strlen(sz_msg) );
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\nOK") )
			return true;
	}
	return false;
}


// ��ȡָ���Ķ���
// msg_type: ��������  0: UNREAD 1: READ 2�� UNSENT  3: SENT  4: ALL
// p_msg: ���������Ķ��Ŵ��������
// idx_msg: Ҫ��ȡ�Ķ���Ϣ����š�
// msg_type: ����Ϣ�Ĳ������� 
// ����: �յ��Ķ���Ϣ����
/*
int     gsm_api_sms_read(int idx_msg, unsigned msg_type, SM_PARAM* p_msg)
{
	int    i, n_msg = 0;           // ����Ϣ����ֵ
	char*  p_str = 0;
	char   sz_buf[1024];
	char   sz_cmd[32];
	
	memset( sz_buf, 0, sizeof(sz_buf) );
	memset( sz_cmd, 0, sizeof(sz_cmd) );
	p_msg->index = idx_msg;

	sprintf( sz_cmd, "AT+CMGF=%d\r", 0 );
	__gsm_gprs_send( sz_cmd, strlen(sz_cmd) );

	for( i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if ( g_ctrl.check_ack("\r\nOK") )
			break;
	}
	

	
	sprintf( sz_cmd, "AT+CMGR=%d\r", idx_msg );
	__gsm_gprs_send( sz_cmd, strlen(sz_cmd) );
	
	for( i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		p_str = strstr( g_ctrl.ack_buf(), "\r\n+CMGR:" );
		if ( p_str )
			break;
	}
	if ( p_str )
	{
		p_str += 2;
		p_str = strstr(p_str,"\r\n");             // ����һ��
		p_str += 2;                               // ����"\r\n"
		gsmDecodePdu(p_str, p_msg);               // PDU������
		return 1;
	}
	return 0;


#if 0	
	p_msg->index = idx_msg;
	p_str = (char*)gsm_api_at_cmd(ATCMGF, "0");
	g_ctrl.send_msg(p_str, strlen(p_str) );
	Sleep(5);
	watching_gsm_read_empy(sz_buf, sizeof(sz_buf));
	
	sprintf( sz_buf, "%d", idx_msg);
	p_str = (char*)gsm_api_at_cmd(ATCMGR, sz_buf);
	g_ctrl.send_msg( p_str, strlen(p_str) );
	Sleep(100);
	for( int i = 0; i < 20; i++ )
	{
		if ( watching_gsm_read_empy(sz_buf, sizeof(sz_buf)) > 0 )
			break;
		Sleep(100);
	}
	
	p_str = sz_buf;
	if ( strncmp( sz_buf, "ERROR", 5) != 0 )
	{
		p_str = strstr( p_str, "+CMGR:");
		if ( p_str != 0 )
		{
			p_str = strstr(p_str,"\r\n");             // ����һ��
			p_str += 2;                               // ����"\r\n"
			gsmDecodePdu(p_str, p_msg);               // PDU������
			n_msg++;                                  // ����Ϣ������1
		}
	}
	return n_msg;
#endif
}
*/

bool    gsm_api_sms_sim_count( int& n_max, int& n_cur )
{
	char* p_cmd = "AT+CPMS?\r";
	char* p_str = 0;
	__gsm_gprs_send( p_cmd, strlen(p_cmd) );

	for( int i = 0; i <10; i++ )
	{
		Sleep( 50 * i );
		p_str = strstr( g_ctrl.ack_buf(), "+CPMS:");
		if ( p_str ) break;
	}

	if ( p_str )
	{
		p_str = strstr( p_str, "SM" );
		if ( p_str )
		{
			p_str = strstr(p_str, ",");
			if ( p_str )
			{
				p_str++;
				n_cur = atoi( p_str );
				p_str = strstr( p_str,",");
				p_str++;
				n_max = atoi( p_str );				
				return true;
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
//at+cpbs? 
//+CPBS: "SM",0,100 
//"SM": storage; 
//0:   used; 
//100: total; 
//////////////////////////////////////////////////////////////////////////
bool    gms_api_pbook_sim_count(int& n_max, int& n_cur)
{
	char* p_cmd = "AT+CPBS?\r";
	char* p_str = 0;
	__gsm_gprs_send( p_cmd, strlen(p_cmd) );
	
	for( int i = 0; i <10; i++ )
	{
		Sleep( 50 * i );
		p_str = strstr( g_ctrl.ack_buf(), "+CPBS:");
		if ( p_str ) break;
	}

	if ( p_str )
	{
		p_str = strstr(p_str, "SM");
		if ( p_str )
		{
			p_str = strstr(p_str, ",");
			if ( p_str )
			{
				p_str++;
				n_cur = atoi( p_str );
				p_str = strstr( p_str,",");
				p_str++;
				n_max = atoi( p_str );
				return true;
			}
		}
	}	
	return false;
}

/*
//////////////////////////////////////////////////////////////////////////
bool     gsm_api_phook_sim_read(unsigned nIdx, PBOOK_ITEM* pItem)
{
	//AT + CPBS = ��SM��        // ָ��Ҫ��ȡ�绰���Ĵ洢λ��ΪSIM��
    //AT + CPBR = 1��3          // ��ȡ�洢��SIM���м�¼��Ϊ1~3�绰����¼
	//"AT+CSCS?\r";
	char sz_cmd[32];
	char* p_str = 0;

	sprintf(sz_cmd,"AT+CPBR=%d\r", nIdx );
	__gsm_gprs_send( sz_cmd, strlen( sz_cmd) );
	Sleep( 100 );
	g_ctrl.empty_data(true);
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 50 * i );
		p_str = strstr( g_ctrl.ack_buf(), "\r\n+CPBR:");
		if ( p_str )
			break;
	}

	int n_len = 0;
	if ( p_str )
	{
		pItem->nIdx = nIdx;
		p_str = strstr(p_str,"\"");
		//�绰����
		if ( p_str )
		{
			n_len = 0;
			p_str++;
			while( p_str && *p_str != '\"' )
			{
				pItem->phone[n_len++] = *p_str++;
			}
		}
		
		p_str++;
		p_str = strstr(p_str,"\"");
		if ( p_str )
		{
			p_str++;
			n_len = 0;
			while( p_str && *p_str != '\"' )
			{
				pItem->name[n_len++] = (*p_str++);
			}
		}
		
		// from IRA
		BYTE  sz_buf[64];
		memset( sz_buf, 0, sizeof(sz_buf) );
        n_len = gsmString2Bytes( pItem->name, sz_buf, n_len );   // ��ʽת��
		n_len = gsmDecodeUcs2(sz_buf, pItem->name, n_len );      // ת����TP-DU
		pItem->state = 1;

		return true;
	}
	return false;
}

void    gsm_api_decode_mode(const char* p_str)
{
	char sz_buf[32];
	sprintf(sz_buf, "AT+CSCS=\"%s\"\r", p_str);
	__gsm_gprs_send( sz_buf, strlen( sz_buf) );
	g_ctrl.empty_data();
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 50 * i );
		if ( strstr( g_ctrl.ack_buf(), "OK") )
			break;
	}
}


//////////////////////////////////////////////////////////////////////////
//4����13311988888����129����JACK��
bool    gsm_api_pbook_write(PBOOK_ITEM* pItem)
{
	char sz_buf[256];
	BYTE sz_name[64];
	char sz_gsm[128];
	memset( sz_buf, 0, sizeof(sz_buf) );
	memset( sz_name, 0, sizeof(sz_name) );
	memset( sz_gsm, 0, sizeof(sz_gsm) );
	int nLen = gsmEncodeUcs2(pItem->name, sz_name, strlen(pItem->name) );
	gsmBytes2String(sz_name, sz_gsm, nLen );
	sprintf(sz_buf,"AT+CPBW=%d,\"%s\",129,\"%s\" \r", pItem->nIdx,pItem->phone,sz_gsm );

	g_ctrl.empty_data();
	__gsm_gprs_send( sz_buf, strlen( sz_buf) );
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 50 * i );
		if ( strstr( g_ctrl.ack_buf(), "\r\nOK") )
			return true;
		else if ( strstr( g_ctrl.ack_buf(), "\r\nERROR") )
			return false;
	}
	return true;
}

*/

//  AT + CPBW = 4
bool    gsm_api_pbook_del(int nIdx)
{
	char sz_buf[32];
	memset(sz_buf, 0, sizeof(sz_buf));
	sprintf(sz_buf,"AT+CPBW=%d\r", nIdx );
	
	__gsm_gprs_send( sz_buf, strlen( sz_buf) );
	for( int i = 0; i < 20; i++ )
	{
		Sleep( 100 * i );
		if ( strstr( g_ctrl.ack_buf(), "\r\nOK") )
			return true;
		else if ( strstr( g_ctrl.ack_buf(), "\r\n+CME ERROR: ") )
			return false;
	}
	return true;
}
/*
//////////////////////////////////////////////////////////////////////////
// ���Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
bool    gsm_api_sms_send(const char* p_phone_number, const char* p_sms_msg)
{
	SM_PARAM  sms_para;
	//char*     p_str;
	char      pdu_buf[512];         // PDU��
	//char      sz_msg[256];
	char      temp_buf[32];
	
	int       pdu_length;           // PDU������
	unsigned char    smsc_length;   // SMSC������
	bool      b_rst = false;
	
	b_rst = gsm_api_sms_load_sms_param( &sms_para, p_phone_number, p_sms_msg );
	if ( !b_rst ) 
	{
		return false;
	}
	
	// ����PDU����������PDU��
	memset( pdu_buf, 0, sizeof(pdu_buf) );
	pdu_length = gsmEncodePdu( &sms_para, pdu_buf );
	// ��Ctrl-Z����
    strcat(pdu_buf, "\x01a"); 
    gsmString2Bytes(pdu_buf, &smsc_length, 2);    // ȡPDU���е�SMSC��Ϣ����
    smsc_length++;
	// ���ϳ����ֽڱ���

#if 0
	// �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
    sprintf(temp_buf, "%d", pdu_length / 2 - smsc_length);    //��������
	p_str = (char*)gsm_api_at_cmd(ATCMGS, temp_buf);          //���ͷ��Ͷ�������
	g_ctrl.send_msg( p_str, strlen(p_str) );
	//������Է��Ͷ���
	p_str = strstr( g_ctrl.ack_buf(), "\r\n>");
	if ( p_str == 0 )
	{
		return false;
	}
	p_str = (char*)gsm_api_at_cmd(ATNULL,pdu_buf );
	g_ctrl.send_msg( p_str, strlen(p_str) );
	Sleep(500);
#else
	sprintf(temp_buf, "AT+CMGS=%d\r", pdu_length / 2 - smsc_length);
	__gsm_gprs_send( temp_buf, strlen(temp_buf) );
	b_rst = false;
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		if (  g_ctrl.check_ack("\r\n>") )
		{
			b_rst = true;
			break;
		}
	}
	if ( !b_rst ) return false;

	int n_len = strlen( pdu_buf );
	__gsm_gprs_send( pdu_buf, n_len );

// 	+CMGS: 54
// 		
// OK

	EXDBG_TRACE1("Message Length is : %d\n", n_len);
	for( i = 0; i < 10; i++ )
	{
		Sleep( 100 * i );
		
		if ( g_ctrl.check_ack("\r\n+CMS ERROR") != 0 )
			return false;

		if ( g_ctrl.check_ack("\r\n+CMGS:") )
		{
			if ( g_ctrl.check_ack("\r\nOK") )
				return true;
		}
	}
	return true;
#endif	

}
*/

//////////////////////////////////////////////////////////////////////////

unsigned  gsm_api_check_sms_idx( const char* sz_msg )
{
	int n_idx = 0;
	char*  p_str = (char*)sz_msg;
	p_str += 12;   //ʹָ��ָ����ź�
	//������ź�����λ
	if (*(p_str+1) != 13 ) 
	{
		n_idx = (*p_str - '0')*10 + (*(p_str+1)) - '0';
	}
	else	//���ź���1λ
	{
		n_idx = *p_str - '0';
	}
	return n_idx;
}

/*
//��һЩ��ϢԤ�ȷ�����Žṹ����
//pSrc�� ��Ҫд��Ľṹ��	
//strPhoneNum: Ҫ���Ͷ��ŵ�����绰
//strusrinfo:  ��������
//���أ� ����Ƿ�ɹ�
bool   gsm_api_sms_load_sms_param(SM_PARAM* p_src, 
								  const char* str_phone_number, 
								  const char* str_struct_info )
{
//	char   sz_msg[256];
	char*  p_str = 0;
	int    n_len = strlen( str_phone_number );
	
	//��ѯ�������ĺ���
#if 0
	p_str = (char*)gsm_api_at_cmd(ATCSCA,"?");
	g_ctrl.send_msg( p_str, strlen(p_str) );
	//watching_gsm_read_empy(sz_msg, sizeof(sz_msg) );
#else
	char* p_cmd = "AT+CSCA?\r";
	__gsm_gprs_send( p_cmd, strlen(p_cmd) );
	for( int i = 0; i < 10; i++ )
	{
		Sleep( 50 * i );
		if (  g_ctrl.check_ack("+86") )
			break;
	}
#endif
	p_str = strstr( g_ctrl.ack_buf(), "+86");
	if ( 0 == p_str )
	{
		return false;
	}else
	{
		//����õ��������ĺ��룬�Ѹú���д��
		p_str++;
		memcpy( p_src->SCA, p_str, 13);
		p_src->SCA[13] = '\0';
	}
	
	p_src->index = 1;              //���ű��
	p_src->TP_PID = 0;
	if ( n_len == 0 || n_len > 11) 
	{
		return false;		
	}
	
	p_src->TPA[0] = '8';
	p_src->TPA[1] = '6';
	memcpy(&(p_src->TPA[2]), str_phone_number, n_len);
    p_src->TPA[n_len+2] = '\0';
	
	//����Ϣ����
	n_len = strlen(str_struct_info);
	//�����ASSIC,����UCS2��ʽ
	if( sms_is_only_assic(str_struct_info, n_len) )
	{
		p_src->TP_DCS = GSM_7BIT;      //UCS2����
		if ( n_len > 140 ) 	n_len = 140;
	}else{
		p_src->TP_DCS = GSM_UCS2;      //UCS2����
		n_len = sms_ucs2_length(str_struct_info, n_len );
	}
	
	if ( n_len == 0 || n_len > 140 )
	{
		return false;
	}
	
	memcpy( p_src->TP_UD, str_struct_info, n_len );
	p_src->TP_UD[n_len] = '\0';
	return true;
}



// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
// pDst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
    int nLength;             // �ڲ��õĴ�����
    int nDstLength;          // Ŀ��PDU������
    unsigned char buf[256];  // �ڲ��õĻ�����
	
    // SMSC��ַ��Ϣ��
    nLength = strlen(pSrc->SCA);    // SMSC��ַ�ַ����ĳ���
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC��ַ��Ϣ����
    buf[1] = 0x91;        // �̶�: �ù��ʸ�ʽ����
    nDstLength = gsmBytes2String(buf, pDst, 2);        // ת��2���ֽڵ�Ŀ��PDU��
    nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // ת��SMSC��Ŀ��PDU��
	
    // TPDU�λ���������Ŀ���ַ��
    nLength = strlen(pSrc->TPA);    // TP-DA��ַ�ַ����ĳ���
    buf[0] = 0x11;            // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
    buf[1] = 0;               // TP-MR=0
    buf[2] = (char)nLength;   // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
    buf[3] = 0x91;            // �̶�: �ù��ʸ�ʽ����
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);  // ת��4���ֽڵ�Ŀ��PDU��
    nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength); // ת��TP-DA��Ŀ��PDU��
	
    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    nLength = strlen(pSrc->TP_UD);    // �û���Ϣ�ַ����ĳ���
    buf[0] = pSrc->TP_PID;        // Э���ʶ(TP-PID)
    buf[1] = pSrc->TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
    buf[2] = 0;            // ��Ч��(TP-VP)Ϊ5����
    if (pSrc->TP_DCS == GSM_7BIT)
    {
        // 7-bit���뷽ʽ
        buf[3] = nLength;            // ����ǰ����
        nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;    // ת��TP-DA��Ŀ��PDU��
    }
    else if (pSrc->TP_DCS == GSM_UCS2)
    {
        // UCS2���뷽ʽ
        buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);    // ת��TP-DA��Ŀ��PDU��
        nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
    }
    else
    {
        // 8-bit���뷽ʽ
		// buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);    // ת��TP-DA��Ŀ��PDU��
        nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
    }
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);        // ת���ö����ݵ�Ŀ��PDU��
	
    // ����Ŀ���ַ�������
    return nDstLength;
}
*/


// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ������ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ�����ݳ���
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    for (int i = 0; i < nSrcLength; i++)
    {
        // �����4λ
        if(*pSrc>='0' && *pSrc<='9')
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }
		
        pSrc++;
		
        // �����4λ
        if(*pSrc>='0' && *pSrc<='9')
        {
            *pDst |= *pSrc - '0';
        }
        else
        {
            *pDst |= *pSrc - 'A' + 10;
        }        
		pSrc++;
        pDst++;
    }
	*pDst = '\0';
    
    // ����Ŀ�����ݳ���
    return nSrcLength / 2;
}


// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
// pSrc: Դ����ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���ݳ���
// ����: Ŀ���ַ�������
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�
	
    for (int i = 0; i < nSrcLength; i++)
    {
        // �����4λ
        *pDst++ = tab[*pSrc >> 4];
		
        // �����4λ
        *pDst++ = tab[*pSrc & 0x0f];
		
        pSrc++;
    }
	
    // ����ַ����Ӹ�������
    *pDst = '\0';
	
    // ����Ŀ���ַ�������
    return nSrcLength * 2;
}


// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������''F''�ճ�ż��
// �磺"8613851872468" --> "683158812764F8"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ���ַ�������
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;   // Ŀ���ַ�������
    char ch;          // ���ڱ���һ���ַ�
	
    // ���ƴ�����
    nDstLength = nSrcLength;
	
    // �����ߵ�
    for (int i = 0; i < nSrcLength; i += 2)
    {
        ch = *pSrc++;        // �����ȳ��ֵ��ַ�
        *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
        *pDst++ = ch;        // �����ȳ��ֵ��ַ�
    }
	
    // Դ��������������
    if (nSrcLength & 1)
    {
        *(pDst-2) = 'F';     // ��''F''
        nDstLength++;        // Ŀ�괮���ȼ�1
    }
	
    // ����ַ����Ӹ�������
    *pDst = '\0';
	
    // ����Ŀ���ַ�������
    return nDstLength;
}


// 7-bit����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ����봮����
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;        // Դ�ַ����ļ���ֵ
    int nDst;        // Ŀ����봮�ļ���ֵ
    int nChar;       // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
    unsigned char nLeft;    // ��һ�ֽڲ��������
	
    // ����ֵ��ʼ��
    nSrc = 0;
    nDst = 0;
	
    // ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
    // ѭ���ô�����̣�ֱ��Դ����������
    // ������鲻��8�ֽڣ�Ҳ����ȷ����
    while (nSrc < nSrcLength)
    {
        // ȡԴ�ַ����ļ���ֵ�����3λ
        nChar = nSrc & 7;
		
        // ����Դ����ÿ���ֽ�
        if (nChar == 0)
        {
            // ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
            nLeft = *pSrc;
        }
        else
        {
            // ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
            *pDst = (*pSrc << (8 - nChar)) | nLeft;
			
            // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
            nLeft = *pSrc >> nChar;
			
            // �޸�Ŀ�괮��ָ��ͼ���ֵ
            pDst++;
            nDst++;
        }
		
        // �޸�Դ����ָ��ͼ���ֵ
        pSrc++;
        nSrc++;
    }
	
    // ����Ŀ�괮����
    return nDst;
}



// UCS2����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����:Ŀ����봮����
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int        nDstLength; // UNICODE���ַ���Ŀ
    WCHAR      wchar[128]; // UNICODE��������
    
    //�ַ���-->UNICODE��
    nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);
	
    // �ߵ��ֽڶԵ������
    for (int i = 0; i < nDstLength; i++)
    {
        // �������λ�ֽ�
        *pDst++ = wchar[i] >> 8;
		
        // �������λ�ֽ�
        *pDst++ = wchar[i] & 0xff;
    }
	
    // ����Ŀ����봮����
    return nDstLength * 2;
}


bool  sms_is_only_assic(const char* str_sms, int n_len)
{
	for( int i = 0; i < n_len; i++ )
	{
		if ( str_sms[i] > 127 || str_sms[i] < 0 )
			return false;
	}
	return true;
}


int sms_ucs2_length(const char* str_sms, int n_len)
{
	int n_count = 0;
	for(int i = 0; i < n_len; i++ )
	{
		if ( str_sms[i] <= 127 && str_sms[i] >= 0 )
			n_count+=2;
		else
		{
			n_count += 2;
			i++;
		}
		
		if ( n_count > 138 )
		{
			return n_count;
		}
	}
	
	return n_count;
}
/*

// PDU���룬���ڽ��ա��Ķ�����Ϣ
// pSrc: ԴPDU��ָ��
// pDst: Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nTPUDHI = 0;
    int nDstLength;          // Ŀ��PDU������
    unsigned char tmp;       // �ڲ��õ���ʱ�ֽڱ���
    unsigned char buf[512];  // �ڲ��õĻ�����
	memset(buf, 0, sizeof(buf));
    
    // SMSC��ַ��Ϣ��
    gsmString2Bytes(pSrc, &tmp, 2);    // ȡ����
    tmp = (tmp - 1) * 2;    // SMSC���봮����ָ�����
    pSrc += 4;              // 
    gsmSerializeNumbers(pSrc, pDst->SCA, tmp);    // ת��SMSC���뵽Ŀ��PDU��
    pSrc += tmp;        // ָ�����
    
    // TPDU�λ����������ظ���ַ��
    gsmString2Bytes(pSrc, &tmp, 2);    // ȡ��������
    pSrc += 2;        // ָ�����
	if ( tmp & 0x40 )
		nTPUDHI = 1;
    //if(tmp & 0x04 )
    //{
	// �����ظ���ַ��ȡ�ظ���ַ��Ϣ
	gsmString2Bytes(pSrc, &tmp, 2);    // ȡ����
	if(tmp & 1) tmp += 1;    // ������ż��
	pSrc += 4;          // ָ�����
	gsmSerializeNumbers(pSrc, pDst->TPA, tmp);    // ȡTP-RA����
	pSrc += tmp;        // ָ�����
    //}
	//else
	//{
    //     pSrc += 18;
	//}
    
    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // ȡЭ���ʶ(TP-PID)
    pSrc += 2;        // ָ�����
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // ȡ���뷽ʽ(TP-DCS)
    pSrc += 2;        // ָ�����
    gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // ����ʱ����ַ���(TP_SCTS) 
    pSrc += 14;       // ָ�����
    gsmString2Bytes(pSrc, &tmp, 2);    // �û���Ϣ����(TP-UDL)
    pSrc += 2;        // ָ�����
    if(pDst->TP_DCS == GSM_7BIT)
    {
        // 7-bit����
		
		nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4); // ��ʽת��
		if (nTPUDHI)
			gsmDecode7bit(buf+6, pDst->TP_UD, nDstLength-6);    // ת����TP-DU
		else
			gsmDecode7bit(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
		
        nDstLength = tmp;
    }
    else if(pDst->TP_DCS == GSM_UCS2)
    {
        // UCS2����
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);        // ��ʽת��
		if (nTPUDHI)
			nDstLength = gsmDecodeUcs2(buf+6, pDst->TP_UD, nDstLength-6);    // ת����TP-DU
		else
			nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
    }
    else
    {
        // 8-bit����
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);              // ��ʽת��
        //nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
    }
    
    // ����Ŀ���ַ�������
    return nDstLength;
}


*/

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ���ַ�������
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;   // Ŀ���ַ�������
    //char ch;          // ���ڱ���һ���ַ�
    
    // ���ƴ�����
    nDstLength = nSrcLength;
	
    
    // �����ߵ�
	//    for(int i=0; i<nSrcLength; i++)
	//    {
	//        ch = *pSrc++;        // �����ȳ��ֵ��ַ�
	//        *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
	//        *pDst++ = ch;        // �����ȳ��ֵ��ַ�
	//    }
    
    for(int i=0; i<nSrcLength; i=i+2)
	{
		pDst[i] = pSrc[i+1];
		pDst[i+1] = pSrc[i];
	}
	
	
	// �����ַ���'F'��
    if(pDst[nSrcLength-1] == 'F')
	{
        pDst[nDstLength-1] = '\0';
		
	}
    else
	{
		pDst[nDstLength] = '\0';
	}
    // ����Ŀ���ַ�������
    return nDstLength;
}


// 7-bit����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ����봮����
int  gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    int nSrc;        // Դ�ַ����ļ���ֵ
    int nDst;        // Ŀ����봮�ļ���ֵ
    int nByte;       // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
    unsigned char nLeft;    // ��һ�ֽڲ��������
	
    // ����ֵ��ʼ��
    nSrc = 0;
    nDst = 0;
	
    // �����ֽ���źͲ������ݳ�ʼ��
    nByte = 0;
    nLeft = 0;
	
    // ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
    // ѭ���ô�����̣�ֱ��Դ���ݱ�������
    // ������鲻��7�ֽڣ�Ҳ����ȷ����
    while (nSrc < nSrcLength)
    {
        // ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
		
        // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
        nLeft = *pSrc >> (7 - nByte);
		
        // �޸�Ŀ�괮��ָ��ͼ���ֵ
        pDst++;
        nDst++;
		
        // �޸��ֽڼ���ֵ
        nByte++;
		
        // ����һ������һ���ֽ�
        if (nByte == 7)
        {
            // ����õ�һ��Ŀ������ֽ�
            *pDst = nLeft;
			
            // �޸�Ŀ�괮��ָ��ͼ���ֵ
            pDst++;
            nDst++;
			
            // �����ֽ���źͲ������ݳ�ʼ��
            nByte = 0;
            nLeft = 0;
        }
		
        // �޸�Դ����ָ��ͼ���ֵ
        pSrc++;
        nSrc++;
    }
	
    *pDst = 0;
	
    // ����Ŀ�괮����
    return nDst;
}



// UCS2����
// pSrc: Դ���봮ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���봮����
// ����: Ŀ���ַ�������
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int i;
    int nDstLength;        // UNICODE���ַ���Ŀ
    WCHAR wchar[128];      // UNICODE��������
	
    // �ߵ��ֽڶԵ���ƴ��UNICODE
    for (i = 0; i < nSrcLength/2; i++)
    {
        // �ȸ�λ�ֽ�
        wchar[i] = *pSrc++ << 8;
		
        // ���λ�ֽ�
        wchar[i] |= *pSrc++;
		
    }
	wchar[i] = '\0';
	
    
    //UNICODE��-->�ַ���
    nDstLength = WideCharToMultiByte(936, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);
	if ( nDstLength == 0 )
		nDstLength = WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);
	
    pDst[nDstLength] = '\0';
    // ����Ŀ���ַ�������
    return nDstLength;
}

bool gsm_api_get_imsi(char* p_str)
{
	if(NULL == p_str)
		return false;

	bool  b_suc = false;
	char* gsm_cmd = "AT+CIMI\r";                //��ѯGSMģ��IMEI
	
	g_ctrl.send_msg( gsm_cmd, strlen(gsm_cmd) );
	for( int j = 0; j < 10; j++ )
	{
		Sleep( 100 * j );
		b_suc = g_ctrl.check_ack( "\r\nOK\r\n" );
		if ( b_suc )
			break;
	}
	
	if ( !b_suc )
	{
		EXDBG_TRACE1("CMD:[%s] ack error!\n", gsm_cmd );
		return false;
	}
	else
	{
		char* p_ack = strstr( g_ctrl.ack_buf(),"\r\n");
		if ( p_ack )
		{
			p_ack += 2;
			memcpy( p_str, p_ack, 15 );
		}
	}
	
	g_ctrl.empty_data();
	
	return true;
}

bool gsm_api_get_imei(char* p_str)
{
	if(NULL == p_str)
		return false;

	bool  b_suc = false;
	char* gsm_cmd = "AT+GSN\r";                //��ѯGSMģ��IMEI
	
	g_ctrl.send_msg( gsm_cmd, strlen(gsm_cmd) );
	for( int j = 0; j < 10; j++ )
	{
		Sleep( 100 * j );
		b_suc = g_ctrl.check_ack( "\r\nOK\r\n" );
		if ( b_suc )
			break;
	}
	
	if ( !b_suc )
	{
		EXDBG_TRACE1("CMD:[%s] ack error!\n", gsm_cmd );
		return false;
	}
	else
	{
		char* p_ack = strstr( g_ctrl.ack_buf(),"\r\n");
		if ( p_ack )
		{
			p_ack += 2;
			memcpy( p_str, p_ack, 15 );
		}
	}
	
	g_ctrl.empty_data();
	
	return true;
}

//��ѯ���������״̬
bool gsm_api_get_connectstatus()
{
	bool  b_suc = false;
	//char* gsm_cmd = "AT+CIPSTATUS\r";                //��ѯ���������״̬
	char* gsm_cmd = "AT+CIPSTATUS\r";                //��ѯ���������״̬
	
	g_ctrl.send_msg( gsm_cmd, strlen(gsm_cmd) );
	for( int j = 0; j < 10; j++ )
	{
		Sleep( 100 * j );
		b_suc = g_ctrl.check_ack( "\r\nOK\r\n" );
		if ( b_suc )
			break;
	}
	
	if ( !b_suc )
	{
		EXDBG_TRACE1("CMD:[%s] ack error!\n", gsm_cmd );
		return false;
	}
	else
	{
//		char* p_ack = strstr( g_ctrl.ack_buf(),"\r\n");
//		if ( p_ack )
//		{
//			p_ack += 2;
//			memcpy( p_str, p_ack, 15 );
//		}
	}
	
	g_ctrl.empty_data();
	
	return true;
}