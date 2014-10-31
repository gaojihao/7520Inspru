/************************************************************************
* Copyright (C)2011, �Ϻ��ﲩͨѶ�������޹�˾
* All rights reserved.
* 
* �ļ�����: DataType.h
* �ļ���ʶ: 
* ժ    Ҫ: 
* ��    ��: 
* �������: 2011/09/26
* ��ע: 
************************************************************************/
#pragma once

#define WM_HARDWARE_BUTTON		0x00000710		//Ӳ������ϢID
#define WPARAM_BTN_1			0x0000F1F1		//����1
#define WPARAM_BTN_2			0x0000F3F3		//����2
#define WPARAM_BTN_3			0x0000F4F4		//����3
#define WPARAM_BTN_4			0x0000F5F5		//����4
#define WPARAM_BTN_5			0x0000F2F2		//����5
#define WPARAM_BTN_6			0x0000F6F6		//����6


//����socket buffer��󳤶�
#define MSG_BUFFER_LEN					1280
#define NUM_256B						256
#define NUM_512B						512
#define NUM_1K							1024
#define NUM_2K							(1024*2)
#define NUM_3K							(1024*3)
#define NUM_4K							(1024*4)

//���������ά��
#define dim(x)							(sizeof(x) / sizeof(x[0]))

//config.iniϵͳ�����ļ�·��
#define	PATH_CONFIG_INI					_T("\\Flashdrv Storage\\MENU\\config.ini")
//TerminalParam.iniϵͳ�����ļ�·��
#define	PATH_PARAM_INI					_T("\\Flashdrv Storage\\MENU\\TerminalParam.ini")

#define	MenuApp							_T("WND_MAIN_MENU")

//��Ϣ����
//ͨ��״̬����Ϣ   ����ͨ����Ϣ
#define	WM_COMMAND_TIANZE				(WM_USER+0x1000)
#define	WPARAM_SERVER_LOGIN				10					//LPARAM:1 ��¼�ɹ�  0 ����
//���յ����������͵�socket���ݺ�Ĵ�����Ϣ
#define WM_SERVER						(WM_USER+0x4000)

#define WM_ALARM                        0x0171

enum enMSG_ID
{
	//�ն�(TerMiNal)��ϢID
	MSG_TMN_COMMON_ACK					= 0x0001,	//1.�ն�ͨ��Ӧ��
	MSG_TMN_HEARTBEAT					= 0x0002,	//3.�ն�����
	MSG_TMN_REGISTER					= 0x0100,	//4.�ն�ע��
	MSG_TMN_LOGOUT						= 0x0003,	//6.�ն�ע��
	MSG_TMN_AUTHENTICATION				= 0x0102,	//7.�ն˼�Ȩ
	MSG_TMN_PARAM_GET_ACK				= 0x0104,	//10.��ѯ�ն˲���Ӧ��
	MSG_TMN_POS_REPORT					= 0x0200,	//12.λ����Ϣ�㱨
	MSG_TMN_POS_GET_ACK					= 0x0201,	//14.λ����Ϣ��ѯӦ��
	MSG_TMN_EVENT_REPORT				= 0x0301,	//18.�¼�����
	MSG_TMN_QUESTION_ACK				= 0x0302,	//20.����Ӧ��
	MSG_TMN_INFO_DEMAND_CANCEL			= 0x0303,	//22.��Ϣ�㲥/ȡ��
	MSG_TMN_CAR_CONTROL_ACK				= 0x0500,	//27.��������Ӧ��
	MSG_TMN_DRIVE_RECORD_UPLOAD			= 0x0700,	//37.��ʻ��¼�����ϴ�
	MSG_TMN_E_AWB_UPLOAD				= 0x0701,	//39.�����˵��ϱ�(Auto Waybill)
	MSG_TMN_DRIVER_INFO_UPLOAD			= 0x0702,	//40.��ʻԱ�����Ϣ�ɼ��ϱ�
	MSG_TMN_MULTIMEDIA_CMD				= 0x0800,	//41.��ý���¼���Ϣ�ϴ�
	MSG_TMN_MULTIMEDIA_UPLOAD			= 0x0801,	//42.��ý�������ϴ�
	MSG_TMN_MULTIMEDIA_SEARCH_ACK		= 0x0802,	//46.�洢��ý�����ݼ���Ӧ��
	MSG_TMN_DATA_UPLOAD					= 0x0900,	//51.��������͸��
	MSG_TMN_COMPRESS_DATA_UPLOAD		= 0x0901,	//52.����ѹ���ϱ�
	MSG_TMN_RSA_PUBLIC_KEY				= 0x0A00,	//54.�ն�RSA��Կ
	MSG_TMN_TEXT_SMS					= 0x0300,	//��1.�ı���Ϣ�ϴ�

