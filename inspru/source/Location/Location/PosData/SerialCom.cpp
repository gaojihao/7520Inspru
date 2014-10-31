/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* 文件名称:SerialCom.c

* 文件标识: 串口类
* 摘    要: 

* 作    者: 
* 完成日期: 2011/02/22
* 备注:在生成对象时，需要输入两个回调函数，原型为：

void (CALLBACK *READ_DATA)(BYTE *buf, DWORD buflen);
用于输出从串口读取的内容。

int(CALLBACK *ERROR_CONTENT)(LPCTSTR);
用于输出错误信息。
************************************************************************/
#include "stdafx.h"
#include "SerialCom.h"

//构造函数
CSerialCom::CSerialCom(ERROR_CONTENT FunctionnameError,READ_DATA FunctionnameRead)
{
	m_hComm = INVALID_HANDLE_VALUE;		/* 串口操作句柄无效 */
	m_hRecvThread = NULL;
	m_ExitThreadEvent = NULL;			/* 串口接收线程退出事件无效 */

	m_ErrorFoc = FunctionnameError;		//保存回调函数名
	m_ReadFoc = FunctionnameRead;
}

//析构函数
CSerialCom::~CSerialCom()
{
	//if (m_hRecvThread != NULL)
	//{
	//	CloseHandle(m_hRecvThread);
	//	SetEvent(m_ExitThreadEvent);				/* 通知串口接收线程退出 */
	//	Sleep(200);
	//	CloseHandle(m_ExitThreadEvent);				/* 关闭线程退出事件 */
	//}
}

/*******************************************************************************************
函数名称: CSerialCom::OpenPort
描    述: 打开串口
输入参数: 端口参数结构体，用户参数
输出参数: 若失败，调用回调函数，输出错误信息
返    回: FALSE: 失败;    TRUE: 成功
********************************************************************************************/
BOOL CSerialCom::OpenPort(Port_Parameters port_param)
{
	COMMTIMEOUTS CommTimeOuts;

	// 打开串口
	m_hComm = CreateFile(port_param.tzPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if (m_ErrorFoc !=NULL)
		{
			(*m_ErrorFoc)(_T("无法打开端口或端口已打开!请检查是否已被占用."));
		}
		return FALSE;
	}

	GetCommState(m_hComm, &m_dcb);						//读取串口的DCB,串口参数结构体
	m_dcb.BaudRate = port_param.nBaudRate;			
	m_dcb.ByteSize = port_param.nDataBits;
	m_dcb.Parity = port_param.nParity;
	m_dcb.StopBits = port_param.nStopBits;	
	m_dcb.fParity = FALSE;								// 禁止奇偶校验 
	m_dcb.fBinary = TRUE;

	if (FALSE == port_param.bDTRShake)
		m_dcb.fDtrControl = 0;							// 禁止流量控制
	else
		m_dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;

	if (FALSE ==port_param.bRTSShake )
		m_dcb.fRtsControl = 0;
	else
		m_dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

	m_dcb.fOutX = 0;
	m_dcb.fInX = 0;
	m_dcb.fTXContinueOnXoff = 0;
	
	//设置状态参数
	SetCommMask(m_hComm, EV_RXCHAR);					//串口事件:接收到一个字符 
	SetupComm(m_hComm, 16384, 16384);					// 设置接收与发送的缓冲区大小
	if(!SetCommState(m_hComm, &m_dcb))					// 设置串口的DCB 
	{
		if (m_ErrorFoc !=NULL)
		{
			(*m_ErrorFoc)(_T("无法按当前参数配置端口，请检查参数!"));
		}
		ClosePort();
		return FALSE;
	}
		
	//设置超时参数
	GetCommTimeouts(m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = 100;				// 接收字符间最大时间间隔
	CommTimeOuts.ReadTotalTimeoutMultiplier = 1;		
	CommTimeOuts.ReadTotalTimeoutConstant = 100;		// 读数据总超时常量
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;
	if(!SetCommTimeouts(m_hComm, &CommTimeOuts))
	{
		if (m_ErrorFoc != NULL)
		{
			(*m_ErrorFoc)(_T("无法设置超时参数!"));
		}
		ClosePort();
		return FALSE;
	}
		
	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);	 // 清除收/发缓冲区 
	

	// 创建线程及句柄
	DWORD IDThread;

	// 创建串口接收线程退出事件
	m_ExitThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		

	// 创建串口接收线程
	m_hRecvThread = CreateThread(0, 0, CommRecvTread, this, 0, &IDThread);
	if (m_hRecvThread == NULL) 
	{
		ClosePort();
		if (m_ErrorFoc != NULL)
		{
			(*m_ErrorFoc)(TEXT("创建接收线程失败!"));
		}
		return FALSE;
	}	
	
	return TRUE;		
}
/*******************************************************************************************
函数名称: CSerialCom::IsPortOpen()
描    述: 测试端口是否打开
输入参数: 无
输出参数: 无
返    回: FALSE: 关闭;    TRUE: 打开
********************************************************************************************/
BOOL CSerialCom::IsPortOpen()
{
	if(m_hComm != INVALID_HANDLE_VALUE) 
		return TRUE;

	return FALSE;
}
/*******************************************************************************************
函数名称: CSerialCom::ClosePort()
描    述: 关闭串口
输入参数: 无
输出参数: 无
返    回: FALSE: 失败;    TRUE: 成功
********************************************************************************************/
BOOL CSerialCom::ClosePort()
{

	if (m_hRecvThread != NULL)
	{
		CloseHandle(m_hRecvThread);
		SetEvent(m_ExitThreadEvent);				/* 通知串口接收线程退出 */
		CloseHandle(m_ExitThreadEvent);				/* 关闭线程退出事件 */
		Sleep(200);
		m_ExitThreadEvent = NULL;
	}

	if(m_hComm != INVALID_HANDLE_VALUE)
	{
		SetCommMask(m_hComm, 0);		
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);	/* 清除收/发缓冲 */
		CloseHandle(m_hComm);								/* 关闭串口操作句柄 */
		m_hComm = INVALID_HANDLE_VALUE;
		return TRUE;
	}

	return TRUE;
}

