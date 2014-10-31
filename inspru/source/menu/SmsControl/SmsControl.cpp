/************************************************************************
* Copyright (C)2012, 上海鲲博通讯技术有限公司
* All rights reserved.
* 
* 文件名称: SmsControl.cpp

* 文件标识: 
* 摘    要: 

* 作    者: 
* 完成日期: 2012/01/04
* 备注: 
************************************************************************/

#include "stdafx.h"
#include "SmsControl.h"
#include "../ICCardTools/Tools/LogTool.h"
#include "../SQLite3/CppSQLite3.h"
#include "../SysConfig/SysConfig.h"
#include "../DriveLib/midware/DllMidware.h"
#include "../ExTool/ex_basics.h"
#include "../GSM/GSMLogic.h"
#include "../FileMapping/FileMapping.h"

extern CGSMLogic g_GsmLogic;

//初始化静态成员变量
CSmsControl*	CSmsControl::m_pInstance = NULL;

//参数代码列表
const char*  g_szAPN						= "A";		//APN
const char*  g_szMainCenterIPAddress		= "I1";		//主中心IP
const char*  g_szMainCenterDomainName		= "Y1";		//主中心域名
const char*  g_szPairCenterIPAddress		= "I2";		//副中心IP
const char*  g_szPairCenterDomainName		= "Y2";		//副中心域名
const char*  g_szPortNum					= "P";		//端口
const char*  g_szSoftwareVersion			= "S";		//软件版本
const char*  g_szHardwareVersion			= "H";		//硬件版本
const char*  g_szConnectState				= "L";		//连接状态
const char*  g_szSatelliteCount			= "D";		//卫星颗数
const char*  g_szID						= "ID";		//ID号
const char*  g_szCurrent_Ip				= "DI";		//当前中心IP
const char*  g_szCurrent_Port				= "DP";		//当前端口

/************************************************************************
* 函数名: SmsControl
* 描  述: 构造函数
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
CSmsControl::CSmsControl(void)
{
	//m_bSMSEnable = FALSE;
	memset(m_cSMSCenterNum, NULL, sizeof(m_cSMSCenterNum));
	m_nConnectState	= 0;	//0：RAS断开
}

/************************************************************************
* 函数名: SmsControl
* 描  述: 析构函数
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
CSmsControl::~CSmsControl(void)
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/************************************************************************
* 函数名: Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: singleton
************************************************************************/
CSmsControl* CSmsControl::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CSmsControl;
	}
	return m_pInstance;
}

/************************************************************************
* 函数名: SmsParser
* 描  述: 解析协议消息
* 入  参: 1. pcContent:		要解析的短信字符串
2. pcPhoneNum:	要解析的短信号码字符串
* 出  参: 
* 返  回: BOOL	协议控制短信   TRUE    普通短信  FALSE
* 备  注: 
************************************************************************/
BOOL CSmsControl::SmsParser(PCSTR pcPhoneNum, PCSTR pcContent)
{
	//CLogTool::Instance()->WriteLogFile(">>", (PBYTE)pcContent, strlen(pcContent));
	if (NULL == pcContent || NULL == pcPhoneNum
		|| strlen(pcContent)>140 || strlen(pcPhoneNum)>=16 )
	{
		return FALSE;
	}

	//短信长度
	WORD wSMSLen = strlen(pcContent);

	CLogTool::Instance()->WriteLogFile("SMS_NUM>>%s", pcPhoneNum);
	CLogTool::Instance()->WriteLogFile("SMS_CONTENT>>%s", pcContent);

	//检查'*','#',确定是否为协议短信,长度不能小于3
	if(pcContent[0] != '*' || pcContent[wSMSLen-1] != '#' || wSMSLen < 3)
	{
		return FALSE;
	}

	//短信控制功能关闭的情况下，如果不是中心短信号码发送的控制短信，则不处理
	if( !CSmsControl::Instance()->IsSMSEnable() )
	{
		if( strcmp(pcPhoneNum, m_cSMSCenterNum) != 0 )
			return TRUE;
	}

	//获取短信和号码
	m_wContentLen = wSMSLen-2;
	memset(m_cPhoneNum, NULL, sizeof(m_cPhoneNum));
	memset(m_cContent, NULL, sizeof(m_cContent));
	//需要将号码开头的"86"去掉，否则无法回复短信
	//if(strncmp(pcPhoneNum, "86", 2) == 0)
	//strcpy(m_cPhoneNum, &pcPhoneNum[2]);
	//else
	strcpy(m_cPhoneNum, pcPhoneNum);
	strncpy(m_cContent, &pcContent[1], m_wContentLen);	//短信体去掉'*','#'

	//分发消息体
	DataDispatch((PCSTR)m_cContent, m_wContentLen);
	return TRUE;
}

