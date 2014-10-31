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

HANDLE				g_hMemShare;			//�����ڴ���
extern PPOS_INFO	g_pPosInfo;				//�����ڴ���ָ��

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

//����BD2/GPSģ������ģʽ
BOOL SetDataMode(enPOS_DATA_MODE data_mode)
{
	return CPosData::Instance()->SetDataMode(data_mode);
}

//��ȡBD2/GPSģ������ģʽ
enPOS_DATA_MODE GetDataMode()
{
	return CPosData::Instance()->GetDataMode();
}

//��ȡ���λ����Ϣ�Ĺ����ڴ�
const POS_INFO* GetSharePosInfo()
{
	static PPOS_INFO	pPosInfo	= NULL; 
	InitFileMapping(&pPosInfo, FILE_MAP_READ);
	return pPosInfo;
}

//��Ӵ��ھ�����Ի�ȡλ����Ϣ��Ϣ
//BOOL AddHWND(HWND hWnd)
//{
//	return CPosData::Instance()->AddHWND(hWnd);
//}

////ɾ�����ھ��
//void RemoveHWND(HWND hWnd)
//{
//	CPosData::Instance()->RemoveHWND(hWnd);
//}

//ʹ��ת����GPS����
BOOL EnableConvertGPS(WORD wConvertCom)
{
	return CPosData::Instance()->EnableConvertGPS(wConvertCom);
}

//Disableת��GPS����
void DisableConvertGPS()
{
	CPosData::Instance()->DisableConvertGPS();
}
