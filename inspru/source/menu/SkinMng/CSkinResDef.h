#ifndef _CSKINRESDEF_H_
#define _CSKINRESDEF_H_


typedef struct  tagBMP
{
	TCHAR*	szBmpPath;
	POINT	nBmpSize;
} BMP_INFO;

enum  battery_e{
	battery_empy,
	battery_20,
	battery_40,
	battery_60,
	battery_80,
	battery_100,
	battery_none
};

enum  bmp_file_e{
 
	bmp_file_adj = 0,
	bmp_file_state ,
	bmp_file_LRBtn ,
	bmp_file_TimeAdjustBtn,
	bmp_file_TimeFmtBtn,
	bmp_file_TimeZoneBtn,
	bmp_file_MainMenuSetBtn,
	bmp_file_MainMenuBtn,
	bmp_file_BackBtn,
	bmp_file_SysSetBtn1,
	bmp_file_SysSetBtn2,
	bmp_file_LightSet_120x120,
	bmp_file_Style_103x93,
	bmp_file_LanguageArrow_54x62,
	bmp_file_SetInfo_Arrow_52x32,
	bmp_file_FourBtn_150x120,
	bmp_file_TIMESET_107x32,
	bmp_file_SetTimeFmt_90x90,
	bmp_file_UpDownBtn,
	bmp_file_TimeAdjArrow_53x38,
	bmp_file_PairSearch,
	bmp_file_back_31x28,
	bmp_file_phonebook_320x35,
	bmp_file_phonebookState_27x33,
	bmp_file_DialPane_47x23,
	bmp_file_Dial_47x46,
	bmp_file_DialNum_260x31,
	bmp_file_EditName_170x30,
	bmp_file_EditOk_37x37,
	bmp_file_btn3_80x40,
	bmp_file_btn_endd_85x65,
	bmp_file_TipInfo_213x119,
	bmp_file_TipOneBtn_54x124,
	bmp_file_TipTwoBtn_54x62,

	bmp_file_Volume_480x30,

	bmp_file_23,
	bmp_file_24,
	bmp_file_25,
	bmp_file_26,
	bmp_file_27,
	bmp_file_28,
	bmp_file_29,
	bmp_file_30,
	bmp_file_31,
	bmp_file_32,
	bmp_file_33,
	bmp_file_34,
	
	bmp_file_Calcu,
	bmp_file_BluetoothBtn,

	bmp_file_BtnAccept,
	bmp_file_BtnReject,

	bmp_file_batteryempty,
	bmp_file_bluetoothState,
	
	bmp_file_count
};

enum  bmp_bg_e{
	SKIN_BG_MENU_N = bmp_file_count,
	SKIN_BG_MENU_P,

	// Add Harry@20081030
	SKIN_MEDIA_MAIN_N,
	SKIN_MEDIA_MAIN_P,
	SKIN_BG_SMS_N,
	SKIN_BG_SMS_P,
	SKIN_BG_SMS_LIST_N,
	SKIN_BG_SMS_LIST_P,
	SKIN_BTN_DRAFT_ADD,
	BMP_BG_SENDING,
	BMP_BG_SENDING_LIST,

	//Add harry@20081110
	SKIN_BG_GSM_DIAL_MAIN_N,
	SKIN_BG_GSM_DIAL_MAIN_P,
	SKIN_BG_GSM_DIALING,
	SKIN_DIALING_LIST,
	
	//Add harry@20081112
	SKIN_BG_VOL_LIST,
	SKIN_SMS_FLAG,
	BMP_BG_SMS_DETAIL_N,
	BMP_BG_SMS_DETAIL_P,
	BMP_BG_SMS_DETAIL_D,
	SKIN_GSM_SIGNAL,
	BMP_CALLING_LIST,
	SKIN_BTN_SAVE,
	//调度消息
	BMP_BG_DISPATCH_N,
	BMP_BG_DISPATCH_P,

	SKIN_BG_SET_N,
	SKIN_BG_SET_P,
	SKIN_BG_DIAOUT_N,
	SKIN_BG_DIAOUT_P,
	SKIN_BG_DIAOUT_D,
	
