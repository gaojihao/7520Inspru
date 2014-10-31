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

/*�����趨�����ʱ������״̬λ����������趨ʱ�����ʾ���ո�ID���ݳ���*/
typedef struct tagTimeOut
{
	UCHAR ucCluster1_Timeout:1;                //18FF6017������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucCluster2_Timeout:1;                //18FF6217������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucEngineSpeedTimeout:1;              //������ת��(0CF00400)������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucInstantOilConsumTimeout:1;         //˲ʱ�ͺ�(18FEF200)������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucOilPressTimeout:1;                 //����ѹ��(18FEEF00)������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucCoolentWaterTempTimeout:1;         //��ȴҺ�¶�(18FEEE00)������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucTotalOilConsumTimeout:1;           //���ͺ�(18FEE900)������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	UCHAR ucBreakeClutchTimeout:1;             //���������/ɲ������(18FEF100)������ʱ�������־λ����Ϊ1���ID������������ݶ����ֹ��ϣ�Ϊ0������
	
	UCHAR ucTimeout1:1;                        //Timeout1~24����
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
	UCHAR ucData0;          //bit1:פ���ź�:0:OFF 1:ON;
	                        //bit2:������ź�:0:OFF 1:ON;
						    //bit3:N/A:���0;
						    //bit4:����Ƭ�����ź�:0:�ޱ���1:�б���;
						    //bit5:Զ����ź�:0:OFF 1: ON;
						    //bit6:����Ƭ�����ź�:0:�ޱ���1:�б���; 
						    //bit7:ǰ����ź�:0:OFF 1:ON;
						    //bit8:������ź�:0:OFF 1:ON; 			
	UCHAR ucData1;          //bit1:��ת���ź�:0:OFF 1:ON;
                            //bit2:��ת���ź�:0:OFF 1:ON;
						    //bit3:N/A:��0���;
						    //bit4:�����ź�:0:OFF 1:ON;
						    //bit5:�ƶ��ź�:0:OFF 1:ON; 
						    //bit6:N/A:��0���;
						    //bit7:N/A:��0���;
						    //bit8:N/A:��0���;
	UCHAR ucData2;   	    //bit1:��ι����ź�:0:OFF 1:ON; 
                            //bit2:ASR�ź�:0:�ޱ���1:�б���;
						    //bit3:С���ź�:0:OFF 1:ON;
						    //bit4:��1���ź�:0:OFF 1:ON;
						    //bit5:��2���ź�:0:OFF 1:ON; 
						    //bit6:N/A:��0���;
						    //bit7:N/A:��0���;
						    //bit8:N/A:��0���;
	UCHAR ucData3;   	    //bit1:N/A:��0���;
						    //bit2:N/A:��0���;
						    //bit3:N/A:��0���;
						    //bit4:N/A:��0���;
						    //bit5:�յ��ź�:0:OFF 1: ON;
						    //bit6:��˪���ź�:0:�ޱ���1:�б���; 
						    //bit7:N/A:��0���;
						    //bit8:N/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           :��0���;
	UCHAR ucData4;  	    //bit1:ͣ��������Ƭ:0:OFF 1:ON;
						    //bit2:ά�޷������Ƭ:0:OFF 1:ON;
						    //bit3:ABS�����ź�:0:OFF 1:ON;
						    //bit4:N/A:��0���;
						    //bit5:N/A:��0���;
						    //bit6:N/A:��0���;
						    //bit7:N/A:��0���;
						    //bit8:N/A:��0���;
	UCHAR ucReservedData5;  //0x00,N/A:��0���;
	UCHAR ucReservedData6;  //0x00,N/A:��0���;
	UCHAR ucBatteryVoltage; //(0-0x20)0-32V 
}PGNData65378_CLUSTER2;

//Multi Packets
/********************************************************************************/
/*DTC_NB:��ʾ�յ���DTC����                                                      */
/*SA:    ������Դ:SA 00 Ϊ������ Engine                                         */
/*                SA 03 Ϊ������ Transmission                                   */
/*                SA 10 Ϊ������ Retarder                                       */
/*BYTE1,BYTE2:��״̬��Ϣ                                                        */
/*DTC���:                                                                      */
/*SPN:ǰ19λ����SPN,���ҵ����˳���                                            */
/*FMI:��20λ��24λ                                                              */
/*SMC:��25λ            Ϊ��ֵ0                                                 */
/*OC :��26λ����32λ    ������Ϸ�������                                        */ 
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
	UCHAR					        ucCoolentTemp;				//(0x00-0xFA) -40 -40~210���϶�;
	UCHAR					        ucBrakeClutch;				//ɲ������:bit5-6 00: OFF  01: ON 10: Error 11: N/A;
	                                                            //���������bit7-8 00: OFF  01: ON 10: Error 11: N/A;
	UCHAR					        ucGearSwitchStatus;		    //������ʾ 1:��Ҫ����,0:���軻��;
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
