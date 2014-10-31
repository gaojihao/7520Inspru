/************************************************************************
* Copyright (C)2011, 上海鲲博通讯技术有限公司
* All rights reserved.
* 
* 文件名称: DataParser.h
* 文件标识: 
* 摘    要: 
* 作    者: 
* 完成日期: 2011/09/26
* 备注: 
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
	static CDataParser*		m_pInstance;				//Singleton模式
	MSG_HEAD				m_stMsgHead;				//数据头

	COMMON_ACK				m_stCommonAck;				//平台通用应答0x8001
	REGISTER_ACK			m_stRegisterAck;			//终端注册应答0x8100

	TEMP_POS_TRACE			m_stTemPosTrace;			//临时位置跟踪控制0x8202

	CSocketManager*			m_pSocket;				   //Socket连接

public:
	int m_iMaxStopCar;

public:
	CDataParser(void);
	~CDataParser(void);

	static CDataParser* Instance();


	//协议解析
	BOOL	DataParser(UCHAR *pucTempData, WORD wTempLenght);
	BOOL	DataParserPot(UCHAR *pucTempData, WORD wTempLenght);

	//服务器平台(SerVeR)消息ID
	BOOL	OnCOMMON_ACK(PBYTE pBodyBuffer, WORD wBodyLength);				//2.平台通用应答0x8001
	BOOL	OnSUBCONTRACT(PBYTE pBodyBuffer, WORD wBodyLength);				//3.补传分包请求0x8003
	BOOL	OnREGISTER_ACK(PBYTE pBodyBuffer, WORD wBodyLength);			//5.终端注册应答0x8100
	BOOL	OnPARAM_SET(PBYTE pBodyBuffer, WORD wBodyLength);				//8.设置终端参数0x8103
	BOOL	OnPARAM_GET();									//9.查询终端参数0x8104
	BOOL	OnTMN_CONTROL(PBYTE pBodyBuffer, WORD wBodyLength);				//11.终端控制0x8105
	BOOL	OnTMN_APPOINTPARAM(PBYTE pBodyBuffer, WORD wBodyLength);		//12.查询指定终端参数0x8106
	BOOL	OnTMN_ATTRIBUTE(PBYTE pBodyBuffer, WORD wBodyLength);			//...查询终端属性0x8107
	BOOL	OnPOS_GET(WORD wBodyLength);									//13.位置信息查询0x8201
	BOOL	OnTEMP_POS_TRACE(PBYTE pBodyBuffer, WORD wBodyLength);			//15.临时位置跟踪控制0x8202
	BOOL	OnTEMP_ALARM_SMS(PBYTE pBodyBuffer, WORD wBodyLength);			//...人工确认报警消息
	BOOL	OnTEXT_SMS(PBYTE pBodyBuffer, WORD wBodyLength);				//16.文本信息下发0x8300
	BOOL	OnEVENT_SET(PBYTE pBodyBuffer, WORD wBodyLength);				//17.事件设置0x8301
	BOOL	OnQUESTION(PBYTE pBodyBuffer, WORD wBodyLength);				//19.提问下发0x8302
	BOOL	OnINFO_MENU_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//21.信息点播菜单设置0x8303
	BOOL	OnINFO_SERVICE(PBYTE pBodyBuffer, WORD wBodyLength);			//23.信息服务0x8304
	BOOL	OnCALL_BACK(PBYTE pBodyBuffer, WORD wBodyLength);				//24.电话回拨0x8400
	BOOL	OnPHONE_BOOK_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//25.设置电话本0x8401
	BOOL	OnCAR_CONTROL(PBYTE pBodyBuffer, WORD wBodyLength);				//26.车辆控制0x8500
	BOOL	OnCIRC_REGION_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//28.设置圆形区域0x8600
	BOOL	OnCIRC_REGION_DEL(PBYTE pBodyBuffer, WORD wBodyLength);			//29.删除圆形区域0x8601

	BOOL	OnRECT_REGION_SET(PBYTE pBodyBuffer, WORD wBodyLength);			//30.设置矩形区域0x8602
	BOOL	OnRECT_REGION_DEL(PBYTE pBodyBuffer, WORD wBodyLength);			//31.删除矩形区域0x8603
	BOOL	OnROUTE_SET(PBYTE pBodyBuffer, WORD wBodyLength);				//34.设置路线0x8606
	BOOL	OnROUTE_DEL(PBYTE pBodyBuffer, WORD wBodyLength);				//35.删除路线0x8607

	BOOL	OnMULTIMEDIA_UPLOAD_ACK(PBYTE pBodyBuffer, WORD wBodyLength);	//43.多媒体数据上传应答0x8800
	BOOL	OnCAMERA_PHOTOGRAPH(PBYTE pBodyBuffer, WORD wBodyLength,BOOL bAuto = FALSE);		//44.摄像头立即拍摄命令0x8801
	BOOL	OnMULTIMEDIA_UPLOAD(PBYTE pBodyBuffer, WORD wBodyLength);		//47.存储多媒体数据上传命令0x8803
	
	BOOL	OnTIME_Adjust(PBYTE pBodyBuffer, WORD wBodyLength);				//时间校准

	void	ResetOS();
	void	DevicePoweroff();
	BOOL	FtpConnect();

	VOID	DeleteUserRoadFile(DWORD dwRoadID);

	void	LoadPhotoNum();

	static DWORD THDPhotoUpload(LPVOID lp);
	static DWORD THDManualPhotoUpload(LPVOID lp);
	static DWORD THDStartGuider(LPVOID lp);

	//拍照相关
	WORD m_wPhotoCmd;
	WORD m_wPhotoTime;
	bool m_bIsAutoPhoto;

	DWORD m_dwRoadID;  //记录的线路ID
	

private:
	//
	BOOL	GetDataRegion(UCHAR *pucTempData, int nTempLenght, int& nIndexStart, int& nIndexEnd);
	//协议数据标识位转义
	//BOOL	DataTranscoding(UCHAR *pucDstData, int *nDstDataLenght, UCHAR *pucSrcData, int nSrcDataLenght);
	INT		DataDecoding(PBYTE pcBuffer, int nBufferLen);
	//消息头
	BOOL	MsgHeadBodyAnalysis(PBYTE pBuffer, int nBufferLength);
	//消息体
	BOOL	MsgBodyAnalysis(UCHAR *pucBuffer, int nBufferLength);
	//设置终端参数
	BOOL	SetParam(DWORD wParamID, DWORD dwParam, LPSTR lpParam, WORD wParamLen);
	//解析无线升级参数
	BOOL	GetUpgradeParam(PBYTE pBuffer, int nBufferLength);
	//解析控制终端连接指定服务器参数
	BOOL	GetConnectParam(PBYTE pBuffer, int nBufferLength);

	BYTE    CalcXor(BYTE *pbySrc, WORD wLenSrc);


	
	static DWORD DowLoadThread(LPVOID lparam);

	HINTERNET m_hConnection;

	CString m_strFtpIP;
	WORD	m_wFtpPort;
	CString m_strUserName;
	CString m_strPassword;


	
	static DWORD THDAppExit(LPVOID lpParameter);

	PROCESS_INFORMATION		m_sProcessInfo;			//进程启动信息


	WORD   m_VideoTime;

	
	BYTE byTerminalParam[NUM_1K];

	WORD wTerminalParamLen;
	ROUTE_LIST m_RouteList;



public:
	//大端模式(big-endian)的网络字节序传送字和双字
	BOOL	Str2Int(PBYTE pBuffer, WORD wLen, WORD& wValue);
	BOOL	Str2Int(PBYTE pBuffer, WORD wLen, DWORD& dwValue);
	DWORD	Str2Int(PBYTE pBuffer, WORD wLen);
	//获取消息头结构体
	const PMSG_HEAD		GetMsgHead();
	//获取注册应答结构体
	const PREGISTER_ACK	GetRegisterAck();
	//获取通用应答结构体
	const PCOMMON_ACK	GetCommonAck();
	//获取临时位置跟踪控制结构体
	const PTEMP_POS_TRACE	GetTempPosTrace();

public:


};
