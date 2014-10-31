// gsm_parse.cpp: implementation of the gsm_parse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scom_parse.h"
#include "srv_gps.h"
//#include "msg_imm.h"
#include "ex_debug.h"
#include "msg_data.h"
#include "scom_api.h"
#include "rectrl_api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
stpos_request scom_parse::m_st_pos_request;
stfht_param          m_fht_param;
st_sms	g_stSMS;

extern msg_ipc*        g_pIPC;
//是否在读取电话号码薄
BOOL   g_bReadPhoneBooking = FALSE;

scom_parse::scom_parse()
{
}

scom_parse::~scom_parse()
{
}

void   scom_parse::msg_notify(unsigned int n_type, unsigned int n_param, bool b_sync /* =false */)
{
	if ( b_sync )
		::SendMessage( m_notify_wnd, MSG_GSM_NOTIFY, n_type, n_param );
	else
		::PostMessage( m_notify_wnd, MSG_GSM_NOTIFY, n_type, n_param );
}

//////////////////////////////////////////////////////////////////////////

void   scom_parse::parse(const char* p_buf, int n_size )
{
	msg_data* p_data =  msg_data::instance();

	return;
}



void  scom_parse::parse_ack( const char* p_buf , int n_len)
{
}

int  scom_parse::parse_null(const char* p_buf, int n_len)
{
	return parse_svr_param( p_buf,n_len );
}


bool   scom_parse::fht_ack( const char* p_buf, int n_size )
{
	msg_cunion msg_ack(0,0);
	msg_ack.set_ack_msg( p_buf, n_size );
	int nType = msg_ack.fth_ack_parse( p_buf, n_size, &m_fht_param );
	if ( nType != fth_svr_null )
		msg_notify( nType, (unsigned int)&m_fht_param, false );
	return true;
}

//////////////////////////////////////////////////////////////////////////

