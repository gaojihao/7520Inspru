// PhoneBookFile1.cpp: implementation of the CPhoneInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PhoneInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CPhoneInfo ,CObject , 1 )

CPhoneInfo::CPhoneInfo()
{

}

CPhoneInfo::CPhoneInfo(CString csNum, CString csName)
{
	m_csName= csName;
	m_csNum=csNum;

}

void CPhoneInfo::SetValue(CString csNum, CString  csName)
{
	m_csName= csName;
	m_csNum=csNum;
}

CPhoneInfo::~CPhoneInfo()
{

}

void CPhoneInfo::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar<<m_csNum;
		ar<<m_csName;
	}
	else
	{
		ar>>m_csNum;
		ar>>m_csName;
	}

}
