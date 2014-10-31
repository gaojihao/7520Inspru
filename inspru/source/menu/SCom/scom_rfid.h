
#ifndef  SCOM_RFID_H
#define  SCOM_RFID_H

#include "../ExTool/ex_basics.h"
#include "serial_win32.h"

class  scom_rfid
{
public:
	typedef void (*PFN_GET_RFID)(PVOID pCWnd, char* pBuffer, int nLen);
public:
	scom_rfid();
	~scom_rfid();

public:
	static scom_rfid& Instance();
	int ConnectCardReader(int n_port, int n_baud);
	void parse_icdata(char* pBuffer, int nLen);
	bool check_cardnumber(char* pBuffer, int nLen);
	static DWORD THDICCardWatch(LPVOID lpThreadParameter);

	void AddFnc(PVOID pCWnd, PFN_GET_RFID pFnGetRFID);
	void RemoveFnc(PFN_GET_RFID pFnGetRFID);

protected:
	static scom_rfid m_instance;
	HANDLE			m_hTHDICCardWatch;
	BOOL			m_bTHDIC;	//是否结束运行
	PFN_GET_RFID	m_pFnGetRFID[3];
	PVOID			m_pCWnd[3];	//3个函数对应的窗口指针
public:
	serial_win32	m_serial;
};

#endif //SCOM_RFID_H