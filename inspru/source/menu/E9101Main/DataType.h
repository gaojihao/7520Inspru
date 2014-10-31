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

#define WM_HARDWARE_BUTTON		0x00000710		//硬按键消息ID
#define WPARAM_BTN_1			0x0000F1F1		//按键1
#define WPARAM_BTN_2			0x0000F3F3		//按键2
#define WPARAM_BTN_3			0x0000F4F4		//按键3
#define WPARAM_BTN_4			0x0000F5F5		//按键4
#define WPARAM_BTN_5			0x0000F2F2		//按键5
#define WPARAM_BTN_6			0x0000F6F6		//按键6


//接收socket buffer最大长度
#define MSG_BUFFER_LEN					1280
#define NUM_256B						256
#define NUM_512B						512
#define NUM_1K							1024
#define NUM_2K							(1024*2)
#define NUM_3K							(1024*3)
#define NUM_4K							(1024*4)

//计算数组的维数
#define dim(x)							(sizeof(x) / sizeof(x[0]))

//config.ini系统配置文件路径
#define	PATH_CONFIG_INI					_T("\\Flashdrv Storage\\MENU\\config.ini")
//TerminalParam.ini系统配置文件路径
#define	PATH_PARAM_INI					_T("\\Flashdrv Storage\\MENU\\TerminalParam.ini")

#define	MenuApp							_T("WND_MAIN_MENU")

//消息定义
//通话状态的消息   进程通信消息
#define	WM_COMMAND_TIANZE				(WM_USER+0x1000)
#define	WPARAM_SERVER_LOGIN				10					//LPARAM:1 登录成功  0 掉线
//接收到服务器发送的socket数据后的处理消息
#define WM_SERVER						(WM_USER+0x4000)

#define WM_ALARM                        0x0171

enum enMSG_ID
{
	//终端(TerMiNal)消息ID
	MSG_TMN_COMMON_ACK					= 0x0001,	//1.终端通用应答
	MSG_TMN_HEARTBEAT					= 0x0002,	//3.终端心跳
	MSG_TMN_REGISTER					= 0x0100,	//4.终端注册
	MSG_TMN_LOGOUT						= 0x0003,	//6.终端注销
	MSG_TMN_AUTHENTICATION				= 0x0102,	//7.终端鉴权
	MSG_TMN_PARAM_GET_ACK				= 0x0104,	//10.查询终端参数应答
	MSG_TMN_POS_REPORT					= 0x0200,	//12.位置信息汇报
	MSG_TMN_POS_GET_ACK					= 0x0201,	//14.位置信息查询应答
	MSG_TMN_EVENT_REPORT				= 0x0301,	//18.事件报告
	MSG_TMN_QUESTION_ACK				= 0x0302,	//20.提问应答
	MSG_TMN_INFO_DEMAND_CANCEL			= 0x0303,	//22.信息点播/取消
	MSG_TMN_CAR_CONTROL_ACK				= 0x0500,	//27.车辆控制应答
	MSG_TMN_DRIVE_RECORD_UPLOAD			= 0x0700,	//37.行驶记录数据上传
	MSG_TMN_E_AWB_UPLOAD				= 0x0701,	//39.电子运单上报(Auto Waybill)
	MSG_TMN_DRIVER_INFO_UPLOAD			= 0x0702,	//40.驾驶员身份信息采集上报
	MSG_TMN_MULTIMEDIA_CMD				= 0x0800,	//41.多媒体事件信息上传
	MSG_TMN_MULTIMEDIA_UPLOAD			= 0x0801,	//42.多媒体数据上传
	MSG_TMN_MULTIMEDIA_SEARCH_ACK		= 0x0802,	//46.存储多媒体数据检索应答
	MSG_TMN_DATA_UPLOAD					= 0x0900,	//51.数据上行透传
	MSG_TMN_COMPRESS_DATA_UPLOAD		= 0x0901,	//52.数据压缩上报
	MSG_TMN_RSA_PUBLIC_KEY				= 0x0A00,	//54.终端RSA公钥
	MSG_TMN_TEXT_SMS					= 0x0300,	//附1.文本信息上传

