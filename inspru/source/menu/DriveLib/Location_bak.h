// Location.h : main header file for the Location DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "UsCommDef.h"
#include "resource.h"


// CLocationApp
// See Location.cpp for the implementation of this class
//

#ifdef LOCATION_EXPORTS
#define LOCATION_API  __declspec(dllexport)
#else
#define LOCATION_API  __declspec(dllimport)
#endif

class CGPSData;

// This class is exported from the Location.dll
class LOCATION_API CLocation  
{
public:
//	CLocation();
//	virtual ~CLocation();

	//设置串口号和波特率
	BOOL SetPort(int nPort, long lBaud);

	//打开串口  TRUE－打开成功
	BOOL Open();

	//关闭串口	TRUE－关闭成功
	BOOL Close();

	//TRUE－串口打开 FALSE－串口关闭
	BOOL IsOpen();

	//获取GPS数据信息 TRUE－取得数据 FALSE－没有取得数据
	BOOL GetGpsData(SGpsData &GpsData);

	//获取DOP数据信息 TRUE－取得数据 FALSE－没有取得数据
	BOOL GetDopData(SDopData &DopData);

	//获取卫星数据信息 TRUE－取得数据 FALSE－没有取得数据
	BOOL GetSatData(SSatData *SatData, int& rRevNum);

	//设置窗口句柄
	BOOL SetAimHwnd(HWND hwnd);

	//设置是否将信号记录
	BOOL SetbRecord(bool bRec, TCHAR *FilePath);
};