/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* �ļ�����: SocketManager.h

* �ļ���ʶ: UDPͨѶ��̬��
* ժ    Ҫ: 

* ��    ��:  
* �������: 2011/03/21
* ��ע: 
************************************************************************/
#ifndef _TZ_UDP_CLASS_H_
#define _TZ_UDP_CLASS_H_

#include <Windows.h>
#include "winsock.h"

typedef int (CALLBACK *RECEIVE_DATA)(BYTE *cBuffer, DWORD dwLen);

//Socket�ͻ��˷��ʹ���ص�����
typedef void (*SOCKETERROR)(int nErrorCode);

class CSocketManager
{
public:
	/*	
		CSocketManager�Ĺ��캯��
			@���������ն����ݵĻص�����  	IPPROTO_UDP	IPPROTO_TCP*/
  	 CSocketManager(RECEIVE_DATA pfnRecvFunc, SOCKETERROR pfnErrorFunc, BYTE cSocketType = IPPROTO_UDP);

	 //��������
	~CSocketManager(void);

	/*	
		OpenSocketConnect() ��Socket����
			@���������ӷ�����IP��ַ���������˿ڣ����ض˿�  */
	BOOL OpenSocketConnect(char *pszServiceAddr, WORD wServicePort/*, WORD wLocalPort*/);

	/*	
		CloseUdpConnection() �ر�UDP����
			@������												 */
	BOOL CloseUdpConnection(void);

	/*	
		SendData() ��������
			@�������ֽ���������							 */
	int SendData(const char *pszBuffer, int nLen);

	/*	
		GetUdpStatus() ��ȡUDP����״̬ 
			@������
			@����ֵ��0 ��ɹ�				*/
	int GetUdpStatus();


private:
	CSocketManager(void);
	static DWORD UdpRecvThreadFunc(LPVOID lparam);


public:
	//UDP�������ݻص�����
	RECEIVE_DATA		m_pfnRecvData;	

	//UDP���������¼�
	SOCKETERROR			m_pfnError;

private:
	HANDLE			m_hUdpRecvThread;				//���߳̾��
	SOCKET			m_UdpSocket;					//UDP���Ӿ������������
	sockaddr_in		m_ServerAddr;					//sock��ַ�ṹ��
	int				m_nUdpStatus;					//UDP����״̬		��0 ��ɹ���
	static WORD		m_wLocalPort;					//���ض˿�
	BYTE			m_cSocketType;					//IPPROTO_UDP:  UDP		IPPROTO_TCP:  TCP
	bool			m_bConnected;
};

#endif /* _TZ_UDP_CLASS_H_ */