	//������ƽ̨(SerVeR)��ϢID
	MSG_SVR_COMMON_ACK					= 0x8001,	//2.ƽ̨ͨ��Ӧ��
	MSG_SVR_REGISTER_ACK				= 0x8100,	//5.�ն�ע��Ӧ��
	MSG_SVR_PARAM_SET					= 0x8103,	//8.�����ն˲���
	MSG_SVR_PARAM_GET					= 0x8104,	//9.��ѯ�ն˲���
	MSG_SVR_TMN_CONTROL					= 0x8105,	//11.�ն˿���
	MSG_SVR_POS_GET						= 0x8201,	//13.λ����Ϣ��ѯ
	MSG_SVR_TEMP_POS_TRACE				= 0x8202,	//15.��ʱλ�ø��ٿ���
	MSG_SVR_TEXT_SMS					= 0x8300,	//16.�ı���Ϣ�·�
	MSG_SVR_EVENT_SET					= 0x8301,	//17.�¼�����
	MSG_SVR_QUESTION					= 0x8302,	//19.�����·�
	MSG_SVR_INFO_MENU_SET				= 0x8303,	//21.��Ϣ�㲥�˵�����
	MSG_SVR_INFO_SERVICE				= 0x8304,	//23.��Ϣ����
	MSG_SVR_CALL_BACK					= 0x8400,	//24.�绰�ز�
	MSG_SVR_PHONE_BOOK_SET				= 0x8401,	//25.���õ绰��
	MSG_SVR_CAR_CONTROL					= 0x8500,	//26.��������
	MSG_SVR_CIRC_REGION_SET				= 0x8600,	//28.����Բ������
	MSG_SVR_CIRC_REGION_DEL				= 0x8601,	//29.ɾ��Բ������
	MSG_SVR_RECT_REGION_SET				= 0x8602,	//30.���þ�������
	MSG_SVR_RECT_REGION_DEL				= 0x8603,	//31.ɾ����������
	MSG_SVR_NGON_REGION_SET				= 0x8604,	//32.���ö��������
	MSG_SVR_NGON_REGION_DEL				= 0x8605,	//33.ɾ�����������
	MSG_SVR_ROUTE_SET					= 0x8606,	//34.����·��
	MSG_SVR_ROUTE_DEL					= 0x8607,	//35.ɾ��·��
	MSG_SVR_DRIVE_RECORD_COLLECT		= 0x8700,	//36.��ʻ��¼���ݲɼ�����
	MSG_SVR_DRIVE_RECORD_PARAM_SET		= 0x8701,	//38.��ʻ��¼�����´�����
	MSG_SVR_MULTIMEDIA_UPLOAD_ACK		= 0x8800,	//43.��ý�������ϴ�Ӧ��
	MSG_SVR_CAMERA_PHOTOGRAPH			= 0x8801,	//44.����ͷ������������
	MSG_SVR_MULTIMEDIA_SEARCH			= 0x8802,	//45.�洢��ý�����ݼ���
	MSG_SVR_MULTIMEDIA_UPLOAD			= 0x8803,	//47.�洢��ý�������ϴ�����
	MSG_SVR_SOUND_RECORD				= 0x8804,	//48.¼����ʼ����
	MSG_SVR_SINGLE_MULT_SEARCH			= 0x8805,	//49.�����洢��ý�����ݼ����ϴ�����
	MSG_SVR_DATA_DOWNLOAD				= 0x8900,	//50.��������͸��
	MSG_SVR_RSA_PUBLIC_KEY				= 0x8A00,	//53.ƽ̨RSA��Կ
	MSG_TMN_DESTINATION_NAVI			= 0x8F01,	//��2.ƽ̨�·�"Ŀ�ĵص���"ָ��
};

