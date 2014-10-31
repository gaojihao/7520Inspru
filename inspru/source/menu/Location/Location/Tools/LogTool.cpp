#include "StdAfx.h"
#include "LogTool.h"

//初始化静态成员变量
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
* 函数名:	Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注:	singleton
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
* 函数名:	WriteLogFile
* 描  述:	写日志
* 入  参:	1. lpszFormat:	变长参数列表，参考printf参数
* 出  参: 
* 返  回:	
* 备  注:	UNICODE版本
************************************************************************/
BOOL CLogTool::WriteLogFile(LPCWSTR lpszFormat, ...)
{
	//打印buffer格式
	va_list args;
	va_start(args, lpszFormat);
	TCHAR szBuffer[4096] = {0};
	_vstprintf(szBuffer, lpszFormat, args);
	va_end(args);

	//加上时间和换行
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
* 函数名:	WriteLogFile
* 描  述:	写日志
* 入  参:	1. lpszFormat:	变长参数列表，参考printf参数
* 出  参: 
* 返  回:	
* 备  注:	MBCS版本
************************************************************************/
BOOL CLogTool::WriteLogFile(LPCSTR lpszFormat, ...)
{
	//打印buffer格式
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[4096] = {0};
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);

	TCHAR szwBuffer[1024] = {0};
	MultiByteToWideChar (CP_ACP, 0, szBuffer, -1, szwBuffer, strlen(szBuffer)); 

	//加上时间和换行
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
* 函数名:	WriteLogFile
* 描  述:	写日志
* 入  参:	1. lpBuffer:		接收或发送buffer
			2. wBufferLength:	buffer长度
* 出  参: 
* 返  回:	
* 备  注:	
************************************************************************/
BOOL CLogTool::WriteLogFile(LPCTSTR lpInfo, LPBYTE lpBuffer, WORD wBufferLength)
{
	if(NULL == lpBuffer || 0 == wBufferLength)
		return FALSE;

	//加上时间和换行
	SYSTEMTIME systime;
	memset(&systime, NULL, sizeof(SYSTEMTIME));
	GetLocalTime(&systime);
	
	//打印时间
	WORD wLogLen = wBufferLength*4+256;
	PTCHAR pszLog = new TCHAR[wLogLen];
	memset(pszLog, NULL, wLogLen*2);
	_stprintf(pszLog, _T("[%02d:%02d:%02d]%s\r\n"),
		systime.wHour, systime.wMinute, systime.wSecond, lpInfo);

	WORD wOffset = _tcslen(pszLog);

	//拼凑日志内容
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
	//buffer最后需要一个换行
	_stprintf(&pszLog[wOffset], _T("\r\n"));
	wOffset += 2;

	Output(pszLog, SCRN|FILE);

	delete[] pszLog;

	return TRUE;
}

/************************************************************************
* 函数名:	Output
* 描  述:	写日志
* 入  参:	1. pszLog:		接收或发送buffer
			2. dwType:		输出类型，包括Output调试窗口，日志文件，调试进程
* 出  参: 
* 返  回:	
* 备  注:	
************************************************************************/
BOOL CLogTool::Output(PTCHAR pszLog, DWORD dwType)
{
	//输出到debug窗口
	if(dwType & SCRN)
	{
		OutputDebugString(pszLog);
	}

	//输出到日志文件
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

		//写入文件
		HANDLE hFile = CreateFile( szFilePath,  
			GENERIC_WRITE,  FILE_SHARE_READ,  NULL,  OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			return FALSE;

		SetFilePointer(hFile, 0, NULL, FILE_END);
		//写数据到本地文件
		DWORD dwSizeWrite = 0;
		WriteFile( hFile, pszLog, _tcslen(pszLog)*2, &dwSizeWrite, 0);
		CloseHandle(hFile);
	}

	//输出日志到调试进程
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