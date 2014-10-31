// SysConfig.h: interface for the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSCONFIG_H__B43181FF_8F45_475A_81BB_872143416752__INCLUDED_)
#define AFX_SYSCONFIG_H__B43181FF_8F45_475A_81BB_872143416752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfigBase.h"

enum enVolume
{
	ID_VOLUME_0 =0 ,
	ID_VOLUME_1 = 1,
	ID_VOLUME_2 = 2,
	ID_VOLUME_3 = 3,
	ID_VOLUME_4 = 4,
	ID_VOLUME_5 = 5,
	ID_VOLUME_6 =6 ,
	ID_VOLUME_7 = 7,
	ID_VOLUME_8 = 8,
	ID_VOLUME_9 = 9,
	ID_VOLUME_10 = 10,
	ID_VOLUME_11 = 11,
	ID_VOLUME_12 =12 ,
	ID_VOLUME_13 = 13,
	ID_VOLUME_14 = 14,
	ID_VOLUME_15 = 15,
	ID_VOLUME_16 = 16,
	ID_VOLUME_17 = 17,
	ID_VOLUME_18 =18 ,
	ID_VOLUME_19 = 19,
	ID_VOLUME_20 = 20

};

enum enBrightness
{
	ID_BRIGHTNESS_INIT =0 ,
	ID_BRIGHTNESS_LOW = 1,
	ID_BRIGHTNESS_MIDDLE = 2,
	ID_BRIGHTNESS_HIGHT = 3
};

enum enKeySound
{
	KeySnd_none = 0,
	KeySnd_1,
	KeySnd_2,
	KeySnd_3,
	KeySnd_4,
};

struct ChileProcessInfo
{
	CString ExeName;
	CString EXEPath;
	CString eXEIniName;
};

class CSysConfig  : public CConfigBase
{
public:

	/**************************************************/
	int GetVerVol( );
	/**************************************************/


	virtual ~CSysConfig();

	static CSysConfig& Instance(); 

	virtual void ResetDefault();

	enKeySound GetKeySoundType();
	void SetKeySoundType( enKeySound );

	int GetAVInfo();
	void SetAVInfo( int );

	//自动关背光时间
	UINT GetSpareTime();
	void SetSpareTime(UINT);

	UINT  GetGSMVol();
	void  SetGSMVol(UINT);

	void SetBrightness(enBrightness);
	enBrightness GetBrightness();
	
	void SetTimeUnit(enTimeUnit);
	enTimeUnit GetTimeUnit();

	void SetTimeZone(int);
 	int GetTimeZone();

	void SetBtVolume(int);
	int GetBtVolume();

	void  SetGSMSelect( int nSel);
	int   GetGSMSelect();

	void  SetGSMType(int nType);
	int   GetGSMType();


	BOOL GetNaviInfo(ChileProcessInfo&);
	BOOL GetMoviesInfo(ChileProcessInfo&);
	BOOL GetMusicInfo(ChileProcessInfo&);
	BOOL GetBlueToothInfo(ChileProcessInfo&);
	BOOL GetPictureInfo(ChileProcessInfo&);
	BOOL GetVolumeInfo(ChileProcessInfo&);
	BOOL GetTxtInfo(ChileProcessInfo&);
	void SetIsSpareMode(bool);
	BOOL GetIsSpareMode();
	
	BOOL GetAutoBkLight();
	void SetAutoBkLight( bool blight );

	BOOL GetAutoAccept();
	void SetAutoAccept( int bSwitch );
	void SetBtPin( CString path );
	CString GetBtPin();

	BOOL GetBtSwitch();
	void SetBtSwitch( bool bSwitch );

	void SetNaviPath( CString path );
	CString GetNaviPath();

	void SetAppPath( CString appPath );
	CString GetAppPath();

	BOOL GetAutoNavi();
	void SetAutoNavi( bool iNavi );

	void SetSysVol(int nVol);
	int  GetSysVol();

	void SetAmplifierVol(int nVol);
	int  GetAmplifierVol();

	void SetCenterNum( CString strNum );
	CString GetCenterNum();

	void SetHospitalNum( CString strNum );
	CString GetHospitalNum();

	void SetFireNum( CString strNum );
	CString GetFireNum();

	void SetAlarmNum( CString strNum );
	CString GetAlarmNum();

	void SetGearSwitch( int nNum );
	int GetGearSwitch();

	void SetPushToTalk( BOOL iNavi );
	BOOL GetPushToTalk();

	//设置关机前最后一个Dialog的ID号,用于开机启动,0表示不启动任何页面
	//仅用于中心信息,救援信息,定期定点列表3个页面
	void	SetLastDialogID(int nIDD);	
	//读取关机前最后一个Dialog的ID号
	int		GetLastDialogID();

	//[Function]
	BOOL GetPhone();
	BOOL GetICCard();
	CString GetTianzeAppPath();

	//[COM]
	void SetComICCard( int nCom );
	int GetComICCard();
	void SetComScanner( int nCom );
	int GetComScanner();
	void SetComRFID( int nCom );
	int GetComRFID();


private:

	CSysConfig();
	static CSysConfig _instance;

	virtual void ResetChildConfigDefault(CString Path,CString Name);
};

#endif // !defined(AFX_SYSCONFIG_H__B43181FF_8F45_475A_81BB_872143416752__INCLUDED_)
