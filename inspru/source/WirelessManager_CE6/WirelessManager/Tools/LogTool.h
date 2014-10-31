#pragma once

//��־Ŀ¼
#define PATH_LOG_FILE					_T("\\Flashdrv Storage\\LogoFile\\")

class CLogTool
{
public:
	//�����־����
	typedef enum
	{
		SCRN				= 0x0001,		//�����־��VS Output����
		FILE				= 0x0010,		//�����־����־�ļ�
		PROC				= 0x0100,		//�����־�����Խ���
	} EN_LOG_TYPE;

private:
	static CLogTool*	m_pInstance;				//Singletonģʽʵ��ָ��

public:
	CLogTool(void);
	~CLogTool(void);

	//Singletonģʽʵ������
	static CLogTool* Instance();

	//д��־
	BOOL	WriteLogFile(LPCWSTR lpszFormat, ...);
	BOOL	WriteLogFile(LPCSTR lpszFormat, ...);
	BOOL	WriteLogFile(LPCTSTR lpInfo, LPBYTE lpBuffer, WORD wBufferLength);

private:
	BOOL	Output(PTCHAR pszLog, DWORD dwType);
};
