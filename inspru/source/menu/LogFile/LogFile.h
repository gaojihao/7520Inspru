// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LGOFILE_H
#define _LGOFILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogFile  
{
public:
	CLogFile(CString strLogFile);
	virtual ~CLogFile();

public:
	void	Append(CString strLog);		//追加日志

private:
	CString m_strLogFile;	//日志文件名称
};

#endif // #ifndef _LGOFILE_H
