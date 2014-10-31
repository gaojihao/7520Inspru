
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFMFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


int CFMFile::GetFMCount()
{
	return m_FileContainer.GetSize();
}

void CFMFile::RemoveFM(int nIndex)
{
	m_FileContainer.RemoveAt(nIndex);
}

int CFMFile::GetFM(int nIndex)
{
	CString str = m_FileContainer.GetAt(nIndex);
	return _ttoi(str.GetBuffer(str.GetLength()));
}
//返回插入频率的index(0~size)
int	CFMFile::AddFM(int nFrequant)
{
	CString str;
	str.Format(_T("%d"), nFrequant);
	for(int i=0; i<m_FileContainer.GetSize(); i++)
	{
		int nCurrFrequant = GetFM(i);
		if( nFrequant < nCurrFrequant )
		{
			m_FileContainer.InsertAt(i, str);
			return i;
		}
		else if(nFrequant == nCurrFrequant)
		{
			return i;
		}
	}
	m_FileContainer.Add(str);
	return m_FileContainer.GetSize()-1;
}
