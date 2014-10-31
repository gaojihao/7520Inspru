#pragma once

//日志目录
#define PATH_LOG_FILE					_T("\\Flashdrv Storage\\LogoFile\\")

class CLogTool
{
public:
	//输出日志类型
	typedef enum
	{
		SCRN				= 0x0001,		//输出日志到VS Output窗口
		FILE				= 0x0010,		//输出日志到日志文件
		PROC				= 0x0100,		//输出日志到调试进程
	} EN_LOG_TYPE;

private:
	static CLogTool*	m_pInstance;				//Singleton模式实例指针

public:
	CLogTool(void);
	~CLogTool(void);

	//Singleton模式实例函数
	static CLogTool* Instance();

	//写日志
	BOOL	WriteLogFile(LPCWSTR lpszFormat, ...);
	BOOL	WriteLogFile(LPCSTR lpszFormat, ...);
	BOOL	WriteLogFile(LPCTSTR lpInfo, LPBYTE lpBuffer, WORD wBufferLength);

private:
	BOOL	Output(PTCHAR pszLog, DWORD dwType);
};
