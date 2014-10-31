
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
		NO_POWER ,        //��ѹ����
		NO_SIMCARD,       //δ����SIM��
		FAIL_GETOPERATOR, //�Ҳ�����Ӫ��
		PORTOPENERROR,    //�˿ڴ�ʧ��
		OTHERSUSEPORT,    //�˿ڱ�ռ��
		PORTRESPONSERROR, //�˿���Ӧ��ʱ
		FAIL_FIND_CFG,    //�Ҳ��������ļ�
		SET_APN_FAIL,     //�����ļ��д���
		APP_DISCONNECT,   //���������Ͽ�
		SIMCARD_COVER_LOOSER,  //SIM���ɶ�
		LOW_GSM_SIGNAL,	  //�ź���
		SIM_PINPUK_BLOCK, //SIM��������PIN/PUK
		RESET_GPRS_MODULE,//GPRSģ���ڲ�����
		DEVICE_SLEEP,     //�豸����
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

