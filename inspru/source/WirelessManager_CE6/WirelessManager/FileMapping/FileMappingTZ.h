#ifndef __FILEMAPPING_TIANZE__
#define __FILEMAPPING_TIANZE__

#define	FILE_MAPPING_NAME_TIANZE				_T("FILE_MAPPING_TIANZE")

//�ڴ湲������GSM_AT.exe & gsm.dll���̽������ݣ���ͬ���̼乲��
typedef struct 
{
	TCHAR		szIMSI[32];						//ͨ��ģ��IMSI����
	TCHAR		szIMEI[32];						//ͨ��ģ��IMEI����
	unsigned	nSignal;						//�ź���
}stShareMemoryTianze;

//�����ļ�ӳ��
extern HANDLE				g_hMemShareTianze;			//�����ڴ���
extern stShareMemoryTianze*	g_pstShareMemTianze;		//�����ڴ���ָ��

BOOL InitFileMappingTianze();
BOOL UnInitFileMappingTianze();

#endif//__FILEMAPPING_TIANZE__

