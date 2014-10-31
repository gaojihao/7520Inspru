/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* 文件名称: SocketManager.h

* 文件标识: UDP通讯动态库
* 摘    要: 

* 作    者:  
* 完成日期: 2011/03/21
* 备注: 
************************************************************************/
#ifndef _TZ_UDP_CLASS_H_
#define _TZ_UDP_CLASS_H_

#include <Windows.h>
#include "winsock.h"

typedef int (CALLBACK *RECEIVE_DATA)(BYTE *cBuffer, DWORD dwLen);

//Socket客户端发送错误回调函数
typedef void (*SOCKETERROR)(int nErrorCode);

class CSocketManager
{
public:
	/*	
		CSocketManager的构造函数
			@参数：接收读数据的回调函数  	IPPROTO_UDP	IPPROTO_TCP*/
  	 CSocketManager(RECEIVE_DATA pfnRecvFunc, SOCKETERROR pfnErrorFunc, BYTE cSocketType = IPPROTO_UDP);

	 //析构函数
	~CSocketManager(void);

	/*	
		OpenSocketConnect() 打开Socket连接
			@参数：连接服务器IP地址，服务器端口，本地端口  */
	BOOL OpenSocketConnect(char *pszServiceAddr, WORD wServicePort/*, WORD wLocalPort*/);

	/*	
		CloseUdpConnection() 关闭UDP连接
			@参数：												 */
	BOOL CloseUdpConnection(void);

	/*	
		SendData() 发送数据
			@参数：字节流，长度							 */
	int SendData(const char *pszBuffer, int nLen);

	/*	
		GetUdpStatus() 获取UDP连接状态 
			@参数：
			@返回值：0 表成功				*/
	int GetUdpStatus();


private:
	CSocketManager(void);
	static DWORD UdpRecvThreadFunc(LPVOID lparam);


public:
	//UDP接收数据回调函数
	RECEIVE_DATA		m_pfnRecvData;	

	//UDP发生错误事件
	SOCKETERROR			m_pfnError;

private:
	HANDLE			m_hUdpRecvThread;				//读线程句柄
	SOCKET			m_UdpSocket;					//UDP连接句柄（描述符）
	sockaddr_in		m_ServerAddr;					//sock地址结构体
	int				m_nUdpStatus;					//UDP连接状态		（0 表成功）
	static WORD		m_wLocalPort;					//本地端口
	BYTE			m_cSocketType;					//IPPROTO_UDP:  UDP		IPPROTO_TCP:  TCP
	bool			m_bConnected;
};

#endif /* _TZ_UDP_CLASS_H_ */
