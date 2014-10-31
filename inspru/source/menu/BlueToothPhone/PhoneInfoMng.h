// PhoneInfoMng.h: interface for the PhoneInfoMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PHONE_INFO_MNG)
#define _PHONE_INFO_MNG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PhoneInfo.h"

class PhoneInfoMng  
{
public:
	PhoneInfoMng();
	virtual ~PhoneInfoMng();

	static PhoneInfoMng& Instance();
	static PhoneInfoMng _PhoneInfo;

	BOOL Load();

	CString FindPhoneNum(CString csNum);

	CObArray m_ObArr;
	UINT m_Cnt;

	CPhoneInfo* m_PhoneInfo;

};

#endif // !defined(AFX_PHONEBOOKMNG_H__9D89E808_17F1_443E_93FB_1846D44F059B__INCLUDED_)
