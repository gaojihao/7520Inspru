/*******************************************************************/

// CE�������ļ��࣬���Բ��޸��κδ�����ֲ��Win32ƽ̨��
// ֧���ļ��Ķ�д������Ŀǰ֧�ֵ�������string,int��Float
//	�Բ�ѯ����һЩ�Ż���ʩ
// �ļ���ʽ��PC��INI��ʽ��ͬ,�����ı��ֶ��༭
// ���ʹ��ֻ��ģʽ��ֻ��Ҫ�ڵ�һ��ʹ�õ���Load,�Ժ����Get�Ϳ���
// �����дģʽ,��Ҫ�ڵ�һ��ʹ�õ���Load,�Ժ����Set�Ϳ���


//					¬��

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

	//�����������ļ���
	BOOL Load(const CString csFileName, BOOL IsCreate = FALSE);

	//���ͷ������ļ���
	void Release();

	//��ȡ�ַ�����
	BOOL GetString(const CString& Section, const CString& Item, CString& Value);

	//��ȡint��
	BOOL GetInt(const CString& Section, const CString& Item, int& Value);

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

#endif // !defined(AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_)