	//服务器平台(SerVeR)消息ID
	MSG_SVR_COMMON_ACK					= 0x8001,	//2.平台通用应答
	MSG_SVR_REGISTER_ACK				= 0x8100,	//5.终端注册应答
	MSG_SVR_PARAM_SET					= 0x8103,	//8.设置终端参数
	MSG_SVR_PARAM_GET					= 0x8104,	//9.查询终端参数
	MSG_SVR_TMN_CONTROL					= 0x8105,	//11.终端控制
	MSG_SVR_POS_GET						= 0x8201,	//13.位置信息查询
	MSG_SVR_TEMP_POS_TRACE				= 0x8202,	//15.临时位置跟踪控制
	MSG_SVR_TEXT_SMS					= 0x8300,	//16.文本信息下发
	MSG_SVR_EVENT_SET					= 0x8301,	//17.事件设置
	MSG_SVR_QUESTION					= 0x8302,	//19.提问下发
	MSG_SVR_INFO_MENU_SET				= 0x8303,	//21.信息点播菜单设置
	MSG_SVR_INFO_SERVICE				= 0x8304,	//23.信息服务
	MSG_SVR_CALL_BACK					= 0x8400,	//24.电话回拨
	MSG_SVR_PHONE_BOOK_SET				= 0x8401,	//25.设置电话本
	MSG_SVR_CAR_CONTROL					= 0x8500,	//26.车辆控制
	MSG_SVR_CIRC_REGION_SET				= 0x8600,	//28.设置圆形区域
	MSG_SVR_CIRC_REGION_DEL				= 0x8601,	//29.删除圆形区域
	MSG_SVR_RECT_REGION_SET				= 0x8602,	//30.设置矩形区域
	MSG_SVR_RECT_REGION_DEL				= 0x8603,	//31.删除矩形区域
	MSG_SVR_NGON_REGION_SET				= 0x8604,	//32.设置多边形区域
	MSG_SVR_NGON_REGION_DEL				= 0x8605,	//33.删除多边形区域
	MSG_SVR_ROUTE_SET					= 0x8606,	//34.设置路线
	MSG_SVR_ROUTE_DEL					= 0x8607,	//35.删除路线
	MSG_SVR_DRIVE_RECORD_COLLECT		= 0x8700,	//36.行驶记录数据采集命令
	MSG_SVR_DRIVE_RECORD_PARAM_SET		= 0x8701,	//38.行驶记录参数下传命令
	MSG_SVR_MULTIMEDIA_UPLOAD_ACK		= 0x8800,	//43.多媒体数据上传应答
	MSG_SVR_CAMERA_PHOTOGRAPH			= 0x8801,	//44.摄像头立即拍摄命令
	MSG_SVR_MULTIMEDIA_SEARCH			= 0x8802,	//45.存储多媒体数据检索
	MSG_SVR_MULTIMEDIA_UPLOAD			= 0x8803,	//47.存储多媒体数据上传命令
	MSG_SVR_SOUND_RECORD				= 0x8804,	//48.录音开始命令
	MSG_SVR_SINGLE_MULT_SEARCH			= 0x8805,	//49.单条存储多媒体数据检索上传命令
	MSG_SVR_DATA_DOWNLOAD				= 0x8900,	//50.数据下行透传
	MSG_SVR_RSA_PUBLIC_KEY				= 0x8A00,	//53.平台RSA公钥
	MSG_TMN_DESTINATION_NAVI			= 0x8F01,	//附2.平台下发"目的地导航"指令
};

