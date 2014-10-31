// PhoneBookMng.cpp: implementation of the PhoneBookMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "e9101main.h"
#include "PhoneBookMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PhoneBookMng PhoneBookMng::_PhoneBook;

PhoneBookMng::PhoneBookMng()
{
	m_Cnt = 0;
	m_CurPos = 0;
}

PhoneBookMng::~PhoneBookMng()
{
//	delete[] m_pPhoneBookFile;
}

PhoneBookMng& PhoneBookMng::Instance()
{
	return _PhoneBook;
}

BOOL PhoneBookMng::Load()
{
	m_ObArr.RemoveAll();
	m_Cnt = 0;

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
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneBook.jdt");


 	CFile DialBookFile;
 	if( !DialBookFile.Open(strFullPath,  CFile::modeReadWrite ) )
		return FALSE;

	if( DialBookFile.GetLength() == 0 )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[4096];
	CArchive ar(&DialBookFile,CArchive::load,4096,bookBuf);

	ar >> m_Cnt;


	CPhoneBookFile* temppDialLog;
	m_pPhoneBookFile = new CPhoneBookFile[m_Cnt+1];
	temppDialLog = m_pPhoneBookFile;
 
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
	DialBookFile.Close();

	delete[] bookBuf;
	
	return TRUE;
}

UINT PhoneBookMng::GetCount()
{
	return m_Cnt;
}

BOOL PhoneBookMng::Delete ( CPhoneBookFile* pData)
{
	CPhoneBookFile* CurCPhoneBookFile = NULL;
	for( UINT i=0; i<m_ObArr.GetSize();i++)
	{
		if( pData == (CPhoneBookFile*)m_ObArr.GetAt(i) ) 
		{
			m_ObArr.RemoveAt(i);
			m_Cnt -= 1;

// 			if( NULL != pData)
// 			{
// 				delete pData;
// 				pData = NULL;
//			}
			return TRUE;
		}

	}
	return FALSE;
}

BOOL PhoneBookMng::DeleteLastPhone( )
{

	if(m_Cnt > 0)
	{
		m_ObArr.RemoveAt(m_ObArr.GetSize()-1);
		m_Cnt -= 1;
		return TRUE;
	}
	else
		return FALSE;
}


BOOL PhoneBookMng::Delete( UINT ipos )
{

	if( 0 < ipos <= m_ObArr.GetSize() )
	{
// 		CObject* obj = m_ObArr.GetAt(ipos);
// 	
// 		if( NULL != obj)
// 		{
// 				delete obj;
// 				obj = NULL;
//		}	
		
		m_ObArr.RemoveAt( ipos );
		m_Cnt -= 1;
		return TRUE;
	}
	else
		return FALSE;
}


BOOL PhoneBookMng::DelAll()
{
// 	for( int i =0;i< m_ObArr.GetSize();i++ )
// 	{
// 		CObject* obj = m_ObArr.GetAt(i);
// 
// 		if( NULL != obj )
// 		{
// 			delete obj;
// 			obj = NULL;
// 		}
//	}
	
	m_ObArr.RemoveAll();
	m_Cnt = 0;
	WriteNull();
	return TRUE;
}

BOOL PhoneBookMng::AddPhone( CPhoneBookFile* pPhone)
{
	m_ObArr.Add(pPhone);
	m_Cnt++;

	return TRUE;
}

BOOL PhoneBookMng::AddLastPhone( CPhoneBookFile* pPhone)
{
	m_ObArr.RemoveAt(m_ObArr.GetSize()-1);
	m_ObArr.Add(pPhone);
//	m_Cnt++;

	return TRUE;
}

BOOL PhoneBookMng::Write()
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
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneBook.jdt");


 	CFile DialBookFile;
 	if( !DialBookFile.Open(strFullPath,  CFile::modeCreate | CFile::modeReadWrite) )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[40960];
	CArchive ar(&DialBookFile,CArchive::store,40960,bookBuf);
	
	ar << m_Cnt;

	CPhoneBookFile* pPhone = NULL;
	for(UINT j=0;j<m_Cnt;j++)
	{
		pPhone = (CPhoneBookFile*)m_ObArr.GetAt(j);
		pPhone->Serialize(ar);
	}

	ar.Flush();
	ar.Close();
	DialBookFile.Close();

	delete[] bookBuf;
	delete[] m_pPhoneBookFile;

	return TRUE;
}


BOOL PhoneBookMng::WriteNull()
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
	CString strFullPath = strPath.Left(iPos+1) + _T("PhoneBook.jdt");


 	CFile DialBookFile;
 	if( !DialBookFile.Open(strFullPath,  CFile::modeCreate | CFile::modeReadWrite) )
		return FALSE;

 	TCHAR* bookBuf = new TCHAR[40960];
	CArchive ar(&DialBookFile,CArchive::store,40960,bookBuf);
	
	ar << m_Cnt;

	CPhoneBookFile* pPhone = NULL;
	for(UINT j=0;j<m_Cnt;j++)
	{
		pPhone = (CPhoneBookFile*)m_ObArr.GetAt(j);
		pPhone->Serialize(ar);
	}

	ar.Flush();
	ar.Close();
	DialBookFile.Close();

	delete bookBuf;
	return TRUE;
}


CObArray* PhoneBookMng::GetPhoneBook()
{
	return &m_ObArr;
}

CPhoneBookFile* PhoneBookMng::GetCurPhoneBookItem()
{
	return 	(CPhoneBookFile*)m_ObArr.GetAt(m_CurPos);
}

CPhoneBookFile* PhoneBookMng::GetPhoneBookItem(UINT pos)
{
	return 	(CPhoneBookFile*)m_ObArr.GetAt(pos);
}

void PhoneBookMng::SetCurPhoneBookItem( UINT pos )
{
	m_CurPos = pos;
}

void PhoneBookMng::ReSetCurItem( CPhoneBookFile* pItem )
{
	m_ObArr.SetAt(m_CurPos,pItem);
}

//////////////////////////////////////////////////////////////////////////

CPhoneBookFile* PhoneBookMng::SearchPhoneItem( CString& strPhone )
{
	if ( m_ObArr.GetSize() <=0 || strPhone.GetLength() <= 0 )
		return NULL;

	CPhoneBookFile* pItem = NULL;
	UINT Count = GetCount();
	for(int i=0; i<Count; i++)
	{
		pItem = GetPhoneBookItem(i);
		if ( pItem )
		{
			if( pItem->m_csNum == strPhone )
				return pItem;
		}
	}

	return NULL;
}