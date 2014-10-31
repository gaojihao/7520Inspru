// IMMMng.h: interface for the CIMMMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMMMNG_H__7FA7E9CE_CD3F_45A6_A500_03FB8BF7E697__INCLUDED_)
#define AFX_IMMMNG_H__7FA7E9CE_CD3F_45A6_A500_03FB8BF7E697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InputShl.h"
#include "../SysConfig/SysConfig.h"
class CIMMMng  
{
public:
	CIMMMng();
	virtual ~CIMMMng();
	static CIMMMng& Instance();
	static CIMMMng  _IMMMng;
	
	BOOL Load();

	void IMMInit(ScreenSize nSize);
	
	void      ImmSetKeySound(enKeySound enkey);
	ImmReturn IMMCreateMessage(enLanguage enlang);
	ImmReturn IMMEditMessage(enLanguage enlang, LPTSTR strMessage);

	ImmReturn IMMAddPhone(enLanguage enLang);
	ImmReturn IMMEditPhone(enLanguage enlang, LPTSTR strPhone, enKeySound enkey,enSkinType enSkin);

	ImmReturn IMMAddPhoneNum(HWND hParentHwnd, HWND hWndEdit);
	ImmReturn IMMEditPhoneNum(HWND hParentHwnd, HWND hWndEdit);

	IMMRESULT IMMGetResult();
private:
	INIT				m_fIMMInit; 				   
	IMMCREATEMESSAGE	m_fIMMCreateMsg;
	IMMEDITMESSAGE		m_fIMMEditMsg;	
	IMMEDITPHONE		m_fIMMEditPhone;	
	IMMADDPHONE			m_fIMMAddPhone;	
	IMMADDPHONENUM		m_fIMMAddPhoneNum;
	IMMEDITPHONENUM		m_fIMMEditPhoneNum;	
	GETRESULT			m_fGetReslut;
	IMMSETKEYSOUND      m_fSetKeySound;
	HINSTANCE           m_hInst;
	BOOL                m_bSucceed;
};

#endif // !defined(AFX_IMMMNG_H__7FA7E9CE_CD3F_45A6_A500_03FB8BF7E697__INCLUDED_)
