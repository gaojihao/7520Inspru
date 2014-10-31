#include "StdAfx.h"
#include "ComManager.h"
#include "../lib/IniManager.h"
#include "../MsgProc/MsgProc.h"
#include "../DataParser/DataParser.h"
#include "../GPSData/GPSData.h"
#include "../Tools/LogTool.h"

//��ʼ����̬��Ա����
CComManager*	CComManager::m_pInstance	= NULL;

CComManager::CComManager(void)
{
	m_pSocket		= NULL;
}

CComManager::~CComManager(void)
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

	if(m_pSocket != NULL)
		delete m_pSocket;
}

/************************************************************************
* ������: Instance
* ��  ��: 
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: singleton
************************************************************************/
CComManager* CComManager::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CComManager;
	}
	return m_pInstance;
}

/************************************************************************
* ������:	InitCom
* ��  ��:	��ʼ���ײ�ͨѶ������һ����������ѡ��
* ��  ��:
* ��  ��: 
* ��  ��:	BOOL
* ��  ע: 
************************************************************************/
BOOL CComManager::InitCom()
{
	//1.��ʼ��Socket
	m_pSocket = new CSocketManager(SocketReceive, SocketError, IPPROTO_TCP);
	if (m_pSocket == NULL)
	{
		return 0;
	}
	return TRUE;
}

/************************************************************************
* ������:	SendData
* ��  ��:	��������
* ��  ��:
* ��  ��: 
* ��  ��:	int:	-1	����ʧ��	>0 ���ͳɹ�
* ��  ע: 
************************************************************************/
int CComManager::SendData(PBYTE pBuffer, DWORD dwLen)
{

	//��ͨ��ʱ��Ҫ��������
	if(m_pSocket == NULL)
		return -1;

	CLogTool::Instance()->WriteLogFile(_T("Send:"), pBuffer, (WORD)dwLen);

	return m_pSocket->SendData((const char*)pBuffer, dwLen);
}

/************************************************************************
* ������:	OpenSocketConnect
* ��  ��:	��Socket����
* ��  ��:
* ��  ��: 
* ��  ��:	int:	-1	����ʧ��	>0 ���ͳɹ�
* ��  ע: 
************************************************************************/
BOOL CComManager::OpenSocketConnect(char *pszServiceAddr, WORD wServicePort)
{
	if(m_pSocket == NULL)
		return FALSE;

	return m_pSocket->OpenSocketConnect(pszServiceAddr, wServicePort);
}

void CComManager::CloseSocketConnect()
{
	m_pSocket->CloseUdpConnection();
}

//Socket���ݴ���
int CComManager::SocketReceive(BYTE *pBuffer, DWORD dwLen)
{
	return CDataParser::Instance()->DataParser(pBuffer, (WORD)dwLen);
}

//������
void CComManager::SocketError(int nErrorCode)
{
}

int CComManager::GetConnectStatus()
{
	return m_pSocket->GetUdpStatus();
}