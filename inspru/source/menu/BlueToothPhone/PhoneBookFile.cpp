// PhoneBookFile1.cpp: implementation of the CPhoneBookFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "e9101main.h"
#include "PhoneBookFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CPhoneBookFile ,CObject , 1 )

CPhoneBookFile::CPhoneBookFile()
{

}

CPhoneBookFile::CPhoneBookFile(CString csName, CString csNum,CString csCity)
{
	m_csName= csName;
	m_csName=csNum;
	m_csCity = csCity; 
}

void CPhoneBookFile::SetValue(CString csName, CString csNum,CString csCity)
{
	m_csName= csName;
	m_csNum=csNum;
	m_csCity = csCity;
}

CPhoneBookFile::~CPhoneBookFile()
{

}

void CPhoneBookFile::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_csName;
		ar<<m_csNum;
		ar<<m_csCity;
	}
	else
	{
		ar>>m_csName;
		ar>>m_csNum;
		ar>>m_csCity;
	}

}
