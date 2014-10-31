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
	static CDataPack*	m_pInstance;				//Singleton模式实例指针
	CSocketManager *m_psocket;
	
	DWORD m_dwDataLength;
	BYTE				m_databuf[MSG_BUFFER_LEN];	//打包数据buffer
	MSG_HEAD			m_stMsgHead;				//消息头
	queue<Subcontract_Data> m_Subqueue; //多媒体补传分包队列
	stack<Blind_Data> m_BlindStack;

	bool m_bIsSubing;

	WORD m_wjpgpacknum;

public:
	CDataPack(void);
	~CDataPack(void);

	//Singleton模式实例函数
	static CDataPack* Instance();
	//设置消息头属性
	VOID	SetMsgProperty(LPCSTR pcPhoneNumber, BYTE cEncryptionType);

	//终端(TerMiNal)消息ID
	BOOL	PackCOMMON_ACK(enResult Result);									//1.终端通用应答0x0001
	BOOL	PackHEARTBEAT();													//3.终端心跳0x0002
	BOOL	PackTimeRequest();
	//4.终端注册0x0100
	BOOL	PackREGISTER(WORD wProvinceID, WORD wCityID, LPCSTR pcManufacturerID, LPCSTR pcTerminalType, 
						LPCSTR pcTerminalID, BYTE cNumberPlateColor, PCSTR pcPlateNumber, WORD wNumberLength);	
	BOOL	PackLOGOUT();														//6.终端注销0x0003
	BOOL	PackAUTHENTICATION(PCSTR pAuthenCode = NULL);						//7.终端鉴权0x0102
	BOOL	PackPARAM_GET_ACK();												//10.查询终端参数应答0x0104
	BOOL	PackPOS_REPORT();													//12.位置信息汇报0x0200
	BOOL	PackPOS_GET_ACK();													//14.位置信息查询应答0x0201
	BOOL	PackEVENT_REPORT(BYTE cEventID);									//18.事件报告0x0301
	BOOL	PackQUESTION_ACK(/*DWORD dwQuestionUID,*/ BYTE cAnswerID);			//20.提问应答0x0302
	BOOL	PackINFO_DEMAND_CANCEL(DWORD dwAck_result/*BYTE cInfoType, BYTE cFlag*/);	//22.信息点播/取消0x0303
	BOOL	PackCAR_CONTROL_ACK();												//27.车辆控制应答0x0500
	BOOL	PackDRIVE_RECORD_UPLOAD(BYTE cCMD, PBYTE pcDataBlock, WORD wLen);	//37.行驶记录数据上传0x0700
	BOOL    Pack_AWB_UPLoad(DWORD dwID);										//39.电子运单上报(Auto Waybill)0x0701

	BOOL	Pack_Upgrade_ACK(BYTE byok);
	
	BOOL	SetillegalDisplace(WORD wdis);

	static DWORD THDAuthentiaction(LPVOID lp);

	HANDLE m_hMutex;

	//42.多媒体数据上传0x0801
	BOOL	PackMULTIMEDIA_UPLOAD(DWORD dwID);


	BOOL	PackTextSMS(DWORD nTextSMS_ID);				//附1.文本信息上传0x0F01
	BOOL	PackTMN_ATTRIBUTE(LPCSTR pcManufacturerID, LPCSTR pcTerminalType, 
							LPCSTR pcTerminalID, LPCSTR pcHardwareVersion, LPCSTR pcFirmwareVersion);	//打包查询终端属性

	//生成消息结构
	VOID	BuildFullMsg(WORD wMsgID, PBYTE pcMsgBody, WORD wBodyLength, 
		BOOL bSubPack = FALSE, WORD wMsgPackNum = 0, WORD wMsgPackIndex = 0);

	
	//大端模式(big-endian)的网络字节序传送字和双字
	WORD	Int2Str(PBYTE pcMsgBody, WORD wData);
	WORD	Int2Str(PBYTE pcMsgBody, DWORD dwData);
	//打包位置信息，包括位置基本信息和位置附加信息项列表
	WORD	PackPosInfo(PBYTE pcPosInfo);
	WORD    PackGPSPosInfo(PBYTE pGpsInfo);
	VOID    OnAlarm();
	VOID	OnSysPowerOff();
	SYSTEMTIME st;

	void	SaveGpsBlindData ();//保存盲区数据
	bool	SendBlindAreaData();//发送盲区数据
	void	SplitBlindAreaData(); //分拆盲区数据


	void	UploadPhotoPack();

	//系统消息
	VOID OutsideEvmDetect(BYTE bydetect);
	VOID SystemPowerOff(WPARAM wparam);
	void SetSendCountZero();


private:
	//协议数据标识位转义
	VOID	DataCoding(/*PBYTE pcBuffer, int nBufferLen*/);
	//在buffer里面插入字符
	VOID	InsertChar(INT nIndex, BYTE cValue);
	//生成消息头
	WORD	BuildHead();

	WORD m_wsendcount;

	/********Tools**********/
	//检查是否为数字字符串
	BOOL	IsDigitalStr(PBYTE pBuffer, DWORD dwLen);
	//字符串拷贝
	//WORD	Str2Str(PBYTE pcMsgBody, PCSTR pszStr);
	//打包参数
	WORD	PackParam(DWORD dwParamID, PBYTE pcMsgBody, WORD wData);
	WORD	PackParam(DWORD dwParamID, PBYTE pcMsgBody, DWORD dwData);
	WORD	PackParam(DWORD dwParamID, PBYTE pcMsgBody, PCSTR pszStr);
};
