////////////////////////////////////////////////////////////////////////////////////////
// 
//	Project:    BT_BlueCap
//	File:       command.cpp
//	Start date: 07.17.2007
//	Update:     
//	Version:    1.0
//	Author:     sunwave
//	Copyright
//
////////////////////////////////////////////////////////////////////////////////////////
//include file
//#include <conio.h>     // for _getch()
#include "StdAfx.h"
#include <string.h>     // for memcmp()
#include "comaccess.h"
#include "HBCP.h"
#include "command.h"
//#include "E9101HardInterface.h"
//#include "../DriveLib/ak4642api.h"

int g_VolLev = 3 ;
int g_CurPairList = 0;
CString strPairList[11];
HWND g_PairList;
//variable define
//RM_BT_DEV BlueToothDevice[MAX_DEV_NUM];
//int	g_BTDeviceNum;
#define	BLUECAP_DEBUG
unsigned char g_TranBuffer[TX_BUF_MAX_SIZE];
unsigned char g_ReceBuffer[RX_BUF_MAX_SIZE];
TCHAR UnicodeBuffer [2*RX_BUF_MAX_SIZE];	//for debug message use
TCHAR m_lpszErrorMessage[256];

ComAccess  *com;
static HANDLE ReadDoneEvent; 
HANDLE ReceDoneEvent; 
HANDLE PairDoneEvent; 
static unsigned int g_read_timeout = SYNC_SIGNALED; /* 0:timeout, 1:sync sucessful */
static unsigned int g_pair_status = SYNC_UNUSED; /* 0:timeout, 1:sync sucessful, 0xff:no use */
unsigned char g_power_status;
//2007-07-31
HBCPSTATE g_hbcpstate = UnInitial;
#define WM_HBCPSTATE			WM_USER + 0x1111
#define WM_HBCPCALLIN			WM_USER + 0x1112
#define WM_HBCPCALLER_NUMBER    WM_USER + 0x1113
#define WM_HBCPRINGIND          WM_USER + 0x1115
#define WM_HBCPOFFHOOK          WM_USER + 0x1116
#define WM_HBCPOUTGOINGCALL     WM_USER + 0x1117
#define WM_HBCPCONNECTED        WM_USER + 0x1118
#define WM_VOL                  WM_USER + 0x1119
#define WM_HBCPACTIVECALL		WM_USER + 0x1120

