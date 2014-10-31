// DialLog.cpp: implementation of the CDialLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "e9101main.h"
#include "DialLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CDialLog ,CObject , 1 )

CDialLog::CDialLog()
{

}

CDialLog::~CDialLog()
{

}

void CDialLog::Serialize( CArchive& ar )
{
	//����Ϣд���ļ�
	if(ar.IsStoring())
	{
		ar << m_iLogFlag << m_csName << m_csNum << m_csCity ;//<< m_csDate << m_csTime;
	}
	//���ļ���ȡ��Ϣ
	else
	{
		ar >> m_iLogFlag >> m_csName >> m_csNum >> m_csCity ;//>> m_csDate >> m_csTime;
	}
}
