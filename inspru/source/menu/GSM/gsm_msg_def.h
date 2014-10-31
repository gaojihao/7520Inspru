/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $Ex\inc\ex_config.h

 $Reversion History:

        (1) Created Liuhl@2008-01-14 11:40

 $Notes:
		
--*/

#ifndef  GSM_MSG_DEF_H
#define  GSM_MSG_DEF_H

#include <windows.h>

#define  UMSG_GSM_CMD                  ( WM_USER + 0x4001 ) 
#define  MSG_REPORT_LAC					( WM_USER+0x1557 )

//打电话的消息ID
#define	 UMSG_DIAL				       (WM_USER + 1110) 

enum  eGsmCmd
{
	GSM_CMD_OPEN  =  0,     //初始化GSM模块
	GSM_CMD_CLOSE,          //关闭GSM模块，关闭串口
	GSM_CMD_INIT,           // GSM 模块 相关注册信息
	GSM_CMD_SUSPEND,        //关闭串口，挂起线程，让GPRS开始工作
	GSM_CMD_RESUME,         //打开串口，回复线程，让GSM工作
	GSM_CMD_REFIND_WND,     //重新查找窗口，用来向主线程发送消息
	

	GSM_CMD_SENDMSG,        //发送消息命令
	GSM_CMD_DIAL,           //拨打电话命令
	GSM_CMD_DIAL_EXT,       //拨打分机命令
	GSM_CMD_HANGUP,         //用户挂机命令
	GSM_CMD_ACCEPTED,       //接听电话

	GSM_CMD_CHECK_LINE,     //检查连接后的
	GSM_CMD_CHECK_SIGNAL,   //检测SIM卡的信号和状态，目前只是做了信号
	GSM_CMD_SET_VOL,        //设置GSM的模块的声音

	GSM_ACK_MSG_RESULT,     //发送短消息后的ACK, 1:成功; 0:失败
	GSM_ACK_DIAL_RESULT,    //拨打电话后的状态ACK, 1:成功； 0:失败
	GSM_ACK_DIALING_LINE,    //电话连接好后的系统状态

	GSM_ACK_LINE_DISCONNECT, //电话拨号拨通后的状态，取消连接
	GSM_ACK_LINE_NOCARRIER,  //电话拨号拨通后的状态，没有拨号音

	GSM_ACK_NEW_MSG,        //接收到新的短消息
	GSM_ACK_NEW_CALLING,    //有新的电话打进来了
	GSM_ACK_SIGNAL,         //报告当前的GSM信号强度
	GSM_ACK_SIM_STATE,      //SIM卡的状态, 1有SIM卡，0:没有SIM卡, 2 系统初始化失败,模块没有应答

	GSM_ACK_SMS_STATE,      //报告SMS的状态 HIWORD()=最大存储容量,LOWORD() = 已经有的短信个数
	GSM_ACK_PBOOK_STATE,    //报告PBOOK的状态 HIWORD()=最大存储容量,LOWORD() = 已经实用的个数

	GSM_SIM_SMS_READ,       //读取SIM卡上的所有短消息
	GSM_SIM_PBOOK_READ,     //读取SIM卡上的所有电话薄完成
	GSM_SIM_SMS_DEL,        //删除卡上的指定短消息，WPARAM = idx;
	GSM_SIM_PBOOK_DEL,      //删除卡的指定的电话薄记录,WPARAM = idx;
	GSM_NET_SET,            //HIWORD(LPARAM) = Auto,User, LOWORD(LPARAM) = 0,1
	GSM_QUERY_NET,          //查询GSM当前网络
	GSM_ACK_QUERY_NET,      //当前系统的网络信息 0,中国移动 1,中国联通
	GSM_CMD_ADD_SIM_PBOOK,  //向SIM卡上增加电话本
	GSM_SIM_PBOOK_READ_PROCESS,    //读SIM卡上电话薄的进度HIWORD():总共电话个数 LOWORD():正在读的条数
	GSM_SIM_SMS_READ_PROCESS,      //读SIM卡上的SMS进度，HIWORD:总数，LOWORD:当前的个数
	//LiuHL@20090910
	GPRS_SVR_NEW_MSG,              //服务器下发的数据，最大长度为256个字节，而且为GB2312编码   
	GSM_CMD_RESERVED,
	GSM_CMD_LISTEN
};

