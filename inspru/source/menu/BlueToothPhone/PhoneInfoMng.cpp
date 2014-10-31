// PhoneInfoMng.cpp: implementation of the PhoneInfoMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PhoneInfoMng.h"
#include "SysConfig.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PhoneInfoMng PhoneInfoMng::_PhoneInfo;

extern CString g_FileName;

PhoneInfoMng::PhoneInfoMng()
{
	Load();
}

PhoneInfoMng::~PhoneInfoMng()
{
	delete[] m_PhoneInfo;
}

PhoneInfoMng& PhoneInfoMng::Instance()
{
	return _PhoneInfo;
}



CString PhoneInfoMng::FindPhoneNum(CString csNum)
{
	CString head = csNum;
	CString kj = csNum;
	CString subNum = csNum;
	CString fg = csNum;

	head.Delete(1,head.GetLength()-1);

	if( head == _T('1') && csNum.GetLength() >= 8)
	{
		kj.Delete(3,kj.GetLength()-3);
	}
	else if( head == _T('0') )
	{
		kj = _T("Tel");
	}
	else
	{
		if( CSysConfig::Instance().GetLanguage() == ID_CHINESE )
			return _T("±¾µØºÅÂë");
		else
			return _T("Local number");
	}




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
	CString strFullPath = strPath.Left(iPos+1) + _T("Tel\\") + kj ;


 	CFile DialBookFile;

	if( !DialBookFile.Open(strFullPath,  CFile::modeRead ) )
		return _T("");
	

	if( head == _T('1') )
	{
		subNum.Delete(7,subNum.GetLength()-7);
		subNum.Delete(0,3);


		int isubNum = _ttol(subNum);

		int pos = 29 * isubNum ;
		DialBookFile.Seek( pos,CFile::begin );

		char buf[28];
		DialBookFile.Read(buf,27);

		buf[27] = '\0';

		CString rts(buf);

		rts.Delete(0,7);
		rts.TrimRight();
		DialBookFile.Close();

		return rts;
	}
	else if( head == _T('0') )
	{
		
		CPhoneInfo* pInfo = NULL;
		fg.Delete(fg.GetLength()-7,7);
		subNum.Delete(subNum.GetLength()-8,8);
		
		for( int h=0;h<m_ObArr.GetSize();h++)
		{
			pInfo = (CPhoneInfo*)m_ObArr.GetAt(h);
			
			if( pInfo->m_csNum == fg || pInfo->m_csNum == subNum )
				return pInfo->m_csName;
				
		}
		return _T("");
	}
}

BOOL PhoneInfoMng::Load()
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
	CString strFullPath = strPath.Left(iPos+1) + _T("Tel\\Tel");


 	CFile DialBookFile;
 	if( !DialBookFile.Open(strFullPath,  CFile::modeReadWrite ) )
		return FALSE;

	if( DialBookFile.GetLength() == 0 )
		return FALSE;
 
	TCHAR* bookBuf = new TCHAR[4096];
	CArchive ar(&DialBookFile,CArchive::load,4096,bookBuf);

	ar >> m_Cnt;	
 	if( m_Cnt == 0 )
		return FALSE;

	CPhoneInfo* temppDialLog;
	m_PhoneInfo = new CPhoneInfo[m_Cnt];
	temppDialLog = m_PhoneInfo;
	
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