//终端参数(用于服务器设置或获取)
enum TerminalParam
{
	PARAM_HEART_BEAT_SLICE				= 0x0001,	//终端心跳发送间隔，单位为秒(s)
	PARAM_TCP_ACK_OVERTIME				= 0x0002,	//TCP消息应答超时时间，单位为秒(s)
	PARAM_TCP_RETRANSMIT_COUNT			= 0x0003,	//TCP消息重传次数
	PARAM_UDP_ACK_OVERTIME				= 0x0004,	//UDP消息应答超时时间，单位为秒(s)
	PARAM_UDP_RETRANSMIT_COUNT			= 0x0005,	//UDP消息重传次数
	PARAM_SMS_ACK_OVERTIME				= 0x0006,	//SMS消息应答超时时间，单位为秒(s)
	PARAM_SMS_RETRANSMIT_COUNT			= 0x0007,	//SMS消息重传次数
	//0x0008~0x000F		保留
	PARAM_MAIN_SVR_APN					= 0x0010,	//主服务器APN
	PARAM_MAIN_SVR_NAME					= 0x0011,	//主服务器无线通信拨号用户名
	PARAM_MAIN_SVR_PWD					= 0x0012,	//主服务器无线通信拨号密码
	PARAM_MAIN_SVR_IP					= 0x0013,	//主服务器地址，IP或域名
	PARAM_BAK_SVR_APN					= 0x0014,	//备份服务器APN
	PARAM_BAK_SVR_NAME					= 0x0015,	//备份服务器无线通信拨号用户名
	PARAM_BAK_SVR_PWD					= 0x0016,	//备份服务器无线通信拨号密码
	PARAM_BAK_SVR_IP					= 0x0017,	//备份服务器地址，IP或域名
	PARAM_SVR_TCP_PORT					= 0x0018,	//服务器TCP端口
	PARAM_SVR_UDP_PORT					= 0x0019,	//服务器UDP端口
	//0x001A~0x001F		保留
	PARAM_POS_REPORT_TYPE				= 0x0020,	//位置汇报策略
	PARAM_POS_REPORT_CONDITION			= 0x0021,	//位置汇报方案
	PARAM_POS_REPORT_SLICE_LOGOUT		= 0x0022,	//驾驶员未登录汇报时间间隔，单位为秒(s), >0
	//0x0023~0x0026		保留
	PARAM_POS_REPORT_SLICE_SLEEP		= 0x0027,	//休眠时汇报时间间隔，单位为秒(s), >0
	PARAM_POS_REPORT_SLICE_ALARM		= 0x0028,	//紧急报警时汇报时间间隔，单位为秒(s), >0
	PARAM_POS_REPORT_SLICE_DEFAULT		= 0x0029,	//缺省时间汇报间隔，单位为秒(s), >0
	//0x002A~0x002B		保留
	PARAM_POS_REPORT_DIST_DEFAULT		= 0x002C,	//缺省距离汇报间隔，单位为米(m), >0
	PARAM_POS_REPORT_DIST_LOGOUT		= 0x002D,	//驾驶员未登录汇报距离间隔，单位为米(m), >0
	PARAM_POS_REPORT_DIST_SLEEP			= 0x002E,	//休眠时汇报距离间隔，单位为米(m), >0
	PARAM_POS_REPORT_DIST_ALARM			= 0x002F,	//紧急报警时汇报距离间隔，单位为米(m), >0
	PARAM_CORNER_RETRANSMIT_ANGLE		= 0x0030,	//拐点补传角度，<180
	PARAM_ELECTRONIC_FENCE_RADIUS		= 0x0031,	//电子围栏半径（非法位移阈值），单位为米  Radius of the electronic fence
	//0x0032~0x003F		保留
	PARAM_PHONE_NUM_MONITOR				= 0x0040,	//监控平台电话号码
	PARAM_PHONE_NUM_RESET				= 0x0041,	//复位电话号码
	PARAM_PHONE_NUM_RESTORE				= 0x0042,	//恢复出厂设置电话号码
	PARAM_PHONE_NUM_SVR_SMS				= 0x0043,	//监控平台SMS电话号码
	PARAM_PHONE_NUM_TMN_SMS				= 0x0044,	//接收终端SMS文本报警号码
	PARAM_ANSWER_PHONE_TYPE				= 0x0045,	//终端电话接听策略
	PARAM_SINGLE_CALL_TIME_LIMIT		= 0x0046,	//每次最长通话时间
	PARAM_MONTH_CALL_TIME_LIMIT			= 0x0047,	//当月最长通话时间
	PARAM_MONITOR_PHONE_NUM				= 0x0048,	//监听电话号码
	PARAM_SVR_PRIVILEGE_SMS_NUM			= 0x0049,	//监管平台特权短信号码
	//0x004A~0x004F		保留
	PARAM_ALARM_MASK_BIT				= 0x0050,	//报警屏蔽字
	PARAM_ALARM_SEND_SMS_SWITCH			= 0x0051,	//报警发送文本SMS开关
	PARAM_ALARM_CAPTURE_SWITCH			= 0x0052,	//报警拍摄开关
	PARAM_ALARM_CAPTURE_STORE_FLAG		= 0x0053,	//报警拍摄存储标志
	PARAM_KEY_FLAG						= 0x0054,	//关键标志
	PARAM_SPEED_LIMIT					= 0x0055,	//最高速度
	PARAM_OVERSPEED_DUREATION			= 0x0056,	//超速持续时间
	PARAM_CONTINUE_DRIVE_TIME_LIMIT		= 0x0057,	//连续驾驶时间门限
	PARAM_ONE_DAY_DRIVE_TIME_LIMIT		= 0x0058,	//当天累计驾驶时间门限
	PARAM_MINIMUM_REST_TIME				= 0x0059,	//最小休息时间
	PARAM_MAXIMUM_PARKING_TIME			= 0x005A,	//最长停车时间
	//0x005B~0x006F		保留
	PARAM_IMAGE_VIDEO_QUALITY			= 0x0070,	//图像/视频质量，1～10，1最好
	PARAM_BRIGHTNESS					= 0x0071,	//亮度，0～255
	PARAM_CONTRAST						= 0x0072,	//对比度，0～127
	PARAM_SATURATION					= 0x0073,	//饱和度，0～127
	PARAM_CHROMA						= 0x0074,	//色度，0～255
	//0x0075~0x007F		保留
	PARAM_VEHICLE_ODOMETER				= 0x0080,	//车辆里程表读数，1/10km
	PARAM_VEHICLE_PROVINCE_ID			= 0x0081,	//车辆所在的省域ID
	PARAM_VEHICLE_CITY_ID				= 0x0082,	//车辆所在的市域ID
	PARAM_VEHICLE_ID					= 0x0083,	//公安交通管理部门颁发的机动车号码
	PARAM_PLATE_COLOR					= 0x0084,	//车牌颜色
};

