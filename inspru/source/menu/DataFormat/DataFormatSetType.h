#ifndef _DATAFORMAT_SETTYPE_H_
#define _DATAFORMAT_SETTYPE_H_


enum enTimeUnit
{
	ID_24_TYPE = 0,
	ID_12_TYPE = 1
};

enum enDistanceUnit
{
	ID_MILT = 0,			
	ID_KM = 1
};

enum enSpeedUnit
{
	ID_M_S = 0,
	ID_KM_H = 1
};

enum enHourUnit
{
	ID_HOUR = 0,
	ID_MINUTE =1,
	ID_SECOND =2
};

enum enLongitudeALatitudeUnit
{
	ID_DEGREE = 0,
	ID_DEGREE_MINUTE = 1
};

enum enAngleUnit
{
	ID_Angle = 0,
	ID_Radian = 1
};

//enum enTimeZone
//{
//	ID_BEIJING = 0
// };

static TIME_ZONE_INFORMATION gTziData[] = 
{
	{
0,//Bias: 
_T("(GMT) ��������ƽʱ��׼ʱ��"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT) ��������ƽʱ�ļ�ʱ��"),//DaylightName:
{0,3,0,5,1,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 1--------------
{
-60,//Bias: 
_T("(GMT+01:00)��ŷ��׼ʱ��"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+01:00)��ŷ�ļ�ʱ��"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 2--------------
{
-120,//Bias: 
_T("(GMT+02:00)��ŷ��׼ʱ��"),//StandardName: 
{0,10,0,5,1,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+02:00)��ŷ�ļ�ʱ��"),//DaylightName:
{0,3,0,5,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 3--------------
{
-180,//Bias: 
_T("(GMT+03:00)��������׼ʱ��"),//StandardName: 
{0,10,0,1,4,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+03:00)�������ļ�ʱ��"),//DaylightName:
{0,4,0,1,3,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 4--------------
{
-240,//Bias: 
_T("(GMT+04:00)�������뵺��׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+04:00)�������뵺�ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 5--------------
{
-300,//Bias: 
_T("(GMT+05:00)Ҷ�����ձ���׼ʱ��"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+05:00)Ҷ�����ձ��ļ�ʱ��"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 6--------------
{
-360,//Bias: 
_T("(GMT+06:00)���Ǳ�����׼ʱ��"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+06:00)���Ǳ����ļ�ʱ��"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 7--------------
{
-420,//Bias: 
_T("(GMT+07:00)���Ǳ�׼ʱ��"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+07:00)�����ļ�ʱ��"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 8--------------
{
-480,//Bias: 
_T("(GMT+08:00)�й���׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+08:00)�й��ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 9--------------
{
-540,//Bias: 
_T("(GMT+09:00)������׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+09:00)�����ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 10--------------
{
-600,//Bias: 
_T("(GMT+10:00)�Ĵ����Ƕ�����׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+10:00)�Ĵ����Ƕ����ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 11--------------
{
-660,//Bias: 
_T("(GMT+11:00)̫ƽ���в���׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+11:00)̫ƽ���в��ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 12--------------
{
-720,//Bias: 
_T("(GMT+12:00)��ʿ��׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+12:00)��ʿ�ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 13--------------
{
-780,//Bias: 
_T("(GMT+13:00)���ӱ�׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+13:00)�����ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -1--------------
{
60,//Bias: 
_T("(GMT-01:00)��ý�Ⱥ����׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-01:00)��ý�Ⱥ���ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -2--------------
{
120,//Bias: 
_T("(GMT-02:00)�д������׼ʱ��"),//StandardName: 
{0,9,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-02:00)�д������ļ�ʱ��"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -3--------------
{
180,//Bias: 
_T("(GMT-03:00)������������׼ʱ��"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-03:00)�����������ļ�ʱ��"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -4--------------
{
240,//Bias: 
_T("(GMT-04:00)�������׼ʱ��"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-04:00)�������ļ�ʱ��"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -5--------------
{
300,//Bias: 
_T("(GMT-05:00)������׼ʱ��"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-05:00)�����ļ�ʱ��"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -6--------------
{
360,//Bias: 
_T("(GMT-06:00)ī�����׼ʱ��"),//StandardName: 
{0,9,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-06:00)ī�����ļ�ʱ��"),//DaylightName:
{0,5,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -7--------------
{
420,//Bias: 
_T("(GMT-07:00)ī�����׼ʱ�� 2"),//StandardName: 
{0,9,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-07:00)ī�����ļ�ʱ�� 2"),//DaylightName:
{0,5,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -8--------------
{
480,//Bias: 
_T("(GMT-08:00)̫ƽ���׼ʱ��"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-08:00)̫ƽ���ļ�ʱ��"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -9--------------
{
540,//Bias: 
_T("(GMT-09:00)����˹�ӱ�׼ʱ��"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-09:00)����˹���ļ�ʱ��"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -10--------------
{
600,//Bias: 
_T("(GMT-10:00)�����ı�׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-10:00)�������ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -11--------------
{
660,//Bias: 
_T("(GMT-11:00)��Ħ��Ⱥ����׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-11:00)��Ħ��Ⱥ���ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -12--------------
{
720,//Bias: 
_T("(GMT-12:00)�ս��߱�׼ʱ��"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-12:00)�ս����ļ�ʱ��"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},

};

#endif
