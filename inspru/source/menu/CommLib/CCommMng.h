// CommMng.h: interface for the CommMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMMNG_H__BB4B692C_2147_4DCD_ADFE_5215D45DB27F__INCLUDED_)
#define AFX_COMMMNG_H__BB4B692C_2147_4DCD_ADFE_5215D45DB27F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSerial.h"
#include "CE920EventProtocol.h"
#include "CE920Event.h"

class CCommMng  : public CSerial
{
public:
	static CCommMng& Instance();
	virtual ~CCommMng();

	//注册绑定在设备上的协议
	BOOL RegeditProtocol(PROTOCOL_TYPE = en_E920Event_PROTOCOL);

	//注销绑定在设备上的协议
	BOOL UnRegeditProtocol();

	//初始化串口设备
	BOOL OpenDevice(DWORD dwPort, DWORD dwBaudRate);

	//设置接收数据窗口
	void SetRecvWnd(HWND hWnd);
	
	//开始读线程
	BOOL BeginRecvThread();

	//判断读线程是否启动
	BOOL IsRecvThreadRuning();

	//挂起读线程
	BOOL SuspendRecvThread();

	//恢复读线程
	BOOL ResumeRecvThread();

	//结束读线程
	BOOL EndRecvThread();

	//发送数据
	BOOL SendData(CE920Event* lpInData);

	//接收数据
	BOOL RecvData(CE920Event*& lpOutData);

	//关闭设备
	void CloseDevice();

private:
	static CCommMng _instance;
	CCommMng();
	CE920EventProtocol* m_pProtocol;
	BOOL  m_bProtocolState;
};

#endif // !defined(AFX_COMMMNG_H__BB4B692C_2147_4DCD_ADFE_5215D45DB27F__INCLUDED_)
