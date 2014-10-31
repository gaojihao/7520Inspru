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
#define NUM_1K							1024
#define NUM_2K							(1024*2)
#define NUM_3K							(1024*3)
#define NUM_4K							(1024*4)

//��ʱλ�ø��ٿ���0x8202
typedef struct _TEMP_POS_TRACE
{
	WORD	wEnalbe;								//��ʱλ�ø��ٿ����Ƿ���Ч��0:��Ч 1:��Ч
	WORD	wSlice;									//ʱ����
	DWORD	dwPosTraceValidity;						//λ�ø�����Ч��
} TEMP_POS_TRACE, *PTEMP_POS_TRACE;


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