extern HWND m_MainWnd;
/*--------------------------------------------------------------------------------*/
/*---------------------- Handle events from BLUECORE prototype -------------------*/
/*--------------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Pair_Status(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_Pair_Status
void HBCP_Pair_Status(unsigned char status)
{
#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_PAIR_SUCCESS:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_PAIR_SUCCESS\r\n")));
			SetEvent(PairDoneEvent);
	//		HBCP_Volume_Up();
			break;
		case HBCP_STATUS_PAIR_TIMEOUT:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_PAIR_TIMEOUT\r\n")));
			break;
		case HBCP_STATUS_PAIR_CANCELLED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_PAIR_CANCELLED\r\n")));
			break;
		case HBCP_STATUS_PAIR_FAILED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_PAIR_FAILED\r\n")));
			break;
		case HBCP_STATUS_PAIRING_IN_PROGRESS:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_PAIRING_IN_PROGRESS\r\n")));
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_HsHf_Ind(unsigned char ind)
//
//  Return value: 
//
//  Brief:			HBCP_HsHf_Ind
void HBCP_HsHf_Ind(unsigned char ind)
{
#ifdef BLUECAP_DEBUG
	switch (ind)
	{
		case HBCP_IND_HS_PROFILE:
//			RETAILMSG(TRUE, (TEXT("HBCP_IND_HS_PROFILE\r\n")));
			
			break;
		case HBCP_IND_HF_PROFILE:
//			RETAILMSG(TRUE, (TEXT("HBCP_IND_HF_PROFILE\r\n")));
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Ring_Ind(void)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Ring_Ind
void HBCP_Hshf_Ring_Ind(void)
{
//	RETAILMSG(TRUE, (TEXT("\r\n!!!!!!!!!!!!!HBCP_IND_Enter!!!!!!!!!!!\r\n")));
	::PostMessage(m_MainWnd, WM_HBCPRINGIND, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Set_Storage_Status(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Set_Storage_Status
//  Note:			This feature is supported only by Sony Ericsson and Motorola
void HBCP_Hshf_Set_Storage_Status(unsigned char status)
{
#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_STORAGE_FAILED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_STORAGE_FAILED\r\n")));
			break;
		case HBCP_STATUS_STORAGE_MISSED_CALL:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_STORAGE_MISSED_CALL\r\n")));
			break;
		case HBCP_STATUS_STORAGE_RECEIVED_CALL:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_STORAGE_RECEIVED_CALL\r\n")));
			break;
		case HBCP_STATUS_STORAGE_DIALED_CALL:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_STORAGE_DIALED_CALL\r\n")));
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Volume_Level(unsigned char *pVol)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Volume_Level
void HBCP_Hshf_Volume_Level(unsigned char *pVol)
{
//#ifdef BLUECAP_DEBUG
	
//		RETAILMSG(TRUE, (TEXT("hfp volume = %x\r\n"), *pVol));
//		RETAILMSG(TRUE, (TEXT("av volume = %x\r\n"), *(pVol+1)));

//#endif

//	g_VolLev = pVol[0];
//	SendMessage(HWND_BROADCAST, WM_VOL, WPARAM(pVol), 0);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Call_Status(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Call_Status
void HBCP_Hshf_Call_Status(unsigned char status)
{
#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_CALL_IN_PROGRESS:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_CALL_IN_PROGRESS\r\n")));
			break;
		case HBCP_STATUS_CALL_ESTABLISHED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_CALL_ESTABLISHED\r\n")));
			PostMessage(m_MainWnd, WM_HBCPACTIVECALL, 0, 0);
			break;
		case HBCP_STATUS_CALL_OFFHOOK:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_CALL_OFFHOOK\r\n")));
			::PostMessage(m_MainWnd, WM_HBCPOFFHOOK, 0, 0);
			break;
		case HBCP_STATUS_CALL_FAILED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_CALL_FAILED\r\n")));
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Call_List_Display(unsigned char *pPara)
//
//  Return value: 
//
//  Brief:			HBCP_Call_List_Display
//  Note:			This feature is supported only by Sony Ericsson and Motorola
void HBCP_Call_List_Display(unsigned char *pPara)
{

}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Sco_Ind(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Sco_Ind
void HBCP_Hshf_Sco_Ind(unsigned char status)
{

#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_SCO_DISCONNECTED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_SCO_DISCONNECTED\r\n")));
			break;
		case HBCP_STATUS_SCO_CONNECTED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_SCO_CONNECTED\r\n")));
//			HBCP_Volume_Up();
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Call_Waiting(unsigned char len, unsigned char *p)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Call_Waiting
void HBCP_Hshf_Call_Waiting(unsigned char len, unsigned char *pNum)
{
#ifdef BLUECAP_DEBUG
		//RETAILMSG(TRUE, (TEXT("another caller ID = %s\r\n"), pNum));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Paired_Dev_Name(unsigned char *pDev)
//
//  Return value: 
//
//  Brief:			HBCP_Paired_Dev_Name
void HBCP_Paired_Dev_Name(unsigned char *pDev)
{
//#ifdef BLUECAP_DEBUG
	CString str;
	str = ++pDev;
//	RETAILMSG(TRUE, (TEXT("caller NAME is %s\r\n"), str));
		//RETAILMSG(TRUE, (TEXT("another caller ID = %s\r\n"), pDev));
//#endif

	if( g_CurPairList >= 10)
		return;

	g_CurPairList++;
//	::PostMessage(m_MainWnd, WM_HBCPCALLER_NUMBER, (WPARAM)pID, LPARAM(len));
	strPairList[g_CurPairList] = str;

// #ifdef T7006
// 	strPairList[0] = _T("Complete");
// 	::PostMessage(g_PairList, WM_HBCPPAIRLISTCOMPLETE, g_CurPairList, 0);
//#endif
}

void HBCP_Paired_List_Complete()
{
#ifdef BLUECAP_DEBUG
// 	CString str;
// 	str = ++pDev;
//	RETAILMSG(TRUE, (TEXT("caller NAME is %s\r\n"), str));
		//RETAILMSG(TRUE, (TEXT("another caller ID = %s\r\n"), pDev));
#endif

	strPairList[0] = _T("Complete");
//	::PostMessage(g_PairList, WM_HBCPPAIRLISTCOMPLETE, g_CurPairList, 0);
//	::PostMessage(m_MainWnd, WM_HBCPPAIRLISTCOMPLETE, g_CurPairList, 0);	
	g_CurPairList = 0;

}
////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Caller_Id(unsigned char len, unsigned char *pID)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Caller_Id
void HBCP_Hshf_Caller_Id(unsigned char len, unsigned char *pID)
{
	//RETAILMSG(TRUE, (TEXT("caller ID = %s\r\n"), pID));
	::SendMessage(m_MainWnd, WM_HBCPCALLER_NUMBER, (WPARAM)pID, LPARAM(len));
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_Caller_Name(unsigned char len, unsigned char *pName)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_Caller_Name
void HBCP_Hshf_Caller_Name(unsigned char len, unsigned char *pName)
{
#ifdef BLUECAP_DEBUG
	CString str;
	str = pName;
//	RETAILMSG(TRUE, (TEXT("HBCP_Hshf_Caller_Name is %s\r\n"), str));
		//RETAILMSG(TRUE, (TEXT("caller NAME = %s\r\n"), pName));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_BLUECORE_On_Off(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_BLUECORE_On_Off
void HBCP_BLUECORE_On_Off(unsigned char status)
{
#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_POWER_ON:
			g_power_status = HBCP_STATUS_POWER_ON;
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_POWER_ON\r\n")));
			break;
		case HBCP_STATUS_POWER_OFF:
			g_power_status = HBCP_STATUS_POWER_OFF;
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_POWER_OFF\r\n")));
			break;
	}
#endif
}
 
////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_AV_Status(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_AV_Status
void HBCP_AV_Status(unsigned char status)
{
#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_AV_INITIALISING:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_AV_INITIALISING\r\n")));
			break;
		case HBCP_STATUS_AV_READY:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_AV_READY\r\n")));
			break;
		case HBCP_STATUS_AV_INITIATING:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_AV_INITIATING\r\n")));
			break;
		case HBCP_STATUS_AV_CONNECTED:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_AV_CONNECTED\r\n")));
			break;
		case HBCP_STATUS_AV_STREAMING:
//			RETAILMSG(TRUE, (TEXT("HBCP_STATUS_AV_STREAMING\r\n")));
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_HSHF_Status(unsigned char status)
//
//  Return value: 
//
//  Brief:			HBCP_HSHF_Status
void HBCP_HSHF_Status(unsigned char status)
{
#ifdef BLUECAP_DEBUG
	switch (status)
	{
		case HBCP_STATUS_HSHF_INITIALISING:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_INITIALISING\r\n")));
			break;
		case HBCP_STATUS_HSHF_READY:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_READY\r\n")));
			PostMessage(m_MainWnd, WM_HBCPSTATE, WPARAM(Ready), 0);
			break;
		case HBCP_STATUS_HSHF_CONNECTING:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_CONNECTING\r\n")));
			break;
		case HBCP_STATUS_HSHF_CONNECTED:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_CONNECTED\r\n")));
			PostMessage(m_MainWnd, WM_HBCPCONNECTED, 0, 0);
			break;
		case HBCP_STATUS_HSHF_OUTGOING_CALL:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_OUTGOING_CALL\r\n")));
			PostMessage(m_MainWnd, WM_HBCPOUTGOINGCALL, 0, 0);
			break;
		case HBCP_STATUS_HSHF_INCOMING_CALL:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_INCOMING_CALL\r\n")));
			PostMessage(m_MainWnd, WM_HBCPCALLIN, 0, 0);
			break;
		case HBCP_STATUS_HSHF_ACTIVE_CALL:
	//		RETAILMSG(TRUE, (TEXT("HBCP_STATUS_HSHF_ACTIVE_CALL\r\n")));

			break;
	}
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Checksum_Error(void)
//
//  Return value: 
//
//  Brief:			HBCP_Checksum_Error
void HBCP_Checksum_Error(void)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("HBCP_Checksum_Error\r\n")));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Debug_Msg(void)
//
//  Return value: 
//
//  Brief:			HBCP_Debug_Msg
void HBCP_Debug_Msg(unsigned char len, unsigned char *pMsg)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("HBCP_Debug_Msg\r\n")));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Connected_Dev_Name(unsigned char *pName)
//
//  Return value: 
//
//  Brief:			HBCP_Connected_Dev_Name
void HBCP_Connected_Dev_Name(unsigned char *pName)
{	
#ifdef BLUECAP_DEBUG
	CString str;
	str = pName;
//	RETAILMSG(TRUE, (TEXT("name of the device is %s\r\n"), str));
	//RETAILMSG(TRUE, (TEXT("name of the device is %s\r\n"), pName));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Iinitialized_Complete()
//
//  Return value: 
//
//  Brief:			HBCP_Iinitialized_Complete
void HBCP_Iinitialized_Complete()
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("HBCP_Iinitialized_Complete\r\n")));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Get_Pin(unsigned char *pPin)
//
//  Return value: 
//
//  Brief:			HBCP_Get_Pin
void HBCP_Get_Pin(unsigned char *pPin)
{
#ifdef BLUECAP_DEBUG
	CString str;
	str = pPin;
//	RETAILMSG(TRUE, (TEXT("the pin is %s\r\n"), str));
	//RETAILMSG(TRUE, (TEXT("the pin is %s\r\n"), pPin));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_All_Status(unsigned char *pSts)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_All_Status
//Modify by trengole 2007-7-31
void HBCP_Hshf_All_Status(unsigned char *pSts)
{
	switch(pSts[0]&0xf0)
	{
	case 0x10:
		g_hbcpstate = Ready;
		break;
	case 0x30:
	case 0x40:
	case 0x70:
	case 0x80:
		g_hbcpstate = Connected;
		break;
	default:
		g_hbcpstate = UnInitial;
		break;
	}
	//PostMessage(m_MainWnd, WM_HBCPSTATE, WPARAM(g_hbcpstate), 0);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Hshf_All_Status(unsigned char *pSts)
//
//  Return value: 
//
//  Brief:			HBCP_Hshf_All_Status
void HBCP_Hshf_Inquiry_Result(unsigned char *pSts)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("inquery status: %x\r\n"), *pSts));
//	RETAILMSG(TRUE, (TEXT("device number: %x\r\n"), *(pSts+1)));
//	RETAILMSG(TRUE, (TEXT("bluetooth: %x\r\n"), *(pSts+2)));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_BcPull_Begin(void)
//
//  Return value: 
//
//  Brief:			HBCP_BcPull_Begin
void HBCP_BcPull_Begin(void)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("BC PULL transfer is started\r\n")));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_BcPull_End(void)
//
//  Return value: 
//
//  Brief:			HBCP_BcPull_End
void HBCP_BcPull_End(void)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("BC PULL transfer is completed\r\n")));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_OPP_Filename(pFile)
//
//  Return value: 
//
//  Brief:			HBCP_OPP_Filename
void HBCP_OPP_Filename(unsigned char *pFile)
{
#ifdef BLUECAP_DEBUG
	//RETAILMSG(TRUE, (TEXT("the filename receiving by BlueCore is %s\r\n"), pFile));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_OPP_FileLen(unsigned char len)
//
//  Return value: 
//
//  Brief:			HBCP_OPP_FileLen
void HBCP_OPP_FileLen(unsigned char len)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("the file length receiving by BlueCore is %x\r\n"), len));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Save_OPP_Data(unsigned char *pData)
//
//  Return value: 
//
//  Brief:			HBCP_Save_OPP_Data
void HBCP_Save_OPP_Data(unsigned char *pData)
{
#ifdef BLUECAP_DEBUG
	//RETAILMSG(TRUE, (TEXT("the file data receiving by BlueCore is %s\r\n"), pData));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Spp_Connected(void)
//
//  Return value: 
//
//  Brief:			HBCP_Spp_Connected
void HBCP_Spp_Connected(void)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("HBCP_Spp_Connected\r\n")));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Spp_receive_Data(unsigned char *pData)
//
//  Return value: 
//
//  Brief:			HBCP_Spp_receive_Data
void HBCP_Spp_receive_Data(unsigned char *pData)
{
#ifdef BLUECAP_DEBUG
	//RETAILMSG(TRUE, (TEXT("The contents receiving by BlueCore through SPP: %s\r\n"), pData));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Spp_Disconnected(void)
//
//  Return value: 
//
//  Brief:			HBCP_Spp_Disconnected
void HBCP_Spp_Disconnected(void)
{
#ifdef BLUECAP_DEBUG
//	RETAILMSG(TRUE, (TEXT("HBCP_Spp_Disconnected\r\n")));
#endif
}



/*--------------------------------------------------------------------------------*/
/*-------------------------------- command prototype -----------------------------*/
/*--------------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Dial_Number(unsigned char ucLen, unsigned char * pStr)
//
//  Return value: 
//
//  Brief:			HBCP_Dial_Number
void HBCP_Dial_Number(unsigned char ucLen, unsigned char * pStr)
{
	HBCP_Send_Command(HBCP_CMD_HSHF_DIAL_NUM, ucLen, pStr);
}

void HBCP_Connect_AG_By_Addr( unsigned char ucIndex)
{
	HBCP_Send_Command(HBCP_CMD_HSHF_CONNECT_AG_BY_ADDR, 1, &ucIndex);
}
////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCP_Dial_Number(unsigned char ucLen, unsigned char * pStr)
//
//  Return value: 
//
//  Brief:			HBCP_Dial_Number
void HBCP_Set_Pin(unsigned char ucLen,	unsigned char * pStr)
{
	HBCP_Send_Command(HBCP_CMD_HSHF_SET_PIN, ucLen, pStr);
}


void HBCP_Set_BcName(unsigned char ucLen, unsigned char *pStr)
{
	//HBCP_Send_Command(HBCP_CMD_SET_BC_NAME, ucLen, pStr);
	HBCP_Send_Command(HBCP_CMD_SET_BC_FNAME, ucLen, pStr);
}

void HBCP_Dial_NumberExtra(unsigned char * pStr)
{
	HBCP_Send_Command(HBCP_CMD_HSHF_SEND_DTMF, 1, pStr);
}


////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPInit(char ch)
//
//  Return value: 
//
//  Brief:			HBCP Init
void HBCPInit(void)
{
	int timeout = 5000;

//	RETAILMSG(TRUE,	(TEXT("Class ComAccess Test Application Ver. 1.2\r\nCopyright 2007 all Rights reserved\r\nby sunwave\r\n")));
		
	ReadDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(ReadDoneEvent == INVALID_HANDLE_VALUE) {
//		RETAILMSG(TRUE, (TEXT("Wrong event handle\r\n")));
	}

	ReceDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(ReceDoneEvent == INVALID_HANDLE_VALUE) {
//		RETAILMSG(TRUE, (TEXT("Wrong event handle\r\n")));
	}
	
	PairDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(PairDoneEvent == INVALID_HANDLE_VALUE) {
//		RETAILMSG(TRUE, (TEXT("Wrong event handle\r\n")));
	}
	
	
	
	//
	// check nums arguments
	//
	/*if ( argc < 4 )
	{
		printf("Usage:\n"
		       "comaccess comX stringToSend lenStringToReceive\n"
			   "Press any key!\n");
		_getch();
		return;
	}*/


	//
	// construct the communication device object
	//
	if ( ! (com = new ComAccess()) )
	{
//		RETAILMSG(TRUE, (TEXT("Error: Not enough memory to create ComAccess object!\r\n")));
		return;
	}


	//
	// open and init the communication device with the passed com number
	//
	//if ( ! com->Open(argv[1]) )
	if ( ! com->Open() )
	{
//		RETAILMSG(TRUE, (TEXT("Error: Can't open communication device!\r\n%s\r\n"), /*com->GetErrorMessage()*/m_lpszErrorMessage));
		delete com;
		return;
	}

}


