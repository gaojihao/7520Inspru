#include "StdAfx.h"
#include "GPSData.h"
#include "../Tools/LogTool.h"
#include "../TerminalConfig/TerminalConfig.h"
#include "../DataParser/DataPack.h"
#include "../DataParser/DataParser.h"
#include "../MsgProc/MsgProc.h"

extern void TTSSpeaking(CString csText);

//��ʼ����̬��Ա����
CGPSData*	CGPSData::m_pInstance	= NULL;
GPS_ALARM_STATE     g_alarm_state;		//������ʶ״̬λ�͸�����Ϣ
Illegal_DisplaceMent_ALARM g_illegalDisMent_alarm; //�Ƿ�λ�Ʊ���
LIST_CIRC			m_listCircRegion;	//Բ����������

bool g_bAdjust = false;

CGPSData::CGPSData(void)
{
	m_bSwitch	= FALSE;
	m_bfixed      = FALSE;

	g_alarm_state.unState.gps_state.cACCState = 1;
}

CGPSData::~CGPSData(void)
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
CGPSData* CGPSData::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CGPSData;
	}
	return m_pInstance;
}


/************************************************************************
* ������:	SetReportPos
* ��  ��:	����λ����Ϣ�ϱ�����
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
void CGPSData::SetReportPos( BOOL bSwitch )
{
	m_bSwitch = bSwitch;
}


void CGPSData::GPSReportAndAdjusttime()
{
	//λ����Ϣ�㱨
	ReportPos();

	int iStopTime = 0; 
	static DWORD dwStart = 0;

	dwStart++;

	if ( (0 == g_pGpsPosInfo->rmc_data.pos_state.cFix) && dwStart >180)//�ж�ΪGPS���߹���
	{
		g_alarm_state.unAlarm.alarm_flag.cGNSSAntennaDetach = 1;
	}

	if (1 == g_pGpsPosInfo->rmc_data.pos_state.cFix)
	{
		g_alarm_state.unAlarm.alarm_flag.cGNSSAntennaDetach = 0;
	}


	m_bfixed = g_pGpsPosInfo->rmc_data.pos_state.cFix;

	if (m_bfixed && (g_pGpsPosInfo->rmc_data.dbSpeed < 2) && (CDataParser::Instance()->m_iMaxStopCar !=0))
	{
		iStopTime++;

		if (iStopTime > CDataParser::Instance()->m_iMaxStopCar)
		{
			g_alarm_state.unAlarm.alarm_flag.cParkingOverTime = 1;
		}
		else
		{
			g_alarm_state.unAlarm.alarm_flag.cParkingOverTime = 0;
		}
	}
	else if(m_bfixed && g_pGpsPosInfo->rmc_data.dbSpeed >= 2)
	{

		g_alarm_state.gps_extra_info.dwMileage += g_pGpsPosInfo->rmc_data.dbSpeed * 10;  //�ۼ����


		g_alarm_state.unAlarm.alarm_flag.cParkingOverTime = 0;

		
		iStopTime = 0;
	}

	
	
	//У׼ϵͳʱ��
	AdjustSysTime();
}


/************************************************************************
* ������:	AdjustSysTime
* ��  ��:	У׼ϵͳʱ��
* ��  ��:
* ��  ��:
* ��  ��:
************************************************************************/
void CGPSData::AdjustSysTime()
{
	if (g_bAdjust)
	{
		return;
	}
	//�ڵ�һ�ζ�λ������²Ž���У׼
	static bool  bFirstTime = true;

	SYSTEMTIME loctime;
	GetLocalTime(&loctime);
	if (loctime.wYear == 2009)
	{
		bFirstTime = true;
	}

	if( bFirstTime && g_pGpsPosInfo->rmc_data.pos_state.cFix)
	{
		bFirstTime = false;

		m_dLastAzimuth = g_pGpsPosInfo->rmc_data.dbAzimuth;

		SYSTEMTIME sys_time;
		memset(&sys_time, NULL, sizeof(SYSTEMTIME));

		sys_time = g_pGpsPosInfo->rmc_data.UtcTime;

		SetSystemTime(&sys_time);
		g_bAdjust = true;
	}
}

