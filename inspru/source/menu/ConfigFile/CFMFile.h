#if !defined(AFX_CFMFile_H__33CAFWEFSDAFF856C35C__INCLUDED_)
#define AFX_CFMFile_H__33CAFWEFSDAFF856C35C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CIniFile.h"

class CFMFile : public CIniFile
{
public:
	int		GetFMCount();
	void	RemoveFM(int nIndex);
	int		GetFM(int nIndex);
	int		AddFM(int nFrequant);
};

#endif // !defined(AFX_CFMFile_H__33CAFWEFSDAFF856C35C__INCLUDED_)
