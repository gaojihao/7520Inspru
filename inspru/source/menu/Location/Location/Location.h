// Location.h : main header file for the Location DLL
//

#pragma once

//#include "UsCommDef.h"

#ifdef LOCATION_EXPORTS
#define LOCATION_API  __declspec(dllexport)
#else
#define LOCATION_API  __declspec(dllimport)
#endif

//���������
#define MAX_SATE_NUM				12
//ģ��λ�����ݸ�����Ϣ�ַ�
#define WM_POS_DATA_REFRESH			_T("WM_POS_DATA_REFRESH")

enum enPOS_DATA_MODE
{
	MODE_SGL_BD2	= 0,			//��BD2
	MODE_SGL_GPS,					//��GPS
	MODE_DBL_BD2,					//BD2/GPS˫ģ, ѡ����BD2����
	MODE_DBL_GPS,					//BD2/GPS˫ģ, ѡ����GPS����
};

//��λ״̬��Ϣ
typedef struct _POS_STATE
{
	BYTE	cFix:1;					//1,	0:Vδ��λ		1:A��λ
	BYTE	cLatitudeType:1;		//2,	0:N��γ			1:S��γ
	BYTE	cLongitudeType:1;		//3,	0:E����			1:W����
	DWORD	dwSateNum:29;			//���ӵ����Ǹ���(GSV�����Ǹ���)
} POS_STATE, *PPOS_STATE;

//��򵼺���������
typedef struct _RMC_DATA
{
	POS_STATE		pos_state;		//״̬
	double			dbLatitude;		//γ��:��
	double			dbLongitude;	//����:��
	double			dbAltitude;		//���θ߶�:-9999.9~99999.9
	double			dbSpeed;		//�����ٶ�:km/h
	double			dbAzimuth;		//���溽��:��, ���汱Ϊ�ο���׼����˳ʱ�뷽��������ĽǶȡ�
	SYSTEMTIME		UtcTime;		//utcʱ��:������ʱ����
} RMC_DATA, *PRMC_DATA;

//�������Ӻ���Ч���Ǻ�
typedef struct _GSA_DATA
{
	double			dbPdop;			//PDOPλ�þ������ӣ�0.5~99.9��
	double			dbHdop;			//HDOPˮƽ�������ӣ�0.5~99.9��
	double			dbVdop;			//VDOP��ֱ�������ӣ�0.5~99.9��
	WORD			wSatNum;		//�������Ǹ���
	WORD			wUsedID[MAX_SATE_NUM];	//��������id
} GSA_DATA, *PGSA_DATA;

//���ӵ�����״̬
typedef struct _GSV_DATA
{
	WORD			wSatID;			//���Ǳ��
	WORD			wElevation;		//��������:��
	WORD			wAzimuth;		//���Ƿ�λ��:��
	WORD			wSNR;			//�����signal-to-noise performance .0~99,-1Ϊ��Ч	dB-Hz
	BOOL			bAvailible;		//�Ƿ���Ч
} GSV_DATA, *PGSV_DATA;

//���Ƕ�λ����
typedef struct _POS_INFO
{
	RMC_DATA	rmc_data;
	GSA_DATA	gsa_data;
	GSV_DATA	gsv_data[MAX_SATE_NUM];

	_POS_INFO()		//��ʼ��
	{
		memset(&rmc_data, NULL, sizeof(rmc_data));
		memset(&gsa_data, NULL, sizeof(gsa_data));
		memset(gsv_data, NULL, sizeof(gsv_data));
	}
} POS_INFO, *PPOS_INFO;

//�򿪴���  TRUE���򿪳ɹ�
LOCATION_API BOOL Open(WORD wPosCom, WORD wBaudRate, enPOS_DATA_MODE data_mode = MODE_SGL_GPS);

//�رմ���	TRUE���رճɹ�
LOCATION_API BOOL Close();

//����BD2/GPSģ������ģʽ
LOCATION_API BOOL SetDataMode(enPOS_DATA_MODE data_mode);

//��ȡBD2/GPSģ������ģʽ
LOCATION_API enPOS_DATA_MODE GetDataMode();

//��ȡ���λ����Ϣ�Ĺ����ڴ�
LOCATION_API const POS_INFO* GetSharePosInfo();

//��Ӵ��ھ�����Ի�ȡλ����Ϣ��Ϣ
//LOCATION_API BOOL AddHWND(HWND hWnd);

//ɾ�����ھ��
//LOCATION_API void RemoveHWND(HWND hWnd);

//ʹ��ת��GPS����
LOCATION_API BOOL EnableConvertGPS(WORD wConvertCom);

//Disableת��GPS����
LOCATION_API void DisableConvertGPS();
