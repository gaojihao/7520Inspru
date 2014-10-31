 
#include "StdAfx.h"
#include "CSkinResDef.h"

//#define SKIN_73_92								_T("SKIN_73_92")
//#define SKIN_SysSet_90_90						_T("SKIN_SysSet_90_90")
//#define SKIN_SysSetBk_240_320					_T("SKIN_SysSetBk_240_320")
//#define SKIN_SysSet_36x30						_T("SKIN_SysSet_36x30")

/*
	bmp_file_73x92 = 0,
	bmp_file_SysSet_36_30,
	bmp_file_SysSet_90_90,
	bmp_file_count
};
enum  bmp_bg_e{
	bmp_bg_start = bmp_file_count,
	bmp_bg_mainmenu,
	bmp_bg_systemset,
	bmp_bg_lightset,
	bmp_bg_timefmt,
	bmp_bg_timeadjust,
	bmp_bg_timezone,
	bmp_bg_skin,
	bmp_bg_language,
*/
BMP_INFO	g_stBmpInfo[] = 
{
	{ _T("SKIN_FM_adj"),			{48,64}		}	,
	{ _T("SKIN_FM_STATE"),			{192,60}	}	,
	{ _T("SKIN_btnLR"),				{60,80}		}	,
	{ _T("SKIN_TimeAdjustBtn"), 	{106,29}	}	,
	{ _T("SKIN_TimeFmtBtn"), 		{108,29}	}	,
	{ _T("SKIN_TimeZoneBtn"), 		{106,29}	}	,
	{ _T("SKIN_MainMenuSet"), 		{60,50}		}	,
	{ _T("SKIN_MainMenuBtn"),      	{120,120}	}	,
	{ _T("SKIN_BTN_BACK"),  		{160,34}	}	,
	{ _T("SKIN_SysSetBtn1"),		{120,120}	}	,
	{ _T("SKIN_SysSetBtn2"),		{192,174}	}	,
	{ _T("SKIN_LightSet_120_120"),	{120,120}	}	,
	{ _T("SKIN_Style_103_93"),		{223,58}	}	,
	{ _T("SKIN_Language_Arrow_54_62"),{54,62}	}	,
	{ _T("SKIN_SetInfo_Arrow_52_32"),{54,62}	}	,
	{ _T("SKIN_Four_btn"),			{150,120}	}	,
	{ _T("SKIN_TIMESET_107_32"),	{107,32}	}	,
	{ _T("SKIN_SetTimeFmt_223_58"),	{223,58}	}	,
	{ _T("SKIN_UpDownArrow"),		{80,34}		}	,
	{ _T("SKIN_TimeAdjArrow_53_38"),{75,54}		}	,
	{ _T("SKIN_PAIR_SEARCH"),		{130,33}	}	,
	{ _T("SKIN_BACK"),				{80,32}		}	,
	{ _T("SKIN_PhoneBook"),			{320,35}	}	,
	{ _T("SKIN_PhoneBookState"),	{27,32}		}	,
	{ _T("SKIN_DiaBtn"),			{47,34}		}	,
	{ _T("SKIN_Dial"),				{47,46}		}	,
	{ _T("SKIN_DialNum"),			{260,31}	}	,
	{ _T("SKIN_btnEditName"),		{170,30}	}	,
	{ _T("SKIN_btnEditOk"),			{37,37}		}	,
	{ _T("SKIN_btn3"),				{80,40}		}	,
	{ _T("SKIN_btnEndd"),			{85,65}		}	,
	{ _T("SKIN_TIPINFO"),			{150,75}	}	,
	{ _T("SKIN_TIP_ONE_BTN"),		{54,124}	}	,
	{ _T("SKIN_TIP_TWO_BTN"),		{54,62}		}	,
	{ _T("SKIN_Volume"),			{480,40}	}	,
	{ _T("bmp_file_23"),			{320,160}	}	,
	{ _T("bmp_file_24"),			{320,160}	}	,
	{ _T("bmp_file_25"),			{320,160}	}	,
	{ _T("bmp_file_26"),			{320,160}	}	,
	{ _T("bmp_file_27"),			{320,160}	}	,
	{ _T("bmp_file_28"),			{320,160}	}	,
	{ _T("bmp_file_29"),			{320,160}	}	,
	{ _T("bmp_file_30"),			{320,160}	}	,
	{ _T("bmp_file_31"),			{320,160}	}	,
	{ _T("bmp_file_32"),			{36,44}		}	,
	{ _T("bmp_file_33"),			{320,160}	}	,
	{ _T("bmp_file_34"),			{320,160}	}	,
	{ _T("bmp_file_calculator"),	{78,42}		}	,
	{ _T("bmp_file_BlueToothBtn"),	{80,56}		}	,
	{ _T("bmp_file_BtnAccept"),		{265,60}	}	,
	{ _T("bmp_file_BtnReject"),		{265,60}	}	,
	{ _T("bmp_file_batteryempty3017"),{30, 17}	}	,
	{ _T("bmp_file_BlueToothState"),{24, 32}	}	,
//bmp_file_count
	{ _T("SKIN_BG_MENU_N"),			{800,480}	}	,	//主菜单
	{ _T("SKIN_BG_MENU_P"),			{800,480}	}	,
	{ _T("SKIN_MEDIA_MAIN_N"),		{800,480}	}	,	//多媒体
	{ _T("SKIN_MEDIA_MAIN_P"),		{800,480}	}	,
	{ _T("SKIN_BG_SMS_N"),			{800,480}	}	,	//短信
	{ _T("SKIN_BG_SMS_P"),			{800,480}	}	,
	{ _T("SKIN_BG_SMS_LIST_N"),		{800,480}	}	,
	{ _T("SKIN_BG_SMS_LIST_P"),		{800,480}	}	,
	{ _T("SKIN_BTN_DRAFT_ADD"),		{146,168}	}	,
	{ _T("BMP_BG_SENDING"),			{480,272}	}	,
	{ _T("BMP_BG_SENDING_LIST"),	{165,44 }	}	,
	{ _T("SKIN_BG_GSM_DIAL_MAIN_N"),{800,480}	}	,
	{ _T("SKIN_BG_GSM_DIAL_MAIN_P"),{800,480}	}	,
	{ _T("SKIN_BG_GSM_DIALING"),	{494,294}	}	,
	{ _T("SKIN_DIALING_LIST"),		{150,25}	}	,
	{ _T("SKIN_BG_VOL_LIST"),		{59,125 }	}	,
	{ _T("SKIN_SMS_FLAG"),			{40,40 }	}	,
	{ _T("BMP_BG_SMS_DETAIL_N"),	{480,272}	}	,
	{ _T("BMP_BG_SMS_DETAIL_P"),	{480,272}	}	,
	{ _T("BMP_BG_SMS_DETAIL_D"),	{480,272}	}	,
	{ _T("SKIN_GSM_SIGNAL"),		{64,64}		}	,
	{ _T("BMP_CALLING_LIST"),		{270,47}	}	,
	{ _T("SKIN_BTN_SAVE"),			{144,164}	}	,
	{ _T("BMP_BG_DISPATCH_N"),		{480,272}	}	,
	{ _T("BMP_BG_DISPATCH_P"),		{480,272}	}	,
	{ _T("SKIN_BG_SET_N"),			{800,480}	}	,
	{ _T("SKIN_BG_SET_P"),			{800,480}	}	,
	{ _T("SKIN_BG_DIAOUT_N"),		{800,480}	}	,
	{ _T("SKIN_BG_DIAOUT_P"),		{800,480}	}	,
	{ _T("SKIN_BG_DIAOUT_D"),		{800,480}	}	,
	{ _T("BMP_BG_DIAL_IN_N"),		{494,274}	}	,
	{ _T("BMP_BG_DIAL_IN_P"),		{494,274}	}	,
	{ _T("BG_BACKGROUND"),			{800,480}	}	,
	{ _T("SKIN_BG_SYSINFO_N"),		{800,480}	}	,
	{ _T("SKIN_BG_SYSINFO_P"),		{800,480}	}	,
	{ _T("BG_BRIGHTNESS_N"),		{800,480}	}	,
	{ _T("BG_BRIGHTNESS_P"),		{800,480}	}	,
	{ _T("BG_BRIGHTNESS_D"),		{800,480}	}	,
	{ _T("BMP_BG_NAVISET_N"),		{480,272}	}	,
	{ _T("BMP_BG_NAVISET_P"),		{480,272}	}	,
	{ _T("SKIN_BG_NAVIBRO_N"),		{480,272}	}	,
	{ _T("SKIN_BG_NAVIBRO_P"),		{480,272}	}	,
	{ _T("SKIN_BG_NAVIBRO_D"),		{480,272}	}	,
	{ _T("SKIN_BTN_BROWER"),		{210,195}	}	,
	{ _T("SKIN_BG_PHONELOG_N"),		{800,480}	}	,
	{ _T("SKIN_BG_PHONELOG_P"),		{800,480}	}	,
	{ _T("SKIN_BG_PHONELOG_D"),		{800,480}	}	,
	{ _T("BMP_BG_VOLUME_N"),		{480,272}	}	,
	{ _T("BG_PHONE_NUM_INPUT_N"),	{800,480}	}	,
	{ _T("BG_PHONE_NUM_INPUT_P"),	{800,480}	}	,
	{ _T("BMP_BG_PHONEBOOK_N"),		{480,272}	}	,
	{ _T("BMP_BG_PHONEBOOK_P"),		{480,272}	}	,
	{ _T("BMP_BG_PHONEBOOK_D"),		{480,272}	}	,
	{ _T("SKIN_BG_PHONEBOOKEDIT_N"),{480,272}	}	,
	{ _T("SKIN_BG_PHONEBOOKEDIT_P"),{480,272}	}	,
	{ _T("SKIN_BG_CONFIRM_N"),		{494,274}	}	,
	{ _T("SKIN_BG_CONFIRM_P"),		{494,274}	}	,
	{ _T("SKIN_BG_CONFIRM2_N"),		{494,274}	}	,
	{ _T("SKIN_BG_CONFIRM2_P"),		{494,274}	}	,
	{ _T("BMP_BG_NUMBER_N"),		{480,272}	}	,
	{ _T("BMP_BG_NUMBER_P"),		{480,272}	}	,
	{ _T("BMP_BG_DIAL_NUMBER_N"),	{480,272}	}	,
	{ _T("BMP_BG_DIAL_NUMBER_P"),	{480,272}	}	,
	{ _T("SKIN_BATTERY"),			{32,19}		}	,
	{ _T("BMP_BG_GAME_LISG_N"),		{480,272}	}	,
	{ _T("BMP_BG_GAME_LISG_p"), 	{480,272}	}	,
	{ _T("BMP_BG_GAME_MANUAL"),		{480,272}	}	,
	{ _T("BMP_BG_PIN_N"),			{480,272}	}	,
	{ _T("BMP_BG_PIN_P"),			{480,272}	}	,
	{ _T("BMP_BG_PWD_N"),			{800,480}	}	,
	{ _T("BMP_BG_PWD_P"),			{800,480}	}	,

	{ _T("BMP_VEHICLE_INFO_N"),			{800,480}	}	,
	{ _T("BMP_VEHICLE_INFO_P"),			{800,480}	}	,

	{ _T("BMP_RFID_MAIN_N"),			{800,480}	}	,
	{ _T("BMP_RFID_MAIN_P"),			{800,480}	}	,
	{ _T("BMP_RFID_SCAN_N"),			{800,480}	}	,
	{ _T("BMP_RFID_SCAN_P"),			{800,480}	}	,
	{ _T("BMP_RFID_CHECK_N"),			{800,480}	}	,
	{ _T("BMP_RFID_CHECK_P"),			{800,480}	}	,

	{ _T("BMP_FAILURE_DIAGNOSTIC_N"),	{800,480}	}	,
	{ _T("BMP_FAILURE_DIAGNOSTIC_P"),	{800,480}	}	,
	{ _T("BMP_SECURITY_MONITOR_N"),		{800,480}	}	,
	{ _T("BMP_SECURITY_MONITOR_P"),		{800,480}	}	,

	{ _T("BMP_DRIVE_ECONOMY_N"),		{800,480}	}	,
	{ _T("BMP_DRIVE_ECONOMY_P"),		{800,480}	}	,
	{ _T("BMP_DRIVE_ECONOMY_SETTING_N"),{800,480}	}	,
	{ _T("BMP_DRIVE_ECONOMY_SETTING_P"),{800,480}	}	,
	
	{ _T("BMP_DRIVE_RECORD_N"),			{800,480}	}	,
	{ _T("BMP_DRIVE_RECORD_P"),			{800,480}	}	,
	{ _T("BMP_FILE_DOWNLOAD_N"),		{800,480}	}	,
	{ _T("BMP_FILE_DOWNLOAD_P"),		{800,480}	}	,
	
	{ _T("BMP_PUSH_TO_TALK_N"),			{800,480}	}	,
	{ _T("BMP_PUSH_TO_TALK_P"),			{800,480}	}	,

	{ _T("BMP_DRIVE_INFO_N"),			{800,480}	}	,
	{ _T("BMP_DRIVE_INFO_P"),			{800,480}	}	,

	{ _T("BMP_EXT_APP_N"),				{800,480}	}	,
	{ _T("BMP_EXT_APP_P"),				{800,480}	}	,
	
	{ _T("BMP_DIAODU_INFO_N"),			{800,480}	}	,
	{ _T("BMP_DIAODU_INFO_P"),			{800,480}	}	,
	
	{ _T("BG_PHONE_SMS_N"),				{800,480}	}	,
	{ _T("BG_PHONE_SMS_P"),				{800,480}	}	,

	{ _T("BMP_IC_LOGIN_N"),				{800,480}	}	,
	{ _T("BMP_IC_LOGIN_P"),				{800,480}	}	,

	{ _T("BMP_COM_SETTING_N"),			{800,480}	}	,
	{ _T("BMP_COM_SETTING_P"),			{800,480}	}	,
	
	{ _T("BMP_LOGIN"),					{800,480}	}	,
	{ _T("BMP_OIL_CONSUM"),				{800,480}	}	,
	
	{ _T("BG_SMS_CENTER_N"),			{800,480}	}	,
	{ _T("BG_SMS_CENTER_P"),			{800,480}	}	,
	
	{ _T("BG_SMS_TASK_N"),				{800,480}	}	,
	{ _T("BG_SMS_TASK_P"),				{800,480}	}	,
	
	{ _T("BG_FM_N"),					{800,480}	}	,
	{ _T("BG_FM_P"),					{800,480}	}	,
	
	{ _T("BG_INPUT1_N"),				{800,480}	}	,
	{ _T("BG_INPUT1_P"),				{800,480}	}	,
	{ _T("BG_INPUT3_N"),				{800,480}	}	,
	{ _T("BG_INPUT3_P"),				{800,480}	}	,
	{ _T("BG_INPUT4_N"),				{800,480}	}	,
	{ _T("BG_INPUT4_P"),				{800,480}	}	,
	
	{ _T("BG_LIST_N"),					{800,480}	}	,
	{ _T("BG_LIST_P"),					{800,480}	}	,
	
	{ _T("BG_BARCODESCAN_N"),			{800,480}	}	,
	{ _T("BG_BARCODESCAN_P"),			{800,480}	}	,
	
	{ _T("BG_TASK_POINT_LIST_N"),		{800,480}	}	,
	{ _T("BG_TASK_POINT_LIST_P"),		{800,480}	}	,
	
	{ _T("BG_POINT_LIST_N"),			{800,480}	}	,
	{ _T("BG_POINT_LIST_P"),			{800,480}	}	,
	
	{ _T("BG_POINT_INFO_N"),			{800,480}	}	,
	{ _T("BG_POINT_INFO_P"),			{800,480}	}	,
	
	{ _T("BG_HELP_LIST_N"),				{800,480}	}	,
	{ _T("BG_HELP_LIST_P"),				{800,480}	}	,

};
