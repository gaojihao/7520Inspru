// CResBitmap.h: interface for the CResBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRESBITMAP_H__3C7325E8_A138_40F0_97FE_8A89BEB4D108__INCLUDED_)
#define AFX_CRESBITMAP_H__3C7325E8_A138_40F0_97FE_8A89BEB4D108__INCLUDED_

#include "LanguageSetType.h"
#include "CIniFile.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResBitmap  : public CBitmap
{
public:
	CResBitmap();
	virtual ~CResBitmap();

	//设置根路径
	static void SetLanguageRootPath( const CString& csPath);

	//载入图片
	virtual BOOL LoadBitmap( const CString nNameID);

	//设置要读的字符串语言
	static BOOL SetLanguage(enLanguage LanguageId);

	//释放语言
	static void ReleaseLanguage();

	//取当前语言类型
	static enLanguage GetCurrencyLanguage();

	//语言配置文件
	static CIniFile m_BmpIniFile;

	//当前语言
	static enLanguage g_Language;

	//语言文件根路径
	static CString m_csLanguageRootPath;

	//以下是各种语言文件相对路径
	static CString g_csChinese;
	static CString g_csEnglish;
	static CString g_csGermany;
	static CString g_csFrance;
	static CString g_csJapan;
	static CString g_csItaly;
	static CString g_csRussia;
	static CString g_csIndia;
};

#endif // !defined(AFX_CRESBITMAP_H__3C7325E8_A138_40F0_97FE_8A89BEB4D108__INCLUDED_)
