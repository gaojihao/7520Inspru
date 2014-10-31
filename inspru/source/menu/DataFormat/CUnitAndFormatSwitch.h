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
	//����Ҫ�����ַ�������
//	static BOOL SetLanguage( enLanguage);

	//ȡ��ǰ��������
	static enLanguage GetCurrencyLanguage();
	
	//��ֵ������̵�λ
	static CString NumAddDistanceUnit(FLOAT iDistance);

	//��ֵ�����ٶȵ�λ
	static CString NumAddSpeedUnit(FLOAT iSpeed,BOOL IsKmH = FALSE);
	
	//��ֵ����ʱ�̵�λ
	static CString NumAddTimeUnit(UINT iHour, UINT iMinute = -1, UINT iSec = -1);

	//��ֵ����ʱ�䵥λ
	static CString NumAddHourUnit(UINT iSecond);

	//�ȸ��ӵ�λ,LongitudeOrLatitude�Ǿ��ȣ�FALSE��γ��
	static CString NumAddLongitudeALatitudeUnit(FLOAT fLoLaUint, BOOL LongitudeOrLatitude);

	//�Ƕȸ��ӵ�λ�������ǰ�ǻ��ȣ��򷵻���Ӧ�Ļ���,�����Ƕ�ֵ 
	static CString NumAddAngleUnit( FLOAT , UINT availability = 5, UINT decimal = 1);

	//���õ�ǰ��ʱ���ʽ
	static BOOL SetCurTimeUnit( enTimeUnit );

	//���õ�ǰ��̵�λ
	static BOOL SetCurDistanceUnit( enDistanceUnit );

	//���õ�ǰ�ٶȵ�λ
	static BOOL SetCurSpeedUnit( enSpeedUnit );

	//���õ�ǰʱ�䵥λ
	static BOOL SetCurHourUnit( enHourUnit );

	//���õ�ǰ��γ�ȵ�λ
	static BOOL SetCurLongitudeALatitudeUnit( enLongitudeALatitudeUnit );

	//���õ�ǰ�ǽǶȻ��ǻ���
	static BOOL SetAngleUnit( enAngleUnit );

	//ȡ��ǰʱ�䵥λ
	static enHourUnit GetCurHourUnit();

	//ȡ��ǰ��̵�λ
	static enDistanceUnit GetCurDistanceUnit( );

	//ȡ��ǰ�ٶȵ�λ
	static enSpeedUnit GetCurSpeedUnit( );

	//ȡ��ǰʱ�̵�λ
	static enTimeUnit GetCurTimeUnit( );

	//ȡ��ǰ��γ�ȵ�λ
	static enLongitudeALatitudeUnit GetCurLongitudeALatitudeUnit();

	//ȡ��ǰ�ǽǶȻ��ǻ���
	static enAngleUnit GetAngleUnit();

//	static enTimeZone GetCurTimeZone();
//	static BOOL SetCueTimeZone(enTimeZone);
//	static enTimeZone g_TimeZone;

	///ʱ��
	static INT32 GetTimeZone();
	static BOOLEAN SetTimeZone(INT32 Bias);
	static CString GetTimeZoneInfo();

	//��ǰ�ٶȵ�λ
	static enSpeedUnit g_SpeedUnit;
	//��ǰʱ�̸�ʽ
	static enTimeUnit g_TimeFormat;
	//��ǰ��̵�λ
	static enDistanceUnit g_DistanceUnit;
	//ʱ���ʽ
	static enHourUnit g_HourUnit;
	//��γ�ȵ�λ
	static enLongitudeALatitudeUnit g_LongitudeALatitudeUnit;
	//��ǰ�ǶȻ򻡶�����
	static enAngleUnit g_Angle;

	static CString GetVoiceDisText(int dis); //���������ı�

	//�ͷ�����
//	static void ReleaseLanguage();

	//���������ļ�
//	static CIniFile m_IniFile;

	//��ǰ����
	static enLanguage g_Language;
	
	//�����Ǹ��������ļ�·��
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
