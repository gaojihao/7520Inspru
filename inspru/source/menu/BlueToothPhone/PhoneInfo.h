// PhoneBookFile1.h: interface for the CPhoneInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PHONE_INFO)
#define _PHONE_INFO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPhoneInfo : public CObject
{
public:
	CPhoneInfo();
	CPhoneInfo( CString csName, CString csNum );
	virtual ~CPhoneInfo();
	void SetValue(CString csName, CString csNum);

	virtual void Serialize( CArchive& ar );

	CString m_csName;
	CString m_csNum;


DECLARE_SERIAL(CPhoneInfo)

};

#endif // !defined(AFX_PHONEBOOKFILE1_H__AFF76BE5_C411_4D37_9578_59629A4B0460__INCLUDED_)
