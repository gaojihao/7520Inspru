// CResBitmap.cpp: implementation of the CResBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CResBitmap.h"
#include "Shellapi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile CResBitmap::m_BmpIniFile;

CString CResBitmap::m_csLanguageRootPath = _T("\\NandFlashPartition\\Main\\Language");

CString CResBitmap::g_csChinese = _T("\\Chinese.ini");
CString CResBitmap::g_csEnglish = _T("\\English.ini");
CString CResBitmap::g_csGermany = _T("\\Germany.ini");
CString CResBitmap::g_csFrance = _T("\\France.ini");
CString CResBitmap::g_csJapan = _T("\\Japan.ini");
CString CResBitmap::g_csItaly = _T("\\Italy.ini");
CString CResBitmap::g_csRussia = _T("\\Russia.ini");
CString CResBitmap::g_csIndia = _T("\\India.ini");

enLanguage CResBitmap::g_Language = ID_CHINESE;

void CResBitmap::SetLanguageRootPath(const CString& csPath)
{
	m_csLanguageRootPath = csPath;
}

CResBitmap::CResBitmap()
{

}

CResBitmap::~CResBitmap()
{

}

BOOL CResBitmap::SetLanguage(enLanguage LanguageId)
{
	BOOL bRts = FALSE;

	CString csName;

//	if(m_BmpIniFile.IsOpen())
//	{
//		m_BmpIniFile.Close();
//	}

	switch(LanguageId)
	{
	case ID_CHINESE:
		{
			csName = m_csLanguageRootPath + g_csChinese;
 			if(m_BmpIniFile.Load(csName))
			{
				g_Language = ID_CHINESE;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}

	case ID_ENGLISH:
		{
			csName = m_csLanguageRootPath + g_csEnglish;
 			if(m_BmpIniFile.Load(csName))
			{
				g_Language = ID_ENGLISH;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}	
	case ID_GERMANY:
		{
			csName = m_csLanguageRootPath + g_csGermany;
 			if(m_BmpIniFile.Load(csName))
 			{
				g_Language = ID_GERMANY;	
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}	
	case ID_FRANCE:
		{
			csName = m_csLanguageRootPath + g_csFrance;
 			if(m_BmpIniFile.Load(csName))	
			{
				g_Language = ID_FRANCE;		
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}	
	case ID_JAPAN:
		{
			csName = m_csLanguageRootPath + g_csJapan;
 			if(m_BmpIniFile.Load(csName))
			{
				g_Language = ID_JAPAN;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}	
	case ID_ITALY:
		{
			csName = m_csLanguageRootPath + g_csItaly;
 			if(m_BmpIniFile.Load(csName))
			{
				g_Language = ID_ITALY;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}
	case ID_RUSSIA:
		{
			csName = m_csLanguageRootPath + g_csRussia;
 			if(m_BmpIniFile.Load(csName))
			{
				g_Language = ID_RUSSIA;	
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}
	case ID_INDIA:
		{
			csName = m_csLanguageRootPath + g_csIndia;
 			if(m_BmpIniFile.Load(csName))
			{
				g_Language = ID_INDIA;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}
	default:
		bRts = FALSE;
		break;
	}


	return bRts;
}

void CResBitmap::ReleaseLanguage()
{
//	if(m_BmpIniFile.IsOpen())
//	{
//		m_BmpIniFile.Close();
// 	}

}

enLanguage CResBitmap::GetCurrencyLanguage()
{
	return g_Language;
}

BOOL CResBitmap::LoadBitmap( CString nNameID)
{
	CString csSection = _T("LanguageBmp");
//	CString csItem;
// 	csItem.Format(_T("%d"),nIDResource);

	CString csValue;
	
	if(m_BmpIniFile.GetString(csSection,nNameID,csValue))
	{
		return Attach(SHLoadDIBitmap(csValue));
	}
	else
		return FALSE;
 }
