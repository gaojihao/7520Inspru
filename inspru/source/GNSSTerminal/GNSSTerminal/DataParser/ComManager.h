/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* �ļ�����:	ComManager.h
* �ļ���ʶ:	
* ժ    Ҫ:	ͨѶ�����࣬������ͨѶ��socketͨѶ��
* ��    ��:	
* �������: 2012/07/12
* ��ע:		���ݻ��������Զ��ж�һ����͵�����
*			һ�����GPSЭ��--���ڣ�808Э��--Socket
*			��������GPSЭ��--���ڣ�808Э��--����
************************************************************************/
#pragma once

#include "../DataParser/DataType.h"
#include "../SocketManager/SocketManager.h"

class CComManager
{
private:
	static CComManager*		m_pInstance;			//Singletonģʽʵ��ָ��
	CSocketManager*			m_pSocket;				//Socket����

public:
	CComManager(void);
	~CComManager(void);

	//Singletonģʽʵ������
	static CComManager* Instance();
	//��ʼ���ײ�ͨѶ
	BOOL	InitCom();
	//��������
	//��������
	int		SendData(PBYTE pBuffer, DWORD dwLen);
	int		GetConnectStatus();

	//һ���ͨѶ����////////////////////////////////////////////////////////////////////////
	//��Socket����
	BOOL	OpenSocketConnect(char *pszServiceAddr, WORD wServicePort);
	void	CloseSocketConnect();
	//Socket��������
	static int	SocketReceive(BYTE *pBuffer, DWORD dwLen);
	static void	SocketError(int nErrorCode);
};
