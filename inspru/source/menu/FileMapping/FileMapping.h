#ifndef __FILEMAPPING__
#define __FILEMAPPING__

//#include "../DriveLib/VehicleInfo.h"
#include "../DriveLib/VehicleInfo.h"

#define	FILE_MAPPING_NAME					_T("FILE_MAPPING_MENU_GSM")

typedef struct
{
	unsigned		m_longitude;			//���ڵ㾭��
	unsigned		m_latitude;				//���ڵ�γ��
	unsigned char	m_location;				//�Ƿ�λ  0��λ, 1δ��λ
	unsigned char	m_isBlindArea;			//�Ƿ�ä��  ras��������״̬  0δ����   1������
	unsigned char	m_speed;				//�ٶ�
	unsigned char	m_direction;			//����
	SYSTEMTIME		m_sys_time;				//GPSʱ��
}stGPS_INFO;

//�ڴ湲����������ͬGSM_AT.exe���̽������ݣ���ͬ���̼乲��
typedef struct 
{
	VEHICLE_INFO	stVehicle;				//������Ϣ
	UCHAR			szDriverID[128];		//��ʻԱ���
	stGPS_INFO		st_GPS;					//GPS��Ϣ
}stShareMemory;

//�����ļ�ӳ��
extern HANDLE				g_hMemShare;			//�����ڴ���
extern stShareMemory*		g_pstShareMem;			//�����ڴ���ָ��

BOOL InitFileMapping();
BOOL UnInitFileMapping();

#endif//__FILEMAPPING__

