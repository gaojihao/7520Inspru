/************************************************************************
* Copyright (C)2011, 上海鲲博通讯技术有限公司
* All rights reserved.
* 
* 文件名称: DataParser.cpp

* 文件标识: 
* 摘    要: 

* 作    者: 
* 完成日期: 2011/09/26
* 备注: 
************************************************************************/

#include "stdafx.h"
#include "DataParser.h"
#include <afxinet.h>
#include "../Tools/LogTool.h"
#include "../Lib/IniManager.h"
#include "../Lib/midware.h"
#include "../TerminalConfig/TerminalConfig.h"
#include "../MsgProc/MsgProc.h"
#include "../SQLite3/SQLite3Class.h"
#include "../DataParser/DataPack.h"
#include "../GPSData/GPSData.h"
#include <Tlhelp32.h>


extern void TTSSpeaking(CString csText);
//#define MSG_WND_NAME _T("Guider")	// 消息接收窗口名称



//初始化静态成员变量
CDataParser*	CDataParser::m_pInstance = NULL;
#define SAFE_FREE(x)			if(x){LocalFree(x);x=NULL;}


/************************************************************************
* 函数名:	DataParser
* 描  述:	构造函数
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
CDataParser::CDataParser(void)
{
	memset(&m_stMsgHead, NULL, sizeof(m_stMsgHead));			//数据头
	memset(&m_stCommonAck, NULL, sizeof(m_stCommonAck));		//平台通用应答0x8001
	memset(&m_stRegisterAck, NULL, sizeof(m_stRegisterAck));	//终端注册应答0x8100
	memset(&m_stTemPosTrace, NULL, sizeof(m_stTemPosTrace));	//临时位置跟踪控制0x8202
	memset(byTerminalParam, 0, sizeof(byTerminalParam));
	wTerminalParamLen = 0;
	m_iMaxStopCar = 0;
	m_hConnection = NULL;
	m_bIsAutoPhoto = false;

	m_dwRoadID = 0;
}

/************************************************************************
* 函数名: DataParser
* 描  述: 析构函数
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
CDataParser::~CDataParser(void)
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/************************************************************************
* 函数名:	Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注:	singleton
************************************************************************/
CDataParser* CDataParser::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CDataParser;
	}
	return m_pInstance;
}

/************************************************************************
* 函数名:	DataParser
* 描  述:	解析协议消息
* 入  参:	1. ucTempData:	要解析的协议消息字符串
*			2. wTempLenght:	要解析的协议消息字符串的长度
* 出  参: 
* 返  回:	VOID
* 备  注: 
************************************************************************/
BOOL CDataParser::DataParser(UCHAR *pucTempData, WORD wTempLenght)
{
	if (NULL == pucTempData || 0 == wTempLenght)
		return FALSE;
	bool bend = true;
	int nIndexStart	= -1;	//协议头0x7E的index
	int nIndexEnd	= -1;	//协议尾0x7E的index

	CLogTool::Instance()->WriteLogFile(_T("Recv"), pucTempData, wTempLenght);

	for (int i = 0; i < wTempLenght; i++)
	{
		if( 0x7E == pucTempData[i] )	//找到协议标志
		{
			bend = !bend;

			if (!bend)
			{
				nIndexStart = i;
			}
			else
			{
				nIndexEnd = i;
				DataParserPot(&pucTempData[nIndexStart],nIndexEnd-nIndexStart+1);
			}
		}
	}

	return TRUE;
}

BOOL CDataParser::DataParserPot(UCHAR *pucTempData, WORD wTempLenght)
{
	CDataPack::Instance()->SetSendCountZero();

	//ucData数组中储存协议头0x7E和协议尾0x7E之间的数据(头标识位，消息头，消息体，检验码，尾标识位)，包括0x7E
	UCHAR ucData[MSG_BUFFER_LEN];
	memset(ucData, 0, sizeof(ucData));
	memcpy(ucData, pucTempData, wTempLenght);

	//标识位转义处理:0x7D02->0x7E	0x7D01->0x7D
	int nLenght = DataDecoding(ucData, wTempLenght);
	if( -1 == nLenght )
		return FALSE;

	//解析消息头和消息体
	return MsgHeadBodyAnalysis(&ucData[1], nLenght-3);

}

