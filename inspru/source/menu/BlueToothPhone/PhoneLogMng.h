// PhoneLogMng.h: interface for the PhoneLogMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONELOGMNG_H__8E0080B8_099E_4896_B358_38660BD7E824__INCLUDED_)
#define AFX_PHONELOGMNG_H__8E0080B8_099E_4896_B358_38660BD7E824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialLog.h"

class PhoneLogMng  
{
public:
	PhoneLogMng();
	virtual ~PhoneLogMng();

	static PhoneLogMng& Instance();
	static PhoneLogMng _PhoneLog;

	BOOL Load();

	BOOL AddLog( CDialLog* );

	BOOL Write();
	BOOL WriteNull();
	CObArray* GetPhoneLog();

	CDialLog* GetCurPhoneLogItem();
	void SetCurPhoneLogItem(UINT Pos);
	void SetLPhoneLogState(UINT state);
	UINT GetCount();
	void DelLPhoneLogState(UINT state);
	BOOL LoadAll();

	BOOL Delete( UINT ipos );

	BOOL Delete ( CDialLog* );

	BOOL DelAll();

	UINT m_Cnt;
	UINT tempCnt;

	CObArray m_ObArr;

	CDialLog* m_pDialLog;

	UINT m_CurPos;

	UINT m_PhoneLogState;

};

#endif // !defined(AFX_PHONELOGMNG_H__8E0080B8_099E_4896_B358_38660BD7E824__INCLUDED_)
