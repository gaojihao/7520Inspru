  /*******************************************************************/
/*
串口基础类库

定义
cnComm MyComm1(false, 0);//第1个参数为是否启动监视线程， 第2个参数为阻塞（0）/异步方式（默认）
cnComm MyComm2;//默认启动监视线程， 异步
cnComm MyComm3(true);//监视线程， 阻塞

打开
MyComm1.Open(1);//1-1024  2k支持1024串口
MyComm1.Open(1, 1200);
MyComm1.Open(1, 9600);
MyComm1.Open(1, "9600,8,n,1");//可使用标准的设置字符串
MyComm1.SetBufferSize(1200, 4800)//设置缓冲区大小,可不用设置
MyComm1.SetState("9600,8,n,1")//修改波特率等


读写

MyComm1.Read(buf, 1000);
MyComm1.Write(buf, 200);


窗口句柄
MyComm1.SetWnd(hWnd);

线程控制下面都有注明




				卢海

				2005 - 12 -08
*/
/*******************************************************************/


#ifndef	_SERIAL_H_
#define _SERIAL_H_

#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>


//当接受到数据送到窗口的消息
#define ON_COM_RECEIVE WM_USER + 618	 //	 WPARAM 端口号

class CSerial	  
{
public:

	//构造函数，fAutoBeginThread是否启动读线程
	CSerial(bool fAutoBeginThread = false)
		: _fAutoBeginThread(fAutoBeginThread)
	{
		_dwIOMode = FILE_FLAG_OVERLAPPED;
		Init();	
	}
	
	//析构函数
	virtual ~CSerial()
	{
		Close();	
		UnInit();
	}

