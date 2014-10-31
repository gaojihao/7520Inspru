#pragma once

#include "MsgBase.h"
#include "../Lib/Location.h"
#include "../SocketManager/SocketManager.h"
#include "DataType.h"
#include <stack>
#include <queue>

using std::stack;
using std::queue;





class CDataPack : public CMsgBase
{
private:
	static CDataPack*	m_pInstance;				//Singletonģʽʵ��ָ��
	CSocketManager *m_psocket;
	
	DWORD m_dwDataLength;
	BYTE				m_databuf[MSG_BUFFER_LEN];	//�������buffer
	MSG_HEAD			m_stMsgHead;				//��Ϣͷ
	queue<Subcontract_Data> m_Subqueue; //��ý�岹���ְ�����
	stack<Blind_Data> m_BlindStack;

	bool m_bIsSubing;

	WORD m_wjpgpacknum;

public:
	CDataPack(void);
	~CDataPack(void);

	//Singletonģʽʵ������
	static CDataPack* Instance();
	//������Ϣͷ����
	VOID	SetMsgProperty(LPCSTR pcPhoneNumber, BYTE cEncryptionType);

	//�ն�(TerMiNal)��ϢID
	BOOL	PackCOMMON_ACK(enResult Result);									//1.�ն�ͨ��Ӧ��0x0001
	BOOL	PackHEARTBEAT();													//3.�ն�����0x0002
	BOOL	PackTimeRequest();
	//4.�ն�ע��0x0100
	BOOL	PackREGISTER(WORD wProvinceID, WORD wCityID, LPCSTR pcManufacturerID, LPCSTR pcTerminalType, 
						LPCSTR pcTerminalID, BYTE cNumberPlateColor, PCSTR pcPlateNumber, WORD wNumberLength);	
	BOOL	PackLOGOUT();														//6.�ն�ע��0x0003
	BOOL	PackAUTHENTICATION(PCSTR pAuthenCode = NULL);						//7.�ն˼�Ȩ0x0102
	BOOL	PackPARAM_GET_ACK();												//10.��ѯ�ն˲���Ӧ��0x0104
	BOOL	PackPOS_REPORT();													//12.λ����Ϣ�㱨0x0200
	BOOL	PackPOS_GET_ACK();													//14.λ����Ϣ��ѯӦ��0x0201
	BOOL	PackEVENT_REPORT(BYTE cEventID);									//18.�¼�����0x0301
	BOOL	PackQUESTION_ACK(/*DWORD dwQuestionUID,*/ BYTE cAnswerID);			//20.����Ӧ��0x0302
	BOOL	PackINFO_DEMAND_CANCEL(DWORD dwAck_result/*BYTE cInfoType, BYTE cFlag*/);	//22.��Ϣ�㲥/ȡ��0x0303
	BOOL	PackCAR_CONTROL_ACK();												//27.��������Ӧ��0x0500
	BOOL	PackDRIVE_RECORD_UPLOAD(BYTE cCMD, PBYTE pcDataBlock, WORD wLen);	//37.��ʻ��¼�����ϴ�0x0700
	BOOL    Pack_AWB_UPLoad(DWORD dwID);										//39.�����˵��ϱ�(Auto Waybill)0x0701

	BOOL	Pack_Upgrade_ACK(BYTE byok);
	
	BOOL	SetillegalDisplace(WORD wdis);

	static DWORD THDAuthentiaction(LPVOID lp);

	HANDLE m_hMutex;

	//42.��ý�������ϴ�0x0801
	BOOL	PackMULTIMEDIA_UPLOAD(DWORD dwID);


	BOOL	PackTextSMS(DWORD nTextSMS_ID);				//��1.�ı���Ϣ�ϴ�0x0F01
	BOOL	PackTMN_ATTRIBUTE(LPCSTR pcManufacturerID, LPCSTR pcTerminalType, 
							LPCSTR pcTerminalID, LPCSTR pcHardwareVersion, LPCSTR pcFirmwareVersion);	//�����ѯ�ն�����

	//������Ϣ�ṹ
	VOID	BuildFullMsg(WORD wMsgID, PBYTE pcMsgBody, WORD wBodyLength, 
		BOOL bSubPack = FALSE, WORD wMsgPackNum = 0, WORD wMsgPackIndex = 0);

	
	//���ģʽ(big-endian)�������ֽ������ֺ�˫��
	WORD	Int2Str(PBYTE pcMsgBody, WORD wData);
	WORD	Int2Str(PBYTE pcMsgBody, DWORD dwData);
	//���λ����Ϣ������λ�û�����Ϣ��λ�ø�����Ϣ���б�
	WORD	PackPosInfo(PBYTE pcPosInfo);
	WORD    PackGPSPosInfo(PBYTE pGpsInfo);
	VOID    OnAlarm();
	VOID	OnSysPowerOff();
	SYSTEMTIME st;

	void	SaveGpsBlindData ();//����ä������
	bool	SendBlindAreaData();//����ä������
	void	SplitBlindAreaData(); //�ֲ�ä������


	void	UploadPhotoPack();

	//ϵͳ��Ϣ
	VOID OutsideEvmDetect(BYTE bydetect);
	VOID SystemPowerOff(WPARAM wparam);
	void SetSendCountZero();


private:
	//Э�����ݱ�ʶλת��
	VOID	DataCoding(/*PBYTE pcBuffer, int nBufferLen*/);
	//��buffer��������ַ�
	VOID	InsertChar(INT nIndex, BYTE cValue);
	//������Ϣͷ
	WORD	BuildHead();

	WORD m_wsendcount;

	/********Tools**********/
	//����Ƿ�Ϊ�����ַ���
	BOOL	IsDigitalStr(PBYTE pBuffer, DWORD dwLen);
	//�ַ�������
	//WORD	Str2Str(PBYTE pcMsgBody, PCSTR pszStr);
	//�������
	WORD	PackParam(DWORD dwParamID, PBYTE pcMsgBody, WORD wData);
	WORD	PackParam(DWORD dwParamID, PBYTE pcMsgBody, DWORD dwData);
	WORD	PackParam(DWORD dwParamID, PBYTE pcMsgBody, PCSTR pszStr);
};