/************************************************************************
* 函数名: InitSmsControl
* 描  述: 初始化短信控制
* 入  参: 
* 出  参: 
* 返  回: BOOL
* 备  注: 1.如果有短信重启指令，重启后须发送重启成功指令"*TERMINAL RESET FINISH#"
************************************************************************/
BOOL CSmsControl::InitSmsControl()
{
	//1.获取短信中心号码
	CppSQLite3DB db;
	db.open(PATH_DB_CONFIG);	//打开数据库
	//在数据库中查询第nRecordIndex条数据
	char szSQL[512];
	sprintf(szSQL, "SELECT * FROM config;");
	CppSQLite3Query q = db.execQuery(szSQL);
	if ( !q.eof() )
	{
		//获取短信中心号码
		memset(m_cSMSCenterNum, NULL, sizeof(m_cSMSCenterNum));
		strncpy(m_cSMSCenterNum, q.fieldValue("MsgCenterNum"), sizeof(m_cSMSCenterNum));
	}
	//释放statement
	q.finalize();
	db.close();	//关闭数据库

	//2.启动发送短信队列线程
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThrdSendSMS, this, 0, NULL);

	//3.设备重启状态
	if( GetFileAttributes(PATH_SMS_RESET_SYS) != 0xFFFFFFFF )
	{
		DeleteFile(PATH_SMS_RESET_SYS);
		//回复信息
		ReplySMS("*TERMINAL RESET FINISH#");
	}

	return TRUE;
}

/************************************************************************
* 函数名: IsSMSEnable
* 描  述: 判断当前短信控制功能是否可用
* 入  参: 
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::IsSMSEnable()
{
	return CSysConfig::Instance().GetSMS();
}

/************************************************************************
* 函数名: SetConnectState
* 描  述: 保存当前网络连接状态
* 入  参: nConnectState: 连接状态：0：RAS断开	1：RAS连接	2：登录服务器 
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::SetConnectState(INT nConnectState)
{
	if(nConnectState<0 || nConnectState>2)
		return FALSE;

	m_nConnectState = nConnectState;
	return TRUE;
}

/************************************************************************
* 函数名: OnQueryParam
* 描  述: 中心下发参数查询指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::OnQueryParam(PCSTR pcParam)
{
	if(NULL == pcParam)
		return FALSE;

	stParam Param[13];
	memset(Param, NULL, sizeof(Param));
	GetParam(Param, 13);	//获取终端参数

	//拼接查询参数列表
	CHAR	szParamList[512];
	memset(szParamList, NULL, sizeof(szParamList));
	//参数code长度
	WORD	wParamCodeLen = 0;
	//获取第一节参数
	PCSTR	pcParamCode = pcParam;	//搜索开始地址
	INT		nFirstTime	= 0;
	while( (pcParamCode = ScanField(pcParamCode, nFirstTime, wParamCodeLen)) != NULL )
	{
		nFirstTime	= 1;	//第一次从当前节开始搜索，之后从下一节循环搜索
		if( !AddParam2List(Param, 13, pcParamCode, wParamCodeLen, szParamList) )
			return FALSE;
	}

	CLogTool::Instance()->WriteLogFile("Parse query parameter command success!");

	//发送参数查询结果短信
	SendParamList(szParamList);

	return TRUE;
}

/************************************************************************
* 函数名: OnSetParam
* 描  述: 中心下发参数设置指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: enReturnType:	0:成功		1:指令错误(回复*COMMAND ERR#)		2:参数错误(回复*CONFIG ERR#):参数不符合规定
* 备  注: *SZ,X:TZ-SN,A:CMNET,Y1:255.255.255.255,P:2000#
************************************************************************/
CSmsControl::enReturnType CSmsControl::OnSetParam(PCSTR pcParam)
{
	if(NULL == pcParam)
		return RESULT_CONFIG_ERR;

	stParam Param[6];	//最多参数6条：APN,主中心IP,主中心域名,副中心IP,副中心域名
	memset(Param, NULL, sizeof(Param));

	WORD wParamCount = 0;

	//解析参数列表pcParam，存放参数到数组Param中
	ParseParam(Param, 6, pcParam, wParamCount);

	//检验参数合法性
	enReturnType enResult = VerifyParam(Param, wParamCount);
	if(enResult != RESULT_SUCCESS)
		return enResult;

	//设置参数
	SetParam(Param, wParamCount);

	CLogTool::Instance()->WriteLogFile("Set parameter success!");

	return RESULT_SUCCESS;
}

