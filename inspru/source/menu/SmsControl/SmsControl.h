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
	//返回值类型
	typedef enum _enReturnType
	{
		RESULT_SUCCESS	= 0,		//成功
		RESULT_COMMAND_ERR,			//指令错误
		RESULT_CONFIG_ERR,			//参数错误:参数不符合规定
	} enReturnType;

	//终端参数结构体
	typedef struct _stParam
	{
		char szParamCode[4];
		char szParamValue[68];
	} stParam;

	//短信结构体
	typedef struct _stSMS
	{
		char szPhoneNum[16];		//短信号码
		char szContent[140];		//短信内容，不超过140字节
	} stSMS;

private:
	static CSmsControl*		m_pInstance;				//Singleton模式

	CHAR					m_cContent[140];			//短信内容
	WORD					m_wContentLen;				//短信长度
	CHAR					m_cPhoneNum[16];			//接收短信电话号码
	CHAR					m_cSMSCenterNum[16];		//短信中心号码
	queue<stSMS>			m_queueSendSMS;				//发送短信队列

	//BOOL					m_bSMSEnable;				//短信功能是否可用
	INT						m_nConnectState;			//连接状态：0：RAS断开	1：RAS连接	2：登录服务器 

	//TRUE:数据解析成功		FALSE:数据解析失败
	BOOL					m_bDataStatus;

public:
	CSmsControl(void);
	~CSmsControl(void);

	static CSmsControl* Instance();

	//协议解析
	BOOL	SmsParser(PCSTR pcPhoneNum, PCSTR pcContent);
	//初始化短信控制
	BOOL	InitSmsControl();
	//获取短信功能是否有效
	BOOL	IsSMSEnable();
	//设置网络连接状态
	BOOL	SetConnectState(INT nConnectState);

private:
	//中心下发参数查询指令
	BOOL	OnQueryParam(PCSTR pcParam);
	//中心下发参数设置指令
	enReturnType	OnSetParam(PCSTR pcParam);
	//中心下发终端重启指令
	BOOL	OnResetDevice(PCSTR pcParam);
	//中心下发恢复出厂值指令
	BOOL	OnRestoreDefault(PCSTR pcParam);
	//中心下发关闭短信功能使用权限指令
	BOOL	OnDisableSMS(PCSTR pcParam);
	//中心下发打开短信功能使用权限指令
	BOOL	OnEnableSMS(PCSTR pcParam);
	//删除指定文件指令
	BOOL	OnDelFile(PCSTR pcParam);

	//回复短信
	BOOL	ReplySMS(PCSTR pcSMS/*, WORD wSMSLen*/);

private:
	//获取节字符串
	PCSTR	ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen);
	//根据消息类型分发
	BOOL	DataDispatch(PCSTR pcBuffer, WORD wBufferLen);
	//校验密码
	BOOL	CheckPWD(PCSTR pcBuffer, WORD wBufferLen);
	//获取终端参数
	BOOL	GetParam(stParam Param[], INT nParamCount);
	//解析终端参数
	BOOL	ParseParam(stParam Param[], const INT nArrayCount, PCSTR pcParamList, WORD& wParamCount);
	//设置终端参数
	BOOL	SetParam(stParam Param[], const INT nArrayCount);
	//拼接查询参数列表
	BOOL	AddParam2List(stParam Param[], INT nParamCount, PCSTR pcParamCode, WORD wParamCodeLen, PSTR szParamList);
	//发送查询的参数列表
	BOOL	SendParamList(PCSTR pszParamList);
	//路径中的特殊字符处理
	BOOL	DecodePath(PSTR szPath, PCSTR szOriginPath);
	/*********检查参数是否合法********/
	//检查参数是否符合规定
	enReturnType	VerifyParam(stParam Param[], const INT nArrayCount);
	//检查IP
	BOOL	VerifyIP(PCSTR pszIP);

	/*************线程函数********************/
	static DWORD ThrdResetSystem(LPVOID lpParameter);
	static DWORD ThrdSendSMS(LPVOID lpParameter);
};
