/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* �ļ�����:SerialCom.h

* �ļ���ʶ: ������
* ժ    Ҫ: 

* ��    ��: 
* �������: 2011/02/22
* ��ע:��ʹ����֮ǰ����ʹ��ע�ắ��ע�������ص�������ԭ��Ϊ��

void (CALLBACK *READ_DATA)(BYTE *buf, DWORD buflen);
��������Ӵ��ڶ�ȡ�����ݡ�

int(CALLBACK *ERROR_CONTENT)(LPCTSTR);
�������������Ϣ��
************************************************************************/
#ifndef __SERIAL_INTERFACE_CLASS__
#define __SERIAL_INTERFACE_CLASS__


//���崮����������ṹ��
typedef struct
{
	LPCTSTR tzPort;							 //������
	int nBaudRate;							//������
	int nDataBits;								//ָ��ÿ���ֽڵ�λ����ͨ��Ϊ7��8��
	int nStopBits;								//ֹͣλ
	int nParity;									//���Ա�����Ϊ�Խ��ͳ���
	BOOL bDTRShake;						//�Ƿ����DTR��
	BOOL bRTSShake;						//�Ƿ����RTS��
}Port_Parameters;
/***************************�����ص�����ԭ��*****************************************
��������: CALLBACK *ERROR_CONTENT
��    ��: ���ڴ������ݻص�����
�������: ����ľ�����Ϣ
�������: 
��    ��: 
********************************************************************************************/
typedef int(CALLBACK *ERROR_CONTENT)(LPCTSTR);
/***************************�����ص�����ԭ��*****************************************
��������:CALLBACK *READ_DATA
��    ��: �Ӵ��ڶ�ȡ���ݻص�����
�������: UserParam�û�����
buf			ָ��洢���ݵĿռ�
buflen		����ʵ�ʶ�ȡ���ֽڳ���
�������: 
��    ��: 
********************************************************************************************/
typedef void (CALLBACK *READ_DATA)(BYTE *cBuffer, DWORD dwLen);

class CSerialCom
{
public:
	CSerialCom(ERROR_CONTENT FunctionnameError,READ_DATA FunctionnameRead);
	~CSerialCom();

	// �򿪴���
	BOOL OpenPort(Port_Parameters port_param);

	// �����Ƿ��
	BOOL IsPortOpen();

	// �رմ���
	BOOL ClosePort(void);

	// ��������
	BOOL SendData(BYTE *sendbuf, DWORD length);

private:
	CSerialCom();
	CSerialCom(CSerialCom &other);
	CSerialCom operator = (CSerialCom &other);

	DCB m_dcb;								/* ���ڲ����ṹ�� */
	HANDLE m_hRecvThread;						/* �����߳̾�� */
	HANDLE m_ExitThreadEvent;				/* ���ڽ����߳��˳��¼� */
	HANDLE m_hComm;							/* ���ڲ������ */
	ERROR_CONTENT m_ErrorFoc;			//���淵�ش���Ļص�������
	READ_DATA m_ReadFoc;				//���淵�ض������ݵĻص�������

	// ���ڽ����߳�
	static DWORD CommRecvTread(LPVOID lparam);
};


#endif