////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPPowerOnOff(char ch)
//
//  Return value: 
//
//  Brief:			HBCP Power On
int HBCPPowerOnOff(void)
{
	int  timeout = 5000;
	
	//HBCPInit();
	//HBCPSYNC();
	//while(!HBCPSYNC());
	//do{
		HBCP_POWER_ON_OFF();
	
		/*ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPPowerOnOff:ERROR:HBCP_EVT_BLUECORE_ON_OFF\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);*/
	//}
	//while(g_ReceBuffer[1] != HBCP_EVT_BLUECORE_ON_OFF);
//	RETAILMSG(TRUE, (TEXT("--HBCPPowerOnOff\r\n")));
	return TRUE;
	
}

/*_inline void PairSyncThrd(int itimeout)
{
	g_pair_status = SYNC_UNUSED;

	int w = WaitForSingleObject(PairDoneEvent, itimeout*1000);
	if (w == WAIT_TIMEOUT) 
	{
		g_pair_status = SYNC_TIMEOUT;
	}
	else
		g_pair_status = SYNC_SIGNALED;

}*/
////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPEnterPair(unsigned char ucTimeoutSec)
//
//  Return value: 
//
//  Brief:			HBCPPair
int HBCPEnterPair(unsigned char ucTimeoutSec)
{

	
	//HBCPSYNC();
	HBCP_Enter_Pair();
	ResetEvent(PairDoneEvent);
	//HANDLE hsync = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PairSyncThrd, (LPVOID)ucTimeoutSec, 0, NULL);
	//if(hsync) {
	//	CloseHandle(hsync);
	//}
	do {
		int w = WaitForSingleObject(PairDoneEvent, ucTimeoutSec*1000);
		if (w == WAIT_TIMEOUT) 
		{
			g_pair_status = SYNC_TIMEOUT;
			return FALSE;		//pair time out
		}
		else
		{
			g_pair_status = SYNC_SIGNALED;
			return TRUE;		//pair success
		}
		/*if (g_pair_status == SYNC_TIMEOUT)
		{
			g_pair_status = SYNC_UNUSED;
			return FALSE;		//pair time out
		}
		else if (g_pair_status == SYNC_SIGNALED)
		{
			g_pair_status = SYNC_UNUSED;
			return TRUE;		//pair success
		}*/
	}while(TRUE);
	CloseHandle(PairDoneEvent);
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPAnswer(char ch)
//
//  Return value: 
//
//  Brief:			HBCP answer incoming call
int HBCPAnswer(void)
{
	int timeout = 2000;
	
	//HBCPSYNC();
	HBCP_Call_Accept();
	/*do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPAnswer:ERROR:HBCP_EVT_HSHF_STATUS--HBCP_STATUS_HSHF_ACTIVE_CALL\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_HSHF_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_HSHF_ACTIVE_CALL));
	
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPAnswer:ERROR:HBCP_EVT_SCO_IND--HBCP_STATUS_SCO_CONNECTED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_SCO_IND) && (g_ReceBuffer[4] != HBCP_STATUS_SCO_CONNECTED));
	
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPAnswer:ERROR:HBCP_EVT_CALL_STATUS--HBCP_STATUS_CALL_ESTABLISHED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_CALL_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_CALL_ESTABLISHED));*/
//	RETAILMSG(TRUE, (TEXT("--HBCPAnswer\r\n")));

	return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPHungUp(char ch)
//
//  Return value: 
//
//  Brief:			HBCP hung up call
int HBCPHungUp(void)
{
	int timeout = 2000;
	
	//HBCPSYNC();
	HBCP_End_Call();
	/*do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPHungUp:ERROR:HBCP_EVT_HSHF_STATUS--HBCP_STATUS_HSHF_CONNECTED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_HSHF_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_HSHF_CONNECTED));
	
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPHungUp:ERROR:HBCP_EVT_CALL_STATUS--HBCP_STATUS_CALL_OFFHOOK\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_CALL_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_CALL_OFFHOOK));
	
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPHungUp:ERROR:HBCP_EVT_SCO_IND--HBCP_STATUS_SCO_DISCONNECTED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_SCO_IND) && (g_ReceBuffer[4] != HBCP_STATUS_SCO_DISCONNECTED));
	RETAILMSG(TRUE, (TEXT("--HBCPHungUp\r\n")));*/

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPReject(char ch)
//
//  Return value: 
//
//  Brief:			HBCP reject incoming call
int HBCPReject(void)
{
	int timeout = 2000;
	
	//HBCPSYNC();
	HBCP_Call_Reject();
	/*do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPHungUp:ERROR:HBCP_EVT_HSHF_STATUS--HBCP_STATUS_HSHF_CONNECTED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_HSHF_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_HSHF_CONNECTED));
	
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPHungUp:ERROR:HBCP_EVT_CALL_STATUS--HBCP_STATUS_CALL_OFFHOOK\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_CALL_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_CALL_OFFHOOK));
	
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPHungUp:ERROR:HBCP_EVT_SCO_IND--HBCP_STATUS_SCO_DISCONNECTED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_SCO_IND) && (g_ReceBuffer[4] != HBCP_STATUS_SCO_DISCONNECTED));*/
//	RETAILMSG(TRUE, (TEXT("--HBCPReject\r\n")));
	return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPDailNum(void)
//
//  Return value: 
//
//  Brief:			HBCP dail a number
int HBCPDailNum(void)
{
	int timeout = 2000;
	
	//HBCPSYNC();
	HBCP_Dial_Number(5, (unsigned char *)"10086");
	/*do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPDailNum:ERROR:HBCP_EVT_HSHF_STATUS--HBCP_STATUS_HSHF_ACTIVE_CALL\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_HSHF_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_HSHF_ACTIVE_CALL));
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPDailNum:ERROR:HBCP_EVT_CALL_STATUS--HBCP_STATUS_CALL_ESTABLISHED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_CALL_STATUS) && (g_ReceBuffer[4] != HBCP_STATUS_CALL_ESTABLISHED));
	do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
		{
			RETAILMSG(TRUE, (TEXT("HBCPDailNum:ERROR:HBCP_EVT_SCO_IND--HBCP_STATUS_SCO_CONNECTED\r\n")));
			return ret;
		}
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while((g_ReceBuffer[1] != HBCP_EVT_SCO_IND) && (g_ReceBuffer[4] != HBCP_STATUS_SCO_CONNECTED));*/

//	RETAILMSG(TRUE, (TEXT("--HBCPDailNum\r\n")));

	return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		HBCPSendDTMF(char ch)
//
//  Return value: 
//
//  Brief:			HBCP send DTMF
void HBCPSendDTMF(unsigned char dtmf)
{
	int timeout = 2000;
	
	HBCP_Send_Command(HBCP_CMD_HSHF_SEND_DTMF, 1, &dtmf);
	/*do{
		ret = Receive(g_ReceBuffer, timeout);
		if (ret == FALSE)
			return ret;
		
		HBCP_Receive(ret, g_ReceBuffer);
	}
	while(TRUE);*/
	//RETAILMSG(TRUE, (TEXT("--HBCPSendDTMF\r\n")));

	//return TRUE;
}

_inline void ReadSyncThrd(unsigned int itimeout)
{

		int w = WaitForSingleObject(ReadDoneEvent, itimeout);
		if (w == WAIT_TIMEOUT) 
		{
			g_read_timeout = SYNC_TIMEOUT;
		}
		else
		{
			g_read_timeout = SYNC_SIGNALED;
		}


}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		Transmit(unsigned char* pt, int len)
//
//  Return value: 
//
//  Brief:			Transmit command to bluetooth
void Transmit(unsigned char* pt, int len)
{

	int			write_result;
	TCHAR		*p = UnicodeBuffer;

	//
	// write the passed string to the communication device
	//
	
	//write_result = com->WriteData(argv[2], strlen(argv[2]));
	memset(UnicodeBuffer, '\0', sizeof(UnicodeBuffer));
	write_result = com->WriteData(pt, len);

	// -1 ? then we got an error and print it
//	if ( write_result < 0 )
//		RETAILMSG(TRUE, (TEXT("%s"), /*com->GetErrorMessage()*/m_lpszErrorMessage));

	//
	// display results
	//
//	RETAILMSG(TRUE, (TEXT("TX[%02d]: \r\n"), write_result));
	MultiByteToWideChar(CP_ACP, 0, (char *)pt, len, UnicodeBuffer, 2*(len));
	//mbstowcs(UnicodeBuffer, pt, len);
//	for (i = 0; i < len; i++)
	{
//		RETAILMSG(TRUE, (TEXT("%02x \r\n"), *(p++)));
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// 
//  Function:		Receive(unsigned char* pr, int timeout)
//
//  Return value: 
//
//  Brief:			Receive command from bluetooth
int Receive(unsigned char* pr, int timeout)
{


	int			i = 0,
				read_result,
				read_times,
				read_result1;
	
	//
	// read incoming data if exist
	//
	memset(pr, '\0', sizeof(g_ReceBuffer));
	memset(UnicodeBuffer, '\0', sizeof(UnicodeBuffer));

	// make sure not to overload ReceBuffer
	read_result = 0;
	read_result1 = 0;
	read_times = 0;
	//int timeout = 20000;

// 	ResetEvent(ReadDoneEvent);
// 	HANDLE hsync = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadSyncThrd, (LPVOID)timeout, 0, NULL);
// 	g_read_timeout = SYNC_SIGNALED;
// 	if(hsync) {
// 		CloseHandle(hsync);
//	}
	do 
	{
		read_result1 = com->ReadData(pr + read_result, 1);

		// -1 ? then we got an error and print it
		if (read_result1 <= 0)
		{
//			RETAILMSG(TRUE, (TEXT("%s\r\n"), /*com->GetErrorMessage()*/m_lpszErrorMessage));
			return 0;
		}
		else// if (read_result1 > 0)
		{
			read_result += read_result1;
		}
		if ((*(pr + read_result - 1) == HBCP_FLAG)
			&& (read_result != 1)
			&& (*(pr + read_result - 2) != HBCP_ESCAPE))
		{
//		SetEvent(ReadDoneEvent);
			break; 
		}
	}
	while(1);	

	if ((g_read_timeout == SYNC_TIMEOUT) && (read_result == 0))
	{
//		CloseHandle(ReadDoneEvent);
		return FALSE;
	}
	// display results
	//
	TCHAR *p = UnicodeBuffer;
	TCHAR *p1 = UnicodeBuffer+4;
	MultiByteToWideChar(CP_ACP, 0, (char *)pr, read_result, UnicodeBuffer, 2*(read_result));
	//mbstowcs(UnicodeBuffer, pr, read_result*2);
//	RETAILMSG(TRUE, (TEXT("                 RX[%02d]: "), 
//		   			read_result)); 
//	for (i = 0; i < read_result; i++)
	{
//		RETAILMSG(TRUE, (TEXT("%02x "), *(p++)));
	}
//	RETAILMSG(TRUE, (TEXT("\r\n                 RX[string]:")));
//	for (i = 0; i < read_result-7; i++)
	{
//		RETAILMSG(TRUE, (TEXT("%c"), *(p1++)));
	}
//	RETAILMSG(TRUE, (TEXT("\r\n")));
//	CloseHandle(ReadDoneEvent);
	return read_result;
}
