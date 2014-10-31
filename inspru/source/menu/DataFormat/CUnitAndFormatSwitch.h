// CUnitAndFormatSwitch.h: interface for the CUnitAndFormatSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUNITANDFORMATSWITCH_H__5A22D0AD_74BF_4690_B957_865D5FA55B70__INCLUDED_)
#define AFX_CUNITANDFORMATSWITCH_H__5A22D0AD_74BF_4690_B957_865D5FA55B70__INCLUDED_

#include "CIniFile.h"
#include "DataFormatSetType.h"
#include  "LanguageSetType.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUnitAndFormatSwitch  
{
public:
	CUnitAndFormatSwitch();
	virtual ~CUnitAndFormatSwitch();
	//设置要读的字符串语言
//	static BOOL SetLanguage( enLanguage);

	//取当前语言类型
	static enLanguage GetCurrencyLanguage();
	
	//数值附加里程单位
	static CString NumAddDistanceUnit(FLOAT iDistance);

	//数值附加速度单位
	static CString NumAddSpeedUnit(FLOAT iSpeed,BOOL IsKmH = FALSE);
	
	//数值附加时刻单位
	static CString NumAddTimeUnit(UINT iHour, UINT iMinute = -1, UINT iSec = -1);

	//数值附加时间单位
	static CString NumAddHourUnit(UINT iSecond);

	//度附加单位,LongitudeOrLatitude是经度，FALSE是纬度
	static CString NumAddLongitudeALatitudeUnit(FLOAT fLoLaUint, BOOL LongitudeOrLatitude);

	//角度附加单位，如果当前是弧度，则返回相应的弧度,参数角度值 
	static CString NumAddAngleUnit( FLOAT , UINT availability = 5, UINT decimal = 1);

	//设置当前的时间格式
	static BOOL SetCurTimeUnit( enTimeUnit );

	//设置当前里程单位
	static BOOL SetCurDistanceUnit( enDistanceUnit );

	//设置当前速度单位
	static BOOL SetCurSpeedUnit( enSpeedUnit );

	//设置当前时间单位
	static BOOL SetCurHourUnit( enHourUnit );

	//设置当前经纬度单位
	static BOOL SetCurLongitudeALatitudeUnit( enLongitudeALatitudeUnit );

	//设置当前是角度还是弧度
	static BOOL SetAngleUnit( enAngleUnit );

	//取当前时间单位
	static enHourUnit GetCurHourUnit();

	//取当前里程单位
	static enDistanceUnit GetCurDistanceUnit( );

	//取当前速度单位
	static enSpeedUnit GetCurSpeedUnit( );

	//取当前时刻单位
	static enTimeUnit GetCurTimeUnit( );

	//取当前经纬度单位
	static enLongitudeALatitudeUnit GetCurLongitudeALatitudeUnit();

	//取当前是角度还是弧度
	static enAngleUnit GetAngleUnit();

//	static enTimeZone GetCurTimeZone();
//	static BOOL SetCueTimeZone(enTimeZone);
//	static enTimeZone g_TimeZone;

	///时区
	static INT32 GetTimeZone();
	static BOOLEAN SetTimeZone(INT32 Bias);
	static CString GetTimeZoneInfo();

	//当前速度单位
	static enSpeedUnit g_SpeedUnit;
	//当前时刻格式
	static enTimeUnit g_TimeFormat;
	//当前里程单位
	static enDistanceUnit g_DistanceUnit;
	//时间格式
	static enHourUnit g_HourUnit;
	//经纬度单位
	static enLongitudeALatitudeUnit g_LongitudeALatitudeUnit;
	//当前角度或弧度类型
	static enAngleUnit g_Angle;

	static CString GetVoiceDisText(int dis); //语音距离文本

	//释放语言
//	static void ReleaseLanguage();

	//语言配置文件
//	static CIniFile m_IniFile;

	//当前语言
	static enLanguage g_Language;
	
	//以下是各种语言文件路径
//	static CString g_csChinese;
//	static CString g_csEnglish;
//	static CString g_csGermany;
//	static CString g_csFrance;
//	static CString g_csJapan;
//	static CString g_csItaly;
//	static CString g_csRussia;
// 	static CString g_csIndia;
};

#endif // !defined(AFX_CUNITANDFORMATSWITCH_H__5A22D0AD_74BF_4690_B957_865D5FA55B70__INCLUDED_)