//�ն˲���(���ڷ��������û��ȡ)
enum TerminalParam
{
	PARAM_HEART_BEAT_SLICE				= 0x0001,	//�ն��������ͼ������λΪ��(s)
	PARAM_TCP_ACK_OVERTIME				= 0x0002,	//TCP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	PARAM_TCP_RETRANSMIT_COUNT			= 0x0003,	//TCP��Ϣ�ش�����
	PARAM_UDP_ACK_OVERTIME				= 0x0004,	//UDP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	PARAM_UDP_RETRANSMIT_COUNT			= 0x0005,	//UDP��Ϣ�ش�����
	PARAM_SMS_ACK_OVERTIME				= 0x0006,	//SMS��ϢӦ��ʱʱ�䣬��λΪ��(s)
	PARAM_SMS_RETRANSMIT_COUNT			= 0x0007,	//SMS��Ϣ�ش�����
	//0x0008~0x000F		����
	PARAM_MAIN_SVR_APN					= 0x0010,	//��������APN
	PARAM_MAIN_SVR_NAME					= 0x0011,	//������������ͨ�Ų����û���
	PARAM_MAIN_SVR_PWD					= 0x0012,	//������������ͨ�Ų�������
	PARAM_MAIN_SVR_IP					= 0x0013,	//����������ַ��IP������
	PARAM_BAK_SVR_APN					= 0x0014,	//���ݷ�����APN
	PARAM_BAK_SVR_NAME					= 0x0015,	//���ݷ���������ͨ�Ų����û���
	PARAM_BAK_SVR_PWD					= 0x0016,	//���ݷ���������ͨ�Ų�������
	PARAM_BAK_SVR_IP					= 0x0017,	//���ݷ�������ַ��IP������
	PARAM_SVR_TCP_PORT					= 0x0018,	//������TCP�˿�
	PARAM_SVR_UDP_PORT					= 0x0019,	//������UDP�˿�
	//0x001A~0x001F		����
	PARAM_POS_REPORT_TYPE				= 0x0020,	//λ�û㱨����
	PARAM_POS_REPORT_CONDITION			= 0x0021,	//λ�û㱨����
	PARAM_POS_REPORT_SLICE_LOGOUT		= 0x0022,	//��ʻԱδ��¼�㱨ʱ��������λΪ��(s), >0
	//0x0023~0x0026		����
	PARAM_POS_REPORT_SLICE_SLEEP		= 0x0027,	//����ʱ�㱨ʱ��������λΪ��(s), >0
	PARAM_POS_REPORT_SLICE_ALARM		= 0x0028,	//��������ʱ�㱨ʱ��������λΪ��(s), >0
	PARAM_POS_REPORT_SLICE_DEFAULT		= 0x0029,	//ȱʡʱ��㱨�������λΪ��(s), >0
	//0x002A~0x002B		����
	PARAM_POS_REPORT_DIST_DEFAULT		= 0x002C,	//ȱʡ����㱨�������λΪ��(m), >0
	PARAM_POS_REPORT_DIST_LOGOUT		= 0x002D,	//��ʻԱδ��¼�㱨����������λΪ��(m), >0
	PARAM_POS_REPORT_DIST_SLEEP			= 0x002E,	//����ʱ�㱨����������λΪ��(m), >0
	PARAM_POS_REPORT_DIST_ALARM			= 0x002F,	//��������ʱ�㱨����������λΪ��(m), >0
	PARAM_CORNER_RETRANSMIT_ANGLE		= 0x0030,	//�յ㲹���Ƕȣ�<180
	PARAM_ELECTRONIC_FENCE_RADIUS		= 0x0031,	//����Χ���뾶���Ƿ�λ����ֵ������λΪ��  Radius of the electronic fence
	//0x0032~0x003F		����
	PARAM_PHONE_NUM_MONITOR				= 0x0040,	//���ƽ̨�绰����
	PARAM_PHONE_NUM_RESET				= 0x0041,	//��λ�绰����
	PARAM_PHONE_NUM_RESTORE				= 0x0042,	//�ָ��������õ绰����
	PARAM_PHONE_NUM_SVR_SMS				= 0x0043,	//���ƽ̨SMS�绰����
	PARAM_PHONE_NUM_TMN_SMS				= 0x0044,	//�����ն�SMS�ı���������
	PARAM_ANSWER_PHONE_TYPE				= 0x0045,	//�ն˵绰��������
	PARAM_SINGLE_CALL_TIME_LIMIT		= 0x0046,	//ÿ���ͨ��ʱ��
	PARAM_MONTH_CALL_TIME_LIMIT			= 0x0047,	//�����ͨ��ʱ��
	PARAM_MONITOR_PHONE_NUM				= 0x0048,	//�����绰����
	PARAM_SVR_PRIVILEGE_SMS_NUM			= 0x0049,	//���ƽ̨��Ȩ���ź���
	//0x004A~0x004F		����
	PARAM_ALARM_MASK_BIT				= 0x0050,	//����������
	PARAM_ALARM_SEND_SMS_SWITCH			= 0x0051,	//���������ı�SMS����
	PARAM_ALARM_CAPTURE_SWITCH			= 0x0052,	//�������㿪��
	PARAM_ALARM_CAPTURE_STORE_FLAG		= 0x0053,	//��������洢��־
	PARAM_KEY_FLAG						= 0x0054,	//�ؼ���־
	PARAM_SPEED_LIMIT					= 0x0055,	//����ٶ�
	PARAM_OVERSPEED_DUREATION			= 0x0056,	//���ٳ���ʱ��
	PARAM_CONTINUE_DRIVE_TIME_LIMIT		= 0x0057,	//������ʻʱ������
	PARAM_ONE_DAY_DRIVE_TIME_LIMIT		= 0x0058,	//�����ۼƼ�ʻʱ������
	PARAM_MINIMUM_REST_TIME				= 0x0059,	//��С��Ϣʱ��
	PARAM_MAXIMUM_PARKING_TIME			= 0x005A,	//�ͣ��ʱ��
	//0x005B~0x006F		����
	PARAM_IMAGE_VIDEO_QUALITY			= 0x0070,	//ͼ��/��Ƶ������1��10��1���
	PARAM_BRIGHTNESS					= 0x0071,	//���ȣ�0��255
	PARAM_CONTRAST						= 0x0072,	//�Աȶȣ�0��127
	PARAM_SATURATION					= 0x0073,	//���Ͷȣ�0��127
	PARAM_CHROMA						= 0x0074,	//ɫ�ȣ�0��255
	//0x0075~0x007F		����
	PARAM_VEHICLE_ODOMETER				= 0x0080,	//������̱������1/10km
	PARAM_VEHICLE_PROVINCE_ID			= 0x0081,	//�������ڵ�ʡ��ID
	PARAM_VEHICLE_CITY_ID				= 0x0082,	//�������ڵ�����ID
	PARAM_VEHICLE_ID					= 0x0083,	//������ͨ�����Ű䷢�Ļ���������
	PARAM_PLATE_COLOR					= 0x0084,	//������ɫ
};

