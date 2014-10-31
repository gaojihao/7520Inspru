// DmmUtil.h: interface for the DmmUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMMUTIL_H__C9138A84_1687_4EB0_A1A2_A50E9B01FB77__INCLUDED_)
#define AFX_DMMUTIL_H__C9138A84_1687_4EB0_A1A2_A50E9B01FB77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const double PI				= 3.14159265358979;

class CDmmUtil  
{
public:
	CDmmUtil();
	virtual ~CDmmUtil();

	//从ddmm.mmmm转换到带小数的d（度）
	static double DM_2_D(double dValue);
	static double D_2_M(double dValue);

	//弧度，角度转化
	static double AngleToRadian(double angle);
	static double RadianToAngle(double radian);

};

#endif // !defined(AFX_DMMUTIL_H__C9138A84_1687_4EB0_A1A2_A50E9B01FB77__INCLUDED_)
