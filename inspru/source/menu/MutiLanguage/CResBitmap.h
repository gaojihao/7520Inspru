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

	//���ø�·��
	static void SetLanguageRootPath( const CString& csPath);

	//����ͼƬ
	virtual BOOL LoadBitmap( const CString nNameID);

	//����Ҫ�����ַ�������
	static BOOL SetLanguage(enLanguage LanguageId);

	//�ͷ�����
	static void ReleaseLanguage();

	//ȡ��ǰ��������
	static enLanguage GetCurrencyLanguage();

	//���������ļ�
	static CIniFile m_BmpIniFile;

	//��ǰ����
	static enLanguage g_Language;

	//�����ļ���·��
	static CString m_csLanguageRootPath;

	//�����Ǹ��������ļ����·��
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
