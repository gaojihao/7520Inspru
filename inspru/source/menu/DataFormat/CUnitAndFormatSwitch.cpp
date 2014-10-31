// CUnitAndFormatSwitch.cpp: implementation of the CUnitAndFormatSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CUnitAndFormatSwitch.h"
#include "CResString.h"
#include "DmmUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CIniFile CUnitAndFormatSwitch::m_IniFile;

//CString CUnitAndFormatSwitch::g_csChinese = "\\NAND Flash\\Language\\StrLanguage\\Chinese.ini";
//CString CUnitAndFormatSwitch::g_csEnglish = "\\NAND Flash\\Language\\StrLanguage\\English.ini";
//CString CUnitAndFormatSwitch::g_csGermany = "\\NAND Flash\\Language\\StrLanguage\\Germany.ini";
//CString CUnitAndFormatSwitch::g_csFrance = "\\NAND Flash\\Language\\StrLanguage\\France.ini";
//CString CUnitAndFormatSwitch::g_csJapan = "\\NAND Flash\\Language\\StrLanguage\\Japan.ini";
//CString CUnitAndFormatSwitch::g_csItaly = "\\NAND Flash\\Language\\StrLanguage\\Italy.ini";
//CString CUnitAndFormatSwitch::g_csRussia = "\\NAND Flash\\Language\\StrLanguage\\Russia.ini";
//CString CUnitAndFormatSwitch::g_csIndia = "\\NAND Flash\\Language\\StrLanguage\\India.ini";

enLanguage CUnitAndFormatSwitch::g_Language = ID_CHINESE;

enSpeedUnit CUnitAndFormatSwitch::g_SpeedUnit = ID_KM_H;
enTimeUnit CUnitAndFormatSwitch::g_TimeFormat = ID_24_TYPE;
enDistanceUnit CUnitAndFormatSwitch::g_DistanceUnit = ID_KM;
enHourUnit CUnitAndFormatSwitch::g_HourUnit = ID_MINUTE;
enLongitudeALatitudeUnit CUnitAndFormatSwitch::g_LongitudeALatitudeUnit = ID_DEGREE;
enAngleUnit CUnitAndFormatSwitch::g_Angle = ID_Angle;

CUnitAndFormatSwitch::CUnitAndFormatSwitch()
{

}

CUnitAndFormatSwitch::~CUnitAndFormatSwitch()
{

}

//BOOL CUnitAndFormatSwitch::SetLanguage(enLanguage LanguageId)
//{
//	BOOL bRts = FALSE;
//
////	CreateDirectory(g_csLanguageDirPath,NULL);
//
//	if(m_IniFile.IsOpen())
//	{
//		m_IniFile.Close();
//	}
//
//	switch(LanguageId)
//	{
//	case ID_CHINESE:
//		{
// 			if(m_IniFile.Load(g_csChinese))
//			{
//				g_Language = ID_CHINESE;
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}
//
//	case ID_ENGLISH:
//		{
// 			if(m_IniFile.Load(g_csEnglish))
//			{
//				g_Language = ID_ENGLISH;
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}	
//	case ID_GERMANY:
//		{
// 			if(m_IniFile.Load(g_csGermany))
//			{
//				g_Language = ID_GERMANY;	
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}	
//	case ID_FRANCE:
//		{
// 			if(m_IniFile.Load(g_csFrance))
//			{
//				g_Language = ID_FRANCE;		
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}	
//	case ID_JAPAN:
//		{
// 			if(m_IniFile.Load(g_csJapan))
//			{
//				g_Language = ID_JAPAN;
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}	
//	case ID_ITALY:
//		{
// 			if(m_IniFile.Load(g_csItaly))
//			{
//				g_Language = ID_ITALY;
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}
//	case ID_RUSSIA:
//		{
// 			if(m_IniFile.Load(g_csRussia))
//			{
//				g_Language = ID_RUSSIA;	
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}
//	case ID_INDIA:
//		{
// 			if(m_IniFile.Load(g_csIndia))
//			{
//				g_Language = ID_INDIA;
//				bRts = TRUE;
//			}
//			else
//				bRts = FALSE;
//			break;
//		}
//	default:
//		bRts = FALSE;
//		break;
//	}
//
//	m_IniFile.Close();
//
//	return bRts;
//}

