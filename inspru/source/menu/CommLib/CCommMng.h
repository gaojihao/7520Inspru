// CommMng.h: interface for the CommMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMMNG_H__BB4B692C_2147_4DCD_ADFE_5215D45DB27F__INCLUDED_)
#define AFX_COMMMNG_H__BB4B692C_2147_4DCD_ADFE_5215D45DB27F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSerial.h"
#include "CE920EventProtocol.h"
#include "CE920Event.h"

class CCommMng  : public CSerial
{
public:
	static CCommMng& Instance();
	virtual ~CCommMng();

	//ע������豸�ϵ�Э��
	BOOL RegeditProtocol(PROTOCOL_TYPE = en_E920Event_PROTOCOL);

	//ע�������豸�ϵ�Э��
	BOOL UnRegeditProtocol();

	//��ʼ�������豸
	BOOL OpenDevice(DWORD dwPort, DWORD dwBaudRate);

	//���ý������ݴ���
	void SetRecvWnd(HWND hWnd);
	
	//��ʼ���߳�
	BOOL BeginRecvThread();

	//�ж϶��߳��Ƿ�����
	BOOL IsRecvThreadRuning();

	//������߳�
	BOOL SuspendRecvThread();

	//�ָ����߳�
	BOOL ResumeRecvThread();

	//�������߳�
	BOOL EndRecvThread();

	//��������
	BOOL SendData(CE920Event* lpInData);

	//��������
	BOOL RecvData(CE920Event*& lpOutData);

	//�ر��豸
	void CloseDevice();

private:
	static CCommMng _instance;
	CCommMng();
	CE920EventProtocol* m_pProtocol;
	BOOL  m_bProtocolState;
};

#endif // !defined(AFX_COMMMNG_H__BB4B692C_2147_4DCD_ADFE_5215D45DB27F__INCLUDED_)
