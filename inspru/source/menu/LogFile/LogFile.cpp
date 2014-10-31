// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile(CString strLogFile)
{
	m_strLogFile = strLogFile;
}

CLogFile::~CLogFile()
{

}
//////////////////////////////////////////////
// Purpose:	追加日志
// Para:	strLog	-	日志内容
//
void CLogFile::Append(CString strLog)
{
	if( strLog.IsEmpty() )
		return;

	//写文件
	CFile LogFile;
	BOOL bOpen = LogFile.Open( m_strLogFile, 
		CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );
	
	if (!bOpen)
		return;

	CString strHour,strMinute,strSecond;
	CTime time = CTime::GetCurrentTime();

	strHour.Format( L"%02d:",time.GetHour());
	strMinute.Format( L"%02d:",time.GetMinute());
	strSecond.Format( L"%02d\t",time.GetSecond());

	CString strTime = strHour+strMinute+strSecond;
	strLog = strTime + strLog + _T("\r\n");

	int nLen = WideCharToMultiByte(
				CP_ACP,
				0, 
				strLog.GetBuffer(strLog.GetLength()),
				-1,
				NULL,
				0,
				NULL,
				NULL
				);

	char * szLog = new char[nLen+1];
	ZeroMemory( szLog, nLen );
	WideCharToMultiByte(  //转换Unicode到Ansi
		CP_ACP,
		WC_COMPOSITECHECK | WC_DEFAULTCHAR,
		strLog.GetBuffer(strLog.GetLength()),
		strLog.GetLength(),
		szLog,  //转换到缓冲区中
		nLen,  //最多字节数
		0,
		0
		);

	LogFile.SeekToEnd();
	LogFile.Write( szLog, nLen );

	LogFile.Close();
}