// PhoneBookFile1.h: interface for the CPhoneBookFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONEBOOKFILE1_H__AFF76BE5_C411_4D37_9578_59629A4B0460__INCLUDED_)
#define AFX_PHONEBOOKFILE1_H__AFF76BE5_C411_4D37_9578_59629A4B0460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPhoneBookFile : public CObject
{
public:
	CPhoneBookFile();
	CPhoneBookFile( CString csName, CString csNum ,CString csCity);
	virtual ~CPhoneBookFile();
	void SetValue(CString csName, CString csNum,CString csCity);

	virtual void Serialize( CArchive& ar );

	CString m_csName;
	CString m_csNum;
	CString m_csCity;

DECLARE_SERIAL(CPhoneBookFile)

};

#endif // !defined(AFX_PHONEBOOKFILE1_H__AFF76BE5_C411_4D37_9578_59629A4B0460__INCLUDED_)
