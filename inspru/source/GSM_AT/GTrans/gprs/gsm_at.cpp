
#include "stdafx.h"
#include "gsm_at.h"
#include <stdio.h>
#include <windows.h>
//////////////////////////////////////////////////////////////////////////

static char   g_at_cmd[4*1024];
static const char* p_cmd_list[] = 
{
	"at+cmgf=1\r\0",     // CMD_AT_TEXT_ENCODE
	"ate0\r\0",          // 'ate0\r'
	"AT+CPIN?\r\0",      // CMD_QUERY_SIM
	"AT+CLIP=1\r\0",     // ������ʾ
	"AT+CSQ\r\0",        //	CMD_AT_QSP
	
	
	// ACK result
	"OK\r\n\0",           // CMD_ACK_STR_OK
	"+CPIN: READY\0",     // ACK for 
	"+CLCC:\0",           // CDM_ACK_CLCC  

		// ack for calling 
	"NO ANSWER\0",          //CMD_ACK_NO_ANSWER, �û�û�н���
	"NO DIALTONE\0",        //CMD_ACK_CALL_NO_DIALTONE,  
	"NO CARRIER\0",         //CMD_ACK_CALL_NO_CARRIER,   
	"Disconnecting\0",      //CMD_ACK_CALL_DISCONNECT, ����ʱ,���ڽ�ͨ��Ҷ��д���Ϣ
	"RING\r\n",             //CMD_ACK_CALL_ERROR,    
	
	"+CSQ:",                // +CSQ: 16,0 ;��ѯGSM�źŵ�ACK
	"\0"
};

//���еķ���
// BUSY �Է�æ���߶Է�ȡ������
// +COLP: "13564821116",129(145) OK ����ժ��
// NO CARRIER �Է��һ�
// NO ANSWER ���˽���

const char*  str_null                = "";
const char*  str_atcmgd              = "AT+CMGD=";
const char*  str_atcmgf              = "AT+CMGF=";
const char*  str_atcmgl              = "AT+CMGL=";
const char*  str_atcmgr              = "AT+CMGR=";
const char*  str_atcmgs              = "AT+CMGS=";
const char*  str_atcmgw              = "AT+CMGW=";
const char*  str_atcmss              = "AT+CMSS=";
const char*  str_atcmgc              = "AT+CMGC=";
const char*  str_atcnmi              = "AT+CNMI=";
const char*  str_atcpms              = "AT+CPMS=";
const char*  str_atcres              = "AT+CRES=";
const char*  str_atcsas              = "AT+CSAS=";
const char*  str_atcsca              = "AT+CSCA";
const char*  str_atcscb              = "AT+CSCB=";
const char*  str_atcsdh              = "AT+CSDH=";
const char*  str_atcsmp              = "AT+CSMP=";
const char*  str_atcsms              = "AT+CSMS=";
const char*  str_atsmalphaid         = "AT+SMALPHAID=";
const char*  str_atsmextrainfo       = "AT+SMEXTRAINFO=";
const char*  str_atsmextraunsol      = "AT+SMEXTRAUNSOL=";
const char*  str_ate                 = "ATE";




//�й�GPRS��AT����

const char*  str_cipstart             = "AT+CIPSTART=";    //����TCP/IP ���񣬺�Զ�̵ķ���������   
const char*  str_cstt                 = "AT+CSTT=";        //����GPRS���Ӳ��� 
const char*  str_cctt                 = "AT+CCTT";         //�ر�TCP/IP����
const char*  str_ciicr                = "AT+CIICR";        //�����ƶ�����
const char*  str_cipshut              = "AT+CIPSHUT";      //�ر��ƶ�����
const char*  str_cipc                 = "AT+CIPC=";        //��Զ�̷�������������
const char*  str_cipclose             = "AT+CIPCLOSE";     //�ر���Զ�̷���������������
const char*  str_cipsend              = "AT+CIPSEND";      //��Զ�̷�������������
const char*  str_cping                = "AT+CPING=";       //pingԶ��IP��ַ
const char*  str_cifsr                = "AT+CIFSR";        //�鿴����IP��ַ�Ͷ˿ں�
const char*  str_clport               = "AT+CLPORT=";      //����TCP/UDP�ı��ض˿�
const char*  str_cipflp               = "AT+CIPFLP=";      //�����Ƿ�̶����ض˿�
const char*  str_cipsrip              = "AT+CIPSRIP=";     //�����Ƿ��ڷ�����Ϣ����ʾԶ�̻�����IP��ַ     
const char*  str_cipdpdp              = "AT+CIPDPDP=";     //�����Զ������Զ����������ʱ����
const char*  str_cipstatus            = "AT+CIPSTATUS";    //��ѯ���������״̬
const char*  str_crestart             = "AT+CRESTART";     //���½���TCP/UDP����
const char*  str_cipact               = "AT+CIPACT=";      //����һ������
const char*  str_cipcn                = "AT+CIPCN";        //�õ���ǰ�ɹ����ӵ���Ŀ
const char*  str_crcfm                = "AT+CRCFM=";       //����TCP/IP AT����ķ��ظ�ʽ
const char*  str_cipret               = "AT+CIPRET";       //���TCP/IP��Ϣ


//��������ͨ����ATָ��
const char*  str_ata                  = "ATA";            //ժ�� Ӧ��绰
const char*  str_atd                  = "ATD";            //���Ż��߷��ʹ���
const char*  str_atw                  = "AT&W";
const char*  str_atvts                = "AT+VTS=";
const char*  str_atdl                 = "ATDL";           //�ز�
const char*  str_ath                  = "AT+CHUP";        //�һ�
const char*  str_atm                  = "ATM";            //�����������Ĺ���ģʽ
const char*  str_atp                  = "ATP";            //�������岦��
const char*  str_atq                  = "ATQ";            //Set result code presentation mode
const char*  str_ats0                 = "ATS0";            //Set number of rings before automatically answering the call
const char*  str_atcr                   = "AT+CR=";
const char*  str_atcrc                  = "AT+CRC=";
const char*  str_atclcc                 = "AT+CLCC";        //�õ���ǰ��ͨ�ź���

const char*  str_atclvl               = "AT+CLVL=";        //�������� (1-100)
const char*  str_atclip               = "AT+CLIP=";        //����������ʾ
const char*  str_atcolp               = "AT+COLP=";        //Connected Line Identification Presentation


const char*  ATCMDINFO[] = {
	//GSMָ��
	str_null, str_atcmgd, str_atcmgf, str_atcmgl, str_atcmgr, 
	str_atcmgs, str_atcmgw, str_atcmss, str_atcmgc, str_atcnmi, 
	str_atcpms, str_atcres, str_atcsas, str_atcsca, str_atcscb, 
	str_atcsdh, str_atcsmp, str_atcsms, str_atsmalphaid, str_atsmextrainfo, 
	str_atsmextraunsol, str_ate, 
	//GPRSָ��
	str_cipstart, str_cstt, str_cctt, str_ciicr, str_cipshut, 
	str_cipc, str_cipclose, str_cipsend,str_cping, str_cifsr, 
	str_clport, str_cipflp, str_cipsrip, str_cipdpdp, str_cipstatus, 
	str_crestart, str_cipact, str_cipcn, str_crcfm, str_cipret,
	//����ͨ��ָ��
	str_ata, str_atd, str_atw, str_atvts, str_atdl, str_ath, str_atm, 
	str_atp, str_atq, str_ats0, str_atcr, str_atcrc, str_atclcc, 
	str_atclvl, str_atclip, str_atcolp
};

//////////////////////////////////////////////////////////////////////////

const char*     gsm_api_cmd( unsigned n_type )
{
	if ( n_type < CMD_NUM ) 
		return p_cmd_list[n_type];
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////////

const char*     gsm_api_at_cmd( unsigned n_idx, const char* p_param )
{
	if ( n_idx )
		sprintf( g_at_cmd, "%s%s\r\0", ATCMDINFO[n_idx], p_param );
	else
		sprintf( g_at_cmd, "%s\0", p_param );

	return g_at_cmd;
}