//�ն˲���(���ڷ��������û��ȡ)
typedef struct _TERMINAL_PARAM
{
	DWORD	dwHEART_BEAT_SLICE;			//(0x0001)�ն��������ͼ������λΪ��(s)
	DWORD	dwTCP_ACK_OVERTIME;			//(0x0002)TCP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	DWORD	dwTCP_RETRANSMIT_COUNT;		//(0x0003)TCP��Ϣ�ش�����
	DWORD	dwUDP_ACK_OVERTIME;			//(0x0004)UDP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	DWORD	dwUDP_RETRANSMIT_COUNT;		//(0x0005)UDP��Ϣ�ش�����
	DWORD	dwSMS_ACK_OVERTIME;			//(0x0006)SMS��ϢӦ��ʱʱ�䣬��λΪ��(s)
	DWORD	dwSMS_RETRANSMIT_COUNT;		//(0x0007)SMS��Ϣ�ش�����
	//0x0008~0x000F		����
	CHAR	szMAIN_SVR_APN[8];			//(0x0010)��������APN
	CHAR	szMAIN_SVR_NAME[16];		//(0x0011)������������ͨ�Ų����û���
	CHAR	szMAIN_SVR_PWD[16];			//(0x0012)������������ͨ�Ų�������
	CHAR	szMAIN_SVR_IP[16];			//(0x0013)����������ַ��IP������
	CHAR	szBAK_SVR_APN[8];			//(0x0014)���ݷ�����APN
	CHAR	szBAK_SVR_NAME[16];			//(0x0015)���ݷ���������ͨ�Ų����û���
	CHAR	szBAK_SVR_PWD[16];			//(0x0016)���ݷ���������ͨ�Ų�������
	CHAR	szBAK_SVR_IP[16];			//(0x0017)���ݷ�������ַ��IP������
	DWORD	dwSVR_TCP_PORT;				//(0x0018)������TCP�˿�
	DWORD	dwSVR_UDP_PORT;				//(0x0019)������UDP�˿�
	//0x001A~0x001F		����
	DWORD	dwPOS_REPORT_TYPE;			//(0x0020)λ�û㱨���� 0:��ʱ�㱨 1:����㱨 2:��ʱ�Ͷ���㱨
	DWORD	dwPOS_REPORT_CONDITION;		//(0x0021)λ�û㱨���� 0:����ACC״̬ 1:���ݵ�¼״̬��ACC״̬
	DWORD	dwPOS_REPORT_SLICE_LOGOUT;	//(0x0022)��ʻԱδ��¼�㱨ʱ��������λΪ��(s), >0
	//0x0023~0x0026		����
	DWORD	dwPOS_REPORT_SLICE_SLEEP;	//(0x0027)����ʱ�㱨ʱ��������λΪ��(s), >0
	DWORD	dwPOS_REPORT_SLICE_ALARM;	//(0x0028)��������ʱ�㱨ʱ��������λΪ��(s), >0
	DWORD	dwPOS_REPORT_SLICE_DEFAULT;	//(0x0029)ȱʡʱ��㱨�������λΪ��(s), >0
	//0x002A~0x002B		����
	DWORD	dwPOS_REPORT_DIST_DEFAULT;	//(0x002C)ȱʡ����㱨�������λΪ��(m), >0
	DWORD	dwPOS_REPORT_DIST_LOGOUT;	//(0x002D)��ʻԱδ��¼�㱨����������λΪ��(m), >0
	DWORD	dwPOS_REPORT_DIST_SLEEP;	//(0x002E)����ʱ�㱨����������λΪ��(m), >0
	DWORD	dwPOS_REPORT_DIST_ALARM;	//(0x002F)��������ʱ�㱨����������λΪ��(m), >0
	DWORD	dwCORNER_RETRANSMIT_ANGLE;	//(0x0030)�յ㲹���Ƕȣ�<180
	DWORD	dwELECTRONIC_FENCE_RADIUS;	//(0x0031)����Χ���뾶���Ƿ�λ����ֵ������λΪ��  Radius of the electronic fence
	//0x0032~0x003F		����
	CHAR	szPHONE_NUM_MONITOR[16];	//(0x0040)���ƽ̨�绰����
	CHAR	szPHONE_NUM_RESET[16];		//(0x0041)��λ�绰����
	CHAR	szPHONE_NUM_RESTORE[16];	//(0x0042)�ָ��������õ绰����
	CHAR	szPHONE_NUM_SVR_SMS[16];	//(0x0043)���ƽ̨SMS�绰����
	CHAR	szPHONE_NUM_TMN_SMS[16];	//(0x0044)�����ն�SMS�ı���������
	DWORD	dwANSWER_PHONE_TYPE;		//(0x0045)�ն˵绰��������
	DWORD	dwSINGLE_CALL_TIME_LIMIT;	//(0x0046)ÿ���ͨ��ʱ��
	DWORD	dwMONTH_CALL_TIME_LIMIT;	//(0x0047)�����ͨ��ʱ��
	CHAR	szMONITOR_PHONE_NUM[16];	//(0x0048)�����绰����
	CHAR	szSVR_PRIVILEGE_SMS_NUM[16];//(0x0049)���ƽ̨��Ȩ���ź���
	//0x004A~0x004F		����
	DWORD	dwALARM_MASK_BIT;			//(0x0050)����������
	DWORD	dwALARM_SEND_SMS_SWITCH;	//(0x0051)���������ı�SMS����
	DWORD	dwALARM_CAPTURE_SWITCH;		//(0x0052)�������㿪��
	DWORD	dwALARM_CAPTURE_STORE_FLAG;	//(0x0053)��������洢��־
	DWORD	dwKEY_FLAG;					//(0x0054)�ؼ���־
	DWORD	dwSPEED_LIMIT;				//(0x0055)����ٶ�
	DWORD	dwOVERSPEED_DUREATION;		//(0x0056)���ٳ���ʱ��
	DWORD	dwCONTINUE_DRIVE_TIME_LIMIT;//(0x0057)������ʻʱ������
	DWORD	dwONE_DAY_DRIVE_TIME_LIMIT;	//(0x0058)�����ۼƼ�ʻʱ������
	DWORD	dwMINIMUM_REST_TIME;		//(0x0059)��С��Ϣʱ��
	DWORD	dwMAXIMUM_PARKING_TIME;		//(0x005A)�ͣ��ʱ��
	//0x005B~0x006F		����
	DWORD	dwIMAGE_VIDEO_QUALITY;		//(0x0070)ͼ��/��Ƶ������1��10��1���
	DWORD	dwBRIGHTNESS;				//(0x0071)���ȣ�0��255
	DWORD	dwCONTRAST;					//(0x0072)�Աȶȣ�0��127
	DWORD	dwSATURATION;				//(0x0073)���Ͷȣ�0��127
	DWORD	dwCHROMA;					//(0x0074)ɫ�ȣ�0��255
	//0x0075~0x007F		����
	DWORD	dwVEHICLE_ODOMETER;			//(0x0080)������̱������1/10km
	WORD	wVEHICLE_PROVINCE_ID;		//(0x0081)�������ڵ�ʡ��ID
	WORD	wVEHICLE_CITY_ID;			//(0x0082)�������ڵ�����ID
	CHAR	szVEHICLE_ID[16];			//(0x0083)������ͨ�����Ű䷢�Ļ���������
	DWORD	dwPLATE_COLOR;				//(0x0084)������ɫ 1 ��   2 ��   3 ��   4 ��   9 ����
} TERMINAL_PARAM, *PTERMINAL_PARAM;