/************************************************************************
* ������:	ReportPos
* ��  ��:	λ����Ϣ�㱨
* ��  ��:
* ��  ��:
* ��  ��:
* ��  ע:	һ�����һ��
************************************************************************/
static const PTERMINAL_PARAM pParam			= CTerminalConfig::Instance()->GetTerminalParam();
//��ʱλ�ø��ٿ���
static const PTEMP_POS_TRACE pTempPosTrace	= CDataParser::Instance()->GetTempPosTrace();
void CGPSData::ReportPos()
{
	static DWORD	dwTime		= 0;	//GPS���ݼ�ʱ
	static DWORD	dwDistance	= 0;	//GPS���ݼ������


	if (m_bfixed && (0 != g_pGpsPosInfo->rmc_data.dbAzimuth))
	{
		if ( fabs(g_pGpsPosInfo->rmc_data.dbAzimuth - m_dLastAzimuth) > pParam->dwCORNER_RETRANSMIT_ANGLE ) //�յ㲹��
		{
			m_dLastAzimuth = g_pGpsPosInfo->rmc_data.dbAzimuth;
			CLogTool::Instance()->WriteLogFile(_T("ReportPos at corner"));
			goto REPORT_POS;
		}
	}



	//��ʱλ�ø��ٿ���
	static DWORD	dwPosTraceValidity	= 0;
	if( pTempPosTrace->wEnalbe )
	{
		//�ж�λ�ø�����Ч��
		if( dwPosTraceValidity++ > pTempPosTrace->dwPosTraceValidity )
		{
			pTempPosTrace->wEnalbe	= 0;
			dwPosTraceValidity		= 0;
			dwTime					= 0;
			dwDistance				= 0;
			return;
		}

		static WORD	wTempTime	= 0;
		//��ʱ
		if( wTempTime++ < pTempPosTrace->wSlice )
			return;

		//��ʱ������
		wTempTime = 0;
		goto REPORT_POS;
	}

	//λ�û㱨���� 0:��ʱ�㱨 1:����㱨 2:��ʱ�Ͷ���㱨
	if( 0 == pParam->dwPOS_REPORT_TYPE )		//��ʱ�㱨
	{
		//��ʱ
		if( dwTime++ < pParam->dwPOS_REPORT_SLICE_DEFAULT )
			return;
		//��ʱ������
		dwTime = 0;
	}
	else if( 1 == pParam->dwPOS_REPORT_TYPE )	//����㱨
	{
		//δ��λ���������
		if (g_pGpsPosInfo->rmc_data.pos_state.cFix != 1)
		{
			return;
		}

		//ÿ5s����һ�ξ���
		if ( g_pGpsPosInfo->rmc_data.UtcTime.wSecond%5 !=0 )
		{
			return;
		}

		//��ʼ����γ��
		static double	dwLastLongitude	= g_pGpsPosInfo->rmc_data.dbLongitude;
		static double	dwLastLatitude	= g_pGpsPosInfo->rmc_data.dbLatitude;

		dwDistance += CalcDistance(dwLastLatitude, dwLastLongitude,g_pGpsPosInfo->rmc_data.dbLatitude, g_pGpsPosInfo->rmc_data.dbLongitude);

		//ȱʡ�㱨����
		if( dwDistance < pParam->dwPOS_REPORT_DIST_DEFAULT )
			return;

		dwDistance = 0;
		//������ξ�γ��
		dwLastLongitude	= g_pGpsPosInfo->rmc_data.dbLongitude;
		dwLastLatitude	= g_pGpsPosInfo->rmc_data.dbLatitude;
	}

REPORT_POS:
	if (!CMsgProc::Instance()->GetLoginState())
	{
		if (m_bfixed)//����ä������
		{
			CDataPack::Instance()->SaveGpsBlindData();
		} 
		else
		{
			CLogTool::Instance()->WriteLogFile(_T("gps error"));
			return;
		}	
	}
	else
	{
		//�ϱ�λ����Ϣ
		CDataPack::Instance()->PackPOS_REPORT();
	}
	
}

/************************************************************************
* ������:	rad
* ��  ��:	���㻡��
* ��  ��:
* ��  ��:
* ��  ��:
* ��  ע:	
************************************************************************/
double CGPSData::rad(double d)
{
	const double pi = 3.1415926535898;
	return d * pi / 180.0;
}

