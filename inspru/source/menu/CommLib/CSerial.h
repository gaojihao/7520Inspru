  /*******************************************************************/
/*
���ڻ������

����
cnComm MyComm1(false, 0);//��1������Ϊ�Ƿ����������̣߳� ��2������Ϊ������0��/�첽��ʽ��Ĭ�ϣ�
cnComm MyComm2;//Ĭ�����������̣߳� �첽
cnComm MyComm3(true);//�����̣߳� ����

��
MyComm1.Open(1);//1-1024  2k֧��1024����
MyComm1.Open(1, 1200);
MyComm1.Open(1, 9600);
MyComm1.Open(1, "9600,8,n,1");//��ʹ�ñ�׼�������ַ���
MyComm1.SetBufferSize(1200, 4800)//���û�������С,�ɲ�������
MyComm1.SetState("9600,8,n,1")//�޸Ĳ����ʵ�


��д

MyComm1.Read(buf, 1000);
MyComm1.Write(buf, 200);


���ھ��
MyComm1.SetWnd(hWnd);

�߳̿������涼��ע��




				¬��

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


//�����ܵ������͵����ڵ���Ϣ
#define ON_COM_RECEIVE WM_USER + 618	 //	 WPARAM �˿ں�

class CSerial	  
{
public:

	//���캯����fAutoBeginThread�Ƿ��������߳�
	CSerial(bool fAutoBeginThread = false)
		: _fAutoBeginThread(fAutoBeginThread)
	{
		_dwIOMode = FILE_FLAG_OVERLAPPED;
		Init();	
	}
	
	//��������
	virtual ~CSerial()
	{
		Close();	
		UnInit();
	}

	//�򿪴��� ȱʡ 9600, 8, n, 1
	inline bool Open(DWORD dwPort)
	{
		return Open(dwPort, 9600);
	}
	//�򿪴��� ȱʡ baud_rate, 8, n, 1
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
	//�򿪴���, ʹ������"9600, 8, n, 1"�������ַ������ô���
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
	//�жϴ����ǻ��
	inline bool IsOpen()
	{
		return _hCommHandle != INVALID_HANDLE_VALUE;
	}
	//��ô��ھ��
	inline HANDLE GetHandle()
	{
		return _hCommHandle;
	}
	//���ô��ڲ����������ʣ�ֹͣλ���� ֧�������ַ��� "9600, 8, n, 1"
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
	//���ô��ڲ����������ʣ�ֹͣλ
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

				//�ı�DCB�ṹ����
			_DCB.BaudRate = dwBaudRate;               //������ 
			_DCB.fBinary = TRUE;                 //Win32��֧�ַǶ����ƴ��д���ģʽ������ΪTRUE 
			_DCB.fParity = TRUE;                 //������żУ�� 

			_DCB.fOutxCtsFlow = FALSE;            //���ж˿ڵ������CTS�߿���
			_DCB.fOutxDsrFlow = FALSE;           //�رմ��ж˿ڵ�DSR������ 
			_DCB.fDtrControl = DTR_CONTROL_ENABLE;   //����DTR��
			_DCB.fDsrSensitivity = FALSE;        //�����ΪTRUE�������κ�������ֽڣ�����DSR�߱����� 
			//PortDCB.fTXContinueOnXoff = TRUE;       //��ΪTRUEʱ��������ջ��������������������Ѵ���XOFF�ַ�����ʹ��������ֹͣ�����ַ�
			_DCB.fTXContinueOnXoff = TRUE;
			_DCB.fOutX = FALSE;                  //��ΪTRUEָ��XON/XOFF���Ʊ����ڿ��ƴ������ 
			_DCB.fInX = FALSE;                   //��ΪTRUEָ��XON/XOFF���Ʊ����ڿ��ƴ������� 
			_DCB.fErrorChar = FALSE;             //WINCE�������������Ĭ��ִ�н���������ֶ� 
			_DCB.fNull = FALSE;                  //��ΪTRUE��ʹ����������������յ��Ŀ��ֽ� 
			_DCB.fRtsControl = RTS_CONTROL_ENABLE;   //����RTS�� 
			_DCB.fAbortOnError = FALSE;          //WINCE�������������Ĭ��ִ�н���������ֶ�
			_DCB.ByteSize = 8;                   //ÿ�ֽڵ�λ�� 
			_DCB.Parity = NOPARITY;              //����żУ�� 
			_DCB.StopBits = ONESTOPBIT;          //ÿ�ֽ�һλֹͣλ 

	
			return ::SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}
	//���ô��ڵ�I/O��������С
	bool SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize)
	{
		if(IsOpen())
			return ::SetupComm(_hCommHandle, dwInputSize, dwOutputSize);
		return false;	
	}
	//��ȡ���� dwBufferLength - 1 ���ַ��� szBuffer ����ʵ�ʶ������ַ���
	DWORD Read(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 50)
	{
		if(!IsOpen())
			return 0;

//		szBuffer[0] = '\0';
//
//		COMSTAT  Stat;
//		DWORD dwError;
//
//		if(::ClearCommError(_hCommHandle, &dwError, &Stat) && dwError > 0)	//�������
//		{
//			::PurgeComm(_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR); /*������뻺����*/
//			return 0;
//		}
//
//		if(!Stat.cbInQue)// ������������
//			return 0;
//
//		unsigned long uReadLength = 0;
//
//		dwBufferLength = dwBufferLength - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufferLength - 1;
//
//		if(!::ReadFile(_hCommHandle, szBuffer, dwBufferLength, &uReadLength, NULL)) //2000 �� ReadFile ʼ�շ��� True
//		{
//			if(::GetLastError() == ERROR_IO_PENDING) // �����첽I/O
//			{
//				WaitForSingleObject(_ReadOverlapped.hEvent, dwWaitTime);	//�ȴ�20ms
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
	//д���� szBuffer
	DWORD Write(char *szBuffer, DWORD dwBufferLength)
	{
		if(!IsOpen())
			return 0;
		
		DWORD dwError;

		if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//�������
			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);	

		unsigned long uWriteLength = 0;

		if(!::WriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, &_WriteOverlapped))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;

		return uWriteLength;
	}
	//д���� szBuffer
	inline DWORD Write(char *szBuffer)
	{
		assert(szBuffer);

		return Write(szBuffer, strlen(szBuffer));
	}
	//ǿ��ͬ��д
	inline DWORD WriteSync(char *szBuffer, DWORD dwBufferLength)
	{
		if(!IsOpen())
			return 0;
		
//		DWORD dwError;
//
//		if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//�������
//			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);	

		unsigned long uWriteLength = 0;

		if(!::WriteFile(_hCommHandle, szBuffer, dwBufferLength, &uWriteLength, NULL))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;

		return uWriteLength;
	}
	//д���� szBuffer ���������ʽ�ַ���
	DWORD Write(char *szBuffer, DWORD dwBufferLength, char * szFormat, ...)
	{
		if(!IsOpen())
			return 0;
		
		DWORD dwError;

		if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)	//�������
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
	//�رմ���
	inline virtual void Close()
	{
		//������������ڲ��ķ��ͺͽ��ն���
		PurgeComm(_hCommHandle,PURGE_TXCLEAR|PURGE_RXCLEAR);
		
		//����˿���ָ���źŵ�״̬
		EscapeCommFunction(_hCommHandle,CLRDTR);
		EscapeCommFunction(_hCommHandle,CLRRTS);
		if(IsOpen())		
		{
			EndThread();
			::CloseHandle(_hCommHandle);

			_hCommHandle = INVALID_HANDLE_VALUE;
		}
	}
	//�趨����֪ͨ, �����ַ���Сֵ
	inline void SetNotifyNum(int iNum)
	{
		_dwNotifyNum = iNum;
	}
	//����Ϣ�Ĵ��ھ��
	inline void SetWnd(HWND hWnd)
	{
		_hNotifyWnd = hWnd;
	}
	//�����߳̿��� �������߳�
	bool BeginThread() 
	{
		if(!IsThreadRunning()) 
		{
			_fRunFlag = true;
			_hThreadHandle = NULL;

			DWORD id;

			_hThreadHandle = ::CreateThread(NULL, 0, CommThreadProc, this, 0, &id); //����98 ��ʹ��&ID

			return (_hThreadHandle != NULL); //�����߳�
		}
		return false;
	}
	//�߳��Ƿ�����
	inline bool IsThreadRunning()
	{
		return _hThreadHandle != NULL;
	}
	//����߳̾��
	inline HANDLE GetThread()
	{
		return _hThreadHandle;
	}
	//��ͣ�����߳�
	inline bool SuspendThread()
	{
		return IsThreadRunning() ? ::SuspendThread(_hThreadHandle) != 0xFFFFFFFF : false;
	}
	//�ָ������߳�
	inline bool ResumeThread()
	{
		return IsThreadRunning() ? ::ResumeThread(_hThreadHandle) != 0xFFFFFFFF : false;
	}
	//��ֹ�߳�
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

	volatile DWORD _dwPort;	 //���ں�
	volatile HANDLE _hCommHandle;//���ھ��
	TCHAR _szCommStr[30];

	DCB _DCB;			  //�����ʣ�ֹͣλ����
	COMMTIMEOUTS _CO;     //��ʱ�ṹ

	DWORD _dwIOMode; // 0 ͬ��  Ĭ�� FILE_FLAG_OVERLAPPED �ص�I/O �첽
	OVERLAPPED _ReadOverlapped, _WriteOverlapped; //	�ص�I/O

	//�߳���
	volatile HANDLE _hThreadHandle;	//�����߳�
	volatile HWND _hNotifyWnd; // ֪ͨ����
	volatile DWORD _dwNotifyNum;//���ܶ����ֽ�(>_dwNotifyNum)����֪ͨ��Ϣ
	volatile bool _fRunFlag; //�߳�����ѭ����־
	bool _fAutoBeginThread;//Open() �Զ� BeginThread();
	OVERLAPPED _WaitOverlapped; //WaitCommEvent use

	//�߳��յ���Ϣ�Զ�����, �細�ھ����Ч, �ͳ���Ϣ, �������ڱ��
	virtual void OnReceive()
	{
		if(_hNotifyWnd)
			::PostMessage(_hNotifyWnd, ON_COM_RECEIVE, WPARAM(_dwPort), LPARAM(0));
 	}
	
	void Init()	//��ʼ��
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
	//�󶨴���
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
	//�򿪴���
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
				Close(); //�����߳�ʧ��
				return false;
			}
		}
		return IsOpen();//��⴮���Ƿ�ɹ���
	}
	//���ô���
	virtual bool SetupPort()
	{
		if(!IsOpen())
			return false;

		//�����Ƽ�������
		if(!::SetupComm(_hCommHandle, 1024, 1024))
			return false;	

		//���ó�ʱʱ��
		if(!::GetCommTimeouts(_hCommHandle, &_CO))
			return false;
		_CO.ReadIntervalTimeout = MAXDWORD;
		_CO.ReadTotalTimeoutMultiplier = MAXDWORD;
		_CO.ReadTotalTimeoutConstant = 20;
		_CO.WriteTotalTimeoutMultiplier = 0;
		_CO.WriteTotalTimeoutConstant = 0;
		if(!::SetCommTimeouts(_hCommHandle, &_CO))
			return false;	

		//��մ��ڻ�����
		if(!::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
			return false;	

		return true;
	}			
	
private:
	//�����߳�
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

		//�����߳�
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