//void CUnitAndFormatSwitch::ReleaseLanguage()
//{
//	if(m_IniFile.IsOpen())
//	{
//		m_IniFile.Close();
//	}
//
// }

enLanguage CUnitAndFormatSwitch::GetCurrencyLanguage()
{
	return g_Language;
}

CString CUnitAndFormatSwitch::NumAddDistanceUnit(FLOAT iDistance)
{
	CString csSection = _T("DistanceUnit");
	CString csValue;
 	
	CString NameID;
	
	if(iDistance >= 1000.0)
	{
		NameID.Format(_T("%d"),g_DistanceUnit);
		if(CResString::m_IniFile.GetString(csSection,NameID,csValue))
		{
			CString Distance;

			if(g_DistanceUnit == ID_MILT)
			{
				iDistance = (FLOAT)(iDistance/1000 * 1.609);
				Distance.Format(_T("%f"),iDistance);
				UINT pos = Distance.Find('.',0) + 2;
				Distance.Delete(pos,Distance.GetLength()-pos);
				csValue = Distance + csValue;
			}
			else if( g_DistanceUnit == ID_KM )
			{
				iDistance = iDistance/1000;
				Distance.Format(_T("%f"),iDistance);
				UINT pos = Distance.Find('.',0) + 2;
				Distance.Delete(pos,Distance.GetLength()-pos);
				csValue = Distance + csValue;
			}

			return csValue;
		}
		else
			return _T("");
	}
	else if(iDistance < 1000.0 && iDistance >= 0.0)
	{
		if( g_DistanceUnit == ID_MILT)
			NameID.Format(_T("%d"),2);
		else if( g_DistanceUnit = ID_KM )
			NameID.Format(_T("%d"),3);

		if(CResString::m_IniFile.GetString(csSection,NameID,csValue))
		{
			CString Distance;

			if(g_DistanceUnit == ID_MILT)
			{
				iDistance = (FLOAT)(iDistance/0.3048);
				Distance.Format(_T("%f"),iDistance);
				UINT pos = Distance.Find('.',0);
				Distance.Delete(pos,Distance.GetLength()-pos);
				csValue = Distance + csValue;
			}
			else if( g_DistanceUnit == ID_KM )
			{
				iDistance = iDistance;
				Distance.Format(_T("%f"),iDistance);
				UINT pos = Distance.Find('.',0);
				Distance.Delete(pos,Distance.GetLength()-pos);
				csValue = Distance + csValue;
			}

			return csValue;
		}
		else
			return _T("");
	}
	else
		return _T("");
}


CString CUnitAndFormatSwitch::NumAddSpeedUnit(FLOAT iSpeed,BOOL IsKmH)
{
	CString csSection = _T("SpeedUnit");
	CString csValue;
 	CString NameID;

	if(IsKmH)
	{
		CString Speed;
		NameID.Format(_T("%d"),ID_KM_H);
		if(CResString::m_IniFile.GetString(csSection,NameID,csValue))
			Speed.Format(_T("%d"),(UINT)iSpeed);
		csValue = Speed + csValue;
		return csValue;
	}


	NameID.Format(_T("%d"),g_SpeedUnit);
	
	if(CResString::m_IniFile.GetString(csSection,NameID,csValue))
	{
		if(g_SpeedUnit == ID_M_S)
		{
			iSpeed = iSpeed;
		}
		else if(g_SpeedUnit == ID_KM_H)
		{
			iSpeed = iSpeed/1000*3600;
 		}

		CString Speed;
		Speed.Format(_T("%f"),iSpeed);
		UINT pos = Speed.Find('.',0) + 2;
		Speed.Delete(pos,Speed.GetLength()-pos);
		csValue = Speed + csValue;
		return csValue;
	}
	else
		return _T("");

}	