/************************************************************************
* ������:	CalcDistance
* ��  ��:	����������ľ���
* ��  ��:
* ��  ��:
* ��  ��:
* ��  ע:	������gps����㣨��γ�ȣ���������ֱ�߾��룬��λ����
************************************************************************/
int CGPSData::CalcDistance(double fLati1, double fLong1, double fLati2, double fLong2)
{
	const double EARTH_RADIUS = 6378.137;
	double radLat1 = rad(fLati1);  
	double radLat2 = rad(fLati2);  
	double a = radLat1 - radLat2;  
	double b = rad(fLong1) - rad(fLong2);  
	double s = 2 * asin(sqrt(pow(sin(a/2),2) +cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));  
	s = s * EARTH_RADIUS*1000;  

	return s;
}

/************************************************************************
* ������:	CalcDistance
* ��  ��:	����������ľ���
* ��  ��:	�Զ�Ϊ��λ�Ķ�ֵ����10��6�η�����ȷ�������֮һ��
* ��  ��:
* ��  ��:
* ��  ע:	������gps����㣨��γ�ȣ���������ֱ�߾��룬��λ����
************************************************************************/
int CGPSData::CalcDistance(DWORD dwLati1, DWORD dwLong1, DWORD dwLati2, DWORD dwLong2)
{
	return CalcDistance(dwLati1*1e-6, dwLong1*1e-6, dwLati2*1e-6, dwLong2*1e-6);
}

/************************************************************************
* ������:	CheckState
* ��  ��:	���λ����Ϣ״̬λ
* ��  ��:
* ��  ��:
* ��  ��:
* ��  ע:	��鳬�ٱ������������򣬽���·��
************************************************************************/
void CGPSData::CheckState()
{
	//δ��λ�򲻼��
	if( g_alarm_state.unState.gps_state.cFix != 1 )
		return;

	DWORD		dwRegionID	= 0;				//����ID
	enPOS_TYPE	enPosType	= enNO_GIVEN_POS;	//λ������
	//���Ƿ�λ��
	CheckIllegalDisMent();

	//����Ƿ��������·��
	dwRegionID = CheckRegionLine(enPosType);

	//���ٱ���������Ϣ
	SetOverSpeedState(enPosType, dwRegionID);

	//�������򱨾�������Ϣ
	//SetRegionAlarmState(enPosType, dwRegionID);
}
/************************************************************************
* ������:	CheckRegionLine
* ��  ��:	�����ʲô������
* ��  ��:
* ��  ��:	enPOS_TYPE:	��������
* ��  ��:	DWORD:	0:�����κ�������	>0:���������ID
* ��  ע:	
************************************************************************/
DWORD CGPSData::CheckRegionLine(enPOS_TYPE& enPosType)
{
	//����ID
	DWORD	dwRegionID	= 0;
	enPosType	= enNO_GIVEN_POS;	//Ĭ��Ϊ���ض�λ��

	//·��
	dwRegionID = CheckLine();
	if (dwRegionID == 1)
	{
		DeleteRoute();
	}
	return 0;
}
/************************************************************************
* ������:	CheckCircRegion
* ��  ��:	����Ƿ���Բ������
* ��  ��:
* ��  ��:
* ��  ��:	DWORD:	0:�����κ�Բ��������	>0:����Բ�������ID
* ��  ע:	
************************************************************************/
DWORD CGPSData::CheckCircRegion()
{
	//�����Ƿ���Բ������
	if( m_listCircRegion.empty() )
		return 0;

	//��ǰ�㵽Բ������Բ�ĵľ���
	DWORD dwDistance = 0;

	//��������Բ������
	for( LIST_CIRC::iterator i = m_listCircRegion.begin(); i != m_listCircRegion.end(); i++ )
	{
		dwDistance = CalcDistance(i->dwCenterPointLatitude, i->dwCenterPointLongitude, 
			(DWORD)(g_pGpsPosInfo->rmc_data.dbLatitude*1e6),(DWORD)(g_pGpsPosInfo->rmc_data.dbLongitude*1e6) );

		//�ж��Ƿ���Բ��������
		if( dwDistance < i->dwRadius )
			return i->dwRegionID;
	}

	return 0;
}

