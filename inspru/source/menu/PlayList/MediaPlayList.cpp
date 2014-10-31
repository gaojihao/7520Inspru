// SysConfig.cpp: implementation of the CSysConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MediaPlayList.h"
#include "CDirsRef.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMediaPlayList::CMediaPlayList()
{
	if(!m_vFilePath.IsEmpty())
		m_vFilePath.RemoveAll();
	m_iFileCnt = 0;
}

CMediaPlayList::~CMediaPlayList()
{
	if(!m_vFilePath.IsEmpty())
		m_vFilePath.RemoveAll();
	m_iFileCnt = 0;
}

void CMediaPlayList::SearchVideo( CString SerachPath, UINT IsRecursion )
{
	CDirsRef AviDirs(SerachPath,IsRecursion,_T(".avi"));
	CDirsRef mpgDirs(SerachPath,IsRecursion,_T(".mpg"));
	CDirsRef WmvDirs(SerachPath,IsRecursion,_T(".wmv"));
	
	m_iFileCnt = AviDirs.GetFilesCnt() + mpgDirs.GetFilesCnt() + WmvDirs.GetFilesCnt();

	int i;
	CString FileName;
	for(i=0; i<AviDirs.GetFilesCnt(); i++)
	{
		FileName = AviDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
		 
	}

	for(i=0; i<mpgDirs.GetFilesCnt(); i++)
	{
		FileName = mpgDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
	}

	for(i=0; i<WmvDirs.GetFilesCnt(); i++)
	{
		FileName = WmvDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
	}
}

void CMediaPlayList::SearchMusic( CString SerachPath, UINT IsRecursion )
{
	CDirsRef Mp3Dirs(SerachPath,IsRecursion,_T(".mp3"));
	CDirsRef WmaDirs(SerachPath,IsRecursion,_T(".wma"));
	
	m_iFileCnt = Mp3Dirs.GetFilesCnt() + WmaDirs.GetFilesCnt() ;

	int i;
	CString FileName;
	for(i=0; i<Mp3Dirs.GetFilesCnt(); i++)
	{
		FileName = Mp3Dirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
		 
	}

	for(i=0; i<WmaDirs.GetFilesCnt(); i++)
	{
		FileName = WmaDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
	}

}

void CMediaPlayList::SearchPicture( CString SerachPath, UINT IsRecursion )
{
	CDirsRef BmpDirs(SerachPath,IsRecursion,_T(".bmp"));
	CDirsRef JpgDirs(SerachPath,IsRecursion,_T(".Jpg"));
	CDirsRef PngDirs(SerachPath,IsRecursion,_T(".Png"));
	
	m_iFileCnt = BmpDirs.GetFilesCnt() + JpgDirs.GetFilesCnt() + PngDirs.GetFilesCnt();

	int i;
	CString FileName;
	for(i=0; i<BmpDirs.GetFilesCnt(); i++)
	{
		FileName = BmpDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
		 
	}

	for(i=0; i<JpgDirs.GetFilesCnt(); i++)
	{
		FileName = JpgDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
	}

	for(i=0; i<PngDirs.GetFilesCnt(); i++)
	{
		FileName = PngDirs.ReadOneFileName(i);
		int pos = FileName.ReverseFind('\\') + 1;
		m_vFilePath.AddTail( FileName.Delete(0,pos) );
	}	
}

void CMediaPlayList::InitMediaPlayList( CString SerachPath, enMediaType type , UINT IsRecursion )
{
	switch(type)
	{
	case ID_Video:
		SearchVideo(SerachPath,IsRecursion);
		break;
	case ID_Music:
		SearchMusic(SerachPath,IsRecursion);
		break;
	case ID_Picture:
		SearchPicture(SerachPath,IsRecursion);
		break;
	default:
		break;
	}
}


	//查找符合类型的文件数量
UINT CMediaPlayList::GetListCount()
{
	return m_iFileCnt;
}

	//读取一条文件名
CString CMediaPlayList::ReadOneList( UINT Pos )
{
	POSITION posFile = m_vFilePath.FindIndex(Pos);
	CString str = m_vFilePath.GetAt(posFile);
	return str;
}