CString CUnitAndFormatSwitch::NumAddTimeUnit(UINT iHour, UINT iMinute, UINT iSec)
{
	ASSERT(iHour >= 0 && iHour <= 23);
	ASSERT(iMinute >= 0 && iMinute <= 59);
	ASSERT(iSec >= 0 && iSec <= 59);

	CString csSection = _T("TimeUnit");
 	CString TIME = _T("");

	if(ID_24_TYPE == g_TimeFormat)	
	{
		if(iMinute<10 && iSec<10)
			TIME.Format(_T("%d:0%d:0%d"),iHour,iMinute,iSec);
		else if(iMinute<10 && iSec>=10)
			TIME.Format(_T("%d:0%d:%d"),iHour,iMinute,iSec);
		else if(iMinute>=10 && iSec<10)
			TIME.Format(_T("%d:%d:0%d"),iHour,iMinute,iSec);
		else
			TIME.Format(_T("%d:%d:%d"),iHour,iMinute,iSec);
	}
	else if(ID_12_TYPE == g_TimeFormat)
	{
		if(iHour > 12)
		{
			iHour = iHour - 12;
		if(iMinute<10 && iSec<10)
			TIME.Format(_T("PM %d:0%d:0%d"),iHour,iMinute,iSec);
		else if(iMinute<10 && iSec>=10)
			TIME.Format(_T("PM %d:0%d:%d"),iHour,iMinute,iSec);
		else if(iMinute>=10 && iSec<10)
			TIME.Format(_T("PM %d:%d:0%d"),iHour,iMinute,iSec);
		else
			TIME.Format(_T("PM %d:%d:%d"),iHour,iMinute,iSec);
		}
		else
		{
		if(iMinute<10 && iSec<10)
			TIME.Format(_T("AM %d:0%d:0%d"),iHour,iMinute,iSec);
		else if(iMinute<10 && iSec>=10)
			TIME.Format(_T("AM %d:0%d:%d"),iHour,iMinute,iSec);
		else if(iMinute>=10 && iSec<10)
			TIME.Format(_T("AM %d:%d:0%d"),iHour,iMinute,iSec);
		else
			TIME.Format(_T("AM %d:%d:%d"),iHour,iMinute,iSec);
		}
	}
	return TIME;
}

CString CUnitAndFormatSwitch::NumAddHourUnit(UINT iSecond)
{
	CString csSection = _T("HourUnit");
	CString csValue;
 	

	CString csHour;
	CString csMinute;
	csHour.Format(_T("%d"),ID_HOUR);
	csMinute.Format(_T("%d"),ID_MINUTE);

	CString csHValue;
	CString csMValue;
	CResString::m_IniFile.GetString(csSection,csHour,csHValue);
	CResString::m_IniFile.GetString(csSection,csMinute,csMValue);

//Modify On 2006 01 16
//如果时间小于一分钟，则一定以秒的形式出现
	if (iSecond < 60)
	{
		CString s;
		s.Format(_T("%d"), ID_SECOND);
		CResString::m_IniFile.GetString(csSection, s, csValue);
		s.Format(_T("%d"), iSecond);
		s += csValue;
		return s;
	}
	else if( iSecond >= 60 && iSecond < 3600)
	{
		CString Minute;
		iSecond = iSecond/60;
		Minute.Format(_T("%d"),iSecond);
		Minute = Minute + csMValue;
		return Minute;
 	}
	else if( iSecond >= 3600)
	{
		CString Hour;
		CString Minute;
		UINT ihour = iSecond/3600;
		UINT iLevSec = iSecond%3600;
		UINT iMinute = iLevSec/60;
		Hour.Format(_T("%d"),ihour);
		Minute.Format(_T("%d"),iMinute);
		Hour = Hour + csHValue + Minute + csMValue;
		return Hour;
	}
	else
		return _T("");

//	if(CResString::m_IniFile.GetString(csSection,NameID,csValue))
//	{
//		CString Hour;
//		if(g_HourUnit == ID_MINUTE)
//		{
//			iSecond = iSecond/60;
//			Hour.Format(_T("%d"),iSecond);
//		}
//		else if(g_HourUnit == ID_HOUR)
//		{
//			FLOAT hour = (FLOAT)iSecond/3600;
//			Hour.Format(_T("%f"),hour);
//		}
//
//		csValue = Hour + csValue;
//		return csValue;
// 	}


}

