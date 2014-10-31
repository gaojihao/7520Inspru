/************************************************************************
* Copyright (C)2011, �Ϻ��ﲩͨѶ�������޹�˾
* All rights reserved.
* 
* �ļ�����: DataParser.h
* �ļ���ʶ: 
* ժ    Ҫ: 
* ��    ��: 
* �������: 2011/09/26
* ��ע: 
************************************************************************/

#pragma once

#include <list>
#include "MsgBase.h"
#include "DataType.h"
#include "../SocketManager/SocketManager.h"
#include <wininet.h>
using std::list;

typedef	list<RouteSet_Data>	ROUTE_LIST;


class CDataParser : public CMsgBase
{
private:
	static CDataParser*		m_pInstance;				//Singletonģʽ
	MSG_HEAD				m_stMsgHead;				//����ͷ

	COMMON_ACK				m_stCommonAck;				//ƽ̨ͨ��Ӧ��0x8001
	REGISTER_ACK			m_stRegisterAck;			//�ն�ע��Ӧ��0x8100

	TEMP_POS_TRACE			m_stTemPosTrace;			//��ʱλ�ø��ٿ���0x8202

	CSocketManager*			m_pSocket;				   //Socket����

public:
	int m_iMaxStopCar;

public:
	CDataParser(void);
	~CDataParser(void);

	static CDataParser* Instance();


	//Э�����
	BOOL	DataParser(UCHAR *pucTempData, WORD wTempLenght);
	BOOL	DataParserPot(UCHAR *pucTempData, WORD wTempLenght);

	//������ƽ̨(SerVeR)��ϢID
	BOOL	OnCOMMON_ACK(PBYTE pBodyBuffer, WORD wBodyLength);				//2.ƽ̨ͨ��Ӧ��0x8001
	BOOL	OnSUBCONTRACT(PBYTE pBodyBuffer, WORD wBodyLength);				//3.�����ְ�����0x8003
	BOOL	OnREGISTER_ACK(PBYTE pBodyBuffer, WORD wBodyLength);			//5.�ն�ע��Ӧ��0x8100
	BOOL	OnPARAM_SET(PBYTE pBodyBuffer, WORD wBodyLength);				//8.�����ն˲���0x8103
	BOOL	OnPARAM_GET();									//9.��ѯ�ն˲���0x8104
	BOOL	OnTMN_CONTROL(PBYTE pBodyBuffer, WORD wBodyLength);				//11.�ն˿���0x8105
	BOOL	OnTMN_APPOINTPARAM(PBYTE pBodyBuffer, WORD wBodyLength);		//12.��ѯָ���ն˲���0x8106
	BOOL	OnTMN_ATTRIBUTE(PBYTE pBodyBuffer, WORD wBodyLength);			//...��ѯ�ն�����0x8107
	BOOL	OnPOS_GET(WORD wBodyLength);									//13.λ����Ϣ��ѯ0x8201
	BOOL	OnTEMP_POS_TRACE(PBYTE pBodyBuffer, WORD wBodyLength);			//15.��ʱλ�ø��ٿ���0x8202
	BOOL	OnTEMP_ALARM_SMS(PBYTE pBodyBuffer, WORD wBodyLength);			//...�˹�ȷ�ϱ�����Ϣ
	BOOL	OnTEXT_SMS(PBYTE pBodyBuffer, WORD wBodyLength);				//16.�ı���Ϣ�·�0x8300
	BOOL	OnEVENT_SET(PBYTE pBodyBuffer, WORD wBodyLength);				//17.�¼�����0x8301
	BOOL	OnQUESTION(PBYTE pBodyBuffer, WORD wBodyLength);				//19.�����·�0x8302
	BOOL	OnINFO_MENU_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//21.��Ϣ�㲥�˵�����0x8303
	BOOL	OnINFO_SERVICE(PBYTE pBodyBuffer, WORD wBodyLength);			//23.��Ϣ����0x8304
	BOOL	OnCALL_BACK(PBYTE pBodyBuffer, WORD wBodyLength);				//24.�绰�ز�0x8400
	BOOL	OnPHONE_BOOK_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//25.���õ绰��0x8401
	BOOL	OnCAR_CONTROL(PBYTE pBodyBuffer, WORD wBodyLength);				//26.��������0x8500
	BOOL	OnCIRC_REGION_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//28.����Բ������0x8600
	BOOL	OnCIRC_REGION_DEL(PBYTE pBodyBuffer, WORD wBodyLength);			//29.ɾ��Բ������0x8601

