
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
		//"AT+CGMR\r",               //查询模块的版本
		"ATE0\r",                  //关闭回显命令

#ifdef DEBUG
		"AT+CMEE=2\r",             //URC的错误提示
		"AT+CGMR\r",               //查询模块的版本
#endif
		"AT+GSN\r",                //查询GSM模块IMEI
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
			"AT+COLP=1\r",             //对方摘机通知
			"AT+CLIP=1\r",             //来电通知
			"AT+CNMI=2,1,0,0,0\r",     //短消息通知,存储在SIM卡上
			"AT+CMGF=0\r",             //PDU模式
			"AT+CIMI\r",
			"AT+CIPFLP=0\r",
			"AT+CRSL=95\r",            //设置来电振铃声音
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
		//AT+CSTT               // 设置APN、用户名、密码
		//AT+CIICR              // 激活移动场景
		//AT+CIFSR              // 获得本地IP
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
		//@@@ -- 时间上面可能更长一点
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



//删除一条短信
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


// 读取指定的短信
// msg_type: 短信类型  0: UNREAD 1: READ 2： UNSENT  3: SENT  4: ALL
// p_msg: 解析出来的短信存放在这里
// idx_msg: 要读取的短消息的序号。
// msg_type: 短消息的操作类型 
// 返回: 收到的短信息条数
/*
int     gsm_api_sms_read(int idx_msg, unsigned msg_type, SM_PARAM* p_msg)
{
	int    i, n_msg = 0;           // 短消息计数值
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
		p_str = strstr(p_str,"\r\n");             // 找下一行
		p_str += 2;                               // 跳过"\r\n"
		gsmDecodePdu(p_str, p_msg);               // PDU串解码
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
			p_str = strstr(p_str,"\r\n");             // 找下一行
			p_str += 2;                               // 跳过"\r\n"
			gsmDecodePdu(p_str, p_msg);               // PDU串解码
			n_msg++;                                  // 短消息计数加1
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
	//AT + CPBS = “SM”        // 指定要读取电话本的存储位置为SIM卡
    //AT + CPBR = 1，3          // 读取存储在SIM卡中记录号为1~3电话本记录
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
		//电话号码
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
        n_len = gsmString2Bytes( pItem->name, sz_buf, n_len );   // 格式转换
		n_len = gsmDecodeUcs2(sz_buf, pItem->name, n_len );      // 转换到TP-DU
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
//4，“13311988888”，129，“JACK”
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
// 发送短消息
// pSrc: 源PDU参数指针
bool    gsm_api_sms_send(const char* p_phone_number, const char* p_sms_msg)
{
	SM_PARAM  sms_para;
	//char*     p_str;
	char      pdu_buf[512];         // PDU串
	//char      sz_msg[256];
	char      temp_buf[32];
	
	int       pdu_length;           // PDU串长度
	unsigned char    smsc_length;   // SMSC串长度
	bool      b_rst = false;
	
	b_rst = gsm_api_sms_load_sms_param( &sms_para, p_phone_number, p_sms_msg );
	if ( !b_rst ) 
	{
		return false;
	}
	
	// 根据PDU参数，编码PDU串
	memset( pdu_buf, 0, sizeof(pdu_buf) );
	pdu_length = gsmEncodePdu( &sms_para, pdu_buf );
	// 以Ctrl-Z结束
    strcat(pdu_buf, "\x01a"); 
    gsmString2Bytes(pdu_buf, &smsc_length, 2);    // 取PDU串中的SMSC信息长度
    smsc_length++;
	// 加上长度字节本身

#if 0
	// 命令中的长度，不包括SMSC信息长度，以数据字节计
    sprintf(temp_buf, "%d", pdu_length / 2 - smsc_length);    //生成命令
	p_str = (char*)gsm_api_at_cmd(ATCMGS, temp_buf);          //发送发送短信命令
	g_ctrl.send_msg( p_str, strlen(p_str) );
	//假如可以发送短信
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
	p_str += 12;   //使指针指向短信号
	//假如短信号是两位
	if (*(p_str+1) != 13 ) 
	{
		n_idx = (*p_str - '0')*10 + (*(p_str+1)) - '0';
	}
	else	//短信号是1位
	{
		n_idx = *p_str - '0';
	}
	return n_idx;
}

/*
//把一些信息预先放入短信结构体中
//pSrc： 将要写入的结构体	
//strPhoneNum: 要发送短信到这个电话
//strusrinfo:  短信内容
//返回： 填充是否成功
bool   gsm_api_sms_load_sms_param(SM_PARAM* p_src, 
								  const char* str_phone_number, 
								  const char* str_struct_info )
{
//	char   sz_msg[256];
	char*  p_str = 0;
	int    n_len = strlen( str_phone_number );
	
	//查询服务中心号码
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
		//如果得到短信中心号码，把该号码写入
		p_str++;
		memcpy( p_src->SCA, p_str, 13);
		p_src->SCA[13] = '\0';
	}
	
	p_src->index = 1;              //短信编号
	p_src->TP_PID = 0;
	if ( n_len == 0 || n_len > 11) 
	{
		return false;		
	}
	
	p_src->TPA[0] = '8';
	p_src->TPA[1] = '6';
	memcpy(&(p_src->TPA[2]), str_phone_number, n_len);
    p_src->TPA[n_len+2] = '\0';
	
	//短消息长度
	n_len = strlen(str_struct_info);
	//如果是ASSIC,则不用UCS2格式
	if( sms_is_only_assic(str_struct_info, n_len) )
	{
		p_src->TP_DCS = GSM_7BIT;      //UCS2编码
		if ( n_len > 140 ) 	n_len = 140;
	}else{
		p_src->TP_DCS = GSM_UCS2;      //UCS2编码
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



// PDU编码，用于编制、发送短消息
// pSrc: 源PDU参数指针
// pDst: 目标PDU串指针
// 返回: 目标PDU串长度
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
    int nLength;             // 内部用的串长度
    int nDstLength;          // 目标PDU串长度
    unsigned char buf[256];  // 内部用的缓冲区
	
    // SMSC地址信息段
    nLength = strlen(pSrc->SCA);    // SMSC地址字符串的长度
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC地址信息长度
    buf[1] = 0x91;        // 固定: 用国际格式号码
    nDstLength = gsmBytes2String(buf, pDst, 2);        // 转换2个字节到目标PDU串
    nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // 转换SMSC到目标PDU串
	
    // TPDU段基本参数、目标地址等
    nLength = strlen(pSrc->TPA);    // TP-DA地址字符串的长度
    buf[0] = 0x11;            // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
    buf[1] = 0;               // TP-MR=0
    buf[2] = (char)nLength;   // 目标地址数字个数(TP-DA地址字符串真实长度)
    buf[3] = 0x91;            // 固定: 用国际格式号码
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);  // 转换4个字节到目标PDU串
    nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength); // 转换TP-DA到目标PDU串
	
    // TPDU段协议标识、编码方式、用户信息等
    nLength = strlen(pSrc->TP_UD);    // 用户信息字符串的长度
    buf[0] = pSrc->TP_PID;        // 协议标识(TP-PID)
    buf[1] = pSrc->TP_DCS;        // 用户信息编码方式(TP-DCS)
    buf[2] = 0;            // 有效期(TP-VP)为5分钟
    if (pSrc->TP_DCS == GSM_7BIT)
    {
        // 7-bit编码方式
        buf[3] = nLength;            // 编码前长度
        nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;    // 转换TP-DA到目标PDU串
    }
    else if (pSrc->TP_DCS == GSM_UCS2)
    {
        // UCS2编码方式
        buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);    // 转换TP-DA到目标PDU串
        nLength = buf[3] + 4;        // nLength等于该段数据长度
    }
    else
    {
        // 8-bit编码方式
		// buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);    // 转换TP-DA到目标PDU串
        nLength = buf[3] + 4;        // nLength等于该段数据长度
    }
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);        // 转换该段数据到目标PDU串
	
    // 返回目标字符串长度
    return nDstLength;
}
*/


