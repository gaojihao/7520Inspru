// SysConfig.h: interface for the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CCONFIGBASE_H_
#define _CCONFIGBASE_H_

#include "CIniFile.h"
#include "CUiMng.h"

class CConfigBase
{
public:
	virtual ~CConfigBase();
	CConfigBase();

	void SetConfigFileName(CString CnfName);
	void SetConfigFilePath(CString CnfPath);

	BOOL Load();
	BOOL Save();

	//恢复默认
	virtual void ResetDefault();

	void SetLanguage(enLanguage);
	enLanguage GetLanguage();

	void SetSkinType(enSkinType);
	enSkinType GetSkinType();

	CString GetLanguagePath();
	CString GetSkinPath();

	
protected:
	CIniFile    m_ConfigFile;
	CString		m_strFileName;	//配置文件名称
	CString		m_strFilePath;	//配置文件路径

};

#endif 
