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


//����socket buffer��󳤶�
#define MSG_BUFFER_LEN					1280
#define NUM_256B						256
#define NUM_512B						512
#define NUM_960B						960
#define NUM_1K							1024
#define NUM_2K							(1024*2)

#define IOCTRL_DEVICE_PWROff			(3033)
#define IOCTRL_SYSTEM_PowrOff		       (3039)
#define IOCTRL_SYSTEM_RESET		        (3038)


//���������ά��
#define dim(x)							(sizeof(x) / sizeof(x[0]))

#define MSG_WND_NAME L"Guider_MsgWnd"	// ��Ϣ���մ�������
//config.iniϵͳ�����ļ�·��
#define	PATH_CONFIG_INI					_T("\\Flashdrv Storage\\MENU\\config.ini")
//TerminalParam.iniϵͳ�����ļ�·��
#define	PATH_PARAM_INI					_T("\\Flashdrv Storage\\MENU\\TerminalParam.ini")
#define WNDTITLE_CAMERA_VIDEO				_T("CAMERA_VIDEO_20110411")

#define	MenuApp							_T("WND_MAIN_MENU")

#define PATH_GNSS							_T("\\Flashdrv Storage\\Menu\\GNSSTerminal.exe")		//GNSSԴ�ļ�
#define PATH_GNSS_BAK						_T("\\Flashdrv Storage\\Menu\\GNSSTerminal_Bak.exe")	//GNSS�������ļ�

#define PATH_UPGRADE_ZIP					_T("\\Flashdrv Storage\\Upgrade.zip")					//ftp���ص������ļ�
#define PATH_UPGRADE_EXE					_T("\\Flashdrv Storage\\GNSSTerminal.exe")				//��ѹ��������ļ�
#define PATH_BLIND_TXT						_T("\\Flashdrv Storage\\Menu\\BlindData.txt")			//ä�����ݱ�����ʱ�ļ�


#define PATH_ROUTE1_TXT						_T("\\Flashdrv Storage\\Menu\\Route1.txt")			//��·1�ļ�
#define PATH_ROUTE2_TXT						_T("\\Flashdrv Storage\\Menu\\Route2.txt")			//��·2�ļ�
#define PATH_ROUTE3_TXT						_T("\\Flashdrv Storage\\Menu\\Route3.txt")			//��·3�ļ�

#define PATH_ROAD1_TXT						_T("\\Storage Card\\Guider4u\\Road1.txt")			//�ļ�������·1�ļ�
#define PATH_ROAD2_TXT						_T("\\Storage Card\\Guider4u\\Road2.txt")			//�ļ�������·2�ļ�
#define PATH_ROAD3_TXT						_T("\\Storage Card\\Guider4u\\Road3.txt")			//�ļ�������·3�ļ�


//��Ϣ����
//ͨ��״̬����Ϣ   ����ͨ����Ϣ
#define	WM_COMMAND_TIANZE				(WM_USER+0x1000)
#define	WPARAM_SERVER_LOGIN				10					//LPARAM:1 ��¼�ɹ�  0 ����
//���յ����������͵�socket���ݺ�Ĵ�����Ϣ
#define WM_SERVER						(WM_USER+0x4000)
#define WM_ALARM                        0x0171

//ϵͳ�㲥����Ϣ
#define	WM_MESSAGE_SYSTEN				(WM_USER+0x1003)
#define WM_WPARAM_ACC                    0x0013
#define WM_WPARAM_SLEEP                  0x0014

#define HW_CAR_REVERSE					(WM_USER+0x350)//�����෭
#define HW_CAR_HIT              		(WM_USER+0x351)//������ײ
#define HW_BATT_STATE       			(WM_USER+0x360)//�ⲿ��Դ���
#define MSG_RAS_RECONNECT				(WM_USER+0x2008)
#define HW_ACCDET						(WM_USER+0x320)
#define HW_PowerOff						(WM_USER+0x321)

#define MSG_UPGRADE_WPARAM				0x599

#define MSG_COLSE_VIDEO				(WM_USER+0x376)//����ͷ�˳�

#define MSG_TO_GUIDER4U				(WM_USER+0x377)//�յ�����·����֪ͨ�������

#define MSG_OPEN_GUIDER4U			(WM_USER+0x378)//�����صĵ������

#define MSG_TO_OUTSIDEPROCESS		(WM_USER+0x379)//����������ⲿ���̹㲥����ϢID

enum enWPARAM
{
	PARAM_ROUTE_NORMAL		= 0x01,		//1.��·����
	PARAM_ROUTE_YEBOND		= 0x02,		//2.ƫ����·
	PARAM_ARRIVE_TERMINI	= 0x03,		//3.Ŀ�ĵص���
};

