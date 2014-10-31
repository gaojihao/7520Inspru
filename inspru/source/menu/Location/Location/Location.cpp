// Location.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Location.h"
//#include "resource.h"
#include "PosData/PosData.h"
#include "./FileMapping/FileMapping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE				g_hMemShare;			//共享内存句柄
extern PPOS_INFO	g_pPosInfo;				//共享内存区指针

BOOL Open(WORD wPosCom, WORD wBaudRate, enPOS_DATA_MODE data_mode /*= MODE_SGL_GPS*/)
{
	BOOL bOK = FALSE;
	bOK = CPosData::Instance()->InitPosCom(wPosCom, wBaudRate, data_mode);
	if( bOK )
		g_hMemShare = InitFileMapping(&g_pPosInfo);

	return bOK;
}

BOOL Close()
{
	BOOL bOK = FALSE;
	bOK = CPosData::Instance()->CloseCom();
	UnInitFileMapping(g_hMemShare, &g_pPosInfo);
	return bOK;
}

//设置BD2/GPS模块数据模式
BOOL SetDataMode(enPOS_DATA_MODE data_mode)
{
	return CPosData::Instance()->SetDataMode(data_mode);
}

//获取BD2/GPS模块数据模式
enPOS_DATA_MODE GetDataMode()
{
	return CPosData::Instance()->GetDataMode();
}

//获取存放位置信息的共享内存
const POS_INFO* GetSharePosInfo()
{
	static PPOS_INFO	pPosInfo	= NULL; 
	InitFileMapping(&pPosInfo, FILE_MAP_READ);
	return pPosInfo;
}

//添加窗口句柄，以获取位置信息消息
//BOOL AddHWND(HWND hWnd)
//{
//	return CPosData::Instance()->AddHWND(hWnd);
//}

////删除窗口句柄
//void RemoveHWND(HWND hWnd)
//{
//	CPosData::Instance()->RemoveHWND(hWnd);
//}

//使能转换成GPS数据
BOOL EnableConvertGPS(WORD wConvertCom)
{
	return CPosData::Instance()->EnableConvertGPS(wConvertCom);
}

//Disable转换GPS数据
void DisableConvertGPS()
{
	CPosData::Instance()->DisableConvertGPS();
}