//��Ϣ������
typedef struct _MSG_PROPERTY
{
	WORD	wMsgBodyLength;							//��Ϣ���ݳ���
	BYTE	ucEncryptionType;						//���ݼ��ܷ�ʽ	0:������	1:RSA����
	BYTE	ucSubPack;								//�ְ� 0:����Ϣ����װ��	1:����Ϣ����װ��
} MSG_PROPERTY, *PMSG_PROPERTY;

//��Ϣ����װ��
typedef struct _MSG_PACK_ITEM
{
	WORD	wMsgPackNum;							//��Ϣ�ܰ���
	WORD	wMsgPackIndex;							//�����
} MSG_PACK_ITEM, *PMSG_PACK_ITEM;

//����ͷ
typedef struct _MSG_HEAD
{
	WORD			wMsgID;							//��ϢID		0~1
	MSG_PROPERTY	stProperty;						//��Ϣ������	2~3
	BYTE			ucPhoneNumber[12];				//�ն��ֻ�����	4~9
	WORD			wMsgSerialNum;					//��Ϣ��ˮ��	10~11
	MSG_PACK_ITEM	stPackItem;						//��Ϣ����װ��	12~15	����stProperty.ucSubPack�����Ƿ����
} MSG_HEAD, *PMSG_HEAD;

//���ؽ��
typedef enum _enResult
{
	RESULT_SUCCESS			= 0,	//�ɹ�
	RESULT_FAILURE			= 1,	//ʧ��
	RESULT_MSG_ERROR		= 2,	//��Ϣ����
	RESULT_UNSUPPORT		= 3,	//��֧��
	RESULT_ALARM_CONFIRM	= 4,	//��������ȷ��(��ƽ̨ͨ��Ӧ��)
} enResult;

//Ӧ��	�ն�/ƽ̨ͨ��Ӧ��
typedef struct _COMMON_ACK
{
	WORD		wAckSerialNum;							//Ӧ����ˮ��
	WORD		wAckMsgID;								//Ӧ��ID
	enResult	eRslt;									//���	0:�ɹ�/ȷ��	1:ʧ��	2:��Ϣ����	3:��֧��
} COMMON_ACK, *PCOMMON_ACK;

//�ն�ע��Ӧ��0x8100
typedef struct _REGISTER_ACK
{
	WORD	wAckSerialNum;							//Ӧ����ˮ��
	WORD	wResult;								//���
	BYTE	cAuthenCode[64];						//��Ȩ��
} REGISTER_ACK, *PREGISTER_ACK;

//�ն˿���0x8105��ָ������
enum enTMN_CONTROL
{
	CONTROL_WIRELESSUPGRADE	= 1,	//��������
	CONTROL_CONNECT2SERVER,			//����ָ��������
	CONTROL_POWEROFF,				//�ն˹ػ�
	CONTROL_RESET,					//�ն˸�λ
	CONTROL_RESTORE_DEFAULT,		//�ն˻ظ���������
	CONTROL_CLOSE_GPRS,				//�ر�����ͨ��
	CONTROL_WIRELESS,				//�ر���������ͨ��
};

//��ʱλ�ø��ٿ���0x8202
typedef struct _TEMP_POS_TRACE
{
	WORD	wEnalbe;								//��ʱλ�ø��ٿ����Ƿ���Ч��0:��Ч 1:��Ч
	WORD	wSlice;									//ʱ����
	DWORD	dwPosTraceValidity;						//λ�ø�����Ч��
} TEMP_POS_TRACE, *PTEMP_POS_TRACE;

