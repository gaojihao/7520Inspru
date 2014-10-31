// CResString.cpp: implementation of the CResString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CResString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIniFile CResString::m_IniFile;

CArray<enLanguage,enLanguage> CResString::g_LanguageList;

CString CResString::m_csLanguageRootPath = _T("\\Storage Card");
CString CResString::g_csLanguageIni = _T("\\Language.ini");

CString CResString::g_csLanguageFileName = _T("");


enLanguage CResString::g_Language = ID_CHINESE;

void CResString::SetLanguageRootPath( const CString& csPath)
{
	m_csLanguageRootPath = csPath;
}

CResString::CResString()
{

}

CResString::CResString( const CResString& stringSrc )
			 : CString(stringSrc)
{

}

CResString::CResString( TCHAR ch, int nRepeat)
			 : CString(ch,nRepeat)
{

}

CResString::CResString( LPCTSTR lpch, int nLength )
			 : CString(lpch,nLength)
{

}

CResString::CResString( const unsigned char* psz )
			 : CString(psz)
{

}

CResString::CResString( LPCWSTR lpsz )
			 : CString(lpsz)
{

}

CResString::CResString( LPCSTR lpsz )
			: CString(lpsz)
{

}

CResString::~CResString()
{

}

BOOL CResString::LoadString( CString NameID )
{
	CString csSection = _T("LanguageString");
//	CString csItem;
// 	csItem.Format(_T("%d"),nID);

	CString csValue;
	
	if(m_IniFile.GetString(csSection,NameID,csValue))
	{
		CResString tem(csValue);
		*this = tem; 
		return TRUE;
	}
	else
		return FALSE;

}

UINT CResString::GetLanguageCount()
{
	return g_LanguageList.GetSize();
}

enLanguage CResString::GetValidLanguage(UINT pos)
{
//	enLanguage khj;
//	for(int i=0 ; i<g_LanguageList.GetSize(); i++)
//	{
//		khj = g_LanguageList.GetAt(i);
// 	}
	return g_LanguageList.GetAt(pos-1);
}

BOOL CResString::SetLanguage(enLanguage LanguageId)
{
	BOOL bRts = FALSE;
	CString csName;

//ÔØÈëÓïÑÔÅäÖÃ

	csName = m_csLanguageRootPath + g_csLanguageIni;
	m_IniFile.Load(csName);

	g_LanguageList.RemoveAll();
	CString Sec = _T("Language");
	CString LanguageID;
	UINT LanPos;
	for(LanPos=0; LanPos<23;LanPos++)
	{
		CString LanguageName;
		LanguageID.Format(_T("Language_%d"),LanPos);
		m_IniFile.GetString(Sec,LanguageID,LanguageName);
		if(!LanguageName.IsEmpty())
		{
			g_LanguageList.Add(enLanguage(LanPos));
		}

		if(LanguageId == (enLanguage)LanPos)
		{
			g_csLanguageFileName = _T("\\") + LanguageName;
		}
	}

//	switch(LanguageId)
//	{
//	case ID_CHINESE:
		if(LanguageId>= 0 && LanguageId<23)
		{
			csName = m_csLanguageRootPath + g_csLanguageFileName;
 			if(m_IniFile.Load(csName))
			{
				g_Language = LanguageId;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
		}
		else
			bRts = FALSE;

// 	case ID_ENGLISH:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_ENGLISH;
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}	
// 	case ID_GERMANY:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_GERMANY;	
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}	
// 	case ID_FRANCE:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_FRANCE;		
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}
// 	case ID_PORTUGAL:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_FRANCE;		
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}
// 	case ID_ITALY:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_ITALY;
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}
// 	case ID_RUSSIA:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_RUSSIA;	
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}
// 	case ID_INDIA:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))	
// 			{
// 				g_Language = ID_INDIA;
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}
// 	case ID_JAPAN:
// 		{
// 			csName = m_csLanguageRootPath + g_csLanguageFileName;
//  			if(m_IniFile.Load(csName))
// 			{
// 				g_Language = ID_JAPAN;
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
//		}
// 	default:
// 		bRts = FALSE;
// 		break;
//	}



	return bRts;
}

void CResString::ReleaseLanguage()
{
//	if(m_IniFile.IsOpen())
//	{
//		m_IniFile.Close();
// 	}

}

enLanguage CResString::GetCurrencyLanguage()
{
	return g_Language;
}

BOOL CResString::SetString(CString NameID, CString Value)
{
	CString csSection = _T("LanguageString");
//	CString csItem;
// 	csItem.Format(_T("%d"),nID);

	if(m_IniFile.SetString(csSection, NameID, Value))
	{
		return TRUE;
	}
	else
		return FALSE;
}

CString CResString::CResStringToCString()
{	
	CString str(this->GetBuffer(this->GetLength()));
	return str;
}

CResString AFXAPI operator+ (CResString& csFirest,CResString& csSecond)
{
	CString str = csFirest.CResStringToCString() + csSecond.CResStringToCString();
	CResString str2(str);
	return str;
}

CResString AFXAPI operator+ (CString& csFirest,CResString& csSecond)
{
	CResString str = csFirest + csSecond.CResStringToCString();
	return str;
 }

CResString AFXAPI operator+ (CResString& csFirest,CString& csSecond)
{
	CResString str = csFirest.CResStringToCString() + csSecond;
	return str;
}