/************************************************************************
* 函数名:	MsgHeadBodyAnalysis
* 描  述:	解析协议消息头和消息体
* 入  参:	1. pucBuffer:		消息头+消息体字符串
*			2. nBufferLength:	消息头+消息体的长度，不能小于12byte(消息头除消息包封装项的长度)
* 出  参: 
* 返  回:	TRUE:解析成功	FALSE:解析失败
* 备  注:	消息头的长度应12或16byte
************************************************************************/
BOOL CDataParser::MsgHeadBodyAnalysis(PBYTE pBuffer, int nBufferLength)
{
	if (NULL == pBuffer || nBufferLength<12)
		return FALSE;

	memset(&m_stMsgHead, 0, sizeof(m_stMsgHead));
	int nOffset = 0;

	/************消息头******************/
	//消息ID
	m_stMsgHead.wMsgID = (pBuffer[nOffset]<<8)+pBuffer[nOffset+1];
	nOffset += 2;
	//消息体属性
	WORD wTemp = (pBuffer[nOffset]<<8)+pBuffer[nOffset+1];
	nOffset += 2;
	//消息体长度
//	m_stMsgHead.stProperty.wMsgBodyLength = wTemp & 0x03FF;
//	CLogTool::Instance()->WriteLogFile(_T("recvlen:%d"), m_stMsgHead.stProperty.wMsgBodyLength);
	//数据加密方式	第10~12位都为0表示不加密，第10位为1时表示RSA算法加密，其它暂时保留
	//在此解析处理为第10位为1时表示RSA算法加密，0时表示不加密
	m_stMsgHead.stProperty.ucEncryptionType = (wTemp>>10) & 0x0001;
	//分包
	m_stMsgHead.stProperty.ucSubPack = (wTemp>>13) & 0x0001;
	//终端手机号，例：0x01 34 39 00 41 54 -> "13439004154"
	m_stMsgHead.ucPhoneNumber[0] = (pBuffer[nOffset++] & 0x0F) + '0';
	m_stMsgHead.ucPhoneNumber[1] = (pBuffer[nOffset]>>4) + '0';
	m_stMsgHead.ucPhoneNumber[2] = (pBuffer[nOffset++] & 0x0F) + '0';
	m_stMsgHead.ucPhoneNumber[3] = (pBuffer[nOffset]>>4) + '0';
	m_stMsgHead.ucPhoneNumber[4] = (pBuffer[nOffset++] & 0x0F) + '0';
	m_stMsgHead.ucPhoneNumber[5] = (pBuffer[nOffset]>>4) + '0';
	m_stMsgHead.ucPhoneNumber[6] = (pBuffer[nOffset++] & 0x0F) + '0';
	m_stMsgHead.ucPhoneNumber[7] = (pBuffer[nOffset]>>4) + '0';
	m_stMsgHead.ucPhoneNumber[8] = (pBuffer[nOffset++] & 0x0F) + '0';
	m_stMsgHead.ucPhoneNumber[9] = (pBuffer[nOffset]>>4) + '0';
	m_stMsgHead.ucPhoneNumber[10] = (pBuffer[nOffset++] & 0x0F) + '0';
	//流水号
	m_stMsgHead.wMsgSerialNum = (pBuffer[nOffset]<<8)+pBuffer[nOffset+1];

	
	nOffset += 2;

	//ucSubPack字段为1时才有消息包封装项
	if( m_stMsgHead.stProperty.ucSubPack )
	{
		if( nBufferLength < 16 )	//有分包时，长度不能小于16
			return FALSE;

		//消息包
		m_stMsgHead.stPackItem.wMsgPackNum = (pBuffer[nOffset]<<8)+pBuffer[nOffset+1];
		nOffset += 2;
		//包序号
		m_stMsgHead.stPackItem.wMsgPackIndex = (pBuffer[nOffset]<<8)+pBuffer[nOffset+1];
		nOffset += 2;
	}

	CLogTool::Instance()->WriteLogFile(_T("CMD ID:%x"), m_stMsgHead.wMsgID);
	/************消息体******************/
	//服务器平台(SerVeR)消息ID
	switch( m_stMsgHead.wMsgID )
	{
	case MSG_SVR_COMMON_ACK:				//2.平台通用应答0x8001
		return OnCOMMON_ACK(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_SUBCONTRACT:				//3.补传分包请求0x8003
		return OnSUBCONTRACT(&pBuffer[nOffset], nBufferLength-nOffset);				
	case MSG_SVR_REGISTER_ACK:				//5.终端注册应答0x8100
		return OnREGISTER_ACK(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_PARAM_SET:					//8.设置终端参数0x8103
		return OnPARAM_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_PARAM_GET:					//9.查询终端参数0x8104
		return OnPARAM_GET();
	case MSG_SVR_TMN_CONTROL:				//11.终端控制0x8105
		return OnTMN_CONTROL(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_TMN_APPOINT_PARAMETER:		//12.查询指定终端参数
		return OnTMN_APPOINTPARAM(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_TMN_ATTRIBUTE:				//.查询终端属性
		return OnTMN_ATTRIBUTE(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_POS_GET:					//13.位置信息查询0x8201
		return OnPOS_GET(nBufferLength-nOffset);
	case MSG_SVR_TEMP_POS_TRACE:			//15.临时位置跟踪控制0x8202
		return OnTEMP_POS_TRACE(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_ALARM_SMS:					//...人工确认报警消息
		return OnTEMP_ALARM_SMS(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_TEXT_SMS:					//16.文本信息下发0x8300
		return OnTEXT_SMS(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_EVENT_SET:					//17.事件设置0x8301
		return OnEVENT_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_QUESTION:					//19.提问下发0x8302
		return OnQUESTION(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_INFO_MENU_SET:				//21.信息点播菜单设置0x8303
		return OnINFO_MENU_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_INFO_SERVICE:				//23.信息服务0x8304
		return OnINFO_SERVICE(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_CALL_BACK:					//24.电话回拨0x8400
		return OnCALL_BACK(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_PHONE_BOOK_SET:			//25.设置电话本0x8401
		return OnPHONE_BOOK_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_CAR_CONTROL:				//26.车辆控制0x8500
		return OnCAR_CONTROL(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_CIRC_REGION_SET:			//28.设置圆形区域0x8600
		return OnCIRC_REGION_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_CIRC_REGION_DEL:			//29.删除圆形区域0x8601
		return OnCIRC_REGION_DEL(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_RECT_REGION_SET:			//30.设置矩形区域0x8602
		return OnRECT_REGION_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_RECT_REGION_DEL:			//31.删除矩形区域0x8603
		return OnRECT_REGION_DEL(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_ROUTE_SET:					//34.设置路线0x8606
		return OnROUTE_SET(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_ROUTE_DEL:					//35.删除路线0x8607
		return OnROUTE_DEL(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_MULTIMEDIA_UPLOAD_ACK:		//43.多媒体数据上传应答0x8800
		//return OnMULTIMEDIA_UPLOAD_ACK(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_CAMERA_PHOTOGRAPH:			//44.摄像头立即拍摄命令0x8801
		return OnCAMERA_PHOTOGRAPH(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_MULTIMEDIA_UPLOAD:			//47.存储多媒体数据上传命令0x8803
		return OnMULTIMEDIA_UPLOAD(&pBuffer[nOffset], nBufferLength-nOffset);
	case MSG_SVR_TIME_ADJUST:
		return OnTIME_Adjust(&pBuffer[nOffset], nBufferLength-nOffset);
	default:	return FALSE;
	}
}

/************************************************************************
* 函数名:	DataDecoding
* 描  述:	标识位转义，将源pcBuffer里面的相应字符转换:0x7D02->0x7E	0x7D01->0x7D
* 入  参:	1. pcBuffer:	目标协议消息字符串
*			2. nBufferLen:	目标协议消息字符串的长度
* 出  参: 
* 返  回:	INT:	返回转换后的buffer长度，小于源长度，-1代表操作失败
* 备  注:	0x30 7D 02 08 7D 01 55 -> 0x30 7E 08 7D 55
************************************************************************/
INT CDataParser::DataDecoding(PBYTE pcBuffer, int nBufferLen)
{
	if (NULL == pcBuffer || 0 == nBufferLen)
	{
		return -1;
	}

	int nDstLen = nBufferLen;	//在源buffer的空间上转换，nDstLen为转换之后buffer的长度

	for(int i=0; i<nDstLen; i++)
	{
		if (i >= nDstLen-1)
			break;
		//0x7D02->0x7E
		if (0x7D == pcBuffer[i] && 0x02 == pcBuffer[i+1])
		{
			pcBuffer[i] = 0x7E;
			for(int j=i+1; j<nDstLen-1; j++)
				pcBuffer[j] = pcBuffer[j+1];

			nDstLen--;
		}
		//0x7D01->0x7D
		else if (0x7D == pcBuffer[i] && 0x01 == pcBuffer[i+1])
		{
			pcBuffer[i] = 0x7D;
			for(int j=i+1; j<nDstLen-1; j++)
				pcBuffer[j] = pcBuffer[j+1];

			nDstLen--;
		}
	}
	return nDstLen;
}

/************************************************************************
* 函数名:	GetMsgHead
* 描  述:	获取消息头结构体
* 入  参:
* 出  参:
* 返  回:
* 备  注: 
************************************************************************/
const PMSG_HEAD CDataParser::GetMsgHead()
{
	return &m_stMsgHead;
}

/************************************************************************
* 函数名:	GetRegisterAck
* 描  述:	获取注册应答结构体
* 入  参:
* 出  参:
* 返  回:
* 备  注: 
************************************************************************/
const PREGISTER_ACK CDataParser::GetRegisterAck()
{
	return &m_stRegisterAck;
}

/************************************************************************
* 函数名:	GetCommonAck
* 描  述:	获取通用应答结构体
* 入  参:
* 出  参:
* 返  回:
* 备  注: 
************************************************************************/
const PCOMMON_ACK CDataParser::GetCommonAck()
{
	return &m_stCommonAck;
}

/************************************************************************
* 函数名:	GetTempPosTrace
* 描  述:	获取临时位置跟踪控制结构体
* 入  参:
* 出  参:
* 返  回:
* 备  注: 
************************************************************************/
const PTEMP_POS_TRACE CDataParser::GetTempPosTrace()
{
	return &m_stTemPosTrace;
}

/************************************************************************
* 函数名:	GetDataRegion
* 描  述:	检查协议头标识位和尾标识位
* 入  参:	1. ucTempData:	协议消息字符串
*			2. nTempLenght:	协议消息字符串的长度
* 出  参:	nIndexStart:		第一个0x7E的index
*			nIndexEnd:		第二个0x7E的index
* 返  回:	TRUE:找到协议头0x7E和尾0x7E		FALSE:没找到
* 备  注: 
************************************************************************/
BOOL CDataParser::GetDataRegion(UCHAR *pucTempData, int nTempLenght, int& nIndexStart, int& nIndexEnd)
{
	int i;
	//初始化index, -1表示没找到
	nIndexStart = -1;
	nIndexEnd	= -1;

	for(i=0; i<nTempLenght-1 && nIndexStart==-1; i++)
	{
		if( 0x7E == pucTempData[i] )	//找到协议头
		{
			nIndexStart = i;
			break;
		}
	}
	//没有找到协议头
	if(-1 == nIndexStart)
		return FALSE;

	for(i=nIndexStart+1; i<nTempLenght && nIndexEnd==-1; i++)
	{
		if( 0x7E == pucTempData[i] )	//找到协议尾
		{
			nIndexEnd = i;
			break;
		}
	}

	//没有找到协议头
	if(-1 == nIndexStart)
		return FALSE;

	//数据体少于一个字节，则失败
	if( nIndexEnd <= nIndexStart+1 )
		return FALSE;

	return TRUE;
}

/************************************************************************
* 函数名:	OnCOMMON_ACK
* 描  述:	平台通用应答0x8001的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. wBodyLength:	消息体的长度:2+2+1
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注: 
************************************************************************/
BOOL CDataParser::OnCOMMON_ACK(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer/* || wBodyLength != 5*/ )
		return FALSE;

	memset(&m_stCommonAck, NULL, sizeof(m_stCommonAck));
	m_stCommonAck.wAckSerialNum	= (pBodyBuffer[0]<<8) + pBodyBuffer[1];
	m_stCommonAck.wAckMsgID		= (pBodyBuffer[2]<<8) + pBodyBuffer[3];
	m_stCommonAck.eRslt			= (enResult)pBodyBuffer[4];	//0:成功/确认	1:失败	2:消息有误	3:不支持  4:报警处理确认

	unCOMMON_ACK_RESULT common_result;
	common_result.ack_result.wMsgID		= m_stCommonAck.wAckMsgID; //应答ID
	common_result.ack_result.wResult	= m_stCommonAck.eRslt;		   //结果

	if(RESULT_SUCCESS == m_stCommonAck.eRslt)
	{
		CLogTool::Instance()->WriteLogFile("Server common ack--MsgID: 0x%04X, SerialNum: 0x%04X, result: success", 
			m_stCommonAck.wAckMsgID, m_stCommonAck.wAckSerialNum);

		
		if (m_stCommonAck.wAckMsgID == MSG_TMN_POS_REPORT )
		{
			g_alarm_state.unAlarm.alarm_flag.cRegionInOut = 0;//收到平台应答后清零
			g_alarm_state.unAlarm.alarm_flag.cEmergency   = 0;//收到平台应答后清零
			g_alarm_state.unAlarm.alarm_flag.cCollisionAlarm = 0;
			g_alarm_state.unAlarm.alarm_flag.cHitAlarm = 0;
			g_alarm_state.unAlarm.alarm_flag.cIllegalDisplacement = 0;
			g_alarm_state.unAlarm.alarm_flag.cGPSSignIn = 0;

			CDataPack::Instance()->SendBlindAreaData();
		}
		else if (m_stCommonAck.wAckMsgID == MSG_TMN_MULTIMEDIA_UPLOAD)
		{
			CDataPack::Instance()->UploadPhotoPack();
		}

		CMsgProc::Instance()->SendServerMsg(MSG_SVR_COMMON_ACK, RESULT_SUCCESS);
		return TRUE;
	}
	else if(RESULT_FAILURE == m_stCommonAck.eRslt)
	{
		CLogTool::Instance()->WriteLogFile("Server common ack--MsgID: 0x%04X, SerialNum: 0x%04X, result: failure", 
			m_stCommonAck.wAckMsgID, m_stCommonAck.wAckSerialNum);

		CMsgProc::Instance()->SendServerMsg(MSG_SVR_COMMON_ACK, RESULT_FAILURE);
		return FALSE;
	}
	else if(RESULT_MSG_ERROR == m_stCommonAck.eRslt)
	{
		CLogTool::Instance()->WriteLogFile("Server common ack--MsgID: 0x%04X, SerialNum: 0x%04X, result: msg_error", 
			m_stCommonAck.wAckMsgID, m_stCommonAck.wAckSerialNum);

		CMsgProc::Instance()->SendServerMsg(MSG_SVR_COMMON_ACK, RESULT_MSG_ERROR);
		return FALSE;
	}
	else if(RESULT_UNSUPPORT == m_stCommonAck.eRslt)
	{
		CLogTool::Instance()->WriteLogFile("Server common ack--MsgID: 0x%04X, SerialNum: 0x%04X, result: unsupported", 
			m_stCommonAck.wAckMsgID, m_stCommonAck.wAckSerialNum);

		//CMsgProc::Instance()->SendMsg2Menu( MSG_SVR_COMMON_ACK, common_result.dwAck_result );
		CMsgProc::Instance()->SendServerMsg(MSG_SVR_COMMON_ACK, RESULT_UNSUPPORT);
		return FALSE;
	}
	else if(RESULT_ALARM_CONFIRM == m_stCommonAck.eRslt)
	{
		CLogTool::Instance()->WriteLogFile("Server common ack--MsgID: 0x%04X, SerialNum: 0x%04X, result: alarm confirm", 
			m_stCommonAck.wAckMsgID, m_stCommonAck.wAckSerialNum);
		//CMsgProc::Instance()->SendMsg2Menu( MSG_SVR_COMMON_ACK, common_result.dwAck_result );
		CMsgProc::Instance()->SendServerMsg(MSG_SVR_COMMON_ACK, RESULT_ALARM_CONFIRM);
		return FALSE;
	}

	return FALSE;
}

/************************************************************************
* 函数名:	OnREGISTER_ACK
* 描  述:	终端注册应答0x8100的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. wBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注: 
************************************************************************/
BOOL CDataParser::OnREGISTER_ACK(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength < 3 )
		goto END_FAILURE;

	memset(&m_stRegisterAck, NULL, sizeof(m_stRegisterAck));
	m_stRegisterAck.wAckSerialNum	= (pBodyBuffer[0]<<8) + pBodyBuffer[1];
	m_stRegisterAck.wResult			= pBodyBuffer[2];
	//只有成功后才有该字段
	if(0 == m_stRegisterAck.wResult)
	{
		if(wBodyLength < 4)
			goto END_ERROR;

		memcpy(m_stRegisterAck.cAuthenCode, &pBodyBuffer[3], wBodyLength-3);
		CLogTool::Instance()->WriteLogFile("Register success--AuthenCode:%s", m_stRegisterAck.cAuthenCode);

		CIniFile IniFile;

		//加载文件失败
		if( !IniFile.Load( PATH_CONFIG_INI ) )
			goto END_FAILURE;

		//URL地址
		CHAR		szTemp[512];
		CString		strTemp;
		
		strTemp = m_stRegisterAck.cAuthenCode;
		memset(szTemp, NULL, sizeof(szTemp));
		//strncpy(szTemp, m_stRegisterAck.cAuthenCode, sizeof(m_stRegisterAck.cAuthenCode));
		//strTemp	= szTemp;
		IniFile.SetString(_T("Set"), _T("AuthenCode"), strTemp);
		IniFile.Flush();
//		IniFile.Release();

		goto END_SUCCESS;
	}
	//车辆已被注册
	else if(1 == m_stRegisterAck.wResult)
	{
		if(wBodyLength < 4)
			goto END_ERROR;

		memcpy(m_stRegisterAck.cAuthenCode, &pBodyBuffer[3], wBodyLength-3);
		CLogTool::Instance()->WriteLogFile("车辆已被注册--AuthenCode:%s", m_stRegisterAck.cAuthenCode);
		goto END_SUCCESS;
	}
	//数据库中无该车辆
	else if(2 == m_stRegisterAck.wResult)
	{
		CLogTool::Instance()->WriteLogFile("数据库中无该车辆!");
		goto END_FAILURE;
	}
	//终端已被注册
	else if(3 == m_stRegisterAck.wResult)
	{
		if(wBodyLength < 4)
			goto END_ERROR;

		memcpy(m_stRegisterAck.cAuthenCode, &pBodyBuffer[3], wBodyLength-3);
		CLogTool::Instance()->WriteLogFile("终端已被注册--AuthenCode:%s", m_stRegisterAck.cAuthenCode);
		goto END_SUCCESS;
	}
	//数据库中无该终端
	else if(4 == m_stRegisterAck.wResult)
	{
		CLogTool::Instance()->WriteLogFile("数据库中无该终端!");
		goto END_FAILURE;
	}
	else
	{
		goto END_ERROR;
	}

END_SUCCESS:
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_REGISTER_ACK, RESULT_SUCCESS);
	return TRUE;

END_FAILURE:
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_REGISTER_ACK, RESULT_FAILURE);
	return FALSE;

END_ERROR:
	CLogTool::Instance()->WriteLogFile("协议错误!");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_REGISTER_ACK, RESULT_MSG_ERROR);
	return FALSE;
}

/************************************************************************
* 函数名:	OnPARAM_SET
* 描  述:	设置终端参数0x8103的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. wBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnPARAM_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength < 3 )
		goto END_FAILURE;

	memset(byTerminalParam, 0, sizeof(byTerminalParam));
	wTerminalParamLen = wBodyLength;

	memcpy(byTerminalParam, pBodyBuffer, wBodyLength);

	WORD wOffset	= 0;	//buffer位移

	DWORD	dwParamID	= 0;	//参数ID
	WORD	wParamLen	= 0;	//参数长度
	DWORD	wParamValue	= 0;	//参数值
	CHAR	szParamValue[128];	//参数值

	//参数总数
	WORD wParamNum = pBodyBuffer[wOffset++];
	CLogTool::Instance()->WriteLogFile("Set Param Num: %u", wParamNum);
	for(int i=0; i<wParamNum; i++)
	{
		//包括参数ID和参数长度，至少有5个字节
		if(wOffset+5 >= wBodyLength)
			goto END_FAILURE;

		//解析参数ID
		dwParamID = (pBodyBuffer[wOffset]<<24) + (pBodyBuffer[wOffset+1]<<16) + (pBodyBuffer[wOffset+2]<<8) + pBodyBuffer[wOffset+3];
		wOffset += 4;

		//解析参数长度
		wParamLen	= pBodyBuffer[wOffset++];

		
		memset(szParamValue, NULL, sizeof(szParamValue));
		strncpy(szParamValue, (char*)&pBodyBuffer[wOffset], wParamLen);	//存储字符串值
		if(4 == wParamLen)		//DWORD值
			wParamValue = (pBodyBuffer[wOffset]<<24) + (pBodyBuffer[wOffset+1]<<16) + (pBodyBuffer[wOffset+2]<<8) + pBodyBuffer[wOffset+3];
		else if(2 == wParamLen)	//WORD值
			wParamValue = (pBodyBuffer[wOffset]<<8) + pBodyBuffer[wOffset+1];

		wOffset += wParamLen;

		//保存参数到CTerminalConfig的结构体m_TerminalParam中
		SetParam(dwParamID, wParamValue, szParamValue, wParamLen);
	}
	CTerminalConfig::Instance()->SaveConfigFile();	//保存设置的参数到文件
	goto END_SUCCESS;

END_SUCCESS:
	CLogTool::Instance()->WriteLogFile("Set Param Success");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_PARAM_SET, RESULT_SUCCESS);
	return TRUE;

END_FAILURE:
	CLogTool::Instance()->WriteLogFile("Set Param Fail");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_PARAM_SET, RESULT_FAILURE);
	return FALSE;
}

/************************************************************************
* 函数名:	OnPARAM_GET
* 描  述:	查询终端参数0x8104的消息处理函数
* 入  参:	1.wBodyLength: 消息体长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:	该消息的消息体为空
************************************************************************/
BOOL CDataParser::OnPARAM_GET()
{
	CLogTool::Instance()->WriteLogFile("Get Param(0x8104) Success");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_PARAM_GET, RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnTMN_CONTROL
* 描  述:	终端控制0x8105的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注: 
************************************************************************/
BOOL CDataParser::OnTMN_CONTROL(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength < 1 )
		return FALSE;

	WORD wOffset	= 0;						//buffer位移
	BYTE cCmd		= pBodyBuffer[wOffset++];	//命令字
	BOOL bOK = FALSE;

	CLogTool::Instance()->WriteLogFile("Get TMN_CONTROL(0x8105) Success CMD:%d", cCmd);

	switch( cCmd )
	{
	case CONTROL_WIRELESSUPGRADE:	//无线升级
		GetUpgradeParam( &pBodyBuffer[wOffset], wBodyLength-wOffset );
		bOK = FtpConnect();
		break;
	case CONTROL_CONNECT2SERVER:	//连接指定服务器
		break;
	case CONTROL_POWEROFF:			//终端关机
		ResetOS();
		break;
	case CONTROL_RESET:				//终端复位
		TTSSpeaking(_T("终端即将重启"));
		ResetOS();
		break;
	case CONTROL_RESTORE_DEFAULT:	//终端回复出厂设置
		break;
	case CONTROL_CLOSE_GPRS:		//关闭数据通信
		break;
	case CONTROL_WIRELESS:			//关闭所有无线通信
		break;
	default:
		return FALSE;
	}

	if (bOK)
	{
		CMsgProc::Instance()->SendServerMsg(MSG_SVR_TMN_CONTROL, RESULT_SUCCESS);
	}
	else
	{
		CMsgProc::Instance()->SendServerMsg(MSG_SVR_TMN_CONTROL, RESULT_FAILURE);
	}

		return TRUE;
}

/************************************************************************
* 函数名:	OnPOS_GET
* 描  述:	位置信息查询0x8201的消息处理函数
* 入  参: 
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:	该消息的消息体为空
************************************************************************/
BOOL CDataParser::OnPOS_GET(WORD wBodyLength)
{
	//该消息的消息体为空
	if( wBodyLength != 0)
		goto END_FAILURE;
	else
		goto END_SUCCESS;

END_SUCCESS:
	CLogTool::Instance()->WriteLogFile("Get Position(0x8201) Success");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_POS_GET, RESULT_SUCCESS);
	return TRUE;

END_FAILURE:
	CLogTool::Instance()->WriteLogFile("Get Position(0x8201) Fail");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_POS_GET, RESULT_FAILURE);
	return FALSE;
}

/************************************************************************
* 函数名:	OnREGISTER_ACK
* 描  述:	临时位置跟踪控制0x8202的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:2+4
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnTEMP_POS_TRACE(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength != 6 )
		return FALSE;

	memset(&m_stTemPosTrace, NULL, sizeof(m_stTemPosTrace));
	m_stTemPosTrace.wEnalbe				= 1;	//使有效
	m_stTemPosTrace.wSlice				= (pBodyBuffer[0]<<8) + pBodyBuffer[1];
	m_stTemPosTrace.dwPosTraceValidity	= (pBodyBuffer[2]<<24) + (pBodyBuffer[3]<<16) + (pBodyBuffer[4]<<8) + pBodyBuffer[5];

	CLogTool::Instance()->WriteLogFile("Temp position trace(0x8202)--Slice:%d(s/t),Validity:%d(s)", 
		m_stTemPosTrace.wSlice, m_stTemPosTrace.dwPosTraceValidity);
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_TEMP_POS_TRACE, RESULT_SUCCESS);

	return TRUE;
}

/************************************************************************
* 函数名:	OnTEXT_SMS
* 描  述:	文本信息下发0x8300的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:1+n
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnTEXT_SMS(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength < 1 )
		return FALSE;

	TEXT_SMS	stText;
	memset(&stText, NULL, sizeof(stText));
	stText.unFlag.cFlag	= pBodyBuffer[0];
	if(wBodyLength-1 > NUM_1K)
		memcpy(stText.cText, &pBodyBuffer[1], NUM_1K);
	else
		memcpy(stText.cText, &pBodyBuffer[1], wBodyLength-1);

	//写入数据库
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	char szSql[NUM_2K];	
	memset(szSql, 0, sizeof(szSql));
	sprintf(szSql, "INSERT INTO text_info(text_type, flag, text_content) VALUES(%d, %d, '%s');", 
		0, stText.unFlag.cFlag, stText.cText);
	db.execDML(szSql);

	db.execDML("DELETE FROM text_info WHERE UID NOT IN (SELECT UID FROM text_info ORDER BY UID DESC LIMIT 36);");


	db.close();

	char szbuffer[NUM_1K];
	memset(szbuffer, 0, sizeof(szbuffer));
	memcpy(szbuffer,stText.cText,wBodyLength-1);

	TCHAR tchar[NUM_1K];
	memset(tchar, 0, sizeof(tchar));

	MultiByteToWideChar( CP_ACP, 0, szbuffer,strlen(szbuffer)+1, tchar, sizeof(tchar)/sizeof(tchar[0]) );

	CString strtemp = _T("");
	strtemp.Format(_T("%s"),tchar);

	
	TTSSpeaking(strtemp);
	CLogTool::Instance()->WriteLogFile("Text SMS(0x8300)--Flag:0x%02X, Text:%s", stText.unFlag.cFlag, stText.cText);
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_TEXT_SMS, RESULT_SUCCESS);

	return TRUE;
}

/************************************************************************
* 函数名:	OnEVENT_SET
* 描  述:	事件设置0x8301的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:1+n
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnEVENT_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnQUESTION
* 描  述:	提问下发0x8302的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnQUESTION(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnINFO_MENU_SET
* 描  述:	信息点播菜单设置0x8303的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnINFO_MENU_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnINFO_SERVICE
* 描  述:	信息服务0x8304的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:1+2+2+n
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnINFO_SERVICE(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnCALL_BACK
* 描  述:	电话回拨0x8400的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnCALL_BACK(PBYTE pBodyBuffer, WORD wBodyLength)
{
	CLogTool::Instance()->WriteLogFile("CALL_BACK(0x8400)");
	CppSQLite3DB db;
	if( NULL == pBodyBuffer || wBodyLength < 3 || wBodyLength > 21)
		goto END_ERROR;

	WORD	wOffset		= 0;		//buffer位移
	BYTE	cFlag		= 0;		//标志
	CHAR	szBuffer[NUM_256B];		//信息内容
	memset(szBuffer, NULL, sizeof(szBuffer));

	
	char szSql[NUM_1K];
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	//标志	0:普通通话; 1:监听
	cFlag	= pBodyBuffer[wOffset++];
	//电话号码
	memcpy(szBuffer, &pBodyBuffer[wOffset], wBodyLength-wOffset);
	wOffset	= wBodyLength;
	sprintf(szSql, "INSERT INTO call_back(flag, phone_number) VALUES(%d, '%s');", 
		cFlag, szBuffer);

	db.execDML(szSql);
	db.close();

	
	CLogTool::Instance()->WriteLogFile("Callback--cFlag:%d, phone numb:%s", cFlag, szBuffer);
	goto END_SUCCESS;

END_SUCCESS:
	CLogTool::Instance()->WriteLogFile("Parse success");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_CALL_BACK, RESULT_SUCCESS);
	CMsgProc::Instance()->SendMsg2Menu(MSG_SVR_CALL_BACK, cFlag);
	return TRUE;

END_ERROR:
	CLogTool::Instance()->WriteLogFile("Parse error");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_CALL_BACK, RESULT_MSG_ERROR);
	return FALSE;
}

/************************************************************************
* 函数名:	OnPHONE_BOOK_SET
* 描  述:	设置电话本0x8401的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnPHONE_BOOK_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnCAR_CONTROL
* 描  述:	车辆控制0x8500的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnCAR_CONTROL(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength != 1 )
		return FALSE;
	return TRUE;
}

/************************************************************************
* 函数名:	OnCIRC_REGION_SET
* 描  述:	设置圆形区域0x8600的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnCIRC_REGION_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer )
		return FALSE;

	CMsgProc::Instance()->SendServerMsg(MSG_SVR_CIRC_REGION_SET, RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnCIRC_REGION_DEL
* 描  述:	删除圆形区域0x8601的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnCIRC_REGION_DEL(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer )
		return FALSE;

	CMsgProc::Instance()->SendServerMsg(MSG_SVR_CIRC_REGION_DEL, RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnRECT_REGION_SET
* 描  述:	设置矩形区域0x8602的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnRECT_REGION_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_RECT_REGION_SET, RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnRECT_REGION_DEL
* 描  述:	删除矩形区域0x8603的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnRECT_REGION_DEL(PBYTE pBodyBuffer, WORD wBodyLength)
{
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_RECT_REGION_DEL, RESULT_SUCCESS);
	return TRUE;
}

bool cmp(const RouteSet_Data& r1,const RouteSet_Data& r2)
{
	return r1.wPackIndex < r2.wPackIndex;
}
/************************************************************************
* 函数名:	OnROUTE_SET
* 描  述:	设置路线0x8606的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnROUTE_SET(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if (pBodyBuffer == NULL || wBodyLength == 0)
	{
		return FALSE;
	}
//	CLogTool::Instance()->WriteLogFile(_T("Route set:"),pBodyBuffer,wBodyLength);

	CLogTool::Instance()->WriteLogFile(_T("set Route"));
	HWND hwnd = NULL;

	//关闭摄像头
	hwnd = ::FindWindow(NULL,WNDTITLE_CAMERA_VIDEO);
	if (NULL != hwnd)
	{
		CLogTool::Instance()->WriteLogFile(_T("send message to MSG_COLSE_VIDEO"));
		::PostMessage(hwnd,MSG_COLSE_VIDEO, NULL, NULL);
	}
	Sleep(1);

	char sz_data[1024];
	memset(sz_data, 0, sizeof(sz_data) );

	memcpy(sz_data, pBodyBuffer, wBodyLength );

	
	ROUTE_INFO   RoutInfo;
	memset(&RoutInfo,0,sizeof(RoutInfo));

	int noffset = 0;

	if (m_stMsgHead.stProperty.ucSubPack)//判断有分包则为路径导航
	{
		if (m_stMsgHead.stPackItem.wMsgPackIndex == 1) //第一包
		{
			DWORD dwRoutID = (pBodyBuffer[0]<<24) + (pBodyBuffer[1]<<16) + (pBodyBuffer[2]<<8) + pBodyBuffer[3]; //线路ID
			m_dwRoadID = dwRoutID; //保存线路ID

			//保存路径数据
			CFile file;
			if (m_dwRoadID == 0)
			{
				file.Open(PATH_ROUTE1_TXT, CFile::modeCreate | CFile::modeReadWrite);
			}
			else if (m_dwRoadID == 1)
			{
				file.Open(PATH_ROUTE2_TXT, CFile::modeCreate | CFile::modeReadWrite);
			}
			else if (m_dwRoadID == 2)
			{
				file.Open(PATH_ROUTE3_TXT, CFile::modeCreate | CFile::modeReadWrite);
			}
			else
			{
				return FALSE;
			}


			Sleep(500);
			file.Write(sz_data,wBodyLength);
			Sleep(1);
			file.Flush();
			Sleep(1);
			file.Close();
			m_RouteList.clear();
			Sleep(1000);

			CMsgProc::Instance()->SendServerMsg(MSG_SVR_ROUTE_SET, RESULT_SUCCESS);

		}
		else //分包
		{
			CMsgProc::Instance()->SendServerMsg(MSG_SVR_ROUTE_SET, RESULT_SUCCESS);

			RouteSet_Data RouteData; 
			RouteData.wPackIndex = m_stMsgHead.stPackItem.wMsgPackIndex;
			RouteData.wBodylen = wBodyLength;
			memcpy(RouteData.nRouteData, sz_data, wBodyLength);

			m_RouteList.push_back(RouteData);
			Sleep(100);
		}
		CLogTool::Instance()->WriteLogFile(_T("PackNum:%d,PackIndex:%d"), m_stMsgHead.stPackItem.wMsgPackNum,m_stMsgHead.stPackItem.wMsgPackIndex);

		if (m_RouteList.size() >= m_stMsgHead.stPackItem.wMsgPackNum-1) //收到完整数据
		{
			CFile file;
			if (m_dwRoadID == 0)
			{
				file.Open(PATH_ROUTE1_TXT, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
			}
			else if (m_dwRoadID == 1)
			{
				file.Open(PATH_ROUTE2_TXT, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
			}
			else if (m_dwRoadID == 2)
			{
				file.Open(PATH_ROUTE3_TXT, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
			}
			else
			{
				return FALSE;
			}

			m_RouteList.sort(cmp);

			for( ROUTE_LIST::iterator iter = m_RouteList.begin(); iter != m_RouteList.end(); iter++ )
			{
				Sleep(50);
				file.SeekToEnd();
				CLogTool::Instance()->WriteLogFile(_T("PackIndex:%d"),iter->wPackIndex);
				file.Write(iter->nRouteData,iter->wBodylen);
				Sleep(1);
				file.Flush();
				Sleep(1);
			}
			file.Close();

			CreateThread(NULL, 0, THDStartGuider, this, 0, NULL);
		}
	}
	else //无分包
	{
		CMsgProc::Instance()->SendServerMsg(MSG_SVR_ROUTE_SET, RESULT_SUCCESS);

		hwnd = ::FindWindow(NULL, MSG_WND_NAME); //打开导航软件
		if (NULL == hwnd)
		{
			TCHAR bCmdline[100];
			LPCWSTR srCmdLine = bCmdline;

			BOOL bProcessState = CreateProcess( _T("\\Storage Card\\Guider4u\\guider.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &m_sProcessInfo);
			if (bProcessState)
			{
				CreateThread(NULL, 0, THDAppExit, this, 0, NULL);
			}

			for(int i = 0; i < 1 * 60; i++ )
			{
				Sleep(1000);
				hwnd = ::FindWindow(NULL, MSG_WND_NAME);
				if ( hwnd ) 
					break;
			}
		}

		DWORD dwRoutID = (pBodyBuffer[0]<<24) + (pBodyBuffer[1]<<16) + (pBodyBuffer[2]<<8) + pBodyBuffer[3]; //线路ID
		WORD  dRoutAttribute = (pBodyBuffer[4]<<8) + pBodyBuffer[5];//路线属性
		WORD dRoutInflexionNum = (pBodyBuffer[18]<<8) + pBodyBuffer[19];//路线总拐点个数
		noffset = 20;

		for (int i = 0; i < dRoutInflexionNum; i++)
		{
			RoutInfo.dwInflexionID =  (pBodyBuffer[noffset]<<24) + (pBodyBuffer[noffset+1]<<16) 
				+ (pBodyBuffer[noffset+2]<<8) + pBodyBuffer[noffset+3];

			noffset += 4;
			RoutInfo.dwRouteID =  (pBodyBuffer[noffset]<<24) + (pBodyBuffer[noffset+1]<<16) 
				+ (pBodyBuffer[noffset+2]<<8) + pBodyBuffer[noffset+3];

			noffset += 4;

			RoutInfo.dwInflexionLat = (pBodyBuffer[noffset]<<24) + (pBodyBuffer[noffset+1]<<16) 
				+ (pBodyBuffer[noffset+2]<<8) + pBodyBuffer[noffset+3];

			noffset += 4;

			RoutInfo.dwInflexionLong = (pBodyBuffer[noffset]<<24) + (pBodyBuffer[noffset+1]<<16) 
				+ (pBodyBuffer[noffset+2]<<8) + pBodyBuffer[noffset+3];
			noffset += 13;

			CLogTool::Instance()->WriteLogFile(_T("Route Attribute: %d, %d,%d"), dRoutAttribute,RoutInfo.dwInflexionLat,RoutInfo.dwInflexionLong);
		}

		if ( ((dRoutAttribute>>12)&0x01) == 1)  //一键导航
		{
			if (hwnd)
			{
				float fPointLat = RoutInfo.dwInflexionLat*1e-6;
				float fPointLong = RoutInfo.dwInflexionLong*1e-6;

				wchar_t buf[256] = {0};
				wsprintf(buf, L"SET_DESTINATION,%f,%f,%s", fPointLong,fPointLat, _T("导航终点") );
				COPYDATASTRUCT cpd;
				cpd.dwData = 0;
				cpd.cbData = wcslen(buf)*sizeof(wchar_t) + 2;
				cpd.lpData = (PVOID)buf;

				::SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cpd);
			}
		}
		else //if (((dRoutAttribute>>13)&0x01) == 1) //路线导航
		{
			//保存路径数据
			CFile file;
			if (m_dwRoadID == 0)
			{
				file.Open(PATH_ROUTE1_TXT, CFile::modeCreate | CFile::modeReadWrite);
			}
			else if (m_dwRoadID == 1)
			{
				file.Open(PATH_ROUTE2_TXT, CFile::modeCreate | CFile::modeReadWrite);
			}
			else if (m_dwRoadID == 2)
			{
				file.Open(PATH_ROUTE3_TXT, CFile::modeCreate | CFile::modeReadWrite);
			}
			else
			{
				return FALSE;
			}

			Sleep(500);
			file.Write(sz_data,wBodyLength);
			Sleep(1);
			file.Flush();
			Sleep(1);
			file.Close();

			if (hwnd != NULL)
			{
				DeleteUserRoadFile(m_dwRoadID);
				CLogTool::Instance()->WriteLogFile(_T("PostMessage to Guider"));
				::PostMessage(hwnd,MSG_TO_GUIDER4U,m_dwRoadID,0);
			}
		}
	}

//	Sleep(200);
	

	return TRUE;																				   		
}																		   	
																		   	
/************************************************************************   			
* 函数名:	OnROUTE_DEL													   		
* 描  述:	删除路线0x8607的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnROUTE_DEL(PBYTE pBodyBuffer, WORD wBodyLength)
{

//	CGPSData::Instance()->DeleteRoute();
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_ROUTE_DEL, RESULT_SUCCESS);

	return TRUE;
}


BYTE CDataParser::CalcXor(BYTE *pbySrc, WORD wLenSrc)
{

	char byTemp = pbySrc[0];
	for(int i=1; i<wLenSrc; i++)
	{
		byTemp ^= pbySrc[i];
	}
	return byTemp;
}
/************************************************************************
* 函数名:	OnMULTIMEDIA_UPLOAD_ACK
* 描  述:	多媒体数据上传应答0x8800的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnMULTIMEDIA_UPLOAD_ACK(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnCAMERA_PHOTOGRAPH
* 描  述:	摄像头立即拍摄命令0x8801的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnCAMERA_PHOTOGRAPH(PBYTE pBodyBuffer, WORD wBodyLength,BOOL bAuto)
{

	if( NULL == pBodyBuffer || wBodyLength < 7)
		return FALSE;


	WORD	wPhotoCmd = (pBodyBuffer[1]<<8) + pBodyBuffer[2];
	WORD	wPhotoTime = (pBodyBuffer[3]<<8) + pBodyBuffer[4];

	if (wPhotoCmd == 0) //停止拍摄
	{
		m_bIsAutoPhoto = false;
		CLogTool::Instance()->WriteLogFile(_T("exit photoupload thread"));

		//CppSQLite3DB db;
		//char szSql[512];
		//memset(szSql, 0,sizeof(szSql));
		//db.open(PATH_SQLITE_DB_808);	//打开数据库
		//sprintf(szSql, "update photo_Info set photo_num = %d, photo_time = %d;", 0, 0);
		//db.execDML(szSql);
		//db.close();
	}
	else//拍一张或连拍
	{

		if (wPhotoTime == 0)//拍一张
		{
			CreateThread(NULL, 0, THDManualPhotoUpload, this, 0, NULL);
		}
		else //连拍
		{
			m_wPhotoCmd = wPhotoCmd;
			m_wPhotoTime = wPhotoTime;

			CppSQLite3DB db;
			char szSql[512];
			memset(szSql, 0,sizeof(szSql));
			db.open(PATH_SQLITE_DB_808);	//打开数据库
			sprintf(szSql, "update photo_Info set photo_num = %d, photo_time = %d;", m_wPhotoCmd, m_wPhotoTime);
			db.execDML(szSql);
			db.close();

			if (m_bIsAutoPhoto)  //拍照线程已在运行
			{
				CLogTool::Instance()->WriteLogFile(_T("photoupload thread is running"));
				CMsgProc::Instance()->SendServerMsg(MSG_SVR_CAMERA_PHOTOGRAPH, RESULT_SUCCESS);
				return FALSE;
			}

			m_bIsAutoPhoto = true;
			Sleep(1);
			CLogTool::Instance()->WriteLogFile(_T("OnCAMERA_PHOTOGRAPH start photoupload thread"));

			CreateThread(NULL, 0, THDPhotoUpload, this, 0, NULL);
		}
	}

	CLogTool::Instance()->WriteLogFile("CAMERA_PHOTOGRAPH(0x0801)");
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_CAMERA_PHOTOGRAPH, RESULT_SUCCESS);

	return TRUE;

}

DWORD CDataParser::THDManualPhotoUpload(LPVOID lp)
{
	TCHAR tcpath[256];
	DWORD dwPhotoID = 1;

	memset (tcpath, 0, sizeof(tcpath) );
	wsprintf(tcpath, _T("\\Flashdrv Storage\\%d.jpg"), dwPhotoID);

	if (GetFileAttributes(tcpath) != 0xFFFFFFFF)
	{
		DeleteFile(tcpath);
	}

	CamPohto(tcpath, CRect(0,0,320,240));

	CLogTool::Instance()->WriteLogFile(_T("take photo"));
	CDataPack::Instance()->PackMULTIMEDIA_UPLOAD(dwPhotoID);

	return 0;
}

DWORD CDataParser::THDPhotoUpload(LPVOID lp)
{
	CDataParser *inst = (CDataParser*)lp;
	DWORD dwPhotoID = 0;

	CLogTool::Instance()->WriteLogFile(_T("THDPhotoUpload start"));

	while(inst->m_bIsAutoPhoto)
	{

		if (CMsgProc::Instance()->GetLoginState())
		{

			dwPhotoID++;

			TCHAR tcpath[256];

			memset (tcpath, 0, sizeof(tcpath) );
			wsprintf(tcpath, _T("\\Flashdrv Storage\\%d.jpg"), dwPhotoID);

			if (GetFileAttributes(tcpath) != 0xFFFFFFFF)
			{
				DeleteFile(tcpath);
			}

			CamPohto(tcpath, CRect(0,0,320,240));

			CLogTool::Instance()->WriteLogFile(_T("take photo"));
			CDataPack::Instance()->PackMULTIMEDIA_UPLOAD(dwPhotoID);

		}

		Sleep(inst->m_wPhotoTime * 1000);
	}

	CLogTool::Instance()->WriteLogFile(_T("THDPhotoUpload end"));

	return 0;
}
/************************************************************************
* 函数名:	OnMULTIMEDIA_UPLOAD
* 描  述:	存储多媒体数据上传命令0x8803的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnMULTIMEDIA_UPLOAD(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

BOOL CDataParser::OnTIME_Adjust(PBYTE pBodyBuffer, WORD wBodyLength)
{
	if( NULL == pBodyBuffer || wBodyLength < 6)
		return FALSE;

	if (g_bAdjust)
	{
		return TRUE;
	}

	CLogTool::Instance()->WriteLogFile("Time_Adjust");
	
	SYSTEMTIME sys_time;
	SYSTEMTIME	loc_time;
	memset(&sys_time, NULL, sizeof(SYSTEMTIME));
	GetLocalTime(&loc_time);

	sys_time.wYear = 2000+( (pBodyBuffer[0]/16)*10 + pBodyBuffer[0]%16 );
	sys_time.wMonth = (pBodyBuffer[1]/16)*10 + pBodyBuffer[1]%16;
	sys_time.wDay = (pBodyBuffer[2]/16)*10 + pBodyBuffer[2]%16;
	sys_time.wHour = (pBodyBuffer[3]/16)*10 + pBodyBuffer[3]%16;
	sys_time.wMinute = (pBodyBuffer[4]/16)*10 + pBodyBuffer[4]%16;
	sys_time.wSecond = (pBodyBuffer[5]/16)*10 + pBodyBuffer[5]%16;

	if (loc_time.wYear != sys_time.wYear)
	{
		SetLocalTime(&sys_time);
	}

	return TRUE;

}


/************************************************************************
* 函数名:	SetParam
* 描  述:	平台RSA公钥0x8A00的消息处理函数
* 入  参:	1. pBodyBuffer:	消息体字符串
*			2. nBodyLength:	消息体的长度
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::SetParam(DWORD wParamID, DWORD dwParam, LPSTR lpParam, WORD wParamLen)
{
	if(4==wParamLen || 2==wParamLen)
		CLogTool::Instance()->WriteLogFile("Set Param--ParamID: 0x%04X, Param value: %u", wParamID, dwParam);
	else
		CLogTool::Instance()->WriteLogFile("Set Param--ParamID: 0x%04X, Param value: %-16.16s", wParamID, lpParam);

	//获取终端参数结构体
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();

	switch(wParamID)
	{
	case PARAM_HEART_BEAT_SLICE:				//(0x0001)终端心跳发送间隔，单位为秒(s)
		pParam->dwHEART_BEAT_SLICE = dwParam;
		break;
	case PARAM_TCP_ACK_OVERTIME:				//(0x0002)TCP消息应答超时时间，单位为秒(s)
		pParam->dwTCP_ACK_OVERTIME = dwParam;
		break;
	case PARAM_TCP_RETRANSMIT_COUNT:			//(0x0003)TCP消息重传次数
		pParam->dwTCP_RETRANSMIT_COUNT = dwParam;
		break;
	case PARAM_UDP_ACK_OVERTIME:				//(0x0004)UDP消息应答超时时间，单位为秒(s)
		pParam->dwUDP_ACK_OVERTIME = dwParam;
		break;
	case PARAM_UDP_RETRANSMIT_COUNT:			//(0x0005)UDP消息重传次数
		pParam->dwUDP_RETRANSMIT_COUNT = dwParam;
		break;
	case PARAM_SMS_ACK_OVERTIME:				//(0x0006)SMS消息应答超时时间，单位为秒(s)
		pParam->dwSMS_ACK_OVERTIME = dwParam;
		break;
	case PARAM_SMS_RETRANSMIT_COUNT:			//(0x0007)SMS消息重传次数
		pParam->dwSMS_RETRANSMIT_COUNT = dwParam;
		break;
		//0x0008~0x000F		保留
	case PARAM_MAIN_SVR_APN:					//(0x0010)主服务器APN
		strncpy( pParam->szMAIN_SVR_APN, lpParam, wParamLen );
		break;
	case PARAM_MAIN_SVR_NAME:					//(0x0011)主服务器无线通信拨号用户名
		strncpy( pParam->szMAIN_SVR_NAME, lpParam, wParamLen );
		break;
	case PARAM_MAIN_SVR_PWD:					//(0x0012)主服务器无线通信拨号密码
		strncpy( pParam->szMAIN_SVR_PWD, lpParam, wParamLen );
		break;
	case PARAM_MAIN_SVR_IP:						//(0x0013)主服务器地址，IP或域名
		strncpy( pParam->szMAIN_SVR_IP, lpParam, wParamLen );
		break;
	case PARAM_BAK_SVR_APN:						//(0x0014)备份服务器APN
		strncpy( pParam->szBAK_SVR_APN, lpParam, wParamLen );
		break;
	case PARAM_BAK_SVR_NAME:					//(0x0015)备份服务器无线通信拨号用户名
		strncpy( pParam->szBAK_SVR_NAME, lpParam, wParamLen );
		break;
	case PARAM_BAK_SVR_PWD:						//(0x0016)备份服务器无线通信拨号密码
		strncpy( pParam->szBAK_SVR_PWD, lpParam, wParamLen );
		break;
	case PARAM_BAK_SVR_IP:						//(0x0017)备份服务器地址，IP或域名
		strncpy( pParam->szBAK_SVR_IP, lpParam, wParamLen );
		break;
	case PARAM_SVR_TCP_PORT:					//(0x0018)服务器TCP端口
		pParam->dwSVR_TCP_PORT = dwParam;
		break;
	case PARAM_SVR_UDP_PORT:					//(0x0019)服务器UDP端口
		pParam->dwSVR_UDP_PORT = dwParam;
		break;
		//0x001A~0x001F		保留
	case PARAM_POS_REPORT_TYPE:					//(0x0020)位置汇报策略
		pParam->dwPOS_REPORT_TYPE = dwParam;
		break;
	case PARAM_POS_REPORT_CONDITION:			//(0x0021)位置汇报方案
		pParam->dwPOS_REPORT_CONDITION = dwParam;
		break;
	case PARAM_POS_REPORT_SLICE_LOGOUT:			//(0x0022)驾驶员未登录汇报时间间隔，单位为秒(s), >0
		pParam->dwPOS_REPORT_SLICE_LOGOUT = dwParam;
		break;
		//0x0023~0x0026		保留
	case PARAM_POS_REPORT_SLICE_SLEEP:			//(0x0027)休眠时汇报时间间隔，单位为秒(s), >0
		pParam->dwPOS_REPORT_SLICE_SLEEP = dwParam;
		break;
	case PARAM_POS_REPORT_SLICE_ALARM:			//(0x0028)紧急报警时汇报时间间隔，单位为秒(s), >0
		pParam->dwPOS_REPORT_SLICE_ALARM = dwParam;
		break;
	case PARAM_POS_REPORT_SLICE_DEFAULT:		//(0x0029)缺省时间汇报间隔，单位为秒(s), >0
		pParam->dwPOS_REPORT_SLICE_DEFAULT = dwParam;
		break;
		//0x002A~0x002B		保留
	case PARAM_POS_REPORT_DIST_DEFAULT:			//(0x002C)缺省距离汇报间隔，单位为米(m), >0
		pParam->dwPOS_REPORT_DIST_DEFAULT = dwParam;
		break;
	case PARAM_POS_REPORT_DIST_LOGOUT:			//(0x002D)驾驶员未登录汇报距离间隔，单位为米(m), >0
		pParam->dwPOS_REPORT_DIST_LOGOUT = dwParam;
		break;
	case PARAM_POS_REPORT_DIST_SLEEP:			//(0x002E)休眠时汇报距离间隔，单位为米(m), >0
		pParam->dwPOS_REPORT_DIST_SLEEP = dwParam;
		break;
	case PARAM_POS_REPORT_DIST_ALARM:			//(0x002F)紧急报警时汇报距离间隔，单位为米(m), >0
		pParam->dwPOS_REPORT_DIST_ALARM = dwParam;
		break;
	case PARAM_CORNER_RETRANSMIT_ANGLE:			//(0x0030)拐点补传角度，<180
		pParam->dwCORNER_RETRANSMIT_ANGLE = dwParam;
		break;
	case PARAM_ELECTRONIC_FENCE_RADIUS:			//(0x0031)电子围栏半径（非法位移阈值），单位为米  Radius of the electronic fence
		pParam->wELECTRONIC_FENCE_RADIUS = (WORD)dwParam;

		CDataPack::Instance()->SetillegalDisplace(pParam->wELECTRONIC_FENCE_RADIUS);
		break;
		//0x0032~0x003F		保留
	case PARAM_PHONE_NUM_MONITOR:				//(0x0040)监控平台电话号码
		strncpy( pParam->szPHONE_NUM_MONITOR, lpParam, wParamLen );
		break;
	case PARAM_PHONE_NUM_RESET:					//(0x0041)复位电话号码
		strncpy( pParam->szPHONE_NUM_RESET, lpParam, wParamLen );
		break;
	case PARAM_PHONE_NUM_RESTORE:				//(0x0042)恢复出厂设置电话号码
		strncpy( pParam->szPHONE_NUM_RESTORE, lpParam, wParamLen );
		break;
	case PARAM_PHONE_NUM_SVR_SMS:				//(0x0043)监控平台SMS电话号码
		strncpy( pParam->szPHONE_NUM_SVR_SMS, lpParam, wParamLen );
		break;
	case PARAM_PHONE_NUM_TMN_SMS:				//(0x0044)接收终端SMS文本报警号码
		strncpy( pParam->szPHONE_NUM_TMN_SMS, lpParam, wParamLen );
		break;
	case PARAM_ANSWER_PHONE_TYPE:				//(0x0045)终端电话接听策略
		pParam->dwANSWER_PHONE_TYPE = dwParam;
		break;
	case PARAM_SINGLE_CALL_TIME_LIMIT:			//(0x0046)每次最长通话时间
		pParam->dwSINGLE_CALL_TIME_LIMIT = dwParam;
		break;
	case PARAM_MONTH_CALL_TIME_LIMIT:			//(0x0047)当月最长通话时间
		pParam->dwMONTH_CALL_TIME_LIMIT = dwParam;
		break;
	case PARAM_MONITOR_PHONE_NUM:				//(0x0048)监听电话号码
		strncpy( pParam->szMONITOR_PHONE_NUM, lpParam, wParamLen );
		break;
	case PARAM_SVR_PRIVILEGE_SMS_NUM:			//(0x0049)监管平台特权短信号码
		strncpy( pParam->szSVR_PRIVILEGE_SMS_NUM, lpParam, wParamLen );
		break;
		//0x004A~0x004F		保留
	case PARAM_ALARM_MASK_BIT:					//(0x0050)报警屏蔽字
		pParam->dwALARM_MASK_BIT = dwParam;
		break;
	case PARAM_ALARM_SEND_SMS_SWITCH:			//(0x0051)报警发送文本SMS开关
		pParam->dwALARM_SEND_SMS_SWITCH = dwParam;
		break;
	case PARAM_ALARM_CAPTURE_SWITCH:			//(0x0052)报警拍摄开关
		pParam->dwALARM_CAPTURE_SWITCH = dwParam;
		break;
	case PARAM_ALARM_CAPTURE_STORE_FLAG:		//(0x0053)报警拍摄存储标志
		pParam->dwALARM_CAPTURE_STORE_FLAG = dwParam;
		break;
	case PARAM_KEY_FLAG:						//(0x0054)关键标志
		pParam->dwKEY_FLAG = dwParam;
		break;
	case PARAM_SPEED_LIMIT:						//(0x0055)最高速度
		pParam->dwSPEED_LIMIT = dwParam;
		break;
	case PARAM_OVERSPEED_DUREATION:				//(0x0056)超速持续时间
		pParam->dwOVERSPEED_DUREATION = dwParam;
		break;
	case PARAM_CONTINUE_DRIVE_TIME_LIMIT:		//(0x0057)连续驾驶时间门限
		pParam->dwCONTINUE_DRIVE_TIME_LIMIT = dwParam;
		break;
	case PARAM_ONE_DAY_DRIVE_TIME_LIMIT:		//(0x0058)当天累计驾驶时间门限
		pParam->dwONE_DAY_DRIVE_TIME_LIMIT = dwParam;
		break;
	case PARAM_MINIMUM_REST_TIME:				//(0x0059)最小休息时间
		pParam->dwMINIMUM_REST_TIME = dwParam;
		break;
	case PARAM_MAXIMUM_PARKING_TIME:			//(0x005A)最长停车时间
		pParam->dwMAXIMUM_PARKING_TIME = dwParam;
		m_iMaxStopCar = dwParam;
		break;
		//0x005B~0x006F		保留
	case PARAM_TIMER_TAKEPHOTO:
		//pParam->dwTimer_Take_Photo = dwParam;
		break;
	case PARAM_IMAGE_VIDEO_QUALITY:				//(0x0070)图像/视频质量，1～10，1最好
		pParam->dwIMAGE_VIDEO_QUALITY = dwParam;
		break;
	case PARAM_BRIGHTNESS:						//(0x0071)亮度，0～255
		pParam->dwBRIGHTNESS = dwParam;
		break;
	case PARAM_CONTRAST:						//(0x0072)对比度，0～127
		pParam->dwCONTRAST = dwParam;
		break;
	case PARAM_SATURATION:						//(0x0073)饱和度，0～127
		pParam->dwSATURATION = dwParam;
		break;
	case PARAM_CHROMA:							//(0x0074)色度，0～255
		pParam->dwCHROMA = dwParam;
		break;
		//0x0075~0x007F		保留
	case PARAM_VEHICLE_ODOMETER:				//(0x0080)车辆里程表读数，1/10km
		pParam->dwVEHICLE_ODOMETER = dwParam;
		g_alarm_state.gps_extra_info.dwMileage = dwParam*3600;
		break;
	case PARAM_VEHICLE_PROVINCE_ID:				//(0x0081)车辆所在的省域ID
		pParam->wVEHICLE_PROVINCE_ID = (WORD)dwParam;
		break;
	case PARAM_VEHICLE_CITY_ID:					//(0x0082)车辆所在的市域ID
		pParam->wVEHICLE_CITY_ID = (WORD)dwParam;
		break;
	case PARAM_VEHICLE_ID:						//(0x0083)公安交通管理部门颁发的机动车号码
		strncpy( pParam->szVEHICLE_ID, lpParam, wParamLen );
		break;
	case PARAM_PLATE_COLOR:						//(0x0084)车牌颜色
		pParam->dwPLATE_COLOR = dwParam;
		break;
	case PARAM_GNSS_MODE:                       //GNSS定位模式
		pParam->byGnssfixMode = (BYTE)dwParam;
		break;
	case PARAM_GNSS_BANDRATE:					//GNSS波特率
		pParam->byGnssBandrate = (BYTE)dwParam;
		break;
	case PARAM_GNSS_OUTPUTRATE:					//GNSS输出频率
		pParam->byGnssfrequency = (BYTE)dwParam;
		break;
	case PARAM_GNSS_PICKUPRATE:					//GNSS数据采集频率
		pParam->dwGnssPickupfrequency = dwParam;
		break;
	case PARAM_GNSS_UPLOADMODE:					//GNSS数据上传方式
		pParam->byGnssUploadMode = (BYTE)dwParam;
		break;
	case PARAM_GNSS_PICKUPSET:					//GNSS数据上传设置
		pParam->dwGnssPickupSet = dwParam;
		break;
	case PARAM_CAN1_PICKYTIME:                  //CAN1采集间隔
		pParam->dwCan1PickTime = dwParam;
		break;
	case PARAM_CAN1_UPLOADTIME:					//CAN1上传间隔
		pParam->wCan1UploadTime = (WORD)dwParam;
		break;
	case PARAM_CAN2_PICKYTIME:					//CAN2采集间隔
		pParam->dwCan2PickTime = dwParam;
		break;
	case PARAM_CAN2_UPLOADTIME:					//CAN2上传间隔
		pParam->wCan2UploadTime = (WORD)dwParam;
		break;
	case PARAM_CAN_PICKUPSET:
		strncpy( pParam->byCanPickSet, lpParam, wParamLen );
		break;
		}
	return TRUE;
}

/************************************************************************
* 函数名:	GetUpgradeParam
* 描  述:	解析无线升级参数
* 入  参:	1. pBuffer:			消息体字符串
*			2. nBufferLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::GetUpgradeParam(PBYTE pBuffer, int nBufferLength)
{
	WORD		wFieldLen = 0;
	CIniFile	IniFile;
	CHAR		szTemp[512];
	CString		strTemp;

	WORD woffset = 0;
	m_strFtpIP	= _T("");
	m_wFtpPort	= 21;
	m_strUserName = _T("");
	m_strPassword = _T("");

	//加载文件失败
	IniFile.Load( PATH_CONFIG_INI );
	
	//URL地址
	PCSTR pcField	= ScanField((PCSTR)pBuffer, 0, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("URL"), strTemp);

	//拨号点名称
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("APN"), strTemp);

	//拨号用户名
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));

	//拨号密码
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));

	//地址
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	m_strFtpIP	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("Address"), m_strFtpIP);

	//TCP端口
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	m_wFtpPort = atoi(szTemp);
	IniFile.SetInt(_T("WirelessUpgrade"), _T("TCP_Port"), m_wFtpPort);
	

	//UDP端口
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	WORD wUdpPort = atoi(szTemp);
	IniFile.SetInt(_T("WirelessUpgrade"), _T("UDP_Port"), wUdpPort);

	//制造商ID
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("VenderID"), strTemp);

	//硬件版本
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("HardwareVersion"), strTemp);

	//固件版本
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	IniFile.SetString(_T("WirelessUpgrade"), _T("FirmwareVersion"), strTemp);


	//连接服务器时限
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);


	//FTP用户名
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	m_strUserName	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("UserName"), m_strUserName);


	////FTP用户名密码
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	m_strPassword	= szTemp;
	IniFile.SetString(_T("WirelessUpgrade"), _T("PWD"), m_strPassword);

	IniFile.Flush();
//	IniFile.Release();

	goto END_SUCCESS;

END_SUCCESS:
	return TRUE;
}

/************************************************************************
* 函数名:	GetConnectParam
* 描  述:	解析控制终端连接指定服务器参数
* 入  参:	1. pBuffer:			消息体字符串
*			2. nBufferLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::GetConnectParam(PBYTE pBuffer, int nBufferLength)
{
	WORD		wFieldLen = 0;
	CIniFile	IniFile;
	CHAR		szTemp[512];
	CString		strTemp;
	//加载文件失败
	if( !IniFile.Load( PATH_CONFIG_INI ) )
		goto END_FAILURE;

	//连接控制	0:切换到指定监管平台服务器  1:切换回原缺省监管平台服务器

	BYTE byControlType = pBuffer[0];

	IniFile.SetBool(_T("Connect2Server"), _T("ControlType"), byControlType);
	if (byControlType == 0x01)
	{
		return TRUE;
	}

	//监管平台鉴权码
	PCSTR pcField = ScanField((PCSTR)pBuffer, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("Connect2Server"), _T("AuthenCode"), strTemp);

	//拨号点名称
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("Connect2Server"), _T("APN"), strTemp);

	//拨号用户名
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("Connect2Server"), _T("UserName"), strTemp);

	//拨号密码
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("Connect2Server"), _T("PWD"), strTemp);

	//地址
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	strTemp	= szTemp;
	IniFile.SetString(_T("Connect2Server"), _T("Address"), strTemp);

	//TCP端口
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	WORD wTcpPort = (szTemp[0]<<8) + (szTemp[1]&0x00FF);
	IniFile.SetInt(_T("Connect2Server"), _T("TCP_Port"), wTcpPort);

	//UDP端口
	pcField	= ScanField(pcField, 1, wFieldLen, ';');
	memset(szTemp, NULL, sizeof(szTemp));
	strncpy(szTemp, pcField, wFieldLen);
	WORD wUdpPort = (szTemp[0]<<8) + (szTemp[1]&0x00FF);
	IniFile.SetInt(_T("Connect2Server"), _T("UDP_Port"), wUdpPort);


	WORD wTime = (pBuffer[nBufferLength-2]<<8) + (pBuffer[nBufferLength-1]&0x00FF);
	IniFile.SetInt(_T("Connect2Server"), _T("TimeLimit"), wTime);

	IniFile.Flush();
//	IniFile.Release();

	goto END_SUCCESS;

END_SUCCESS:
	return TRUE;

END_FAILURE:
	IniFile.Flush();
	return FALSE;
}

/************************************************************************
* 函数名:	Str2Int
* 描  述:	大端模式(big-endian)的网络字节序传送字和双字
* 入  参:	1. pBuffer:	保存数据的buffer
*			2. wLen:		
*			3. wValue:	返回的值
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CDataParser::Str2Int(PBYTE pBuffer, WORD wLen, WORD& wValue)
{
	if(NULL == pBuffer || wLen != 2)
		return FALSE;

	wValue	= (pBuffer[0]<<8) + pBuffer[0];
	return TRUE;
}

/************************************************************************
* 函数名:	Str2Int
* 描  述:	大端模式(big-endian)的网络字节序传送字和双字
* 入  参:	1. pBuffer:	保存数据的buffer
*			2. wLen:		
*			3. wValue:	返回的值
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CDataParser::Str2Int(PBYTE pBuffer, WORD wLen, DWORD& dwValue)
{
	if(NULL == pBuffer || wLen != 4)
		return FALSE;

	dwValue	= (pBuffer[0]<<24) + (pBuffer[1]<<16) + (pBuffer[2]<<8) + pBuffer[3];
	return TRUE;
}

/************************************************************************
* 函数名:	Str2Int
* 描  述:	大端模式(big-endian)的网络字节序传送字和双字
* 入  参:	1. pBuffer:	保存数据的buffer
*			2. wLen:		
* 出  参:	DWORD   返回的值
* 返  回: 
* 备  注: 
************************************************************************/
DWORD CDataParser::Str2Int(PBYTE pBuffer, WORD wLen)
{
	if(NULL == pBuffer || ( wLen != 2 && wLen != 4 ) )
		return 0;

	DWORD	dwValue = 0;
	for(WORD i=0; i<wLen; i++)
	{
		dwValue += ( pBuffer[i] << ( (wLen-i-1)*8 ) );
	}
	return dwValue;
}

/************************************************************************
* 函数名:	OnSUBCONTRACT
* 描  述:	补传分包请求
* 入  参:	1. pBuffer:			消息体字符串
*			2. nBufferLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnSUBCONTRACT(PBYTE pBodyBuffer, WORD wBodyLength)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnSUBCONTRACT
* 描  述:	查询指定终端参数
* 入  参:	1. pBuffer:			消息体字符串
*			2. nBufferLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnTMN_APPOINTPARAM(PBYTE pBodyBuffer, WORD wBodyLength)
{
	WORD		wOffset = 0;
	BYTE		ByTotalnum = 0;
	WORD        wMsgBodyLen = 0;

	ByTotalnum = pBodyBuffer[wOffset++];	//总包数

	BYTE byMsgBody[NUM_1K];
	memset(byMsgBody, 0, sizeof(byMsgBody));

	CIniFile	IniFile;
	//加载文件失败
	if( !IniFile.Load( PATH_CONFIG_INI ) )
		return FALSE;

	const PTERMINAL_PARAM	pParam		= CTerminalConfig::Instance()->GetTerminalParam();

	static WORD iSerNum = 2;

	CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen], iSerNum);
	iSerNum++;
	wMsgBodyLen += 2;

	byMsgBody[wMsgBodyLen++] = ByTotalnum;

	for (int i = 0; i < ByTotalnum; i++)
	{
		//解析参数ID
		DWORD dwParamID = (pBodyBuffer[wOffset]<<24) + (pBodyBuffer[wOffset+1]<<16) + (pBodyBuffer[wOffset+2]<<8) + pBodyBuffer[wOffset+3];
		wOffset += 4;
		CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen], dwParamID);
		wMsgBodyLen += 4;

		switch(dwParamID)
		{
		case PARAM_HEART_BEAT_SLICE:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwHEART_BEAT_SLICE);
			wMsgBodyLen += 4;
			break;
		case PARAM_TCP_ACK_OVERTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwTCP_ACK_OVERTIME);
			wMsgBodyLen += 4;
			break;
		case PARAM_TCP_RETRANSMIT_COUNT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwTCP_RETRANSMIT_COUNT);
			wMsgBodyLen += 4;
			break;
		case PARAM_UDP_ACK_OVERTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwUDP_ACK_OVERTIME);
			wMsgBodyLen += 4;
			break;
		case PARAM_UDP_RETRANSMIT_COUNT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwUDP_RETRANSMIT_COUNT);
			wMsgBodyLen += 4;
			break;
		case PARAM_SMS_ACK_OVERTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSMS_ACK_OVERTIME);
			wMsgBodyLen += 4;
			break;
		case PARAM_SMS_RETRANSMIT_COUNT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSMS_RETRANSMIT_COUNT);
			wMsgBodyLen += 4;
			break;
			//0x0008~0x000F		保留
		case PARAM_MAIN_SVR_APN:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szMAIN_SVR_APN, 8);
			wMsgBodyLen += 8;
			break;
		case PARAM_MAIN_SVR_NAME:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szMAIN_SVR_NAME, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_MAIN_SVR_PWD:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szMAIN_SVR_PWD, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_MAIN_SVR_IP:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szMAIN_SVR_IP, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_BAK_SVR_APN:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szBAK_SVR_APN, 8);
			wMsgBodyLen += 8;
			break;
		case PARAM_BAK_SVR_NAME:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szBAK_SVR_NAME, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_BAK_SVR_PWD:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szBAK_SVR_PWD, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_BAK_SVR_IP:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szBAK_SVR_IP, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_SVR_TCP_PORT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSVR_TCP_PORT);
			wMsgBodyLen += 4;
			break;
		case PARAM_SVR_UDP_PORT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSVR_UDP_PORT);
			wMsgBodyLen += 4;
			break;
			//0x001A~0x001F		保留
		case PARAM_POS_REPORT_TYPE:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_TYPE);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_CONDITION:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_CONDITION);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_SLICE_LOGOUT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_SLICE_LOGOUT);
			wMsgBodyLen += 4;
			break;
			//0x0023~0x0026		保留
		case PARAM_POS_REPORT_SLICE_SLEEP:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_SLICE_SLEEP);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_SLICE_ALARM:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_SLICE_ALARM);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_SLICE_DEFAULT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_SLICE_DEFAULT);
			wMsgBodyLen += 4;
			break;
			//0x002A~0x002B		保留
		case PARAM_POS_REPORT_DIST_DEFAULT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_DIST_DEFAULT);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_DIST_LOGOUT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_DIST_LOGOUT);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_DIST_SLEEP:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_DIST_SLEEP);
			wMsgBodyLen += 4;
			break;
		case PARAM_POS_REPORT_DIST_ALARM:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPOS_REPORT_DIST_ALARM);
			wMsgBodyLen += 4;
			break;
		case PARAM_CORNER_RETRANSMIT_ANGLE:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwCORNER_RETRANSMIT_ANGLE);
			wMsgBodyLen += 4;
			break;
		case PARAM_ELECTRONIC_FENCE_RADIUS:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->wELECTRONIC_FENCE_RADIUS);
			wMsgBodyLen += 2;
			break;
			//0x0032~0x003F		保留
		case PARAM_PHONE_NUM_MONITOR:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szPHONE_NUM_MONITOR, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_PHONE_NUM_RESET:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szPHONE_NUM_RESET, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_PHONE_NUM_RESTORE:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szPHONE_NUM_RESTORE, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_PHONE_NUM_SVR_SMS:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szPHONE_NUM_SVR_SMS, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_PHONE_NUM_TMN_SMS:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szPHONE_NUM_TMN_SMS, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_ANSWER_PHONE_TYPE:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwANSWER_PHONE_TYPE);
			wMsgBodyLen += 4;
			break;
		case PARAM_SINGLE_CALL_TIME_LIMIT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSINGLE_CALL_TIME_LIMIT);
			wMsgBodyLen += 4;
			break;
		case PARAM_MONTH_CALL_TIME_LIMIT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwMONTH_CALL_TIME_LIMIT);
			wMsgBodyLen += 4;
			break;
		case PARAM_MONITOR_PHONE_NUM:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szMONITOR_PHONE_NUM, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_SVR_PRIVILEGE_SMS_NUM:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szSVR_PRIVILEGE_SMS_NUM, 16);
			wMsgBodyLen += 16;
			break;
			//0x004A~0x004F		保留
		case PARAM_ALARM_MASK_BIT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwALARM_MASK_BIT);
			wMsgBodyLen += 4;
			break;
		case PARAM_ALARM_SEND_SMS_SWITCH:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwALARM_SEND_SMS_SWITCH);
			wMsgBodyLen += 4;
			break;
		case PARAM_ALARM_CAPTURE_SWITCH:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwALARM_CAPTURE_SWITCH);
			wMsgBodyLen += 4;
			break;
		case PARAM_ALARM_CAPTURE_STORE_FLAG:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwALARM_CAPTURE_STORE_FLAG);
			wMsgBodyLen += 4;
			break;
		case PARAM_KEY_FLAG:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwKEY_FLAG);
			wMsgBodyLen += 4;
			break;
		case PARAM_SPEED_LIMIT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSPEED_LIMIT);
			wMsgBodyLen += 4;
			break;
		case PARAM_OVERSPEED_DUREATION:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwOVERSPEED_DUREATION);
			wMsgBodyLen += 4;
			break;
		case PARAM_CONTINUE_DRIVE_TIME_LIMIT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwCONTINUE_DRIVE_TIME_LIMIT);
			wMsgBodyLen += 4;
			break;
		case PARAM_ONE_DAY_DRIVE_TIME_LIMIT:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwONE_DAY_DRIVE_TIME_LIMIT);
			wMsgBodyLen += 4;
			break;
		case PARAM_MINIMUM_REST_TIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwMINIMUM_REST_TIME);
			wMsgBodyLen += 4;
			break;
		case PARAM_MAXIMUM_PARKING_TIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwMAXIMUM_PARKING_TIME);
			wMsgBodyLen += 4;
			break;
			//0x005B~0x006F		保留
		case PARAM_IMAGE_VIDEO_QUALITY:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwIMAGE_VIDEO_QUALITY);
			wMsgBodyLen += 4;
			break;
		case PARAM_BRIGHTNESS:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwBRIGHTNESS);
			wMsgBodyLen += 4;
			break;
		case PARAM_CONTRAST:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwCONTRAST);
			wMsgBodyLen += 4;
			break;
		case PARAM_SATURATION:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwSATURATION);
			wMsgBodyLen += 4;
			break;
		case PARAM_CHROMA:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwCHROMA);
			wMsgBodyLen += 4;
			break;
			//0x0075~0x007F		保留
		case PARAM_VEHICLE_ODOMETER:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwVEHICLE_ODOMETER);
			wMsgBodyLen += 4;
			break;
		case PARAM_VEHICLE_PROVINCE_ID:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->wVEHICLE_PROVINCE_ID);
			wMsgBodyLen += 2;
			break;
		case PARAM_VEHICLE_CITY_ID:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->wVEHICLE_CITY_ID);
			wMsgBodyLen += 2;
			break;
		case PARAM_VEHICLE_ID:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->szVEHICLE_ID, 16);
			wMsgBodyLen += 16;
			break;
		case PARAM_PLATE_COLOR:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwPLATE_COLOR);
			wMsgBodyLen += 4;
			break;
		case PARAM_GNSS_MODE:
			byMsgBody[wMsgBodyLen++] = pParam->byGnssfixMode;
			break;
		case PARAM_GNSS_BANDRATE:
			byMsgBody[wMsgBodyLen++] = pParam->byGnssBandrate;
			break;
		case PARAM_GNSS_OUTPUTRATE:
			byMsgBody[wMsgBodyLen++] = pParam->byGnssfrequency;
			break;
		case PARAM_GNSS_PICKUPRATE:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwGnssPickupfrequency);
			wMsgBodyLen += 4;
			break;
		case PARAM_GNSS_UPLOADMODE:
			byMsgBody[wMsgBodyLen++] = pParam->byGnssUploadMode;
			break;
		case PARAM_GNSS_PICKUPSET:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwGnssPickupSet);
			wMsgBodyLen += 4;
			break;
		case PARAM_CAN1_PICKYTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwCan1PickTime);
			wMsgBodyLen += 4;
			break;
		case PARAM_CAN1_UPLOADTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->wCan1UploadTime);
			wMsgBodyLen += 2;
			break;
		case PARAM_CAN2_PICKYTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->dwCan2PickTime);
			wMsgBodyLen += 4;
			break;
		case PARAM_CAN2_UPLOADTIME:
			CDataPack::Instance()->Int2Str(&byMsgBody[wMsgBodyLen],pParam->wCan2UploadTime);
			wMsgBodyLen += 2;
			break;
		case PARAM_CAN_PICKUPSET:
			memcpy(&byMsgBody[wMsgBodyLen], pParam->byCanPickSet, 16);
			wMsgBodyLen += 16;
			break;
		}
	}

	IniFile.Flush();
//	IniFile.Release();

	CLogTool::Instance()->WriteLogFile("Report terminal param(0x0104)!");
	CDataPack::Instance()->BuildFullMsg(MSG_TMN_PARAM_GET_ACK, pBodyBuffer, wMsgBodyLen);
	return TRUE;
}

/************************************************************************
* 函数名:	OnTMN_ATTRIBUTE
* 描  述:	查询终端属性
* 入  参:	1. pBuffer:			消息体字符串
*			2. nBufferLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnTMN_ATTRIBUTE(PBYTE pBodyBuffer, WORD wBodyLength)
{
	CMsgProc::Instance()->SendServerMsg(MSG_SVR_TMN_ATTRIBUTE, RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnTMN_ATTRIBUTE
* 描  述:	人工确认报警消息
* 入  参:	1. pBuffer:			消息体字符串
*			2. nBufferLength:	消息体的长度:
* 出  参: 
* 返  回:	TRUE:解析成功		FALSE:解析失败
* 备  注:  
************************************************************************/
BOOL CDataParser::OnTEMP_ALARM_SMS(PBYTE pBodyBuffer, WORD wBodyLength)
{
	WORD		dwSerialnum = 0;
	DWORD		dwAlarmType = 0;
	WORD		wOffset = 0;

	dwSerialnum = (WORD)Str2Int(&pBodyBuffer[wOffset], 2); //报警消息流水号
	wOffset	+= 2;
	dwAlarmType = (DWORD)Str2Int(&pBodyBuffer[wOffset], 4);	//人工确认报警类型
	wOffset	+= 4;

	CMsgProc::Instance()->SendServerMsg(MSG_SVR_ALARM_SMS, RESULT_SUCCESS);
	return TRUE;
}


DWORD CDataParser::THDAppExit(LPVOID lpParameter)
{
	CDataParser *dataparse = (CDataParser *)lpParameter;

	DWORD  dwSig = WaitForSingleObject(dataparse->m_sProcessInfo.hProcess, INFINITE );
	switch( dwSig )
	{
	case WAIT_OBJECT_0:
		{
			HWND hwd = ::FindWindow( _T("HHTaskBar"), NULL );
			if( hwd != NULL )
				::ShowWindow( hwd, SW_HIDE );
		}
		break;
	case WAIT_TIMEOUT:
		break;
	default:
		break;
	}
	return 1;
}


void CDataParser::ResetOS()
{
	HANDLE h_handle = NULL;

	h_handle = CreateFile(TEXT("MID0:"), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h_handle == INVALID_HANDLE_VALUE)
		return;

	if(!DeviceIoControl(h_handle, IOCTRL_SYSTEM_RESET, NULL, 0, NULL, 0, NULL, NULL))
		return;

	CloseHandle(h_handle);
	h_handle = NULL;
}

void CDataParser::DevicePoweroff()
{
	HANDLE hVideoCamera = NULL;

	if(hVideoCamera == NULL)
		hVideoCamera = CreateFile(L"MID0:", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	DeviceIoControl(hVideoCamera, IOCTRL_DEVICE_PWROff, NULL, 0, NULL, 0, NULL, NULL);

	CloseHandle(hVideoCamera);
	hVideoCamera = NULL;
}

BOOL CDataParser::FtpConnect()
{
	HINTERNET m_hInet = NULL;

	//打开网络
	m_hInet = InternetOpen(_T("ftpDownload"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	//打开网络失败，回调函数返回错误信息，关闭连接，函数返回FALSE
	if(!m_hInet)
	{
		InternetCloseHandle(m_hInet); 
		return FALSE;
	}


	m_hConnection = InternetConnect(m_hInet,m_strFtpIP,m_wFtpPort,m_strUserName, m_strPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE , 0);
	


	//连接服务器失败，回调函数返回错误信息，关闭连接，函数返回FALSE
	if (m_hConnection	== NULL)
	{
		InternetCloseHandle(m_hInet);
		CLogTool::Instance()->WriteLogFile(_T("FTP Connect:%s  %d  %s  %s Failure"),m_strFtpIP, m_wFtpPort, m_strUserName, m_strPassword);
		return FALSE;
	}

	if (0xFFFFFFFF != GetFileAttributes(PATH_UPGRADE_ZIP))
	{
		::DeleteFile(PATH_UPGRADE_ZIP);
	}
	Sleep(500);

	CreateThread(NULL, 0, DowLoadThread, this, 0, NULL);

	return TRUE;
}

DWORD CDataParser::DowLoadThread(LPVOID lparam)
{
	CDataParser *dataparse = (CDataParser*)lparam;

	

	BOOL bFlagGetFile = false;
	HANDLE hFile  = NULL;

	hFile = CreateFile( PATH_UPGRADE_ZIP,  GENERIC_WRITE,  FILE_SHARE_READ,  NULL,  OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL, NULL);
	Sleep(1);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle( hFile);
		InternetCloseHandle(dataparse->m_hConnection);
		CDataPack::Instance()->Pack_Upgrade_ACK(0x01);
		CLogTool::Instance()->WriteLogFile(_T("CreateFile Failure"));
		return 0;
	}

	DWORD  dwSizeFileExist = GetFileSize(hFile, NULL);
	if (0xFFFFFFFF == dwSizeFileExist)
	{
		CloseHandle( hFile);
		InternetCloseHandle(dataparse->m_hConnection);
		CDataPack::Instance()->Pack_Upgrade_ACK(0x01);
		CLogTool::Instance()->WriteLogFile(_T("GetFileSize 0xFFFFFFFF"));
		return 0;
	}

	//文件存在则需要断点续传
	if (0 < dwSizeFileExist)
	{
		TCHAR szTemp[50];  
		memset( szTemp, 0, sizeof(szTemp));
		swprintf(szTemp, _T("REST %ld\r\n"), dwSizeFileExist);
		FtpCommand(dataparse->m_hConnection, FALSE,  FTP_TRANSFER_TYPE_BINARY, szTemp,  0,  0);
	}

	HINTERNET hSession = NULL;
	//获取FTP文件大小
	hSession = FtpOpenFile( dataparse->m_hConnection,  _T("Update.zip"),  GENERIC_READ,  FTP_TRANSFER_TYPE_UNKNOWN,  NULL);


	if (hSession == NULL)
	{
		CDataPack::Instance()->Pack_Upgrade_ACK(0x01);
		CLogTool::Instance()->WriteLogFile(_T("FtpOpenFile Failure"));
		return 1;
	}

	//需下载文件大小（此处只处理小于4G的文件）
	DWORD dwFtpFileHighSize = 0;
	DWORD dwFtpFileLowSize = FtpGetFileSize(hSession, &dwFtpFileHighSize);
	DWORD dwFileTotalSize = dwFtpFileLowSize;

	DWORD dwReadedSize = 0;
	dwReadedSize += dwSizeFileExist;

	DWORD dwOffset_local = SetFilePointer(hFile, dwSizeFileExist, NULL, FILE_BEGIN);

	DWORD dwSizeRead = 0;
	while (dwReadedSize < dwFileTotalSize)
	{
		//每次读取满2000到缓存中再写到文件中去
		char szBuffer[100] = {0};
		BOOL bRet = InternetReadFile(hSession, szBuffer, 100, &dwSizeRead);

		if (FALSE == bRet )
		{
			break;
		}
		Sleep(10);

		dwReadedSize += dwSizeRead;

		// 写数据到本地文件
		DWORD dwSizeWrite = 0;
		BOOL bWrite = WriteFile( hFile, szBuffer, dwSizeRead, &dwSizeWrite, 0);
	}

	if (dwFileTotalSize == dwReadedSize)
	{
		bFlagGetFile = true;
	}
	else
	{
		bFlagGetFile = false;
	}
	CloseHandle( hFile);
	InternetCloseHandle(hSession);
	InternetCloseHandle(dataparse->m_hConnection);

	CMsgProc::Instance()->SendServerMsg(MSG_UPGRADE_WPARAM, bFlagGetFile);
	return bFlagGetFile;
}

void CDataParser::LoadPhotoNum()
{
	CppSQLite3DB db;

	char szSql[NUM_1K];
	memset(szSql, 0,sizeof(szSql));
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	sprintf(szSql, "Select * From photo_Info;");
	CppSQLite3Query q = db.execQuery(szSql);
	m_wPhotoCmd = (WORD)q.getIntField("photo_num");
	m_wPhotoTime = (WORD)q.getIntField("photo_time");

	if (m_wPhotoTime < 180)
	{
		m_wPhotoTime = 600;
	}

	//释放statement
	q.finalize();
	db.close();	//关闭数据库

	if (m_bIsAutoPhoto)
	{
		return;
	}

	m_bIsAutoPhoto = true;

	CLogTool::Instance()->WriteLogFile(_T("LoadPhotoNum start photoupload thread"));

	CreateThread(NULL, 0, THDPhotoUpload, this, 0, NULL);

}

DWORD CDataParser::THDStartGuider(LPVOID lp)
{
	CDataParser *inst = (CDataParser*)lp;

	HWND hwnd = ::FindWindow(NULL, MSG_WND_NAME); //打开导航软件
	if (NULL == hwnd)
	{

		CreateProcess( _T("\\Storage Card\\Guider4u\\guider.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);

		for(int i = 0; i < 1 * 60; i++ )
		{
			Sleep(1000);
			hwnd = ::FindWindow(NULL, MSG_WND_NAME);
			if ( hwnd ) 
				break;
		}
	}
	inst->DeleteUserRoadFile(inst->m_dwRoadID);
	CLogTool::Instance()->WriteLogFile(_T("PostMessage to Guider"));
	::PostMessage(hwnd,MSG_TO_GUIDER4U,inst->m_dwRoadID,0);
	return 0;
}

VOID CDataParser::DeleteUserRoadFile(DWORD dwRoadID)
{
	switch(dwRoadID)
	{
	case 0:
		{
			if (::GetFileAttributes(PATH_ROAD1_TXT) != 0xFFFFFFFF)
			{
				DeleteFile(PATH_ROAD1_TXT);
			}
		}
		break;
	case 1:
		{
			if (::GetFileAttributes(PATH_ROAD2_TXT) != 0xFFFFFFFF)
			{
				DeleteFile(PATH_ROAD2_TXT);
			}
		}
		break;
	case 2:
		{
			if (::GetFileAttributes(PATH_ROAD3_TXT) != 0xFFFFFFFF)
			{
				DeleteFile(PATH_ROAD3_TXT);
			}
		}
		break;
	default:
		break;
	}
}