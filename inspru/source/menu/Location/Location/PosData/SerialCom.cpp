/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* �ļ�����:SerialCom.c

* �ļ���ʶ: ������
* ժ    Ҫ: 

* ��    ��: 
* �������: 2011/02/22
* ��ע:�����ɶ���ʱ����Ҫ���������ص�������ԭ��Ϊ��

void (CALLBACK *READ_DATA)(BYTE *buf, DWORD buflen);
��������Ӵ��ڶ�ȡ�����ݡ�

int(CALLBACK *ERROR_CONTENT)(LPCTSTR);
�������������Ϣ��
************************************************************************/
#include "stdafx.h"
#include "SerialCom.h"

//���캯��
CSerialCom::CSerialCom(ERROR_CONTENT FunctionnameError,READ_DATA FunctionnameRead)
{
	m_hComm = INVALID_HANDLE_VALUE;		/* ���ڲ��������Ч */
	m_hRecvThread = NULL;
	m_ExitThreadEvent = NULL;			/* ���ڽ����߳��˳��¼���Ч */

	m_ErrorFoc = FunctionnameError;		//����ص�������
	m_ReadFoc = FunctionnameRead;
}

//��������
CSerialCom::~CSerialCom()
{
	//if (m_hRecvThread != NULL)
	//{
	//	CloseHandle(m_hRecvThread);
	//	SetEvent(m_ExitThreadEvent);				/* ֪ͨ���ڽ����߳��˳� */
	//	Sleep(200);
	//	CloseHandle(m_ExitThreadEvent);				/* �ر��߳��˳��¼� */
	//}
}

/*******************************************************************************************
��������: CSerialCom::OpenPort
��    ��: �򿪴���
�������: �˿ڲ����ṹ�壬�û�����
�������: ��ʧ�ܣ����ûص����������������Ϣ
��    ��: FALSE: ʧ��;    TRUE: �ɹ�
********************************************************************************************/
BOOL CSerialCom::OpenPort(Port_Parameters port_param)
{
	COMMTIMEOUTS CommTimeOuts;

	// �򿪴���
	m_hComm = CreateFile(port_param.tzPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if (m_ErrorFoc !=NULL)
		{
			(*m_ErrorFoc)(_T("�޷��򿪶˿ڻ�˿��Ѵ�!�����Ƿ��ѱ�ռ��."));
		}
		return FALSE;
	}

	GetCommState(m_hComm, &m_dcb);						//��ȡ���ڵ�DCB,���ڲ����ṹ��
	m_dcb.BaudRate = port_param.nBaudRate;			
	m_dcb.ByteSize = port_param.nDataBits;
	m_dcb.Parity = port_param.nParity;
	m_dcb.StopBits = port_param.nStopBits;	
	m_dcb.fParity = FALSE;								// ��ֹ��żУ�� 
	m_dcb.fBinary = TRUE;

	if (FALSE == port_param.bDTRShake)
		m_dcb.fDtrControl = 0;							// ��ֹ��������
	else
		m_dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;

	if (FALSE ==port_param.bRTSShake )
		m_dcb.fRtsControl = 0;
	else
		m_dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

	m_dcb.fOutX = 0;
	m_dcb.fInX = 0;
	m_dcb.fTXContinueOnXoff = 0;
	
	//����״̬����
	SetCommMask(m_hComm, EV_RXCHAR);					//�����¼�:���յ�һ���ַ� 
	SetupComm(m_hComm, 16384, 16384);					// ���ý����뷢�͵Ļ�������С
	if(!SetCommState(m_hComm, &m_dcb))					// ���ô��ڵ�DCB 
	{
		if (m_ErrorFoc !=NULL)
		{
			(*m_ErrorFoc)(_T("�޷�����ǰ�������ö˿ڣ��������!"));
		}
		ClosePort();
		return FALSE;
	}
		
	//���ó�ʱ����
	GetCommTimeouts(m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = 100;				// �����ַ������ʱ����
	CommTimeOuts.ReadTotalTimeoutMultiplier = 1;		
	CommTimeOuts.ReadTotalTimeoutConstant = 100;		// �������ܳ�ʱ����
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;
	if(!SetCommTimeouts(m_hComm, &CommTimeOuts))
	{
		if (m_ErrorFoc != NULL)
		{
			(*m_ErrorFoc)(_T("�޷����ó�ʱ����!"));
		}
		ClosePort();
		return FALSE;
	}
		
	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);	 // �����/�������� 
	

	// �����̼߳����
	DWORD IDThread;

	// �������ڽ����߳��˳��¼�
	m_ExitThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		

	// �������ڽ����߳�
	m_hRecvThread = CreateThread(0, 0, CommRecvTread, this, 0, &IDThread);
	if (m_hRecvThread == NULL) 
	{
		ClosePort();
		if (m_ErrorFoc != NULL)
		{
			(*m_ErrorFoc)(TEXT("���������߳�ʧ��!"));
		}
		return FALSE;
	}	
	
	return TRUE;		
}
/*******************************************************************************************
��������: CSerialCom::IsPortOpen()
��    ��: ���Զ˿��Ƿ��
�������: ��
�������: ��
��    ��: FALSE: �ر�;    TRUE: ��
********************************************************************************************/
BOOL CSerialCom::IsPortOpen()
{
	if(m_hComm != INVALID_HANDLE_VALUE) 
		return TRUE;

	return FALSE;
}
/*******************************************************************************************
��������: CSerialCom::ClosePort()
��    ��: �رմ���
�������: ��
�������: ��
��    ��: FALSE: ʧ��;    TRUE: �ɹ�
********************************************************************************************/
BOOL CSerialCom::ClosePort()
{

	if (m_hRecvThread != NULL)
	{
		CloseHandle(m_hRecvThread);
		SetEvent(m_ExitThreadEvent);				/* ֪ͨ���ڽ����߳��˳� */
		CloseHandle(m_ExitThreadEvent);				/* �ر��߳��˳��¼� */
		Sleep(200);
		m_ExitThreadEvent = NULL;
	}

	if(m_hComm != INVALID_HANDLE_VALUE)
	{
		SetCommMask(m_hComm, 0);		
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);	/* �����/������ */
		CloseHandle(m_hComm);								/* �رմ��ڲ������ */
		m_hComm = INVALID_HANDLE_VALUE;
		return TRUE;
	}

	return TRUE;
}

