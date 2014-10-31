#include "StdAfx.h"
#include "TerminalConfig.h"
#include "../GPSData/GPSData.h"

//��ʼ����̬��Ա����
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
* ������:	Instance
* ��  ��: 
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע:	singleton
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
* ������:	LoadConfigFile
* ��  ��:	����config�ļ����ڴ���
* ��  ��:	strPath: config�ļ�·��
* ��  ��:	
* ��  ��:	
* ��  ע:	
************************************************************************/
BOOL CTerminalConfig::LoadConfigFile(CString strPath)
{
	CIniFile	IniFile;
	memset(&m_TerminalParam, NULL, sizeof(m_TerminalParam));
	//�����ļ�ʧ��
	if( !IniFile.Load( strPath ) )
		return FALSE;

	//�����ļ�·��
	m_strConfigFilePath	= strPath;

	//(0x0001)�ն��������ͼ������λΪ��(s)
	IniFile.GetInt(_T("TerminalParam"), _T("HEART_BEAT_SLICE"), m_TerminalParam.dwHEART_BEAT_SLICE);
	//(0x0002)TCP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	IniFile.GetInt(_T("TerminalParam"), _T("TCP_ACK_OVERTIME"), m_TerminalParam.dwTCP_ACK_OVERTIME);
	//(0x0003)TCP��Ϣ�ش�����
	IniFile.GetInt(_T("TerminalParam"), _T("TCP_RETRANSMIT_COUNT"), m_TerminalParam.dwTCP_RETRANSMIT_COUNT);
	//(0x0004)UDP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	IniFile.GetInt(_T("TerminalParam"), _T("UDP_ACK_OVERTIME"), m_TerminalParam.dwUDP_ACK_OVERTIME);
	//(0x0005)UDP��Ϣ�ش�����
	IniFile.GetInt(_T("TerminalParam"), _T("UDP_RETRANSMIT_COUNT"), m_TerminalParam.dwUDP_RETRANSMIT_COUNT);
	//(0x0006)SMS��ϢӦ��ʱʱ�䣬��λΪ��(s)
	IniFile.GetInt(_T("TerminalParam"), _T("SMS_ACK_OVERTIME"), m_TerminalParam.dwSMS_ACK_OVERTIME);
	//(0x0007)SMS��Ϣ�ش�����
	IniFile.GetInt(_T("TerminalParam"), _T("SMS_RETRANSMIT_COUNT"), m_TerminalParam.dwSMS_RETRANSMIT_COUNT);
	//0x0008~0x000F		����
	//(0x0010)��������APN
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_APN"), m_TerminalParam.szMAIN_SVR_APN, sizeof(m_TerminalParam.szMAIN_SVR_APN));
	//(0x0011)������������ͨ�Ų����û���
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_NAME"), m_TerminalParam.szMAIN_SVR_NAME, sizeof(m_TerminalParam.szMAIN_SVR_NAME));
	//(0x0012)������������ͨ�Ų�������
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_PWD"), m_TerminalParam.szMAIN_SVR_PWD, sizeof(m_TerminalParam.szMAIN_SVR_PWD));
	//(0x0013)����������ַ��IP������
	IniFile.GetString(_T("TerminalParam"), _T("MAIN_SVR_IP"), m_TerminalParam.szMAIN_SVR_IP, sizeof(m_TerminalParam.szMAIN_SVR_IP));
	//(0x0014)���ݷ�����APN
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_APN"), m_TerminalParam.szBAK_SVR_APN, sizeof(m_TerminalParam.szBAK_SVR_APN));
	//(0x0015)���ݷ���������ͨ�Ų����û���
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_NAME"), m_TerminalParam.szBAK_SVR_NAME, sizeof(m_TerminalParam.szBAK_SVR_NAME));
	//(0x0016)���ݷ���������ͨ�Ų�������
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_PWD"), m_TerminalParam.szBAK_SVR_PWD, sizeof(m_TerminalParam.szBAK_SVR_PWD));
	//(0x0017)���ݷ�������ַ��IP������
	IniFile.GetString(_T("TerminalParam"), _T("BAK_SVR_IP"), m_TerminalParam.szBAK_SVR_IP, sizeof(m_TerminalParam.szBAK_SVR_IP));
	//(0x0018)������TCP�˿�
	IniFile.GetInt(_T("TerminalParam"), _T("SVR_TCP_PORT"), m_TerminalParam.dwSVR_TCP_PORT);
	//(0x0019)������UDP�˿�
	IniFile.GetInt(_T("TerminalParam"), _T("SVR_UDP_PORT"), m_TerminalParam.dwSVR_UDP_PORT);
	//0x001A~0x001F		����
	//(0x0020)λ�û㱨����
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_TYPE"), m_TerminalParam.dwPOS_REPORT_TYPE);
	//(0x0021)λ�û㱨����
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_CONDITION"), m_TerminalParam.dwPOS_REPORT_CONDITION);
	//(0x0022)��ʻԱδ��¼�㱨ʱ��������λΪ��(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_LOGOUT"), m_TerminalParam.dwPOS_REPORT_SLICE_LOGOUT);
	//0x0023~0x0026		����
	//(0x0027)����ʱ�㱨ʱ��������λΪ��(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_SLEEP"), m_TerminalParam.dwPOS_REPORT_SLICE_SLEEP);
	//(0x0028)��������ʱ�㱨ʱ��������λΪ��(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_ALARM"), m_TerminalParam.dwPOS_REPORT_SLICE_ALARM);
	//(0x0029)ȱʡʱ��㱨�������λΪ��(s), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_DEFAULT"), m_TerminalParam.dwPOS_REPORT_SLICE_DEFAULT);
	//0x002A~0x002B		����
	//(0x002C)ȱʡ����㱨�������λΪ��(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_DEFAULT"), m_TerminalParam.dwPOS_REPORT_DIST_DEFAULT);
	//(0x002D)��ʻԱδ��¼�㱨����������λΪ��(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_LOGOUT"), m_TerminalParam.dwPOS_REPORT_DIST_LOGOUT);
	//(0x002E)����ʱ�㱨����������λΪ��(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_SLEEP"), m_TerminalParam.dwPOS_REPORT_DIST_SLEEP);
	//(0x002F)��������ʱ�㱨����������λΪ��(m), >0
	IniFile.GetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_ALARM"), m_TerminalParam.dwPOS_REPORT_DIST_ALARM);
	//(0x0030)�յ㲹���Ƕȣ�<180
	IniFile.GetInt(_T("TerminalParam"), _T("CORNER_RETRANSMIT_ANGLE"), m_TerminalParam.dwCORNER_RETRANSMIT_ANGLE);
	//(0x0031)����Χ���뾶���Ƿ�λ����ֵ������λΪ��  Radius of the electronic fence
	IniFile.GetInt(_T("TerminalParam"), _T("ELECTRONIC_FENCE_RADIUS"), m_TerminalParam.wELECTRONIC_FENCE_RADIUS);
	//0x0032~0x003F		����
	//(0x0040)���ƽ̨�绰����
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_MONITOR"), m_TerminalParam.szPHONE_NUM_MONITOR, sizeof(m_TerminalParam.szPHONE_NUM_MONITOR));
	//(0x0041)��λ�绰����
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_RESET"), m_TerminalParam.szPHONE_NUM_RESET, sizeof(m_TerminalParam.szPHONE_NUM_RESET));
	//(0x0042)�ָ��������õ绰����
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_RESTORE"), m_TerminalParam.szPHONE_NUM_RESTORE, sizeof(m_TerminalParam.szPHONE_NUM_RESTORE));
	//(0x0043)���ƽ̨SMS�绰����
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_SVR_SMS"), m_TerminalParam.szPHONE_NUM_SVR_SMS, sizeof(m_TerminalParam.szPHONE_NUM_SVR_SMS));
	//(0x0044)�����ն�SMS�ı���������
	IniFile.GetString(_T("TerminalParam"), _T("PHONE_NUM_TMN_SMS"), m_TerminalParam.szPHONE_NUM_TMN_SMS, sizeof(m_TerminalParam.szPHONE_NUM_TMN_SMS));
	//(0x0045)�ն˵绰��������
	IniFile.GetInt(_T("TerminalParam"), _T("ANSWER_PHONE_TYPE"), m_TerminalParam.dwANSWER_PHONE_TYPE);
	//(0x0046)ÿ���ͨ��ʱ��
	IniFile.GetInt(_T("TerminalParam"), _T("SINGLE_CALL_TIME_LIMIT"), m_TerminalParam.dwSINGLE_CALL_TIME_LIMIT);
	//(0x0047)�����ͨ��ʱ��
	IniFile.GetInt(_T("TerminalParam"), _T("MONTH_CALL_TIME_LIMIT"), m_TerminalParam.dwMONTH_CALL_TIME_LIMIT);
	//(0x0048)�����绰����
	IniFile.GetString(_T("TerminalParam"), _T("MONITOR_PHONE_NUM"), m_TerminalParam.szMONITOR_PHONE_NUM, sizeof(m_TerminalParam.szMONITOR_PHONE_NUM));
	//(0x0049)���ƽ̨��Ȩ���ź���
	IniFile.GetString(_T("TerminalParam"), _T("SVR_PRIVILEGE_SMS_NUM"), m_TerminalParam.szSVR_PRIVILEGE_SMS_NUM, sizeof(m_TerminalParam.szSVR_PRIVILEGE_SMS_NUM));
	//0x004A~0x004F		����
	//(0x0050)����������
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_MASK_BIT"), m_TerminalParam.dwALARM_MASK_BIT);
	//(0x0051)���������ı�SMS����
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_SEND_SMS_SWITCH"), m_TerminalParam.dwALARM_SEND_SMS_SWITCH);
	//(0x0052)�������㿪��
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_SWITCH"), m_TerminalParam.dwALARM_CAPTURE_SWITCH);
	//(0x0053)��������洢��־
	IniFile.GetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_STORE_FLAG"), m_TerminalParam.dwALARM_CAPTURE_STORE_FLAG);
	//(0x0054)�ؼ���־
	IniFile.GetInt(_T("TerminalParam"), _T("KEY_FLAG"), m_TerminalParam.dwKEY_FLAG);
	//(0x0055)����ٶ�
	IniFile.GetInt(_T("TerminalParam"), _T("SPEED_LIMIT"), m_TerminalParam.dwSPEED_LIMIT);
	//(0x0056)���ٳ���ʱ��
	IniFile.GetInt(_T("TerminalParam"), _T("OVERSPEED_DUREATION"), m_TerminalParam.dwOVERSPEED_DUREATION);
	//(0x0057)������ʻʱ������
	IniFile.GetInt(_T("TerminalParam"), _T("CONTINUE_DRIVE_TIME_LIMIT"), m_TerminalParam.dwCONTINUE_DRIVE_TIME_LIMIT);
	//(0x0058)�����ۼƼ�ʻʱ������
	IniFile.GetInt(_T("TerminalParam"), _T("ONE_DAY_DRIVE_TIME_LIMIT"), m_TerminalParam.dwONE_DAY_DRIVE_TIME_LIMIT);
	//(0x0059)��С��Ϣʱ��
	IniFile.GetInt(_T("TerminalParam"), _T("MINIMUM_REST_TIME"), m_TerminalParam.dwMINIMUM_REST_TIME);
	//(0x005A)�ͣ��ʱ��
	IniFile.GetInt(_T("TerminalParam"), _T("MAXIMUM_PARKING_TIME"), m_TerminalParam.dwMAXIMUM_PARKING_TIME);
	//0x005B~0x006F		����
	//(0x0070)ͼ��/��Ƶ������1��10��1���
	IniFile.GetInt(_T("TerminalParam"), _T("IMAGE_VIDEO_QUALITY"), m_TerminalParam.dwIMAGE_VIDEO_QUALITY);
	//(0x0071)���ȣ�0��255
	IniFile.GetInt(_T("TerminalParam"), _T("BRIGHTNESS"), m_TerminalParam.dwBRIGHTNESS);
	//(0x0072)�Աȶȣ�0��127
	IniFile.GetInt(_T("TerminalParam"), _T("CONTRAST"), m_TerminalParam.dwCONTRAST);
	//(0x0073)���Ͷȣ�0��127
	IniFile.GetInt(_T("TerminalParam"), _T("SATURATION"), m_TerminalParam.dwSATURATION);
	//(0x0074)ɫ�ȣ�0��255
	IniFile.GetInt(_T("TerminalParam"), _T("CHROMA"), m_TerminalParam.dwCHROMA);
	//0x0075~0x007F		����
	//(0x0080)������̱������1/10km
	IniFile.GetInt(_T("TerminalParam"), _T("VEHICLE_ODOMETER"), m_TerminalParam.dwVEHICLE_ODOMETER);
	g_alarm_state.gps_extra_info.dwMileage = m_TerminalParam.dwVEHICLE_ODOMETER*3600;
	//(0x0081)�������ڵ�ʡ��ID
	IniFile.GetInt(_T("TerminalParam"), _T("VEHICLE_PROVINCE_ID"), m_TerminalParam.wVEHICLE_PROVINCE_ID);
	//(0x0082)�������ڵ�����ID
	IniFile.GetInt(_T("TerminalParam"), _T("VEHICLE_CITY_ID"), m_TerminalParam.wVEHICLE_CITY_ID);
	//(0x0083)������ͨ�����Ű䷢�Ļ���������
	IniFile.GetString(_T("TerminalParam"), _T("VEHICLE_ID"), m_TerminalParam.szVEHICLE_ID, sizeof(m_TerminalParam.szVEHICLE_ID));
	//(0x0084)������ɫ
	IniFile.GetInt(_T("TerminalParam"), _T("PLATE_COLOR"), m_TerminalParam.dwPLATE_COLOR);
//	IniFile.Release();

	return TRUE;
}

