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

// �û���Ϣ���뷽ʽ
#define GSM_7BIT        0
#define GSM_8BIT        4
#define GSM_UCS2        8

typedef struct
{
	unsigned  m_type;          //0 : delte ; 1:read, 2 : unread,3:SIM READ, 4SIM:UNREAD
	char      m_time[16];      //080319121015 2008-03-19 12:10:15
	char      m_number[16];    //�绰����8613818763854
	char      m_context[160];  // max number is 160
}st_sms;

typedef struct
{
	BYTE		ucSMSType;          //A  B  C  D  E
	BYTE		ucVoiceType;		//0: ������   1:������
	BYTE		ucTime[16];			//080319121015 2008-03-19 12:10:15
	char		ucContent[160];		//140
}st_dispatch;

// ����Ϣ�����ṹ������/���빲��
// ���У��ַ�����0��β
typedef struct {
    char SCA[16];       // ����Ϣ�������ĺ���(SMSC��ַ)
    char TPA[16];       // Ŀ������ظ�����(TP-DA��TP-RA)
    char TP_PID;        // �û���ϢЭ���ʶ(TP-PID)
    char TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
    char TP_SCTS[16];   // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
    char TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD)
    char index;         // ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

typedef struct
{
	short   nIdx;
	short   state;    // 0,�޼�¼��1���м�¼
	char    phone[64];
	char    name[64];
}PBOOK_ITEM;


enum egsm_state_network_sel
{
	gsm_net_null         = 0,
	gsm_net_china_mobile = 1,	//�й��ƶ�
	gsm_net_china_unicom = 2,   //�й���ͨ
	gsm_net_china_CUGSM	 = 3,	//�й���ͨWCDMA
};

enum egsm_state_sim
{
	gsm_sim_null     = 0,   //SIM��û�г�ʼ��
	gsm_sim_insert   = 1,   //SIM���Ѿ�����
	gsm_sim_uninsert = 2    //SIMû�в���
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
	unsigned char   sim_card_state;    //SIM��״̬
	unsigned char   gsm_signal;        //GSM�ź�״̬
	unsigned char   gsm_network_sel;   //������Ӫ��
	unsigned char   gsm_is_dialing;
	unsigned int    m_sim_sms;
	unsigned int    m_sim_pbook;
}GSM_STATE;

//////////////////////////////////////////////////////////////////////////

enum  gsm_act_e
{
	gsm_act_error    = 0,
	gsm_act_waiting,
		
	gsm_master_act_connect,        //���к��У���·�Ѿ����Ӻ�
	gsm_master_act_no_carrier,
	gsm_master_act_no_diatone,
		
	gsm_master_act_calling,        //���з�  ������....
	gsm_master_act_accepting,      //���з����У��Է��Ѿ�����....
	gsm_master_act_error,          //���з����У����з������⣬���ܽ�ͨ
	gsm_master_act_no_anser,       //���з����У����з�û�н����绰
		
	gsm_slaver_act_ring,           //���µ绰�������....
	gsm_slaver_act_accepting,      //�µ绰�����󣬱����ѽ�ͨ
	gsm_act_none                   //û���κ�״̬	
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
	eGSM_READ_NEW_SMS_CONTENT,		//�����¶�������
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
	eGSM_SEND_POS,					//��Ӧ��������0x30�ն�λ��������Ϣ
	eGSM_SET_TIME,
	eGSM_SET_NET,
	eGSM_SVR_MSG,
	eGSM_SET_SPEEDALERT,
	eGSM_HEART_ACK,
	eGSM_LISTEN_PHONE,				//����

	eGSM_RECTRL_OPEN,               //��������״̬
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
#define  GSM_ACK_SINGLE_RESULT         13 //�ź�ֵ


enum  eGsmCmd
{
	GSM_CMD_OPEN  =  0,     //��ʼ��GSMģ��
	GSM_CMD_CLOSE,          //�ر�GSMģ�飬�رմ���
	GSM_CMD_INIT,           // GSM ģ�� ���ע����Ϣ
	GSM_CMD_SUSPEND,        //�رմ��ڣ������̣߳���GPRS��ʼ����
	GSM_CMD_RESUME,         //�򿪴��ڣ��ظ��̣߳���GSM����
	GSM_CMD_REFIND_WND,     //���²��Ҵ��ڣ����������̷߳�����Ϣ
	

	GSM_CMD_SENDMSG,        //������Ϣ����
	GSM_CMD_DIAL,           //����绰����
	GSM_CMD_DIAL_EXT,       //����ֻ�����
	GSM_CMD_HANGUP,         //�û��һ�����
	GSM_CMD_ACCEPTED,       //�����绰

	GSM_CMD_CHECK_LINE,     //������Ӻ��
	GSM_CMD_CHECK_SIGNAL,   //���SIM�����źź�״̬��Ŀǰֻ�������ź�
	GSM_CMD_SET_VOL,        //����GSM��ģ�������

