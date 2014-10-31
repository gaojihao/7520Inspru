/************************************************************************
* Copyright (C)2011, 上海鲲博通讯技术有限公司
* All rights reserved.
* 
* 文件名称: DataType.h
* 文件标识: 
* 摘    要: 
* 作    者: 
* 完成日期: 2011/09/26
* 备注: 
************************************************************************/
#pragma once

//接收socket buffer最大长度
#define MSG_BUFFER_LEN					1280
#define NUM_256B						256
#define NUM_512B						512
#define NUM_1K							1024
#define NUM_2K							(1024*2)
#define NUM_3K							(1024*3)
#define NUM_4K							(1024*4)

//临时位置跟踪控制0x8202
typedef struct _TEMP_POS_TRACE
{
	WORD	wEnalbe;								//临时位置跟踪控制是否有效，0:无效 1:有效
	WORD	wSlice;									//时间间隔
	DWORD	dwPosTraceValidity;						//位置跟踪有效期
} TEMP_POS_TRACE, *PTEMP_POS_TRACE;


//区域属性
typedef struct _REGION_PROPERTY
{
	BYTE	cAccord2Time:1;			//0,	0:			1:根据时间
	BYTE	cSpeedLimit:1;			//1,	0:			1:限速
	BYTE	cInRegion:1;			//2,	0:			1:进区域报警给驾驶员
	BYTE	cDummyData1:2;			//3~4,	填充数据1
	BYTE	cOutRegion:1;			//5,	0:			1:出区域报警给平台
	BYTE	cLatitudeType:1;		//6,	0:北纬		1:南纬
	BYTE	cLongitudeType:1;		//7,	0:东经		1:西经
	BYTE	cDummyData;				//填充数据
} REGION_PROPERTY, *PREGION_PROPERTY;

//区域属性
typedef union _unREGION_PROPERTY
{
	WORD				wRegionProperty;
	REGION_PROPERTY		region_property;
} unREGION_PROPERTY;

//设置圆形区域0x8600
typedef struct _CIRCULAR_REGION
{
	DWORD				dwRegionID;								//区域ID
	unREGION_PROPERTY	unRegionProperty;						//区域属性
	WORD				wDummyData;								//填充数据1
	DWORD				dwCenterPointLatitude;					//中心点纬度
	DWORD				dwCenterPointLongitude;					//中心点经度
	DWORD				dwRadius;								//半径
	BYTE				cDatetimeBegin[6];						//起始时间
	BYTE				cDatetimeEnd[6];						//结束时间
	WORD				wMaxSpeed;								//最高速度
	BYTE				cOverspeedTime;							//超速持续时间
	BYTE				cDummyData;								//填充数据2
} CIRCULAR_REGION, *PCIRCULAR_REGION;





//////////////////////////////////////////////////////////////////////////
//发送队列结构体
typedef struct _SEND_BUFFER
{
	DWORD	dwLength;	//buffer长度
	PBYTE	pBuffer;	//buffer地址
} SEND_BUFFER, *PSEND_BUFFER;

//主消息结构体
struct decodeUINT
{
	UINT Code;
	LRESULT (*Fxn)(HWND, UINT, WPARAM, LPARAM);
};

//////////////////////////////////////////////////////////////////////////
//GPS info
//报警标志
typedef struct _ALARM_FLAG
{
	BYTE	cEmergency:1;				//0,	紧急报警，触动报警开关后触发	收到应答后清零
	BYTE	cOverSpeed:1;				//1,	超速报警						标志维持至报警条件解除
	BYTE	cFatigueDriving:1;			//2,	疲劳驾驶						标志维持至报警条件解除
	BYTE	cEarlyWarning:1;			//3,	预警							收到应答后清零
	BYTE	cGNSSModuleFailure:1;		//4,	GNSS模块发生故障				标志维持至报警条件解除
	BYTE	cGNSSAntennaDetach:1;		//5,	GNSS天线未接或被剪断			标志维持至报警条件解除
	BYTE	cGNSSAntennaShort:1;		//6,	GNSS天线未接或被剪断			标志维持至报警条件解除
	BYTE	cLowVoltage:1;				//7,	终端主电源欠压					标志维持至报警条件解除
	BYTE	cNoExternalPower:1;			//8,	终端主电源掉电					标志维持至报警条件解除
	BYTE	cDisplayFailure:1;			//9,	终端LCD或显示器故障				标志维持至报警条件解除
	BYTE	cTTSFailure:1;				//10,	TTS模块故障						标志维持至报警条件解除
	BYTE	cCameraFailure:1;			//11,	摄像头故障						标志维持至报警条件解除
	BYTE	cDummyData1:6;				//12~17,填充数据1
	BYTE	cDriveOverTime:1;			//18,	当天累计驾驶超时				标志维持至报警条件解除
	BYTE	cParkingOverTime:1;			//19,	超时停车						标志维持至报警条件解除
	BYTE	cRegionInOut:1;				//20,	进出区域						收到应答后清零
	BYTE	cLineInOut:1;				//21,	进出路线						收到应答后清零
	BYTE	cDriveTimeFail:1;			//22,	路段行驶时间不足/过长			收到应答后清零
	BYTE	cDriftAlarm:1;				//23,	路线偏离报警					标志维持至报警条件解除
	BYTE	cVSSFailure:1;				//24,	车辆VSS故障						标志维持至报警条件解除
	BYTE	cOilMassAbnomity:1;			//25,	车辆油量异常					标志维持至报警条件解除
	BYTE	cVehicleStolen:1;			//26,	车辆被盗						标志维持至报警条件解除
	BYTE	cIllegalIgnition:1;			//27,	车辆非法点火					收到应答后清零
	BYTE	cIllegalDisplacement:1;		//28,	车辆非法位移					标志维持至报警条件解除
	BYTE	cCollisionAlarm:1;			//29,	碰撞侧翻报警					收到应答后清零
	BYTE	cDummyData2:2;				//30~31,填充数据2
} ALARM_FLAG, *PALARM_FLAG;