	//打开串口 缺省 9600, 8, n, 1
	inline bool Open(DWORD dwPort)
	{
		return Open(dwPort, 9600);
	}
	//打开串口 缺省 baud_rate, 8, n, 1
	inline bool Open(DWORD dwPort, DWORD dwBaudRate)
	{
		if(dwPort < 1 || dwPort > 1024)
			return false;

		BindCommPort(dwPort);

		if(!OpenCommPort())
			return false;

		if(!SetupPort())
			return false;

		return SetState(dwBaudRate);
	}
	//打开串口, 使用类似"9600, 8, n, 1"的设置字符串设置串口
	inline bool Open(DWORD dwPort, char *szSetStr)
	{
		if(dwPort < 1 || dwPort > 1024)
			return false;

		BindCommPort(dwPort);

		if(!OpenCommPort())
			return false;

		if(!SetupPort())
			return false;

		return SetState(szSetStr);
	}
	//判断串口是或打开
	inline bool IsOpen()
	{
		return _hCommHandle != INVALID_HANDLE_VALUE;
	}
	//获得串口句炳
	inline HANDLE GetHandle()
	{
		return _hCommHandle;
	}
	//设置串口参数：波特率，停止位，等 支持设置字符串 "9600, 8, n, 1"
	bool SetState(char *szSetStr)	
	{
		if(IsOpen())
		{
			if(!GetCommState(_hCommHandle, &_DCB))
				return false;
//			if(!::BuildCommDCB(szSetStr, &_DCB))
// 				return false;
			return SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}
	//设置串口参数：波特率，停止位
	bool SetState(DWORD dwBaudRate, DWORD dwByteSize = 8, DWORD dwParity = NOPARITY, DWORD dwStopBits = ONESTOPBIT)
	{
		if(IsOpen())
		{
			if(!::GetCommState(_hCommHandle, &_DCB))
				return false;
//			_DCB.BaudRate = dwBaudRate;
//		    _DCB.ByteSize = dwByteSize;
//		    _DCB.Parity   = dwParity;
// 			_DCB.StopBits = dwStopBits;

				//改变DCB结构设置
			_DCB.BaudRate = dwBaudRate;               //波特率 
			_DCB.fBinary = TRUE;                 //Win32不支持非二进制串行传输模式，必须为TRUE 
			_DCB.fParity = TRUE;                 //启用奇偶校验 

			_DCB.fOutxCtsFlow = FALSE;            //串行端口的输出由CTS线控制
			_DCB.fOutxDsrFlow = FALSE;           //关闭串行端口的DSR流控制 
			_DCB.fDtrControl = DTR_CONTROL_ENABLE;   //启用DTR线
			_DCB.fDsrSensitivity = FALSE;        //如果设为TRUE将忽略任何输入的字节，除非DSR线被启用 
			//PortDCB.fTXContinueOnXoff = TRUE;       //当为TRUE时，如果接收缓冲区已满且驱动程序已传送XOFF字符，将使驱动程序停止传输字符
			_DCB.fTXContinueOnXoff = TRUE;
			_DCB.fOutX = FALSE;                  //设为TRUE指定XON/XOFF控制被用于控制串行输出 
			_DCB.fInX = FALSE;                   //设为TRUE指定XON/XOFF控制被用于控制串行输入 
			_DCB.fErrorChar = FALSE;             //WINCE串行驱动程序的默认执行将忽略这个字段 
			_DCB.fNull = FALSE;                  //设为TRUE将使串行驱动程序忽略收到的空字节 
			_DCB.fRtsControl = RTS_CONTROL_ENABLE;   //启用RTS线 
			_DCB.fAbortOnError = FALSE;          //WINCE串行驱动程序的默认执行将忽略这个字段
			_DCB.ByteSize = 8;                   //每字节的位数 
			_DCB.Parity = NOPARITY;              //无奇偶校验 
			_DCB.StopBits = ONESTOPBIT;          //每字节一位停止位 

	
			return ::SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}
	//设置串口的I/O缓冲区大小
	bool SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize)
	{
		if(IsOpen())
			return ::SetupComm(_hCommHandle, dwInputSize, dwOutputSize);
		return false;	
	}
	//读取串口 dwBufferLength - 1 个字符到 szBuffer 返回实际读到的字符数
	DWORD Read(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 50)
	{
		if(!IsOpen())
			return 0;

//		szBuffer[0] = '\0';
//
//		COMSTAT  Stat;
//		DWORD dwError;
//
//		if(::ClearCommError(_hCommHandle, &dwError, &Stat) && dwError > 0)	//清除错误
//		{
//			::PurgeComm(_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR); /*清除输入缓冲区*/
//			return 0;
//		}
//
//		if(!Stat.cbInQue)// 缓冲区无数据
//			return 0;
//
//		unsigned long uReadLength = 0;
//
//		dwBufferLength = dwBufferLength - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufferLength - 1;
//
//		if(!::ReadFile(_hCommHandle, szBuffer, dwBufferLength, &uReadLength, NULL)) //2000 下 ReadFile 始终返回 True
//		{
//			if(::GetLastError() == ERROR_IO_PENDING) // 结束异步I/O
//			{
//				WaitForSingleObject(_ReadOverlapped.hEvent, dwWaitTime);	//等待20ms
//			}
//			else
//				uReadLength = 0;
//		}
//			
//		szBuffer[uReadLength] = '\0';
//		return uReadLength;

		DWORD dwRead;
		DWORD dwLeft=dwBufferLength;
		DWORD dwLength=0;
		LPVOID lpBegin=szBuffer;

		while(dwLeft>0)
		{
			::ReadFile(_hCommHandle,lpBegin,dwLeft,&dwRead,NULL);

			dwLeft-=dwRead;
			dwLength+=dwRead;

			lpBegin=(char*)szBuffer+dwLength;

			if(dwRead==0 && dwLength>0)
				break;

		} 
		return (UINT)(dwBufferLength-dwLeft);
	}
	//写串口 szBuffer
	DWORD Write(char *szBuffer, DWORD dwBufferLength)
	{
		if(!IsOpen())
			return 0;
		
		DWORD dwError;

		if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//清除错误
			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);	

		unsigned long uWriteLength = 0;

		if(!::WriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, &_WriteOverlapped))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;

