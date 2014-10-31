#ifndef VEHICLEINFO_20110121
#define VEHICLEINFO_20110121

#ifdef __cplusplus
extern "C" {
#endif

#ifdef VEHICLEINFO_EXPORTS
#define VEHICLEINFO_API __declspec(dllexport)
#else
#define VEHICLEINFO_API __declspec(dllimport)
#endif

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long DWORD;

/*根据设定的溢出时间设置状态位，如果过了设定时间则表示接收该ID数据出处*/
typedef struct tagTimeOut
{
	UCHAR ucCluster1_Timeout:1;                //18FF6017的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucCluster2_Timeout:1;                //18FF6217的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucEngineSpeedTimeout:1;              //发动机转速(0CF00400)的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucInstantOilConsumTimeout:1;         //瞬时油耗(18FEF200)的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucOilPressTimeout:1;                 //机油压力(18FEEF00)的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucCoolentWaterTempTimeout:1;         //冷却液温度(18FEEE00)的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucTotalOilConsumTimeout:1;           //总油耗(18FEE900)的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	UCHAR ucBreakeClutchTimeout:1;             //离合器开关/刹车开关(18FEF100)的数据时间溢出标志位，如为1则该ID号所代表的数据都出现故障，为0则正常
	
	UCHAR ucTimeout1:1;                        //Timeout1~24保留
	UCHAR ucTimeout2:1;
	UCHAR ucTimeout3:1;
	UCHAR ucTimeout4:1;
	UCHAR ucTimeout5:1;
	UCHAR ucTimeout6:1;
	UCHAR ucTimeout7:1;
	UCHAR ucTimeout8:1;
	
	UCHAR ucTimeout9:1;
	UCHAR ucTimeout10:1;
	UCHAR ucTimeout11:1;
	UCHAR ucTimeout12:1;
	UCHAR ucTimeout13:1;
	UCHAR ucTimeout14:1;
	UCHAR ucTimeout15:1;
	UCHAR ucTimeout16:1;
	
	UCHAR ucTimeout17:1;
	UCHAR ucTimeout18:1;
	UCHAR ucTimeout19:1;
	UCHAR ucTimeout20:1;
	UCHAR ucTimeout21:1;
	UCHAR ucTimeout22:1;
	UCHAR ucTimeout23:1;
	UCHAR ucTimeout24:1;
}TimeOut;

typedef union tagUTimeOut
{
	DWORD	ulong;
	TimeOut bit;
} St_TimeOut;

//Signal Packet
typedef struct tagPGNData65376_CLUSTER1
{
	UCHAR ucFuelLevel;     //(0x00-0x64)0-100%; 
	UCHAR ucAirPresure1;   //(0-0xFA)0-1000Kpa; 
	UCHAR ucAirPresure2;   //(0-0xFA)0-1000Kpa; 
	UCHAR ucVechileSpeed;  //(0-0xFA)0-250Km/h; 
	DWORD dwODO;           //(0-1E847F)0-1999999Km; 
}PGNData65376_CLUSTER1;


typedef struct tagPGNData65378_CLUSTER2
{
	UCHAR ucData0;          //bit1:驻车信号:0:OFF 1:ON;
	                        //bit2:近光灯信号:0:OFF 1:ON;
						    //bit3:N/A:填充0;
						    //bit4:左蹄片报警信号:0:无报警1:有报警;
						    //bit5:远光灯信号:0:OFF 1: ON;
						    //bit6:右蹄片报警信号:0:无报警1:有报警; 
						    //bit7:前雾灯信号:0:OFF 1:ON;
						    //bit8:后雾灯信号:0:OFF 1:ON; 			
	UCHAR ucData1;          //bit1:左转向信号:0:OFF 1:ON;
                            //bit2:右转向信号:0:OFF 1:ON;
						    //bit3:N/A:以0填充;
						    //bit4:倒车信号:0:OFF 1:ON;
						    //bit5:制动信号:0:OFF 1:ON; 
						    //bit6:N/A:以0填充;
						    //bit7:N/A:以0填充;
						    //bit8:N/A:以0填充;
	UCHAR ucData2;   	    //bit1:雨刮工作信号:0:OFF 1:ON; 
                            //bit2:ASR信号:0:无报警1:有报警;
						    //bit3:小灯信号:0:OFF 1:ON;
						    //bit4:门1开信号:0:OFF 1:ON;
						    //bit5:门2开信号:0:OFF 1:ON; 
						    //bit6:N/A:以0填充;
						    //bit7:N/A:以0填充;
						    //bit8:N/A:以0填充;
	UCHAR ucData3;   	    //bit1:N/A:以0填充;
						    //bit2:N/A:以0填充;
						    //bit3:N/A:以0填充;
						    //bit4:N/A:以0填充;
						    //bit5:空调信号:0:OFF 1: ON;
						    //bit6:除霜器信号:0:无报警1:有报警; 
						    //bit7:N/A:以0填充;
						    //bit8:N/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           :以0填充;
	UCHAR ucData4;  	    //bit1:停车检查符号片:0:OFF 1:ON;
						    //bit2:维修服务符号片:0:OFF 1:ON;
						    //bit3:ABS故障信号:0:OFF 1:ON;
						    //bit4:N/A:以0填充;
						    //bit5:N/A:以0填充;
						    //bit6:N/A:以0填充;
						    //bit7:N/A:以0填充;
						    //bit8:N/A:以0填充;
	UCHAR ucReservedData5;  //0x00,N/A:以0填充;
	UCHAR ucReservedData6;  //0x00,N/A:以0填充;
	UCHAR ucBatteryVoltage; //(0-0x20)0-32V 
}PGNData65378_CLUSTER2;

//Multi Packets
/********************************************************************************/
/*DTC_NB:表示收到的DTC包数                                                      */
/*SA:    数据来源:SA 00 为发动机 Engine                                         */
/*                SA 03 为变速箱 Transmission                                   */
/*                SA 10 为缓速器 Retarder                                       */
/*BYTE1,BYTE2:灯状态信息                                                        */
/*DTC组成:                                                                      */
/*SPN:前19位代表SPN,从右到左的顺序读                                            */
/*FMI:第20位到24位                                                              */
/*SMC:第25位            为常值0                                                 */
/*OC :第26位到第32位    代表故障发生次数                                        */ 
/********************************************************************************/
typedef struct tagTstBits_Engine_DM1_Data
{
	UCHAR ucDTC_NB;
	UCHAR ucSA;
	UCHAR ucByte1;
	UCHAR ucByte2;
	DWORD dwDTC1;
	DWORD dwDTC2;
	DWORD dwDTC3;
	DWORD dwDTC4;
	DWORD dwDTC5;
	DWORD dwDTC6;
	DWORD dwDTC7;
	DWORD dwDTC8;
	DWORD dwDTC9;
	DWORD dwDTC10;
}TstBits_Engine_DM1_Data;

typedef struct tagTstBits_Transmission_DM1_Data
{
	UCHAR ucDTC_NB;
	UCHAR ucSA;
	UCHAR ucByte1;
	UCHAR ucByte2;
	DWORD dwDTC1;
	DWORD dwDTC2;
	DWORD dwDTC3;
	DWORD dwDTC4;
	DWORD dwDTC5;
	DWORD dwDTC6;
	DWORD dwDTC7;
	DWORD dwDTC8;
	DWORD dwDTC9;
	DWORD dwDTC10;
}TstBits_Transmission_DM1_Data;

typedef struct tagTstBits_Retarder_DM1_Data
{
	UCHAR ucDTC_NB;
	UCHAR ucSA;
	UCHAR ucByte1;
	UCHAR ucByte2;
	DWORD dwDTC1;
	DWORD dwDTC2;
	DWORD dwDTC3;
	DWORD dwDTC4;
	DWORD dwDTC5;
	DWORD dwDTC6;
	DWORD dwDTC7;
	DWORD dwDTC8;
	DWORD dwDTC9;
	DWORD dwDTC10;
}TstBits_Retarder_DM1_Data;

typedef struct tagVehicleInfo_APP
{
	PGNData65376_CLUSTER1			stCluster1;
	PGNData65378_CLUSTER2			stCluster2;
	USHORT					        usEngineSpeed;				//(0x00-0xFAFF) *0.125 0-8031.875rpm; 
	USHORT					        usInstantOilConsum;			//(0x00-0xFAFF) *0.05 0-3212.75L/h;
	UCHAR					        ucOilPress;					//(0x00-0xFA) *4 0-1000kPa; 
	UCHAR					        ucCoolentTemp;				//(0x00-0xFA) -40 -40~210摄氏度;
	UCHAR					        ucBrakeClutch;				//刹车开关:bit5-6 00: OFF  01: ON 10: Error 11: N/A;
	                                                            //离合器开关bit7-8 00: OFF  01: ON 10: Error 11: N/A;
	UCHAR					        ucGearSwitchStatus;		    //换挡提示 1:需要换挡,0:不需换挡;
	DWORD					        dwTotalOilConsum;			//(0-0xFAFFFFFF) *0.5 0-2105540607.5L; 
	TstBits_Engine_DM1_Data			stEngineDM1;
	TstBits_Transmission_DM1_Data	stTransmissionDM1;
	TstBits_Retarder_DM1_Data		stRetarderDm1;
	St_TimeOut						stTimeOut;
} VEHICLE_INFO;
/*
void mcp_init();
void mcp_deinit();
*/
const VEHICLE_INFO* GetVehicleInfo();

#ifdef __cplusplus
}
#endif

#endif//VEHICLEINFO_20110121