/*******************************************************************************************
函数名称: CSerialCom::Senddata(BYTE *psendbuf, DWORD length) 
描    述: 发送数据
输入参数: psendbuf 指向要发送的数据
			   length  数据长度
输出参数: 如发送失败，回调函数输出失败信息
返    回: 成功返回TRUE; 失败返回FALSE；
********************************************************************************************/
BOOL CSerialCom::SendData(BYTE *psendbuf, DWORD length) 
{
	DWORD dwactlen;

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		if (m_ErrorFoc != NULL)
		{
			(*m_ErrorFoc)(_T("串口未打开!"));
		}
		return FALSE;
	}

	BOOL flag = WriteFile(m_hComm, psendbuf, length, &dwactlen, NULL);	 /* 从串口发送数据 */
	if (!flag)
	{
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************************
函数名称: CSerialCom::CommRecvTread
描    述: 串口接收线程
输入参数: LPVOID lparam: 线程参数,创建线程时传入
输出参数: 若读取失败，回调函数输出错误信息
返    回: 0: 线程退出, 返回值没特殊含义
********************************************************************************************/
DWORD CSerialCom::CommRecvTread(LPVOID lparam)
{
	DWORD dwLength = 0;//接受实际读取的字节长度
	BYTE *recvBuf = new BYTE[4096];
	CSerialCom *pCeSerial = (CSerialCom *)lparam;  //空指针，强转为CSerialCom型

	while(TRUE)
	{																// 等待线程退出事件 ,此时这个0参数，说明立刻判断是否标记，然后返回
		if (WaitForSingleObject(pCeSerial->m_ExitThreadEvent, 0) == WAIT_OBJECT_0)
			break;	//函数作用为，当这个事件被标记（signaled），或者定时时间到，返回。 用SetEvent来标记

		if (pCeSerial->m_hComm != INVALID_HANDLE_VALUE)
		{
			ZeroMemory(recvBuf, 4096);
			// 从串口读取数据 
			ReadFile(pCeSerial->m_hComm, recvBuf, 4096, &dwLength, NULL);

			if(dwLength != 0 && pCeSerial->m_ReadFoc != NULL)	
			{
				(*(pCeSerial->m_ReadFoc))(recvBuf, dwLength);			//接收成功调用回调函数
			}
		}
	}		

	delete[] recvBuf;
	recvBuf =	NULL;
	return 0;
}
