#pragma once

#include "../DataParser/DataType.h"
#include "../Lib/IniManager.h"

class CTerminalConfig
{
private:
	static CTerminalConfig*	m_pInstance;				//Singletonģʽʵ��ָ��
	CString					m_strConfigFilePath;		//config�ļ�·��
public:
	TERMINAL_PARAM			m_TerminalParam;			//�ն˲���

public:
	CTerminalConfig(void);
	~CTerminalConfig(void);

	//Singletonģʽʵ������
	static CTerminalConfig* Instance();

	//BOOL	SetConfigFilePath(CString );

	//����config�ļ����ڴ���
	BOOL	LoadConfigFile(CString strPath);
	//���ڴ��еı��浽�ļ�
	BOOL	SaveConfigFile();
	BOOL	SaveMileage(DWORD dwMileage);
	//��ȡ�ն˲����ṹ��
	const PTERMINAL_PARAM	GetTerminalParam();
};
