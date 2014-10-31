// PhoneLogMng.cpp: implementation of the PhoneLogMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "e9101main.h"
#include "PhoneLogMng.h"
#include "../SQLite3/CppSQLite3.h"
#include "../ExTool/ex_basics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



PhoneLogMng PhoneLogMng::_PhoneLog;

PhoneLogMng::PhoneLogMng()
{
	m_Cnt = 0;
	tempCnt = 0;
	m_CurPos = 0;
	m_PhoneLogState = 0;
}

PhoneLogMng::~PhoneLogMng()
{
//	delete[] m_pDialLog;
}

PhoneLogMng& PhoneLogMng::Instance()
{
	return _PhoneLog;
}

BOOL PhoneLogMng::Load()
{
	m_ObArr.RemoveAll();
	m_Cnt = 0;
	tempCnt = 0;

	TCHAR strModuleFileName[256];
	CString strPath;
	memset((void *)strModuleFileName,0,256);


	GetModuleFileName(NULL,strModuleFileName,256);
	strPath = strModuleFileName;
	UINT iPos=-1;

 	for(int i=strPath.GetLength()-1;i>=0;i--)
 	{
		TCHAR ch = strPath.GetAt(i);
		if(ch==L'\\')
 		{
 			iPos = i;
 			break;
 		}
	}
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneLog.jdt");


 	CFile Diallog;
 	if( !Diallog.Open(strFullPath,  CFile::modeReadWrite ) )
		return FALSE;

	if( Diallog.GetLength() == 0 )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[40960];
	CArchive ar(&Diallog,CArchive::load,40960,bookBuf);

	ar >> m_Cnt;

 	if( m_Cnt == 0 )
		return FALSE;
	CDialLog* temppDialLog;
	m_pDialLog = new CDialLog[m_Cnt];
	temppDialLog=m_pDialLog;
	if(m_PhoneLogState > 0)
	{
		for( UINT j = 0; j<m_Cnt; j++ )
		{
			temppDialLog->Serialize(ar);
			if(temppDialLog->m_iLogFlag == m_PhoneLogState)
			{
				tempCnt++;
				m_ObArr.Add(temppDialLog);
			}
//			else
//				m_Cnt--;
			temppDialLog++;
		}
	}
	else
	{

		for( UINT j = 0; j<m_Cnt; j++ )
		{
			temppDialLog->Serialize(ar);
			m_ObArr.Add(temppDialLog);
			temppDialLog++;
		}
	}


	ar.Flush();
	ar.Close();
	Diallog.Close();

	delete[] bookBuf;
//	delete[] (m_pDialLog-m_Cnt);

	return TRUE;
}

UINT PhoneLogMng::GetCount()
{	
	if(m_PhoneLogState > 0)
	{
		return tempCnt;
	}
	return m_Cnt;
}

BOOL PhoneLogMng::Delete ( CDialLog* pData)
{
	CDialLog* CurCPhoneBookFile = NULL;

	for( UINT i=0; i<m_ObArr.GetSize();i++)
	{
		CurCPhoneBookFile = (CDialLog*)m_ObArr.GetAt(i);
		if( pData->m_csNum == CurCPhoneBookFile->m_csNum && pData->m_iLogFlag == CurCPhoneBookFile->m_iLogFlag ) 
		{
			m_ObArr.RemoveAt(i);
			m_Cnt -= 1;
			return TRUE;
		}

	}
	return FALSE;

}

