/************************************************************************
* Copyright (C)2012, �Ϻ��ﲩͨѶ�������޹�˾
* All rights reserved.
* 
* �ļ�����: SmsControl.cpp

* �ļ���ʶ: 
* ժ    Ҫ: 

* ��    ��: 
* �������: 2012/01/04
* ��ע: 
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

//��ʼ����̬��Ա����
CSmsControl*	CSmsControl::m_pInstance = NULL;

//���������б�
const char*  g_szAPN						= "A";		//APN
const char*  g_szMainCenterIPAddress		= "I1";		//������IP
const char*  g_szMainCenterDomainName		= "Y1";		//����������
const char*  g_szPairCenterIPAddress		= "I2";		//������IP
const char*  g_szPairCenterDomainName		= "Y2";		//����������
const char*  g_szPortNum					= "P";		//�˿�
const char*  g_szSoftwareVersion			= "S";		//����汾
const char*  g_szHardwareVersion			= "H";		//Ӳ���汾
const char*  g_szConnectState				= "L";		//����״̬
const char*  g_szSatelliteCount			= "D";		//���ǿ���
const char*  g_szID						= "ID";		//ID��
const char*  g_szCurrent_Ip				= "DI";		//��ǰ����IP
const char*  g_szCurrent_Port				= "DP";		//��ǰ�˿�

/************************************************************************
* ������: SmsControl
* ��  ��: ���캯��
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
CSmsControl::CSmsControl(void)
{
	//m_bSMSEnable = FALSE;
	memset(m_cSMSCenterNum, NULL, sizeof(m_cSMSCenterNum));
	m_nConnectState	= 0;	//0��RAS�Ͽ�
}

/************************************************************************
* ������: SmsControl
* ��  ��: ��������
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������: Instance
* ��  ��: 
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: singleton
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
* ������: SmsParser
* ��  ��: ����Э����Ϣ
* ��  ��: 1. pcContent:		Ҫ�����Ķ����ַ���
2. pcPhoneNum:	Ҫ�����Ķ��ź����ַ���
* ��  ��: 
* ��  ��: BOOL	Э����ƶ���   TRUE    ��ͨ����  FALSE
* ��  ע: 
************************************************************************/
BOOL CSmsControl::SmsParser(PCSTR pcPhoneNum, PCSTR pcContent)
{
	//CLogTool::Instance()->WriteLogFile(">>", (PBYTE)pcContent, strlen(pcContent));
	if (NULL == pcContent || NULL == pcPhoneNum
		|| strlen(pcContent)>140 || strlen(pcPhoneNum)>=16 )
	{
		return FALSE;
	}

	//���ų���
	WORD wSMSLen = strlen(pcContent);

	CLogTool::Instance()->WriteLogFile("SMS_NUM>>%s", pcPhoneNum);
	CLogTool::Instance()->WriteLogFile("SMS_CONTENT>>%s", pcContent);

	//���'*','#',ȷ���Ƿ�ΪЭ�����,���Ȳ���С��3
	if(pcContent[0] != '*' || pcContent[wSMSLen-1] != '#' || wSMSLen < 3)
	{
		return FALSE;
	}

	//���ſ��ƹ��ܹرյ�����£�����������Ķ��ź��뷢�͵Ŀ��ƶ��ţ��򲻴���
	if( !CSmsControl::Instance()->IsSMSEnable() )
	{
		if( strcmp(pcPhoneNum, m_cSMSCenterNum) != 0 )
			return TRUE;
	}

	//��ȡ���źͺ���
	m_wContentLen = wSMSLen-2;
	memset(m_cPhoneNum, NULL, sizeof(m_cPhoneNum));
	memset(m_cContent, NULL, sizeof(m_cContent));
	//��Ҫ�����뿪ͷ��"86"ȥ���������޷��ظ�����
	//if(strncmp(pcPhoneNum, "86", 2) == 0)
	//strcpy(m_cPhoneNum, &pcPhoneNum[2]);
	//else
	strcpy(m_cPhoneNum, pcPhoneNum);
	strncpy(m_cContent, &pcContent[1], m_wContentLen);	//������ȥ��'*','#'

	//�ַ���Ϣ��
	DataDispatch((PCSTR)m_cContent, m_wContentLen);
	return TRUE;
}