enum enMSG_ID
{
	//�ն�(TerMiNal)��ϢID
	MSG_TMN_COMMON_ACK					= 0x0001,	//1.�ն�ͨ��Ӧ��
	MSG_TMN_HEARTBEAT					= 0x0002,	//3.�ն�����
	MSG_TMN_REGISTER					= 0x0100,	//4.�ն�ע��
	MSG_TMN_LOGOUT						= 0x0003,	//6.�ն�ע��
	MSG_TMN_AUTHENTICATION				= 0x0102,	//7.�ն˼�Ȩ
	MSG_TMN_PARAM_GET_ACK				= 0x0104,	//10.��ѯ�ն˲���Ӧ��
	MSG_TMN_UPGRADE_ACK					= 0x0108,	//�ն��������֪ͨ
	MSG_TMN_ATTRIBUTE_ACK				= 0x0107,	//11.��ѯ�ն�����Ӧ��
	MSG_TMN_POS_REPORT					= 0x0200,	//12.λ����Ϣ�㱨
	MSG_TMN_POS_GET_ACK					= 0x0201,	//14.λ����Ϣ��ѯӦ��
	MSG_TMN_EVENT_REPORT				= 0x0301,	//18.�¼�����
	MSG_TMN_QUESTION_ACK				= 0x0302,	//20.����Ӧ��
	MSG_TMN_INFO_DEMAND_CANCEL			= 0x0303,	//22.��Ϣ�㲥/ȡ��
	MSG_TMN_CAR_CONTROL_ACK				= 0x0500,	//27.��������Ӧ��
	MSG_TMN_DRIVE_RECORD_UPLOAD			= 0x0700,	//37.��ʻ��¼�����ϴ�
	MSG_TMN_E_AWB_UPLOAD				= 0x0701,	//39.�����˵��ϱ�(Auto Waybill)
	MSG_TMN_DRIVER_INFO_UPLOAD			= 0x0702,	//40.��ʻԱ�����Ϣ�ɼ��ϱ�
	MSG_SATELLITE_DATAUPLAOD           = 0x0704,   //��λ���������ϴ�
	MSG_CAN_DATAUPLOAD                = 0x0705,   //CAN���������ϴ�
	MSG_TMN_MULTIMEDIA_CMD				= 0x0800,	//41.��ý���¼���Ϣ�ϴ�
	MSG_TMN_MULTIMEDIA_UPLOAD			= 0x0801,	//42.��ý�������ϴ�
	MSG_TMN_MULTIMEDIA_SEARCH_ACK		= 0x0802,	//46.�洢��ý�����ݼ���Ӧ��
	MSG_TMN_TAKERHOTO_ACK				= 0x0805,	//47.����ͷ��������Ӧ��
	MSG_TMN_DATA_UPLOAD					= 0x0900,	//51.��������͸��
	MSG_TMN_COMPRESS_DATA_UPLOAD		= 0x0901,	//52.����ѹ���ϱ�
	MSG_TMN_RSA_PUBLIC_KEY				= 0x0A00,	//54.�ն�RSA��Կ
	MSG_TMN_TEXT_SMS					= 0x0300,	//��1.�ı���Ϣ�ϴ�

	//������ƽ̨(SerVeR)��ϢID
	MSG_SVR_COMMON_ACK					= 0x8001,	//2.ƽ̨ͨ��Ӧ��
	MSG_SVR_SUBCONTRACT					= 0x8003,	//3.�����ְ�����
	MSG_SVR_REGISTER_ACK				= 0x8100,	//5.�ն�ע��Ӧ��
	MSG_SVR_PARAM_SET					= 0x8103,	//8.�����ն˲���
	MSG_SVR_PARAM_GET					= 0x8104,	//9.��ѯ�ն˲���
	MSG_SVR_TMN_CONTROL					= 0x8105,	//11.�ն˿���
	MSG_SVR_TMN_APPOINT_PARAMETER		= 0x8106,	//12.��ѯָ���ն˲���
	MSG_SVR_TMN_ATTRIBUTE				= 0x8107,	//...��ѯ�ն�����
	MSG_SVR_POS_GET						= 0x8201,	//13.λ����Ϣ��ѯ
	MSG_SVR_TEMP_POS_TRACE				= 0x8202,	//15.��ʱλ�ø��ٿ���
	MSG_SVR_ALARM_SMS					= 0x8203,	//...�˹�ȷ�ϱ�����Ϣ
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
	MSG_SVR_REQUST_DRIVER_INFO		    = 0x8702,	//39.�ϱ���ʻԱ�����Ϣ����
	MSG_SVR_MULTIMEDIA_UPLOAD_ACK		= 0x8800,	//43.��ý�������ϴ�Ӧ��
	MSG_SVR_CAMERA_PHOTOGRAPH			= 0x8801,	//44.����ͷ������������
	MSG_SVR_MULTIMEDIA_SEARCH			= 0x8802,	//45.�洢��ý�����ݼ���
	MSG_SVR_MULTIMEDIA_UPLOAD			= 0x8803,	//47.�洢��ý�������ϴ�����
	MSG_SVR_SOUND_RECORD				= 0x8804,	//48.¼����ʼ����
	MSG_SVR_SINGLE_MULT_SEARCH			= 0x8805,	//49.�����洢��ý�����ݼ����ϴ�����
	MSG_SVR_DATA_DOWNLOAD				= 0x8900,	//50.��������͸��
	MSG_SVR_RSA_PUBLIC_KEY				= 0x8A00,	//53.ƽ̨RSA��Կ
	MSG_TMN_DESTINATION_NAVI			= 0x8F01,	//��2.ƽ̨�·�"Ŀ�ĵص���"ָ��
	MSG_TMN_TIME_ADJUST_REQUEST			= 0x0FF0,	//�ն�����Уʱ
	MSG_SVR_TIME_ADJUST					= 0x8FF0,	//�ظ�Уʱ
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
	PARAM_TIMER_TAKEPHOTO				= 0x0064,	//��ʱ����
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
	PARAM_GNSS_MODE                   = 0x0090,   //��λģʽ
	PARAM_GNSS_BANDRATE               = 0x0091,   //GNSS������
	PARAM_GNSS_OUTPUTRATE				= 0x0092,   //Gnss���Ƶ��
	PARAM_GNSS_PICKUPRATE				= 0x0093,   //Gnss�ɼ�Ƶ��
	PARAM_GNSS_UPLOADMODE				= 0x0094,   //Gnss�����ϴ���ʽ
	PARAM_GNSS_PICKUPSET				= 0x0095,   //Gnss�����ϴ�����
	PARAM_CAN1_PICKYTIME				= 0x0100,   //CAN1���߲ɼ�ʱ����
	PARAM_CAN1_UPLOADTIME				= 0x0101,   //CAN1�����ϴ�ʱ����
	PARAM_CAN2_PICKYTIME              = 0x0102,   //CAN2���߲ɼ�ʱ����
	PARAM_CAN2_UPLOADTIME             = 0x0103,   //CAN2�����ϴ�ʱ����
	PARAM_CAN_PICKUPSET               = 0x0110,   //CAN����ID�����ɼ�����
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
	WORD	wELECTRONIC_FENCE_RADIUS;	//(0x0031)����Χ���뾶���Ƿ�λ����ֵ������λΪ��  Radius of the electronic fence
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
	DWORD	dwTimer_Take_Photo;			//(0x0064)��ʱ����
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
	BYTE    byGnssfixMode;				//(0x0090)Gnss��λģʽ
	BYTE    byGnssBandrate;				//(0x0091)Gnss������
	BYTE    byGnssfrequency;			//(0x0092)Gnss�������Ƶ��
	DWORD   dwGnssPickupfrequency;      //(0x0093)Gnss���ݲɼ�Ƶ��
	BYTE    byGnssUploadMode;			//(0x0094)Gnss�ϴ���ʽ
	DWORD   dwGnssPickupSet;            //(0x0095)Gnss�����ϴ�����
	DWORD   dwCan1PickTime;             //(0x0100)CAN1�ɼ�ʱ����
	WORD    wCan1UploadTime;            //(0x0101)CAN1�ϴ�ʱ����
	DWORD   dwCan2PickTime;             //(0x0102)CAN2�ɼ�ʱ����
	WORD    wCan2UploadTime;            //(0x0103)CAN2�ϴ�ʱ����
	char    byCanPickSet[16];            //(0x0110)CAN����ID�����ɼ�����
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
};

