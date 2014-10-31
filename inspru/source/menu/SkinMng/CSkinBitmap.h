// CSkinBitmap.h: interface for the CSkinBitMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSKINBITMAP_H__599641EE_298F_4A6E_8515_087D62446FFC__INCLUDED_)
#define AFX_CSKINBITMAP_H__599641EE_298F_4A6E_8515_087D62446FFC__INCLUDED_

#include "SkinSetType.h"
#include "CIniFile.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ST_RGB
{
	UINT red;
	UINT green;
	UINT blue;
};

class CSkinBitMap : public CBitmap
{
public:

	CSkinBitMap();
	virtual ~CSkinBitMap();

	//����Ƥ����·��
	static void SetSkinRootPath(const CString& csSkinPath); 

	//����ͼƬ
	virtual BOOL LoadBitmap( const CString nNameID);

	static HBITMAP GetBmpHandle(const CString nNameID);

	//ȡ��ɫ
	static ST_RGB GetRGB(const CString nNameID);

	//����Ƥ������
	static BOOL SetSkinType(enSkinType SkinType);

	//�ͷ�Ƥ��
	static void ReleaseSkin();

	//ȡ��ǰƤ������
	static enSkinType GetCurSkinType();

	//Ƥ�������ļ�
 	static CIniFile m_SkinIniFile;

	//��ǰƤ��
	static enSkinType g_CurSkinType;

	//Ƥ���ļ���·��
	static CString m_SkinPath;

	//��ȡjpegͼƬ������ת��Ϊbmp
	static HBITMAP LoadImageFromFile(TCHAR * pFileImage);
	
	//�����Ǹ���Ƥ���ļ�·��
	static CString g_csSkin1;
	static CString g_csSkin2;
	static CString g_csSkin3;
	static CString g_csSkin4;
	static CString g_csSkin5;
};

#endif // !defined(AFX_CSKINBITMAP_H__599641EE_298F_4A6E_8515_087D62446FFC__INCLUDED_)
