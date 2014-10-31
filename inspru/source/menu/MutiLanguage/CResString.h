/////////////////////////////////////////////////////////////////////
//		CString继承类
//		实现了LoadString从字符串配置文件读
//		其余CString功能都已实现

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRESSTRING_H__4F08583E_01BD_4D34_ADE6_172E3C334C0B__INCLUDED_)
#define AFX_CRESSTRING_H__4F08583E_01BD_4D34_ADE6_172E3C334C0B__INCLUDED_

#include "Afxtempl.h"
#include "CIniFile.h"
#include "LanguageSetType.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResString  : public CString
{
public:
	CResString();
	CResString( const CResString& stringSrc );
	CResString( TCHAR ch, int nRepeat = 1 );
	CResString( LPCTSTR lpch, int nLength );
	CResString( const unsigned char* psz );
	CResString( LPCWSTR lpsz );
	CResString( LPCSTR lpsz );

	virtual ~CResString();

	//设置根路径
	static void SetLanguageRootPath( const CString& csPath);

	//语言配置文件名
	static CString g_csLanguageIni; 

	//从字符串配置表读字符串
	virtual BOOL LoadString( const CString NameID );

	//设置字符串
	virtual BOOL SetString(const CString NameID, const CString Value);

	//设置要读的字符串语言
	static BOOL SetLanguage( enLanguage);

	//取当前语言类型
	static enLanguage GetCurrencyLanguage();
	
	//继承类CResString转化成CString
	CString CResStringToCString();

	//释放语言
	static void ReleaseLanguage();

	//语言配置文件
	static CIniFile m_IniFile;

	//当前语言
	static enLanguage g_Language;

	//语言文件根路径
	static CString m_csLanguageRootPath;

	//以下是语言文件相对路径
	static CString g_csLanguageFileName;

	static CArray<enLanguage,enLanguage> g_LanguageList;

	
	//取可用多语言类型数目
	static UINT GetLanguageCount();

	static enLanguage GetValidLanguage(UINT pos);

	//重载 + 操作符
	friend CResString AFXAPI operator+ ( CResString& csFirest,  CResString& csSecond);
	friend CResString AFXAPI operator+ ( CString& csFirest,  CResString& csSecond);
	friend CResString AFXAPI operator+ ( CResString& csFirest, CString& csSecond);
};


#endif // !defined(AFX_CRESSTRING_H__4F08583E_01BD_4D34_ADE6_172E3C334C0B__INCLUDED_)
