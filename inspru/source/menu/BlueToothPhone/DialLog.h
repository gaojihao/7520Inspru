// DialLog.h: interface for the CDialLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALLOG_H__E698DA06_29CF_4232_A0D7_7CB8B7523EE5__INCLUDED_)
#define AFX_DIALLOG_H__E698DA06_29CF_4232_A0D7_7CB8B7523EE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSkinResDef.h"

enum enDialType
{
	Dial_Out = bmp_file_BtnDiaOut,
	Dial_In = bmp_file_BtnDiaIn,
	Dial_Shut = bmp_file_BtnDiaNotDia
};

class CDialLog  : public CObject
{
public:
	CDialLog();
	virtual ~CDialLog();

	virtual void Serialize( CArchive& ar );

	UINT m_iLogFlag;
	CString m_csName;
	CString m_csNum;
	CString m_csCity;
	CString m_csDate;
	CString m_csTime;

DECLARE_SERIAL(CDialLog)
};

#endif // !defined(AFX_DIALLOG_H__E698DA06_29CF_4232_A0D7_7CB8B7523EE5__INCLUDED_)