	GSM_ACK_MSG_RESULT,     //���Ͷ���Ϣ���ACK, 1:�ɹ�; 0:ʧ��
	GSM_ACK_DIAL_RESULT,    //����绰���״̬ACK, 1:�ɹ��� 0:ʧ��
	GSM_ACK_DIALING_LINE,    //�绰���Ӻú��ϵͳ״̬

	GSM_ACK_LINE_DISCONNECT, //�绰���Ų�ͨ���״̬��ȡ������
	GSM_ACK_LINE_NOCARRIER,  //�绰���Ų�ͨ���״̬��û�в�����

	GSM_ACK_NEW_MSG,        //���յ��µĶ���Ϣ
	GSM_ACK_NEW_CALLING,    //���µĵ绰�������
	GSM_ACK_SIGNAL,         //���浱ǰ��GSM�ź�ǿ��
	GSM_ACK_SIM_STATE,      //SIM����״̬, 1��SIM����0:û��SIM��, 2 ϵͳ��ʼ��ʧ��,ģ��û��Ӧ��

	GSM_ACK_SMS_STATE,      //����SMS��״̬ HIWORD()=���洢����,LOWORD() = �Ѿ��еĶ��Ÿ���
	GSM_ACK_PBOOK_STATE,    //����PBOOK��״̬ HIWORD()=���洢����,LOWORD() = �Ѿ�ʵ�õĸ���

	GSM_SIM_SMS_READ,       //��ȡSIM���ϵ����ж���Ϣ
	GSM_SIM_PBOOK_READ,     //��ȡSIM���ϵ����е绰�����
	GSM_SIM_SMS_DEL,        //ɾ�����ϵ�ָ������Ϣ��WPARAM = idx;
	GSM_SIM_PBOOK_DEL,      //ɾ������ָ���ĵ绰����¼,WPARAM = idx;
	GSM_NET_SET,            //HIWORD(LPARAM) = Auto,User, LOWORD(LPARAM) = 0,1
	GSM_QUERY_NET,          //��ѯGSM��ǰ����
	GSM_ACK_QUERY_NET,      //��ǰϵͳ��������Ϣ
	GSM_CMD_ADD_SIM_PBOOK,  //��SIM�������ӵ绰��
	GSM_SIM_PBOOK_READ_PROCESS,    //��SIM���ϵ绰���Ľ���HIWORD():�ܹ��绰���� LOWORD():���ڶ�������
	GSM_SIM_SMS_READ_PROCESS,      //��SIM���ϵ�SMS���ȣ�HIWORD:������LOWORD:��ǰ�ĸ���
	//LiuHL@20090910
	GPRS_SVR_NEW_MSG,              //�������·������ݣ���󳤶�Ϊ256���ֽڣ�����ΪGB2312����             
	GSM_CMD_RESERVED,
	GSM_CMD_LISTEN
};

// for WM_DATACOPY����
enum eDataCopy
{
	GSM_DCOPY_PHONE      = 1027,  //�绰����
	GSM_DCOPY_PHONE_EXT,          //�绰����ֻ�
	GSM_DCOPY_MSG,                //��Ҫ���͵���Ϣ
	GSM_DCOPY_PBOOK,              //��SIM�������ӵ绰��
	GSM_DCOPY_ACK_MSG,            //����Ϣ
	GSM_DCOPY_DIPATCH_MSG,        //������Ϣ
	GSM_DCOPY_ACK_PHONE,
	GSM_DCOPY_SIM_SMS,
	GSM_DCOPY_SIM_PHONEBOOK,
	GPS_DCOPY_GPRMC,				//����λ���ݴ��͵�menu.exe
};


#define PATH_NEW_CALL_INCOMING   ("\\Windows\\incalling.dat")
#define PATH_NEW_CALL_OUT        ("\\Windows\\outing.dat")
#define PATH_NEW_CALL_OUT_EX     ("\\Windows\\outingex.dat")
#define PATH_SEND_SMS_TEXT       ("\\Windows\\sendsms.dat")
#define PATH_NEW_SVR_MSG         ("\\Windows\\svrmsg.dat")


#define  PNDMSG_GSM_SRV_NOTIFY     (_PPNDMSG_GSM_ + 5) // GSM Srv����Ϣ
		#define WGSM_SRV_LINE_CONNECT        1         // �绰�Ѿ�����
		#define WGSM_SRV_LINE_ERROR          2         // �绰���ӳ���
		#define WGSM_SRV_LINE_STATE          3         // �绰�����˵�״̬
		#define WGSM_SRV_LINE_DISCONNECT     4         // �绰�����Է��Ҷ�
		#define WGSM_SRV_LINE_NOCARRIER      5         // �绰û������
		#define WGSM_SRV_LINE_INCOMING       6         // �绰û������
		#define WGSM_SRV_SMS_INCOMING        7         // ��һ���¶���Ϣ����
		#define WGSM_SRV_SMS_SENDING_ACK     8         // ���Ͷ���Ϣ��ķ���״̬0:ʧ�� 1:�ɹ�
 



#endif  //MSG_DEF_H