	BMP_BG_DIAL_IN_N,
	BMP_BG_DIAL_IN_P,
	BG_BACKGROUND,
	SKIN_BG_SYSINFO_N,
	SKIN_BG_SYSINFO_P,
	BG_BRIGHTNESS_N,
	BG_BRIGHTNESS_P,
	BG_BRIGHTNESS_D,
	BMP_BG_NAVISET_N,
	BMP_BG_NAVISET_P,
	SKIN_BG_NAVIBRO_N,
	SKIN_BG_NAVIBRO_P,
	SKIN_BG_NAVIBRO_D,
	SKIN_BTN_BROWER,
	SKIN_BG_PHONELOG_N,
	SKIN_BG_PHONELOG_P,
	SKIN_BG_PHONELOG_D,
	BMP_BG_VOLUME_N,
	BG_PHONE_NUM_INPUT_N,
	BG_PHONE_NUM_INPUT_P,
	BMP_BG_PHONEBOOK_N,
	BMP_BG_PHONEBOOK_P,
	BMP_BG_PHONEBOOK_D,
	SKIN_BG_PHONEBOOKEDIT_N,
	SKIN_BG_PHONEBOOKEDIT_P,
	SKIN_BG_CONFIRM_N,
	SKIN_BG_CONFIRM_P,
	SKIN_BG_CONFIRM2_N,
	SKIN_BG_CONFIRM2_P,
	BMP_BG_NUMBER_N,
	BMP_BG_NUMBER_P,
	BMP_BG_DIAL_NUMBER_N,
	BMP_BG_DIAL_NUMBER_P,

	bmp_battery,
	BMP_BG_GAME_LIST_N,
	BMP_BG_GAME_LIST_P,
	BMP_BG_GAME_MANUAL,
	BMP_BG_PIN_N,
	BMP_BG_PIN_P,
	BMP_BG_PWD_N,
	BMP_BG_PWD_P,

	BMP_VEHICLE_INFO_N,
	BMP_VEHICLE_INFO_P,

	BMP_RFID_MAIN_N,
	BMP_RFID_MAIN_P,
	BMP_RFID_SCAN_N,
	BMP_RFID_SCAN_P,
	BMP_RFID_CHECK_N,
	BMP_RFID_CHECK_P,

	BMP_FAILURE_DIAGNOSTIC_N,
	BMP_FAILURE_DIAGNOSTIC_P,
	BMP_SECURITY_MONITOR_N,
	BMP_SECURITY_MONITOR_P,

	BMP_DRIVE_ECONOMY_N,
	BMP_DRIVE_ECONOMY_P,
	BMP_DRIVE_ECONOMY_SETTING_N,
	BMP_DRIVE_ECONOMY_SETTING_P,

	BMP_DRIVE_RECORD_N,
	BMP_DRIVE_RECORD_P,
	BMP_FILE_DOWNLOAD_N,
	BMP_FILE_DOWNLOAD_P,

	BMP_PUSH_TO_TALK_N,
	BMP_PUSH_TO_TALK_P,

	BMP_DRIVE_INFO_N,
	BMP_DRIVE_INFO_P,

	BMP_EXT_APP_N,
	BMP_EXT_APP_P,

	BMP_DIAODU_INFO_N,
	BMP_DIAODU_INFO_P,

	BG_PHONE_SMS_N,
	BG_PHONE_SMS_P,

	BMP_IC_LOGIN_N,
	BMP_IC_LOGIN_P,

	BMP_COM_SETTING_N,
	BMP_COM_SETTING_P,

	BMP_LOGIN,
	BMP_OIL_CONSUM,

	BG_SMS_CENTER_N,
	BG_SMS_CENTER_P,

	BG_SMS_TASK_N,
	BG_SMS_TASK_P,

	BG_FM_N,
	BG_FM_P,

	BG_INPUT1_N,
	BG_INPUT1_P,
	BG_INPUT3_N,
	BG_INPUT3_P,
	BG_INPUT4_N,
	BG_INPUT4_P,

	BG_LIST_N,
	BG_LIST_P,
	
	BG_BARCODESCAN_N,
	BG_BARCODESCAN_P,

	BG_TASK_POINT_LIST_N,
	BG_TASK_POINT_LIST_P,

	BG_POINT_LIST_N,
	BG_POINT_LIST_P,

	BG_POINT_INFO_N,
	BG_POINT_INFO_P,

	BG_HELP_LIST_N,
	BG_HELP_LIST_P,
	
	bmp_count
};

extern	 BMP_INFO      g_stBmpInfo[];

#define    BMP_ID_STEP   (100)

enum  bmp_id_e{

	bmp_id_adj =  bmp_file_adj * BMP_ID_STEP,
		bmp_file_FM_sub,
		bmp_file_FM_add,

	bmp_id_state =  bmp_file_state * BMP_ID_STEP,
		bmp_file_FM_state_open,
		bmp_file_FM_state_close,

	bmp_id_LRBtn =  bmp_file_LRBtn * BMP_ID_STEP,
		bmp_file_LBtn,
		bmp_file_RBtn,

