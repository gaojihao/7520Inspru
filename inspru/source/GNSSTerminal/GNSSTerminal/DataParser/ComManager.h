/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* 文件名称:	ComManager.h
* 文件标识:	
* 摘    要:	通讯管理类，管理串口通讯和socket通讯，
* 作    者:	
* 完成日期: 2012/07/12
* 备注:		根据机器类型自动判定一体机和调度屏
*			一体机：GPS协议--串口，808协议--Socket
*			调度屏：GPS协议--串口，808协议--串口
************************************************************************/
#pragma once

#include "../DataParser/DataType.h"
#include "../SocketManager/SocketManager.h"

class CComManager
{
private:
	static CComManager*		m_pInstance;			//Singleton模式实例指针
	CSocketManager*			m_pSocket;				//Socket连接

public:
	CComManager(void);
	~CComManager(void);

	//Singleton模式实例函数
	static CComManager* Instance();
	//初始化底层通讯
	BOOL	InitCom();
	//串口配置
	//发送数据
	int		SendData(PBYTE pBuffer, DWORD dwLen);
	int		GetConnectStatus();

	//一体机通讯函数////////////////////////////////////////////////////////////////////////
	//打开Socket连接
	BOOL	OpenSocketConnect(char *pszServiceAddr, WORD wServicePort);
	void	CloseSocketConnect();
	//Socket接收数据
	static int	SocketReceive(BYTE *pBuffer, DWORD dwLen);
	static void	SocketError(int nErrorCode);
};