CString CUnitAndFormatSwitch::NumAddLongitudeALatitudeUnit(FLOAT fLoLaUint, BOOL LongitudeOrLatitude)
{
	CString NumAdded = _T("");
	if( g_LongitudeALatitudeUnit == ID_DEGREE)
	{
		if(LongitudeOrLatitude == TRUE)
		{
			if(fLoLaUint > 0.0)
				NumAdded.Format(_T("E%f°"),fLoLaUint);
			else
			{
				fLoLaUint = (FLOAT)0.0 - fLoLaUint;
				NumAdded.Format(_T("W%f°"),fLoLaUint);
			}
		}
		else if(LongitudeOrLatitude == FALSE)
		{
			if(fLoLaUint > 0.0)
				NumAdded.Format(_T("N%f°"),fLoLaUint);
			else
			{
				fLoLaUint = (FLOAT)0.0 - fLoLaUint;
				NumAdded.Format(_T("S%f°"),fLoLaUint);
			}
		}
	}
	else if(g_LongitudeALatitudeUnit == ID_DEGREE_MINUTE)
	{
		DOUBLE min = CDmmUtil::D_2_M(fLoLaUint);
		UINT degree = (UINT)min/60;
		UINT minute = (UINT)min%60;

		if(LongitudeOrLatitude == TRUE)
		{
			if(min > 0.0)
				NumAdded.Format(_T("E%d°%d′"),degree,minute);
			else
			{
				fLoLaUint = (FLOAT)0.0 - fLoLaUint;
				NumAdded.Format(_T("W%d°%d′"),degree,minute);
			}

		}
		else if(LongitudeOrLatitude == FALSE)
		{
			if(min > 0.0)
				NumAdded.Format(_T("N%d°%d′"),degree,minute);
			else
			{
				fLoLaUint = (FLOAT)0.0 - fLoLaUint;
				NumAdded.Format(_T("S%d°%d′"),degree,minute);
			}
		}
	}
	return NumAdded;
}


CString CUnitAndFormatSwitch::NumAddAngleUnit( FLOAT angle, UINT availability, UINT decimal)
{
	CString csFormat;
	csFormat.Format(_T("%%%d.%df"),availability,decimal);
	CString csAngle;
	if( g_Angle == ID_Angle )
	{
//		csAngle.Format(_T("%f°"),angle);
		csFormat += _T("°");
		swprintf(csAngle.GetBuffer(csAngle.GetLength()),
			csFormat.GetBuffer(csFormat.GetLength()),angle);
	}
	else if( g_Angle == ID_Radian )
	{
		FLOAT rad;
		if( angle > 0.0)
		{
			rad = (FLOAT)CDmmUtil::AngleToRadian(angle);
//			csAngle.Format(_T("%fRad"),rad);
			csFormat += _T("Rad");
			swprintf(csAngle.GetBuffer(csAngle.GetLength()),
				csFormat.GetBuffer(csFormat.GetLength()),rad);
		}
		else
		{
			angle = (FLOAT)360.0 + angle;
			rad = (FLOAT)CDmmUtil::AngleToRadian(angle);
	//		csAngle.Format(_T("%fRad"),rad);
			csFormat += _T("Rad");
			swprintf(csAngle.GetBuffer(csAngle.GetLength()),
				csFormat.GetBuffer(csFormat.GetLength()),rad);
		}
	}

	return csAngle;
}

BOOL CUnitAndFormatSwitch::SetCurTimeUnit(enTimeUnit enTime)
{
	g_TimeFormat = enTime;
	return TRUE;
}