	bmp_id_TimeAdjustBtn =  bmp_file_TimeAdjustBtn * BMP_ID_STEP,
		bmp_file_TimeAdjustBtn1,
	

	bmp_id_TimeFmtBtn =  bmp_file_TimeFmtBtn * BMP_ID_STEP,
		bmp_file_TimeFmtBtn1,

	bmp_id_TimeZoneBtn =  bmp_file_TimeZoneBtn * BMP_ID_STEP,
		bmp_file_TimeZoneBtn1,


	bmp_id_MainMenuSetBtn =  bmp_file_MainMenuSetBtn * BMP_ID_STEP,
		bmp_file_Set,
		bmp_file_Adjust,
		bmp_file_BlueTooth,

	bmp_id_MainMenuBtn =  bmp_file_MainMenuBtn * BMP_ID_STEP,
		bmp_file_Navi,
		bmp_file_ImageView,
		bmp_file_Music,
		bmp_file_Movie,
		bmp_file_main_SysSet,
		bmp_file_main_Game,
		bmp_file_main_GpsInfo,
		bmp_file_main_eBook,
		bmp_file_main_Bluetooth,
	

	bmp_id_BackBtn = bmp_file_BackBtn * BMP_ID_STEP,
		bmp_file_BackBtn0,


	bmp_id_SysSetBtn1 =  bmp_file_SysSetBtn1 * BMP_ID_STEP,
		bmp_file_SysSet_TimeSetBtn,
		bmp_file_SysSet_Time_12_24_SetBtn,
		bmp_file_SysSet_TimeZone,
		bmp_file_SysSet_Clock,
		bmp_file_SysSet_Calibrate,
		bmp_file_SysSet_Language,
		bmp_file_SysSet_Skin,
		bmp_file_SysSet_Default,

		bmp_file_Game_Daisy,
		bmp_file_Game_Chuzzle,
		bmp_file_Game_Bzzz,
		bmp_file_Game_Zuma,
		bmp_file_Game_1,
		bmp_file_Game_2,
		bmp_file_Game_3,
		bmp_file_Game_4,

		bmp_file_SysSet_info,

		bmp_file_SysSet_bluetooth,
		bmp_file_SysSet_back,
		bmp_file_SysSet_foword,
		bmp_file_SysSet_close_screen,
		
	bmp_id_SysSetBtn2 =  bmp_file_SysSetBtn2 * BMP_ID_STEP,
		bmp_file_SetChild_12,
		bmp_file_SetChild_24,
		bmp_file_SetChild_CN,
		bmp_file_SetChild_EN,
		bmp_file_SetChild_Style1,
		bmp_file_SetChild_Style2,
		bmp_file_SetChild_GpsInfo,
		bmp_file_SetChild_Calibrater,

	bmp_id_LightSet_120x120 = bmp_file_LightSet_120x120 * BMP_ID_STEP,
		bmp_file_LightSet_Low,
		bmp_file_LightSet_Mid,
		bmp_file_LightSet_High,

	bmp_id_Style_103_93 = bmp_file_Style_103x93 * BMP_ID_STEP,
		bmp_file_Style1_103_93,
		bmp_file_Style2_103_93,

	bmp_id_LanguageArrow_54_62 = bmp_file_LanguageArrow_54x62 * BMP_ID_STEP,
		bmp_file_LanguageArrow_B,
		bmp_file_LanguageArrow_F,

	bmp_id_SetInfo_Arrow_52_32 = bmp_file_SetInfo_Arrow_52x32 * BMP_ID_STEP,
		bmp_file_SetInfo_Arrow_B,
		bmp_file_SetInfo_Arrow_F,

	bmp_id_FourBtn_150x120 = bmp_file_FourBtn_150x120 * BMP_ID_STEP,
		bmp_file_FourBtn_BlueTooth_Set,
		bmp_file_FourBtn_BlueToothPhoneBook,
		bmp_file_FourBtn_BlueTooth_Dial,
		bmp_file_FourBtn_BlueTooth_RecentInOut,
		bmp_file_FourBtn_Ebook_Pdf,
		bmp_file_FourBtn_Ebook_Word,
		bmp_file_FourBtn_Ebook_txt,
		bmp_file_FourBtn_Ebook_PPt,
		bmp_file_FourBtn_NAVI_NAVI1,
		bmp_file_FourBtn_NAVI_NAVI2,
		bmp_file_FourBtn_NAVI_NAVI3,
		bmp_file_FourBtn_NAVI_NAVI4,

	bmp_id_TIMESET_107_32 = bmp_file_TIMESET_107x32 * BMP_ID_STEP,
		bmp_file_TIMESET_107_32_Btn,
	
