/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $Ex\inc\ex_config.h

 $Reversion History:

        (1) Created Liuhl@2008-01-14 11:40

 $Notes:
		
--*/

#ifndef  GSM_MSG_DEF_H
#define  GSM_MSG_DEF_H

#include <windows.h>
 
#define  MSG_GSM_NOTIFY							( WM_USER+0x1234 )
#define  MSG_GSM_RESET_OVER						( WM_USER+0x1235 )
#define  MSG_CREATE_LOGO_FILE					( WM_USER+0x1236 )
//
//define notify message
//

// 用户信息编码方式
#define GSM_7BIT        0
#define GSM_8BIT        4
#define GSM_UCS2        8

typedef struct
{
	unsigned  m_type;          //0 : delte ; 1:read, 2 : unread,3:SIM READ, 4SIM:UNREAD
	char      m_time[16];      //080319121015 2008-03-19 12:10:15
	char      m_number[16];    //电话号码8613818763854
	char      m_context[160];  // max number is 160
}st_sms;

typedef struct
{
	BYTE		ucSMSType;          //A  B  C  D  E
	BYTE		ucVoiceType;		//0: 无语音   1:有语音
	BYTE		ucTime[16];			//080319121015 2008-03-19 12:10:15
	char		ucContent[160];		//140
}st_dispatch;

// 短消息参数结构，编码/解码共用
// 其中，字符串以0结尾
typedef struct {
    char SCA[16];       // 短消息服务中心号码(SMSC地址)
    char TPA[16];       // 目标号码或回复号码(TP-DA或TP-RA)
    char TP_PID;        // 用户信息协议标识(TP-PID)
    char TP_DCS;        // 用户信息编码方式(TP-DCS)
    char TP_SCTS[16];   // 服务时间戳字符串(TP_SCTS), 接收时用到
    char TP_UD[161];    // 原始用户信息(编码前或解码后的TP-UD)
    char index;         // 短消息序号，在读取时用到
} SM_PARAM;

typedef struct
{
	short   nIdx;
	short   state;    // 0,无记录，1，有记录
	char    phone[64];
	char    name[64];
}PBOOK_ITEM;


enum egsm_state_network_sel
{
	gsm_net_null         = 0,
	gsm_net_china_mobile = 1,	//中国移动
	gsm_net_china_unicom = 2,   //中国联通
	gsm_net_china_CUGSM	 = 3,	//中国联通WCDMA
};

enum egsm_state_sim
{
	gsm_sim_null     = 0,   //SIM卡没有初始化
	gsm_sim_insert   = 1,   //SIM卡已经插入
	gsm_sim_uninsert = 2    //SIM没有插入
};

//0 -113 dBm or less
//1 -111 dBm
//2...30 -109... -53 dBm
//31 -51 dBm or greater
//99 not known or not detectable
//Signal Quality Report
enum  egsm_state_signal
{
	gsm_signal_null = 0,
	gsm_signal_1    = 1,
	gsm_signal_2    = 2,
	gsm_signal_3    = 3,
	gsm_signal_4    = 4,
	gsm_signal_5    = 5
};


typedef struct _tag_gsm_state
{
	unsigned char   sim_card_state;    //SIM卡状态
	unsigned char   gsm_signal;        //GSM信号状态
	unsigned char   gsm_network_sel;   //网络运营商
	unsigned char   gsm_is_dialing;
	unsigned int    m_sim_sms;
	unsigned int    m_sim_pbook;
}GSM_STATE;

//////////////////////////////////////////////////////////////////////////

enum  gsm_act_e
{
	gsm_act_error    = 0,
	gsm_act_waiting,
		
	gsm_master_act_connect,        //主叫呼叫，线路已经连接好
	gsm_master_act_no_carrier,
	gsm_master_act_no_diatone,
		
	gsm_master_act_calling,        //主叫方  呼叫中....
	gsm_master_act_accepting,      //主叫方呼叫，对方已经接听....
	gsm_master_act_error,          //主叫方呼叫，被叫放有问题，不能接通
	gsm_master_act_no_anser,       //主叫方呼叫，被叫方没有接听电话
		
	gsm_slaver_act_ring,           //有新电话打进来中....
	gsm_slaver_act_accepting,      //新电话进来后，本机已接通
	gsm_act_none                   //没有任何状态	
};

