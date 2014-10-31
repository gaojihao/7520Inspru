
#ifndef __GSM_STATUS__
#define __GSM_STATUS__

#define     WM_CONNECT_STATUS		WM_USER + 542
#define		WM_GSM_SIGNAL			WM_USER + 545

enum ConnectStatus {
	CONNECT_STATUS_UNKNOWN, //The status is unknown.
		CONNECT_STATUS_CONNECTED,//The connection is established.
		CONNECT_STATUS_DISCONNECTED,//The connection is disconnected.
		CONNECT_STATUS_SUSPENDED,//The connection has been established, but has been suspended.
		CONNECT_STATUS_CONNECTIONFAILED, //The connection has failed and cannot be reestablished. 
		CONNECT_STATUS_WAITINGFORPHONE,  //A voice call is in progress and is using resources that this connection requires.
		CONNECT_STATUS_PHONEOFF,         //The phone has been turned off. 
		CONNECT_STATUS_WAITINGCONNECTION, //he device is attempting to connect.
		CONNECT_STATUS_WAITINGDISCONNECTION //The connection is being disconnected.
};
enum ExtraStatus{
		UN_KNOW = 1,
		NO_POWER ,        //电压不足
		NO_SIMCARD,       //未插入SIM卡
		FAIL_GETOPERATOR, //找不到运营商
		PORTOPENERROR,    //端口打开失败
		OTHERSUSEPORT,    //端口被占用
		PORTRESPONSERROR, //端口响应超时
		FAIL_FIND_CFG,    //找不到配置文件
		SET_APN_FAIL,     //配置文件有错误
		APP_DISCONNECT,   //程序主动断开
		SIMCARD_COVER_LOOSER,  //SIM卡松动
		LOW_GSM_SIGNAL,	  //信号弱
		SIM_PINPUK_BLOCK, //SIM卡设置了PIN/PUK
		RESET_GPRS_MODULE,//GPRS模块内部重启
		DEVICE_SLEEP,     //设备休眠
};


#define MSG_ADDLISTENWND					(WM_USER+0x2001)
#define MSG_REMOVELISTENWND					(WM_USER+0x2002)
#define MSG_GETCONNECTSTATUS				(WM_USER+0x2003)
#define MSG_GETIMSI							(WM_USER+0x2004)
#define MSG_GETIMEI							(WM_USER+0x2005)
#define MSG_FORCECHECKGPRS					(WM_USER+0x2006)
#define MSG_RAS_CONNECT						(WM_USER+0x2007)
#define MSG_RAS_RECONNECT					(WM_USER+0x2008)

#define	WPARAM_ADDLISTENWND					1
#define	WPARAM_REMOVELISTENWND				0

#endif//__GSM_STATUS__

