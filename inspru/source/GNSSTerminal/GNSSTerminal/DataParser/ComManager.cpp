#include "StdAfx.h"
#include "ComManager.h"
#include "../lib/IniManager.h"
#include "../MsgProc/MsgProc.h"
#include "../DataParser/DataParser.h"
#include "../GPSData/GPSData.h"
#include "../Tools/LogTool.h"

//初始化静态成员变量
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
* 函数名: Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: singleton
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
* 函数名:	InitCom
* 描  述:	初始化底层通讯，根据一体机或调度屏选择
* 入  参:
* 出  参: 
* 返  回:	BOOL
* 备  注: 
************************************************************************/
BOOL CComManager::InitCom()
{
	//1.初始化Socket
	m_pSocket = new CSocketManager(SocketReceive, SocketError, IPPROTO_TCP);
	if (m_pSocket == NULL)
	{
		return 0;
	}
	return TRUE;
}

/************************************************************************
* 函数名:	SendData
* 描  述:	发送数据
* 入  参:
* 出  参: 
* 返  回:	int:	-1	发送失败	>0 发送成功
* 备  注: 
************************************************************************/
int CComManager::SendData(PBYTE pBuffer, DWORD dwLen)
{

	//在通话时不要发送数据
	if(m_pSocket == NULL)
		return -1;

	CLogTool::Instance()->WriteLogFile(_T("Send:"), pBuffer, (WORD)dwLen);

	return m_pSocket->SendData((const char*)pBuffer, dwLen);
}

/************************************************************************
* 函数名:	OpenSocketConnect
* 描  述:	打开Socket连接
* 入  参:
* 出  参: 
* 返  回:	int:	-1	发送失败	>0 发送成功
* 备  注: 
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

//Socket数据处理
int CComManager::SocketReceive(BYTE *pBuffer, DWORD dwLen)
{
	return CDataParser::Instance()->DataParser(pBuffer, (WORD)dwLen);
}

//错误处理
void CComManager::SocketError(int nErrorCode)
{
}

int CComManager::GetConnectStatus()
{
	return m_pSocket->GetUdpStatus();
}