//�ı���Ϣ��־
typedef struct _TEXT_FLAG
{
	BYTE	cEmergence:1;			//0,	1:����
	BYTE	cDummyData1:1;			//1,	�������1
	BYTE	cDisplay:1;				//2,	1:�ն���ʾ����ʾ
	BYTE	cTTS:1;					//3,	1:�ն�TTS����
	BYTE	cAdvertise:1;			//4,	1:�������ʾ
	BYTE	cDummyData2:3;			//5~7,	�������2
} TEXT_FLAG, *PTEXT_FLAG;

//�ı���Ϣ�·�0x8300
typedef union _unTEXT_FLAG
{
	BYTE				cFlag;						//��־
	TEXT_FLAG			text_flag;
} unTEXT_FLAG;

typedef struct _TEXT_SMS
{
	unTEXT_FLAG		unFlag;							//��־
	BYTE			cDummyData[3];					//�������
	BYTE			cText[NUM_1K];					//�ı���Ϣ
} TEXT_SMS, *PTEXT_SMS;

//�������ͣ������¼�����(0x8301),��Ϣ�㲥�˵�����(0x8303),���õ绰��(0x8401)
enum enSET_TYPE
{
	SET_DELETE_ALL	= 0,	//ɾ��ȫ����¼
	SET_UPDATE,				//��ɾ��ȫ����¼����׷���¼�¼
	SET_ADD,				//���Ӽ�¼
	SET_MODIFY,				//�޸�ָ����¼
	SET_DELETE,				//ɾ��ָ����¼���������¼�����(0x8301)
};

//����ͷ������������0x8801
typedef struct _CAMERA_PHOTO
{
	WORD	wCMD;						//�������0��ֹͣ���㣻0xFFFF��¼��������ʾ��������
	WORD	wTime;						//������/¼��ʱ��,��λ(s)��0��ʾ����Сʱ�������ջ�һֱ¼��
	BYTE	cTunnelID;					//ͨ��ID,>0
	BYTE	cSaveFlag;					//�����־��1�����棻0��ʵʱ�ϴ�
	BYTE	cResolution;				//�ֱ��ʣ�1:320*240,2:640*480,3:800*600,4:1024*768,5:176*144,6:352*288,7:704*288,8:704*576
	BYTE	cPicQuality;				//ͼ��/��Ƶ������1��10��1����������ʧ��С��10��ʾѹ�������
	BYTE	cLuminance;					//����0��255
	BYTE	cContrast;					//�Աȶ�0��255
	BYTE	cSaturation;				//���Ͷ�0��255
	BYTE	cChroma;					//ɫ��0��255
} CAMERA_PHOTO, *PCAMERA_PHOTO;

//�洢��ý�����ݼ���0x8802
//�洢��ý�������ϴ�����0x8803
typedef struct _MULTIMEDIA_DATA
{
	BYTE	cMultimediaType;			//��ý�����ͣ�0��ͼ��1����Ƶ��2����Ƶ
	BYTE	cTunnelID;					//ͨ��ID��0��ʾ������ý�����͵�����ͨ��
	BYTE	cEventCode;					//�¼�����룺0��ƽ̨�·�ָ�1����ʱ������2�����ٱ���������3����ײ�෭������������������
	BYTE	cFlag;						//ɾ����־��0��������1��ɾ��
	BYTE	cStartTime[6];				//��ʼʱ�䣺YY-MM-DD-hh-mm-ss
	BYTE	cEndTime[6];				//����ʱ�䣺YY-MM-DD-hh-mm-ss
} MULTIMEDIA_DATA, *PMULTIMEDIA_DATA;

//¼����ʼ����0x8804
typedef struct _SOUND_RECORDING
{
	BYTE	cRecordCMD:4;				//¼�����0��ֹͣ¼����1����ʼ¼��
	BYTE	cSaveFlag:4;				//�����־��0��ʵʱ�ϴ���1������
	BYTE	cSamplingFrequency;			//����Ƶ�ʣ�0��8K��1��11K��2��23K��3��32K����������
	WORD	wRecordTime;				//¼��ʱ�䣺��λ����(s)��0��ʾһֱ¼��
} SOUND_RECORDING, *PSOUND_RECORDING;

typedef union _unSOUND_RECORDING
{
	SOUND_RECORDING		sound_recording;
	DWORD				dwSoundRecording;
} unSOUND_RECORDING;

//ƽ̨RSA��Կ0x8A00
typedef struct _RSA_DATA
{
	DWORD	dwRSA_E;					//RSA��Կ{e,n}�е�e
	BYTE	cRSA_N[128];				//RSA��Կ{e,n}�е�n
} RSA_DATA, *PRSA_DATA;

//��������
typedef struct _REGION_PROPERTY
{
	BYTE	cAccord2Time:1;			//0,	0:			1:����ʱ��
	BYTE	cSpeedLimit:1;			//1,	0:			1:����
	BYTE	cInRegion:1;			//2,	0:			1:�����򱨾�����ʻԱ
	BYTE	cDummyData1:2;			//3~4,	�������1
	BYTE	cOutRegion:1;			//5,	0:			1:�����򱨾���ƽ̨
	BYTE	cLatitudeType:1;		//6,	0:��γ		1:��γ
	BYTE	cLongitudeType:1;		//7,	0:����		1:����
	BYTE	cDummyData;				//�������
} REGION_PROPERTY, *PREGION_PROPERTY;

//��������
typedef union _unREGION_PROPERTY
{
	WORD				wRegionProperty;
	REGION_PROPERTY		region_property;
} unREGION_PROPERTY;