		return uWriteLength;
	}
	//写串口 szBuffer
	inline DWORD Write(char *szBuffer)
	{
		assert(szBuffer);

		return Write(szBuffer, strlen(szBuffer));
	}
	//强制同步写
	inline DWORD WriteSync(char *szBuffer, DWORD dwBufferLength)
	{
		if(!IsOpen())
			return 0;
		
//		DWORD dwError;
//
//		if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//清除错误
//			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);	

		unsigned long uWriteLength = 0;

		if(!::WriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, NULL))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;

		return uWriteLength;
	}
	//写串口 szBuffer 可以输出格式字符串
	DWORD Write(char *szBuffer, DWORD dwBufferLength, char * szFormat, ...)
	{
		if(!IsOpen())
			return 0;
		
		DWORD dwError;

		if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//清除错误
			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);	

		va_list va;
		va_start(va, szFormat);
		_vsnprintf(szBuffer, dwBufferLength, szFormat, va);
		va_end(va);

		unsigned long uWriteLength = 0;

		if(!::WriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, &_WriteOverlapped))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;

		return uWriteLength;
	}
	//关闭串口
	inline virtual void Close()
	{
		//清除驱动程序内部的发送和接收队列
		PurgeComm(_hCommHandle,PURGE_TXCLEAR|PURGE_RXCLEAR);
		
		//清除端口上指定信号的状态
		EscapeCommFunction(_hCommHandle,CLRDTR);
		EscapeCommFunction(_hCommHandle,CLRRTS);
		if(IsOpen())		
		{
			EndThread();
			::CloseHandle(_hCommHandle);

			_hCommHandle = INVALID_HANDLE_VALUE;
		}
	}
	//设定发送通知, 接受字符最小值
	inline void SetNotifyNum(int iNum)
	{
		_dwNotifyNum = iNum;
	}
	//送消息的窗口句柄
	inline void SetWnd(HWND hWnd)
	{
		_hNotifyWnd = hWnd;
	}
	//辅助线程控制 建监视线程
	bool BeginThread() 
	{
		if(!IsThreadRunning()) 
		{
			_fRunFlag = true;
			_hThreadHandle = NULL;

			DWORD id;

			_hThreadHandle = ::CreateThread(NULL, 0, CommThreadProc, this, 0, &id); //兼容98 故使用&ID

			return (_hThreadHandle != NULL); //辅助线程
		}
		return false;
	}
	//线程是否运行
	inline bool IsThreadRunning()
	{
		return _hThreadHandle != NULL;
	}
	//获得线程句柄
	inline HANDLE GetThread()
	{
		return _hThreadHandle;
	}
	//暂停监视线程
	inline bool SuspendThread()
	{
		return IsThreadRunning() ? ::SuspendThread(_hThreadHandle) != 0xFFFFFFFF : false;
	}
	//恢复监视线程
	inline bool ResumeThread()
	{
		return IsThreadRunning() ? ::ResumeThread(_hThreadHandle) != 0xFFFFFFFF : false;
	}
	//终止线程
	bool EndThread(DWORD dwWaitTime = 100)
	{
		if(IsThreadRunning()) 
		{
			_fRunFlag = false;
			::SetCommMask(_hCommHandle, 0);
			::SetEvent(_WaitOverlapped.hEvent);
			if(::WaitForSingleObject(_hThreadHandle, dwWaitTime) != WAIT_OBJECT_0)
				if(!::TerminateThread(_hThreadHandle, 0))
					return false;

			::CloseHandle(_hThreadHandle);
			::ResetEvent(_WaitOverlapped.hEvent);

			_hThreadHandle = NULL;

			return true;
		}
		return false;
	}
