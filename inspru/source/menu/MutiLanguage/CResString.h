/////////////////////////////////////////////////////////////////////
//		CString�̳���
//		ʵ����LoadString���ַ��������ļ���
//		����CString���ܶ���ʵ��

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

	//���ø�·��
	static void SetLanguageRootPath( const CString& csPath);

	//���������ļ���
	static CString g_csLanguageIni; 

	//���ַ������ñ���ַ���
	virtual BOOL LoadString( const CString NameID );

	//�����ַ���
	virtual BOOL SetString(const CString NameID, const CString Value);

	//����Ҫ�����ַ�������
	static BOOL SetLanguage( enLanguage);

	//ȡ��ǰ��������
	static enLanguage GetCurrencyLanguage();
	
	//�̳���CResStringת����CString
	CString CResStringToCString();

	//�ͷ�����
	static void ReleaseLanguage();

	//���������ļ�
	static CIniFile m_IniFile;

	//��ǰ����
	static enLanguage g_Language;

	//�����ļ���·��
	static CString m_csLanguageRootPath;

	//�����������ļ����·��
	static CString g_csLanguageFileName;

	static CArray<enLanguage,enLanguage> g_LanguageList;

	
	//ȡ���ö�����������Ŀ
	static UINT GetLanguageCount();

	static enLanguage GetValidLanguage(UINT pos);

	//���� + ������
	friend CResString AFXAPI operator+ ( CResString& csFirest,  CResString& csSecond);
	friend CResString AFXAPI operator+ ( CString& csFirest,  CResString& csSecond);
	friend CResString AFXAPI operator+ ( CResString& csFirest, CString& csSecond);
};


#endif // !defined(AFX_CRESSTRING_H__4F08583E_01BD_4D34_ADE6_172E3C334C0B__INCLUDED_)