typedef struct {
	char SCA[16];       // ����Ϣ�������ĺ���(SMSC��ַ)
	char TPA[16];       // Ŀ������ظ�����(TP-DA��TP-RA)
	char TP_PID;        // �û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];   // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	char index;         // ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

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


typedef struct _RECT_REGION
{
	DWORD				dwRegionID;								//����ID
	unREGION_PROPERTY	unRegionProperty;						//��������
	DWORD				dwlefttopLatitude;						//���ϵ�γ��
	DWORD				dwlefttopLongitude;						//���ϵ㾭��
	DWORD				dwrightbuttomLatitude;					//���µ�γ��
	DWORD				dwrightbuttomLongitude;					//���µ㾭��
} RECT_REGION, *PRECT_REGION;


typedef struct _ROUTE_INFO
{
	DWORD				dwInflexionID;							//�յ�ID
	DWORD				dwRouteID;								//·��ID
	DWORD				dwInflexionLat;							//�յ�γ��
	DWORD				dwInflexionLong;						//�յ㾭��
	BYTE              byRoutWidth;                        //·�ο��
} ROUTE_INFO, *PROUTE_INFO;



typedef struct _BLIND_DATA
{
	DWORD	dwLength;	//buffer����
	BYTE	Buffer[32];	//buffer��ַ
} BLIND_DATA;

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
	BYTE	cICCardFailure:1;			//12,	IC������						��־ά���������������
	BYTE	cOverSpeedwarning:1;		//13,	����Ԥ��						��־ά���������������
	BYTE	cTiredDriverwarning:1;		//14,	ƣ�ͼ�ʻԤ��					��־ά���������������
	BYTE	cDummyData1:1;				//15,	�������1
	BYTE	cDummyData2:1;				//16,	�������2
	BYTE	cGPSSignIn:1;				//17,	GPSǩ��							�յ�Ӧ�������
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
	BYTE	cIllegalDisplacement:1;		//28,	�����Ƿ�λ��					�յ�Ӧ�������
	BYTE	cCollisionAlarm:1;			//29,	��ײ�෭����					��־ά���������������
	BYTE	cHitAlarm:1;				//30,	��ײ����						��־ά���������������
	BYTE	cIllegalOpenDoor:1;			//31,	�Ƿ����ű���					�յ�Ӧ�������
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

//������ʶ��״̬λ
typedef struct _GPS_ALARM_STATE
{
	unALARM_FLAG	unAlarm;		//������־
	unGPS_STATE		unState;		//״̬
	GPS_EXTRA_INFO	gps_extra_info;	//λ�ø�����Ϣ
}GPS_ALARM_STATE, *PGPS_ALARM_STATE;

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

//�ն�����
typedef struct _TERMINAL_TYPE
{
	BYTE	cPassengerCar:1;			//0,	0:�����ÿ��˳���,		1:���ÿ��˳���;
	BYTE	cDangerousgoodsCar:1;		//1,	0:������Σ��Ʒ����,		1:����Σ��Ʒ����;
	BYTE	cGeneralcargo:1;			//2,	0:��������ͨ���˳���,	1:������ͨ���˳���;
	BYTE	cTaxi:1;					//3,	0:�����ó��⳵��,		1:���ó��⳵��;
	BYTE	cDummyData1:2;				//4~5,	�������1;
	BYTE	charddiskVideo:1;			//6,	0:��֧��Ӳ��¼��,		1:֧��Ӳ��¼��;
	BYTE	cAIO:1;						//7,	0:һ���,				1:�����.
} TERMINAL_TYPE, *PTERMINAL_TYPE;

//�ն�����
typedef union _unTERMINAL_TYPE
{
	DWORD			dwTerminaltypeFlag;
	TERMINAL_TYPE	Terminaltype_flag;
} unTERMINAL_TYPE;

//�ն�������Ϣ
typedef struct _MSG_TMN_ATTRIBUTE
{
	unTERMINAL_TYPE		unTerminaltype;			//�ն�����
	BYTE				unGnssattribute[4];		//GNSS ģ������
	BYTE				unModule[8];			//ͨ��ģ������
} TMN_ATTRIBUTE, *PTMN_ATTRIBUTE;





//ͼƬ���ݽṹ��
typedef struct _Subcontract_Data
{
	WORD		wBodylen;			//������
	WORD		wTotalnum;			//�ܰ���
	WORD		nSubnum;			//�ְ����
	BYTE		nData[NUM_1K];		//��Ϣ��
} Subcontract_Data;

//ä�����ݽṹ��
typedef struct _Blind_Data
{
	WORD		wBodylen;			//������
	BYTE		nData[65];		//��Ϣ��
} Blind_Data;

struct RouteSet_Data
{
	WORD		wPackIndex;		//�����
	WORD		wBodylen;		//������
	BYTE		nRouteData[1024];		//��Ϣ��
};


typedef struct _Illegal_DisplaceMent_ALARM
{
	double	dPointLat;		//��¼��ά��
	double	dPointLong;		//��¼�ľ���
	WORD	wDis;
	bool	bSet;			//�Ƿ������˷Ƿ�λ�Ʊ���
}Illegal_DisplaceMent_ALARM;

#define FILE_DEVICE_MCU       		0x0000800D
#define METHOD_BUFFERED				0
#define FILE_ANY_ACCESS				0
#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define IOCTL_MCU_CAN_SET  		CTL_CODE(FILE_DEVICE_MCU, 16, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MCU_CAN_READ  	CTL_CODE(FILE_DEVICE_MCU, 17, METHOD_BUFFERED, FILE_ANY_ACCESS)