BOOL CUnitAndFormatSwitch::SetCurDistanceUnit(enDistanceUnit enDistan)
{
	g_DistanceUnit = enDistan;
	return TRUE;
}

BOOL CUnitAndFormatSwitch::SetCurSpeedUnit(enSpeedUnit enTime)
{
	g_SpeedUnit = enTime;
	return TRUE;
}

BOOL CUnitAndFormatSwitch::SetCurHourUnit( enHourUnit enHour)
{
	g_HourUnit = enHour;
	return TRUE;
}

BOOL CUnitAndFormatSwitch::SetCurLongitudeALatitudeUnit( enLongitudeALatitudeUnit enLongitudeALatitude)
{
	g_LongitudeALatitudeUnit = enLongitudeALatitude;
	return TRUE;
}

BOOL CUnitAndFormatSwitch::SetAngleUnit( enAngleUnit angle)
{
	g_Angle = angle;
	return TRUE;
}

enDistanceUnit CUnitAndFormatSwitch::GetCurDistanceUnit( )
{
	return g_DistanceUnit;
}

enTimeUnit CUnitAndFormatSwitch::GetCurTimeUnit()
{
	return g_TimeFormat;
}

enSpeedUnit CUnitAndFormatSwitch::GetCurSpeedUnit()
{
	return g_SpeedUnit;
}

enHourUnit CUnitAndFormatSwitch::GetCurHourUnit()
{
	return g_HourUnit;
}

enLongitudeALatitudeUnit CUnitAndFormatSwitch::GetCurLongitudeALatitudeUnit()
{
	return g_LongitudeALatitudeUnit;
}

enAngleUnit CUnitAndFormatSwitch::GetAngleUnit()
{
	return g_Angle;
}

//BOOL CUnitAndFormatSwitch::SetCueTimeZone(enTimeZone TimeZone)
//{
//	g_TimeZone = TimeZone;
//	return TRUE;
//}
//
//enTimeZone CUnitAndFormatSwitch::GetCurTimeZone()
//{
//	return g_TimeZone;
// }

BOOLEAN CUnitAndFormatSwitch::SetTimeZone(INT32 Bias)
{
	if (Bias > 13 || Bias < -12)
		Bias = 0;
	int nIndex = Bias >= 0?Bias :13-Bias;

//	int nIndex = Bias;
	
	SYSTEMTIME st;
	GetSystemTime(&st);


	CString ZoneInfo;
	CString Name;
	Name.Format(_T("%d"),nIndex);
	CResString::m_IniFile.GetString(_T("TimeZone"), Name, ZoneInfo);
	wcscpy(gTziData[nIndex].StandardName,ZoneInfo);

	if(!SetTimeZoneInformation(&gTziData[nIndex]))
		return FALSE;

	return SetSystemTime(&st);
}

INT32 CUnitAndFormatSwitch::GetTimeZone()
{
	TIME_ZONE_INFORMATION tzi;
	GetTimeZoneInformation(&tzi);

	return -(tzi.Bias/60);
}

CString CUnitAndFormatSwitch::GetTimeZoneInfo()
{
	TIME_ZONE_INFORMATION tzi;
	GetTimeZoneInformation(&tzi);
	return tzi.StandardName;
}

//////////////////////////////////////////////////////////////////////////
/*处理语音播报的距离信息，只是针对语音
*/
CString CUnitAndFormatSwitch::GetVoiceDisText(int dis)
{
	if (dis < 0)//距离为负数
		return _T("");

	CString s;

	if (dis <= 1000)//此时精度为10米
	{
		dis = dis / 10 * 10;
		s.Format(_T("%d米"), dis);
	}
	else if (dis <= 10000)//此时精度为100米
	{
		dis = dis / 100 * 100;
		s.Format(_T("%d米"), dis);
	}
	else if (dis > 10000)//此时采用公里表示，精度0.1公里
	{
		dis = dis / 100 * 100;
		double d = ((double)(dis)) / 1000;
		s.Format(_T("%.1f公里"), d);
	}

	return s;
}