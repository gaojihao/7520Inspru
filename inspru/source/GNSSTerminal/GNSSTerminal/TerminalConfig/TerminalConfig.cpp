#include "StdAfx.h"
#include "TerminalConfig.h"
#include "../GPSData/GPSData.h"

//初始化静态成员变量
CTerminalConfig*	CTerminalConfig::m_pInstance = NULL;

CTerminalConfig::CTerminalConfig(void)
{
	memset(&m_TerminalParam, NULL, sizeof(m_TerminalParam));
}

CTerminalConfig::~CTerminalConfig(void)
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
CTerminalConfig* CTerminalConfig::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CTerminalConfig;
	}
	return m_pInstance;
}

/************************************************************************
* 函数名:	LoadConfigFile
* 描  述:	加载config文件到内存中
* 入  参:	strPath: config文件路径
* 出  参:	
* 返  回:	
* 备  注:	
************************************************************************/
BOOL CTerminalConfig::LoadConfigFile(CString strPath)
{
	CIniFile	IniFile;
	memset(&m_TerminalParam, NULL, sizeof(m_TerminalParam));
	//加载文件失败
	if( !IniFile.Load( strPath ) )
		return FALSE;

	//保存文件路径
	m_strConfigFilePath	= strPath;

	//(0x0001)终端心跳发送间隔，单位为秒(s)
	IniFile.GetInt(_T("TerminalParam"), _T("HEART_BEAT_SLICE"), m_TerminalParam.dwHEART_BEAT_SLICE);
	//(0x0002)TCP消息应答超时时间，单位为秒(s)
	IniFile.GetInt(_T("TerminalParam"), _T("TCP_ACK_OVERTIME"), m_TerminalParam.dwTCP_ACK_OVERTIME);
	//(0x0003)TCP消息重传次数
	IniFile.GetInt(_T("TerminalParam"), _T("TCP_RETRANSMIT_COUNT"), m_TerminalParam.dwTCP_RETRANSMIT_COUNT);
	//(0x0004)UDP消息应答超时时间，单位为秒(s)
	IniFile.GetInt(_T("TerminalParam"), _T("UDP_ACK_OVERTIME"), m_TerminalParam.dwUDP_ACK_OVERTIME);
	//(0x0005)UDP消息重传次数
	IniFile.GetInt(_T("TerminalParam"), _T("UDP_RETRANSMIT_COUNT"), m_TerminalParam.dwUDP_RETRANSMIT_COUNT);
	//(0x0006)SMS消息应答超时时间，单位为秒(s)
	IniFile.GetInt(_T("TerminalParam"), _T("SMS_ACK_OVERTIME"), m_TerminalParam.dwSMS_ACK_OVERTIME);
	//(0x0007)SMS消息重传次数
	IniFile.GetInt(_T("TerminalParam"), _T("SMS_RETRANSMIT_COUNT"), m_TerminalParam.dwSMS_RETRANSMIT_COUNT);
	//0x0008~0x000F		保留
	//(0x0010)主服务器APN
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_APN"), m_TerminalParam.szMAIN_SVR_APN, sizeof(m_TerminalParam.szMAIN_SVR_APN));
	//(0x0011)主服务器无线通信拨号用户名
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_NAME"), m_TerminalParam.szMAIN_SVR_NAME, sizeof(m_TerminalParam.szMAIN_SVR_NAME));
	//(0x0012)主服务器无线通信拨号密码
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_PWD"), m_TerminalParam.szMAIN_SVR_PWD, sizeof(m_TerminalParam.szMAIN_SVR_PWD));
	//(0x0013)主服务器地址，IP或域名
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_IP"), m_TerminalParam.szMAIN_SVR_IP, sizeof(m_TerminalParam.szMAIN_SVR_IP));
	//(0x0014)备份服务器APN
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_APN"), m_TerminalParam.szBAK_SVR_APN, sizeof(m_TerminalParam.szBAK_SVR_APN));
	//(0x0015)备份服务器无线通信拨号用户名
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_NAME"), m_TerminalParam.szBAK_SVR_NAME, sizeof(m_TerminalParam.szBAK_SVR_NAME));
	//(0x0016)备份服务器无线通信拨号密码
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_PWD"), m_TerminalParam.szBAK_SVR_PWD, sizeof(m_TerminalParam.szBAK_SVR_PWD));
	//(0x0017)备份服务器地址，IP或域名
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_IP"), m_TerminalParam.szBAK_SVR_IP, sizeof(m_TerminalParam.szBAK_SVR_IP));
	//(0x0018)服务器TCP端口
	IniFile.GetInt(_T("TerminalParam"), _T("SVR_TCP_PORT"), m_TerminalParam.dwSVR_TCP_PORT);
	//(0x0019)服务器UDP端口
	IniFile.GetInt(_T("TerminalParam"), _T("SVR_UDP_PORT"), m_TerminalParam.dwSVR_UDP_PORT);
	//0x001A~0x001F		保留
	//(0x0020)位置汇报策略
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_TYPE"), m_TerminalParam.dwPOS_REPORT_TYPE);
	//(0x0021)位置汇报方案
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_CONDITION"), m_TerminalParam.dwPOS_REPORT_CONDITION);
	//(0x0022)驾驶员未登录汇报时间间隔，单位为秒(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_LOGOUT"), m_TerminalParam.dwPOS_REPORT_SLICE_LOGOUT);
	//0x0023~0x0026		保留
	//(0x0027)休眠时汇报时间间隔，单位为秒(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_SLEEP"), m_TerminalParam.dwPOS_REPORT_SLICE_SLEEP);
	//(0x0028)紧急报警时汇报时间间隔，单位为秒(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_ALARM"), m_TerminalParam.dwPOS_REPORT_SLICE_ALARM);
	//(0x0029)缺省时间汇报间隔，单位为秒(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_DEFAULT"), m_TerminalParam.dwPOS_REPORT_SLICE_DEFAULT);
	//0x002A~0x002B		保留
	//(0x002C)缺省距离汇报间隔，单位为米(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_DEFAULT"), m_TerminalParam.dwPOS_REPORT_DIST_DEFAULT);
	//(0x002D)驾驶员未登录汇报距离间隔，单位为米(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_LOGOUT"), m_TerminalParam.dwPOS_REPORT_DIST_LOGOUT);
	//(0x002E)休眠时汇报距离间隔，单位为米(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_SLEEP"), m_TerminalParam.dwPOS_REPORT_DIST_SLEEP);
	//(0x002F)紧急报警时汇报距离间隔，单位为米(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_ALARM"), m_TerminalParam.dwPOS_REPORT_DIST_ALARM);
	//(0x0030)拐点补传角度，<180
	IniFile.GetInt(_T("TerminalParam"), _T("CORNER_RETRANSMIT_ANGLE"), m_TerminalParam.dwCORNER_RETRANSMIT_ANGLE);
	//(0x0031)电子围栏半径（非法位移阈值），单位为米  Radius of the electronic fence
	IniFile.GetInt(_T("TerminalParam"), _T("ELECTRONIC_FENCE_RADIUS"), m_TerminalParam.wELECTRONIC_FENCE_RADIUS);
	//0x0032~0x003F		保留
	//(0x0040)监控平台电话号码
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_MONITOR"), m_TerminalParam.szPHONE_NUM_MONITOR, sizeof(m_TerminalParam.szPHONE_NUM_MONITOR));
	//(0x0041)复位电话号码
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_RESET"), m_TerminalParam.szPHONE_NUM_RESET, sizeof(m_TerminalParam.szPHONE_NUM_RESET));
	//(0x0042)恢复出厂设置电话号码
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_RESTORE"), m_TerminalParam.szPHONE_NUM_RESTORE, sizeof(m_TerminalParam.szPHONE_NUM_RESTORE));
	//(0x0043)监控平台SMS电话号码
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_SVR_SMS"), m_TerminalParam.szPHONE_NUM_SVR_SMS, sizeof(m_TerminalParam.szPHONE_NUM_SVR_SMS));
	//(0x0044)接收终端SMS文本报警号码
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_TMN_SMS"), m_TerminalParam.szPHONE_NUM_TMN_SMS, sizeof(m_TerminalParam.szPHONE_NUM_TMN_SMS));
	//(0x0045)终端电话接听策略
	IniFile.GetInt(_T("TerminalParam"), _T("ANSWER_PHONE_TYPE"), m_TerminalParam.dwANSWER_PHONE_TYPE);
	//(0x0046)每次最长通话时间
	IniFile.GetInt(_T("TerminalParam"), _T("SINGLE_CALL_TIME_LIMIT"), m_TerminalParam.dwSINGLE_CALL_TIME_LIMIT);
	//(0x0047)当月最长通话时间
	IniFile.GetInt(_T("TerminalParam"), _T("MONTH_CALL_TIME_LIMIT"), m_TerminalParam.dwMONTH_CALL_TIME_LIMIT);
	//(0x0048)监听电话号码
	IniFile.GetString(_T("TerminalParam"), _T("MONITOR_PHONE_NUM"), m_TerminalParam.szMONITOR_PHONE_NUM, sizeof(m_TerminalParam.szMONITOR_PHONE_NUM));
	//(0x0049)监管平台特权短信号码
	IniFile.GetString(_T("TerminalParam"), _T("SVR_PRIVILEGE_SMS_NUM"), m_TerminalParam.szSVR_PRIVILEGE_SMS_NUM, sizeof(m_TerminalParam.szSVR_PRIVILEGE_SMS_NUM));
	//0x004A~0x004F		保留
	//(0x0050)报警屏蔽字
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_MASK_BIT"), m_TerminalParam.dwALARM_MASK_BIT);
	//(0x0051)报警发送文本SMS开关
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_SEND_SMS_SWITCH"), m_TerminalParam.dwALARM_SEND_SMS_SWITCH);
	//(0x0052)报警拍摄开关
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_SWITCH"), m_TerminalParam.dwALARM_CAPTURE_SWITCH);
	//(0x0053)报警拍摄存储标志
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_STORE_FLAG"), m_TerminalParam.dwALARM_CAPTURE_STORE_FLAG);
	//(0x0054)关键标志
	IniFile.GetInt(_T("TerminalParam"), _T("KEY_FLAG"), m_TerminalParam.dwKEY_FLAG);
	//(0x0055)最高速度
	IniFile.GetInt(_T("TerminalParam"), _T("SPEED_LIMIT"), m_TerminalParam.dwSPEED_LIMIT);
	//(0x0056)超速持续时间
	IniFile.GetInt(_T("TerminalParam"), _T("OVERSPEED_DUREATION"), m_TerminalParam.dwOVERSPEED_DUREATION);
	//(0x0057)连续驾驶时间门限
	IniFile.GetInt(_T("TerminalParam"), _T("CONTINUE_DRIVE_TIME_LIMIT"), m_TerminalParam.dwCONTINUE_DRIVE_TIME_LIMIT);
	//(0x0058)当天累计驾驶时间门限
	IniFile.GetInt(_T("TerminalParam"), _T("ONE_DAY_DRIVE_TIME_LIMIT"), m_TerminalParam.dwONE_DAY_DRIVE_TIME_LIMIT);
	//(0x0059)最小休息时间
	IniFile.GetInt(_T("TerminalParam"), _T("MINIMUM_REST_TIME"), m_TerminalParam.dwMINIMUM_REST_TIME);
	//(0x005A)最长停车时间
	IniFile.GetInt(_T("TerminalParam"), _T("MAXIMUM_PARKING_TIME"), m_TerminalParam.dwMAXIMUM_PARKING_TIME);
	//0x005B~0x006F		保留
	//(0x0070)图像/视频质量，1～10，1最好
	IniFile.GetInt(_T("TerminalParam"), _T("IMAGE_VIDEO_QUALITY"), m_TerminalParam.dwIMAGE_VIDEO_QUALITY);
	//(0x0071)亮度，0～255
	IniFile.GetInt(_T("TerminalParam"), _T("BRIGHTNESS"), m_TerminalParam.dwBRIGHTNESS);
	//(0x0072)对比度，0～127
	IniFile.GetInt(_T("TerminalParam"), _T("CONTRAST"), m_TerminalParam.dwCONTRAST);
	//(0x0073)饱和度，0～127
	IniFile.GetInt(_T("TerminalParam"), _T("SATURATION"), m_TerminalParam.dwSATURATION);
	//(0x0074)色度，0～255
	IniFile.GetInt(_T("TerminalParam"), _T("CHROMA"), m_TerminalParam.dwCHROMA);
	//0x0075~0x007F		保留
	//(0x0080)车辆里程表读数，1/10km
	IniFile.GetInt(_T("TerminalParam"), _T("VEHICLE_ODOMETER"), m_TerminalParam.dwVEHICLE_ODOMETER);
	g_alarm_state.gps_extra_info.dwMileage = m_TerminalParam.dwVEHICLE_ODOMETER*3600;
	//(0x0081)车辆所在的省域ID
	IniFile.GetInt(_T("TerminalParam"), _T("VEHICLE_PROVINCE_ID"), m_TerminalParam.wVEHICLE_PROVINCE_ID);
	//(0x0082)车辆所在的市域ID
	IniFile.GetInt(_T("TerminalParam"), _T("VEHICLE_CITY_ID"), m_TerminalParam.wVEHICLE_CITY_ID);
	//(0x0083)公安交通管理部门颁发的机动车号码
	IniFile.GetString(_T("TerminalParam"), _T("VEHICLE_ID"), m_TerminalParam.szVEHICLE_ID, sizeof(m_TerminalParam.szVEHICLE_ID));
	//(0x0084)车牌颜色
	IniFile.GetInt(_T("TerminalParam"), _T("PLATE_COLOR"), m_TerminalParam.dwPLATE_COLOR);
//	IniFile.Release();

	return TRUE;
}

