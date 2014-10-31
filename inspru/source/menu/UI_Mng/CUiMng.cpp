// CUiMng.cpp: implementation of the CUiMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CUiMng.h"
#include "ResMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUiMng CUiMng::_instance;

CUiMng& CUiMng::Instance()
{
	return _instance;
}

CUiMng::CUiMng()
{
//	m_curWnd = NULL;
//	m_WndMap.RemoveAll();
	m_WndArray.RemoveAll();
}

CUiMng::~CUiMng()
{
	
}

UINT CUiMng::GetWndCount()
{
	return m_WndArray.GetSize();
}

BOOL CUiMng::SetLanguage( enLanguage LanguageType)
{
	BOOL bStr = CResString::SetLanguage(LanguageType);

//	UINT yu =m_WndArray.GetSize();
	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pBaseDlg = m_WndArray.GetAt(i);
		if(IsWindow(pBaseDlg->m_hWnd))
			pBaseDlg->OnLanguageChange();
		else
			Detach(pBaseDlg);
	}

	return TRUE;
}

BOOL CUiMng::SetSkinType( enSkinType skinType )
{
	DWORD dw1 = ::GetTickCount();
	CResMng::Instance()->DestoryAllRes();
	TRACE1("*********%d***********\n", ::GetTickCount() - dw1 );
	BOOL bSkin = CSkinBitMap::SetSkinType(skinType);

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pBaseDlg = m_WndArray.GetAt(i);
		if(IsWindow(pBaseDlg->m_hWnd))
			pBaseDlg->Invalidate();
		else
			Detach(pBaseDlg);
	}

	return TRUE;
}

void CUiMng::ReleaseLanguage()
{
	CResString::ReleaseLanguage();
//	CResBitmap::ReleaseLanguage();
//	CUnitAndFormatSwitch::ReleaseLanguage();
}

void CUiMng::ReleaseSkin()
{
	CSkinBitMap::ReleaseSkin();
}

enLanguage CUiMng::GetCurLanguage()
{
	return CResString::GetCurrencyLanguage();
}

enSkinType CUiMng::GetCurSkin()
{
	return CSkinBitMap::GetCurSkinType();
}

BOOL CUiMng::SetTimeUnit( enTimeUnit timeUnit )
{
	BOOL bUnit = CUnitAndFormatSwitch::SetCurTimeUnit(timeUnit);
	//刷新界面单位

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pDialogBase = m_WndArray.GetAt(i);
		if (IsWindow(pDialogBase->m_hWnd))
		{
			pDialogBase->OnLanguageChange();
		}
		else
			Detach(pDialogBase);
	}

	return TRUE;
}

BOOL CUiMng::SetDistanceUnit( enDistanceUnit distanUnit )
{
	BOOL bUnit = CUnitAndFormatSwitch::SetCurDistanceUnit(distanUnit);	
	//刷新界面单位

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pDialogBase = m_WndArray.GetAt(i);
		if (IsWindow(pDialogBase->m_hWnd))
		{
			pDialogBase->OnLanguageChange();
		}
		else
			Detach(pDialogBase);
	}

	return TRUE;
}

BOOL CUiMng::SetSpeedUnit( enSpeedUnit speedUnit )
{
	BOOL bUnit = CUnitAndFormatSwitch::SetCurSpeedUnit(speedUnit);
	//刷新界面单位

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pDialogBase = m_WndArray.GetAt(i);
		if (IsWindow(pDialogBase->m_hWnd))
		{
			pDialogBase->OnLanguageChange();
		}
		else
			Detach(pDialogBase);
	}

	return TRUE;
}

BOOL CUiMng::SetHoueUnit( enHourUnit hourUnit)
{
	BOOL bUnit = CUnitAndFormatSwitch::SetCurHourUnit(hourUnit);
	//刷新界面单位

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pDialogBase = m_WndArray.GetAt(i);
		if (IsWindow(pDialogBase->m_hWnd))
		{
			pDialogBase->OnLanguageChange();
		}
		else
			Detach(pDialogBase);
	}

	return TRUE;
}