	bmp_id_SetTimeFmt_90_90 = bmp_file_SetTimeFmt_90x90 * BMP_ID_STEP,
		bmp_file_SetTimeFmt_12fmt,
		bmp_file_SetTimeFmt_24fmt,

	bmp_id_UpDownBtn = bmp_file_UpDownBtn * BMP_ID_STEP,
		bmp_file_UpBtn,
		bmp_file_DownBtn,

	bmp_id_TimeAdjArrow_53_38 = bmp_file_TimeAdjArrow_53x38 * BMP_ID_STEP,
		bmp_file_TimeAdjArrow_62_54_up,
		bmp_file_TimeAdjArrow_62_54_down,

	bmp_id_PairSearch = bmp_file_PairSearch * BMP_ID_STEP,
		bmp_file_BtnPairSearch,
		
	bmp_id_back_31x28 = bmp_file_back_31x28 * BMP_ID_STEP,
		bmp_file_back,

	bmp_id_phonebook_320x35 = bmp_file_phonebook_320x35 * BMP_ID_STEP,
		bmp_file_BtnDiaOut,
		bmp_file_BtnDiaIn,
		bmp_file_BtnDiaNotDia,
		bmp_file_BtnSearch,
		bmp_file_BtnPhoneBook,

		
	bmp_id_phonebookState_27x33 = bmp_file_phonebookState_27x33 * BMP_ID_STEP,
		bmp_file_phonebookState_Log,
		bmp_file_phonebookState_Dia,
		bmp_file_phonebookState_AddItem,
		bmp_file_phonebook_PageUp,
		bmp_file_phonebook_PageDown,
		bmp_file_phonebookState_10,
		bmp_file_phonebookState_DiaShut,
		bmp_file_phonebookState_Edit,
		bmp_file_phonebookState_Dle,

	bmp_id_DialPane_47x23 = bmp_file_DialPane_47x23 * BMP_ID_STEP,
		bmp_file_DialPane_1,
		bmp_file_DialPane_2,
		bmp_file_DialPane_3,
		bmp_file_DialPane_4,
		bmp_file_DialPane_5,
		bmp_file_DialPane_6,
		bmp_file_DialPane_7,
		bmp_file_DialPane_8,
		bmp_file_DialPane_9,
		bmp_file_DialPane_0,
		bmp_file_DialPane_star,
		bmp_file_DialPane_shar,

	bmp_id_Dial_47x46 = bmp_file_Dial_47x46 * BMP_ID_STEP,
		bmp_file_DialOut,
		bmp_file_DialBack,
		bmp_file_DialIn,

	bmp_id_DialNum_260x31 = bmp_file_DialNum_260x31 * BMP_ID_STEP,
		bmp_file_DialNum,

	bmp_id_EditName_170x30 = bmp_file_EditName_170x30 * BMP_ID_STEP,
		bmp_file_EditName,
		bmp_file_EditNum,

	bmp_id_EditOk_37x37 = bmp_file_EditOk_37x37 * BMP_ID_STEP,
		bmp_file_EditCancel,
		bmp_file_EditOk,

	bmp_id_btn3_80x40 = bmp_file_btn3_80x40 * BMP_ID_STEP,
		bmp_file_btn3_0,
		bmp_file_btn3_1,
		bmp_file_btn3_2,

	bmp_id_btn_endd_85x65 = bmp_file_btn_endd_85x65 * BMP_ID_STEP,
		bmp_file_btn_end,
		bmp_file_btn_OutF,
		bmp_file_btn_OutA,

	bmp_id_TipInfo_213x119 = bmp_file_TipInfo_213x119 * BMP_ID_STEP,
		bmp_file_TipInfoStc_213x119,

	bmp_id_TipOneBtn_54x124 = bmp_file_TipOneBtn_54x124 * BMP_ID_STEP,
		bmp_file_TipOneBtn_Ok_54x124,

	bmp_id_bmp_file_TipTwoBtn_54x62 = bmp_file_TipTwoBtn_54x62 * BMP_ID_STEP,
		bmp_file_TipTwoBtn_Ok_54x62,
		bmp_file_TipTwoBtn_Cancael_54x62,

