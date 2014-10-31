// SysConfig.cpp: implementation of the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSysConfig CSysConfig::_instance;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysConfig::CSysConfig() 
{

}

CSysConfig::~CSysConfig()
{

}

CSysConfig& CSysConfig::Instance()
{
	return _instance;
}

void CSysConfig::ResetDefault()
{
	CConfigBase::ResetDefault();

	//额外处理代码
//	CUiMng::Instance().SetSkinType(GetSkinType());
//	CUiMng::Instance().SetLanguage(GetLanguage());

	
	CUnitAndFormatSwitch::SetTimeZone(GetTimeZone());
	

	//读取子配置信息
	ChileProcessInfo ProcessInfo;

	CString m_csRadioName;
	CString m_csRadioIconName;

	CString m_csPicName;
	CString m_csPicIconName;

	CString m_csMusicName;
	CString m_csMusicIconName;

	CString m_csMovieName;
	CString m_csMovieIconName;

	CString m_csNaviName;
	CString m_csNaviIconName;

	if(!CSysConfig::Instance().GetNaviInfo(ProcessInfo))
	{
		
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}

	if(!CSysConfig::Instance().GetMoviesInfo(ProcessInfo))
	{
		
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}
	
	if(!CSysConfig::Instance().GetMusicInfo(ProcessInfo))
	{
		
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}
	
	if(!CSysConfig::Instance().GetBlueToothInfo(ProcessInfo))
	{
		
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}
	
	if(!CSysConfig::Instance().GetPictureInfo(ProcessInfo))
	{
	
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}
	
	if(!CSysConfig::Instance().GetVolumeInfo(ProcessInfo))
	{
	
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}

	if(!CSysConfig::Instance().GetTxtInfo(ProcessInfo))
	{
	
	}
	else
	{
//		m_csNaviName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.ExeName;
// 		m_csNaviIconName = ProcessInfo.EXEPath + _T("\\") + ProcessInfo.eXEIniName;
		ResetChildConfigDefault(ProcessInfo.EXEPath,ProcessInfo.eXEIniName);
	}
	
}

enKeySound CSysConfig::GetKeySoundType()
{
	int iValue = 1;
	m_ConfigFile.GetInt(_T("Set"),_T("KeyVol"),iValue);
	return enKeySound(iValue);	
}

void CSysConfig::SetKeySoundType( enKeySound KeySnd )
{
	m_ConfigFile.SetInt(_T("Set"),_T("KeyVol"),(int)KeySnd);
	m_ConfigFile.Flush();
}

void CSysConfig::SetBrightness(enBrightness bright)
{
	m_ConfigFile.SetInt(_T("Set"),_T("Brightness"),(int)bright);
	m_ConfigFile.Flush();
	//添加额外处理代码
}

enBrightness CSysConfig::GetBrightness()
{
	int iValue = 10;
	m_ConfigFile.GetInt(_T("Set"),_T("Brightness"),iValue);
	return enBrightness(iValue);
}

void CSysConfig::SetTimeUnit(enTimeUnit timeUnit)
{
	m_ConfigFile.SetInt(_T("Set"),_T("TimeFormat"),(int)timeUnit);
	m_ConfigFile.Flush();
	CUiMng::Instance().SetTimeUnit(timeUnit);
}

enTimeUnit CSysConfig::GetTimeUnit()
{
	int iValue = 10;
	m_ConfigFile.GetInt(_T("Set"),_T("TimeFormat"),iValue);
	return enTimeUnit(iValue);	
}

void CSysConfig::SetTimeZone(int zone)
{
	m_ConfigFile.SetInt(_T("Set"),_T("TimeZone"),(int)zone);
	m_ConfigFile.Flush();
}

int CSysConfig::GetTimeZone()
{
	int iValue = 10;
	m_ConfigFile.GetInt(_T("Set"),_T("TimeZone"),iValue);
	return iValue;
}


//////////////////////////////////////////////////////////////////////////

UINT  CSysConfig::GetGSMVol()
{
	int vol = 60;
	m_ConfigFile.GetInt(_T("Set"),_T("GSMVolume"),vol);
	return vol;
}

void  CSysConfig::SetGSMVol(UINT vol)
{
	m_ConfigFile.SetInt(_T("Set"),_T("GSMVolume"),vol);
	m_ConfigFile.Flush();
}

void CSysConfig::SetBtVolume(int vol)
{
	m_ConfigFile.SetInt(_T("Set"),_T("BTVolume"),vol);
//	m_ConfigFile.Flush();

}

