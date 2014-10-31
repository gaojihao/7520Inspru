/*******************************************************************/

// CE下配置文件类，可以不修改任何代码移植至Win32平台下
// 支持文件的读写操作，目前支持的类型有string,int，Float
//	对查询做过一些优化措施
// 文件格式与PC下INI格式相同,可以文本手动编辑
// 如果使用只读模式，只需要在第一次使用调用Load,以后调用Get就可以
// 如果是写模式,需要在第一次使用调用Load,以后调用Set就可以


//					卢海

//				2006 - 01 -19

/*******************************************************************/


#if !defined(AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_)
#define AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_

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
	
protected:

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

#endif // !defined(AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_)