/************************************************************************
* ������: InitSmsControl
* ��  ��: ��ʼ�����ſ���
* ��  ��: 
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 1.����ж�������ָ��������뷢�������ɹ�ָ��"*TERMINAL RESET FINISH#"
************************************************************************/
BOOL CSmsControl::InitSmsControl()
{
	//1.��ȡ�������ĺ���
	CppSQLite3DB db;
	db.open(PATH_DB_CONFIG);	//�����ݿ�
	//�����ݿ��в�ѯ��nRecordIndex������
	char szSQL[512];
	sprintf(szSQL, "SELECT * FROM config;");
	CppSQLite3Query q = db.execQuery(szSQL);
	if ( !q.eof() )
	{
		//��ȡ�������ĺ���
		memset(m_cSMSCenterNum, NULL, sizeof(m_cSMSCenterNum));
		strncpy(m_cSMSCenterNum, q.fieldValue("MsgCenterNum"), sizeof(m_cSMSCenterNum));
	}
	//�ͷ�statement
	q.finalize();
	db.close();	//�ر����ݿ�

	//2.�������Ͷ��Ŷ����߳�
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThrdSendSMS, this, 0, NULL);

	//3.�豸����״̬
	if( GetFileAttributes(PATH_SMS_RESET_SYS) != 0xFFFFFFFF )
	{
		DeleteFile(PATH_SMS_RESET_SYS);
		//�ظ���Ϣ
		ReplySMS("*TERMINAL RESET FINISH#");
	}

	return TRUE;
}

/************************************************************************
* ������: IsSMSEnable
* ��  ��: �жϵ�ǰ���ſ��ƹ����Ƿ����
* ��  ��: 
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::IsSMSEnable()
{
	return CSysConfig::Instance().GetSMS();
}

/************************************************************************
* ������: SetConnectState
* ��  ��: ���浱ǰ��������״̬
* ��  ��: nConnectState: ����״̬��0��RAS�Ͽ�	1��RAS����	2����¼������ 
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::SetConnectState(INT nConnectState)
{
	if(nConnectState<0 || nConnectState>2)
		return FALSE;

	m_nConnectState = nConnectState;
	return TRUE;
}

/************************************************************************
* ������: OnQueryParam
* ��  ��: �����·�������ѯָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::OnQueryParam(PCSTR pcParam)
{
	if(NULL == pcParam)
		return FALSE;

	stParam Param[13];
	memset(Param, NULL, sizeof(Param));
	GetParam(Param, 13);	//��ȡ�ն˲���

	//ƴ�Ӳ�ѯ�����б�
	CHAR	szParamList[512];
	memset(szParamList, NULL, sizeof(szParamList));
	//����code����
	WORD	wParamCodeLen = 0;
	//��ȡ��һ�ڲ���
	PCSTR	pcParamCode = pcParam;	//������ʼ��ַ
	INT		nFirstTime	= 0;
	while( (pcParamCode = ScanField(pcParamCode, nFirstTime, wParamCodeLen)) != NULL )
	{
		nFirstTime	= 1;	//��һ�δӵ�ǰ�ڿ�ʼ������֮�����һ��ѭ������
		if( !AddParam2List(Param, 13, pcParamCode, wParamCodeLen, szParamList) )
			return FALSE;
	}

	CLogTool::Instance()->WriteLogFile("Parse query parameter command success!");

	//���Ͳ�����ѯ�������
	SendParamList(szParamList);

	return TRUE;
}

/************************************************************************
* ������: OnSetParam
* ��  ��: �����·���������ָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: enReturnType:	0:�ɹ�		1:ָ�����(�ظ�*COMMAND ERR#)		2:��������(�ظ�*CONFIG ERR#):���������Ϲ涨
* ��  ע: *SZ,X:TZ-SN,A:CMNET,Y1:255.255.255.255,P:2000#
************************************************************************/
CSmsControl::enReturnType CSmsControl::OnSetParam(PCSTR pcParam)
{
	if(NULL == pcParam)
		return RESULT_CONFIG_ERR;

	stParam Param[6];	//������6����APN,������IP,����������,������IP,����������
	memset(Param, NULL, sizeof(Param));

	WORD wParamCount = 0;

	//���������б�pcParam����Ų���������Param��
	ParseParam(Param, 6, pcParam, wParamCount);

	//��������Ϸ���
	enReturnType enResult = VerifyParam(Param, wParamCount);
	if(enResult != RESULT_SUCCESS)
		return enResult;

	//���ò���
	SetParam(Param, wParamCount);

	CLogTool::Instance()->WriteLogFile("Set parameter success!");

	return RESULT_SUCCESS;
}

