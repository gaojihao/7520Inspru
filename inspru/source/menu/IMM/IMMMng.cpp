// IMMMng.cpp: implementation of the CIMMMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
/*#include "..\InpushlMfcTest.h"*/
#include "IMMMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern BOOL g_bLight;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIMMMng CIMMMng::_IMMMng;

CIMMMng::CIMMMng()
{
	m_hInst    = NULL;
	m_fIMMInit = NULL;
	
	m_fIMMCreateMsg = NULL;
	m_fIMMEditMsg   = NULL;
	m_fIMMAddPhone  = NULL;
	m_fIMMEditPhone = NULL;
	
	m_fIMMAddPhoneNum  = NULL;
	m_fIMMEditPhoneNum = NULL;

	m_fGetReslut = NULL;

	m_bSucceed   =  FALSE;
}

CIMMMng::~CIMMMng()
{

}

CIMMMng& CIMMMng::Instance()
{
	return _IMMMng;
}

BOOL CIMMMng::Load()
{
	m_hInst = LoadLibrary(_T(".\\InputShl.dll"));
 	if(m_hInst)
 	{
		m_fIMMInit      = (INIT)GetProcAddress(m_hInst, _T("InitInputShl"));
		if(m_fIMMInit   == NULL)
			return FALSE;

		m_fSetKeySound      = (IMMSETKEYSOUND)GetProcAddress(m_hInst, _T("ImmSetKeySound"));
		if(m_fSetKeySound   == NULL)
			return FALSE;

		m_fGetReslut    = (GETRESULT)GetProcAddress(m_hInst, _T("GetStrText"));
		if(m_fGetReslut == NULL)
			return FALSE;

		m_fIMMAddPhone  = (IMMADDPHONE)GetProcAddress(m_hInst, _T("ImmAddPhone"));
		if(m_fIMMAddPhone == NULL)
			return FALSE;
		
		m_fIMMEditPhone = (IMMEDITPHONE)GetProcAddress(m_hInst, _T("ImmEditPhone"));
		if(m_fIMMEditPhone == NULL)
			return FALSE;
	
		m_fIMMAddPhoneNum = (IMMADDPHONENUM)GetProcAddress(m_hInst, _T("ImmAddPhoneNum"));
		if(m_fIMMAddPhoneNum == NULL)
			return FALSE;

		m_fIMMEditPhoneNum = (IMMEDITPHONENUM)GetProcAddress(m_hInst, _T("ImmEditPhoneNum"));
		if(m_fIMMEditPhoneNum == NULL)
			return FALSE;

		m_fIMMCreateMsg   = (IMMCREATEMESSAGE)GetProcAddress(m_hInst,_T("ImmCreateMessage"));
		if(m_fIMMCreateMsg == NULL)
			return FALSE;

		m_fIMMEditMsg     = (IMMEDITMESSAGE)GetProcAddress(m_hInst, _T("ImmEditMessage"));
		if(m_fIMMEditMsg == NULL)
			return FALSE;

		m_bSucceed = TRUE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void  CIMMMng::ImmSetKeySound(enKeySound enkey)
{
	if ( m_bSucceed )
		m_fSetKeySound( enkey );
}

void CIMMMng::IMMInit(ScreenSize nSize)
{
	if(m_bSucceed == TRUE)
		m_fIMMInit(nSize);
}

ImmReturn CIMMMng::IMMCreateMessage(enLanguage enlang)
{
	if(m_bSucceed == TRUE)
		return	m_fIMMCreateMsg(enlang);
	else
		return  Cancel;
}

ImmReturn CIMMMng::IMMEditMessage(enLanguage enlang, LPTSTR strMessage)
{
	if(m_bSucceed == TRUE)
		return m_fIMMEditMsg(enlang, strMessage);
	else
		return Cancel;
}

ImmReturn CIMMMng::IMMAddPhone(enLanguage enLang)
{
	if(m_bSucceed == TRUE)
		return m_fIMMAddPhone(enLang);
	return Cancel;
}

ImmReturn CIMMMng::IMMEditPhone(enLanguage enlang, LPTSTR strPhone, enKeySound enkey,enSkinType enSkin)
{
	if(m_bSucceed == TRUE)
		return m_fIMMEditPhone(enlang, strPhone, enkey, enSkin);
	else
		return Cancel;
}

ImmReturn CIMMMng::IMMAddPhoneNum(HWND hParentHwnd, HWND hWndEdit)
{
	if(m_bSucceed == TRUE)
		return m_fIMMAddPhoneNum(hParentHwnd,hWndEdit);
	else
		return Cancel;
}

ImmReturn CIMMMng::IMMEditPhoneNum(HWND hParentHwnd, HWND hWndEdit)
{
	if(m_bSucceed == TRUE)
		return m_fIMMEditPhoneNum(hParentHwnd,hWndEdit);
	else
		return Cancel;
}

// IMMRESULT CIMMMng::IMMGetResult()
// {
// 	IMMRESULT res;
// 	if(m_bSucceed == TRUE)
// 		return m_fGetReslut();
// 	else
// 		res.dt = Error;
// 		return res;
//}

IMMRESULT CIMMMng::IMMGetResult()
{
	IMMRESULT res;
	if(m_bSucceed == TRUE)
		return m_fGetReslut();
	else
		res.dt = Error;
		return res;
}



