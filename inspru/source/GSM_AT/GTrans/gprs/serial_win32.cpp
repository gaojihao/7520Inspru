
#include "stdafx.h"
#include "serial_win32.h"


#define SERIAL_READBUFLEN		2048
#define SERIAL_WRITEBUFLEN		2048

serial_win32::serial_win32()
{
	m_device = 0;
	m_state = dev_close;
}

//////////////////////////////////////////////////////////////////////////


serial_win32::~serial_win32()
{
	close();
}

bool  serial_win32::open(int port, long baud, char parity /*=NOPARITY*/, char databits /*=8*/, char stopbit /*=ONESTOPBIT*/ )
{

	if (dev_close != m_state )	//如果已经打开，要关闭重新打开
	{
		if ( !close() )
			return false;
	}

	m_state = dev_error;


#if defined(UNICODE) || defined(_UNICODE)
	wchar_t  sz_dev[32];
	wsprintf(sz_dev, L"COM%d:\0", port );
	m_device= CreateFile(sz_dev, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
						FILE_FLAG_WRITE_THROUGH, NULL );	
#else
	char  sz_dev[32];
	sprintf(sz_dev, "\\\\.\\COM%d:\0", port );
	m_device= CreateFile(sz_dev, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
						FILE_FLAG_WRITE_THROUGH, NULL );
#endif

	if ( m_device == INVALID_HANDLE_VALUE)
	{
		int err = GetLastError();
		return false;
	}

#if 0
	// set buffer size
	if (!SetupComm(m_device, SERIAL_READBUFLEN, SERIAL_WRITEBUFLEN))//设置缓冲区
	{
		CloseHandle(m_device);
		m_device = 0;
		return false;
	}
#endif

	
	COMMTIMEOUTS com_times;							//超时结构，时间单位：毫秒
	com_times.ReadIntervalTimeout		  = 100;	//两字符之间最大的延时
	com_times.ReadTotalTimeoutMultiplier  = 1;      //读取每字符间的超时
	com_times.ReadTotalTimeoutConstant	  = 500;	//一次读取串口数据的固定超时
	com_times.WriteTotalTimeoutMultiplier = 1;  	//写入每字符间的超时
	com_times.WriteTotalTimeoutConstant	  = 100;	//一次写入串口数据的固定超时

	
	if (!SetCommTimeouts(m_device, &com_times) )
	{
		CloseHandle( m_device );
		m_device = 0;
		return false;
	}

	DCB dcb;
	GetCommState(m_device, &dcb);
	dcb.BaudRate = baud;
	dcb.Parity	 = parity;
	dcb.ByteSize = databits;
	dcb.StopBits = stopbit;
	dcb.fNull	 =	FALSE;
	dcb.fAbortOnError =	FALSE;
		
	if (!SetCommState(m_device, &dcb) ||
		!SetCommMask(m_device, EV_BREAK | EV_ERR | EV_RXCHAR))// 设置串口参数
	{
		CloseHandle(m_device);
		m_device = 0;
		return false;
	}

	//清除串口数据
	PurgeComm( m_device, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	m_state = dev_open;
 
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool serial_win32::close()
{
	if (m_state == dev_close)
		return true;

	if (!CloseHandle(m_device))
	{
		m_state = dev_error;
		return FALSE;
	}
	m_device = NULL;
	m_state = dev_close;

	// waiting for watch thread.....
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool serial_win32::write_data( const char* p_data, int n_count)
{
	unsigned long  n_write = 0;
	if ( WriteFile((HANDLE)m_device, p_data, n_count, &n_write,NULL) )
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
int  serial_win32::read_data( char* p_buf, int n_count )
{
	unsigned long n_read = 0;
	ReadFile( m_device, p_buf, n_count, &n_read, 0);

	return n_read;	
}

//////////////////////////////////////////////////////////////////////////
unsigned   serial_win32::get_state()
{
	return m_state;
}

//////////////////////////////////////////////////////////////////////////
bool  serial_win32::clear_in_buffer()
{
	if ( PurgeComm(m_device, PURGE_RXCLEAR) )
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////

void   serial_win32::set_event(unsigned n_event)
{
	::SetCommMask(m_device, EV_RXCHAR);
}

//////////////////////////////////////////////////////////////////////////

unsigned long  serial_win32::wait_event()
{
	unsigned long l_event = 0;
	::WaitCommEvent(m_device, &l_event, 0 );
	return l_event;
}