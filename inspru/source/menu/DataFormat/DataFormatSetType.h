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
_T("(GMT) 格林威治平时标准时间"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT) 格林威治平时夏季时间"),//DaylightName:
{0,3,0,5,1,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 1--------------
{
-60,//Bias: 
_T("(GMT+01:00)西欧标准时间"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+01:00)西欧夏季时间"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 2--------------
{
-120,//Bias: 
_T("(GMT+02:00)东欧标准时间"),//StandardName: 
{0,10,0,5,1,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+02:00)东欧夏季时间"),//DaylightName:
{0,3,0,5,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 3--------------
{
-180,//Bias: 
_T("(GMT+03:00)阿拉伯标准时间"),//StandardName: 
{0,10,0,1,4,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+03:00)阿拉伯夏季时间"),//DaylightName:
{0,4,0,1,3,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 4--------------
{
-240,//Bias: 
_T("(GMT+04:00)阿拉伯半岛标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+04:00)阿拉伯半岛夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 5--------------
{
-300,//Bias: 
_T("(GMT+05:00)叶卡捷琳堡标准时间"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+05:00)叶卡捷琳堡夏季时间"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 6--------------
{
-360,//Bias: 
_T("(GMT+06:00)中亚北部标准时间"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+06:00)中亚北部夏季时间"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 7--------------
{
-420,//Bias: 
_T("(GMT+07:00)北亚标准时间"),//StandardName: 
{0,10,0,5,3,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+07:00)北亚夏季时间"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 8--------------
{
-480,//Bias: 
_T("(GMT+08:00)中国标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+08:00)中国夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 9--------------
{
-540,//Bias: 
_T("(GMT+09:00)东京标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+09:00)东京夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 10--------------
{
-600,//Bias: 
_T("(GMT+10:00)澳大利亚东部标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+10:00)澳大利亚东部夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 11--------------
{
-660,//Bias: 
_T("(GMT+11:00)太平洋中部标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+11:00)太平洋中部夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 12--------------
{
-720,//Bias: 
_T("(GMT+12:00)富士标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+12:00)富士夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone 13--------------
{
-780,//Bias: 
_T("(GMT+13:00)汤加标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT+13:00)汤加夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -1--------------
{
60,//Bias: 
_T("(GMT-01:00)佛得角群岛标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-01:00)佛得角群岛夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -2--------------
{
120,//Bias: 
_T("(GMT-02:00)中大西洋标准时间"),//StandardName: 
{0,9,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-02:00)中大西洋夏季时间"),//DaylightName:
{0,3,0,5,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -3--------------
{
180,//Bias: 
_T("(GMT-03:00)格陵兰东部标准时间"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-03:00)格陵兰东部夏季时间"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -4--------------
{
240,//Bias: 
_T("(GMT-04:00)大西洋标准时间"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-04:00)大西洋夏季时间"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -5--------------
{
300,//Bias: 
_T("(GMT-05:00)东部标准时间"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-05:00)东部夏季时间"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -6--------------
{
360,//Bias: 
_T("(GMT-06:00)墨西哥标准时间"),//StandardName: 
{0,9,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-06:00)墨西哥夏季时间"),//DaylightName:
{0,5,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -7--------------
{
420,//Bias: 
_T("(GMT-07:00)墨西哥标准时间 2"),//StandardName: 
{0,9,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-07:00)墨西哥夏季时间 2"),//DaylightName:
{0,5,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -8--------------
{
480,//Bias: 
_T("(GMT-08:00)太平洋标准时间"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-08:00)太平洋夏季时间"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -9--------------
{
540,//Bias: 
_T("(GMT-09:00)阿拉斯加标准时间"),//StandardName: 
{0,10,0,5,2,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-09:00)阿拉斯加夏季时间"),//DaylightName:
{0,4,0,1,2,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -10--------------
{
600,//Bias: 
_T("(GMT-10:00)夏威夷标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-10:00)夏威夷夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -11--------------
{
660,//Bias: 
_T("(GMT-11:00)萨摩亚群岛标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-11:00)萨摩亚群岛夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},
//-----------Time Zone -12--------------
{
720,//Bias: 
_T("(GMT-12:00)日界线标准时间"),//StandardName: 
{0,0,0,0,0,0,0,0},//StandardDate: 
0,//StandardBias:
_T("(GMT-12:00)日界线夏季时间"),//DaylightName:
{0,0,0,0,0,0,0,0},//DaylightDate: 
-60//DaylightBias: 
},

};

#endif