/************************************************************************
* ������:	CheckRectRegion
* ��  ��:	����Ƿ��ھ�������
* ��  ��:
* ��  ��:
* ��  ��:	DWORD:	0:�����κξ���������	>0:���ھ��������ID
* ��  ע:	
************************************************************************/
DWORD CGPSData::CheckRectRegion()
{
	if ( m_ListRectRegsion.empty() )
	{
		return 0;
	}

	for (LIST_RECT::iterator i = m_ListRectRegsion.begin(); i != m_ListRectRegsion.end(); i++)
	{
		if ( (DWORD)(g_pGpsPosInfo->rmc_data.dbLatitude*1e6) >= ( i->dwlefttopLatitude)
			&& (DWORD)(g_pGpsPosInfo->rmc_data.dbLatitude*1e6) <= ( i->dwrightbuttomLatitude)
			&& (DWORD)(g_pGpsPosInfo->rmc_data.dbLongitude*1e6) >= ( i->dwrightbuttomLongitude)
			&& (DWORD)(g_pGpsPosInfo->rmc_data.dbLongitude*1e6) <= ( i->dwlefttopLongitude) )
		{
			g_alarm_state.gps_extra_info.inout_region_line.cDirection = 0;
		}
		else
		{
			g_alarm_state.gps_extra_info.inout_region_line.cDirection = 1;
		}

		g_alarm_state.unAlarm.alarm_flag.cRegionInOut = 1;
		g_alarm_state.gps_extra_info.inout_region_line.enPosType = enRECTANGULAR;
		g_alarm_state.gps_extra_info.inout_region_line.dwRegionLineID = i->dwRegionID;
		return i->dwRegionID;
	}

	return 1;
}

/************************************************************************
* ������:	CheckPolygonRegion
* ��  ��:	����Ƿ��ڶ��������
* ��  ��:
* ��  ��:
* ��  ��:	DWORD:	0:�����κζ����������	>0:���ڶ���������ID
* ��  ע:	
************************************************************************/
DWORD CGPSData::CheckPolygonRegion()
{
	return 0;
}

/************************************************************************
* ������:	CheckLine
* ��  ��:	����Ƿ���·��
* ��  ��:
* ��  ��:
* ��  ��:	DWORD:	0:�����κ�·����	>0:����·�ε�ID
* ��  ע:	
************************************************************************/
DWORD CGPSData::CheckLine()
{
	if ( m_ListRoute.empty() ) //·������������Ϊ��
	{
		return 0;
	}

	ROUTE_INFO route = m_ListRoute.front();

	int dwDistance = CalcDistance(route.dwInflexionLat, route.dwInflexionLong, 
		(DWORD)(g_pGpsPosInfo->rmc_data.dbLatitude*1e6),(DWORD)(g_pGpsPosInfo->rmc_data.dbLongitude*1e6) );

	if (dwDistance < 50)//route.byRoutWidth
	{
		g_alarm_state.unAlarm.alarm_flag.cGPSSignIn = 0x01;
		CLogTool::Instance()->WriteLogFile(_T("Vehicle arrival"));
		return 1;
	}


	return 0;
}

/************************************************************************
* ������:	SetOverSpeedState
* ��  ��:	���ó��ٱ���״̬
* ��  ��:
* ��  ��:
* ��  ��:
* ��  ע:
************************************************************************/
void CGPSData::SetOverSpeedState(enPOS_TYPE enPosType, DWORD dwRegionID)
{
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();
	static DWORD dwOverSpeedTime = 0;

	//�ж��Ƿ���
	if( (g_pGpsPosInfo->rmc_data.dbSpeed > pParam->dwSPEED_LIMIT) &&(pParam->dwSPEED_LIMIT != 0) )
	{
		dwOverSpeedTime++;
		if (dwOverSpeedTime >= pParam->dwOVERSPEED_DUREATION)
		{
			g_alarm_state.unAlarm.alarm_flag.cOverSpeed	= 1;
			TTSSpeaking(_T("���ѳ�����ʻ�����������"));
		}
	}
	else
	{
		g_alarm_state.unAlarm.alarm_flag.cOverSpeed	= 0;
		dwOverSpeedTime = 0;
	}

	g_alarm_state.gps_extra_info.over_speed.enPosType		= enPosType;	//Բ������
	
	g_alarm_state.gps_extra_info.over_speed.dwRegionLineID	= dwRegionID;
}

/************************************************************************
* ������:	SetRegionAlarmState
* ��  ��:	���ý������򱨾�������Ϣ
* ��  ��:
* ��  ��:
* ��  ��:
* ��  ע:	
************************************************************************/
void CGPSData::SetRegionAlarmState(enPOS_TYPE enPosType, DWORD dwRegionID)
{
	if( dwRegionID > 0 )
	{
		g_alarm_state.unAlarm.alarm_flag.cRegionInOut	= 1;
		g_alarm_state.gps_extra_info.inout_region_line.enPosType		= enPosType;	//Բ������
		g_alarm_state.gps_extra_info.inout_region_line.dwRegionLineID	= dwRegionID;
		g_alarm_state.gps_extra_info.inout_region_line.cDirection		= 0;	//������
	}
	else
	{
		g_alarm_state.unAlarm.alarm_flag.cRegionInOut	= 0;
	}
}

