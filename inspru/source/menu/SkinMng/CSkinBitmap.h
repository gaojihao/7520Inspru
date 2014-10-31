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

	//设置皮肤跟路径
	static void SetSkinRootPath(const CString& csSkinPath); 

	//载入图片
	virtual BOOL LoadBitmap( const CString nNameID);

	static HBITMAP GetBmpHandle(const CString nNameID);

	//取颜色
	static ST_RGB GetRGB(const CString nNameID);

	//设置皮肤类型
	static BOOL SetSkinType(enSkinType SkinType);

	//释放皮肤
	static void ReleaseSkin();

	//取当前皮肤类型
	static enSkinType GetCurSkinType();

	//皮肤配置文件
 	static CIniFile m_SkinIniFile;

	//当前皮肤
	static enSkinType g_CurSkinType;

	//皮肤文件根路径
	static CString m_SkinPath;

	//获取jpeg图片，将其转换为bmp
	static HBITMAP LoadImageFromFile(TCHAR * pFileImage);
	
	//以下是各种皮肤文件路径
	static CString g_csSkin1;
	static CString g_csSkin2;
	static CString g_csSkin3;
	static CString g_csSkin4;
	static CString g_csSkin5;
};

#endif // !defined(AFX_CSKINBITMAP_H__599641EE_298F_4A6E_8515_087D62446FFC__INCLUDED_)
