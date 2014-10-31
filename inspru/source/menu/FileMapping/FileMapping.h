#ifndef __FILEMAPPING__
#define __FILEMAPPING__

//#include "../DriveLib/VehicleInfo.h"
#include "../DriveLib/VehicleInfo.h"

#define	FILE_MAPPING_NAME					_T("FILE_MAPPING_MENU_GSM")

typedef struct
{
	unsigned		m_longitude;			//本节点经度
	unsigned		m_latitude;				//本节点纬度
	unsigned char	m_location;				//是否定位  0定位, 1未定位
	unsigned char	m_isBlindArea;			//是否盲区  ras拨号连接状态  0未连接   1已连接
	unsigned char	m_speed;				//速度
	unsigned char	m_direction;			//方向
	SYSTEMTIME		m_sys_time;				//GPS时间
}stGPS_INFO;

//内存共享区，用于同GSM_AT.exe工程交换数据，不同进程间共享
typedef struct 
{
	VEHICLE_INFO	stVehicle;				//汽车信息
	UCHAR			szDriverID[128];		//驾驶员编号
	stGPS_INFO		st_GPS;					//GPS信息
}stShareMemory;

//共享文件映射
extern HANDLE				g_hMemShare;			//共享内存句柄
extern stShareMemory*		g_pstShareMem;			//共享内存区指针

BOOL InitFileMapping();
BOOL UnInitFileMapping();

#endif//__FILEMAPPING__

