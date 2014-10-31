
#if !defined(AFX_CGDICommon_H__4FFASDFWRGBNHGNFDZS0B__INCLUDED_)
#define AFX_CGDICommon_H__4FFASDFWRGBNHGNFDZS0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
  
class CGDICommon
{
public:
	CGDICommon();
	~CGDICommon();

	static CGDICommon* Instance();

	CFont&		bigbigfont();
	CFont&		bigfont();
	CFont&		smallfont();
	COLORREF	color();
	UINT		format();
	
private:
	static CGDICommon	m_gdicommon;

	CFont				m_bigbigfont;
	CFont				m_bigfont;
	CFont				m_smallfont;
};

#endif // !defined(AFX_CGDICommon_H__4FFASDFWRGBNHGNFDZS0B__INCLUDED_)