//终端参数(用于服务器设置或获取)
typedef struct _TERMINAL_PARAM
{
	DWORD	dwHEART_BEAT_SLICE;			//(0x0001)终端心跳发送间隔，单位为秒(s)
	DWORD	dwTCP_ACK_OVERTIME;			//(0x0002)TCP消息应答超时时间，单位为秒(s)
	DWORD	dwTCP_RETRANSMIT_COUNT;		//(0x0003)TCP消息重传次数
	DWORD	dwUDP_ACK_OVERTIME;			//(0x0004)UDP消息应答超时时间，单位为秒(s)
	DWORD	dwUDP_RETRANSMIT_COUNT;		//(0x0005)UDP消息重传次数
	DWORD	dwSMS_ACK_OVERTIME;			//(0x0006)SMS消息应答超时时间，单位为秒(s)
	DWORD	dwSMS_RETRANSMIT_COUNT;		//(0x0007)SMS消息重传次数
	//0x0008~0x000F		保留
	CHAR	szMAIN_SVR_APN[8];			//(0x0010)主服务器APN
	CHAR	szMAIN_SVR_NAME[16];		//(0x0011)主服务器无线通信拨号用户名
	CHAR	szMAIN_SVR_PWD[16];			//(0x0012)主服务器无线通信拨号密码
	CHAR	szMAIN_SVR_IP[16];			//(0x0013)主服务器地址，IP或域名
	CHAR	szBAK_SVR_APN[8];			//(0x0014)备份服务器APN
	CHAR	szBAK_SVR_NAME[16];			//(0x0015)备份服务器无线通信拨号用户名
	CHAR	szBAK_SVR_PWD[16];			//(0x0016)备份服务器无线通信拨号密码
	CHAR	szBAK_SVR_IP[16];			//(0x0017)备份服务器地址，IP或域名
	DWORD	dwSVR_TCP_PORT;				//(0x0018)服务器TCP端口
	DWORD	dwSVR_UDP_PORT;				//(0x0019)服务器UDP端口
	//0x001A~0x001F		保留
	DWORD	dwPOS_REPORT_TYPE;			//(0x0020)位置汇报策略 0:定时汇报 1:定距汇报 2:定时和定距汇报
	DWORD	dwPOS_REPORT_CONDITION;		//(0x0021)位置汇报方案 0:根据ACC状态 1:根据登录状态和ACC状态
	DWORD	dwPOS_REPORT_SLICE_LOGOUT;	//(0x0022)驾驶员未登录汇报时间间隔，单位为秒(s), >0
	//0x0023~0x0026		保留
	DWORD	dwPOS_REPORT_SLICE_SLEEP;	//(0x0027)休眠时汇报时间间隔，单位为秒(s), >0
	DWORD	dwPOS_REPORT_SLICE_ALARM;	//(0x0028)紧急报警时汇报时间间隔，单位为秒(s), >0
	DWORD	dwPOS_REPORT_SLICE_DEFAULT;	//(0x0029)缺省时间汇报间隔，单位为秒(s), >0
	//0x002A~0x002B		保留
	DWORD	dwPOS_REPORT_DIST_DEFAULT;	//(0x002C)缺省距离汇报间隔，单位为米(m), >0
	DWORD	dwPOS_REPORT_DIST_LOGOUT;	//(0x002D)驾驶员未登录汇报距离间隔，单位为米(m), >0
	DWORD	dwPOS_REPORT_DIST_SLEEP;	//(0x002E)休眠时汇报距离间隔，单位为米(m), >0
	DWORD	dwPOS_REPORT_DIST_ALARM;	//(0x002F)紧急报警时汇报距离间隔，单位为米(m), >0
	DWORD	dwCORNER_RETRANSMIT_ANGLE;	//(0x0030)拐点补传角度，<180
	DWORD	dwELECTRONIC_FENCE_RADIUS;	//(0x0031)电子围栏半径（非法位移阈值），单位为米  Radius of the electronic fence
	//0x0032~0x003F		保留
	CHAR	szPHONE_NUM_MONITOR[16];	//(0x0040)监控平台电话号码
	CHAR	szPHONE_NUM_RESET[16];		//(0x0041)复位电话号码
	CHAR	szPHONE_NUM_RESTORE[16];	//(0x0042)恢复出厂设置电话号码
	CHAR	szPHONE_NUM_SVR_SMS[16];	//(0x0043)监控平台SMS电话号码
	CHAR	szPHONE_NUM_TMN_SMS[16];	//(0x0044)接收终端SMS文本报警号码
	DWORD	dwANSWER_PHONE_TYPE;		//(0x0045)终端电话接听策略
	DWORD	dwSINGLE_CALL_TIME_LIMIT;	//(0x0046)每次最长通话时间
	DWORD	dwMONTH_CALL_TIME_LIMIT;	//(0x0047)当月最长通话时间
	CHAR	szMONITOR_PHONE_NUM[16];	//(0x0048)监听电话号码
	CHAR	szSVR_PRIVILEGE_SMS_NUM[16];//(0x0049)监管平台特权短信号码
	//0x004A~0x004F		保留
	DWORD	dwALARM_MASK_BIT;			//(0x0050)报警屏蔽字
	DWORD	dwALARM_SEND_SMS_SWITCH;	//(0x0051)报警发送文本SMS开关
	DWORD	dwALARM_CAPTURE_SWITCH;		//(0x0052)报警拍摄开关
	DWORD	dwALARM_CAPTURE_STORE_FLAG;	//(0x0053)报警拍摄存储标志
	DWORD	dwKEY_FLAG;					//(0x0054)关键标志
	DWORD	dwSPEED_LIMIT;				//(0x0055)最高速度
	DWORD	dwOVERSPEED_DUREATION;		//(0x0056)超速持续时间
	DWORD	dwCONTINUE_DRIVE_TIME_LIMIT;//(0x0057)连续驾驶时间门限
	DWORD	dwONE_DAY_DRIVE_TIME_LIMIT;	//(0x0058)当天累计驾驶时间门限
	DWORD	dwMINIMUM_REST_TIME;		//(0x0059)最小休息时间
	DWORD	dwMAXIMUM_PARKING_TIME;		//(0x005A)最长停车时间
	//0x005B~0x006F		保留
	DWORD	dwIMAGE_VIDEO_QUALITY;		//(0x0070)图像/视频质量，1～10，1最好
	DWORD	dwBRIGHTNESS;				//(0x0071)亮度，0～255
	DWORD	dwCONTRAST;					//(0x0072)对比度，0～127
	DWORD	dwSATURATION;				//(0x0073)饱和度，0～127
	DWORD	dwCHROMA;					//(0x0074)色度，0～255
	//0x0075~0x007F		保留
	DWORD	dwVEHICLE_ODOMETER;			//(0x0080)车辆里程表读数，1/10km
	WORD	wVEHICLE_PROVINCE_ID;		//(0x0081)车辆所在的省域ID
	WORD	wVEHICLE_CITY_ID;			//(0x0082)车辆所在的市域ID
	CHAR	szVEHICLE_ID[16];			//(0x0083)公安交通管理部门颁发的机动车号码
	DWORD	dwPLATE_COLOR;				//(0x0084)车牌颜色 1 蓝   2 黄   3 黑   4 白   9 其他
} TERMINAL_PARAM, *PTERMINAL_PARAM;