BOOL CUiMng::SetLongitudeALatitudeUnit( enLongitudeALatitudeUnit longAlatitude)
{
	BOOL bUnit = CUnitAndFormatSwitch::SetCurLongitudeALatitudeUnit(longAlatitude);
	//刷新界面单位

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pDialogBase = m_WndArray.GetAt(i);
		if (IsWindow(pDialogBase->m_hWnd))
		{
			pDialogBase->OnLanguageChange();
		}
		else
			Detach(pDialogBase);
	}

	return TRUE;

}

BOOL CUiMng::SetAngleUnit( enAngleUnit angle)
{
	BOOL bUnit = CUnitAndFormatSwitch::SetAngleUnit(angle);
	//刷新界面单位

	for(int i=0; i<m_WndArray.GetSize(); i++)
	{
		CDialogBase* pDialogBase = m_WndArray.GetAt(i);
		if (IsWindow(pDialogBase->m_hWnd))
		{
			pDialogBase->OnLanguageChange();
		}
		else
			Detach(pDialogBase);
	}

	return TRUE;
	
}

enDistanceUnit CUiMng::GetCurDistanceUnit( )
{
	return CUnitAndFormatSwitch::GetCurDistanceUnit();
}

enSpeedUnit CUiMng::GetCurSpeedUnit( )
{
	return CUnitAndFormatSwitch::GetCurSpeedUnit();
}

enTimeUnit CUiMng::GetCurTimeUnit( )
{
	return CUnitAndFormatSwitch::GetCurTimeUnit();
}

enHourUnit CUiMng::GetCurHoueUnit()
{
	return CUnitAndFormatSwitch::GetCurHourUnit();
}

enLongitudeALatitudeUnit CUiMng::GetCurLongitudeALatitudeUnit()
{
	return CUnitAndFormatSwitch::GetCurLongitudeALatitudeUnit();
}

enAngleUnit CUiMng::GetCurAngleUnit()
{
	return CUnitAndFormatSwitch::GetAngleUnit();
}


void CUiMng::SetLanguagePath( const CString& LanguagePath )
{
	CResString::SetLanguageRootPath(LanguagePath);
}

void CUiMng::SetSkinPath( const CString& SkinPath )
{
	CSkinBitMap::SetSkinRootPath(SkinPath);
}



BOOL CUiMng::Attach(CDialogBase* pDialogBase)
{
	m_WndArray.Add(pDialogBase);
	return TRUE;
}

BOOL CUiMng::Detach(CDialogBase* pDialogBase)
{
	for(int i=m_WndArray.GetSize(); i>1; i--)
	{
		if(m_WndArray.GetAt(i-1) == pDialogBase)
			m_WndArray.RemoveAt(i-1);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//
// 函数名       : CUiMng::DetachAll
// 功能描述     : 释放除主才单的所有窗口
// 返回值       : BOOL 
//
///////////////////////////////////////////////////////////////////////
BOOL CUiMng::DetachAll()
{
	CDialogBase* pDialogBase = NULL;
	for(int i = m_WndArray.GetSize(); i>1 ; i--)
	{
		if(IsWindow((pDialogBase = m_WndArray.GetAt(i-1))->m_hWnd))
			pDialogBase->OnExit();
	}
	return TRUE;
}
//时区
//BOOL CUiMng::SetCueTimeZone(enTimeZone TimeZone)
//{
//	BOOL bUnit = CUnitAndFormatSwitch::SetCueTimeZone(TimeZone);
//	//刷新界面单位
//
//	for(int i=0; i<m_WndArray.GetSize(); i++)
//	{
//		m_WndArray.GetAt(i)->OnLanguageChange();
//	}
//
//	return TRUE;
//}
//
//enTimeZone CUiMng::GetCurTimeZone()
//{
//	return CUnitAndFormatSwitch::GetCurTimeZone();
//}