enum eGSMState
{
	eGState_NULL = 0,
	eGState_InitGSM,
	eGState_InitGSM_OK,
	eGState_ConnectingIP,
	eGState_ConnectedIP,
	eGState_LoginingSvr,
	eGState_LoginedSvr,
	eGState_Error,		
	eGState_ReConnect,		
	eGState_Total,
};

enum eGSMNotify
{
	eGSM_Start     = 0x100,

	eGSM_SIGNAL_VALUE,
	eGSM_CONFIG_RST,               // 1 OK; 0: failed
	eGSM_SIM_CARD_RST,             // 1, SIM has inserted, other not.
	eGSM_REGISTER_GSM_RST,
	eGSM_QUERY_GSM_ISP,            // query telecommunication operation discusses
	eGSM_DIAL_CONNECT_RST,         // GSM has connected information 1 : connected, 0 : disconnect
	eGSM_NEW_CALLING_RST,          // new call
	eGSM_NEW_SMS_COMING,           // new SMS
	eGSM_READ_NEW_SMS_CONTENT,		//返回新短信内容
	eGSM_READ_SIM_SMS_RST,         // read SMS result
	eGSM_READ_SIM_PBOOK_RST,       // read phonebook


	eGSM_SIM_CARD,                 // 1, SIM has inserted, other not.
	eGSM_REGISTER_GSM,             // GSM register information
	eGSM_REGISTER_GPRS,            // GSM register information
	eGSM_REGISTER_GPRS_RST,        // GSM register information
	eGSM_ORDER_COMFIRM,			   //@z
	eGSM_CONNECT_IP,
	eGSM_CONNECT_IP_RST,
	eGSM_RESET_GPRS,
	eGSM_LOGIN_RST,
	eGSM_REPORT_POS,
	eGSM_SEND_POS,					//响应服务器的0x30终端位置请求消息
	eGSM_SET_TIME,
	eGSM_SET_NET,
	eGSM_SVR_MSG,
	eGSM_SET_SPEEDALERT,
	eGSM_HEART_ACK,
	eGSM_LISTEN_PHONE,				//监听

	eGSM_RECTRL_OPEN,               //汽车开门状态
	eGSM_Total
};


enum gGSMCmd
{
	eCMD_NULL = 0,
	eCMD_IMEI,                    //CMD for query IMEI
	eCMD_QUERY_SIGNAL,
	eCMD_QUERY_SIM,               //
	eCMD_REGISTER_GSM,            //GSM network register
	eCMD_REGISTER_GPRS,           //GPRS network register
	eCMD_CONNECT_IP,              //Connect to server
};


#define  UMSG_GSM_CMD                  ( WM_USER + 0x4001 ) 
#define  GSM_ACK_SINGLE_RESULT         13 //信号值


enum  eGsmCmd
{
	GSM_CMD_OPEN  =  0,     //初始化GSM模块
	GSM_CMD_CLOSE,          //关闭GSM模块，关闭串口
	GSM_CMD_INIT,           // GSM 模块 相关注册信息
	GSM_CMD_SUSPEND,        //关闭串口，挂起线程，让GPRS开始工作
	GSM_CMD_RESUME,         //打开串口，回复线程，让GSM工作
	GSM_CMD_REFIND_WND,     //重新查找窗口，用来向主线程发送消息
	

	GSM_CMD_SENDMSG,        //发送消息命令
	GSM_CMD_DIAL,           //拨打电话命令
	GSM_CMD_DIAL_EXT,       //拨打分机命令
	GSM_CMD_HANGUP,         //用户挂机命令
	GSM_CMD_ACCEPTED,       //接听电话

	GSM_CMD_CHECK_LINE,     //检查连接后的
	GSM_CMD_CHECK_SIGNAL,   //检测SIM卡的信号和状态，目前只是做了信号
	GSM_CMD_SET_VOL,        //设置GSM的模块的声音

	GSM_ACK_MSG_RESULT,     //发送短消息后的ACK, 1:成功; 0:失败
	GSM_ACK_DIAL_RESULT,    //拨打电话后的状态ACK, 1:成功； 0:失败
	GSM_ACK_DIALING_LINE,    //电话连接好后的系统状态