void CGPSData::CheckIllegalDisMent()
{
	WORD dMaxDistence = 0;

	if (g_illegalDisMent_alarm.bSet)
	{
		dMaxDistence = CalcDistance(g_illegalDisMent_alarm.dPointLat, g_illegalDisMent_alarm.dPointLong, g_pGpsPosInfo->rmc_data.dbLatitude, g_pGpsPosInfo->rmc_data.dbLongitude);

		if (dMaxDistence > g_illegalDisMent_alarm.wDis)
		{
			g_alarm_state.unAlarm.alarm_flag.cIllegalDisplacement = 1;

			g_illegalDisMent_alarm.bSet = false;

			CLogTool::Instance()->WriteLogFile(_T("Illegal DisPlaceMent recordLat:%f recordLong:%f  NowdLat:%f  NowdLong:%f  dis:%d"),
				g_illegalDisMent_alarm.dPointLat,g_illegalDisMent_alarm.dPointLong,g_pGpsPosInfo->rmc_data.dbLatitude, g_pGpsPosInfo->rmc_data.dbLongitude,dMaxDistence);
		}
	}
}

BYTE CGPSData::ChangeDec2Hex(BYTE cData)
{
	return (cData/10)*0x10 + (cData%10);
}

BYTE CGPSData::ChangeHex2Dec(BYTE cHexData)
{
	return (cHexData/0x10)*10 + (cHexData%0x10);
}

BOOL CGPSData::SystemTimeToTzSpecificLocalTime(LPTIME_ZONE_INFORMATION lpTimeZone,LPSYSTEMTIME lpUniversalTime,LPSYSTEMTIME lpLocalTime)  
{  
	if(lpTimeZone == NULL || lpUniversalTime == NULL || lpLocalTime == NULL)  
	{  
		//���ָ��Ϊ�գ���û�б�Ҫ�����κμ���  
		return FALSE;  
	}  

	//��UTCʱ����SYSTEMTIMEת��ΪFILETIME��ʽ  
	FILETIME ftUniversalTime = {0};  
	SystemTimeToFileTime(lpUniversalTime,&ftUniversalTime);  

	//��FILETIME��ʽʱ�����ֵ�洢��һ��DWORD64������  
	DWORD64 ddwUniversalTime = ftUniversalTime.dwHighDateTime;  
	ddwUniversalTime = ddwUniversalTime << 32;  
	ddwUniversalTime += ftUniversalTime.dwLowDateTime;  

	//��ΪFILETIME��ʱ�䵥λ��100���ڷ�֮һ�룬Ȼ��TIME_ZONE_INFORMATION��ʱ�䵥λ�Ƿ֣�����������Ҫ����600000000   
	DWORD64 ddwBias = abs(lpTimeZone->Bias);   
	ddwBias *= 600000000;  

	//ת����ʽΪ��LOCAL_TIME = UTC - BIAS  
	DWORD64 ddwLocalTime = 0;  
	if(lpTimeZone->Bias > 0)  
	{    
		ddwLocalTime = ddwUniversalTime - ddwBias;  
	}  
	else if(lpTimeZone->Bias < 0)  
	{  
		ddwLocalTime = ddwUniversalTime + ddwBias;  
	}  

	//��DWORD64��ֵת��ΪFILETIME��ʽ  
	FILETIME ftLocalTime = {0};  
	ftLocalTime.dwLowDateTime = static_cast<DWORD>(ddwLocalTime);  
	ftLocalTime.dwHighDateTime = static_cast<DWORD>(ddwLocalTime >> 32);  

	//��FILETIME��ֵת��ΪSYSTEMTIME��ʽ������  
	return FileTimeToSystemTime(&ftLocalTime,lpLocalTime);  
}

void CGPSData::AddRectRegsion(RECT_REGION &rectReg)
{
	m_ListRectRegsion.push_back(rectReg);
}

void CGPSData::DeleteRegsion()
{
	m_ListRectRegsion.clear();
}

void CGPSData::AddRoute(ROUTE_INFO &RoutInfo)
{
	m_ListRoute.clear();
	m_ListRoute.push_back(RoutInfo);
}

void CGPSData::DeleteRoute()
{
	m_ListRoute.clear();
}