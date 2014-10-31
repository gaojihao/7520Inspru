
#ifndef _E9101N_HARD_H_
#define _E9101N_HARD_H_

//因为驱动接口是用C实现的，会在C++中使用
#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
//定义布尔类型
typedef int BOOL;
#define TRUE	1
#define FALSE	0
#endif

//耳机插拔消息
#define WM_SOUND_SWITCH 0xc022

// 错误码（所有函数除了返回要获取的参数外，如果出错还要返回错误码）
typedef int ERRCODE;
#define HARD_ERR_OK				0	//没有错误
#define HARD_ERR_FAILED			-1	//一般性失败
#define HARD_ERR_OSD_NOT_SHOW	-2	//OSD界面还没显示

//电源按键（有KEYUP和KEYDOWN消息）
#define LC_KEY_PWR_SHORT				51  //短按
#define LC_KEY_PWR_LONG                                 52  //长按

//	声音
//音频输出API与Windows标准接口一样，这里略
BOOL audio_initdev();
#define SoundInit	audio_initdev

#define SOUND_TRACK_LEFT		0	       //左
#define SOUND_TRACK_RIGHT		1	       //右
#define SOUND_TRACK_STEREO		2	       //立体
#define SOUND_TRACK_MUTEON		3    	   //静音
ERRCODE	SoundSetTrack(int nTrack);	       //设置声道
int		SoundGetTrack();			       //取得当前声道


//音频切换
#define AUDIO_TRACK_GPS         0           //设置音频通道为GPS输出
#define AUDIO_TRACK_FM          1           //设置音频通道为FM输出
#define AUDIO_TRACK_DVBT        2           //设置音频通道为DVBT输出
ERRCODE	SoundSetAudioSwitch(int nState);   
int  	SoundGetAudioSwitch();			   //取得当前音频通道
				
//最好用WINCE的系统静音函数
BOOL    ioc_audmute(DWORD nmute);			//开关静音
DWORD   ioc_audamplify(DWORD dwswi);	    //开关功放

DWORD   ioc_get_audamplify();					
DWORD   ioc_get_audmute();					//取得静音


//屏幕电源
#define LCD_BLIGHT_LOW              0
#define LCD_BLIGHT_MIDDLE           1
#define LCD_BLIGHT_HIGHT            2
ERRCODE LcdBackLightCtrl(int nLight);              //控制LCD背光
ERRCODE	LcdSetPower(BOOL bPower);	               //开关屏幕电源
BOOL	LcdGetPower();			      	           //取得当前屏幕电源开关


// 电源管理
BOOL	 PowerShutDown();			   //软关机
//DWORD ioc_systempwr(DWORD);
int	 PowerGetBatteryRemainPercent();           //取得电池当前剩余电量百分比0-100
BOOL	 PowerIsUseBattery();			   //电池是否在放电状态
typedef enum _POWERSTAT_E
{
   BATTERY_FAILED = 0,
   BATTERY_ONLINE,
   AC_ONLINE,
   AC_OFFLINE, 
   BATTERY_CHARGING,
   BATTERY_FULL,
   BATTERY_CHANGE_DISABLE
}POWERSTAT_E;
POWERSTAT_E     BatteryStatus();


//指示灯管理
//////////////////////////

#define  LED_LEFT_BON      0x01     // 左灯蓝光亮
#define  LED_LEFT_BOFF     0x02     // 左灯蓝光关
#define  LED_LEFT_RON      0x04     // 左灯红光亮
#define  LED_LEFT_ROFF     0x08     // 左灯红光关
#define  LED_RIGHT_BON     0x10     // 右灯蓝光亮
#define  LED_RIGHT_BOFF    0x20     // 右灯蓝光关
#define  LED_RIGHT_RON     0x40     // 右灯红光亮
#define  LED_RIGHT_ROFF    0x80     // 右灯红光关
ERRCODE  LEDControl(int nState);
 
//FM上下电
DWORD ioc_fmdata(DWORD dwfmdata);
/////////////////////////////////////////////

//GPS电源
DWORD ioc_gpspwr(DWORD dwgpspwr);
DWORD ioc_get_gpspwr();
DWORD ioc_get_fmdata();

//按键音
int SoundGetKeySound();
ERRCODE	SoundSetKeySound(BOOL bOpen);

//消息定义
//#define WM_LCD_CLOSE 			WM_USER + 18			//需要LCD关闭时给主进程主线程发送
//#define WM_CHILD_EXIT  			WM_USER + 11			//退出时给主进程主线程发送
//#define WM_POWER_OFF			WM_USER + 12			//需要电源关闭时给主进程主线程发送
// #define WM_LANGUAGE_CHANGE		WM_USER + 14		    //语言改变消息	
#define WM_HBCPSTATE			WM_USER + 0x1111        //蓝牙状态改变消息
#define WM_HBCPCALLIN			WM_USER + 0x1112        //蓝牙电话拨入消息
#define WM_HBCPCALLER_NUMBER    WM_USER + 0x1113        //蓝牙电话拨入电话号码
#define WM_HIDEMAINWND          WM_USER + 0x1114        //子进程发给父进程，用于主窗体隐藏
#define WM_HBCPRINGIND          WM_USER + 0x1115        //当电话拨入时的振铃消息
#define WM_HBCPOFFHOOK          WM_USER + 0x1116        //当接通电话时对方挂断消息
#define WM_HBCPOUTGOINGCALL     WM_USER + 0x1117        //当手机往外拨打电话时发送的消息
#define WM_HBCPCONNECTED        WM_USER + 0x1118        //当模块同手机建立连接时
#define WM_VOL                  WM_USER + 0x1119 
#define WM_HBCPACTIVECALL		WM_USER + 0x1120
#define WM_HBCPPAIRLISTCOMPLETE		WM_USER + 0x1121
#define WM_REFRESH_PAGE			WM_USER + 0x1122		//当有救援任务或定期定点新任务节点时刷新页面

//	SD卡
BOOL	SDCardIsInsert();		           //SD卡是否插好

//SD卡消息值定义
/* internal definition */
#define WM_STORAGE_EVENT        	0x219

/* expose to user */
#define STORAGE_EVENT_SD_PLUGIN    	0x8000			//SD卡插入，参数WPARAM，低16位值	DBT_DEVICEARRIVAL
#define STORAGE_EVENT_SD_PULLOUT   	0x8004			//SD卡拔出，参数WPARAM，低16位值	DBT_DEVICEREMOVECOMPLETE

#define WM_CREATE_SET	0xC021//0xC020	//机器按键消息

//typedef struct savtofile_s  {
//	int vol_field;
//	int bklite_field;
//	int pad[5];
//} savtofile_t;

#ifdef __cplusplus
}
#endif

#endif	//_E9101N_HARD_H_