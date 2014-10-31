
#include "StdAfx.h"
#include "scom_rfid.h"
#include "../ExTool/ex_debug.h"
#include "scom_api.h"

scom_rfid scom_rfid::m_instance;

scom_rfid::scom_rfid()
{
	memset(m_pFnGetRFID, NULL, sizeof(m_pFnGetRFID));
	memset(m_pCWnd, NULL, sizeof(m_pCWnd));	
	m_bTHDIC = FALSE;
}

scom_rfid::~scom_rfid()
{
	m_bTHDIC = FALSE;
	if ( m_serial.is_open() )
		m_serial.close();
}

scom_rfid& scom_rfid::Instance()
{
	return m_instance;
}

int scom_rfid::ConnectCardReader(int n_port, int n_baud)
{
	if ( m_serial.is_open() )
		return true;

	if ( !m_serial.open( n_port, n_baud ))
	{
		CString str;
		str.Format(_T("打开RFID串口失败:com%d,%d"), n_port, n_baud);
		MessageBox(NULL, str, NULL, MB_OK);
		return false;
	}
	m_bTHDIC = TRUE;
	unsigned long  thrd_id;
	m_hTHDICCardWatch = CreateThread(NULL,
		0,
		THDICCardWatch,
		this,
		0,
		&thrd_id );
	if ( m_hTHDICCardWatch == 0 )
		return false;

	return true;
}

DWORD scom_rfid::THDICCardWatch(LPVOID lpThreadParameter)
{
	scom_rfid* pDlg = (scom_rfid*)lpThreadParameter;
	char		cBuffer[100];
	while( pDlg->m_bTHDIC )
	{
		memset(cBuffer, NULL, sizeof(cBuffer));
		int nLenRead = pDlg->m_serial.read_data(cBuffer,20);
		if ( nLenRead == 0 )
		{
			Sleep(50);
			continue;
		}
		pDlg->parse_icdata(cBuffer, nLenRead);
	}
	return 1;
}

void scom_rfid::parse_icdata(char* pBuffer, int nLen)
{
	int i;
	TRACE0("\n>>>>>>>>>>Receive Serial COM Data:>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(i=0; i<nLen; i++)
	{
		TRACE1("%c", pBuffer[i]);
	}
	TRACE0("\n--------16进制数据-------------------------------------\n");
	for(i=0; i<nLen; i++)
	{
		TRACE1("%02X ", (UCHAR)pBuffer[i]);
		if( (i+1)%20 == 0 && (i+1) != nLen )
			TRACE0("\n");
	}
	TRACE0("\n>>>>>>>>>>end>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	if( check_cardnumber(pBuffer, nLen) )
	{
		//CC FF FF 01 08 0D E2 00 10 66 66 13 00 95 20 70 3C 81 6D
		//CC FF FF 01 08 0D E2 00 10 66 66 13 00 85 12 80 92 BE E8
		//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 (index)
		//6~9:存放在整数内
		//如果读出来的串号都是0,则舍弃
		if(pBuffer[6] == 0 
			&& pBuffer[7] == 0 
			&& pBuffer[8] == 0
			&& pBuffer[9] == 0)
		{
			return;
		}

		DWORD dwID = ((UCHAR)pBuffer[6] << 24) + ((UCHAR)pBuffer[7] << 16) + ((UCHAR)pBuffer[8] << 8) +(UCHAR)pBuffer[9];

		char szTemp[256];
		memset( szTemp, NULL, sizeof(szTemp) );
		sprintf(szTemp, ("%010u"), dwID);
		
		//将提取出的RFID编号传入函数数组
		for(i=0; i<3; i++)
		{
			if(m_pFnGetRFID[i] != NULL)
			{
				m_pFnGetRFID[i](m_pCWnd[i], szTemp, 10);
			}
		}
	}
}

bool scom_rfid::check_cardnumber(char* pBuffer, int nLen)
{
	//CC FF FF 01 08 0D E2 00 10 66 66 13 00 95 20 70 3C 81 6D 
	//CC FF FF 01 08 0D E2 00 10 66 66 13 00 85 12 80 92 BE E8 
	if( (UCHAR)pBuffer[0] == 0xcc
		&& (UCHAR)pBuffer[1] == 0xFF
		&& (UCHAR)pBuffer[2] == 0xFF
		&& (UCHAR)pBuffer[3] == 0x01
		&& (UCHAR)pBuffer[4] == 0x08
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void scom_rfid::AddFnc(PVOID pCWnd, PFN_GET_RFID pFnGetRFID)
{
	int i;
	bool bFind = false;
	for(i=0; i<3; i++)
	{
		if(m_pFnGetRFID[i] == pFnGetRFID)
		{
			bFind = true;
		}
	}
	if(bFind)
		return;

	for(i=0; i<3; i++)
	{
		if(m_pFnGetRFID[i] == NULL)
		{
			m_pFnGetRFID[i] = pFnGetRFID;
			m_pCWnd[i] = pCWnd;
			return;
		}
	}
}

void scom_rfid::RemoveFnc(PFN_GET_RFID pFnGetRFID)
{
	int i;
	for(i=0; i<3; i++)
	{
		if(m_pFnGetRFID[i] == pFnGetRFID)
		{
			m_pFnGetRFID[i] = NULL;
			return;
		}
	}
}