// 可打印字符串转换为字节数据
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// pSrc: 源字符串指针
// pDst: 目标数据指针
// nSrcLength: 源字符串长度
// 返回: 目标数据长度
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    for (int i = 0; i < nSrcLength; i++)
    {
        // 输出高4位
        if(*pSrc>='0' && *pSrc<='9')
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }
		
        pSrc++;
		
        // 输出低4位
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
    
    // 返回目标数据长度
    return nSrcLength / 2;
}


// 字节数据转换为可打印字符串
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
// pSrc: 源数据指针
// pDst: 目标字符串指针
// nSrcLength: 源数据长度
// 返回: 目标字符串长度
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表
	
    for (int i = 0; i < nSrcLength; i++)
    {
        // 输出高4位
        *pDst++ = tab[*pSrc >> 4];
		
        // 输出低4位
        *pDst++ = tab[*pSrc & 0x0f];
		
        pSrc++;
    }
	
    // 输出字符串加个结束符
    *pDst = '\0';
	
    // 返回目标字符串长度
    return nSrcLength * 2;
}


// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补''F''凑成偶数
// 如："8613851872468" --> "683158812764F8"
// pSrc: 源字符串指针
// pDst: 目标字符串指针
// nSrcLength: 源字符串长度
// 返回: 目标字符串长度
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;   // 目标字符串长度
    char ch;          // 用于保存一个字符
	
    // 复制串长度
    nDstLength = nSrcLength;
	
    // 两两颠倒
    for (int i = 0; i < nSrcLength; i += 2)
    {
        ch = *pSrc++;        // 保存先出现的字符
        *pDst++ = *pSrc++;   // 复制后出现的字符
        *pDst++ = ch;        // 复制先出现的字符
    }
	
    // 源串长度是奇数吗？
    if (nSrcLength & 1)
    {
        *(pDst-2) = 'F';     // 补''F''
        nDstLength++;        // 目标串长度加1
    }
	
    // 输出字符串加个结束符
    *pDst = '\0';
	
    // 返回目标字符串长度
    return nDstLength;
}


