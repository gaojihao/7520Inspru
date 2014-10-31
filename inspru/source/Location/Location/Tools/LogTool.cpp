#include "StdAfx.h"
#include "LogTool.h"

//��ʼ����̬��Ա����
CLogTool*	CLogTool::m_pInstance = NULL;

CLogTool::CLogTool(void)
{
}

CLogTool::~CLogTool(void)
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/************************************************************************
* ������:	Instance
* ��  ��: 
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע:	singleton
************************************************************************/
CLogTool* CLogTool::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CLogTool;
	}
	return m_pInstance;
}

/************************************************************************
* ������:	WriteLogFile
* ��  ��:	д��־
* ��  ��:	1. lpszFormat:	�䳤�����б��ο�printf����
* ��  ��: 
* ��  ��:	
* ��  ע:	UNICODE�汾
************************************************************************/
BOOL CLogTool::WriteLogFile(LPCWSTR lpszFormat, ...)
{
	//��ӡbuffer��ʽ
	va_list args;
	va_start(args, lpszFormat);
	TCHAR szBuffer[4096] = {0};
	_vstprintf(szBuffer, lpszFormat, args);
	va_end(args);

	//����ʱ��ͻ���
	SYSTEMTIME systime;
	TCHAR szLog[4096] = {0};
	memset(&systime, NULL, sizeof(SYSTEMTIME));
	GetLocalTime(&systime);
	_stprintf(szLog, _T("[%02d:%02d:%02d]%s\r\n"), 
		systime.wHour, systime.wMinute, systime.wSecond, szBuffer);

	Output(szLog, SCRN/*|FILE*/);

	return TRUE;
}

/************************************************************************
* ������:	WriteLogFile
* ��  ��:	д��־
* ��  ��:	1. lpszFormat:	�䳤�����б��ο�printf����
* ��  ��: 
* ��  ��:	
* ��  ע:	MBCS�汾
************************************************************************/
BOOL CLogTool::WriteLogFile(LPCSTR lpszFormat, ...)
{
	//��ӡbuffer��ʽ
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[4096] = {0};
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);

	TCHAR szwBuffer[1024] = {0};
	MultiByteToWideChar (CP_ACP, 0, szBuffer, -1, szwBuffer, strlen(szBuffer)); 

	//����ʱ��ͻ���
	SYSTEMTIME systime;
	TCHAR szLog[4096] = {0};
	memset(&systime, NULL, sizeof(SYSTEMTIME));
	GetLocalTime(&systime);
	_stprintf(szLog, _T("[%02d:%02d:%02d]%s\r\n"), 
		systime.wHour, systime.wMinute, systime.wSecond, szwBuffer);

	Output(szLog, SCRN/*|FILE*/);

	return TRUE;
}

/************************************************************************
* ������:	WriteLogFile
* ��  ��:	д��־
* ��  ��:	1. lpBuffer:		���ջ���buffer
			2. wBufferLength:	buffer����
* ��  ��: 
* ��  ��:	
* ��  ע:	
************************************************************************/
BOOL CLogTool::WriteLogFile(LPCTSTR lpInfo, LPBYTE lpBuffer, WORD wBufferLength)
{
	if(NULL == lpBuffer || 0 == wBufferLength)
		return FALSE;

	//����ʱ��ͻ���
	SYSTEMTIME systime;
	memset(&systime, NULL, sizeof(SYSTEMTIME));
	GetLocalTime(&systime);
	
	//��ӡʱ��
	WORD wLogLen = wBufferLength*4+256;
	PTCHAR pszLog = new TCHAR[wLogLen];
	memset(pszLog, NULL, wLogLen*2);
	_stprintf(pszLog, _T("[%02d:%02d:%02d]%s\r\n"),
		systime.wHour, systime.wMinute, systime.wSecond, lpInfo);

	WORD wOffset = _tcslen(pszLog);

	//ƴ����־����
	for(int i=0; i<wBufferLength; i++)
	{
		_stprintf(&pszLog[wOffset], _T("%02X "), lpBuffer[i]);
		wOffset += 3;
		if( (i+1)%20 == 0 && (i+1) != wBufferLength )
		{
			_stprintf(&pszLog[wOffset], _T("\r\n"));
			wOffset += 2;
		}
	}
	//buffer�����Ҫһ������
	_stprintf(&pszLog[wOffset], _T("\r\n"));
	wOffset += 2;

	Output(pszLog, SCRN|FILE);

	delete[] pszLog;

	return TRUE;
}

/************************************************************************
* ������:	Output
* ��  ��:	д��־
* ��  ��:	1. pszLog:		���ջ���buffer
			2. dwType:		������ͣ�����Output���Դ��ڣ���־�ļ������Խ���
* ��  ��: 
* ��  ��:	
* ��  ע:	
************************************************************************/
BOOL CLogTool::Output(PTCHAR pszLog, DWORD dwType)
{
	//�����debug����
	if(dwType & SCRN)
	{
		OutputDebugString(pszLog);
	}

	//�������־�ļ�
	if(dwType & FILE)
	{
		TCHAR szFilePath[MAX_PATH] = {0};

		SYSTEMTIME systime;
		GetLocalTime(&systime);

		_stprintf(szFilePath, _T("%slog_%04d%02d%02d.txt"), 
			PATH_LOG_FILE,
			systime.wYear,
			systime.wMonth,
			systime.wDay);

		//д���ļ�
		HANDLE hFile = CreateFile( szFilePath,  
			GENERIC_WRITE,  FILE_SHARE_READ,  NULL,  OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			return FALSE;

		SetFilePointer(hFile, 0, NULL, FILE_END);
		//д���ݵ������ļ�
		DWORD dwSizeWrite = 0;
		WriteFile( hFile, pszLog, _tcslen(pszLog)*2, &dwSizeWrite, 0);
		CloseHandle(hFile);
	}

	//�����־�����Խ���
	if(dwType & PROC)
	{
		HWND hWnd = ::FindWindow(NULL, _T("DEBUG_WINDOW"));
		if(hWnd != NULL)
		{
			COPYDATASTRUCT copy_data;
			copy_data.dwData	= 0;
			copy_data.cbData	= _tcslen(pszLog)*2;
			copy_data.lpData	= pszLog;
			::SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&copy_data);
		}
	}
	return TRUE;
}