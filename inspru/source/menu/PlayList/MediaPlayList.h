// SysConfig.h: interface for the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MEDIS_PLAYLIST_H_
#define _MEDIS_PLAYLIST_H_

#include "CDirsRef.h"

enum enMediaType
{
	ID_Video = 0 ,				//多媒体类型，目前支持avi, wmv, mpg
	ID_Music,					//音乐类型,目前支持mp3, wma
	ID_Picture					//图片类型，目前支持Bmp、Jpg、Png
};

class CMediaPlayList
{
public:
	virtual ~CMediaPlayList();
	CMediaPlayList();

	//初始化，首先调用.SerachPath搜索的根路径，IsRecursion 是否递归查找
	//type媒体类型
	void InitMediaPlayList( CString SerachPath, enMediaType type , UINT IsRecursion = 1);

	//查找符合类型的文件数量
	UINT GetListCount();

	//读取一条文件名
	CString ReadOneList( UINT Pos );

private:
	UINT m_iFileCnt;
	CStringList m_vFilePath;

	void SearchVideo( CString SerachPath, UINT IsRecursion );
	void SearchMusic( CString SerachPath, UINT IsRecursion );
	void SearchPicture( CString SerachPath, UINT IsRecursion );
};

#endif 
