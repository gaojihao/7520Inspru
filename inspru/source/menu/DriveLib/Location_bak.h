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

	//���ô��ںźͲ�����
	BOOL SetPort(int nPort, long lBaud);

	//�򿪴���  TRUE���򿪳ɹ�
	BOOL Open();

	//�رմ���	TRUE���رճɹ�
	BOOL Close();

	//TRUE�����ڴ� FALSE�����ڹر�
	BOOL IsOpen();

	//��ȡGPS������Ϣ TRUE��ȡ������ FALSE��û��ȡ������
	BOOL GetGpsData(SGpsData &GpsData);

	//��ȡDOP������Ϣ TRUE��ȡ������ FALSE��û��ȡ������
	BOOL GetDopData(SDopData &DopData);

	//��ȡ����������Ϣ TRUE��ȡ������ FALSE��û��ȡ������
	BOOL GetSatData(SSatData *SatData, int& rRevNum);

	//���ô��ھ��
	BOOL SetAimHwnd(HWND hwnd);

	//�����Ƿ��źż�¼
	BOOL SetbRecord(bool bRec, TCHAR *FilePath);
};