/************************************************************************
* 函数名: OnResetDevice
* 描  述: 中心下发终端重启指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::OnResetDevice(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	//回复信息
	ReplySMS("*TERMINAL RESET START#");

	//写文件记录重启
	HANDLE hFile = ::CreateFile( PATH_SMS_RESET_SYS, GENERIC_READ, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if (  hFile != INVALID_HANDLE_VALUE  )
	{
		::CloseHandle( hFile );
	}

	CLogTool::Instance()->WriteLogFile("Reset system!");
	//重启设备
	CreateThread(NULL, 0, ThrdResetSystem, NULL, 0, NULL);

	return TRUE;
}

/************************************************************************
* 函数名: OnRestoreDefault
* 描  述: 中心下发恢复出厂值指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::OnRestoreDefault(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	TCHAR  szFileDefault[MAX_PATH], szFileConfig[MAX_PATH];
	//短信内容
	memset(szFileDefault, 0, sizeof(szFileDefault) );
	memset(szFileConfig, 0, sizeof(szFileConfig) );
	ex_char2uni( PATH_DB_DEFAULT_CONFIG, szFileDefault, MAX_PATH );
	ex_char2uni( PATH_DB_CONFIG, szFileConfig, MAX_PATH );

	//默认配置文件不存在
	if( GetFileAttributes(szFileDefault) == 0xFFFFFFFF )
	{
		//回复信息
		ReplySMS("*FACTORY RESET FAILED#");
		CLogTool::Instance()->WriteLogFile("Factory set file default.db doesn't exist!");
		return TRUE;
	}

	//复制默认配置文件
	CopyFile(szFileDefault, szFileConfig, FALSE);
	//回复信息
	ReplySMS("*FACTORY RESET SUCCEEDED#");
	CLogTool::Instance()->WriteLogFile("Factory reset success!");

	return TRUE;
}

/************************************************************************
* 函数名: OnCloseSMS
* 描  述: 中心下发关闭短信功能使用权限指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::OnDisableSMS(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	CSysConfig::Instance().SetSMS(FALSE);
	//回复信息
	ReplySMS("*CLOSE SMS SUCCEEDED#");
	CLogTool::Instance()->WriteLogFile("Disable SMS control!");

	return TRUE;
}

/************************************************************************
* 函数名: OnOpenSMS
* 描  述: 中心下发打开短信功能使用权限指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::OnEnableSMS(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	CSysConfig::Instance().SetSMS(TRUE);
	//回复信息
	ReplySMS("*OPEN SMS SUCCEEDED#");
	CLogTool::Instance()->WriteLogFile("Enable SMS control!");

	return TRUE;
}

/************************************************************************
* 函数名: OnDelFile
* 描  述: 删除指定文件指令
* 入  参: pcBody: 参数体
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::OnDelFile(PCSTR pcParam)
{
	if(NULL == pcParam)
		return FALSE;

	CHAR	szPath[MAX_PATH];
	memset(szPath, NULL, sizeof(szPath));

	//转换路径中的特殊字符
	DecodePath(szPath, pcParam);

	//短信内容
	TCHAR	szFilePath[MAX_PATH];
	memset( szFilePath, 0, sizeof(szFilePath) );
	ex_char2uni( szPath, szFilePath, MAX_PATH );

	//要删除的文件存在
	if( GetFileAttributes(szFilePath) != 0xFFFFFFFF )
	{
		DeleteFile(szFilePath);
	}

	return TRUE;
}

/************************************************************************
* 函数名: ScanField
* 描  述: 获取字符串中第一个','后面字符串的地址
* 入  参: 1. pcBuffer:	要解析的短信协议字符串，去掉'*','#'
2. wFieldNO:	解析的字符串的节(两个','之间的字符串)，0：当前节，1：下一节，2,3...
* 出  参: 1. wLen:		节的长度
* 返  回: CHAR*:		指向节的字符串，如果为NULL，则获取节字符串失败
* 备  注: 
************************************************************************/
PCSTR CSmsControl::ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen)
{
	PCSTR pcFieldBegin = pcBuffer;

	while( nFieldNO-- > 0 )
	{
		while ( pcFieldBegin && *pcFieldBegin++ != ',' && *pcFieldBegin != NULL )
			;
	}
	//搜索的节为空
	if(NULL == *pcFieldBegin)
	{
		wLen = 0;
		return NULL;
	}

	//节的最后一个字符指针的后一位
	PCSTR pcFieldEnd = pcFieldBegin;
	while( pcFieldEnd && *pcFieldEnd != ',' && *pcFieldEnd != NULL )
	{
		pcFieldEnd++;
	}

	//计算节的长度
	wLen = pcFieldEnd - pcFieldBegin;

	return pcFieldBegin;
}