// 7-bit编码
// pSrc: 源字符串指针
// pDst: 目标编码串指针
// nSrcLength: 源字符串长度
// 返回: 目标编码串长度
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标编码串的计数值
    int nChar;       // 当前正在处理的组内字符字节的序号，范围是0-7
    unsigned char nLeft;    // 上一字节残余的数据
	
    // 计数值初始化
    nSrc = 0;
    nDst = 0;
	
    // 将源串每8个字节分为一组，压缩成7个字节
    // 循环该处理过程，直至源串被处理完
    // 如果分组不到8字节，也能正确处理
    while (nSrc < nSrcLength)
    {
        // 取源字符串的计数值的最低3位
        nChar = nSrc & 7;
		
        // 处理源串的每个字节
        if (nChar == 0)
        {
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用
            nLeft = *pSrc;
        }
        else
        {
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
            *pDst = (*pSrc << (8 - nChar)) | nLeft;
			
            // 将该字节剩下的左边部分，作为残余数据保存起来
            nLeft = *pSrc >> nChar;
			
            // 修改目标串的指针和计数值
            pDst++;
            nDst++;
        }
		
        // 修改源串的指针和计数值
        pSrc++;
        nSrc++;
    }
	
    // 返回目标串长度
    return nDst;
}



// UCS2编码
// pSrc: 源字符串指针
// pDst: 目标编码串指针
// nSrcLength: 源字符串长度
// 返回:目标编码串长度
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int        nDstLength; // UNICODE宽字符数目
    WCHAR      wchar[128]; // UNICODE串缓冲区
    
    //字符串-->UNICODE串
    nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);
	
    // 高低字节对调，输出
    for (int i = 0; i < nDstLength; i++)
    {
        // 先输出高位字节
        *pDst++ = wchar[i] >> 8;
		
        // 后输出低位字节
        *pDst++ = wchar[i] & 0xff;
    }
	
    // 返回目标编码串长度
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

// PDU解码，用于接收、阅读短消息
// pSrc: 源PDU串指针
// pDst: 目标PDU参数指针
// 返回: 用户信息串长度
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nTPUDHI = 0;
    int nDstLength;          // 目标PDU串长度
    unsigned char tmp;       // 内部用的临时字节变量
    unsigned char buf[512];  // 内部用的缓冲区
	memset(buf, 0, sizeof(buf));
    
    // SMSC地址信息段
    gsmString2Bytes(pSrc, &tmp, 2);    // 取长度
    tmp = (tmp - 1) * 2;    // SMSC号码串长度指针后移
    pSrc += 4;              // 
    gsmSerializeNumbers(pSrc, pDst->SCA, tmp);    // 转换SMSC号码到目标PDU串
    pSrc += tmp;        // 指针后移
    
    // TPDU段基本参数、回复地址等
    gsmString2Bytes(pSrc, &tmp, 2);    // 取基本参数
    pSrc += 2;        // 指针后移
	if ( tmp & 0x40 )
		nTPUDHI = 1;
    //if(tmp & 0x04 )
    //{
	// 包含回复地址，取回复地址信息
	gsmString2Bytes(pSrc, &tmp, 2);    // 取长度
	if(tmp & 1) tmp += 1;    // 调整奇偶性
	pSrc += 4;          // 指针后移
	gsmSerializeNumbers(pSrc, pDst->TPA, tmp);    // 取TP-RA号码
	pSrc += tmp;        // 指针后移
    //}
	//else
	//{
    //     pSrc += 18;
	//}
    
    // TPDU段协议标识、编码方式、用户信息等
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // 取协议标识(TP-PID)
    pSrc += 2;        // 指针后移
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // 取编码方式(TP-DCS)
    pSrc += 2;        // 指针后移
    gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // 服务时间戳字符串(TP_SCTS) 
    pSrc += 14;       // 指针后移
    gsmString2Bytes(pSrc, &tmp, 2);    // 用户信息长度(TP-UDL)
    pSrc += 2;        // 指针后移
    if(pDst->TP_DCS == GSM_7BIT)
    {
        // 7-bit解码
		
		nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4); // 格式转换
		if (nTPUDHI)
			gsmDecode7bit(buf+6, pDst->TP_UD, nDstLength-6);    // 转换到TP-DU
		else
			gsmDecode7bit(buf, pDst->TP_UD, nDstLength);    // 转换到TP-DU
		
        nDstLength = tmp;
    }
    else if(pDst->TP_DCS == GSM_UCS2)
    {
        // UCS2解码
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);        // 格式转换
		if (nTPUDHI)
			nDstLength = gsmDecodeUcs2(buf+6, pDst->TP_UD, nDstLength-6);    // 转换到TP-DU
		else
			nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength);    // 转换到TP-DU
    }
    else
    {
        // 8-bit解码
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);              // 格式转换
        //nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength);    // 转换到TP-DU
    }
    
    // 返回目标字符串长度
    return nDstLength;
}


