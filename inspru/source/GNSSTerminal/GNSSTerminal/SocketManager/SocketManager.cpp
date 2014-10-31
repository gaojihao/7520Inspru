/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* �ļ�����: TZ_Udp_Class.cpp

* �ļ���ʶ: UDPͨѶ��̬��
* ժ    Ҫ: 

* ��    ��:  
* �������: 2011/03/21
* ��ע: 
************************************************************************/
#include "stdafx.h"
#include "../SocketManager/SocketManager.h"
#include "../Tools/LogTool.h"
#include "../MsgProc/MsgProc.h"


WORD CSocketManager::m_wLocalPort = 3000;
/*	
CSocketManager�Ĺ��캯��
@���������ն����ݵĻص�����  	*/
CSocketManager::CSocketManager(RECEIVE_DATA pfnRecvFunc, SOCKETERROR pfnErrorFunc, BYTE cSocketType/* = IPPROTO_UDP*/)
{
	m_pfnRecvData		= pfnRecvFunc;
	m_pfnError			= pfnErrorFunc;
	m_hUdpRecvThread	= NULL;
	m_UdpSocket			= 0;
	m_nUdpStatus		= -1;	
	ZeroMemory(&m_ServerAddr, sizeof(sockaddr_in));
	m_cSocketType		= cSocketType;	//socket����
	m_bConnected		= false;
}

//��������
CSocketManager::~CSocketManager(void)
{
}

/*	
OpenSocketConnect() ��Socket����
@���������ӷ�����IP��ַ���������˿ڣ����ض˿�  */
BOOL CSocketManager::OpenSocketConnect(char *pszServiceAddr, WORD wServicePort/*, WORD wLocalPort*/)
{
	if (m_nUdpStatus == 0)
	{
		return TRUE;
	}
	WSADATA wsaData;


	//����Windows Winsock DLL
	if (WSAStartup(MAKEWORD(2,2),&wsaData))
	{
		WSACleanup();
		return FALSE;
	}

	//����UDP�׽���
	if(IPPROTO_UDP == m_cSocketType)
		m_UdpSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	//����TCP�׽��� 
	else if(IPPROTO_TCP == m_cSocketType)
		m_UdpSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	// �����׽���ʧ��
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

	//ʹ���Լ���IP��ַ
	localAddr.sin_addr.s_addr = INADDR_ANY;
	//ֻ��UDPʱ��ʹ��bind
	if(IPPROTO_UDP == m_cSocketType)
	{
		//�󶨵�ַ
		if(bind(m_UdpSocket, (sockaddr*)&localAddr, sizeof(localAddr)) != 0)
		{
			return FALSE;
		}
	}

	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port = (u_short)htons(wServicePort);  

	
	//�����������ַ
	m_ServerAddr.sin_addr.s_addr = inet_addr(pszServiceAddr);

	//���ӷ�����
	m_nUdpStatus = connect (m_UdpSocket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));

	if(m_nUdpStatus == SOCKET_ERROR)
	{
		int iErrorCode = WSAGetLastError();
		CLogTool::Instance()->WriteLogFile("connect WSAGetLastError:%d", iErrorCode);
		return FALSE;
	}

	m_bConnected = true;
	//����ͨѶģʽΪ�첽ģʽ
	DWORD ul= 1;
	ioctlsocket(m_UdpSocket, FIONBIO, &ul);

	BOOL bNodelay = TRUE;
	setsockopt(m_UdpSocket,IPPROTO_TCP,TCP_NODELAY,(const char*)&bNodelay,sizeof(BOOL));

	if (m_hUdpRecvThread == NULL)
	{
		// ����UDPͨѶ�߳�
		m_hUdpRecvThread = CreateThread (NULL, 0, UdpRecvThreadFunc, this, 0, NULL);
	}

	// ����UDPͨѶ�߳�ʧ��
	if (m_hUdpRecvThread == NULL)                                                      
	{
		closesocket(m_UdpSocket);
		return FALSE;
	}

	return TRUE;
}

/*	
CloseUdpConnection() �ر�UDP����
@������												 */
BOOL CSocketManager::CloseUdpConnection(void)
{
	if (m_nUdpStatus  == -1)
	{
		return FALSE;
	}
	CLogTool::Instance()->WriteLogFile(_T("CloseUdpConnection"));
	m_bConnected = false;
	//ͨѶ�߳̽����¼���Ϊ�ź�̬
	Sleep(1000);

	shutdown (m_UdpSocket, 2);

	//�ر�Socket���ͷ���Դ
	int err = closesocket(m_UdpSocket);

	m_nUdpStatus  = -1;
	if (err == SOCKET_ERROR)
	{
		return FALSE;
	}

	//�ͷ�socket��Դ
	WSACleanup();
	CLogTool::Instance()->WriteLogFile(_T("CloseUdpConnection finish"));
	return TRUE;
}

/*	
SendData() ��������
@�������ֽ���������							 */
int CSocketManager::SendData(const char *pszBuffer, int nLen)
{

	if (m_nUdpStatus != 0)
	{
		return -1;
	}
	//�ѷ����ֽ���
	int nBytes = 0;

	//���η����ֽ���
	int nSendBytes = 0;

	//������
	int nErrorCode =0; 

	//�Ƿ�ȫ���������
	while (nSendBytes < nLen)
	{
		//���η����ֽ���
		//UDP��������
		if(IPPROTO_UDP == m_cSocketType)
			nBytes = sendto(m_UdpSocket, pszBuffer + nSendBytes, nLen - nSendBytes, 0, (sockaddr *)&m_ServerAddr, sizeof(m_ServerAddr));
		//TCP��������
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

		//�ѷ����ֽ����ۼ�
		nSendBytes = nSendBytes + nBytes;

		//���û��ȫ���������
		if (nSendBytes < nLen)
		{
			Sleep(500);
		}
	} 
	return nSendBytes; 
}

/*	
GetUdpStatus() ��ȡUDP����״̬
@������									 */
int CSocketManager::GetUdpStatus()
{
	return m_nUdpStatus;
}


DWORD CSocketManager::UdpRecvThreadFunc(LPVOID lparam)
{
	CSocketManager *pUdpMgr = static_cast<CSocketManager *>(lparam);

	//������¼�����
	fd_set fdRead;
	//�Ƿ����¼�����
	int eventResult = -1;

	//�����¼��ȴ�ʱ��
	TIMEVAL	aTime;

	//��ʼ��ʱ��ȴ�ʱ��
	aTime.tv_sec = 1;
	aTime.tv_usec = 0;

	int iErrorCode = 0;
	while (1)
	{
		if (pUdpMgr->m_bConnected)
		{
			//�ÿ�fdRead�¼�Ϊ��
			FD_ZERO(&fdRead);

			//���ͻ���socket���ö��¼�����gUdpSocket�׽�����ӵ�������
			FD_SET(pUdpMgr->m_UdpSocket, &fdRead);

			//����select�������ж��Ƿ��ж��¼�����
			eventResult = select(0, &fdRead, NULL, NULL, &aTime);

			//�յ�����
			if (eventResult > 0)
			{
				if (FD_ISSET(pUdpMgr->m_UdpSocket, &fdRead))
				{
					//�������¼�
					char szReadBuf[4096];
					//���յ������ݳ���
					int nReadLen;
					ZeroMemory(szReadBuf, 4096);
					//��������
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
						//�������ݽ����¼�
						if (pUdpMgr->m_pfnRecvData != NULL)
						{
							//���ûص�����
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
