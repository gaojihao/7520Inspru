/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* 文件名称: TZ_Udp_Class.cpp

* 文件标识: UDP通讯动态库
* 摘    要: 

* 作    者:  
* 完成日期: 2011/03/21
* 备注: 
************************************************************************/
#include "stdafx.h"
#include "../SocketManager/SocketManager.h"
#include "../Tools/LogTool.h"
#include "../MsgProc/MsgProc.h"


WORD CSocketManager::m_wLocalPort = 3000;
/*	
CSocketManager的构造函数
@参数：接收读数据的回调函数  	*/
CSocketManager::CSocketManager(RECEIVE_DATA pfnRecvFunc, SOCKETERROR pfnErrorFunc, BYTE cSocketType/* = IPPROTO_UDP*/)
{
	m_pfnRecvData		= pfnRecvFunc;
	m_pfnError			= pfnErrorFunc;
	m_hUdpRecvThread	= NULL;
	m_UdpSocket			= 0;
	m_nUdpStatus		= -1;	
	ZeroMemory(&m_ServerAddr, sizeof(sockaddr_in));
	m_cSocketType		= cSocketType;	//socket类型
	m_bConnected		= false;
}

//析构函数
CSocketManager::~CSocketManager(void)
{
}

/*	
OpenSocketConnect() 打开Socket连接
@参数：连接服务器IP地址，服务器端口，本地端口  */
BOOL CSocketManager::OpenSocketConnect(char *pszServiceAddr, WORD wServicePort/*, WORD wLocalPort*/)
{
	if (m_nUdpStatus == 0)
	{
		return TRUE;
	}
	WSADATA wsaData;


	//调用Windows Winsock DLL
	if (WSAStartup(MAKEWORD(2,2),&wsaData))
	{
		WSACleanup();
		return FALSE;
	}

	//创建UDP套接字
	if(IPPROTO_UDP == m_cSocketType)
		m_UdpSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	//创建TCP套接字 
	else if(IPPROTO_TCP == m_cSocketType)
		m_UdpSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	// 建立套接字失败
	if(m_UdpSocket == SOCKET_ERROR)
	{
		return FALSE;
	}

	sockaddr_in localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = (u_short)htons(m_wLocalPort++);

	if (m_wLocalPort >= 65535)
	{
		m_wLocalPort = 3000;
	}

	//使用自己的IP地址
	localAddr.sin_addr.s_addr = INADDR_ANY;
	//只有UDP时才使用bind
	if(IPPROTO_UDP == m_cSocketType)
	{
		//绑定地址
		if(bind(m_UdpSocket, (sockaddr*)&localAddr, sizeof(localAddr)) != 0)
		{
			return FALSE;
		}
	}

	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port = (u_short)htons(wServicePort);  

	
	//保存服务器地址
	m_ServerAddr.sin_addr.s_addr = inet_addr(pszServiceAddr);

	//连接服务器
	m_nUdpStatus = connect (m_UdpSocket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));

	if(m_nUdpStatus == SOCKET_ERROR)
	{
		int iErrorCode = WSAGetLastError();
		CLogTool::Instance()->WriteLogFile("connect WSAGetLastError:%d", iErrorCode);
		return FALSE;
	}

	m_bConnected = true;
	//设置通讯模式为异步模式
	DWORD ul= 1;
	ioctlsocket(m_UdpSocket, FIONBIO, &ul);

	BOOL bNodelay = TRUE;
	setsockopt(m_UdpSocket,IPPROTO_TCP,TCP_NODELAY,(const char*)&bNodelay,sizeof(BOOL));

	if (m_hUdpRecvThread == NULL)
	{
		// 创建UDP通讯线程
		m_hUdpRecvThread = CreateThread (NULL, 0, UdpRecvThreadFunc, this, 0, NULL);
	}

	// 创建UDP通讯线程失败
	if (m_hUdpRecvThread == NULL)                                                      
	{
		closesocket(m_UdpSocket);
		return FALSE;
	}

	return TRUE;
}