//����Բ������0x8600
typedef struct _CIRCULAR_REGION
{
	DWORD				dwRegionID;								//����ID
	unREGION_PROPERTY	unRegionProperty;						//��������
	WORD				wDummyData;								//�������1
	DWORD				dwCenterPointLatitude;					//���ĵ�γ��
	DWORD				dwCenterPointLongitude;					//���ĵ㾭��
	DWORD				dwRadius;								//�뾶
	BYTE				cDatetimeBegin[6];						//��ʼʱ��
	BYTE				cDatetimeEnd[6];						//����ʱ��
	WORD				wMaxSpeed;								//����ٶ�
	BYTE				cOverspeedTime;							//���ٳ���ʱ��
	BYTE				cDummyData;								//�������2
} CIRCULAR_REGION, *PCIRCULAR_REGION;





//////////////////////////////////////////////////////////////////////////
//���Ͷ��нṹ��
typedef struct _SEND_BUFFER
{
	DWORD	dwLength;	//buffer����
	PBYTE	pBuffer;	//buffer��ַ
} SEND_BUFFER, *PSEND_BUFFER;

//����Ϣ�ṹ��
struct decodeUINT
{
	UINT Code;
	LRESULT (*Fxn)(HWND, UINT, WPARAM, LPARAM);
};

//////////////////////////////////////////////////////////////////////////
//GPS info
//������־
typedef struct _ALARM_FLAG
{
	BYTE	cEmergency:1;				//0,	���������������������غ󴥷�	�յ�Ӧ�������
	BYTE	cOverSpeed:1;				//1,	���ٱ���						��־ά���������������
	BYTE	cFatigueDriving:1;			//2,	ƣ�ͼ�ʻ						��־ά���������������
	BYTE	cEarlyWarning:1;			//3,	Ԥ��							�յ�Ӧ�������
	BYTE	cGNSSModuleFailure:1;		//4,	GNSSģ�鷢������				��־ά���������������
	BYTE	cGNSSAntennaDetach:1;		//5,	GNSS����δ�ӻ򱻼���			��־ά���������������
	BYTE	cGNSSAntennaShort:1;		//6,	GNSS����δ�ӻ򱻼���			��־ά���������������
	BYTE	cLowVoltage:1;				//7,	�ն�����ԴǷѹ					��־ά���������������
	BYTE	cNoExternalPower:1;			//8,	�ն�����Դ����					��־ά���������������
	BYTE	cDisplayFailure:1;			//9,	�ն�LCD����ʾ������				��־ά���������������
	BYTE	cTTSFailure:1;				//10,	TTSģ�����						��־ά���������������
	BYTE	cCameraFailure:1;			//11,	����ͷ����						��־ά���������������
	BYTE	cDummyData1:6;				//12~17,�������1
	BYTE	cDriveOverTime:1;			//18,	�����ۼƼ�ʻ��ʱ				��־ά���������������
	BYTE	cParkingOverTime:1;			//19,	��ʱͣ��						��־ά���������������
	BYTE	cRegionInOut:1;				//20,	��������						�յ�Ӧ�������
	BYTE	cLineInOut:1;				//21,	����·��						�յ�Ӧ�������
	BYTE	cDriveTimeFail:1;			//22,	·����ʻʱ�䲻��/����			�յ�Ӧ�������
	BYTE	cDriftAlarm:1;				//23,	·��ƫ�뱨��					��־ά���������������
	BYTE	cVSSFailure:1;				//24,	����VSS����						��־ά���������������
	BYTE	cOilMassAbnomity:1;			//25,	���������쳣					��־ά���������������
	BYTE	cVehicleStolen:1;			//26,	��������						��־ά���������������
	BYTE	cIllegalIgnition:1;			//27,	�����Ƿ����					�յ�Ӧ�������
	BYTE	cIllegalDisplacement:1;		//28,	�����Ƿ�λ��					��־ά���������������
	BYTE	cCollisionAlarm:1;			//29,	��ײ�෭����					�յ�Ӧ�������
	BYTE	cDummyData2:2;				//30~31,�������2
} ALARM_FLAG, *PALARM_FLAG;

//������־
typedef union _unALARM_FLAG
{
	DWORD			dwAlarmFlag;
	ALARM_FLAG		alarm_flag;
} unALARM_FLAG;

//GPS״̬��Ϣ
typedef struct _GPS_STATE
{
	BYTE	cACCState:1;			//0,	0:ACC��			1:ACC��
	BYTE	cFix:1;					//1,	0:δ��λ		1:��λ
	BYTE	cLatitudeType:1;		//2,	0:��γ			1:��γ
	BYTE	cLongitudeType:1;		//3,	0:����			1:����
	BYTE	cActive:1;				//4,	0:��Ӫ״̬		1:ͣ��״̬
	BYTE	cEncript:1;				//5,	0:��γ��δ�����ܲ������	1:��γ���Ѿ����ܲ������
	BYTE	cDummyData1:4;			//6~9,	�������1
	BYTE	cVehicleOil:1;			//10,	0:������·����	1:������·�Ͽ�
	BYTE	cVehicleCircuit:1;		//11,	0:������·����	1:������·�Ͽ�
	BYTE	cDoorLock:1;			//12,	0:���Ž���		1:���ż���
	DWORD	dwDummyData2:19;		//13~31,�������2
} GPS_STATE, *PGPS_STATE;