//消息体属性
typedef struct _MSG_PROPERTY
{
	WORD	wMsgBodyLength;							//消息数据长度
	BYTE	ucEncryptionType;						//数据加密方式	0:不加密	1:RSA加密
	BYTE	ucSubPack;								//分包 0:无消息包封装项	1:有消息包封装项
} MSG_PROPERTY, *PMSG_PROPERTY;

//消息包封装项
typedef struct _MSG_PACK_ITEM
{
	WORD	wMsgPackNum;							//消息总包数
	WORD	wMsgPackIndex;							//包序号
} MSG_PACK_ITEM, *PMSG_PACK_ITEM;

//数据头
typedef struct _MSG_HEAD
{
	WORD			wMsgID;							//消息ID		0~1
	MSG_PROPERTY	stProperty;						//消息体属性	2~3
	BYTE			ucPhoneNumber[12];				//终端手机号码	4~9
	WORD			wMsgSerialNum;					//消息流水号	10~11
	MSG_PACK_ITEM	stPackItem;						//消息包封装项	12~15	根据stProperty.ucSubPack决定是否存在
} MSG_HEAD, *PMSG_HEAD;

//返回结果
typedef enum _enResult
{
	RESULT_SUCCESS			= 0,	//成功
	RESULT_FAILURE			= 1,	//失败
	RESULT_MSG_ERROR		= 2,	//消息有误
	RESULT_UNSUPPORT		= 3,	//不支持
	RESULT_ALARM_CONFIRM	= 4,	//报警处理确认(仅平台通用应答)
} enResult;