/*	
CloseUdpConnection() 关闭UDP连接
@参数：												 */
BOOL CSocketManager::CloseUdpConnection(void)
{
	if (m_nUdpStatus  == -1)
	{
		return FALSE;
	}
	CLogTool::Instance()->WriteLogFile(_T("CloseUdpConnection"));
	m_bConnected = false;
	//通讯线程结束事件置为信号态
	Sleep(1000);

	shutdown (m_UdpSocket, 2);

	//关闭Socket，释放资源
	int err = closesocket(m_UdpSocket);

	m_nUdpStatus  = -1;
	if (err == SOCKET_ERROR)
	{
		return FALSE;
	}

	//释放socket资源
	WSACleanup();
	CLogTool::Instance()->WriteLogFile(_T("CloseUdpConnection finish"));
	return TRUE;
}

/*	
SendData() 发送数据
@参数：字节流，长度							 */
int CSocketManager::SendData(const char *pszBuffer, int nLen)
{

	if (m_nUdpStatus != 0)
	{
		return -1;
	}
	//已发送字节数
	int nBytes = 0;

	//本次发送字节数
	int nSendBytes = 0;

	//错误码
	int nErrorCode =0; 

	//是否全部发送完毕
	while (nSendBytes < nLen)
	{
		//本次发送字节数
		//UDP发送数据
		if(IPPROTO_UDP == m_cSocketType)
			nBytes = sendto(m_UdpSocket, pszBuffer + nSendBytes, nLen - nSendBytes, 0, (sockaddr *)&m_ServerAddr, sizeof(m_ServerAddr));
		//TCP发送数据
		else if(IPPROTO_TCP == m_cSocketType)
			nBytes = send(m_UdpSocket, pszBuffer + nSendBytes, nLen - nSendBytes, 0);

		if (nBytes == SOCKET_ERROR )
		{
			nErrorCode = WSAGetLastError();

			CLogTool::Instance()->WriteLogFile("Send WSAGetLastError:%d", nErrorCode);

			if (m_pfnError != NULL)
			{
				(*m_pfnError)(nErrorCode);
			}
			return -1;
		}

		//已发送字节数累加
		nSendBytes = nSendBytes + nBytes;

		//如果没有全部发送完毕
		if (nSendBytes < nLen)
		{
			Sleep(500);
		}
	} 
	return nSendBytes; 
}

/*	
GetUdpStatus() 获取UDP连接状态
@参数：									 */
int CSocketManager::GetUdpStatus()
{
	return m_nUdpStatus;
}


DWORD CSocketManager::UdpRecvThreadFunc(LPVOID lparam)
{
	CSocketManager *pUdpMgr = static_cast<CSocketManager *>(lparam);

	//定义读事件集合
	fd_set fdRead;
	//是否有事件发生
	int eventResult = -1;

	//定义事件等待时间
	TIMEVAL	aTime;

	//初始化时间等待时间
	aTime.tv_sec = 1;
	aTime.tv_usec = 0;

	int iErrorCode = 0;
	while (1)
	{
		if (pUdpMgr->m_bConnected)
		{
			//置空fdRead事件为空
			FD_ZERO(&fdRead);

			//给客户端socket设置读事件，将gUdpSocket套接字添加到集合中
			FD_SET(pUdpMgr->m_UdpSocket, &fdRead);

			//调用select函数，判断是否有读事件发生
			eventResult = select(0, &fdRead, NULL, NULL, &aTime);

			//收到数据
			if (eventResult > 0)
			{
				if (FD_ISSET(pUdpMgr->m_UdpSocket, &fdRead))
				{
					//发生读事件
					char szReadBuf[4096];
					//接收到的数据长度
					int nReadLen;
					ZeroMemory(szReadBuf, 4096);
					//接收数据
					nReadLen = recv(pUdpMgr->m_UdpSocket, szReadBuf, 4096, 0);
					if (nReadLen <= 0)
					{
						iErrorCode = WSAGetLastError();

						CLogTool::Instance()->WriteLogFile("SOCKET_ERROR WSAGetLastError:%d", iErrorCode);

						pUdpMgr->m_bConnected = false;

						CMsgProc::Instance()->SendServerMsg(10054, 0, FALSE);

					}
					else
					{
						//触发数据接收事件
						if (pUdpMgr->m_pfnRecvData != NULL)
						{
							//调用回调函数
							(*(pUdpMgr->m_pfnRecvData))((PBYTE)szReadBuf, nReadLen);
						}
					}
				}
			}
		}
		Sleep(10);
	}
	return 0;
}