/************************************************************************
* 函数名:	LoadConfigFile
* 描  述:	将内存中的保存到文件
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:	
************************************************************************/
BOOL CTerminalConfig::SaveConfigFile()
{
	if( GetFileAttributes(m_strConfigFilePath) == 0xFFFFFFFF )
		return FALSE;

	CIniFile	IniFile;
	//加载文件失败
	if( !IniFile.Load( m_strConfigFilePath ) )
		return FALSE;

	//(0x0001)终端心跳发送间隔，单位为秒(s)
	IniFile.SetInt(_T("TerminalParam"), _T("HEART_BEAT_SLICE"), m_TerminalParam.dwHEART_BEAT_SLICE);
	//(0x0002)TCP消息应答超时时间，单位为秒(s)
	IniFile.SetInt(_T("TerminalParam"), _T("TCP_ACK_OVERTIME"), m_TerminalParam.dwTCP_ACK_OVERTIME);
	//(0x0003)TCP消息重传次数
	IniFile.SetInt(_T("TerminalParam"), _T("TCP_RETRANSMIT_COUNT"), m_TerminalParam.dwTCP_RETRANSMIT_COUNT);
	//(0x0004)UDP消息应答超时时间，单位为秒(s)
	IniFile.SetInt(_T("TerminalParam"), _T("UDP_ACK_OVERTIME"), m_TerminalParam.dwUDP_ACK_OVERTIME);
	//(0x0005)UDP消息重传次数
	IniFile.SetInt(_T("TerminalParam"), _T("UDP_RETRANSMIT_COUNT"), m_TerminalParam.dwUDP_RETRANSMIT_COUNT);
	//(0x0006)SMS消息应答超时时间，单位为秒(s)
	IniFile.SetInt(_T("TerminalParam"), _T("SMS_ACK_OVERTIME"), m_TerminalParam.dwSMS_ACK_OVERTIME);
	//(0x0007)SMS消息重传次数
	IniFile.SetInt(_T("TerminalParam"), _T("SMS_RETRANSMIT_COUNT"), m_TerminalParam.dwSMS_RETRANSMIT_COUNT);
	//0x0008~0x000F		保留
	//(0x0010)主服务器APN
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_APN"), CString(m_TerminalParam.szMAIN_SVR_APN));
	//(0x0011)主服务器无线通信拨号用户名
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_NAME"), CString(m_TerminalParam.szMAIN_SVR_NAME));
	//(0x0012)主服务器无线通信拨号密码
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_PWD"), CString(m_TerminalParam.szMAIN_SVR_PWD));
	//(0x0013)主服务器地址，IP或域名
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_IP"), CString(m_TerminalParam.szMAIN_SVR_IP));
	//(0x0014)备份服务器APN
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_APN"), CString(m_TerminalParam.szBAK_SVR_APN));
	//(0x0015)备份服务器无线通信拨号用户名
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_NAME"), CString(m_TerminalParam.szBAK_SVR_NAME));
	//(0x0016)备份服务器无线通信拨号密码
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_PWD"), CString(m_TerminalParam.szBAK_SVR_PWD));
	//(0x0017)备份服务器地址，IP或域名
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_IP"), CString(m_TerminalParam.szBAK_SVR_IP));
	//(0x0018)服务器TCP端口
	IniFile.SetInt(_T("TerminalParam"), _T("SVR_TCP_PORT"), m_TerminalParam.dwSVR_TCP_PORT);
	//(0x0019)服务器UDP端口
	IniFile.SetInt(_T("TerminalParam"), _T("SVR_UDP_PORT"), m_TerminalParam.dwSVR_UDP_PORT);
	//0x001A~0x001F		保留
	//(0x0020)位置汇报策略
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_TYPE"), m_TerminalParam.dwPOS_REPORT_TYPE);
	//(0x0021)位置汇报方案
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_CONDITION"), m_TerminalParam.dwPOS_REPORT_CONDITION);
	//(0x0022)驾驶员未登录汇报时间间隔，单位为秒(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_LOGOUT"), m_TerminalParam.dwPOS_REPORT_SLICE_LOGOUT);
	//0x0023~0x0026		保留
	//(0x0027)休眠时汇报时间间隔，单位为秒(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_SLEEP"), m_TerminalParam.dwPOS_REPORT_SLICE_SLEEP);
	//(0x0028)紧急报警时汇报时间间隔，单位为秒(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_ALARM"), m_TerminalParam.dwPOS_REPORT_SLICE_ALARM);
	//(0x0029)缺省时间汇报间隔，单位为秒(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_DEFAULT"), m_TerminalParam.dwPOS_REPORT_SLICE_DEFAULT);
	//0x002A~0x002B		保留
	//(0x002C)缺省距离汇报间隔，单位为米(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_DEFAULT"), m_TerminalParam.dwPOS_REPORT_DIST_DEFAULT);
	//(0x002D)驾驶员未登录汇报距离间隔，单位为米(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_LOGOUT"), m_TerminalParam.dwPOS_REPORT_DIST_LOGOUT);
	//(0x002E)休眠时汇报距离间隔，单位为米(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_SLEEP"), m_TerminalParam.dwPOS_REPORT_DIST_SLEEP);
	//(0x002F)紧急报警时汇报距离间隔，单位为米(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_ALARM"), m_TerminalParam.dwPOS_REPORT_DIST_ALARM);
	//(0x0030)拐点补传角度，<180
	IniFile.SetInt(_T("TerminalParam"), _T("CORNER_RETRANSMIT_ANGLE"), m_TerminalParam.dwCORNER_RETRANSMIT_ANGLE);
	//(0x0031)电子围栏半径（非法位移阈值），单位为米  Radius of the electronic fence
	IniFile.SetInt(_T("TerminalParam"), _T("ELECTRONIC_FENCE_RADIUS"), m_TerminalParam.wELECTRONIC_FENCE_RADIUS);
	//0x0032~0x003F		保留
	//(0x0040)监控平台电话号码
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_MONITOR"), CString(m_TerminalParam.szPHONE_NUM_MONITOR));
	//(0x0041)复位电话号码
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_RESET"), CString(m_TerminalParam.szPHONE_NUM_RESET));
	//(0x0042)恢复出厂设置电话号码
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_RESTORE"), CString(m_TerminalParam.szPHONE_NUM_RESTORE));
	//(0x0043)监控平台SMS电话号码
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_SVR_SMS"), CString(m_TerminalParam.szPHONE_NUM_SVR_SMS));
	//(0x0044)接收终端SMS文本报警号码
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_TMN_SMS"), CString(m_TerminalParam.szPHONE_NUM_TMN_SMS));
	//(0x0045)终端电话接听策略
	IniFile.SetInt(_T("TerminalParam"), _T("ANSWER_PHONE_TYPE"), m_TerminalParam.dwANSWER_PHONE_TYPE);
	//(0x0046)每次最长通话时间
	IniFile.SetInt(_T("TerminalParam"), _T("SINGLE_CALL_TIME_LIMIT"), m_TerminalParam.dwSINGLE_CALL_TIME_LIMIT);
	//(0x0047)当月最长通话时间
	IniFile.SetInt(_T("TerminalParam"), _T("MONTH_CALL_TIME_LIMIT"), m_TerminalParam.dwMONTH_CALL_TIME_LIMIT);
	//(0x0048)监听电话号码
	IniFile.SetString(_T("TerminalParam"), _T("MONITOR_PHONE_NUM"), CString(m_TerminalParam.szMONITOR_PHONE_NUM));
	//(0x0049)监管平台特权短信号码
	IniFile.SetString(_T("TerminalParam"), _T("SVR_PRIVILEGE_SMS_NUM"), CString(m_TerminalParam.szSVR_PRIVILEGE_SMS_NUM));
	//0x004A~0x004F		保留
	//(0x0050)报警屏蔽字
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_MASK_BIT"), m_TerminalParam.dwALARM_MASK_BIT);
	//(0x0051)报警发送文本SMS开关
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_SEND_SMS_SWITCH"), m_TerminalParam.dwALARM_SEND_SMS_SWITCH);
	//(0x0052)报警拍摄开关
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_SWITCH"), m_TerminalParam.dwALARM_CAPTURE_SWITCH);
	//(0x0053)报警拍摄存储标志
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_STORE_FLAG"), m_TerminalParam.dwALARM_CAPTURE_STORE_FLAG);
	//(0x0054)关键标志
	IniFile.SetInt(_T("TerminalParam"), _T("KEY_FLAG"), m_TerminalParam.dwKEY_FLAG);
	//(0x0055)最高速度
	IniFile.SetInt(_T("TerminalParam"), _T("SPEED_LIMIT"), m_TerminalParam.dwSPEED_LIMIT);
	//(0x0056)超速持续时间
	IniFile.SetInt(_T("TerminalParam"), _T("OVERSPEED_DUREATION"), m_TerminalParam.dwOVERSPEED_DUREATION);
	//(0x0057)连续驾驶时间门限
	IniFile.SetInt(_T("TerminalParam"), _T("CONTINUE_DRIVE_TIME_LIMIT"), m_TerminalParam.dwCONTINUE_DRIVE_TIME_LIMIT);
	//(0x0058)当天累计驾驶时间门限
	IniFile.SetInt(_T("TerminalParam"), _T("ONE_DAY_DRIVE_TIME_LIMIT"), m_TerminalParam.dwONE_DAY_DRIVE_TIME_LIMIT);
	//(0x0059)最小休息时间
	IniFile.SetInt(_T("TerminalParam"), _T("MINIMUM_REST_TIME"), m_TerminalParam.dwMINIMUM_REST_TIME);
	//(0x005A)最长停车时间
	IniFile.SetInt(_T("TerminalParam"), _T("MAXIMUM_PARKING_TIME"), m_TerminalParam.dwMAXIMUM_PARKING_TIME);
	//0x005B~0x006F		保留
	//(0x0070)图像/视频质量，1～10，1最好
	IniFile.SetInt(_T("TerminalParam"), _T("IMAGE_VIDEO_QUALITY"), m_TerminalParam.dwIMAGE_VIDEO_QUALITY);
	//(0x0071)亮度，0～255
	IniFile.SetInt(_T("TerminalParam"), _T("BRIGHTNESS"), m_TerminalParam.dwBRIGHTNESS);
	//(0x0072)对比度，0～127
	IniFile.SetInt(_T("TerminalParam"), _T("CONTRAST"), m_TerminalParam.dwCONTRAST);
	//(0x0073)饱和度，0～127
	IniFile.SetInt(_T("TerminalParam"), _T("SATURATION"), m_TerminalParam.dwSATURATION);
	//(0x0074)色度，0～255
	IniFile.SetInt(_T("TerminalParam"), _T("CHROMA"), m_TerminalParam.dwCHROMA);
	//0x0075~0x007F		保留
	//(0x0080)车辆里程表读数，1/10km
	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_ODOMETER"), m_TerminalParam.dwVEHICLE_ODOMETER);
	//(0x0081)车辆所在的省域ID
	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_PROVINCE_ID"), m_TerminalParam.wVEHICLE_PROVINCE_ID);
	//(0x0082)车辆所在的市域ID
	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_CITY_ID"), m_TerminalParam.wVEHICLE_CITY_ID);
	//(0x0083)公安交通管理部门颁发的机动车号码
	IniFile.SetString(_T("TerminalParam"), _T("VEHICLE_ID"), CString(m_TerminalParam.szVEHICLE_ID));
	//(0x0084)车牌颜色
	IniFile.SetInt(_T("TerminalParam"), _T("PLATE_COLOR"), m_TerminalParam.dwPLATE_COLOR);

	IniFile.Flush();
	return TRUE;
}

BOOL CTerminalConfig::SaveMileage(DWORD dwMileage)
{
	if( GetFileAttributes(PATH_CONFIG_INI) == 0xFFFFFFFF )
		return FALSE;

	CIniFile	IniFile;
	//加载文件失败
	if( !IniFile.Load( PATH_CONFIG_INI ) )
		return FALSE;

	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_ODOMETER"), dwMileage);

	IniFile.Flush();
//	IniFile.Release();
	return TRUE;


}

/************************************************************************
* 函数名:	GetTerminalParam
* 描  述:	获取终端参数结构体
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:	
************************************************************************/
const PTERMINAL_PARAM CTerminalConfig::GetTerminalParam()
{
	return &m_TerminalParam;
}