int CSysConfig::GetBtVolume()
{
	int vol = 5;
	m_ConfigFile.GetInt(_T("Set"),_T("BTVolume"),vol);
	return vol;
}



int CSysConfig::GetVerVol( )
{
	int vol = 0;
	m_ConfigFile.GetInt(_T("Set"),_T("VerVol"),vol);
	return vol;
}


int CSysConfig::GetAVInfo()
{
	int av = 0;
	m_ConfigFile.GetInt(_T("Set"),_T("AV"),av);
	return av;
}

void CSysConfig::SetAVInfo( int Avinfo )
{
	m_ConfigFile.SetInt(_T("Set"),_T("AV"),Avinfo);
	m_ConfigFile.Flush();
}


void  CSysConfig::SetGSMSelect( int nSel )
{
	m_ConfigFile.SetInt(_T("GSM"),_T("NetSel"),nSel);
}

int   CSysConfig::GetGSMSelect()
{
	int nVal = 0;
	m_ConfigFile.GetInt(_T("GSM"),_T("NetSel"),nVal);
	return nVal;
}

void  CSysConfig::SetGSMType( int nType )
{
	m_ConfigFile.SetInt(_T("GSM"),_T("NetType"),nType );
}

int   CSysConfig::GetGSMType()
{
	int nVal = 0;
	m_ConfigFile.GetInt(_T("GSM"),_T("NetType"),nVal);
	return nVal;
}