// for WM_DATACOPY类型
enum eDataCopy
{
	GSM_DCOPY_PHONE      = 1027,  //电话号码
	GSM_DCOPY_PHONE_EXT,          //电话号码分机
	GSM_DCOPY_MSG,                //将要发送的消息
	GSM_DCOPY_PBOOK,              //在SIM卡上增加电话薄
	GSM_DCOPY_ACK_MSG,            //新消息
	GSM_DCOPY_DIPATCH_MSG,        //调度信息
	GSM_DCOPY_ACK_PHONE,
	GSM_DCOPY_SIM_SMS,
	GSM_DCOPY_SIM_PHONEBOOK,
};


// state of the call, from 
enum call_state_e
{
	call_active   = '0',
	call_held     = '1',
	call_dialing  = '2',  // MO call
	call_alerting = '3',  // MO call
	call_incoming = '4',  // MT call
	call_waiting  = '5',  // MT call
	call_error,
};


enum egsm_state_network_sel
{
	gsm_net_null         = 0,
	gsm_net_china_mobile = 1,	//中国移动
	gsm_net_china_unicom = 2,   //中国联通
	gsm_net_china_CUGSM	 = 3,	//中国联通WCDMA
};


// 
// 
// 
// #define  TID_SRV_MONITOR_SYSTEM       (0x10)   //  1000
// #define  TID_SRV_REPORT_SIGNAL        (0x11)   //  1000
// #define  TID_SRV_MONITOR_STDBY        (0x12)   //  5000
// #define  TID_SRV_MONITOR_BATTERY      (0x13)   //  3000
// #define  TID_SRV_GSM_DETECH_SIM       (0x14)   //  1000
// #define  TID_SRV_CHECK_HEARTED        (0x15)   //  4000
// 
// 
// #define  TID_GUI_MAIN_UPDATE_INFO       (0x30)
// #define  TID_GUI_GPS_UPDATE_GPS_INFO    (0x31)   // 1000
// #define  TID_GUI_MAIN_AUTO_STARTUP_NAVI (0x32)   // 1000
// #define  TID_GUI_GSM_CALLING            (0x33)   // 1000
// #define  TID_GUI_GSM_CHECK_LINE         (0x34)   // 1000
// #define  TID_GUI_GSM_ERROR_LINE         (0x34)   // 2000
// #define  TID_GUI_GSM_INCOMING           (0x35)   // 300
// #define  TID_GUI_SMS_SENDING            (0x36)   // 300
// #define  ITD_GUI_SMS_SENDING_ACK        (0x37)   // 500   短信发送返回后，退出的时间
// #define  TID_GUI_SMS_CHECK_EXTCALL      (0x38)   // 3000  拨打分机的最长等待时间 
// #define  TID_GUI_LANYA_CONNECT          (0x39)   // 1000  蓝牙连接等待时间  fengyc
// #define  TID_GUI_MEDIA_MONITERO_VOL     (0x40)   // 1500  用来监控系统声音是否改变 
// #define  TID_GUI_SHUTDOWN_COUNT         (0x41)   // 1000  关机倒数定时器
// #define  TID_GUI_HAND_COUNT             (0x42)   // 500  手写识别的时间
// 
// 
// #define  NKMSG_SDCARD_EVENT           (0x219)
// 	#define WSDCARD_EVENT_INSERT    	0x8000			//SD卡插入，参数WPARAM，低16位值
// 	#define WSDCARD_EVENT_PULLOUT   	0x8004			//SD卡拔出，参数WPARAM
// 
// 
// #define  _MSG_START                (WM_USER+0x100)
// /*************************************************************
// *                   (1) PND system message                   *
// *************************************************************/
// #define  PNDMSG_SYS_               (_MSG_START + 100 )
// /*
// 	set system back light value
// */
// 
// 
// 
// #define  PNDMSG_SYS_SET_BKLIGHT       (PNDMSG_SYS_ + 1 )
// 	#define  WBKLIGHT_LEVEL0       0
// 	#define  WBKLIGHT_LEVEL25      1
// 	#define  WBKLIGHT_LEVEL50      2
// 	#define  WBKLIGHT_LEVEL75      3   
// 	#define  WBKLIGHT_LEVEL100     4
// 
// /*
// 	set time for going to stand by.
// 	w_param : the time value, if the value is 0xFFFFFFFF, the system will 
// 	          never going to stand by.
// */
// #define  PNDMSG_SYS_SET_STDBY_TIME    (PNDMSG_SYS_ + 2 ) 
// 	#define  STDBY_TIME_FOREVER    (0xFFFFFFFF)
// 
// /*
// 	the time for stand by will be reset to zero.
// */
// #define  PNDMSG_SYS_RESET_STDBY      (PNDMSG_SYS_ + 3 )
// 
// /*
// 	system will going to stand by
// */
// #define  PNDMSG_SYS_STDBYING         (PNDMSG_SYS_ + 4 )
// 
// /*
// 	all server state( server to main )
// */
// #define  PNDMSG_SYS_SRV_REPORT        (PNDMSG_SYS_ + 5 )
// 	#define  WSRV_REPORT_BATTERY       1 // battery information
// 		#define  LBATTERY_AC_OFF_EXHAUSE        0  // AC off and 5-0
// 		#define  LBATTERY_AC_OFF_LOWPOWER       1  // AC off and 10-5
// 		#define  LBATTERY_AC_OFF_20             2  // AC off and 20-11
// 		#define  LBATTERY_AC_OFF_40             3  // AC off and 40-21
// 		#define  LBATTERY_AC_OFF_60             4  // AC off and 60-41
// 		#define  LBATTERY_AC_OFF_80             5  // AC off and 80-61
// 		#define  LBATTERY_AC_OFF_100            6  // AC off and 100-81
// 		#define  LBATTERY_AC_CHARGING           7  // AC on and charging
// 		#define  LBATTERY_AC_ON_FULL            8  // AC on and full powe
// 		#define  LBATTERY_AC_NUM                9
// 
// 	#define  WSRV_REPORT_GPS_SIGNAL    2 // GPS signal information
// 		#define LGPS_SIGNAL_ERROR               0               
// 		#define LGPS_SIGNAL_SURVEY              1
// 		#define LGPS_SIGNAL_2D                  2
// 		#define LGPS_SIGNAL_3D                  3
// 		#define LGPS_SIGNAL_3DMORE              4
// 		#define LGPS_SIGNAL_NUM                 5
// 
// 	#define  WSRV_REPORT_GSM_SIGNAL    3 // GSM signal
// 
//0 -113 dBm or less
//1 -111 dBm
//2...30 -109... -53 dBm
//31 -51 dBm or greater
//99 not known or not detectable
//Signal Quality Report
enum  gsm_signal_e
{
	sqr_no_sim = 0,
	sqr_very_low = 1,
	sqr_weak     = 2,
	sqr_normal   = 3,
	sqr_good     = 4,
	sqr_strength = 5,
	sqr_count    = 6
};
// 
// 
// /*
// 	open and close FM
// */
// #define  PNDMSG_SYS_FM_CTRL              (PNDMSG_SYS_ + 6 )            
// 	#define  WFM_CTRL_OPEN             1
// 	#define  WFM_CTRL_CLOSE            2
// 	#define  WFM_CTRL_SET_FREQ         3
// 
// 
// //Main给svr的控制命令
// #define  PNDMSG_MCTRL_CMD              (PNDMSG_SYS_ + 10 )
// 	#define  CMD_MCTRL_REFIND_WIN          0  //重新取得主窗口
// 	#define  CMD_MCTRL_SHUTDOWN            1  //关闭当前系统
// 	#define  CMD_MCTRL_UUID                2  //取得系统的UUID给main menu
//     
// 
// // GUI to SRV , re-find window hwnd.
// //#define   PNDMSG_REFIND_HWND            (PNDMSG_SYS_ + 7 ) 
// //关闭系统
// //#define  PNDMSG_SYS_SHUTDOWN            (PNDMSG_SYS_ + 8 )
// 
// 
// //手写的相关命令
// #define  PNDMSG_HW_CMD                 (PNDMSG_SYS_ + 9  ) 
// 	#define  CMD_HW_LOAD                    0  //加载HW库
// 	#define  CMD_HW_RELEASE                 1  //释放HW库
// 	#define  CMD_HW_RECOGNIZE               2  //开始识别
// 
// 
// 
// #define  PNDMSG_SYS_END
//    
// 
// 
// /*************************************************************
// *                       (2) MP3  message                     *
// *************************************************************/
// // PNDMain --> media sever
// #define  PNDMSG_MUSIC_CMD                (_MSG_START + 20 ) //main to sever
// 	#define  CMD_MUSIC_START_SVR         0
// 	#define  CMD_MUSIC_CLOSE_SVR         1
// 	#define  CMD_MUSIC_PAUSE             2
// 	#define  CMD_MUSIC_PLAY              3
// 	#define  CMD_MUSIC_STOP              4
// 	#define  CMD_MUSIC_CONTINUE          5
// 	#define  CMD_MUSIC_CHANGE_SEC        6
// 
// 
// /*************************************************************
// *                       (3) MP4  message                     *
// *************************************************************/
// #define  PNDMSG_VEDIO_CMD                (_MSG_START + 21 ) //main to server
// 	#define  CMD_VEDIO_START_SVR         0
// 	#define  CMD_VEDIO_CLOSE_SVR         1
// 	#define  CMD_VEDIO_PAUSE             2
// 	#define  CMD_VEDIO_PLAY              3
// 	#define  CMD_VEDIO_STOP              4
// 	#define  CMD_VEDIO_CONTINUE          5
// 	#define  CMD_VEDIO_CHANGE_SEC        6
// 
// #define  PNDMSG_MEDIA_SET_VIEW           (_MSG_START + 22 ) //main to server
//    // w_param : x1,y1
//    // l_param : x_width, y_height
// 	#define Media_Viev_PT(x,y)           ((((x)&0xFFFF)<<16) | ((y)&0xFFFF))                  
// 	#define Media_Viev_WH(w,h)           ((((w)&0xFFFF)<<16) | ((h)&0xFFFF))   
// 	#define Media_Viev_PT_x(pt)          (((pt)>>16)&0xFFFF)
// 	#define Media_Viev_PT_y(pt)          ((pt)&0xFFFF)
// 	#define Media_Viev_WH_w(v)           (((v)>>16)&0xFFFF)
// 	#define Media_Viev_WH_h(v)           ((v)&0xFFFF)
// 
// // media server --> PNDMain
// #define   PNDMSG_MEDIA_SVR_PREO          (_MSG_START + 23 )
//     // wparam : total second * tickout
//     // lparam : current second
// 
// 
// 
// /*************************************************************
// *                       (4) GPS  message                     *
// *************************************************************/
// #define  _PPNDMSG_GPS_             (_MSG_START + 50 )
// #define  PNDMSG_GPS_START          (_PPNDMSG_GPS_ + 1 ) // [com1,9600]   
// #define  PNDMSG_GPS_CLOSE          (_PPNDMSG_GPS_ + 2 )
// #define  PNDMSG_GPS_REPORT         (_PPNDMSG_GPS_ + 3 )
//          #define  WREPORT_GPS_NULL         1
//          #define  WREPORT_GPS_0183         2
//          #define  WREPORT_GPS_SIGNAL       3
//          #define  WREPORT_GPS_INFO         4
//          #define  WREPORT_GPS_NO_ADJUST    5       //不需要同步系统时间
// 
// #define  PPNDMSG_GPS__END           (_MSG_START + 60 )
// 
// 
// 
// /*************************************************************
// *                       (5) GSM  message                     *
// *************************************************************/
// #define  _PPNDMSG_GSM_             (_MSG_START + 150 )
// #define  PNDMSG_GSM_START          (_PPNDMSG_GSM_ + 1 ) // [com1,115200]   
// #define  PNDMSG_GSM_CLOSE          (_PPNDMSG_GSM_ + 2 )
// #define  PNDMSG_GSM_REPORT         (_PPNDMSG_GSM_ + 3 )
// #define  PNDMSG_GSM_CMD            (_PPNDMSG_GSM_ + 4 )
// 		#define WGSM_CMD_CALLING        1
// 		#define WGMS_CMD_HANGUP         2
//         #define WGMS_CMD_CHECKLINE      3
// 		#define WGMS_CMD_CHECK_NUMBER   4
// 		#define WGMS_CMD_SMS_SEND       5  // 发送短消息
// 		#define WGMS_CMD_GSM_ACCEPTED   6  //用来接通对方的来电
// 		#define WGMS_CMD_GSM_EXT        7  //用来接通对方的来电
// 		#define WGMS_CMD_SMS_REPORT_POS 8  //报告当前位置
// 		#define WGMS_CMD_GSM_SET_VOL    9  //设置GSM的声音
// 
// 
#define  PNDMSG_GSM_SRV_NOTIFY     (_PPNDMSG_GSM_ + 5) // GSM Srv端消息
		#define WGSM_SRV_LINE_CONNECT        1         // 电话已经连接
		#define WGSM_SRV_LINE_ERROR          2         // 电话连接出错
		#define WGSM_SRV_LINE_STATE          3         // 电话连接逅的状态
		#define WGSM_SRV_LINE_DISCONNECT     4         // 电话连被对方挂断
		#define WGSM_SRV_LINE_NOCARRIER      5         // 电话没有连接
		#define WGSM_SRV_LINE_INCOMING       6         // 电话没有连接
		#define WGSM_SRV_SMS_INCOMING        7         // 有一个新短消息来了
		#define WGSM_SRV_SMS_SENDING_ACK     8         // 发送短消息后的返回状态0:失败 1:成功
 