//GPS״̬��Ϣ
typedef union _unGPS_STATE
{
	DWORD			dwGPSState;
	GPS_STATE		gps_state;
} unGPS_STATE;

//λ�û�����Ϣ
typedef struct _GPS_BASIC_INFO
{
	unALARM_FLAG	unAlarm;		//������־
	unGPS_STATE		unState;		//״̬
	DWORD			dwLatitude;		//γ��:�Զ�Ϊ��λ��γ��ֵ����10��6�η�����ȷ�������֮һ��
	DWORD			dwLongitude;	//����:�Զ�Ϊ��λ�ľ���ֵ����10��6�η�����ȷ�������֮һ��
	WORD			wAltitude;		//�߳�:���θ߶ȣ���λ��
	WORD			wSpeed;			//�ٶ�:1/10km/h
	WORD			wDriection;		//����:0��359������Ϊ0��˳ʱ��
	BYTE			cDateTime[6];	//ʱ��:BCD:YYMMDDhhmmss(GMT+8)
} GPS_BASIC_INFO, *PGPS_BASIC_INFO;

//λ�ø�����Ϣ��ID
enum GPSExtraInfoID
{
	ID_Mileage				= 0x01,		//���,DWORD,1/10km,���ڳ�����̱����
	ID_OilMass				= 0x02,		//����,DWORD,1/10L,���ڳ������������
	ID_Speed				= 0x03,		//��ʻ��¼���ܻ�ȡ���ٶȣ�WORD��1/10km/h
	//0x04��0xF	����
	ID_AlarmOverSpeed		= 0x11,		//���ٱ���������Ϣ
	ID_AlarmInOutRegionLine	= 0x12,		//��������/��·����������Ϣ
	ID_AlarmDriveTime		= 0x13,		//·����ʻʱ�䲻��/��������������Ϣ
};

enum enPOS_TYPE
{
	enNO_GIVEN_POS			= 0,		//���ض�λ��
	enCIRCULAR_REGION		= 1,		//Բ������
	enRECTANGULAR			= 2,		//��������
	enPOLYGON				= 3,		//���������
	enLINE					= 4			//·��
};

//���ٱ���������Ϣ
typedef struct _ALARM_OVER_SPEED
{
	enPOS_TYPE		enPosType;			//λ������:	0:���ض�λ�� 1:Բ������ 2:�������� 3:��������� 4:·��
	DWORD			dwRegionLineID;		//�����·��ID����λ������Ϊ0���޸��ֶ�
} ALARM_OVER_SPEED, *PALARM_OVER_SPEED;

//��������/��·����������Ϣ
typedef struct _ALARM_INOUT_REGION_LINE
{
	enPOS_TYPE		enPosType;			//λ������:	1:Բ������ 2:�������� 3:��������� 4:·��
	BYTE			cDirection;			//����0:�� 1:��
	BYTE			cDummyData[3];		//�������
	DWORD			dwRegionLineID;		//�����·��ID
} ALARM_INOUT_REGION_LINE, *PALARM_INOUT_REGION_LINE;

//·����ʻʱ�䲻��/��������������Ϣ
typedef struct _ALARM_DRIVE_TIME
{
	DWORD			dwLineID;		//·��ID
	WORD			wDriveTime;		//·����ʻʱ�䣬��λΪ��(s)
	BYTE			cResult;		//���  0:����  1:����
	BYTE			cDummyData;		//�������
} ALARM_DRIVE_TIME, *PALARM_DRIVE_TIME;

//λ�ø�����Ϣ��
typedef struct _GPS_EXTRA_INFO
{
	DWORD					dwMileage;			//���
	DWORD					dwOilMass;			//����
	WORD					wSpeed;				//��ʻ��¼���ܻ�ȡ���ٶ�
	WORD					wDummyData;			//�������
	ALARM_OVER_SPEED		over_speed;			//���ٱ���������Ϣ
	ALARM_INOUT_REGION_LINE	inout_region_line;	//��������/��·����������Ϣ
	ALARM_DRIVE_TIME		drive_time;			//·����ʻʱ�䲻��/��������������Ϣ
} GPS_EXTRA_INFO, *PGPS_EXTRA_INFO;

//λ�û㱨��Ϣ
typedef struct _GPS_INFO
{
	GPS_BASIC_INFO			gps_basic_info;		//λ�û�����Ϣ
	GPS_EXTRA_INFO			gps_extra_info;		//λ�ø�����Ϣ
}GPS_INFO, *PGPS_INFO;

//ͨ��Ӧ����
typedef struct _COMMON_ACK_RESULT
{
	WORD	wMsgID;			//�ο�enMSG_ID
	WORD	wResult;		//�ο�enResult
} COMMON_ACK_RESULT;

typedef union _unCOMMON_ACK_RESULT
{
	COMMON_ACK_RESULT	ack_result;						//��־
	DWORD				dwAck_result;
} unCOMMON_ACK_RESULT;

//��Ϣ�㲥/ȡ��
typedef struct _COMMON_DEMAND
{
	WORD	wMsgType;		//����
	WORD	wMsgTag;		//��־
} COMMON_DEMAND;

typedef union _unCOMMON_DEMAND
{
	COMMON_DEMAND		demand_result;		//��־
	DWORD				dwDemand_result;
} unCOMMON_DEMAND;