//应答	终端/平台通用应答
typedef struct _COMMON_ACK
{
	WORD		wAckSerialNum;							//应答流水号
	WORD		wAckMsgID;								//应答ID
	enResult	eRslt;									//结果	0:成功/确认	1:失败	2:消息有误	3:不支持
} COMMON_ACK, *PCOMMON_ACK;

//终端注册应答0x8100
typedef struct _REGISTER_ACK
{
	WORD	wAckSerialNum;							//应答流水号
	WORD	wResult;								//结果
	BYTE	cAuthenCode[64];						//鉴权码
} REGISTER_ACK, *PREGISTER_ACK;

//终端控制0x8105的指令类型
enum enTMN_CONTROL
{
	CONTROL_WIRELESSUPGRADE	= 1,	//无线升级
	CONTROL_CONNECT2SERVER,			//连接指定服务器
	CONTROL_POWEROFF,				//终端关机
	CONTROL_RESET,					//终端复位
	CONTROL_RESTORE_DEFAULT,		//终端回复出厂设置
	CONTROL_CLOSE_GPRS,				//关闭数据通信
	CONTROL_WIRELESS,				//关闭所有无线通信
};

//临时位置跟踪控制0x8202
typedef struct _TEMP_POS_TRACE
{
	WORD	wEnalbe;								//临时位置跟踪控制是否有效，0:无效 1:有效
	WORD	wSlice;									//时间间隔
	DWORD	dwPosTraceValidity;						//位置跟踪有效期
} TEMP_POS_TRACE, *PTEMP_POS_TRACE;

//文本信息标志
typedef struct _TEXT_FLAG
{
	BYTE	cEmergence:1;			//0,	1:紧急
	BYTE	cDummyData1:1;			//1,	填充数据1
	BYTE	cDisplay:1;				//2,	1:终端显示器显示
	BYTE	cTTS:1;					//3,	1:终端TTS播报
	BYTE	cAdvertise:1;			//4,	1:广告屏显示
	BYTE	cDummyData2:3;			//5~7,	填充数据2
} TEXT_FLAG, *PTEXT_FLAG;

//文本信息下发0x8300
typedef union _unTEXT_FLAG
{
	BYTE				cFlag;						//标志
	TEXT_FLAG			text_flag;
} unTEXT_FLAG;

