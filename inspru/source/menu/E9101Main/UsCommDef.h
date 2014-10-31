
// UsCommDef.h

#if !defined(_USCOMDEF_H_2005_12_15_)   
#define   _USCOMDEF_H_2005_12_15_


#define TIMEOUT_LOCATION_MSG	3000		//消息超时

const double	Knot2Meter		    = 1.851985;			// 节->公里/小时
const long		SAT_NUMBER	   	    = 20;				// 定义卫星数
const long		_SAT_ID_NUMBER	    = 12;				// 定义卫星ID数目
const double	DOP_MULTIPLE		= 10.0;				//精度倍数

#define BUFFER_INPUT_COM	1024	//串口输入缓冲区
#define BUFFER_OUTPUT_COM	1024	//串口输出缓冲区
#define RECIEVE_BUFFER_WORD	211		//接收字符串缓冲区

//////////////////////////////////////////////////////////////////////////
/*三种数据类型
*/
enum enDataType
{
	LOCATION_GPSDATA,	//指GPS数据
	LOCATION_DOPDATA,	//指DOP数据
	LOCATION_SATDATA	//指SAT数据
};

//////////////////////////////////////////////////////////////////////////
/*真实定位还是模拟定位
*/
enum enLocation
{
	LOCATION_REAL,	//真实定位
	LOCATION_SIMUL	//模拟定位
};

///////////////////////////////////////////////////////////////////////////////
// GPS主要数据结构体
struct SGpsData
{	
	BYTE				byFixType;					// 定位标志，0-no fixed,1-fixed.
	BOOL				bIsAzimuth;					// 方位角无效				
	double				dbLat;						// 纬度
	double				dbLong;						// 经度
	double				dbSpeed;					// 相对于地面的速度
	double				dbAzimuth;					// 方位角 (度表示)
	SYSTEMTIME			UtcTime;					// utc时间
	double				dbAlt;						// 海拔高度
};

///////////////////////////////////////////////////////////////////////////////
// 定位解算精度的几何图形(DOP)数据
struct SDopData
{
	int					iFixState;		//定位状态:1-无定位信息，2-2D, 3-3D, 6-DR Fixed
	double				dbPdop;					// PDOP值
	double				dbHdop;					// HDOP值
	double				dbVdop;					// VDOP值
	int					nNum;					// 锁定卫星个数
	int					nUsedID[SAT_NUMBER];	// 锁定卫星id
};

///////////////////////////////////////////////////////////////////////////////
// 卫星数据
struct SSatData
{
	int					nSatID;						// 卫星编号
	int					nElevation;					// 仰角
	int					nAzimuth;					// 方位角
	int					nSNR;						// 信噪比.0~99,-1为无效
	BOOL				bUsed;
};

#endif