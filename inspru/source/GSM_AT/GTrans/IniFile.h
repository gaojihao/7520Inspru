// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__86A12AD8_1AB1_480C_B583_5496B9C824DD__INCLUDED_)
#define AFX_INIFILE_H__86A12AD8_1AB1_480C_B583_5496B9C824DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile  
{
public:
	
	//【载入配置文件】
	BOOL Load(const CString csFileName, BOOL IsCreate = FALSE);
	
	//【释放配置文件】
	void Release();
	
	//【取字符串】
	BOOL GetString(const CString& Section, const CString& Item, CString& Value);
	
	//【取int】
	BOOL GetInt(const CString& Section, const CString& Item, int& Value);
	
	//【取float】
	BOOL GetFloat(const CString& Section, const CString& Item, float& Value);
	
	//【取bool】
	BOOL GetBool(const CString& Section, const CString& Item, bool& Value);
	
	//【设置bool】
	BOOL SetBool(CString Section, CString Item, bool Value);
	
	//【设置字符串】
	BOOL SetString(CString Section, CString Item, CString Value);
	
	//【设置int类型】
	BOOL SetInt(CString Section, CString Item, int Value);
	
	//【设置float类型】
	BOOL SetFloat(CString Section, CString Item, float Value);
	
	//【修改数据 写入文件】
	BOOL Flush();
	
	CIniFile();
	virtual ~CIniFile();
	
private:
	
	//【是否文件已经修改过】
	BOOL m_bWrite;
	
	//【判断文件是否已经打开】
	BOOL IsOpen();
	
	//【关闭配置文件】
	void Close();
	
	
	
	BOOL ReadOneString(TCHAR* pIn, UINT iInLen, TCHAR*& pOut, UINT &iOutLen);
	
	BOOL GetFileString(const CString& Section,const CString& Item, CString& Value);
	BOOL ReadIniFile();
	BOOL WriteIniFile();
	void WriteFileString(CString Section, CString Item, CString Value);
	//ini file
	CString m_sFileName;
	CFile m_IniFile;
	BOOL  m_bFileExist;
	CStringArray m_FileContainer;
	
	UINT m_iPos;
	
};

#endif // !defined(AFX_INIFILE_H__86A12AD8_1AB1_480C_B583_5496B9C824DD__INCLUDED_)