/************************************************************************
* ������: OnResetDevice
* ��  ��: �����·��ն�����ָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::OnResetDevice(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	//�ظ���Ϣ
	ReplySMS("*TERMINAL RESET START#");

	//д�ļ���¼����
	HANDLE hFile = ::CreateFile( PATH_SMS_RESET_SYS, GENERIC_READ, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if (  hFile != INVALID_HANDLE_VALUE  )
	{
		::CloseHandle( hFile );
	}

	CLogTool::Instance()->WriteLogFile("Reset system!");
	//�����豸
	CreateThread(NULL, 0, ThrdResetSystem, NULL, 0, NULL);

	return TRUE;
}

/************************************************************************
* ������: OnRestoreDefault
* ��  ��: �����·��ָ�����ֵָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::OnRestoreDefault(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	TCHAR  szFileDefault[MAX_PATH], szFileConfig[MAX_PATH];
	//��������
	memset(szFileDefault, 0, sizeof(szFileDefault) );
	memset(szFileConfig, 0, sizeof(szFileConfig) );
	ex_char2uni( PATH_DB_DEFAULT_CONFIG, szFileDefault, MAX_PATH );
	ex_char2uni( PATH_DB_CONFIG, szFileConfig, MAX_PATH );

	//Ĭ�������ļ�������
	if( GetFileAttributes(szFileDefault) == 0xFFFFFFFF )
	{
		//�ظ���Ϣ
		ReplySMS("*FACTORY RESET FAILED#");
		CLogTool::Instance()->WriteLogFile("Factory set file default.db doesn't exist!");
		return TRUE;
	}

	//����Ĭ�������ļ�
	CopyFile(szFileDefault, szFileConfig, FALSE);
	//�ظ���Ϣ
	ReplySMS("*FACTORY RESET SUCCEEDED#");
	CLogTool::Instance()->WriteLogFile("Factory reset success!");

	return TRUE;
}

/************************************************************************
* ������: OnCloseSMS
* ��  ��: �����·��رն��Ź���ʹ��Ȩ��ָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::OnDisableSMS(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	CSysConfig::Instance().SetSMS(FALSE);
	//�ظ���Ϣ
	ReplySMS("*CLOSE SMS SUCCEEDED#");
	CLogTool::Instance()->WriteLogFile("Disable SMS control!");

	return TRUE;
}

/************************************************************************
* ������: OnOpenSMS
* ��  ��: �����·��򿪶��Ź���ʹ��Ȩ��ָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::OnEnableSMS(PCSTR pcParam)
{
	if(pcParam != NULL)
		return FALSE;

	CSysConfig::Instance().SetSMS(TRUE);
	//�ظ���Ϣ
	ReplySMS("*OPEN SMS SUCCEEDED#");
	CLogTool::Instance()->WriteLogFile("Enable SMS control!");

	return TRUE;
}

/************************************************************************
* ������: OnDelFile
* ��  ��: ɾ��ָ���ļ�ָ��
* ��  ��: pcBody: ������
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::OnDelFile(PCSTR pcParam)
{
	if(NULL == pcParam)
		return FALSE;

	CHAR	szPath[MAX_PATH];
	memset(szPath, NULL, sizeof(szPath));

	//ת��·���е������ַ�
	DecodePath(szPath, pcParam);

	//��������
	TCHAR	szFilePath[MAX_PATH];
	memset( szFilePath, 0, sizeof(szFilePath) );
	ex_char2uni( szPath, szFilePath, MAX_PATH );

	//Ҫɾ�����ļ�����
	if( GetFileAttributes(szFilePath) != 0xFFFFFFFF )
	{
		DeleteFile(szFilePath);
	}

	return TRUE;
}

/************************************************************************
* ������: ScanField
* ��  ��: ��ȡ�ַ����е�һ��','�����ַ����ĵ�ַ
* ��  ��: 1. pcBuffer:	Ҫ�����Ķ���Э���ַ�����ȥ��'*','#'
2. wFieldNO:	�������ַ����Ľ�(����','֮����ַ���)��0����ǰ�ڣ�1����һ�ڣ�2,3...
* ��  ��: 1. wLen:		�ڵĳ���
* ��  ��: CHAR*:		ָ��ڵ��ַ��������ΪNULL�����ȡ���ַ���ʧ��
* ��  ע: 
************************************************************************/
PCSTR CSmsControl::ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen)
{
	PCSTR pcFieldBegin = pcBuffer;

	while( nFieldNO-- > 0 )
	{
		while ( pcFieldBegin && *pcFieldBegin++ != ',' && *pcFieldBegin != NULL )
			;
	}
	//�����Ľ�Ϊ��
	if(NULL == *pcFieldBegin)
	{
		wLen = 0;
		return NULL;
	}

	//�ڵ����һ���ַ�ָ��ĺ�һλ
	PCSTR pcFieldEnd = pcFieldBegin;
	while( pcFieldEnd && *pcFieldEnd != ',' && *pcFieldEnd != NULL )
	{
		pcFieldEnd++;
	}

	//����ڵĳ���
	wLen = pcFieldEnd - pcFieldBegin;

	return pcFieldBegin;
}

