
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
	"AT+CLIP=1\r\0",     // 来电显示
	"AT+CSQ\r\0",        //	CMD_AT_QSP
	
	
	// ACK result
	"OK\r\n\0",           // CMD_ACK_STR_OK
	"+CPIN: READY\0",     // ACK for 
	"+CLCC:\0",           // CDM_ACK_CLCC  

		// ack for calling 
	"NO ANSWER\0",          //CMD_ACK_NO_ANSWER, 用户没有接听
	"NO DIALTONE\0",        //CMD_ACK_CALL_NO_DIALTONE,  
	"NO CARRIER\0",         //CMD_ACK_CALL_NO_CARRIER,   
	"Disconnecting\0",      //CMD_ACK_CALL_DISCONNECT, 被动时,仅在接通后挂断有此消息
	"RING\r\n",             //CMD_ACK_CALL_ERROR,    
	
	"+CSQ:",                // +CSQ: 16,0 ;查询GSM信号的ACK
	"\0"
};

//主叫的返回
// BUSY 对方忙或者对方取消接听
// +COLP: "13564821116",129(145) OK 被叫摘机
// NO CARRIER 对方挂机
// NO ANSWER 无人接听

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




//有关GPRS的AT命令

const char*  str_cipstart             = "AT+CIPSTART=";    //启动TCP/IP 任务，和远程的服务器连接   
const char*  str_cstt                 = "AT+CSTT=";        //配置GPRS链接参数 
const char*  str_cctt                 = "AT+CCTT";         //关闭TCP/IP服务
const char*  str_ciicr                = "AT+CIICR";        //激活移动场景
const char*  str_cipshut              = "AT+CIPSHUT";      //关闭移动场景
const char*  str_cipc                 = "AT+CIPC=";        //与远程服务器建立连接
const char*  str_cipclose             = "AT+CIPCLOSE";     //关闭与远程服务器建立的连接
const char*  str_cipsend              = "AT+CIPSEND";      //向远程服务器发送数据
const char*  str_cping                = "AT+CPING=";       //ping远程IP地址
const char*  str_cifsr                = "AT+CIFSR";        //查看本地IP地址和端口号
const char*  str_clport               = "AT+CLPORT=";      //设置TCP/UDP的本地端口
const char*  str_cipflp               = "AT+CIPFLP=";      //设置是否固定本地端口
const char*  str_cipsrip              = "AT+CIPSRIP=";     //设置是否在返回信息中显示远程机器的IP地址     
const char*  str_cipdpdp              = "AT+CIPDPDP=";     //设置自动设置自动重连网络的时间间隔
const char*  str_cipstatus            = "AT+CIPSTATUS";    //查询网络的连接状态
const char*  str_crestart             = "AT+CRESTART";     //重新建立TCP/UDP连接
const char*  str_cipact               = "AT+CIPACT=";      //激活一个连接
const char*  str_cipcn                = "AT+CIPCN";        //得到当前成功连接的数目
const char*  str_crcfm                = "AT+CRCFM=";       //设置TCP/IP AT命令的返回格式
const char*  str_cipret               = "AT+CIPRET";       //清空TCP/IP信息


//关于语音通话的AT指令
const char*  str_ata                  = "ATA";            //摘机 应答电话
const char*  str_atd                  = "ATD";            //拨号或者发送传真
const char*  str_atw                  = "AT&W";
const char*  str_atvts                = "AT+VTS=";
const char*  str_atdl                 = "ATDL";           //重拨
const char*  str_ath                  = "AT+CHUP";        //挂机
const char*  str_atm                  = "ATM";            //设置扬声器的工作模式
const char*  str_atp                  = "ATP";            //设置脉冲拨号
const char*  str_atq                  = "ATQ";            //Set result code presentation mode
const char*  str_ats0                 = "ATS0";            //Set number of rings before automatically answering the call
const char*  str_atcr                   = "AT+CR=";
const char*  str_atcrc                  = "AT+CRC=";
const char*  str_atclcc                 = "AT+CLCC";        //得到当前的通信号码

const char*  str_atclvl               = "AT+CLVL=";        //调整音量 (1-100)
const char*  str_atclip               = "AT+CLIP=";        //允许来电显示
const char*  str_atcolp               = "AT+COLP=";        //Connected Line Identification Presentation


const char*  ATCMDINFO[] = {
	//GSM指令
	str_null, str_atcmgd, str_atcmgf, str_atcmgl, str_atcmgr, 
	str_atcmgs, str_atcmgw, str_atcmss, str_atcmgc, str_atcnmi, 
	str_atcpms, str_atcres, str_atcsas, str_atcsca, str_atcscb, 
	str_atcsdh, str_atcsmp, str_atcsms, str_atsmalphaid, str_atsmextrainfo, 
	str_atsmextraunsol, str_ate, 
	//GPRS指令
	str_cipstart, str_cstt, str_cctt, str_ciicr, str_cipshut, 
	str_cipc, str_cipclose, str_cipsend,str_cping, str_cifsr, 
	str_clport, str_cipflp, str_cipsrip, str_cipdpdp, str_cipstatus, 
	str_crestart, str_cipact, str_cipcn, str_crcfm, str_cipret,
	//语音通话指令
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