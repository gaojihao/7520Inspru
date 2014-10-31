#pragma once

#include <list>
#include "../DataParser/DataType.h"
#include "../DataParser/MsgBase.h"

using std::list;

typedef	list<CIRCULAR_REGION>	LIST_CIRC;
extern	LIST_CIRC				m_listCircRegion;		//Բ����������
extern GPS_ALARM_STATE			g_alarm_state;
extern Illegal_DisplaceMent_ALARM g_illegalDisMent_alarm;
typedef	list<RECT_REGION>	LIST_RECT;
typedef	list<ROUTE_INFO>	LIST_ROUTE;

extern bool g_bAdjust;

class CGPSData : public CMsgBase
{
	//typedef	list<CIRCULAR_REGION>	LIST_CIRC;
private:
	static CGPSData*		m_pInstance;			//Singletonģʽʵ��ָ��
	BOOL					m_bSwitch;				//λ����Ϣ�ϱ�����
	BOOL                  m_bfixed;               //�Ƿ��Ѿ���λ  
	double					m_dLastAzimuth;
 
public:
	CGPSData(void);
	~CGPSData(void);

	//Singletonģʽʵ������
	static CGPSData* Instance();

	//����λ����Ϣ�ϱ�����
	void	SetReportPos( BOOL bSwitch );
	//��鳬�ٱ������������򣬽���·��
	void	CheckState();
	
	BYTE ChangeHex2Dec(BYTE cHexData);
	BYTE ChangeDec2Hex(BYTE cDecData);
	BOOL SystemTimeToTzSpecificLocalTime(LPTIME_ZONE_INFORMATION lpTimeZone,LPSYSTEMTIME lpUniversalTime,LPSYSTEMTIME lpLocalTime);
	void GPSReportAndAdjusttime();

	void AddRectRegsion(RECT_REGION &rectReg);
	void DeleteRegsion();

	void AddRoute(ROUTE_INFO &RoutInfo);
	void DeleteRoute();

	//������gps����㣨��γ�ȣ���������ֱ�߾��룬��λ����
	int		CalcDistance(double fLati1, double fLong1, double fLati2, double fLong2);
	int		CalcDistance(DWORD dwLati1, DWORD dwLong1, DWORD dwLati2, DWORD dwLong2);
	


private:

	//У׼ϵͳʱ��
	void	AdjustSysTime();
	//λ����Ϣ�㱨
	void	ReportPos();
	//���㻡��
	double	rad(double d);
	
	
	//////////////////////////////����·�α������////////////////////////////////////////////
	//�����ʲô������
	DWORD	CheckRegionLine(enPOS_TYPE& enPosType);
	//����Ƿ���Բ������
	DWORD	CheckCircRegion();
	//����Ƿ��ھ�������
	DWORD	CheckRectRegion();
	//����Ƿ��ڶ��������
	DWORD	CheckPolygonRegion();
	//����Ƿ���·��
	DWORD	CheckLine();
	//���ó��ٱ���״̬
	void	SetOverSpeedState(enPOS_TYPE enPosType, DWORD dwRegionID);
	//���ý������򱨾�������Ϣ
	void	SetRegionAlarmState(enPOS_TYPE enPosType, DWORD dwRegionID);

	void	CheckIllegalDisMent();

private:
	LIST_RECT m_ListRectRegsion;
	LIST_ROUTE m_ListRoute;
};