BOOL PhoneLogMng::Delete( UINT ipos )
{

	if( 0 < ipos <= m_ObArr.GetSize() )
	{
		m_ObArr.RemoveAt( ipos );
		m_Cnt -= 1;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL PhoneLogMng::AddLog( CDialLog* pPhone)
{
	m_ObArr.InsertAt(0,pPhone);
	m_Cnt++;

	char szphonenum[200];
	memset(szphonenum,0,sizeof(szphonenum));
	WideCharToMultiByte(CP_ACP,NULL, pPhone->m_csNum, -1,szphonenum, sizeof(szphonenum), NULL,FALSE );

	char szSql[512];
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(szSql, "INSERT INTO phone_log(flag,phone_number) VALUES(%d, %s);", 
		pPhone->m_iLogFlag, szphonenum);

	db.execDML(szSql);
	db.close();

	return TRUE;
}

BOOL PhoneLogMng::Write()
{
	TCHAR strModuleFileName[256];
	CString strPath;
	memset((void *)strModuleFileName,0,256);

	GetModuleFileName(NULL,strModuleFileName,256);
	strPath = strModuleFileName;
	UINT iPos=-1;

 	for(int i=strPath.GetLength()-1;i>=0;i--)
 	{
		TCHAR ch = strPath.GetAt(i);
		if(ch==L'\\')
 		{
 			iPos = i;
 			break;
 		}
	}
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneLog.jdt");

 	CFile DialBookFile;
 	if( !DialBookFile.Open(strFullPath,  CFile::modeCreate | CFile::modeReadWrite) )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[40960];
	CArchive ar(&DialBookFile,CArchive::store,40960,bookBuf);
	
	ar << m_Cnt;

	CDialLog* pPhone = NULL;
	for(UINT j=0;j<m_Cnt;j++)
	{
		pPhone = (CDialLog*)m_ObArr.GetAt(j);
		pPhone->Serialize(ar);
	}

	ar.Flush();
	ar.Close();
	DialBookFile.Close();

	delete[] bookBuf;
	delete[] m_pDialLog;
	return TRUE;
}


BOOL PhoneLogMng::WriteNull()
{
	TCHAR strModuleFileName[256];
	CString strPath;
	memset((void *)strModuleFileName,0,256);

	GetModuleFileName(NULL,strModuleFileName,256);
	strPath = strModuleFileName;
	UINT iPos=-1;

 	for(int i=strPath.GetLength()-1;i>=0;i--)
 	{
		TCHAR ch = strPath.GetAt(i);
		if(ch==L'\\')
 		{
 			iPos = i;
 			break;
 		}
	}
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneLog.jdt");

 	CFile DialBookFile;
 	if( !DialBookFile.Open(strFullPath,  CFile::modeCreate | CFile::modeReadWrite) )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[40960];
	CArchive ar(&DialBookFile,CArchive::store,40960,bookBuf);
	
	ar << m_Cnt;

	CDialLog* pPhone = NULL;
	for(UINT j=0;j<m_Cnt;j++)
	{
		pPhone = (CDialLog*)m_ObArr.GetAt(j);
		pPhone->Serialize(ar);
	}

	ar.Flush();
	ar.Close();
	DialBookFile.Close();

	delete[] bookBuf;
	return TRUE;
}

CObArray* PhoneLogMng::GetPhoneLog()
{
	if(m_ObArr.GetSize()>0)
		return &m_ObArr;
	else
		return NULL;
}

CDialLog* PhoneLogMng::GetCurPhoneLogItem()
{
	return 	(CDialLog*)m_ObArr.GetAt(m_CurPos);
}

void PhoneLogMng::SetCurPhoneLogItem( UINT pos )
{
	m_CurPos = pos;
}

BOOL PhoneLogMng::DelAll()
{
	m_ObArr.RemoveAll();
	m_Cnt = 0;
	WriteNull();
	return TRUE;
}

void PhoneLogMng::SetLPhoneLogState(UINT state)
{
	m_PhoneLogState = state;
}

void PhoneLogMng::DelLPhoneLogState(UINT state)
{
	int tempsize;
	tempsize = m_ObArr.GetSize();
	CDialLog* CurCPhoneBookFile = NULL;

	if(LoadAll())
	{
		for( int i=0; i<m_ObArr.GetSize();i++)
		{
			CurCPhoneBookFile = (CDialLog*)m_ObArr.GetAt(i);
			if(CurCPhoneBookFile->m_iLogFlag == state) 
			{
				m_ObArr.RemoveAt(i);
				m_Cnt -= 1;
				i -= 1;
			}
		}
		Write();
	}
}

BOOL PhoneLogMng::LoadAll()
{
	m_ObArr.RemoveAll();
	m_Cnt = 0;
	tempCnt = 0;

	TCHAR strModuleFileName[256];
	CString strPath;
	memset((void *)strModuleFileName,0,256);

	GetModuleFileName(NULL,strModuleFileName,256);
	strPath = strModuleFileName;
	UINT iPos=-1;

 	for(int i=strPath.GetLength()-1;i>=0;i--)
 	{
		TCHAR ch = strPath.GetAt(i);
		if(ch==L'\\')
 		{
 			iPos = i;
 			break;
 		}
	}
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneLog.jdt");

	CFile Diallog;
 	if( !Diallog.Open(strFullPath,  CFile::modeReadWrite ) )
		return FALSE;

	if( Diallog.GetLength() == 0 )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[40960];
	CArchive ar(&Diallog,CArchive::load,40960,bookBuf);

	ar >> m_Cnt;

	CDialLog* temppDialLog;
	m_pDialLog = new CDialLog[m_Cnt+1];
	temppDialLog=m_pDialLog;
 	if( m_Cnt == 0 )
		return FALSE;
	for( UINT j = 0; j<m_Cnt; j++ )
	{
		temppDialLog->Serialize(ar);
		m_ObArr.Add(temppDialLog);
		temppDialLog++;
	}

	ar.Flush();
	ar.Close();
	Diallog.Close();

	delete[] bookBuf;
//	delete[] (m_pDialLog-m_Cnt);

	return TRUE;
}