/*******************************************************************************************
��������: CSerialCom::Senddata(BYTE *psendbuf, DWORD length) 
��    ��: ��������
�������: psendbuf ָ��Ҫ���͵�����
			   length  ���ݳ���
�������: �緢��ʧ�ܣ��ص��������ʧ����Ϣ
��    ��: �ɹ�����TRUE; ʧ�ܷ���FALSE��
********************************************************************************************/
BOOL CSerialCom::SendData(BYTE *psendbuf, DWORD length) 
{
	DWORD dwactlen;

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		if (m_ErrorFoc != NULL)
		{
			(*m_ErrorFoc)(_T("����δ��!"));
		}
		return FALSE;
	}

	BOOL flag = WriteFile(m_hComm, psendbuf, length, &dwactlen, NULL);	 /* �Ӵ��ڷ������� */
	if (!flag)
	{
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************************
��������: CSerialCom::CommRecvTread
��    ��: ���ڽ����߳�
�������: LPVOID lparam: �̲߳���,�����߳�ʱ����
�������: ����ȡʧ�ܣ��ص��������������Ϣ
��    ��: 0: �߳��˳�, ����ֵû���⺬��
********************************************************************************************/
DWORD CSerialCom::CommRecvTread(LPVOID lparam)
{
	DWORD dwLength = 0;//����ʵ�ʶ�ȡ���ֽڳ���
	BYTE *recvBuf = new BYTE[4096];
	CSerialCom *pCeSerial = (CSerialCom *)lparam;  //��ָ�룬ǿתΪCSerialCom��

	while(TRUE)
	{																// �ȴ��߳��˳��¼� ,��ʱ���0������˵�������ж��Ƿ��ǣ�Ȼ�󷵻�
		if (WaitForSingleObject(pCeSerial->m_ExitThreadEvent, 0) == WAIT_OBJECT_0)
			break;	//��������Ϊ��������¼�����ǣ�signaled�������߶�ʱʱ�䵽�����ء� ��SetEvent�����

		if (pCeSerial->m_hComm != INVALID_HANDLE_VALUE)
		{
			ZeroMemory(recvBuf, 4096);
			// �Ӵ��ڶ�ȡ���� 
			ReadFile(pCeSerial->m_hComm, recvBuf, 4096, &dwLength, NULL);

			if(dwLength != 0 && pCeSerial->m_ReadFoc != NULL)	
			{
				(*(pCeSerial->m_ReadFoc))(recvBuf, dwLength);			//���ճɹ����ûص�����
			}
		}
	}		

	delete[] recvBuf;
	recvBuf =	NULL;
	return 0;
}
