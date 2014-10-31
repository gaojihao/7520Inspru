// SysConfig.cpp: implementation of the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigBase.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigBase::CConfigBase()
{

}

CConfigBase::~CConfigBase()
{

}


void CConfigBase::SetConfigFileName(CString CnfName)
{
	m_strFileName = CnfName;
}

void CConfigBase::SetConfigFilePath(CString CnfPath)
{
	m_strFilePath = CnfPath;
}

BOOL CConfigBase::Load()
{
	CString Name = m_strFilePath + _T("\\") + m_strFileName;
	return m_ConfigFile.Load(Name);
}


void CConfigBase::ResetDefault()
{

	CString ConfigCompletePath;
	ConfigCompletePath = m_strFilePath + _T("\\") + m_strFileName;

	DeleteFile(ConfigCompletePath);

	CFileException fe;
	CFile ConfigFile;
	CFile defaultfile;

	TRY
	{

		SetFileAttributes(ConfigCompletePath, FILE_ATTRIBUTE_NORMAL);
		ConfigFile.Open(ConfigCompletePath, 
			CFile::modeCreate | CFile::modeReadWrite,&fe);

		CString defaultCompletePath;
		defaultCompletePath = m_strFilePath + _T("\\Default.ini");

		SetFileAttributes(defaultCompletePath, FILE_ATTRIBUTE_NORMAL);
		defaultfile.Open(defaultCompletePath, 
			CFile::modeRead,&fe);
		
		DWORD len = defaultfile.GetLength();

		char* pBuf = new char[len];
		defaultfile.Read(pBuf,len);

		ConfigFile.Write(pBuf,len);

		if(pBuf != NULL)
		{
			delete[] pBuf;
			pBuf = NULL;
		}

		ConfigFile.Close();
		defaultfile.Close();	

		Load();
	}
	CATCH(CException,ep)
	{	
		ConfigFile.Close();
		defaultfile.Close();
		ResetDefault();
	}
	END_CATCH


}

void CConfigBase::SetLanguage(enLanguage enLanguageType)
{
	m_ConfigFile.SetInt(_T("CommonSet"),_T("Language"),int(enLanguageType));
	m_ConfigFile.Flush();
	CUiMng::Instance().SetLanguage(enLanguageType);
}

enLanguage CConfigBase::GetLanguage()
{
	int iValue = 10;
	m_ConfigFile.GetInt(_T("CommonSet"),_T("Language"),iValue);
	return enLanguage(iValue);
}

void CConfigBase::SetSkinType(enSkinType enType)
{
	m_ConfigFile.SetInt(_T("CommonSet"),_T("Skin"),(int)enType);
	m_ConfigFile.Flush();
	CUiMng::Instance().SetSkinType(enType);
}

enSkinType CConfigBase::GetSkinType()
{
	int iValue = 10;
	m_ConfigFile.GetInt(_T("CommonSet"),_T("Skin"),iValue);
	return enSkinType(iValue);
}

CString CConfigBase::GetLanguagePath()
{
	CString str;
	m_ConfigFile.GetString(_T("CommonSet"),_T("LanguagePath"),str);
	return str;
}

CString CConfigBase::GetSkinPath()
{
	CString str;
	m_ConfigFile.GetString(_T("CommonSet"),_T("SkinPath"),str);
	return str;
}


BOOL CConfigBase::Save()
{
	return m_ConfigFile.Flush();
}