/************************************************************************
* ������:	LoadConfigFile
* ��  ��:	���ڴ��еı��浽�ļ�
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:	
************************************************************************/
BOOL CTerminalConfig::SaveConfigFile()
{
	if( GetFileAttributes(m_strConfigFilePath) == 0xFFFFFFFF )
		return FALSE;

	CIniFile	IniFile;
	//�����ļ�ʧ��
	if( !IniFile.Load( m_strConfigFilePath ) )
		return FALSE;

	//(0x0001)�ն��������ͼ������λΪ��(s)
	IniFile.SetInt(_T("TerminalParam"), _T("HEART_BEAT_SLICE"), m_TerminalParam.dwHEART_BEAT_SLICE);
	//(0x0002)TCP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	IniFile.SetInt(_T("TerminalParam"), _T("TCP_ACK_OVERTIME"), m_TerminalParam.dwTCP_ACK_OVERTIME);
	//(0x0003)TCP��Ϣ�ش�����
	IniFile.SetInt(_T("TerminalParam"), _T("TCP_RETRANSMIT_COUNT"), m_TerminalParam.dwTCP_RETRANSMIT_COUNT);
	//(0x0004)UDP��ϢӦ��ʱʱ�䣬��λΪ��(s)
	IniFile.SetInt(_T("TerminalParam"), _T("UDP_ACK_OVERTIME"), m_TerminalParam.dwUDP_ACK_OVERTIME);
	//(0x0005)UDP��Ϣ�ش�����
	IniFile.SetInt(_T("TerminalParam"), _T("UDP_RETRANSMIT_COUNT"), m_TerminalParam.dwUDP_RETRANSMIT_COUNT);
	//(0x0006)SMS��ϢӦ��ʱʱ�䣬��λΪ��(s)
	IniFile.SetInt(_T("TerminalParam"), _T("SMS_ACK_OVERTIME"), m_TerminalParam.dwSMS_ACK_OVERTIME);
	//(0x0007)SMS��Ϣ�ش�����
	IniFile.SetInt(_T("TerminalParam"), _T("SMS_RETRANSMIT_COUNT"), m_TerminalParam.dwSMS_RETRANSMIT_COUNT);
	//0x0008~0x000F		����
	//(0x0010)��������APN
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_APN"), CString(m_TerminalParam.szMAIN_SVR_APN));
	//(0x0011)������������ͨ�Ų����û���
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_NAME"), CString(m_TerminalParam.szMAIN_SVR_NAME));
	//(0x0012)������������ͨ�Ų�������
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_PWD"), CString(m_TerminalParam.szMAIN_SVR_PWD));
	//(0x0013)����������ַ��IP������
	IniFile.SetString(_T("TerminalParam"), _T("MAIN_SVR_IP"), CString(m_TerminalParam.szMAIN_SVR_IP));
	//(0x0014)���ݷ�����APN
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_APN"), CString(m_TerminalParam.szBAK_SVR_APN));
	//(0x0015)���ݷ���������ͨ�Ų����û���
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_NAME"), CString(m_TerminalParam.szBAK_SVR_NAME));
	//(0x0016)���ݷ���������ͨ�Ų�������
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_PWD"), CString(m_TerminalParam.szBAK_SVR_PWD));
	//(0x0017)���ݷ�������ַ��IP������
	IniFile.SetString(_T("TerminalParam"), _T("BAK_SVR_IP"), CString(m_TerminalParam.szBAK_SVR_IP));
	//(0x0018)������TCP�˿�
	IniFile.SetInt(_T("TerminalParam"), _T("SVR_TCP_PORT"), m_TerminalParam.dwSVR_TCP_PORT);
	//(0x0019)������UDP�˿�
	IniFile.SetInt(_T("TerminalParam"), _T("SVR_UDP_PORT"), m_TerminalParam.dwSVR_UDP_PORT);
	//0x001A~0x001F		����
	//(0x0020)λ�û㱨����
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_TYPE"), m_TerminalParam.dwPOS_REPORT_TYPE);
	//(0x0021)λ�û㱨����
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_CONDITION"), m_TerminalParam.dwPOS_REPORT_CONDITION);
	//(0x0022)��ʻԱδ��¼�㱨ʱ��������λΪ��(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_LOGOUT"), m_TerminalParam.dwPOS_REPORT_SLICE_LOGOUT);
	//0x0023~0x0026		����
	//(0x0027)����ʱ�㱨ʱ��������λΪ��(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_SLEEP"), m_TerminalParam.dwPOS_REPORT_SLICE_SLEEP);
	//(0x0028)��������ʱ�㱨ʱ��������λΪ��(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_ALARM"), m_TerminalParam.dwPOS_REPORT_SLICE_ALARM);
	//(0x0029)ȱʡʱ��㱨�������λΪ��(s), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_SLICE_DEFAULT"), m_TerminalParam.dwPOS_REPORT_SLICE_DEFAULT);
	//0x002A~0x002B		����
	//(0x002C)ȱʡ����㱨�������λΪ��(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_DEFAULT"), m_TerminalParam.dwPOS_REPORT_DIST_DEFAULT);
	//(0x002D)��ʻԱδ��¼�㱨����������λΪ��(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_LOGOUT"), m_TerminalParam.dwPOS_REPORT_DIST_LOGOUT);
	//(0x002E)����ʱ�㱨����������λΪ��(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_SLEEP"), m_TerminalParam.dwPOS_REPORT_DIST_SLEEP);
	//(0x002F)��������ʱ�㱨����������λΪ��(m), >0
	IniFile.SetInt(_T("TerminalParam"), _T("POS_REPORT_DIST_ALARM"), m_TerminalParam.dwPOS_REPORT_DIST_ALARM);
	//(0x0030)�յ㲹���Ƕȣ�<180
	IniFile.SetInt(_T("TerminalParam"), _T("CORNER_RETRANSMIT_ANGLE"), m_TerminalParam.dwCORNER_RETRANSMIT_ANGLE);
	//(0x0031)����Χ���뾶���Ƿ�λ����ֵ������λΪ��  Radius of the electronic fence
	IniFile.SetInt(_T("TerminalParam"), _T("ELECTRONIC_FENCE_RADIUS"), m_TerminalParam.wELECTRONIC_FENCE_RADIUS);
	//0x0032~0x003F		����
	//(0x0040)���ƽ̨�绰����
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_MONITOR"), CString(m_TerminalParam.szPHONE_NUM_MONITOR));
	//(0x0041)��λ�绰����
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_RESET"), CString(m_TerminalParam.szPHONE_NUM_RESET));
	//(0x0042)�ָ��������õ绰����
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_RESTORE"), CString(m_TerminalParam.szPHONE_NUM_RESTORE));
	//(0x0043)���ƽ̨SMS�绰����
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_SVR_SMS"), CString(m_TerminalParam.szPHONE_NUM_SVR_SMS));
	//(0x0044)�����ն�SMS�ı���������
	IniFile.SetString(_T("TerminalParam"), _T("PHONE_NUM_TMN_SMS"), CString(m_TerminalParam.szPHONE_NUM_TMN_SMS));
	//(0x0045)�ն˵绰��������
	IniFile.SetInt(_T("TerminalParam"), _T("ANSWER_PHONE_TYPE"), m_TerminalParam.dwANSWER_PHONE_TYPE);
	//(0x0046)ÿ���ͨ��ʱ��
	IniFile.SetInt(_T("TerminalParam"), _T("SINGLE_CALL_TIME_LIMIT"), m_TerminalParam.dwSINGLE_CALL_TIME_LIMIT);
	//(0x0047)�����ͨ��ʱ��
	IniFile.SetInt(_T("TerminalParam"), _T("MONTH_CALL_TIME_LIMIT"), m_TerminalParam.dwMONTH_CALL_TIME_LIMIT);
	//(0x0048)�����绰����
	IniFile.SetString(_T("TerminalParam"), _T("MONITOR_PHONE_NUM"), CString(m_TerminalParam.szMONITOR_PHONE_NUM));
	//(0x0049)���ƽ̨��Ȩ���ź���
	IniFile.SetString(_T("TerminalParam"), _T("SVR_PRIVILEGE_SMS_NUM"), CString(m_TerminalParam.szSVR_PRIVILEGE_SMS_NUM));
	//0x004A~0x004F		����
	//(0x0050)����������
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_MASK_BIT"), m_TerminalParam.dwALARM_MASK_BIT);
	//(0x0051)���������ı�SMS����
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_SEND_SMS_SWITCH"), m_TerminalParam.dwALARM_SEND_SMS_SWITCH);
	//(0x0052)�������㿪��
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_SWITCH"), m_TerminalParam.dwALARM_CAPTURE_SWITCH);
	//(0x0053)��������洢��־
	IniFile.SetInt(_T("TerminalParam"), _T("ALARM_CAPTURE_STORE_FLAG"), m_TerminalParam.dwALARM_CAPTURE_STORE_FLAG);
	//(0x0054)�ؼ���־
	IniFile.SetInt(_T("TerminalParam"), _T("KEY_FLAG"), m_TerminalParam.dwKEY_FLAG);
	//(0x0055)����ٶ�
	IniFile.SetInt(_T("TerminalParam"), _T("SPEED_LIMIT"), m_TerminalParam.dwSPEED_LIMIT);
	//(0x0056)���ٳ���ʱ��
	IniFile.SetInt(_T("TerminalParam"), _T("OVERSPEED_DUREATION"), m_TerminalParam.dwOVERSPEED_DUREATION);
	//(0x0057)������ʻʱ������
	IniFile.SetInt(_T("TerminalParam"), _T("CONTINUE_DRIVE_TIME_LIMIT"), m_TerminalParam.dwCONTINUE_DRIVE_TIME_LIMIT);
	//(0x0058)�����ۼƼ�ʻʱ������
	IniFile.SetInt(_T("TerminalParam"), _T("ONE_DAY_DRIVE_TIME_LIMIT"), m_TerminalParam.dwONE_DAY_DRIVE_TIME_LIMIT);
	//(0x0059)��С��Ϣʱ��
	IniFile.SetInt(_T("TerminalParam"), _T("MINIMUM_REST_TIME"), m_TerminalParam.dwMINIMUM_REST_TIME);
	//(0x005A)�ͣ��ʱ��
	IniFile.SetInt(_T("TerminalParam"), _T("MAXIMUM_PARKING_TIME"), m_TerminalParam.dwMAXIMUM_PARKING_TIME);
	//0x005B~0x006F		����
	//(0x0070)ͼ��/��Ƶ������1��10��1���
	IniFile.SetInt(_T("TerminalParam"), _T("IMAGE_VIDEO_QUALITY"), m_TerminalParam.dwIMAGE_VIDEO_QUALITY);
	//(0x0071)���ȣ�0��255
	IniFile.SetInt(_T("TerminalParam"), _T("BRIGHTNESS"), m_TerminalParam.dwBRIGHTNESS);
	//(0x0072)�Աȶȣ�0��127
	IniFile.SetInt(_T("TerminalParam"), _T("CONTRAST"), m_TerminalParam.dwCONTRAST);
	//(0x0073)���Ͷȣ�0��127
	IniFile.SetInt(_T("TerminalParam"), _T("SATURATION"), m_TerminalParam.dwSATURATION);
	//(0x0074)ɫ�ȣ�0��255
	IniFile.SetInt(_T("TerminalParam"), _T("CHROMA"), m_TerminalParam.dwCHROMA);
	//0x0075~0x007F		����
	//(0x0080)������̱������1/10km
	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_ODOMETER"), m_TerminalParam.dwVEHICLE_ODOMETER);
	//(0x0081)�������ڵ�ʡ��ID
	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_PROVINCE_ID"), m_TerminalParam.wVEHICLE_PROVINCE_ID);
	//(0x0082)�������ڵ�����ID
	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_CITY_ID"), m_TerminalParam.wVEHICLE_CITY_ID);
	//(0x0083)������ͨ�����Ű䷢�Ļ���������
	IniFile.SetString(_T("TerminalParam"), _T("VEHICLE_ID"), CString(m_TerminalParam.szVEHICLE_ID));
	//(0x0084)������ɫ
	IniFile.SetInt(_T("TerminalParam"), _T("PLATE_COLOR"), m_TerminalParam.dwPLATE_COLOR);

	IniFile.Flush();
	return TRUE;
}

BOOL CTerminalConfig::SaveMileage(DWORD dwMileage)
{
	if( GetFileAttributes(PATH_CONFIG_INI) == 0xFFFFFFFF )
		return FALSE;

	CIniFile	IniFile;
	//�����ļ�ʧ��
	if( !IniFile.Load( PATH_CONFIG_INI ) )
		return FALSE;

	IniFile.SetInt(_T("TerminalParam"), _T("VEHICLE_ODOMETER"), dwMileage);

	IniFile.Flush();
//	IniFile.Release();
	return TRUE;


}

/************************************************************************
* ������:	GetTerminalParam
* ��  ��:	��ȡ�ն˲����ṹ��
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:	
************************************************************************/
const PTERMINAL_PARAM CTerminalConfig::GetTerminalParam()
{
	return &m_TerminalParam;
}