BOOL CSysConfig::GetNaviInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("Navi"),_T("NaviName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("Navi"),_T("NaviPath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("Navi"),_T("NaviIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}

BOOL CSysConfig::GetMoviesInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("Video"),_T("VideoName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("Video"),_T("VideoPath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("Video"),_T("VideoIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}

BOOL CSysConfig::GetMusicInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("Music"),_T("MusicName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("Music"),_T("MusicPath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("Music"),_T("MusicIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}

BOOL CSysConfig::GetBlueToothInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("BlueTooth"),_T("BlueToothName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("BlueTooth"),_T("BlueToothPath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("BlueTooth"),_T("BlueToothIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}

BOOL CSysConfig::GetPictureInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("Picture"),_T("PictureName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("Picture"),_T("PicturePath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("Picture"),_T("PictureIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}


BOOL CSysConfig::GetVolumeInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("Volume"),_T("VolumeName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("Volume"),_T("VolumePath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("Volume"),_T("VolumeIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}

BOOL CSysConfig::GetTxtInfo(ChileProcessInfo& processInfo)
{
	CString str;
	BOOL bName = m_ConfigFile.GetString(_T("Txt"),_T("TxtName"),str);
	processInfo.ExeName = str;
	BOOL bPath = m_ConfigFile.GetString(_T("Txt"),_T("TxtPath"),str);
	processInfo.EXEPath = str;
	BOOL bIni = m_ConfigFile.GetString(_T("Txt"),_T("TxtIni"),str);
	processInfo.eXEIniName = str;
	return (bName&&bPath&&bIni);
}

void CSysConfig::SetIsSpareMode(bool mode)
{
	m_ConfigFile.SetBool(_T("Set"),_T("SpareMode"),mode);
	m_ConfigFile.Flush();

	CFile file;
	file.Open(_T("\\Flashdrv Storage\\menu\\Fm.ini"), CFile::modeReadWrite | CFile::modeCreate );
	char n = '0';
	if ( mode )
		n = '1';
	file.Write(&n,1);
	file.Close();
	//添加额外处理代码
}

BOOL CSysConfig::GetIsSpareMode()
{
	bool bValue = false;
	m_ConfigFile.GetBool(_T("Set"),_T("SpareMode"),bValue);
	return bValue;
}

void CSysConfig::ResetChildConfigDefault(CString Path,CString Name)
{
	CString ConfigCompletePath;
	ConfigCompletePath = Path + _T("\\") + Name;

	DeleteFile(ConfigCompletePath);

	CFile ConfigFile;
	CString defaultCompletePath;
	defaultCompletePath = Path + _T("\\Default.ini");
	CFile defaultfile;

	SetFileAttributes(ConfigCompletePath, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(defaultCompletePath, FILE_ATTRIBUTE_NORMAL);
	
	CFileException e;
	TRY
	{
		if(ConfigFile.Open(ConfigCompletePath, 
			CFile::modeCreate | CFile::modeReadWrite,&e) &&
		defaultfile.Open(defaultCompletePath, 
			CFile::modeRead,&e))
		{
			DWORD len = defaultfile.GetLength();

			char* pBuf = new char[len];
			defaultfile.Read(pBuf,len);

			ConfigFile.Write(pBuf,len);

			if(pBuf != NULL)
			{
				delete pBuf;
				pBuf = NULL;
			}

			ConfigFile.Close();
			defaultfile.Close();
		}

	}
	CATCH(CException , e)
	{
		ConfigFile.Close();
		defaultfile.Close();
		return;
	}
	END_CATCH

}

UINT CSysConfig::GetSpareTime()
{
	CString strTemp;
	m_ConfigFile.GetString(_T("Set"),_T("SpareTime"),strTemp);
	
	double numTemp;
	numTemp = wcstod((LPCTSTR)strTemp,_T('\0'));
	return (UINT)numTemp;
}

void CSysConfig::SetSpareTime(UINT uFreqValue)
{
	m_ConfigFile.SetInt(_T("Set"),_T("SpareTime"),(int)uFreqValue);
	m_ConfigFile.Flush();
}


BOOL CSysConfig::GetAutoBkLight()
{
	bool temp = false;
	m_ConfigFile.GetBool(_T("Set"),_T("AutoBklight"),temp);
	
	return temp;
}

void CSysConfig::SetAutoBkLight( bool blight )
{
	m_ConfigFile.SetInt(_T("Set"),_T("AutoBklight"),blight);
	m_ConfigFile.Flush();
}

	
BOOL CSysConfig::GetBtSwitch()
{
	bool temp = false;
	m_ConfigFile.GetBool(_T("Set"),_T("BtSwitch"),temp);
	
	return temp;
}

void CSysConfig::SetBtSwitch( bool bSwitch )
{
	m_ConfigFile.SetInt(_T("Set"),_T("BtSwitch"),bSwitch);
	m_ConfigFile.Flush();
}


void CSysConfig::SetNaviPath( CString path )
{
	m_ConfigFile.SetString(_T("Set"),_T("NaviPath"),path);
	m_ConfigFile.Flush();
}



CString CSysConfig::GetNaviPath()
{
	CString temp;
	m_ConfigFile.GetString(_T("Set"),_T("NaviPath"),temp);
	
	return temp;
}


void CSysConfig::SetAppPath( CString path )
{
	m_ConfigFile.SetString(_T("Set"),_T("AppPath"),path);
	m_ConfigFile.Flush();
}

CString CSysConfig::GetAppPath()
{
	CString temp;
	m_ConfigFile.GetString(_T("Set"),_T("AppPath"),temp);
	
	return temp;
}

BOOL CSysConfig::GetAutoNavi()
{
	int temp = 0;
	m_ConfigFile.GetInt(_T("Set"),_T("AutoNavi"),temp);
	
	return temp;
}

void CSysConfig::SetAutoNavi( bool bNavi )
{
	m_ConfigFile.SetInt(_T("Set"),_T("AutoNavi"),bNavi);
	m_ConfigFile.Flush();
}


BOOL CSysConfig::GetAutoAccept()
{
	int temp = 0;
	m_ConfigFile.GetInt(_T("Set"),_T("AutoAccept"),temp);
	
	return temp;
}

void CSysConfig::SetAutoAccept( int bSwitch )
{
	m_ConfigFile.SetInt(_T("Set"),_T("AutoAccept"),bSwitch);
	m_ConfigFile.Flush();
}

void CSysConfig::SetBtPin( CString path )
{
	m_ConfigFile.SetString(_T("Set"),_T("BtPin"),path);
	m_ConfigFile.Flush();
}

CString CSysConfig::GetBtPin()
{
	CString temp;
	m_ConfigFile.GetString(_T("Set"),_T("BtPin"), temp );
	return temp;
}

//////////////////////////////////////////////////////////////////////////

int  CSysConfig::GetSysVol()
{
	int temp = 10;
	m_ConfigFile.GetInt(_T("Set"),_T("SysVol"),temp);
	return temp;
}
void CSysConfig::SetSysVol(int nVol)
{
	m_ConfigFile.SetInt(_T("Set"),_T("SysVol"),nVol);
	m_ConfigFile.Flush();
}


void CSysConfig::SetAmplifierVol(int nVol)
{
	m_ConfigFile.SetInt(_T("Set"),_T("AmplifierVol"),nVol);
	m_ConfigFile.Flush();
}
int  CSysConfig::GetAmplifierVol()
{
	int temp = 15;
	m_ConfigFile.GetInt(_T("Set"),_T("AmplifierVol"),temp);
	return temp;
}

//////////////////////////////////////////////////////////////////////////


void CSysConfig::SetCenterNum( CString strNum )
{
	m_ConfigFile.SetString(_T("Number"),_T("CenterNum"),strNum);
	m_ConfigFile.Flush();
}
CString CSysConfig::GetCenterNum()
{
	CString temp;
	m_ConfigFile.GetString(_T("Number"),_T("CenterNum"),temp);
	
	return temp;
}

void CSysConfig::SetHospitalNum( CString strNum )
{
	m_ConfigFile.SetString(_T("Number"),_T("HospitalNum"),strNum);
	m_ConfigFile.Flush();
}
CString CSysConfig::GetHospitalNum()
{
	CString temp;
	m_ConfigFile.GetString(_T("Number"),_T("HospitalNum"),temp);
	
	return temp;
}

void CSysConfig::SetFireNum( CString strNum )
{
	m_ConfigFile.SetString(_T("Number"),_T("FireNum"),strNum);
	m_ConfigFile.Flush();
}
CString CSysConfig::GetFireNum()
{
	CString temp;
	m_ConfigFile.GetString(_T("Number"),_T("FireNum"),temp);
	
	return temp;
}
void CSysConfig::SetAlarmNum( CString strNum )
{
	m_ConfigFile.SetString(_T("Number"),_T("AlarmNum"),strNum);
	m_ConfigFile.Flush();
}
CString CSysConfig::GetAlarmNum()
{
	CString temp;
	m_ConfigFile.GetString(_T("Number"),_T("AlarmNum"),temp);
	
	return temp;
}

void CSysConfig::SetGearSwitch( int nNum )
{
	m_ConfigFile.SetInt(_T("Set"),_T("GearSwitch"),nNum);
	m_ConfigFile.Flush();
}
int CSysConfig::GetGearSwitch()
{
	int temp;
	m_ConfigFile.GetInt(_T("Set"),_T("GearSwitch"),temp);
	return temp;
}


BOOL CSysConfig::GetPhone()
{
	bool temp = false;
	m_ConfigFile.GetBool(_T("Function"),_T("Phone"),temp);
	
	return temp;
}

BOOL CSysConfig::GetICCard()
{
	bool temp = false;
	m_ConfigFile.GetBool(_T("Function"),_T("ICCard"),temp);
	
	return temp;
}

CString CSysConfig::GetTianzeAppPath()
{
	CString temp;
	m_ConfigFile.GetString(_T("Function"),_T("TianzeApp"),temp);
	
	return temp;
}
//设置关机前最后一个Dialog的ID号
void CSysConfig::SetLastDialogID(int nIDD)
{
	m_ConfigFile.SetInt(_T("Set"),_T("LastDialogID"),nIDD);
	m_ConfigFile.Flush();
}
//读取关机前最后一个Dialog的ID号
int	CSysConfig::GetLastDialogID()
{
	int nIDD;
	m_ConfigFile.GetInt(_T("Set"),_T("LastDialogID"),nIDD);
 	return nIDD;
}
//设置一键通功能
void CSysConfig::SetPushToTalk( BOOL bNavi )
{
	m_ConfigFile.SetInt(_T("Set"),_T("PushToTalk"),bNavi);
	m_ConfigFile.Flush();
}
BOOL CSysConfig::GetPushToTalk()
{
	int temp = 0;
	m_ConfigFile.GetInt(_T("Set"),_T("PushToTalk"),temp);
	
	return temp;
}

//[COM]
//IC卡串口
void CSysConfig::SetComICCard( int nCom )
{
	m_ConfigFile.SetInt(_T("COM"),_T("ICCard"),nCom);
	m_ConfigFile.Flush();
}
int CSysConfig::GetComICCard()
{
	int nCom;
	m_ConfigFile.GetInt(_T("COM"),_T("ICCard"),nCom);
	return nCom;
}
//扫描枪串口
void CSysConfig::SetComScanner( int nCom )
{
	m_ConfigFile.SetInt(_T("COM"),_T("Scanner"),nCom);
	m_ConfigFile.Flush();
}
int CSysConfig::GetComScanner()
{
	int nCom;
	m_ConfigFile.GetInt(_T("COM"),_T("Scanner"),nCom);
	return nCom;
}
//RFID串口
void CSysConfig::SetComRFID( int nCom )
{
	m_ConfigFile.SetInt(_T("COM"),_T("RFID"),nCom);
	m_ConfigFile.Flush();
}
int CSysConfig::GetComRFID()
{
	int nCom;
	m_ConfigFile.GetInt(_T("COM"),_T("RFID"),nCom);
	return nCom;
}

