#pragma once
#include <queue>

#include "../ICCardTools/DataParser/DataType.h"

#define PATH_DB_CONFIG			"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\config.db"
#define PATH_DB_DEFAULT_CONFIG	"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\default.db"

#define PATH_SMS_RESET_SYS		_T("\\Flashdrv Storage\\Menu\\RESET_SYS")

using std::queue;

class CSmsControl
{
public:
	//����ֵ����
	typedef enum _enReturnType
	{
		RESULT_SUCCESS	= 0,		//�ɹ�
		RESULT_COMMAND_ERR,			//ָ�����
		RESULT_CONFIG_ERR,			//��������:���������Ϲ涨
	} enReturnType;

	//�ն˲����ṹ��
	typedef struct _stParam
	{
		char szParamCode[4];
		char szParamValue[68];
	} stParam;

	//���Žṹ��
	typedef struct _stSMS
	{
		char szPhoneNum[16];		//���ź���
		char szContent[140];		//�������ݣ�������140�ֽ�
	} stSMS;

private:
	static CSmsControl*		m_pInstance;				//Singletonģʽ

	CHAR					m_cContent[140];			//��������
	WORD					m_wContentLen;				//���ų���
	CHAR					m_cPhoneNum[16];			//���ն��ŵ绰����
	CHAR					m_cSMSCenterNum[16];		//�������ĺ���
	queue<stSMS>			m_queueSendSMS;				//���Ͷ��Ŷ���

	//BOOL					m_bSMSEnable;				//���Ź����Ƿ����
	INT						m_nConnectState;			//����״̬��0��RAS�Ͽ�	1��RAS����	2����¼������ 

	//TRUE:���ݽ����ɹ�		FALSE:���ݽ���ʧ��
	BOOL					m_bDataStatus;

public:
	CSmsControl(void);
	~CSmsControl(void);

	static CSmsControl* Instance();

	//Э�����
	BOOL	SmsParser(PCSTR pcPhoneNum, PCSTR pcContent);
	//��ʼ�����ſ���
	BOOL	InitSmsControl();
	//��ȡ���Ź����Ƿ���Ч
	BOOL	IsSMSEnable();
	//������������״̬
	BOOL	SetConnectState(INT nConnectState);

private:
	//�����·�������ѯָ��
	BOOL	OnQueryParam(PCSTR pcParam);
	//�����·���������ָ��
	enReturnType	OnSetParam(PCSTR pcParam);
	//�����·��ն�����ָ��
	BOOL	OnResetDevice(PCSTR pcParam);
	//�����·��ָ�����ֵָ��
	BOOL	OnRestoreDefault(PCSTR pcParam);
	//�����·��رն��Ź���ʹ��Ȩ��ָ��
	BOOL	OnDisableSMS(PCSTR pcParam);
	//�����·��򿪶��Ź���ʹ��Ȩ��ָ��
	BOOL	OnEnableSMS(PCSTR pcParam);
	//ɾ��ָ���ļ�ָ��
	BOOL	OnDelFile(PCSTR pcParam);

	//�ظ�����
	BOOL	ReplySMS(PCSTR pcSMS/*, WORD wSMSLen*/);

private:
	//��ȡ���ַ���
	PCSTR	ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen);
	//������Ϣ���ͷַ�
	BOOL	DataDispatch(PCSTR pcBuffer, WORD wBufferLen);
	//У������
	BOOL	CheckPWD(PCSTR pcBuffer, WORD wBufferLen);
	//��ȡ�ն˲���
	BOOL	GetParam(stParam Param[], INT nParamCount);
	//�����ն˲���
	BOOL	ParseParam(stParam Param[], const INT nArrayCount, PCSTR pcParamList, WORD& wParamCount);
	//�����ն˲���
	BOOL	SetParam(stParam Param[], const INT nArrayCount);
	//ƴ�Ӳ�ѯ�����б�
	BOOL	AddParam2List(stParam Param[], INT nParamCount, PCSTR pcParamCode, WORD wParamCodeLen, PSTR szParamList);
	//���Ͳ�ѯ�Ĳ����б�
	BOOL	SendParamList(PCSTR pszParamList);
	//·���е������ַ�����
	BOOL	DecodePath(PSTR szPath, PCSTR szOriginPath);
	/*********�������Ƿ�Ϸ�********/
	//�������Ƿ���Ϲ涨
	enReturnType	VerifyParam(stParam Param[], const INT nArrayCount);
	//���IP
	BOOL	VerifyIP(PCSTR pszIP);

	/*************�̺߳���********************/
	static DWORD ThrdResetSystem(LPVOID lpParameter);
	static DWORD ThrdSendSMS(LPVOID lpParameter);
};
