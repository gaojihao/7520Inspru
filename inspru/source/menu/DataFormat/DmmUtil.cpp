// DmmUtil.cpp: implementation of the DmmUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DmmUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDmmUtil::CDmmUtil()
{

}

CDmmUtil::~CDmmUtil()
{

}

double CDmmUtil::DM_2_D(double dValue) 
{
	double rst;

	int iDegree=(int)dValue/100;

	double dMinute=dValue-(iDegree*100.0);

	rst=iDegree+(dMinute / 60.0);

	return rst;

}

double CDmmUtil::D_2_M(double dValue)
{
	double dRst;

	int iDegree=(int)dValue;

	double dReserv=dValue-iDegree;

	dRst=(iDegree*100)+dReserv*60.0;

	return dRst;

}

double CDmmUtil::AngleToRadian(double angle)
{
	return (PI * (angle/180.0));
}
double CDmmUtil::RadianToAngle(double radian)
{
	return (radian * 180.0/PI );
}