/************************************************************************
* ������: DataDispatch
* ��  ��: �������ؽ��
* ��  ��: 1. pcBuffer:		Ҫ������Э����Ϣ�ַ���
2. wBufferLen:	Ҫ������Э����Ϣ�ַ����ĳ���
* ��  ��: 
* ��  ��: BOOL
* ��  ע:
************************************************************************/
BOOL CSmsControl::DataDispatch(PCSTR pcBuffer, WORD wBufferLen)
{
	if(NULL == pcBuffer)
		return FALSE;

	BOOL	bResult		= FALSE;
	WORD	wFieldLen	= 0;
	//У������
	PCSTR	pcPWD = ScanField(pcBuffer, 1, wFieldLen);
	if( !CheckPWD( pcPWD, wFieldLen ) )
	{
		goto END_PASSWORD_ERR;
	}

	//��ȡ��������
	PCSTR	pcCMD_ID	= ScanField(pcBuffer, 0, wFieldLen);
	//��ȡ�����б�
	WORD	wParamLen	= 0;
	PCSTR	pcParam		= ScanField(pcBuffer, 2, wParamLen);

	//1.�����·�������ѯָ��:	*CX,X:TZ-SN,A,I1,P,D#
	if( strncmp(pcCMD_ID, "CX", wFieldLen) == 0 )
	{
		if( OnQueryParam( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//2.�����·���������ָ��:	*SZ,X:TZ-SN,A:CMNET,Y1:255.255.255.255,P:2000#
	else if( strncmp(pcCMD_ID, "SZ", wFieldLen) == 0 )
	{
		switch( OnSetParam( pcParam ) )
		{
		case RESULT_SUCCESS:		//�ɹ�
			goto END_SUCCESS;
		case RESULT_COMMAND_ERR:	//ָ�����(�ظ�*COMMAND ERR#)
			goto END_COMMAND_ERR;
		case RESULT_CONFIG_ERR:		//��������(�ظ�*CONFIG ERR#)
			goto END_CONFIG_ERR;
		}
	}
	//3.�����·��ն�����ָ��:	*RESET,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "RESET", wFieldLen) == 0 )
	{
		if( OnResetDevice( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//10.�����·��ָ�����ֵָ��:	*HF,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "HF", wFieldLen) == 0 )
	{
		if( OnRestoreDefault( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//13.�����·��رն��Ź���ʹ��Ȩ��ָ��:	*CLOSE SMS,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "CLOSE SMS", wFieldLen) == 0 )
	{
		if( OnDisableSMS( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//14.�����·��򿪶��Ź���ʹ��Ȩ��ָ��:	*OPEN SMS,X:TZ-SN#
	else if( strncmp(pcCMD_ID, "OPEN SMS", wFieldLen) == 0 )
	{
		if( OnEnableSMS( pcParam ) )
			goto END_SUCCESS;
		else
			goto END_COMMAND_ERR;
	}
	//17.ɾ��ָ���ļ�ָ��:	*DEL,X:TZ-SN,PATH#
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

END_PASSWORD_ERR:	//У���������
	ReplySMS("*PASSWORD ERR#");
	return FALSE;

END_COMMAND_ERR:	//ָ�����
	CLogTool::Instance()->WriteLogFile("COMMAND ERR");
	ReplySMS("*COMMAND ERR#");
	return FALSE;

END_CONFIG_ERR:		//���õĲ��������Ϲ涨
	CLogTool::Instance()->WriteLogFile("CONFIG ERR");
	ReplySMS("*CONFIG ERR#");
	return FALSE;

END_SUCCESS:
	return TRUE;
}

/************************************************************************
* ������: CheckPWD
* ��  ��: У������"X:TZ-SN"
* ��  ��: 1. pcBuffer:		�����ַ���
2. wBufferLen:	�����ַ�������
* ��  ��: 
* ��  ��: BOOL
* ��  ע:
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
* ������: ReplySMS
* ��  ��: �ظ�����
* ��  ��: 
1. pcSMS:	�����ַ���
2. wSMSLen:	�����ַ�������
* ��  ��: 
* ��  ��: BOOL
* ��  ע:
************************************************************************/
BOOL CSmsControl::ReplySMS(PCSTR pcSMS/*, WORD wSMSLen*/)
{
	stSMS sms_buffer;
	memset(&sms_buffer, NULL, sizeof(stSMS));
	strcpy(sms_buffer.szPhoneNum, m_cPhoneNum);
	strncpy(sms_buffer.szContent, pcSMS, 140);

	//���������뷢�Ͷ���
	m_queueSendSMS.push(sms_buffer);

	CLogTool::Instance()->WriteLogFile(pcSMS);
	return TRUE;
}

/************************************************************************
* ������: GetParam
* ��  ��: ��ȡ�ն˲���
* ��  ��: 
1. pcSMS:	�����ַ���
2. wSMSLen:	�����ַ�������
* ��  ��: 
* ��  ��: BOOL
* ��  ע:
************************************************************************/
BOOL CSmsControl::GetParam(stParam Param[], INT nParamCount)
{
	CppSQLite3DB db;
	db.open(PATH_DB_CONFIG);	//�����ݿ�
	//�����ݿ��в�ѯ
	char szSQL[512];
	sprintf(szSQL, "SELECT * FROM config;");
	CppSQLite3Query q = db.execQuery(szSQL);
	if ( !q.eof() )
	{
		//A		APN
		strcpy(Param[0].szParamCode, g_szAPN);
		strcpy(Param[0].szParamValue, q.fieldValue("APN"));
		//I1	������IP
		strcpy(Param[1].szParamCode, g_szMainCenterIPAddress);
		strcpy(Param[1].szParamValue, q.fieldValue("MainCenterIPAddress"));
		//Y1	����������
		strcpy(Param[2].szParamCode, g_szMainCenterDomainName);
		strcpy(Param[2].szParamValue, q.fieldValue("MainCenterDomainName"));
		//I2	������IP
		strcpy(Param[3].szParamCode, g_szPairCenterIPAddress);
		strcpy(Param[3].szParamValue, q.fieldValue("PairCenterIPAddress"));
		//Y2	����������
		strcpy(Param[4].szParamCode, g_szPairCenterDomainName);
		strcpy(Param[4].szParamValue, q.fieldValue("PairCenterDomainName"));
		//P		�˿�
		strcpy(Param[5].szParamCode, g_szPortNum);
		strcpy(Param[5].szParamValue, q.fieldValue("PortNum"));
		//S		����汾
		strcpy(Param[6].szParamCode, g_szSoftwareVersion);
		strcpy(Param[6].szParamValue, q.fieldValue("SoftwareVersion"));
		//H		Ӳ���汾
		strcpy(Param[7].szParamCode, g_szHardwareVersion);
		strcpy(Param[7].szParamValue, q.fieldValue("Pro_Version"));
		//ID	ID��
		strcpy(Param[10].szParamCode, g_szID);
		strcpy(Param[10].szParamValue, q.fieldValue("FactoryId"));
		//DI	��ǰ����IP
		strcpy(Param[11].szParamCode, g_szCurrent_Ip);
		strcpy(Param[11].szParamValue, q.fieldValue("Current_Ip"));
		//DP	��ǰ�˿�
		strcpy(Param[12].szParamCode, g_szCurrent_Port);
		strcpy(Param[12].szParamValue, q.fieldValue("Current_Port"));
	}
	//�ͷ�statement
	q.finalize();
	db.close();	//�ر����ݿ�

	//L		����״̬  0��RAS�Ͽ�	1��RAS����	2����¼������ 
	strcpy(Param[8].szParamCode, g_szConnectState);
	if(0 == m_nConnectState)
		sprintf(Param[8].szParamValue, "OFF");
	else if(1 == m_nConnectState)
		sprintf(Param[8].szParamValue, "ON");
	else if(2 == m_nConnectState)
		sprintf(Param[8].szParamValue, "OK");

	//D		���ǿ���
	strcpy(Param[9].szParamCode, g_szSatelliteCount);
	sprintf(Param[9].szParamValue, "%02d", g_pstShareMem->st_GPS.m_starnum);

	return TRUE;
}

/************************************************************************
* ������:	ParseParam
* ��  ��:	�����ն˲���
* ��  ��:	1. Param:		��Ž�������ն˲���������
			2. nArrayCount:	�����С
			3. pcParamList:	�����б�
* ��  ��:	1. nParamCount:	�����б��в�������
* ��  ��:	BOOL
* ��  ע:	A:CMNET,Y1:255.255.255.255,P:2000
************************************************************************/
BOOL CSmsControl::ParseParam(stParam Param[], const INT nArrayCount, PCSTR pcParamList, WORD& wParamCount)
{
	if(NULL == pcParamList)
		return FALSE;

	PCSTR	pcFieldStart	= pcParamList;
	WORD	wFieldLen		= 0;
	INT		nFirstField		= 0;	//��ʼ�ӵ�һ��������֮�����һ�ڿ�ʼ����
	for(wParamCount=0; wParamCount<nArrayCount; wParamCount++)
	{
		pcFieldStart	= ScanField(pcFieldStart, nFirstField, wFieldLen);
		nFirstField		= 1;
		//�������
		if(NULL == pcFieldStart)
			return TRUE;

		if(wFieldLen < 3)
			return FALSE;

		for(INT i=1; i<wFieldLen-1; i++)
		{
			//':'�ָ���������Ͳ���ֵ
			if(':' == pcFieldStart[i])
			{
				//��������
				_snprintf(Param[wParamCount].szParamCode, i, "%s", pcFieldStart);
				//����ֵ
				_snprintf(Param[wParamCount].szParamValue, wFieldLen-i-1, "%s", &pcFieldStart[i+1]);
				break;
			}
		}
	}

	return TRUE;
}

/************************************************************************
* ������: VerifyParam
* ��  ��: �������Ƿ���Ϲ涨
* ��  ��: 
1. Param:		��������
2. nArrayCount:	��������Ԫ�ظ���
* ��  ��: 
* ��  ��: enReturnType:	0:�ɹ�		1:�����������		2:����ֵ�����Ϲ涨
* ��  ע:
A	APN			����CMNET
I1	������IP	����255.255.255.255
Y1	����������	����v.cheguan.com
I2	������IP	����255.255.255.255
Y2	����������	����v.cheguan.com
P	�˿�		����2000
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
		//������IP
		else if( strcmp(Param[i].szParamCode, g_szMainCenterIPAddress) == 0 )
		{
			if( !VerifyIP(Param[i].szParamValue) )
				return RESULT_CONFIG_ERR;
		}
		//����������
		else if( strcmp(Param[i].szParamCode, g_szMainCenterDomainName) == 0 )
		{
		}
		//������IP
		else if( strcmp(Param[i].szParamCode, g_szPairCenterIPAddress) == 0 )
		{
			if( !VerifyIP(Param[i].szParamValue) )
				return RESULT_CONFIG_ERR;
		}
		//����������
		else if( strcmp(Param[i].szParamCode, g_szPairCenterDomainName) == 0 )
		{
		}
		//�˿�
		else if( strcmp(Param[i].szParamCode, g_szPortNum) == 0 )
		{
			if(  atoi(Param[i].szParamValue) > 0xFFFF )
				return RESULT_CONFIG_ERR;
		}
		else
		{//�����������
			return RESULT_COMMAND_ERR;
		}
	}

	//�ɹ�
	return RESULT_SUCCESS;
}

/************************************************************************
* ������: SetParam
* ��  ��: �����ն˲���
* ��  ��: 
1. Param:		��������
2. nArrayCount:	��������(С��6)
* ��  ��: 
* ��  ��: BOOL
* ��  ע:
************************************************************************/
BOOL CSmsControl::SetParam(stParam Param[], const INT nArrayCount)
{
	if(nArrayCount > 6)
		return FALSE;

	stParam CurrentParam[6];
	memset(CurrentParam, NULL, sizeof(CurrentParam));

	CppSQLite3DB db;
	db.open(PATH_DB_CONFIG);	//�����ݿ�
	//�����ݿ��в�ѯ
	char szSQL[512];
	sprintf(szSQL, "SELECT * FROM config;");
	CppSQLite3Query q = db.execQuery(szSQL);
	if ( !q.eof() )
	{
		//A		APN
		strcpy(CurrentParam[0].szParamCode, g_szAPN);
		strcpy(CurrentParam[0].szParamValue, q.fieldValue("APN"));
		//I1	������IP
		strcpy(CurrentParam[1].szParamCode, g_szMainCenterIPAddress);
		strcpy(CurrentParam[1].szParamValue, q.fieldValue("MainCenterIPAddress"));
		//Y1	����������
		strcpy(CurrentParam[2].szParamCode, g_szMainCenterDomainName);
		strcpy(CurrentParam[2].szParamValue, q.fieldValue("MainCenterDomainName"));
		//I2	������IP
		strcpy(CurrentParam[3].szParamCode, g_szPairCenterIPAddress);
		strcpy(CurrentParam[3].szParamValue, q.fieldValue("PairCenterIPAddress"));
		//Y2	����������
		strcpy(CurrentParam[4].szParamCode, g_szPairCenterDomainName);
		strcpy(CurrentParam[4].szParamValue, q.fieldValue("PairCenterDomainName"));
		//P		�˿�
		strcpy(CurrentParam[5].szParamCode, g_szPortNum);
		strcpy(CurrentParam[5].szParamValue, q.fieldValue("PortNum"));
	}
	//�ͷ�statement
	q.finalize();

	//�������
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

	//���浽���ݿ�
	sprintf(szSQL, "SELECT * FROM config;");
	sprintf(szSQL, "UPDATE config SET APN = '%s', MainCenterIPAddress = '%s', MainCenterDomainName = '%s',	\
		PairCenterIPAddress = '%s', PairCenterDomainName = '%s', PortNum = '%s';", 
		CurrentParam[0].szParamValue, CurrentParam[1].szParamValue, CurrentParam[2].szParamValue, 
		CurrentParam[3].szParamValue, CurrentParam[4].szParamValue, CurrentParam[5].szParamValue);
	db.execDML(szSQL);
	db.close();		//�ر����ݿ�

	return TRUE;
}

/************************************************************************
* ������: AddParam2List
* ��  ��: ƴ�Ӳ�ѯ�����б�
* ��  ��: 
1. Param:		��ǰ�����ṹ������
2. nParamCount:	����Ԫ�ظ���
3. pcParamCode:	��ƴ�ӵĲ�������
4. wParamCodeLen:	�������볤��
5. szParamList:	����ƴ�ӵ�buffer
* ��  ��: 
* ��  ��: BOOL
* ��  ע:
************************************************************************/
BOOL CSmsControl::AddParam2List(stParam Param[], INT nParamCount, PCSTR pcParamCode, WORD wParamCodeLen, PSTR szParamList)
{
	//������ǰ��������
	for(INT i=0; i<nParamCount; i++)
	{
		//ƥ���������
		if( strncmp(Param[i].szParamCode, pcParamCode, wParamCodeLen) == 0 )
		{
			//�������ֵ�������б���
			WORD wLen = strlen(szParamList);
			if( wLen > 0 )
			{//����֮����','���
				szParamList[wLen++] = ',';
			}
			sprintf(&szParamList[wLen], "%s:%s", Param[i].szParamCode, Param[i].szParamValue);
			return TRUE;
		}
	}
	return FALSE;
}

/************************************************************************
* ������: SendParamList
* ��  ��: ���Ͳ�ѯ�Ĳ����б�
* ��  ��: 
1. szParamList:	�����ַ���
* ��  ��: 
* ��  ��: BOOL
* ��  ע: ������͵Ĳ����б���ų���140�ֽڣ���Ҫ��ֶ������ͣ�
�磺*M:TZ-SN,A:CMNET,I1:255.255.255,P:2000,D:06#
��ȥ�����ַ���һ�����ŵĲ����б��е��ַ����130�ֽ�
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

	//�����б��ֽ���С��130
	if(wLen <= 130)
	{
		sprintf(szSMS, "*M:TZ-SN,%s#", pszParamList);
		ReplySMS(szSMS);
		return TRUE;
	}

	//�����б��ֽ���С��130����Ҫ���
	WORD	wFieldLen		= 0;	//��ǰ�ڵĳ���
	WORD	wSMSCount		= 1;	//��ֶ��ŵ�������Ĭ��Ϊ��һ��
	PCSTR	pcSubSMSStart	= pszParamList;		//��i����ֶ��ŵ��׵�ַ
	//PCSTR	pcFirstField	= pcSubSMSStart;	//
	//PCSTR	pcNextField		= NULL;	//
	PCSTR	pcFieldStart	= pcSubSMSStart;	//
	while( (pcFieldStart == ScanField(pcFieldStart, 1, wFieldLen)) != NULL )
	{//A:CMNET,I1:255.255.255,P:2000,D:06 
		WORD nSMSLen = pcFieldStart - pcSubSMSStart;
		//��Խ�������ŵ��ٽ�
		if( nSMSLen-1<=130 && nSMSLen+wFieldLen>130 )
		{
			memset(szSMS, NULL, sizeof(szSMS));
			_snprintf(szSMS, nSMSLen, "*M:TZ-SN,%s#", pcSubSMSStart);
			ReplySMS(szSMS);

			pcSubSMSStart = pcFieldStart;
		}
		//pcFirstField = pcNextField;
	}
	//���һ������
	memset(szSMS, NULL, sizeof(szSMS));
	sprintf(szSMS, "*M:TZ-SN,%s#", pcSubSMSStart);
	ReplySMS(szSMS);

	return TRUE;
}

/************************************************************************
* ������: DecodePath
* ��  ��: ·���е������ַ�����
* ��  ��:	1. szPath:			ת������ַ���
			2. szOriginPath:	ԭʼ�ַ���
* ��  ��: 
* ��  ��: BOOL
* ��  ע: ���Ž��պ�һ���������ַ������ı䣬��Ҫ�����ԭ���ַ�������������£�
�����ַ���\!@#$%^&()_+-=[]{};',.~`
����ת���Ϊ��
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
'@'��ת��Ϊ0x00,�򲻴���
************************************************************************/
BOOL CSmsControl::DecodePath(PSTR szPath, PCSTR szOriginPath)
{
	if(NULL==szPath || NULL==szOriginPath)
		return FALSE;

	//���ų���
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
			//���ȼ���1
			nPathLen--;
		}
		//0x1B14->'^'
		if(0x1B==szPath[i] && 0x14==szPath[i+1])
		{
			szPath[i]	= '^';
				for(j=i+1; j<nPathLen; j++)
					szPath[j] = szPath[j+1];
			//���ȼ���1
			nPathLen--;
		}
		//0x1B3C->'['
		if(0x1B==szPath[i] && 0x3C==szPath[i+1])
		{
			szPath[i]	= '[';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//���ȼ���1
			nPathLen--;
		}
		//0x1B3E->']'
		if(0x1B==szPath[i] && 0x3E==szPath[i+1])
		{
			szPath[i]	= ']';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//���ȼ���1
			nPathLen--;
		}
		//0x1B28->'{'
		if(0x1B==szPath[i] && 0x28==szPath[i+1])
		{
			szPath[i]	= '{';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//���ȼ���1
			nPathLen--;
		}
		//0x1B29->'}'
		if(0x1B==szPath[i] && 0x29==szPath[i+1])
		{
			szPath[i]	= '}';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//���ȼ���1
			nPathLen--;
		}
		//0x1B3D->'~'
		if(0x1B==szPath[i] && 0x3D==szPath[i+1])
		{
			szPath[i]	= '~';
			for(j=i+1; j<nPathLen; j++)
				szPath[j] = szPath[j+1];
			//���ȼ���1
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
* ������: VerifyIP
* ��  ��: ���IP�Ϸ���
* ��  ��: 1. pszIP:	IP�ַ���
* ��  ��: 
* ��  ��: BOOL
* ��  ע: 
************************************************************************/
BOOL CSmsControl::VerifyIP(PCSTR pszIP)
{
	//IP���
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
* ������: ThrdResetSystem
* ��  ��: �����豸�߳�
* ��  ��: 
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
DWORD CSmsControl::ThrdResetSystem(LPVOID lpParameter)
{
	Sleep(5*1000);
	CDllMidware::Instance().SystemReset();
	return 1;
}

/************************************************************************
* ������: ThrdSendSMS
* ��  ��: ���Ͷ����߳�
* ��  ��: 
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
DWORD CSmsControl::ThrdSendSMS(LPVOID lpParameter)
{
	CSmsControl* pDlg = (CSmsControl*)lpParameter;
	while(TRUE)
	{
		//�ڲ�ѯ������isp��ʱ����ܷ��Ͷ���
		while( !pDlg->m_queueSendSMS.empty() && (g_GsmLogic.GetGSMNetType()!=0) )
		{
			//�Ӷ�����ȡ����һ��record
			stSMS &stBuffer = pDlg->m_queueSendSMS.front();

			CString strSMSContent(stBuffer.szContent), strPhoneNum(stBuffer.szPhoneNum);
			//CString strSMSContent("*FACTORY RESET SUCCEEDED#"), strPhoneNum("13482636300");
			g_GsmLogic.SetPhoneNum( strPhoneNum );
			g_GsmLogic.SetMsgBuf( strSMSContent );
			g_GsmLogic.GSMSendSMS();

			//���ͳɹ��򵯳�
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