 	bmp_id_Volume_480x30 = bmp_file_Volume_480x30 * BMP_ID_STEP,
 		bmp_file_Volume_480x40_0,
 		bmp_file_Volume_480x40_1,
 		bmp_file_Volume_480x40_2,
		bmp_file_Volume_480x40_3,
 		bmp_file_Volume_480x40_4,
 		bmp_file_Volume_480x40_5,
 		bmp_file_Volume_480x40_6,
		bmp_file_Volume_480x40_7,
 		bmp_file_Volume_480x40_8,
 		bmp_file_Volume_480x40_9,
 		bmp_file_Volume_480x40_10,
		bmp_file_Volume_480x40_11,
 		bmp_file_Volume_480x40_12,
 		bmp_file_Volume_480x40_13,
 		bmp_file_Volume_480x40_14,
		bmp_file_Volume_480x40_15,
 		bmp_file_Volume_480x40_16,
		bmp_file_Volume_480x40_17,
 		bmp_file_Volume_480x40_18,
 		bmp_file_Volume_480x40_19,
 		bmp_file_Volume_480x40_20,
	
	bmp_slice_of_23 = bmp_file_23 * BMP_ID_STEP,
		bmp_slice_0_of_23,
	bmp_slice_of_24 = bmp_file_24 * BMP_ID_STEP,
		bmp_slice_0_of_24,
	bmp_slice_of_25 = bmp_file_25 * BMP_ID_STEP,
		bmp_slice_0_of_25,
	bmp_slice_of_26 = bmp_file_26 * BMP_ID_STEP,
		bmp_slice_0_of_26,
	bmp_slice_of_27 = bmp_file_27 * BMP_ID_STEP,
		bmp_slice_0_of_27,
	bmp_slice_of_28 = bmp_file_28 * BMP_ID_STEP,
		bmp_slice_0_of_28,
	bmp_slice_of_29 = bmp_file_29 * BMP_ID_STEP,
		bmp_slice_0_of_29,
	bmp_slice_of_30 = bmp_file_30 * BMP_ID_STEP,
		bmp_slice_0_of_30,
	bmp_slice_of_31 = bmp_file_31 * BMP_ID_STEP,
		bmp_slice_0_of_31,
	bmp_slice_of_32 = bmp_file_32 * BMP_ID_STEP,
		bmp_slice_0_of_32,
	bmp_slice_of_33 = bmp_file_33 * BMP_ID_STEP,
		bmp_slice_0_of_33,
	bmp_slice_of_34 = bmp_file_34 * BMP_ID_STEP,
		bmp_slice_0_of_34,
		
	bmp_id_Calcu = bmp_file_Calcu * BMP_ID_STEP,
		bmp_calcu_btn_0,		
		bmp_calcu_btn_1,
		bmp_calcu_btn_2,
		bmp_calcu_btn_3,
		bmp_calcu_btn_4,
		bmp_calcu_btn_5,
		bmp_calcu_btn_6,	
		bmp_calcu_btn_7,
		bmp_calcu_btn_8,
		bmp_calcu_btn_9,
		bmp_calcu_btn_DIV,
		bmp_calcu_btn_MUL,
		bmp_calcu_btn_SUB,
		bmp_calcu_btn_ADD,		
		bmp_calcu_btn_DOT,
		bmp_calcu_btn_PLUSorNEGA,

	bmp_id_BluetoothBtn = bmp_file_BluetoothBtn * BMP_ID_STEP, 
		bmp_BlueToothBtn_0,
		bmp_BlueToothBtn_1,
		bmp_BlueToothBtn_2,
		bmp_BlueToothBtn_3,
		bmp_BlueToothBtn_4,
		bmp_BlueToothBtn_5,
		bmp_BlueToothBtn_6,
		bmp_BlueToothBtn_7,
		bmp_BlueToothBtn_8,
		bmp_BlueToothBtn_9,
		bmp_BlueToothBtn_Star,
		bmp_BlueToothBtn_Sharp,
		bmp_BlueToothBtn_DialOut,
		bmp_BlueToothBtn_Bk,
		bmp_BlueToothBtn_DialIn,
		bmp_BlueToothBtn_Backspace,
		bmp_BlueToothBtn_CE,
		bmp_BlueToothBtn_EQU,
		bmp_BlueToothBtn_share,
		
	bmp_id_btnAccept  = bmp_file_BtnAccept * BMP_ID_STEP,
		bmp_btn_accept,
	
	bmp_id_btnReject  = bmp_file_BtnReject * BMP_ID_STEP,
		bmp_btn_reject,
		
	bmp_id_batteryempty = bmp_file_batteryempty * BMP_ID_STEP,
		bmp_bmp_batteryempty,

	bmp_id_bluetoothState = bmp_file_bluetoothState * BMP_ID_STEP,
		bmp_BlueToothstate_Disconnected,
		bmp_BlueToothState_Connected,
		bmp_BlueToothState_Connecting,

	bmp_id_count,
	bmp_id_none = 0xEFFFFFFF
};



#endif