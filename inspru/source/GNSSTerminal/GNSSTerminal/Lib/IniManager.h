#pragma once

#include <afxwin.h>

#define INIMANAGER_API __declspec(dllexport)

class INIMANAGER_API CIniFile
{
public:

	//�����������ļ���
	BOOL Load(const CString csFileName, BOOL IsCreate = FALSE);

	//���ͷ������ļ���
	void Release();

	//��ȡ�ַ�����
	BOOL GetString(const CString& Section, const CString& Item, CString& Value);
	BOOL GetString(const CString& Section, const CString& Item, LPSTR lpStrValue, WORD wStrLen);

	//��ȡint��
	BOOL GetInt(const CString& Section, const CString& Item, int& Value);
	BOOL GetInt(const CString& Section, const CString& Item, DWORD& Value);
	BOOL GetInt(const CString& Section, const CString& Item, WORD& Value);

	//��ȡfloat��
	BOOL GetFloat(const CString& Section, const CString& Item, float& Value);

	//��ȡbool��
	BOOL GetBool(const CString& Section, const CString& Item, bool& Value);

	//������bool��
	BOOL SetBool(CString Section, CString Item, bool Value);

	//�������ַ�����
	BOOL SetString(CString Section, CString Item, CString Value);

	//������int���͡�
	BOOL SetInt(CString Section, CString Item, int Value);

	//������float���͡�
	BOOL SetFloat(CString Section, CString Item, float Value);

	//���޸����� д���ļ���
	BOOL Flush();

	CIniFile();
	virtual ~CIniFile();

protected:

	//���Ƿ��ļ��Ѿ��޸Ĺ���
	BOOL m_bWrite;

	//���ж��ļ��Ƿ��Ѿ��򿪡�
	BOOL IsOpen();

	//���ر������ļ���
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
