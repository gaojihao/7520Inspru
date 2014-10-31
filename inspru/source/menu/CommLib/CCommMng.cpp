// CommMng.cpp: implementation of the CommMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CCommMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCommMng CCommMng::_instance;

CCommMng& CCommMng::Instance()
{
	return _instance;
}
  
CCommMng::CCommMng()
{
	m_pProtocol = NULL;
	m_bProtocolState = TRUE;
}

CCommMng::~CCommMng()
{
	if(m_pProtocol)
	{
		delete m_pProtocol;
		m_pProtocol = NULL;
	}
}

BOOL CCommMng::RegeditProtocol(PROTOCOL_TYPE enProType)
{
	BOOL bRts = FALSE;
	if(m_bProtocolState)
	{
		switch(enProType)
		{
		case en_E920Event_PROTOCOL:
			m_pProtocol = new CE920EventProtocol;
			bRts = TRUE;
			break;
		default:
			break;
		}
	}
	m_bProtocolState = FALSE;

	return bRts;
}

BOOL CCommMng::UnRegeditProtocol()
{
	BOOL bRts = FALSE;

	if(m_pProtocol != NULL)
	{
		delete m_pProtocol;
		m_pProtocol = NULL;
		m_bProtocolState = FALSE;
		bRts = TRUE;
	}

	return bRts;
}

BOOL CCommMng::SendData(CE920Event* lpInData)
{
	CE920Event* lpEventData = (CE920Event*)lpInData;

	char OutBuf[E920BufSize];
	memset(OutBuf,0,E920BufSize);
	UINT OutBufLen;
	m_pProtocol->Encode(lpInData,OutBuf,OutBufLen);
	Write(OutBuf,OutBufLen);

	return TRUE;
}

BOOL CCommMng::RecvData(CE920Event*& lpOutData)
{
	BOOL bRts = FALSE;;
	char cReadBuf[E920BufSize];
	memset(cReadBuf,0,E920BufSize);
	DWORD dwReadLen = Read(cReadBuf,E920BufSize);

	if(dwReadLen > 0)
	{
		bRts = m_pProtocol->Decode(cReadBuf,dwReadLen,lpOutData);
	}
	else
	{
		bRts = FALSE;
	}
	return bRts;
}

void CCommMng::CloseDevice()
{
	Close();
}

BOOL CCommMng::OpenDevice(DWORD dwPort, DWORD dwBaudRate)
{
	return Open(dwPort,dwBaudRate);
}

void CCommMng::SetRecvWnd(HWND hWnd)
{
	SetWnd(hWnd);
}

BOOL CCommMng::BeginRecvThread()
{
	return BeginThread();
}

BOOL CCommMng::SuspendRecvThread()
{
	return SuspendThread();
}

BOOL CCommMng::ResumeRecvThread()
{
	return ResumeThread();
}

BOOL CCommMng::EndRecvThread()
{
	return EndThread();
}

BOOL CCommMng::IsRecvThreadRuning()
{
	return IsThreadRunning();
}