// #define  PNDMSG_GSM_SRV_INCOMING     (_PPNDMSG_GSM_ + 6) //有新电话来了	
// 

// 
// #define  BRDKEYMSG_VAL          (0xc020)
// 
// typedef enum BRD_KEYMSG_E{
// 	BACKLIGHT_MSG,
// 	VOLUME_MSG,
// 	PWROFF_MSG,
// 	MENU_MSG,
// 	SUSPEND_MSG,
// 	CAMIF_MSG,
// };
// 
// #define  PNDMSG_GSM__END           (_MSG_START + 199 )
// 
// 
// 
// 
// /*************************************************************
// *                   (6) PNDMain  message                     *
// *************************************************************/
// 
// // user select file from file list
// #define  PNDMSG_GUI_USER_SEL_FILE           (_MSG_START + 200 )
// //  user start calling selected number
// #define  PNDMSG_GUI_START_CALLING           (_MSG_START + 201 )    //在地址薄中拨打电话
// #define  PNDMSG_GUI_CALL_FROM_LIST          (_MSG_START + 202 )    //在通话记录的相信信息中拨打 
// #define  PNDMSG_GUI_SEND_SMS_FROM_ADSBOOK   (_MSG_START + 203 )    //在地址薄中发送短消息
//     // WPARM is the index of file list[0, max_num-1]
// 
// // media server --> PNDMain
// #define  PNDMSG_GUI_MEDIA_END          (_MSG_START + 211 )
// #define  PNDMSG_GUI_REFRESH_GPS        (_MSG_START + 212 )
// #define  PNDMSG_GUI_REFRESH_0183       (_MSG_START + 213 )
// #define  PNDMSG_SRV_GPS_ONE_FRAME      (_MSG_START + 300 )
// 
// 
// 
// 
// /*************************************************************
// *                   (6) WM_DATACOPY  type                    *
// *************************************************************/
// // update mp3 file path
// #define DCOPY_TYPE_MP3_PATH           (1)
// #define DCOPY_TYPE_GPS_0183           (4)
// #define DCOPY_TYPE_GPS_SIGNAL         (5)
// #define DCOPY_TYPE_GPS_INFO           (6)
// 
// #define DCOPY_TYPE_GSM_NEW_NUMBER     (10)  // UI2SRV 打电话
// #define DCOPY_TYPE_GSM_IMCOMING_NO    (11)  // SRV2UI 有新电话进来
// #define DCOPY_TYPE_GSM_SMS_INCOMING   (12)  // SRV2UI 新来的短消息
// #define DCOPY_TYPE_SMS_SEND_PREPARE   (13)  // UI2SRV 发送新的短消息
// #define DCOPY_TYPE_GSM_EXT_NUMBER     (14)  // UI2SRV 拨打分机号码
// #define DCOPY_TYPE_HW_NEW_PT_LIST     (15)  // UI2SRV 用户输入的笔迹
// #define DCOPY_TYPE_HW_CHAR_LIST       (16)  // SRV2UI 设别的结果，返回给UI
// #define DCOPY_TYPE_SMS_CENTER_NO      (17)  // UI2Svr 设置报告位置的中心号码 
// #define DCOPY_TYPE_HD_UUID            (18)  // 硬件UUID
// 
// 
// 
// //
// // system version information
// #define  SOFT_VER_MAIN       "Main Ver      : B1.0.0.20080819"
// #define  SOFT_VER_SRV        "Server Ver    : B1.0.0.10080507"
// #define  SOFT_REL_DATE       "Relase Date   : 2008-08-19"
// #define  SOFT_UUID           "Product id    : %s"


#endif  //MSG_DEF_H