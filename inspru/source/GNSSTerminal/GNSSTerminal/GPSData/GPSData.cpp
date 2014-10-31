#include "StdAfx.h"
#include "GPSData.h"
#include "../Tools/LogTool.h"
#include "../TerminalConfig/TerminalConfig.h"
#include "../DataParser/DataPack.h"
#include "../DataParser/DataParser.h"
#include "../MsgProc/MsgProc.h"

extern void TTSSpeaking(CString csText);

//初始化静态成员变量
CGPSData*	CGPSData::m_pInstance	= NULL;
GPS_ALARM_STATE     g_alarm_state;		//报警标识状态位和附加信息
Illegal_DisplaceMent_ALARM g_illegalDisMent_alarm; //非法位移报警
LIST_CIRC			m_listCircRegion;	//圆形区域链表

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
* 函数名: Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: singleton
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
* 函数名:	SetReportPos
* 描  述:	设置位置信息上报开关
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
void CGPSData::SetReportPos( BOOL bSwitch )
{
	m_bSwitch = bSwitch;
}


void CGPSData::GPSReportAndAdjusttime()
{
	//位置信息汇报
	ReportPos();

	int iStopTime = 0; 
	static DWORD dwStart = 0;

	dwStart++;

	if ( (0 == g_pGpsPosInfo->rmc_data.pos_state.cFix) && dwStart >180)//判断为GPS天线故障
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

		g_alarm_state.gps_extra_info.dwMileage += g_pGpsPosInfo->rmc_data.dbSpeed * 10;  //累计里程


		g_alarm_state.unAlarm.alarm_flag.cParkingOverTime = 0;

		
		iStopTime = 0;
	}

	
	
	//校准系统时间
	AdjustSysTime();
}