int  scom_parse::parse_svr_param( const char* p_buf, int n_len )
{
	fht_ack( p_buf, n_len );
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void  scom_parse::parse_sim(const char* p_buf)
{
	char* p_ack = "\r\n+CPIN: READY";
	if ( strstr( p_buf, p_ack ) != 0 )
		msg_notify(eGSM_SIM_CARD,1,false);
	else
		msg_notify(eGSM_SIM_CARD,0,false);
}

//////////////////////////////////////////////////////////////////////////

void  scom_parse::parse_register_gsm(const char* p_buf)
{
	return;
	if ( strstr(p_buf, "\r\n+CREG: 0,1") != 0 )
		msg_notify(escom_REGISTER_GSM,1);
	
	else if ( strstr(p_buf,"\r\n+CREG: 0,5") != 0 )
		msg_notify(eGSM_REGISTER_GSM,1);
	
	else if ( strstr(p_buf, "\r\n+CREG: 0,2" ) != 0 )
		msg_notify(eGSM_REGISTER_GSM,0);
	
	else
		msg_notify(eGSM_REGISTER_GSM,0);
}

void  scom_parse::parse_register_gprs(const char* p_buf)
{
	if ( strstr( p_buf, "\r\n+CGATT: 1") != 0 )
		msg_notify(eGSM_REGISTER_GPRS,1);
	else if ( strstr( p_buf, "\r\n+CGATT: 0") != 0 )
		msg_notify(eGSM_REGISTER_GSM,1);
	else
		msg_notify(eGSM_REGISTER_GSM,0);
}

//////////////////////////////////////////////////////////////////////////

void  scom_parse::parse_connect_ip(const char* p_buf)
{
	if ( strstr(p_buf, "\r\n0,TCP CONNECT OK") != 0 )
	{
		msg_notify(eGSM_CONNECT_IP,1);	
	}
	else if ( strstr(p_buf, "\r\n0,CONNECT EXIST") != 0 ||
		      strstr(p_buf, "\r\nCONNECT FAIL" ) != 0 )
	{
		msg_notify(eGSM_CONNECT_IP, 0 );
	}
}

//调度信息处理
void scom_parse::parse_Dispatch_msg(const char* p_buf, int n_len)
{//+IPD45:*SHZB05,1FW,2A,03,~AA000004F60597D0031003242#
	if (NULL == p_buf || n_len <= 0 )
		return;

	char *pTemp, *pNext;
	pTemp = strstr(p_buf, "+IPD");
	if(pTemp == NULL)
		return;
	pTemp += 4;
	pTemp = strstr(pTemp, ":*");
	if(pTemp == NULL)
		return;
	pTemp += 2;
	pTemp = strstr(pTemp, ",");
	if(pTemp == NULL)
		return;
	pTemp++;
	pTemp = strstr(pTemp, ",");
	if(pTemp == NULL)
		return;
	pTemp++;
	pTemp = strstr(pTemp, ",");
	if(pTemp == NULL)
		return;
	pTemp++;
	pTemp = strstr(pTemp, ",~");
	if(pTemp == NULL)
		return;
	pTemp += 2;	//pTemp -> AA000004F60#
	if(pTemp[0] < 'A' || pTemp[0] > 'E')	//信息类型
		return;
	if(pTemp[1] < 'A' || pTemp[1] > 'B')	//语音类型
		return;
	
	pNext = strstr(pTemp, "#");
	if(pNext == NULL)
		return;
	WORD	nOffset = pNext-pTemp;
	if(nOffset < 9)	//不足 AA00000#  例:pTemp->AA000004F60#,nOffset为13
		return;

	//st_dispatch stDispatch;
	//st_sms stSMS;
	memset(&g_stSMS, 0, sizeof(st_dispatch));
	//stDispatch.ucSMSType	= pTemp[0];	//信息类型
	//stDispatch.ucVoiceType	= pTemp[1];	//语音类型
	
	UCHAR ucData[512], ucMsg[128];
	memset(ucData, 0, sizeof(ucData));
	memset(ucMsg, 0, sizeof(ucMsg));
	nOffset -= 9;	//AA00000 4F60 #,  nOffset为4
	memcpy(ucData, &pTemp[7], nOffset);	
	
	for (int j = 0; j < nOffset; j++)
	{
		if (ucData[j] >= 0x30 && ucData[j] <= 0x39)
		{//0x01 -> '1'(0x31)
			ucData[j] -= 0x30;	
		} 
		else if (ucData[j] >= 0x41 && ucData[j] <= 0x46)
		{//0x0A -> 'A'(0x41)
			ucData[j] -= 0x37;
		} 
	}
	j = 0;
	for(int i=0; i<nOffset; i++)	//shenlin
	{//例：0xFE(pSrc[i])，拆分后字符为：'F'(0x46) 'E'(0x45)
		ucMsg[j]	= ucData[i++];
		ucMsg[j++]	= (ucMsg[j] << 4) + ucData[i];
	}
	TCHAR tcMsg[128];
	memset(tcMsg, 0, sizeof(tcMsg));
	int k = 0;
	for(i=0; i<(j/2); i++)	//shenlin
	{//例：0xFE(pSrc[i])，拆分后字符为：'F'(0x46) 'E'(0x45)
		tcMsg[i]	= ucMsg[k++];
		tcMsg[i]	= (tcMsg[i] << 8) + ucMsg[k++];
	}

	CString str = _T("调度信息");
	WideCharToMultiByte(CP_ACP, 0, str.GetBuffer(str.GetLength()), str.GetLength(), (char*)g_stSMS.m_number, str.GetLength()*2, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tcMsg, i, (char*)g_stSMS.m_context, k, NULL, NULL);

	char chTemp[12];
	memset(chTemp, 0, sizeof(chTemp));
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(chTemp, "%d",time.wYear);
	memcpy(g_stSMS.m_time, &chTemp[2], 2);
	
	memset(chTemp, 0, sizeof(chTemp));
	sprintf(chTemp, "%02d",time.wMonth);
	memcpy(&g_stSMS.m_time[2], chTemp, 2);
	
	memset(chTemp, 0, sizeof(chTemp));
	sprintf(chTemp, "%02d",time.wDay);
	memcpy(&g_stSMS.m_time[4], chTemp, 2);
	
	memset(chTemp, 0, sizeof(chTemp));
	sprintf(chTemp, "%02d",time.wHour);
	memcpy(&g_stSMS.m_time[6], chTemp, 2);
	
	memset(chTemp, 0, sizeof(chTemp));
	sprintf(chTemp, "%02d",time.wMinute);
	memcpy(&g_stSMS.m_time[8], chTemp, 2);
	//发送调度信息
	CreateThread(NULL, 0, THDSendMSG, (LPVOID)&g_stSMS, 0, NULL);

	//递归调用
	parse_Dispatch_msg(pNext, strlen(pNext));
}
void   scom_parse::msg_report2ui(unsigned n_process, unsigned n_rst, bool b_sync /* = false*/ )
{
	msg_notify( n_process, n_rst, b_sync );
}
