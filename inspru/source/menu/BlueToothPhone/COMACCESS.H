////////////////////////////////////////////////////////////////////////////////////////
// 
//	Project:    Class ComAccess
//              Overlapped serial IO communication class
//  System:     Win9x WinNT
//	File:       comaccess.h
//	Start date:	04.19.2007
//	Update:     
//	Version:    1.0
//	Author:     sunwave
//	Copyright ?1997. Alle Rechte vorbehalten
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COMACCESS_H_
#define _COMACCESS_H_


#include <windows.h>


class ComAccess 
{
private:

	HANDLE      m_hCom; // Device handle 

	OVERLAPPED  m_ov;   // A structure that contains informations which are
	                    // used for a synchronous input and output operations

	//TCHAR       m_lpszErrorMessage[256];

public:

	ComAccess(VOID);
	ComAccess(PCTSTR lpszPortNum);

	~ComAccess() { Close(); }
			                        // For more definitions see <winbase.h>
	BOOL	Open(PCTSTR lpszPortNum	= _T("COM2:"),
			     DWORD  dwBaudRate  = CBR_115200, 
			     BYTE   byParity    = NOPARITY,
			     BYTE   byStopBits  = ONESTOPBIT,
			     BYTE   byByteSize  = 8);

	VOID	Close(VOID);
	
	DWORD	WriteData(LPCVOID pdata, DWORD len);
	DWORD	ReadData(LPVOID  pdest, DWORD len, DWORD dwMaxWait = 1000);

//	PTSTR	GetErrorMessage(VOID) { return m_lpszErrorMessage; }

private:

	VOID	ErrorToString(PCTSTR lpszMessage);

	BOOL	IsNT(VOID);
};


#endif // _COMACCESS_H_
