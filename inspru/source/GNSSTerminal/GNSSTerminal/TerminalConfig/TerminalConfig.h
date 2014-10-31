#pragma once

#include "../DataParser/DataType.h"
#include "../Lib/IniManager.h"

class CTerminalConfig
{
private:
	static CTerminalConfig*	m_pInstance;				//Singleton模式实例指针
	CString					m_strConfigFilePath;		//config文件路径
public:
	TERMINAL_PARAM			m_TerminalParam;			//终端参数

public:
	CTerminalConfig(void);
	~CTerminalConfig(void);

	//Singleton模式实例函数
	static CTerminalConfig* Instance();

	//BOOL	SetConfigFilePath(CString );

	//加载config文件到内存中
	BOOL	LoadConfigFile(CString strPath);
	//将内存中的保存到文件
	BOOL	SaveConfigFile();
	BOOL	SaveMileage(DWORD dwMileage);
	//获取终端参数结构体
	const PTERMINAL_PARAM	GetTerminalParam();
};