	GSM_ACK_LINE_DISCONNECT, //电话拨号拨通后的状态，取消连接
	GSM_ACK_LINE_NOCARRIER,  //电话拨号拨通后的状态，没有拨号音

	GSM_ACK_NEW_MSG,        //接收到新的短消息
	GSM_ACK_NEW_CALLING,    //有新的电话打进来了
	GSM_ACK_SIGNAL,         //报告当前的GSM信号强度
	GSM_ACK_SIM_STATE,      //SIM卡的状态, 1有SIM卡，0:没有SIM卡, 2 系统初始化失败,模块没有应答

	GSM_ACK_SMS_STATE,      //报告SMS的状态 HIWORD()=最大存储容量,LOWORD() = 已经有的短信个数
	GSM_ACK_PBOOK_STATE,    //报告PBOOK的状态 HIWORD()=最大存储容量,LOWORD() = 已经实用的个数

	GSM_SIM_SMS_READ,       //读取SIM卡上的所有短消息
	GSM_SIM_PBOOK_READ,     //读取SIM卡上的所有电话薄完成
	GSM_SIM_SMS_DEL,        //删除卡上的指定短消息，WPARAM = idx;
	GSM_SIM_PBOOK_DEL,      //删除卡的指定的电话薄记录,WPARAM = idx;
	GSM_NET_SET,            //HIWORD(LPARAM) = Auto,User, LOWORD(LPARAM) = 0,1
	GSM_QUERY_NET,          //查询GSM当前网络
	GSM_ACK_QUERY_NET,      //当前系统的网络信息
	GSM_CMD_ADD_SIM_PBOOK,  //向SIM卡上增加电话本
	GSM_SIM_PBOOK_READ_PROCESS,    //读SIM卡上电话薄的进度HIWORD():总共电话个数 LOWORD():正在读的条数
	GSM_SIM_SMS_READ_PROCESS,      //读SIM卡上的SMS进度，HIWORD:总数，LOWORD:当前的个数
	//LiuHL@20090910
	GPRS_SVR_NEW_MSG,              //服务器下发的数据，最大长度为256个字节，而且为GB2312编码             
	GSM_CMD_RESERVED,
	GSM_CMD_LISTEN
};

// for WM_DATACOPY类型
enum eDataCopy
{
	GSM_DCOPY_PHONE      = 1027,  //电话号码
	GSM_DCOPY_PHONE_EXT,          //电话号码分机
	GSM_DCOPY_MSG,                //将要发送的消息
	GSM_DCOPY_PBOOK,              //在SIM卡上增加电话薄
	GSM_DCOPY_ACK_MSG,            //新消息
	GSM_DCOPY_DIPATCH_MSG,        //调度信息
	GSM_DCOPY_ACK_PHONE,
	GSM_DCOPY_SIM_SMS,
	GSM_DCOPY_SIM_PHONEBOOK,
	GPS_DCOPY_GPRMC,				//将定位数据传送到menu.exe
};


#define PATH_NEW_CALL_INCOMING   ("\\Windows\\incalling.dat")
#define PATH_NEW_CALL_OUT        ("\\Windows\\outing.dat")
#define PATH_NEW_CALL_OUT_EX     ("\\Windows\\outingex.dat")
#define PATH_SEND_SMS_TEXT       ("\\Windows\\sendsms.dat")
#define PATH_NEW_SVR_MSG         ("\\Windows\\svrmsg.dat")


#define  PNDMSG_GSM_SRV_NOTIFY     (_PPNDMSG_GSM_ + 5) // GSM Srv端消息
		#define WGSM_SRV_LINE_CONNECT        1         // 电话已经连接
		#define WGSM_SRV_LINE_ERROR          2         // 电话连接出错
		#define WGSM_SRV_LINE_STATE          3         // 电话连接逅的状态
		#define WGSM_SRV_LINE_DISCONNECT     4         // 电话连被对方挂断
		#define WGSM_SRV_LINE_NOCARRIER      5         // 电话没有连接
		#define WGSM_SRV_LINE_INCOMING       6         // 电话没有连接
		#define WGSM_SRV_SMS_INCOMING        7         // 有一个新短消息来了
		#define WGSM_SRV_SMS_SENDING_ACK     8         // 发送短消息后的返回状态0:失败 1:成功
 



#endif  //MSG_DEF_H