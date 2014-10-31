////////////////////////////////////////////////////////////////////////////////////////
// 
//	Project:    Class ComAccess
//              Overlapped serial IO communication class
//	File:       command.h
//	Start date: 07.17.2007
//	Update:     
//	Version:    1.0
//	Author:     sunwave
//	Copyright ?1997. Alle Rechte vorbehalten
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMAND_H_
#define _COMMAND_H_


//++++++++++++++++++++++++++++++++++++++++++++++
//macro define
//++++++++++++++++++++++++++++++++++++++++++++++
#define	TRUE	1
#define	FALSE	0

#define	SYNC_TIMEOUT	0
#define	SYNC_SIGNALED	1
#define	SYNC_UNUSED		0xff

#define	TX_BUF_MAX_SIZE				256//(4000)
#define	RX_BUF_MAX_SIZE				256//(4000)

#define   MAX_READTIMES	10				

//Volume Level 

//++++++++++++++++++++++++++++++++++++++++++++++
//function
//++++++++++++++++++++++++++++++++++++++++++++++
void HBCPInit(void);
int HBCPPowerOnOff(void);
int HBCPEnterPair(unsigned char ucTimeoutSec);
int HBCPAnswer(void);
int HBCPHungUp(void);
int HBCPReject(void);
int HBCPDailNum(void);
void HBCPSendDTMF(unsigned char dtmf);
//trengole 2007-07-30
enum HBCPSTATE{UnInitial, Ready, Connected};
enum HBCPCALLINGSTATE{CALLING, ENDCALLING};

//++++++++++++++++++++++++++++++++++++++++++++++
//com function
//++++++++++++++++++++++++++++++++++++++++++++++
void Transmit(unsigned char* pt, int len);
int Receive(unsigned char* pr, int timeout);


//++++++++++++++++++++++++++++++++++++++++++++++
//function from blue cap to host
//++++++++++++++++++++++++++++++++++++++++++++++
void HBCP_Pair_Status(unsigned char status);
void HBCP_HsHf_Ind(unsigned char ind);
void HBCP_Hshf_Ring_Ind(void);
void HBCP_Hshf_Set_Storage_Status(unsigned char status);
void HBCP_Hshf_Volume_Level(unsigned char *pVol);
void HBCP_Hshf_Call_Status(unsigned char status);
void HBCP_Call_List_Display(unsigned char *pPara);
void HBCP_Hshf_Sco_Ind(unsigned char status);
void HBCP_Hshf_Call_Waiting(unsigned char len, unsigned char *pNum);
void HBCP_Paired_Dev_Name(unsigned char *pDev);
void HBCP_Paired_List_Complete();
void HBCP_Hshf_Caller_Id(unsigned char len, unsigned char *pID);
void HBCP_Hshf_Caller_Name(unsigned char len, unsigned char *pName);
void HBCP_BLUECORE_On_Off(unsigned char status);
void HBCP_AV_Status(unsigned char status);
void HBCP_HSHF_Status(unsigned char status);
void HBCP_Checksum_Error(void);
void HBCP_Debug_Msg(unsigned char len, unsigned char *pMsg);
void HBCP_Connected_Dev_Name(unsigned char *pName);
void HBCP_Iinitialized_Complete();
void HBCP_Get_Pin(unsigned char *pPin);
void HBCP_Hshf_All_Status(unsigned char *pSts);
void HBCP_Hshf_Inquiry_Result(unsigned char *pSts);
void HBCP_BcPull_Begin(void);
void HBCP_BcPull_End(void);
void HBCP_OPP_Filename(unsigned char *pFile);
void HBCP_OPP_FileLen(unsigned char len);
void HBCP_Save_OPP_Data(unsigned char *pData);
void HBCP_Spp_Connected(void);
void HBCP_Spp_receive_Data(unsigned char *pData);
void HBCP_Spp_Disconnected(void);

#endif // _COMMAND_H_
