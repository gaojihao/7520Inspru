// CUiMng.h: interface for the CUiMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUIMNG_H__62BF02BC_8525_4A37_9776_A8F9D6393682__INCLUDED_)
#define AFX_CUIMNG_H__62BF02BC_8525_4A37_9776_A8F9D6393682__INCLUDED_

#include "Afxtempl.h"
#include "CResBitmap.h"
#include "CResString.h"
#include "CSkinBitmap.h"
#include "CUnitAndFormatSwitch.h"
#include "DialogBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_CLOSE_ModalDLG WM_USER + 319

class CUiMng  
{
public:
	virtual ~CUiMng();

	static CUiMng& Instance();

	UINT GetWndCount();

	void SetLanguagePath( const CString& LanguagePath);
	void SetSkinPath( const CString& SkinPath);

	//多语言管理
	BOOL SetLanguage( enLanguage );					//设置语言
	enLanguage GetCurLanguage();					//取当前语言

	//皮肤管理
	BOOL SetSkinType( enSkinType );					//设置皮肤
	enSkinType GetCurSkin();						//取当前皮肤

//	//时区
//	BOOL SetCueTimeZone(enTimeZone);
//	enTimeZone GetCurTimeZone();

	//单位、格式管理
	BOOL SetTimeUnit( enTimeUnit );				//设置时间单位
	BOOL SetDistanceUnit( enDistanceUnit );		//设置里程单位
	BOOL SetSpeedUnit( enSpeedUnit );			//设置速度单位
	BOOL SetHoueUnit( enHourUnit );				//设置时刻格式
	BOOL SetLongitudeALatitudeUnit( enLongitudeALatitudeUnit );	//设置经纬度格式
	BOOL SetAngleUnit( enAngleUnit );				//设置角度还是弧度单位

	enDistanceUnit GetCurDistanceUnit( );			//取当前里程单位
	enSpeedUnit GetCurSpeedUnit( );					//取当前速度单位
	enTimeUnit GetCurTimeUnit( );					//取当前时刻格式
	enHourUnit GetCurHoueUnit();					//取当前时间单位
	enLongitudeALatitudeUnit GetCurLongitudeALatitudeUnit(); //取当前经纬度格式
	enAngleUnit GetCurAngleUnit();					//取角度还是弧度单位

	BOOL Attach(CDialogBase* pDialogBase);
	BOOL Detach(CDialogBase* pDialogBase);
	//Detach所有窗口,回到主菜单,added by 陈国乐
	BOOL DetachAll();

private:
	CUiMng();
	static CUiMng _instance;

	//已创建窗口指针
	CArray<CDialogBase*,CDialogBase*&> m_WndArray;
//	CMap<enWndId,enWndId,CDialogBase*,CDialogBase*&> m_WndMap;

	
	//当前窗口指针
	CDialogBase* m_curWnd;

	void ReleaseLanguage();
	void ReleaseSkin();

};

#endif // !defined(AFX_CUIMNG_H__62BF02BC_8525_4A37_9776_A8F9D6393682__INCLUDED_)
