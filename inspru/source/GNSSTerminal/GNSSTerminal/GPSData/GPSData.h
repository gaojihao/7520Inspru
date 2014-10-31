#pragma once

#include <list>
#include "../DataParser/DataType.h"
#include "../DataParser/MsgBase.h"

using std::list;

typedef	list<CIRCULAR_REGION>	LIST_CIRC;
extern	LIST_CIRC				m_listCircRegion;		//圆形区域链表
extern GPS_ALARM_STATE			g_alarm_state;
extern Illegal_DisplaceMent_ALARM g_illegalDisMent_alarm;
typedef	list<RECT_REGION>	LIST_RECT;
typedef	list<ROUTE_INFO>	LIST_ROUTE;

extern bool g_bAdjust;

class CGPSData : public CMsgBase
{
	//typedef	list<CIRCULAR_REGION>	LIST_CIRC;
private:
	static CGPSData*		m_pInstance;			//Singleton模式实例指针
	BOOL					m_bSwitch;				//位置信息上报开关
	BOOL                  m_bfixed;               //是否已经定位  
	double					m_dLastAzimuth;
 
public:
	CGPSData(void);
	~CGPSData(void);

	//Singleton模式实例函数
	static CGPSData* Instance();

	//设置位置信息上报开关
	void	SetReportPos( BOOL bSwitch );
	//检查超速报警，进出区域，进出路线
	void	CheckState();
	
	BYTE ChangeHex2Dec(BYTE cHexData);
	BYTE ChangeDec2Hex(BYTE cDecData);
	BOOL SystemTimeToTzSpecificLocalTime(LPTIME_ZONE_INFORMATION lpTimeZone,LPSYSTEMTIME lpUniversalTime,LPSYSTEMTIME lpLocalTime);
	void GPSReportAndAdjusttime();

	void AddRectRegsion(RECT_REGION &rectReg);
	void DeleteRegsion();

	void AddRoute(ROUTE_INFO &RoutInfo);
	void DeleteRoute();

	//从两个gps坐标点（经纬度）获得两点的直线距离，单位是米
	int		CalcDistance(double fLati1, double fLong1, double fLati2, double fLong2);
	int		CalcDistance(DWORD dwLati1, DWORD dwLong1, DWORD dwLati2, DWORD dwLong2);
	


private:

	//校准系统时间
	void	AdjustSysTime();
	//位置信息汇报
	void	ReportPos();
	//计算弧度
	double	rad(double d);
	
	
	//////////////////////////////区域路段报警相关////////////////////////////////////////////
	//检查在什么区域内
	DWORD	CheckRegionLine(enPOS_TYPE& enPosType);
	//检查是否在圆形区域
	DWORD	CheckCircRegion();
	//检查是否在矩形区域
	DWORD	CheckRectRegion();
	//检查是否在多边形区域
	DWORD	CheckPolygonRegion();
	//检查是否在路段
	DWORD	CheckLine();
	//设置超速报警状态
	void	SetOverSpeedState(enPOS_TYPE enPosType, DWORD dwRegionID);
	//设置进出区域报警附加信息
	void	SetRegionAlarmState(enPOS_TYPE enPosType, DWORD dwRegionID);

	void	CheckIllegalDisMent();

private:
	LIST_RECT m_ListRectRegsion;
	LIST_ROUTE m_ListRoute;
};