*/

// 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"
// pSrc: 源字符串指针
// pDst: 目标字符串指针
// nSrcLength: 源字符串长度
// 返回: 目标字符串长度
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;   // 目标字符串长度
    //char ch;          // 用于保存一个字符
    
    // 复制串长度
    nDstLength = nSrcLength;
	
    
    // 两两颠倒
	//    for(int i=0; i<nSrcLength; i++)
	//    {
	//        ch = *pSrc++;        // 保存先出现的字符
	//        *pDst++ = *pSrc++;   // 复制后出现的字符
	//        *pDst++ = ch;        // 复制先出现的字符
	//    }
    
    for(int i=0; i<nSrcLength; i=i+2)
	{
		pDst[i] = pSrc[i+1];
		pDst[i+1] = pSrc[i];
	}
	
	
	// 最后的字符是'F'吗？
    if(pDst[nSrcLength-1] == 'F')
	{
        pDst[nDstLength-1] = '\0';
		
	}
    else
	{
		pDst[nDstLength] = '\0';
	}
    // 返回目标字符串长度
    return nDstLength;
}


// 7-bit编码
// pSrc: 源字符串指针
// pDst: 目标编码串指针
// nSrcLength: 源字符串长度
// 返回: 目标编码串长度
int  gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标解码串的计数值
    int nByte;       // 当前正在处理的组内字节的序号，范围是0-6
    unsigned char nLeft;    // 上一字节残余的数据
	
    // 计数值初始化
    nSrc = 0;
    nDst = 0;
	
    // 组内字节序号和残余数据初始化
    nByte = 0;
    nLeft = 0;
	
    // 将源数据每7个字节分为一组，解压缩成8个字节
    // 循环该处理过程，直至源数据被处理完
    // 如果分组不到7字节，也能正确处理
    while (nSrc < nSrcLength)
    {
        // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
		
        // 将该字节剩下的左边部分，作为残余数据保存起来
        nLeft = *pSrc >> (7 - nByte);
		
        // 修改目标串的指针和计数值
        pDst++;
        nDst++;
		
        // 修改字节计数值
        nByte++;
		
        // 到了一组的最后一个字节
        if (nByte == 7)
        {
            // 额外得到一个目标解码字节
            *pDst = nLeft;
			
            // 修改目标串的指针和计数值
            pDst++;
            nDst++;
			
            // 组内字节序号和残余数据初始化
            nByte = 0;
            nLeft = 0;
        }
		
        // 修改源串的指针和计数值
        pSrc++;
        nSrc++;
    }
	
    *pDst = 0;
	
    // 返回目标串长度
    return nDst;
}



// UCS2解码
// pSrc: 源编码串指针
// pDst: 目标字符串指针
// nSrcLength: 源编码串长度
// 返回: 目标字符串长度
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int i;
    int nDstLength;        // UNICODE宽字符数目
    WCHAR wchar[128];      // UNICODE串缓冲区
	
    // 高低字节对调，拼成UNICODE
    for (i = 0; i < nSrcLength/2; i++)
    {
        // 先高位字节
        wchar[i] = *pSrc++ << 8;
		
        // 后低位字节
        wchar[i] |= *pSrc++;
		
    }
	wchar[i] = '\0';
	
    
    //UNICODE串-->字符串
    nDstLength = WideCharToMultiByte(936, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);
	if ( nDstLength == 0 )
		nDstLength = WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);
	
    pDst[nDstLength] = '\0';
    // 返回目标字符串长度
    return nDstLength;
}

bool gsm_api_get_imsi(char* p_str)
{
	if(NULL == p_str)
		return false;

	bool  b_suc = false;
	char* gsm_cmd = "AT+CIMI\r";                //查询GSM模块IMEI
	
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
	char* gsm_cmd = "AT+GSN\r";                //查询GSM模块IMEI
	
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

//查询网络的连接状态
bool gsm_api_get_connectstatus()
{
	bool  b_suc = false;
	//char* gsm_cmd = "AT+CIPSTATUS\r";                //查询网络的连接状态
	char* gsm_cmd = "AT+CIPSTATUS\r";                //查询网络的连接状态
	
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