	BOOL	OnRECT_REGION_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//30.���þ�������0x8602
	BOOL	OnRECT_REGION_DEL(PBYTE pBodyBuffer, WORD wBodyLength);			//31.ɾ����������0x8603
	BOOL	OnROUTE_SET(PBYTE pBodyBuffer, WORD wBodyLength);				//34.����·��0x8606
	BOOL	OnROUTE_DEL(PBYTE pBodyBuffer, WORD wBodyLength);				//35.ɾ��·��0x8607

	BOOL	OnMULTIMEDIA_UPLOAD_ACK(PBYTE pBodyBuffer, WORD wBodyLength);	//43.��ý�������ϴ�Ӧ��0x8800
	BOOL	OnCAMERA_PHOTOGRAPH(PBYTE pBodyBuffer, WORD wBodyLength,BOOL bAuto = FALSE);		//44.����ͷ������������0x8801
	BOOL	OnMULTIMEDIA_UPLOAD(PBYTE pBodyBuffer, WORD wBodyLength);		//47.�洢��ý�������ϴ�����0x8803
	
	BOOL	OnTIME_Adjust(PBYTE pBodyBuffer, WORD wBodyLength);				//ʱ��У׼

	void	ResetOS();
	void	DevicePoweroff();
	BOOL	FtpConnect();

	VOID	DeleteUserRoadFile(DWORD dwRoadID);

	void	LoadPhotoNum();

	static DWORD THDPhotoUpload(LPVOID lp);
	static DWORD THDManualPhotoUpload(LPVOID lp);
	static DWORD THDStartGuider(LPVOID lp);

	//�������
	WORD m_wPhotoCmd;
	WORD m_wPhotoTime;
	bool m_bIsAutoPhoto;

	DWORD m_dwRoadID;  //��¼����·ID
	

private:
	//
	BOOL	GetDataRegion(UCHAR *pucTempData, int nTempLenght, int& nIndexStart, int& nIndexEnd);
	//Э�����ݱ�ʶλת��
	//BOOL	DataTranscoding(UCHAR *pucDstData, int *nDstDataLenght, UCHAR *pucSrcData, int nSrcDataLenght);
	INT		DataDecoding(PBYTE pcBuffer, int nBufferLen);
	//��Ϣͷ
	BOOL	MsgHeadBodyAnalysis(PBYTE pBuffer, int nBufferLength);
	//��Ϣ��
	BOOL	MsgBodyAnalysis(UCHAR *pucBuffer, int nBufferLength);
	//�����ն˲���
	BOOL	SetParam(DWORD wParamID, DWORD dwParam, LPSTR lpParam, WORD wParamLen);
	//����������������
	BOOL	GetUpgradeParam(PBYTE pBuffer, int nBufferLength);
	//���������ն�����ָ������������
	BOOL	GetConnectParam(PBYTE pBuffer, int nBufferLength);

	BYTE    CalcXor(BYTE *pbySrc, WORD wLenSrc);


	
	static DWORD DowLoadThread(LPVOID lparam);

	HINTERNET m_hConnection;

	CString m_strFtpIP;
	WORD	m_wFtpPort;
	CString m_strUserName;
	CString m_strPassword;


	
	static DWORD THDAppExit(LPVOID lpParameter);

	PROCESS_INFORMATION		m_sProcessInfo;			//����������Ϣ


	WORD   m_VideoTime;

	
	BYTE byTerminalParam[NUM_1K];

	WORD wTerminalParamLen;
	ROUTE_LIST m_RouteList;



public:
	//���ģʽ(big-endian)�������ֽ������ֺ�˫��
	BOOL	Str2Int(PBYTE pBuffer, WORD wLen, WORD& wValue);
	BOOL	Str2Int(PBYTE pBuffer, WORD wLen, DWORD& dwValue);
	DWORD	Str2Int(PBYTE pBuffer, WORD wLen);
	//��ȡ��Ϣͷ�ṹ��
	const PMSG_HEAD		GetMsgHead();
	//��ȡע��Ӧ��ṹ��
	const PREGISTER_ACK	GetRegisterAck();
	//��ȡͨ��Ӧ��ṹ��
	const PCOMMON_ACK	GetCommonAck();
	//��ȡ��ʱλ�ø��ٿ��ƽṹ��
	const PTEMP_POS_TRACE	GetTempPosTrace();

public:


};
