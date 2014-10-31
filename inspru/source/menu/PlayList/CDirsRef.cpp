// CDirsRef.cpp : 实现文件
//

#include "stdafx.h"
#include "CDirsRef.h"
#include "ffsco.h"

IMPLEMENT_SERIAL(CDirsRef,CObject,VERSIONABLE_SCHEMA|1)

CDirsRef::CDirsRef(CString ParentDirName,UINT recursion,CString fext):m_iDirCnt(0),m_iFileCnt(0)
{
	if(!m_vDirPath.IsEmpty())
		m_vDirPath.RemoveAll();
	if(!m_vFilePath.IsEmpty())
		m_vFilePath.RemoveAll();
	if(fext == _T(""))
		GetFileAndDir(ParentDirName,recursion);
	else
		GetFileAndDir(ParentDirName,recursion,fext);
}

CDirsRef::CDirsRef():m_iDirCnt(0),m_iFileCnt(0)
{
	if(!m_vDirPath.IsEmpty())
		m_vDirPath.RemoveAll();
	if(!m_vFilePath.IsEmpty())
		m_vFilePath.RemoveAll();
}

CDirsRef::~CDirsRef()
{
	if(!m_vDirPath.IsEmpty())
		m_vDirPath.RemoveAll();
	if(!m_vFilePath.IsEmpty())
		m_vFilePath.RemoveAll();
	m_iDirCnt = 0;
	m_iFileCnt = 0;
	
}
void
CDirsRef::GetFileAndDir(CString ParentDirName,UINT recursion,CString fext)
{
	ffsco o;

	int DelPathLen = ParentDirName.GetLength();
	//是否递归查找
	o.dirs(recursion);

	
#ifdef _UNICODE
	TCHAR* str = (TCHAR*)ParentDirName.GetBuffer(ParentDirName.GetLength());
#else
	char* str = (char*)ParentDirName.GetBuffer(ParentDirName.GetLength());
#endif
	
	if(fext == _T(""))
	{
		o.find(str);
	}
	else
	{
#ifdef _UNICODE
	TCHAR* ext = (TCHAR*)fext.GetBuffer(fext.GetLength());
#else
	char* ext = (char*)fext.GetBuffer(fext.GetLength());
#endif
	o.find(str,ext);
	}
	
	ffsco::typeT coo;
	ffsco::typeT::iterator it;

	coo = o.co_dir();

	for (it = coo.begin();coo.end() != it;it ++)
	{
		CString ty = (*it).c_str();
		ty.Delete(0,DelPathLen);
		m_vDirPath.AddTail(ty);
	}
	coo = o.co_file();
	for (it = coo.begin();coo.end() != it;it ++)
	{
		CString uu = (*it).c_str();
		uu.Delete(0,DelPathLen);
		m_vFilePath.AddTail(uu);
	}

	GetFilesCnt();
	GetDirsCnt();
}

void
CDirsRef::WriteDirs(CString DirsName)
{
	m_vDirPath.AddTail(DirsName);
}

void
CDirsRef::WriteFiles(CString FileName)
{
	m_vFilePath.AddTail(FileName);
}

int
CDirsRef::GetFilesCnt()
{
	m_iFileCnt = m_vFilePath.GetCount();
	return m_iFileCnt;
}

int
CDirsRef::GetDirsCnt()
{
	m_iDirCnt = m_vDirPath.GetCount();
	return m_iDirCnt;
}

void
CDirsRef::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_iDirCnt;
		for(int i=0;i<m_vDirPath.GetCount();i++)
		{
			POSITION pos1 = m_vDirPath.FindIndex(i);
			ar << m_vDirPath.GetAt(pos1);
		}


		ar << m_iFileCnt;
		for(int j=0;j<m_vFilePath.GetCount();j++)
		{
			POSITION pos2 = m_vFilePath.FindIndex(j);
			ar << m_vFilePath.GetAt(pos2);
		}

	}
	else
	{
		CString tem;
		UINT i;

		ar >> m_iDirCnt;
		for(i=0;i<m_iDirCnt;i++)
		{
			ar >> tem;
			m_vDirPath.AddTail(tem);
		}
		 
		ar >> m_iFileCnt;
		for(i=0;i<m_iFileCnt;i++)
		{
			ar >> tem;
			m_vFilePath.AddTail(tem);
		}
	}
}

CString
CDirsRef::ReadOneFileName(UINT pos) 
{	
	POSITION posFile = m_vFilePath.FindIndex(pos);
	CString str = m_vFilePath.GetAt(posFile);
	return str;
}

CString
CDirsRef::ReadOneDirName(UINT pos)
{
	POSITION posDir = m_vDirPath.FindIndex(pos);
	CString str = m_vDirPath.GetAt(posDir);
	return str;
}
// CDirsRef 成员函数