/************************************************************************
* 函数名: DataDispatch
* 描  述: 解析返回结果
* 入  参: 1. pcBuffer:		要解析的协议消息字符串
2. wBufferLen:	要解析的协议消息字符串的长度
* 出  参: 
* 返  回: BOOL
* 备  注:
************************************************************************/
BOOL CSmsControl::DataDispatch(PCSTR pcBuffer, WORD wBufferLen)
{
	if(NULL == pcBuffer)
		return FALSE;

	BOOL	bResult		= FALSE;
	WORD	wFieldLen	= 0;
	//校验密码
	PCSTR	pcPWD = ScanField(pcBuffer, 1, wFieldLen);
	if( !CheckPWD( pcPWD, wFieldLen ) )
	{
		goto END_PASSWORD_ERR;
	}

	//获取命令类型
	PCSTR	pcCMD_ID	= ScanField(pcBuffer, 0, wFieldLen);
	//获取参数列表
	WORD	wParamLen	= 0;
	PCSTR	pcParam		= ScanField(pcBuffer, 2, wParamLen);

	//1.中心下发参数查询指令:	*CX,X:TZ-SN,A,I1,P,D#
	if( strncmp(pcCMD_ID, "CX", wFieldLen) == 0 )
	{
		if( OnQueryParam( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//2.中心下发参数设置指令:	*SZ,X:TZ-SN,A:CMNET,Y1:255.255.255.255,P:2000#
	else if( strncmp(pcCMD_ID, "SZ", wFieldLen) == 0 )
	{
		switch( OnSetParam( pcParam ) )
		{
		case RESULT_SUCCESS:		//成功
			goto END_SUCCESS;
		case RESULT_COMMAND_ERR:	//指令错误(回复*COMMAND ERR#)
			goto END_COMMAND_ERR;
		case RESULT_CONFIG_ERR:		//参数错误(回复*CONFIG ERR#)
			goto END_CONFIG_ERR;
		}
	}
	//3.中心下发终端重启指令:	*RESET,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "RESET", wFieldLen) == 0 )
	{
		if( OnResetDevice( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//10.中心下发恢复出厂值指令:	*HF,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "HF", wFieldLen) == 0 )
	{
		if( OnRestoreDefault( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//13.中心下发关闭短信功能使用权限指令:	*CLOSE SMS,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "CLOSE SMS", wFieldLen) == 0 )
	{
		if( OnDisableSMS( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//14.中心下发打开短信功能使用权限指令:	*OPEN SMS,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "OPEN SMS", wFieldLen) == 0 )
	{
		if( OnEnableSMS( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//17.删除指定文件指令:	*DEL,X:TZ-SN,PATH#
	else if( strncmp(pcCMD_ID, "DEL", wFieldLen) == 0 )
	{
		if( OnDelFile( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	else
	{
		goto END_COMMAND_ERR;
	}

END_PASSWORD_ERR:	//校验密码错误
	ReplySMS("*PASSWORD ERR#");
	return FALSE;

END_COMMAND_ERR:	//指令错误
	CLogTool::Instance()->WriteLogFile("COMMAND ERR");
	ReplySMS("*COMMAND ERR#");
	return FALSE;

END_CONFIG_ERR:		//设置的参数不符合规定
	CLogTool::Instance()->WriteLogFile("CONFIG ERR");
	ReplySMS("*CONFIG ERR#");
	return FALSE;

END_SUCCESS:
	return TRUE;
}

/************************************************************************
* 函数名: CheckPWD
* 描  述: 校验密码"X:TZ-SN"
* 入  参: 1. pcBuffer:		密码字符串
2. wBufferLen:	密码字符串长度
* 出  参: 
* 返  回: BOOL
* 备  注:
************************************************************************/
BOOL CSmsControl::CheckPWD(PCSTR pcBuffer, WORD wBufferLen)
{
	if(NULL == pcBuffer)
		return FALSE;

	PCSTR pcPWD = "X:TZ-SN";
	if( strncmp(pcBuffer, pcPWD, wBufferLen) != 0 )
		return FALSE;

	return TRUE;
}

/************************************************************************
* 函数名: ReplySMS
* 描  述: 回复短信
* 入  参: 
1. pcSMS:	短信字符串
2. wSMSLen:	短信字符串长度
* 出  参: 
* 返  回: BOOL
* 备  注:
************************************************************************/
BOOL CSmsControl::ReplySMS(PCSTR pcSMS/*, WORD wSMSLen*/)
{
	stSMS sms_buffer;
	memset(&sms_buffer, NULL, sizeof(stSMS));
	strcpy(sms_buffer.szPhoneNum, m_cPhoneNum);
	strncpy(sms_buffer.szContent, pcSMS, 140);

	//将短信推入发送队列
	m_queueSendSMS.push(sms_buffer);

	CLogTool::Instance()->WriteLogFile(pcSMS);
	return TRUE;
}

/************************************************************************
* 函数名: GetParam
* 描  述: 获取终端参数
* 入  参: 
1. pcSMS:	短信字符串
2. wSMSLen:	短信字符串长度
* 出  参: 
* 返  回: BOOL
* 备  注:
************************************************************************/
BOOL CSmsControl::GetParam(stParam Param[], INT nParamCount)
{
	CppSQLite3DB db;
	db.open(PATH_DB_CONFIG);	//打开数据库
	//在数据库中查询
	char szSQL[512];
	sprintf(szSQL, "SELECT * FROM config;");
	CppSQLite3Query q = db.execQuery(szSQL);
	if ( !q.eof() )
	{
		//A		APN
		strcpy(Param[0].szParamCode, g_szAPN);
		strcpy(Param[0].szParamValue, q.fieldValue("APN"));
		//I1	主中心IP
		strcpy(Param[1].szParamCode, g_szMainCenterIPAddress);
		strcpy(Param[1].szParamValue, q.fieldValue("MainCenterIPAddress"));
		//Y1	主中心域名
		strcpy(Param[2].szParamCode, g_szMainCenterDomainName);
		strcpy(Param[2].szParamValue, q.fieldValue("MainCenterDomainName"));
		//I2	副中心IP
		strcpy(Param[3].szParamCode, g_szPairCenterIPAddress);
		strcpy(Param[3].szParamValue, q.fieldValue("PairCenterIPAddress"));
		//Y2	副中心域名
		strcpy(Param[4].szParamCode, g_szPairCenterDomainName);
		strcpy(Param[4].szParamValue, q.fieldValue("PairCenterDomainName"));
		//P		端口
		strcpy(Param[5].szParamCode, g_szPortNum);
		strcpy(Param[5].szParamValue, q.fieldValue("PortNum"));
		//S		软件版本
		strcpy(Param[6].szParamCode, g_szSoftwareVersion);
		strcpy(Param[6].szParamValue, q.fieldValue("SoftwareVersion"));
		//H		硬件版本
		strcpy(Param[7].szParamCode, g_szHardwareVersion);
		strcpy(Param[7].szParamValue, q.fieldValue("Pro_Version"));
		//ID	ID号
		strcpy(Param[10].szParamCode, g_szID);
		strcpy(Param[10].szParamValue, q.fieldValue("FactoryId"));
		//DI	当前中心IP
		strcpy(Param[11].szParamCode, g_szCurrent_Ip);
		strcpy(Param[11].szParamValue, q.fieldValue("Current_Ip"));
		//DP	当前端口
		strcpy(Param[12].szParamCode, g_szCurrent_Port);
		strcpy(Param[12].szParamValue, q.fieldValue("Current_Port"));
	}
	//释放statement
	q.finalize();
	db.close();	//关闭数据库

	//L		连接状态  0：RAS断开	1：RAS连接	2：登录服务器 
	strcpy(Param[8].szParamCode, g_szConnectState);
	if(0 == m_nConnectState)
		sprintf(Param[8].szParamValue, "OFF");
	else if(1 == m_nConnectState)
		sprintf(Param[8].szParamValue, "ON");
	else if(2 == m_nConnectState)
		sprintf(Param[8].szParamValue, "OK");

	//D		卫星颗数
	strcpy(Param[9].szParamCode, g_szSatelliteCount);
	sprintf(Param[9].szParamValue, "%02d", g_pstShareMem->st_GPS.m_starnum);

	return TRUE;
}

/************************************************************************
* 函数名:	ParseParam
* 描  述:	解析终端参数
* 入  参:	1. Param:		存放解析后的终端参数的数组
			2. nArrayCount:	数组大小
			3. pcParamList:	参数列表
* 出  参:	1. nParamCount:	参数列表中参数个数
* 返  回:	BOOL
* 备  注:	A:CMNET,Y1:255.255.255.255,P:2000
************************************************************************/
BOOL CSmsControl::ParseParam(stParam Param[], const INT nArrayCount, PCSTR pcParamList, WORD& wParamCount)
{
	if(NULL == pcParamList)
		return FALSE;

	PCSTR	pcFieldStart	= pcParamList;
	WORD	wFieldLen		= 0;
	INT		nFirstField		= 0;	//开始从第一节搜索，之后从下一节开始搜索
	for(wParamCount=0; wParamCount<nArrayCount; wParamCount++)
	{
		pcFieldStart	= ScanField(pcFieldStart, nFirstField, wFieldLen);
		nFirstField		= 1;
		//搜索完毕
		if(NULL == pcFieldStart)
			return TRUE;

		if(wFieldLen < 3)
			return FALSE;

		for(INT i=1; i<wFieldLen-1; i++)
		{
			//':'分隔参数代码和参数值
			if(':' == pcFieldStart[i])
			{
				//参数代码
				_snprintf(Param[wParamCount].szParamCode, i, "%s", pcFieldStart);
				//参数值
				_snprintf(Param[wParamCount].szParamValue, wFieldLen-i-1, "%s", &pcFieldStart[i+1]);
				break;
			}
		}
	}

	return TRUE;
}

/************************************************************************
* 函数名: VerifyParam
* 描  述: 检查参数是否符合规定
* 入  参: 
1. Param:		参数数组
2. nArrayCount:	参数数组元素个数
* 出  参: 
* 返  回: enReturnType:	0:成功		1:参数代码错误		2:参数值不符合规定
* 备  注:
A	APN			例如CMNET
I1	主中心IP	例如255.255.255.255
Y1	主中心域名	例如v.cheguan.com
I2	副中心IP	例如255.255.255.255
Y2	副中心域名	例如v.cheguan.com
P	端口		例如2000
************************************************************************/
CSmsControl::enReturnType CSmsControl::VerifyParam(stParam Param[], const INT nArrayCount)
{
	if(nArrayCount > 6)
		return RESULT_COMMAND_ERR;
	for(INT i=0; i<nArrayCount; i++)
	{
		//APN
		if( strcmp(Param[i].szParamCode, g_szAPN) == 0 )	
		{
			if(strcmp(Param[i].szParamValue, "CMNET") != 0)
				return RESULT_CONFIG_ERR;
		}
		//主中心IP
		else if( strcmp(Param[i].szParamCode, g_szMainCenterIPAddress) == 0 )
		{
			if( !VerifyIP(Param[i].szParamValue) )
				return RESULT_CONFIG_ERR;
		}
		//主中心域名
		else if( strcmp(Param[i].szParamCode, g_szMainCenterDomainName) == 0 )
		{
		}
		//副中心IP
		else if( strcmp(Param[i].szParamCode, g_szPairCenterIPAddress) == 0 )
		{
			if( !VerifyIP(Param[i].szParamValue) )
				return RESULT_CONFIG_ERR;
		}
		//副中心域名
		else if( strcmp(Param[i].szParamCode, g_szPairCenterDomainName) == 0 )
		{
		}
		//端口
		else if( strcmp(Param[i].szParamCode, g_szPortNum) == 0 )
		{
			if(  atoi(Param[i].szParamValue) > 0xFFFF )
				return RESULT_CONFIG_ERR;
		}
		else
		{//参数代码错误
			return RESULT_COMMAND_ERR;
		}
	}

	//成功
	return RESULT_SUCCESS;
}

/************************************************************************
* 函数名: SetParam
* 描  述: 设置终端参数
* 入  参: 
1. Param:		参数数组
2. nArrayCount:	参数个数(小于6)
* 出  参: 
* 返  回: BOOL
* 备  注:
************************************************************************/
BOOL CSmsControl::SetParam(stParam Param[], const INT nArrayCount)
{
	if(nArrayCount > 6)
		return FALSE;

	stParam CurrentParam[6];
	memset(CurrentParam, NULL, sizeof(CurrentParam));

	CppSQLite3DB db;
	db.open(PATH_DB_CONFIG);	//打开数据库
	//在数据库中查询
	char szSQL[512];
	sprintf(szSQL, "SELECT * FROM config;");
	CppSQLite3Query q = db.execQuery(szSQL);
	if ( !q.eof() )
	{
		//A		APN
		strcpy(CurrentParam[0].szParamCode, g_szAPN);
		strcpy(CurrentParam[0].szParamValue, q.fieldValue("APN"));
		//I1	主中心IP
		strcpy(CurrentParam[1].szParamCode, g_szMainCenterIPAddress);
		strcpy(CurrentParam[1].szParamValue, q.fieldValue("MainCenterIPAddress"));
		//Y1	主中心域名
		strcpy(CurrentParam[2].szParamCode, g_szMainCenterDomainName);
		strcpy(CurrentParam[2].szParamValue, q.fieldValue("MainCenterDomainName"));
		//I2	副中心IP
		strcpy(CurrentParam[3].szParamCode, g_szPairCenterIPAddress);
		strcpy(CurrentParam[3].szParamValue, q.fieldValue("PairCenterIPAddress"));
		//Y2	副中心域名
		strcpy(CurrentParam[4].szParamCode, g_szPairCenterDomainName);
		strcpy(CurrentParam[4].szParamValue, q.fieldValue("PairCenterDomainName"));
		//P		端口
		strcpy(CurrentParam[5].szParamCode, g_szPortNum);
		strcpy(CurrentParam[5].szParamValue, q.fieldValue("PortNum"));
	}
	//释放statement
	q.finalize();

	//保存参数
	for(INT i=0; i<nArrayCount; i++)
	{
		for(INT j=0; j<6; j++)
		{
			if( strcmp(CurrentParam[j].szParamCode, Param[i].szParamCode) == 0 )
			{
				memset(CurrentParam[j].szParamValue, NULL, sizeof(CurrentParam[j].szParamValue));
				strcpy(CurrentParam[j].szParamValue, Param[i].szParamValue);
			}
		}
	}

	//保存到数据库
	sprintf(szSQL, "SELECT * FROM config;");
	sprintf(szSQL, "UPDATE config SET APN = '%s', MainCenterIPAddress = '%s', MainCenterDomainName = '%s',	\
		PairCenterIPAddress = '%s', PairCenterDomainName = '%s', PortNum = '%s';", 
		CurrentParam[0].szParamValue, CurrentParam[1].szParamValue, CurrentParam[2].szParamValue, 
		CurrentParam[3].szParamValue, CurrentParam[4].szParamValue, CurrentParam[5].szParamValue);
	db.execDML(szSQL);
	db.close();		//关闭数据库

	return TRUE;
}

/************************************************************************
* 函数名: AddParam2List
* 描  述: 拼接查询参数列表
* 入  参: 
1. Param:		当前参数结构体数组
2. nParamCount:	数组元素个数
3. pcParamCode:	需拼接的参数代码
4. wParamCodeLen:	参数代码长度
5. szParamList:	参数拼接的buffer
* 出  参: 
* 返  回: BOOL
* 备  注:
************************************************************************/
BOOL CSmsControl::AddParam2List(stParam Param[], INT nParamCount, PCSTR pcParamCode, WORD wParamCodeLen, PSTR szParamList)
{
	//遍历当前参数数组
	for(INT i=0; i<nParamCount; i++)
	{
		//匹配参数代码
		if( strncmp(Param[i].szParamCode, pcParamCode, wParamCodeLen) == 0 )
		{
			//保存参数值到参数列表中
			WORD wLen = strlen(szParamList);
			if( wLen > 0 )
			{//参数之间用','间隔
				szParamList[wLen++] = ',';
			}
			sprintf(&szParamList[wLen], "%s:%s", Param[i].szParamCode, Param[i].szParamValue);
			return TRUE;
		}
	}
	return FALSE;
}

/************************************************************************
* 函数名: SendParamList
* 描  述: 发送查询的参数列表
* 入  参: 
1. szParamList:	短信字符串
* 出  参: 
* 返  回: BOOL
* 备  注: 如果发送的参数列表短信超过140字节，需要拆分多条发送，
如：*M:TZ-SN,A:CMNET,I1:255.255.255,P:2000,D:06#
除去其他字符，一条短信的参数列表中的字符最多130字节
************************************************************************/
BOOL CSmsControl::SendParamList(PCSTR pszParamList)
{
	if(NULL == pszParamList)
		return FALSE;

	CHAR szSMS[512];
	memset(szSMS, NULL, sizeof(szSMS));
	WORD wLen = strlen(pszParamList);
	if(0 == wLen || wLen > 512)
		return FALSE;

	//参数列表字节数小于130
	if(wLen <= 130)
	{
		sprintf(szSMS, "*M:TZ-SN,%s#", pszParamList);
		ReplySMS(szSMS);
		return TRUE;
	}

	//参数列表字节数小于130则需要拆分
	WORD	wFieldLen		= 0;	//当前节的长度
	WORD	wSMSCount		= 1;	//拆分短信的条数，默认为第一条
	PCSTR	pcSubSMSStart	= pszParamList;		//第i条拆分短信的首地址
	//PCSTR	pcFirstField	= pcSubSMSStart;	//
	//PCSTR	pcNextField		= NULL;	//
	PCSTR	pcFieldStart	= pcSubSMSStart;	//
	while( (pcFieldStart == ScanField(pcFieldStart, 1, wFieldLen)) != NULL )
	{//A:CMNET,I1:255.255.255,P:2000,D:06 
		WORD nSMSLen = pcFieldStart - pcSubSMSStart;
		//跨越两条短信的临界
		if( nSMSLen-1<=130 && nSMSLen+wFieldLen>130 )
		{
			memset(szSMS, NULL, sizeof(szSMS));
			_snprintf(szSMS, nSMSLen, "*M:TZ-SN,%s#", pcSubSMSStart);
			ReplySMS(szSMS);

			pcSubSMSStart = pcFieldStart;
		}
		//pcFirstField = pcNextField;
	}
	//最后一条短信
	memset(szSMS, NULL, sizeof(szSMS));
	sprintf(szSMS, "*M:TZ-SN,%s#", pcSubSMSStart);
	ReplySMS(szSMS);

	return TRUE;
}

/************************************************************************
* 函数名: DecodePath
* 描  述: 路径中的特殊字符处理
* 入  参:	1. szPath:			转换后的字符串
			2. szOriginPath:	原始字符串
* 出  参: 
* 返  回: BOOL
* 备  注: 短信接收后，一部分特殊字符编码会改变，需要解码回原来字符，解码规则如下：
测试字符：\!@#$%^&()_+-=[]{};',.~`
其中转码的为：
0x1B2F->'\'
0x1B14->'^'
0x1B3C->'['
0x1B3E->']'
0x1B28->'{'
0x1B29->'}'
0x1B3D->'~'
0x02->'$'
0x11->'_'
0x3F->'`'
'@'会转码为0x00,则不处理
************************************************************************/
BOOL CSmsControl::DecodePath(PSTR szPath, PCSTR szOriginPath)
{
	if(NULL==szPath || NULL==szOriginPath)
		return FALSE;

	//短信长度
	INT nPathLen	= strlen(szOriginPath);
	INT	i = 0, j = 0;

	strcpy(szPath, szOriginPath);

	for(i=0; i<nPathLen-1; i++)
	{
		//0x1B2F->'\'
		if(0x1B==szPath[i] && 0x2F==szPath[i+1])
		{
			szPath[i]	= '\\';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x1B14->'^'
		if(0x1B==szPath[i] && 0x14==szPath[i+1])
		{
			szPath[i]	= '^';
				for(j=i+1; j<nPathLen; j++)
					szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x1B3C->'['
		if(0x1B==szPath[i] && 0x3C==szPath[i+1])
		{
			szPath[i]	= '[';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x1B3E->']'
		if(0x1B==szPath[i] && 0x3E==szPath[i+1])
		{
			szPath[i]	= ']';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x1B28->'{'
		if(0x1B==szPath[i] && 0x28==szPath[i+1])
		{
			szPath[i]	= '{';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x1B29->'}'
		if(0x1B==szPath[i] && 0x29==szPath[i+1])
		{
			szPath[i]	= '}';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x1B3D->'~'
		if(0x1B==szPath[i] && 0x3D==szPath[i+1])
		{
			szPath[i]	= '~';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//长度减少1
			nPathLen--;
		}
		//0x02->'$'
		if(0x02==szPath[i])
		{
			szPath[i]	= '$';
		}
		//0x11->'_'
		if(0x11==szPath[i])
		{
			szPath[i]	= '_';
		}
		//0x3F->'`'
		if(0x3F==szPath[i])
		{
			szPath[i]	= '`';
		}
	}

	return TRUE;
}

/************************************************************************
* 函数名: VerifyIP
* 描  述: 检查IP合法性
* 入  参: 1. pszIP:	IP字符串
* 出  参: 
* 返  回: BOOL
* 备  注: 
************************************************************************/
BOOL CSmsControl::VerifyIP(PCSTR pszIP)
{
	//IP检查
	PCSTR pIP = pszIP;
	int nIP = atoi(pIP);
	if( nIP > 0xFF)
		return FALSE;

	for(int i=0; i<3; i++)
	{
		pIP = strchr(pIP, '.');
		pIP++;
		nIP = atoi(pIP);
		if( NULL==pIP || nIP>0xFF)
			return FALSE;
	}

	if(strchr(pIP, '.') != NULL)
		return FALSE;

	return TRUE;
}

/************************************************************************
* 函数名: ThrdResetSystem
* 描  述: 重启设备线程
* 入  参: 
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
DWORD CSmsControl::ThrdResetSystem(LPVOID lpParameter)
{
	Sleep(5*1000);
	CDllMidware::Instance().SystemReset();
	return 1;
}

/************************************************************************
* 函数名: ThrdSendSMS
* 描  述: 发送短信线程
* 入  参: 
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
DWORD CSmsControl::ThrdSendSMS(LPVOID lpParameter)
{
	CSmsControl* pDlg = (CSmsControl*)lpParameter;
	while(TRUE)
	{
		//在查询到网络isp的时候才能发送短信
		while( !pDlg->m_queueSendSMS.empty() && (g_GsmLogic.GetGSMNetType()!=0) )
		{
			//从队列中取出第一个record
			stSMS &stBuffer = pDlg->m_queueSendSMS.front();

			CString strSMSContent(stBuffer.szContent), strPhoneNum(stBuffer.szPhoneNum);
			//CString strSMSContent("*FACTORY RESET SUCCEEDED#"), strPhoneNum("13482636300");
			g_GsmLogic.SetPhoneNum( strPhoneNum );
			g_GsmLogic.SetMsgBuf( strSMSContent );
			g_GsmLogic.GSMSendSMS();

			//发送成功则弹出
			//if(bSendResult)
			{
				//CLogTool::Instance()->WriteLogFile(_T("<<"), stBuffer.pBuffer, stBuffer.dwLength);
				pDlg->m_queueSendSMS.pop();
			}
			Sleep(5*1000);
		}
		Sleep(5*1000);
	}

	return 1;
}
