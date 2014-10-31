// PhoneBookMng.h: interface for the PhoneBookMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONEBOOKMNG_H__9D89E808_17F1_443E_93FB_1846D44F059B__INCLUDED_)
#define AFX_PHONEBOOKMNG_H__9D89E808_17F1_443E_93FB_1846D44F059B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PhoneBookFile.h"


class PhoneBookMng  
{
public:
	PhoneBookMng();
	virtual ~PhoneBookMng();

	static PhoneBookMng& Instance();
	static PhoneBookMng _PhoneBook;
	

	BOOL Load();

	BOOL AddPhone( CPhoneBookFile* );
	BOOL AddLastPhone( CPhoneBookFile* );
	BOOL DeleteLastPhone();
	BOOL Write();
	BOOL WriteNull();
	BOOL DelAll();
	CObArray* GetPhoneBook();

	CPhoneBookFile* SearchPhoneItem(CString& strPhone);
	CPhoneBookFile* GetCurPhoneBookItem();
	CPhoneBookFile* GetPhoneBookItem(UINT pos);
	void SetCurPhoneBookItem(UINT Pos);

	void ReSetCurItem( CPhoneBookFile* pItem );

	UINT GetCount();

	BOOL Delete( UINT ipos );

	BOOL Delete ( CPhoneBookFile* );

	UINT m_Cnt;

	CObArray m_ObArr;

	CPhoneBookFile* m_pPhoneBookFile;

	UINT m_CurPos;

};

#endif // !defined(AFX_PHONEBOOKMNG_H__9D89E808_17F1_443E_93FB_1846D44F059B__INCLUDED_)