typedef struct _TEXT_SMS
{
	unTEXT_FLAG		unFlag;							//标志
	BYTE			cDummyData[3];					//填充数据
	BYTE			cText[NUM_1K];					//文本信息
} TEXT_SMS, *PTEXT_SMS;

//设置类型，用于事件设置(0x8301),信息点播菜单设置(0x8303),设置电话本(0x8401)
enum enSET_TYPE
{
	SET_DELETE_ALL	= 0,	//删除全部记录
	SET_UPDATE,				//先删除全部记录，再追加新记录
	SET_ADD,				//增加记录
	SET_MODIFY,				//修改指定记录
	SET_DELETE,				//删除指定记录，仅用于事件设置(0x8301)
};

//摄像头立即拍摄命令0x8801
typedef struct _CAMERA_PHOTO
{
	WORD	wCMD;						//拍摄命令，0：停止拍摄；0xFFFF：录像；其它表示拍照张数
	WORD	wTime;						//拍摄间隔/录像时间,单位(s)，0表示按最小时间间隔拍照或一直录像
	BYTE	cTunnelID;					//通道ID,>0
	BYTE	cSaveFlag;					//保存标志，1：保存；0：实时上传
	BYTE	cResolution;				//分辨率：1:320*240,2:640*480,3:800*600,4:1024*768,5:176*144,6:352*288,7:704*288,8:704*576
	BYTE	cPicQuality;				//图像/视频质量：1～10，1代表质量损失最小，10表示压缩比最大
	BYTE	cLuminance;					//亮度0～255
	BYTE	cContrast;					//对比度0～255
	BYTE	cSaturation;				//饱和度0～255
	BYTE	cChroma;					//色度0～255
} CAMERA_PHOTO, *PCAMERA_PHOTO;

//存储多媒体数据检索0x8802
//存储多媒体数据上传命令0x8803
typedef struct _MULTIMEDIA_DATA
{
	BYTE	cMultimediaType;			//多媒体类型，0：图像；1：音频；2：视频
	BYTE	cTunnelID;					//通道ID，0表示检索该媒体类型的所有通道
	BYTE	cEventCode;					//事件项编码：0：平台下发指令；1：定时动作；2：抢劫报警触发；3：碰撞侧翻报警触发；其他保留
	BYTE	cFlag;						//删除标志：0：保留；1：删除
	BYTE	cStartTime[6];				//起始时间：YY-MM-DD-hh-mm-ss
	BYTE	cEndTime[6];				//结束时间：YY-MM-DD-hh-mm-ss
} MULTIMEDIA_DATA, *PMULTIMEDIA_DATA;

//录音开始命令0x8804
typedef struct _SOUND_RECORDING
{
	BYTE	cRecordCMD:4;				//录音命令，0：停止录音；1：开始录音
	BYTE	cSaveFlag:4;				//保存标志：0：实时上传；1：保存
	BYTE	cSamplingFrequency;			//采样频率：0：8K；1：11K；2：23K；3：32K；其他保留
	WORD	wRecordTime;				//录音时间：单位：秒(s)，0表示一直录音
} SOUND_RECORDING, *PSOUND_RECORDING;

typedef union _unSOUND_RECORDING
{
	SOUND_RECORDING		sound_recording;
	DWORD				dwSoundRecording;
} unSOUND_RECORDING;

//平台RSA公钥0x8A00
typedef struct _RSA_DATA
{
	DWORD	dwRSA_E;					//RSA公钥{e,n}中的e
	BYTE	cRSA_N[128];				//RSA公钥{e,n}中的n
} RSA_DATA, *PRSA_DATA;

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

//通用应答结果
typedef struct _COMMON_ACK_RESULT
{
	WORD	wMsgID;			//参考enMSG_ID
	WORD	wResult;		//参考enResult
} COMMON_ACK_RESULT;

typedef union _unCOMMON_ACK_RESULT
{
	COMMON_ACK_RESULT	ack_result;						//标志
	DWORD				dwAck_result;
} unCOMMON_ACK_RESULT;

//信息点播/取消
typedef struct _COMMON_DEMAND
{
	WORD	wMsgType;		//类型
	WORD	wMsgTag;		//标志
} COMMON_DEMAND;

typedef union _unCOMMON_DEMAND
{
	COMMON_DEMAND		demand_result;		//标志
	DWORD				dwDemand_result;
} unCOMMON_DEMAND;