/************************************************************************
* 函数名:	AdjustSysTime
* 描  述:	校准系统时间
* 入  参:
* 出  参:
* 返  回:
************************************************************************/
void CGPSData::AdjustSysTime()
{
	if (g_bAdjust)
	{
		return;
	}
	//在第一次定位的情况下才进行校准
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
* 函数名:	ReportPos
* 描  述:	位置信息汇报
* 入  参:
* 出  参:
* 返  回:
* 备  注:	一秒调用一次
************************************************************************/
static const PTERMINAL_PARAM pParam			= CTerminalConfig::Instance()->GetTerminalParam();
//临时位置跟踪控制
static const PTEMP_POS_TRACE pTempPosTrace	= CDataParser::Instance()->GetTempPosTrace();
void CGPSData::ReportPos()
{
	static DWORD	dwTime		= 0;	//GPS数据计时
	static DWORD	dwDistance	= 0;	//GPS数据计算距离


	if (m_bfixed && (0 != g_pGpsPosInfo->rmc_data.dbAzimuth))
	{
		if ( fabs(g_pGpsPosInfo->rmc_data.dbAzimuth - m_dLastAzimuth) > pParam->dwCORNER_RETRANSMIT_ANGLE ) //拐点补报
		{
			m_dLastAzimuth = g_pGpsPosInfo->rmc_data.dbAzimuth;
			CLogTool::Instance()->WriteLogFile(_T("ReportPos at corner"));
			goto REPORT_POS;
		}
	}



	//临时位置跟踪控制
	static DWORD	dwPosTraceValidity	= 0;
	if( pTempPosTrace->wEnalbe )
	{
		//判断位置跟踪有效期
		if( dwPosTraceValidity++ > pTempPosTrace->dwPosTraceValidity )
		{
			pTempPosTrace->wEnalbe	= 0;
			dwPosTraceValidity		= 0;
			dwTime					= 0;
			dwDistance				= 0;
			return;
		}

		static WORD	wTempTime	= 0;
		//计时
		if( wTempTime++ < pTempPosTrace->wSlice )
			return;

		//计时器清零
		wTempTime = 0;
		goto REPORT_POS;
	}

	//位置汇报策略 0:定时汇报 1:定距汇报 2:定时和定距汇报
	if( 0 == pParam->dwPOS_REPORT_TYPE )		//定时汇报
	{
		//计时
		if( dwTime++ < pParam->dwPOS_REPORT_SLICE_DEFAULT )
			return;
		//计时器清零
		dwTime = 0;
	}
	else if( 1 == pParam->dwPOS_REPORT_TYPE )	//定距汇报
	{
		//未定位不计算距离
		if (g_pGpsPosInfo->rmc_data.pos_state.cFix != 1)
		{
			return;
		}

		//每5s计算一次距离
		if ( g_pGpsPosInfo->rmc_data.UtcTime.wSecond%5 !=0 )
		{
			return;
		}

		//初始化经纬度
		static double	dwLastLongitude	= g_pGpsPosInfo->rmc_data.dbLongitude;
		static double	dwLastLatitude	= g_pGpsPosInfo->rmc_data.dbLatitude;

		dwDistance += CalcDistance(dwLastLatitude, dwLastLongitude,g_pGpsPosInfo->rmc_data.dbLatitude, g_pGpsPosInfo->rmc_data.dbLongitude);

		//缺省汇报距离
		if( dwDistance < pParam->dwPOS_REPORT_DIST_DEFAULT )
			return;

		dwDistance = 0;
		//保存这次经纬度
		dwLastLongitude	= g_pGpsPosInfo->rmc_data.dbLongitude;
		dwLastLatitude	= g_pGpsPosInfo->rmc_data.dbLatitude;
	}

REPORT_POS:
	if (!CMsgProc::Instance()->GetLoginState())
	{
		if (m_bfixed)//保存盲区数据
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
		//上报位置信息
		CDataPack::Instance()->PackPOS_REPORT();
	}
	
}

/************************************************************************
* 函数名:	rad
* 描  述:	计算弧度
* 入  参:
* 出  参:
* 返  回:
* 备  注:	
************************************************************************/
double CGPSData::rad(double d)
{
	const double pi = 3.1415926535898;
	return d * pi / 180.0;
}

/************************************************************************
* 函数名:	CalcDistance
* 描  述:	计算两个点的距离
* 入  参:
* 出  参:
* 返  回:
* 备  注:	从两个gps坐标点（经纬度）获得两点的直线距离，单位是米
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
* 函数名:	CalcDistance
* 描  述:	计算两个点的距离
* 入  参:	以度为单位的度值乘以10的6次方，精确到百万分之一度
* 出  参:
* 返  回:
* 备  注:	从两个gps坐标点（经纬度）获得两点的直线距离，单位是米
************************************************************************/
int CGPSData::CalcDistance(DWORD dwLati1, DWORD dwLong1, DWORD dwLati2, DWORD dwLong2)
{
	return CalcDistance(dwLati1*1e-6, dwLong1*1e-6, dwLati2*1e-6, dwLong2*1e-6);
}

/************************************************************************
* 函数名:	CheckState
* 描  述:	检查位置信息状态位
* 入  参:
* 出  参:
* 返  回:
* 备  注:	检查超速报警，进出区域，进出路线
************************************************************************/
void CGPSData::CheckState()
{
	//未定位则不检查
	if( g_alarm_state.unState.gps_state.cFix != 1 )
		return;

	DWORD		dwRegionID	= 0;				//区域ID
	enPOS_TYPE	enPosType	= enNO_GIVEN_POS;	//位置类型
	//检查非法位移
	CheckIllegalDisMent();

	//检查是否在区域或路段
	dwRegionID = CheckRegionLine(enPosType);

	//超速报警附加信息
	SetOverSpeedState(enPosType, dwRegionID);

	//进出区域报警附加信息
	//SetRegionAlarmState(enPosType, dwRegionID);
}
/************************************************************************
* 函数名:	CheckRegionLine
* 描  述:	检查在什么区域内
* 入  参:
* 出  参:	enPOS_TYPE:	区域类型
* 返  回:	DWORD:	0:不在任何区域内	>0:所在区域的ID
* 备  注:	
************************************************************************/
DWORD CGPSData::CheckRegionLine(enPOS_TYPE& enPosType)
{
	//区域ID
	DWORD	dwRegionID	= 0;
	enPosType	= enNO_GIVEN_POS;	//默认为无特定位置

	//路段
	dwRegionID = CheckLine();
	if (dwRegionID == 1)
	{
		DeleteRoute();
	}
	return 0;
}
/************************************************************************
* 函数名:	CheckCircRegion
* 描  述:	检查是否在圆形区域
* 入  参:
* 出  参:
* 返  回:	DWORD:	0:不在任何圆形区域内	>0:所在圆形区域的ID
* 备  注:	
************************************************************************/
DWORD CGPSData::CheckCircRegion()
{
	//测试是否有圆形区域
	if( m_listCircRegion.empty() )
		return 0;

	//当前点到圆形区域圆心的距离
	DWORD dwDistance = 0;

	//遍历所有圆形区域
	for( LIST_CIRC::iterator i = m_listCircRegion.begin(); i != m_listCircRegion.end(); i++ )
	{
		dwDistance = CalcDistance(i->dwCenterPointLatitude, i->dwCenterPointLongitude, 
			(DWORD)(g_pGpsPosInfo->rmc_data.dbLatitude*1e6),(DWORD)(g_pGpsPosInfo->rmc_data.dbLongitude*1e6) );

		//判断是否在圆形区域内
		if( dwDistance < i->dwRadius )
			return i->dwRegionID;
	}

	return 0;
}

/************************************************************************
* 函数名:	CheckRectRegion
* 描  述:	检查是否在矩形区域
* 入  参:
* 出  参:
* 返  回:	DWORD:	0:不在任何矩形区域内	>0:所在矩形区域的ID
* 备  注:	
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
* 函数名:	CheckPolygonRegion
* 描  述:	检查是否在多边形区域
* 入  参:
* 出  参:
* 返  回:	DWORD:	0:不在任何多边形区域内	>0:所在多边形区域的ID
* 备  注:	
************************************************************************/
DWORD CGPSData::CheckPolygonRegion()
{
	return 0;
}

/************************************************************************
* 函数名:	CheckLine
* 描  述:	检查是否在路段
* 入  参:
* 出  参:
* 返  回:	DWORD:	0:不在任何路段内	>0:所在路段的ID
* 备  注:	
************************************************************************/
DWORD CGPSData::CheckLine()
{
	if ( m_ListRoute.empty() ) //路线链表内数据为空
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
* 函数名:	SetOverSpeedState
* 描  述:	设置超速报警状态
* 入  参:
* 出  参:
* 返  回:
* 备  注:
************************************************************************/
void CGPSData::SetOverSpeedState(enPOS_TYPE enPosType, DWORD dwRegionID)
{
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();
	static DWORD dwOverSpeedTime = 0;

	//判断是否超速
	if( (g_pGpsPosInfo->rmc_data.dbSpeed > pParam->dwSPEED_LIMIT) &&(pParam->dwSPEED_LIMIT != 0) )
	{
		dwOverSpeedTime++;
		if (dwOverSpeedTime >= pParam->dwOVERSPEED_DUREATION)
		{
			g_alarm_state.unAlarm.alarm_flag.cOverSpeed	= 1;
			TTSSpeaking(_T("您已超速行驶，请减速慢行"));
		}
	}
	else
	{
		g_alarm_state.unAlarm.alarm_flag.cOverSpeed	= 0;
		dwOverSpeedTime = 0;
	}

	g_alarm_state.gps_extra_info.over_speed.enPosType		= enPosType;	//圆形区域
	
	g_alarm_state.gps_extra_info.over_speed.dwRegionLineID	= dwRegionID;
}

/************************************************************************
* 函数名:	SetRegionAlarmState
* 描  述:	设置进出区域报警附加信息
* 入  参:
* 出  参:
* 返  回:
* 备  注:	
************************************************************************/
void CGPSData::SetRegionAlarmState(enPOS_TYPE enPosType, DWORD dwRegionID)
{
	if( dwRegionID > 0 )
	{
		g_alarm_state.unAlarm.alarm_flag.cRegionInOut	= 1;
		g_alarm_state.gps_extra_info.inout_region_line.enPosType		= enPosType;	//圆形区域
		g_alarm_state.gps_extra_info.inout_region_line.dwRegionLineID	= dwRegionID;
		g_alarm_state.gps_extra_info.inout_region_line.cDirection		= 0;	//进区域
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
		//如果指针为空，则没有必要进行任何计算  
		return FALSE;  
	}  

	//将UTC时间由SYSTEMTIME转换为FILETIME格式  
	FILETIME ftUniversalTime = {0};  
	SystemTimeToFileTime(lpUniversalTime,&ftUniversalTime);  

	//将FILETIME格式时间的数值存储到一个DWORD64变量中  
	DWORD64 ddwUniversalTime = ftUniversalTime.dwHighDateTime;  
	ddwUniversalTime = ddwUniversalTime << 32;  
	ddwUniversalTime += ftUniversalTime.dwLowDateTime;  

	//因为FILETIME的时间单位是100个亿分之一秒，然后TIME_ZONE_INFORMATION的时间单位是分，所以这里需要乘以600000000   
	DWORD64 ddwBias = abs(lpTimeZone->Bias);   
	ddwBias *= 600000000;  

	//转换公式为：LOCAL_TIME = UTC - BIAS  
	DWORD64 ddwLocalTime = 0;  
	if(lpTimeZone->Bias > 0)  
	{    
		ddwLocalTime = ddwUniversalTime - ddwBias;  
	}  
	else if(lpTimeZone->Bias < 0)  
	{  
		ddwLocalTime = ddwUniversalTime + ddwBias;  
	}  

	//将DWORD64数值转换为FILETIME格式  
	FILETIME ftLocalTime = {0};  
	ftLocalTime.dwLowDateTime = static_cast<DWORD>(ddwLocalTime);  
	ftLocalTime.dwHighDateTime = static_cast<DWORD>(ddwLocalTime >> 32);  

	//将FILETIME数值转换为SYSTEMTIME格式并返回  
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