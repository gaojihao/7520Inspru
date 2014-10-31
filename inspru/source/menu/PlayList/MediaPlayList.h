// SysConfig.h: interface for the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MEDIS_PLAYLIST_H_
#define _MEDIS_PLAYLIST_H_

#include "CDirsRef.h"

enum enMediaType
{
	ID_Video = 0 ,				//��ý�����ͣ�Ŀǰ֧��avi, wmv, mpg
	ID_Music,					//��������,Ŀǰ֧��mp3, wma
	ID_Picture					//ͼƬ���ͣ�Ŀǰ֧��Bmp��Jpg��Png
};

class CMediaPlayList
{
public:
	virtual ~CMediaPlayList();
	CMediaPlayList();

	//��ʼ�������ȵ���.SerachPath�����ĸ�·����IsRecursion �Ƿ�ݹ����
	//typeý������
	void InitMediaPlayList( CString SerachPath, enMediaType type , UINT IsRecursion = 1);

	//���ҷ������͵��ļ�����
	UINT GetListCount();

	//��ȡһ���ļ���
	CString ReadOneList( UINT Pos );

private:
	UINT m_iFileCnt;
	CStringList m_vFilePath;

	void SearchVideo( CString SerachPath, UINT IsRecursion );
	void SearchMusic( CString SerachPath, UINT IsRecursion );
	void SearchPicture( CString SerachPath, UINT IsRecursion );
};

#endif 
