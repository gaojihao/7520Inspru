// Location.h : main header file for the Location DLL
//

#pragma once

//#include "UsCommDef.h"

#ifdef LOCATION_EXPORTS
#define LOCATION_API  __declspec(dllexport)
#else
#define LOCATION_API  __declspec(dllimport)
#endif

//最大卫星数
#define MAX_SATE_NUM				12
//模块位置数据更新消息字符
#define WM_POS_DATA_REFRESH			_T("WM_POS_DATA_REFRESH")

enum enPOS_DATA_MODE
{
	MODE_SGL_BD2	= 0,			//单BD2
	MODE_SGL_GPS,					//单GPS
	MODE_DBL_BD2,					//BD2/GPS双模, 选择处理BD2数据
	MODE_DBL_GPS,					//BD2/GPS双模, 选择处理GPS数据
};

//定位状态信息
typedef struct _POS_STATE
{
	BYTE	cFix:1;					//1,	0:V未定位		1:A定位
	BYTE	cLatitudeType:1;		//2,	0:N北纬			1:S南纬
	BYTE	cLongitudeType:1;		//3,	0:E东经			1:W西经
	DWORD	dwSateNum:29;			//可视的卫星个数(GSV中卫星个数)
} POS_STATE, *PPOS_STATE;

//最简导航传输数据
typedef struct _RMC_DATA
{
	POS_STATE		pos_state;		//状态
	double			dbLatitude;		//纬度:度
	double			dbLongitude;	//经度:度
	double			dbAltitude;		//海拔高度:-9999.9~99999.9
	double			dbSpeed;		//地面速度:km/h
	double			dbAzimuth;		//地面航向:度, 以真北为参考基准，沿顺时针方向至航向的角度。
	SYSTEMTIME		UtcTime;		//utc时间:年月日时分秒
} RMC_DATA, *PRMC_DATA;

//精度因子和有效卫星号
typedef struct _GSA_DATA
{
	double			dbPdop;			//PDOP位置精度因子（0.5~99.9）
	double			dbHdop;			//HDOP水平精度因子（0.5~99.9）
	double			dbVdop;			//VDOP垂直精度因子（0.5~99.9）
	WORD			wSatNum;		//锁定卫星个数
	WORD			wUsedID[MAX_SATE_NUM];	//锁定卫星id
} GSA_DATA, *PGSA_DATA;

//可视的卫星状态
typedef struct _GSV_DATA
{
	WORD			wSatID;			//卫星编号
	WORD			wElevation;		//卫星仰角:度
	WORD			wAzimuth;		//卫星方位角:度
	WORD			wSNR;			//信噪比signal-to-noise performance .0~99,-1为无效	dB-Hz
	BOOL			bAvailible;		//是否有效
} GSV_DATA, *PGSV_DATA;

//卫星定位数据
typedef struct _POS_INFO
{
	RMC_DATA	rmc_data;
	GSA_DATA	gsa_data;
	GSV_DATA	gsv_data[MAX_SATE_NUM];

	_POS_INFO()		//初始化
	{
		memset(&rmc_data, NULL, sizeof(rmc_data));
		memset(&gsa_data, NULL, sizeof(gsa_data));
		memset(gsv_data, NULL, sizeof(gsv_data));
	}
} POS_INFO, *PPOS_INFO;

//打开串口  TRUE－打开成功
LOCATION_API BOOL Open(WORD wPosCom, WORD wBaudRate, enPOS_DATA_MODE data_mode = MODE_SGL_GPS);

//关闭串口	TRUE－关闭成功
LOCATION_API BOOL Close();

//设置BD2/GPS模块数据模式
LOCATION_API BOOL SetDataMode(enPOS_DATA_MODE data_mode);

//获取BD2/GPS模块数据模式
LOCATION_API enPOS_DATA_MODE GetDataMode();

//获取存放位置信息的共享内存
LOCATION_API const POS_INFO* GetSharePosInfo();

//添加窗口句柄，以获取位置信息消息
//LOCATION_API BOOL AddHWND(HWND hWnd);

//删除窗口句柄
//LOCATION_API void RemoveHWND(HWND hWnd);

//使能转换GPS数据
LOCATION_API BOOL EnableConvertGPS(WORD wConvertCom);

//Disable转换GPS数据
LOCATION_API void DisableConvertGPS();
