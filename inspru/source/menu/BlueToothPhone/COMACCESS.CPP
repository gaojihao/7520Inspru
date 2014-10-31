////////////////////////////////////////////////////////////////////////////////////////
// 
//  Project:    Class ComAccess
//              Overlapped serial IO communication class
//  System:     Win9x WinNT
//  File:       comaccess.cpp
//  Start date:	07.17.2007
//  Update:     
//  Version:    1.0
//  Author:     sunwave
//  Copyright
//
////////////////////////////////////////////////////////////////////////////////////////
#include "comaccess.h"
#include <stdio.h>     // for printf()

extern TCHAR       m_lpszErrorMessage[256];
extern HANDLE ReceDoneEvent;
HANDLE g_hcom;
////////////////////////////////////////////////////////////////////////////////////////
// 
//  Constructors
//

ComAccess::ComAccess(VOID)
{
	m_hCom = 0;
	//m_lpszErrorMessage[0] = '\0';
	ZeroMemory(&m_ov, sizeof(m_ov));
}


ComAccess::ComAccess(PCTSTR lpszPortNum)
{ 
	ComAccess::ComAccess();
	ComAccess::Open(lpszPortNum); 
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:      Open(LPCSTR lpszPortNum,
//                      DWORD  dwBaudRate, 
//                      BYTE   byParity,
//                      BYTE   byStopBits,
//                      BYTE   byByteSize)
//
//  Return value:  BOOL TRUE or FALSE
//

BOOL ComAccess::Open(PCTSTR lpszPortNum, 
                     DWORD  dwBaudRate, 
                     BYTE   byParity,
                     BYTE   byStopBits,
					 BYTE   byByteSize)
{
	DCB  dcb; // structure that defines the control setting for a serial communications device
	BOOL bSuccess;
	COMMTIMEOUTS CommTimeouts;
	
	m_hCom = CreateFile(lpszPortNum,           // pointer to name of the file
	                    GENERIC_READ|GENERIC_WRITE, // access mode
	                    0,                     // comm devices must be opened w/exclusive-access 
	                    NULL,                  // no security attributs 
	                    OPEN_EXISTING,         // comm devices must use OPEN_EXISTING 
	                    NULL,  // overlapped I/O
	                    NULL);                 // hTemplate must be NULL for comm devices 

	if ( m_hCom == INVALID_HANDLE_VALUE ) 
	{
		// handle the error
		ComAccess::ErrorToString(TEXT("Open(): CreateFile() failed, invalid handle value"));
		
		return FALSE;
	}

	//
	// Omit the call to SetupComm to use the default queue sizes.
	// Get the current configuration.
	//
 
	bSuccess = GetCommState(m_hCom, &dcb);

	if ( ! bSuccess ) 
	{
		// Handle the error.
		ComAccess::ErrorToString(TEXT("Open(): GetCommState() failed"));
		ComAccess::Close();

		return FALSE;
	}

	//
	// Fill in the DCB: baud=115200, 8 data bits, no parity, 1 stop bit are default parameters
	//

	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = byByteSize;
	dcb.Parity   = byParity;
	dcb.StopBits = byStopBits;
	
	bSuccess = SetCommState(m_hCom, &dcb);

	if ( ! bSuccess ) 
	{
		// Handle the error. 
		ComAccess::ErrorToString(TEXT("Open(): SetCommState() failed"));
		ComAccess::Close(); 

		return FALSE;
	}
	// ++++++added by sunwave
	//GetCommTimeouts(m_hCom, CommTimeout);
	CommTimeouts.ReadIntervalTimeout =    500;
	CommTimeouts.ReadTotalTimeoutMultiplier = 200;
	CommTimeouts.ReadTotalTimeoutConstant = 5000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 200;
	CommTimeouts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts(m_hCom, &CommTimeouts);

	SetupComm(m_hCom, 1024, 1024);
	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); //清干净输入、输出缓冲区
	// ++++++end

	//printf("open COM1 successful!\n");
	g_hcom = m_hCom;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:      Close(VOID)
//
//  Return value:  VOID
//

VOID ComAccess::Close(VOID)
{
	
	SetEvent(ReceDoneEvent);
	CloseHandle(ReceDoneEvent);
	if ( m_hCom > 0 )
	{
		CloseHandle(m_hCom);
	}
	

	m_hCom = 0; 
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:     WriteData(LPCVOID pdata, 
//                          DWORD   len)
//
//  Return value:  DWORD -1 failed, above, num written bytes
//

DWORD ComAccess::WriteData(LPCVOID pdata, 
                           DWORD   len)
{
	BOOL  bSuccess;
	DWORD written = 0;

	if ( len < 1 )
		return(0);
	// ++++++add by sunwave
	//FlushFileBuffers(m_hCom);
	// ++++++end
	// create event for overlapped I/O
// 	m_ov.hEvent = CreateEvent( NULL,   // pointer to security attributes 
// 	                          FALSE,   // flag for manual-reset event 
// 	                          FALSE,  // flag for initial state 
// 	                          _T("") );    // pointer to event-object name 
// 
// 	if ( m_ov.hEvent == INVALID_HANDLE_VALUE )
// 	{
// 		// Handle the error.
// 		ComAccess::ErrorToString(TEXT("WriteData(): CreateEvent() failed"));
// 	  
// 		return(-1);
//	}

	bSuccess = WriteFile(m_hCom,   // handle to file to write to  
	                     pdata,    // pointer to data to write to file 
	                     len,      // number of bytes to write 
	                     &written, // pointer to number of bytes written 
	                     NULL);   // pointer to structure needed for overlapped I/O


	/*if ( ComAccess::IsNT() )
	{
		bSuccess = GetOverlappedResult(m_hCom, &m_ov, &written, TRUE);

		if ( ! bSuccess )
		{
			// Handle the error.
			CloseHandle(m_ov.hEvent);
			ComAccess::ErrorToString("WriteData(): GetOverlappedResult() failed");

			return(-1);
		}
	}
	else*/
// 	if ( len != written )
// 	{
// 		// Handle the error.
// 		CloseHandle(m_ov.hEvent);
// 		ComAccess::ErrorToString(TEXT("WriteData(): WriteFile() failed"));
// 
// 		return(-1);
// 	}
// 
//	CloseHandle(m_ov.hEvent);

	return written;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:      ReadData(LPVOID pdest, 
//	                        DWORD  len, 
//                          DWORD  dwMaxWait)
//
//  Return value:  DWORD -1 failed, above, num read bytes		
//

DWORD ComAccess::ReadData(LPVOID pdest, 
                          DWORD  len, 
                          DWORD  dwMaxWait)
{
	BOOL  bSuccess;
	DWORD result = 0,
	      read   = 0, // num read bytes
	      mask   = 0; // a 32-bit variable that receives a mask 
	                  // indicating the type of event that occurred

	if ( len < 1 ) return(0);

	// ++++++add by sunwave
	//FlushFileBuffers(m_hCom);
	// ++++++end
	// create event for overlapped I/O

// 	m_ov.hEvent = CreateEvent(NULL,   // pointer to security attributes 
// 	                          FALSE,   // flag for manual-reset event 
// 	                          FALSE,  // flag for initial state 
// 	                          _T("") );    // pointer to event-object name 
// 
// 	if ( m_ov.hEvent == INVALID_HANDLE_VALUE )
// 	{
// 		// Handle the error.
// 		ComAccess::ErrorToString(TEXT("ReadData(): CreateEvent() failed"));
// 	  
// 		return(-1);
//	}

	// Specify here the event to be enabled

	/*bSuccess = SetCommMask(m_hCom, EV_RXCHAR);

	if ( ! bSuccess )
	{
		// Handle the error.
		CloseHandle(m_ov.hEvent);
		ComAccess::ErrorToString("ReadData(): SetCommMask() failed");
	  
		return(-1);
	}*/

	
	// WaitForSingleObject

	/*bSuccess = WaitCommEvent(m_hCom, &mask, &m_ov);

	if ( ! bSuccess )
	{
		int err = GetLastError();

		if ( err == ERROR_IO_PENDING)
		{
			result = WaitForSingleObject(m_ov.hEvent, dwMaxWait);  //wait dwMaxWait
		                                        // milli seconds before returning
			if (result == WAIT_FAILED)
			{
				// Handle the error.
				CloseHandle(m_ov.hEvent);
				ComAccess::ErrorToString("ReadData(): WaitForSingleObject() failed");
	  
				return(-1);
			}
		}
	}*/
	
	// The specified event occured?
	
	//if ( mask & EV_RXCHAR) 
	//{
		bSuccess = ReadFile(m_hCom, // handle of file to read 
			                pdest,  // address of buffer that receives data 
				            len,    // number of bytes to read 
					        &read,  // address of number of bytes read 
						    NULL); // address of structure for data 

		/*if ( ComAccess::IsNT() )
		{
			bSuccess = GetOverlappedResult(m_hCom, &m_ov, &read, TRUE);

			if ( ! bSuccess )
			{
				// Handle the error.
				CloseHandle(m_ov.hEvent);
				ComAccess::ErrorToString("WriteData(): GetOverlappedResult() failed");

				return(-1);
			}
		}
		else*/
// 		if ( ! bSuccess )
// 		{
// 			// Handle the error.
// 			CloseHandle(m_ov.hEvent);
// 			ComAccess::ErrorToString(TEXT("ReadData(): ReadFile() failed"));
// 
// 			return(-1);
//		}
	//}
	/*else
	{
		// Handle the error.
		printf("mask = %d\n", mask);
		CloseHandle(m_ov.hEvent);
		wsprintf(m_lpszErrorMessage, "Error ReadData(): No EV_RXCHAR occured\n");

		return(-1);
	}*/
	
//	CloseHandle(m_ov.hEvent);
	
	return read;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:      ErrorToString(LPCSTR lpszMessage)
//
//  Return value:  VOID
//

VOID ComAccess::ErrorToString(PCTSTR lpszMessage)
{
	LPVOID lpMessageBuffer;
	DWORD  error = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
	              FORMAT_MESSAGE_FROM_SYSTEM,      // source and processing options
	              NULL,                            // pointer to message source
	              error,                           // requested message identifie
	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // the user default language.
	              (LPTSTR) &lpMessageBuffer,       // pointer to message buffer
	              0,                               // maximum size of message buffer
	              NULL);                           // address of array of message inserts 

	// and copy it in our error string
	wsprintf(m_lpszErrorMessage,TEXT("%s: (%d) %s\r\n"), lpszMessage, error, lpMessageBuffer);

	LocalFree(lpMessageBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:     IsNT(VOID)
//
//  Return value: BOOL True or False
//

BOOL ComAccess::IsNT(VOID)
{
    OSVERSIONINFO osvi;
    
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    GetVersionEx(&osvi);

    if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
