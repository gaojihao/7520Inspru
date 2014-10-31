#ifndef __FILEMAPPING_TIANZE__
#define __FILEMAPPING_TIANZE__

#define	FILE_MAPPING_NAME_TIANZE				_T("FILE_MAPPING_TIANZE")

//内存共享区，GSM_AT.exe & gsm.dll工程交换数据，不同进程间共享
typedef struct 
{
	TCHAR		szIMSI[32];						//通信模块IMSI号码
	TCHAR		szIMEI[32];						//通信模块IMEI号码
	unsigned	nSignal;						//信号量
}stShareMemoryTianze;

//共享文件映射
extern HANDLE				g_hMemShareTianze;			//共享内存句柄
extern stShareMemoryTianze*	g_pstShareMemTianze;		//共享内存区指针

BOOL InitFileMappingTianze();
BOOL UnInitFileMappingTianze();

#endif//__FILEMAPPING_TIANZE__