//报警标志
typedef union _unALARM_FLAG
{
	DWORD			dwAlarmFlag;
	ALARM_FLAG		alarm_flag;
} unALARM_FLAG;

//GPS状态信息
typedef struct _GPS_STATE
{
	BYTE	cACCState:1;			//0,	0:ACC关			1:ACC开
	BYTE	cFix:1;					//1,	0:未定位		1:定位
	BYTE	cLatitudeType:1;		//2,	0:北纬			1:南纬
	BYTE	cLongitudeType:1;		//3,	0:东经			1:西经
	BYTE	cActive:1;				//4,	0:运营状态		1:停运状态
	BYTE	cEncript:1;				//5,	0:经纬度未经保密插件加密	1:经纬度已经保密插件加密
	BYTE	cDummyData1:4;			//6~9,	填充数据1
	BYTE	cVehicleOil:1;			//10,	0:车辆油路正常	1:车辆油路断开
	BYTE	cVehicleCircuit:1;		//11,	0:车辆电路正常	1:车辆电路断开
	BYTE	cDoorLock:1;			//12,	0:车门解锁		1:车门加锁
	DWORD	dwDummyData2:19;		//13~31,填充数据2
} GPS_STATE, *PGPS_STATE;

//GPS状态信息
typedef union _unGPS_STATE
{
	DWORD			dwGPSState;
	GPS_STATE		gps_state;
} unGPS_STATE;

//位置基本信息
typedef struct _GPS_BASIC_INFO
{
	unALARM_FLAG	unAlarm;		//报警标志
	unGPS_STATE		unState;		//状态
	DWORD			dwLatitude;		//纬度:以度为单位的纬度值乘以10的6次方，精确到百万分之一度
	DWORD			dwLongitude;	//经度:以度为单位的经度值乘以10的6次方，精确到百万分之一度
	WORD			wAltitude;		//高程:海拔高度，单位米
	WORD			wSpeed;			//速度:1/10km/h
	WORD			wDriection;		//方向:0～359，正北为0，顺时针
	BYTE			cDateTime[6];	//时间:BCD:YYMMDDhhmmss(GMT+8)
} GPS_BASIC_INFO, *PGPS_BASIC_INFO;

//位置附加信息项ID
enum GPSExtraInfoID
{
	ID_Mileage				= 0x01,		//里程,DWORD,1/10km,对于车上里程表读数
	ID_OilMass				= 0x02,		//油量,DWORD,1/10L,对于车上油量表读数
	ID_Speed				= 0x03,		//行驶记录功能获取的速度，WORD，1/10km/h
	//0x04～0xF	保留
	ID_AlarmOverSpeed		= 0x11,		//超速报警附加信息
	ID_AlarmInOutRegionLine	= 0x12,		//进出区域/线路报警附加信息
	ID_AlarmDriveTime		= 0x13,		//路段行驶时间不足/过长报警附加信息
};

enum enPOS_TYPE
{
	enNO_GIVEN_POS			= 0,		//无特定位置
	enCIRCULAR_REGION		= 1,		//圆形区域
	enRECTANGULAR			= 2,		//矩形区域
	enPOLYGON				= 3,		//多边形区域
	enLINE					= 4			//路段
};

//超速报警附加信息
typedef struct _ALARM_OVER_SPEED
{
	enPOS_TYPE		enPosType;			//位置类型:	0:无特定位置 1:圆形区域 2:矩形区域 3:多边形区域 4:路段
	DWORD			dwRegionLineID;		//区域或路段ID，若位置类型为0，无该字段
} ALARM_OVER_SPEED, *PALARM_OVER_SPEED;

//进出区域/线路报警附加信息
typedef struct _ALARM_INOUT_REGION_LINE
{
	enPOS_TYPE		enPosType;			//位置类型:	1:圆形区域 2:矩形区域 3:多边形区域 4:路段
	BYTE			cDirection;			//方向：0:进 1:出
	BYTE			cDummyData[3];		//填充数据
	DWORD			dwRegionLineID;		//区域或路段ID
} ALARM_INOUT_REGION_LINE, *PALARM_INOUT_REGION_LINE;

//路段行驶时间不足/过长报警附加信息
typedef struct _ALARM_DRIVE_TIME
{
	DWORD			dwLineID;		//路段ID
	WORD			wDriveTime;		//路段行驶时间，单位为秒(s)
	BYTE			cResult;		//结果  0:不足  1:过长
	BYTE			cDummyData;		//填充数据
} ALARM_DRIVE_TIME, *PALARM_DRIVE_TIME;

//位置附加信息项
typedef struct _GPS_EXTRA_INFO
{
	DWORD					dwMileage;			//里程
	DWORD					dwOilMass;			//油量
	WORD					wSpeed;				//行驶记录功能获取的速度
	WORD					wDummyData;			//填充数据
	ALARM_OVER_SPEED		over_speed;			//超速报警附加信息
	ALARM_INOUT_REGION_LINE	inout_region_line;	//进出区域/线路报警附加信息
	ALARM_DRIVE_TIME		drive_time;			//路段行驶时间不足/过长报警附加信息
} GPS_EXTRA_INFO, *PGPS_EXTRA_INFO;

//位置汇报信息
typedef struct _GPS_INFO
{
	GPS_BASIC_INFO			gps_basic_info;		//位置基本信息
	GPS_EXTRA_INFO			gps_extra_info;		//位置附加信息
}GPS_INFO, *PGPS_INFO;