protected:

	volatile DWORD _dwPort;	 //串口号
	volatile HANDLE _hCommHandle;//串口句柄
	TCHAR _szCommStr[30];

	DCB _DCB;			  //波特率，停止位，等
	COMMTIMEOUTS _CO;     //超时结构

	DWORD _dwIOMode; // 0 同步  默认 FILE_FLAG_OVERLAPPED 重叠I/O 异步
	OVERLAPPED _ReadOverlapped, _WriteOverlapped; //	重叠I/O

	//线程用
	volatile HANDLE _hThreadHandle;	//辅助线程
	volatile HWND _hNotifyWnd; // 通知窗口
	volatile DWORD _dwNotifyNum;//接受多少字节(>_dwNotifyNum)发送通知消息
	volatile bool _fRunFlag; //线程运行循环标志
	bool _fAutoBeginThread;//Open() 自动 BeginThread();
	OVERLAPPED _WaitOverlapped; //WaitCommEvent use

	//线程收到消息自动调用, 如窗口句柄有效, 送出消息, 包含窗口编号
	virtual void OnReceive()
	{
		if(_hNotifyWnd)
			::PostMessage(_hNotifyWnd, ON_COM_RECEIVE, WPARAM(_dwPort), LPARAM(0));
 	}
	
	void Init()	//初始化
	{
		memset(_szCommStr, 0, 30);
		memset(&_DCB, 0, sizeof(_DCB));
		_DCB.DCBlength = sizeof(_DCB);
		_hCommHandle = INVALID_HANDLE_VALUE;

		memset(&_ReadOverlapped, 0, sizeof(_ReadOverlapped));
		memset(&_WriteOverlapped, 0, sizeof(_WriteOverlapped));

		_ReadOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
		assert(_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE); 
		
		_WriteOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
		assert(_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE);

		_dwNotifyNum = 0;
		_hNotifyWnd = NULL;
		_hThreadHandle = NULL;

		memset(&_WaitOverlapped, 0, sizeof(_WaitOverlapped));
		_WaitOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
		assert(_WaitOverlapped.hEvent != INVALID_HANDLE_VALUE);	
	}		
	void UnInit()
	{
		if(_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(_ReadOverlapped.hEvent);

		if(_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(_WriteOverlapped.hEvent);

		if(_WaitOverlapped.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(_WaitOverlapped.hEvent);
	}
	//绑定串口
	inline void BindCommPort(DWORD dwPort)
	{
		assert(dwPort >= 1 && dwPort <= 1024);

		TCHAR p[5];

		_dwPort = dwPort;
		wcscpy(_szCommStr, _T("COM"));	
		_ltow(_dwPort, p, 10);
		wcscat(_szCommStr, p);
 		wcscat(_szCommStr,_T(":"));
	}
	//打开串口
	virtual bool OpenCommPort()
	{
		if(IsOpen())
			Close();

		_hCommHandle = ::CreateFile(
			_szCommStr,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,	
			NULL
			);

		if(_fAutoBeginThread)
		{
			if(IsOpen() && BeginThread())
				return true;
			else
			{
				Close(); //创建线程失败
				return false;
			}
		}
		return IsOpen();//检测串口是否成功打开
	}
	//设置串口
	virtual bool SetupPort()
	{
		if(!IsOpen())
			return false;

		//设置推荐缓冲区
		if(!::SetupComm(_hCommHandle, 1024, 1024))
			return false;	

		//设置超时时间
		if(!::GetCommTimeouts(_hCommHandle, &_CO))
			return false;
		_CO.ReadIntervalTimeout = MAXDWORD;
		_CO.ReadTotalTimeoutMultiplier = MAXDWORD;
		_CO.ReadTotalTimeoutConstant = 20;
		_CO.WriteTotalTimeoutMultiplier = 0;
		_CO.WriteTotalTimeoutConstant = 0;
		if(!::SetCommTimeouts(_hCommHandle, &_CO))
			return false;	

		//清空串口缓冲区
		if(!::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
			return false;	

		return true;
	}			
	
private:
	//监视线程
	static DWORD WINAPI CommThreadProc(LPVOID lpPara)
	{
		CSerial *pComm = (CSerial *)lpPara;	
		

        if(!::SetCommMask(pComm->_hCommHandle, EV_RXCHAR | EV_ERR))
			return 1;

		COMSTAT Stat;
		DWORD dwError;

		for(DWORD dwLength, dwMask = 0; pComm->_fRunFlag && pComm->IsOpen(); dwMask = 0)
		{
			if(!::WaitCommEvent(pComm->_hCommHandle, &dwMask, &pComm->_WaitOverlapped))
			{

			}

			if(dwMask & EV_ERR) // == EV_ERR
				::ClearCommError(pComm->_hCommHandle, &dwError, &Stat);

			if(dwMask & EV_RXCHAR) // == EV_RXCHAR
			{
				::ClearCommError(pComm->_hCommHandle, &dwError, &Stat);
				if(Stat.cbInQue > pComm->_dwNotifyNum)
					pComm->OnReceive();
			}
	    }
		return 0;
	}

		//监视线程
//	static DWORD WINAPI CommThreadProc(LPVOID lpPara)
//	{
//		Serial *pComm = (Serial *)lpPara;	
//		
//		while(WaitForSingleObject(pComm->_hCommHandle,0)!=WAIT_OBJECT_0)
//		{
//			if(pComm->IsReadReady())
//			{
//				pComm->OnReceive();
//			}
//			else
//			{
//				Sleep(20);
//			}
//		}
//
//		return 0;
//	}
//	
//	BOOL IsReadReady()
//	{
//
//		DWORD dwErrorFlags;
//		COMSTAT ComStat;
//
//		BOOL bRstCode=ClearCommError(_hCommHandle,&dwErrorFlags,&ComStat);
//
//		if(bRstCode)
//		{
//			if(ComStat.cbInQue >0)
//				return TRUE;
//		}
//
//		return FALSE;
//
// 	}
};

#endif 
