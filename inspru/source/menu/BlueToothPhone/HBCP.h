/*******************************************************************************
file: HBCP.h
 
Description: header file for Host-BLUECORE Communication Protocol (HBCP)
*******************************************************************************/

#ifndef	_HBCP_H_
#define _HBCP_H_

/*------------------------	Config Entry    ---------------------*/
#ifdef	WIN32
#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __packed
#pragma pack (1)
typedef	unsigned int UInt32;
#define	False	FALSE
#define True	TRUE
#endif
		
// Config parameters
#define HBCP_MAX_PIN_LEN		16	// maximum pin length
#define	HBCP_BD_ADDR_LEN		6	// maximum BD address length
#define HBCP_MAX_PNUM_LEN		16	// maximum phone number length
#define HBCP_MAX_PNAME_LEN		16	// maximum caller name length
#define HBCP_RX_BUF_SIZE		70	// maximum packet size

// HBCP specified character
#define	HBCP_FLAG		(unsigned char)0xC0
#define HBCP_ESCAPE		(unsigned char)0x7e

// HBCP protocol header
#define HBCP_HEAD_SIZE			3	// head size
typedef /*__packed*/ struct _THBCP_HEAD
{
	unsigned char		ucType;		// packet type
	unsigned char		ucSeqnum;	// sequence number
	unsigned char		ucLen;		// length of payload
}HBCP_HEAD;

#define MAX_TX_BUF_LENGTH 40
typedef struct _TX_BUF{
	unsigned char data[MAX_TX_BUF_LENGTH];
	unsigned char length;//length is the number of data bytes in the buffer    
}TX_BUF;

typedef /*__packed*/ struct _THBCP_SET_PIN{
	HBCP_HEAD		head;
	unsigned char	pStrPin[HBCP_MAX_PIN_LEN];
}THBCP_SET_PIN;


/*-------------------------------------------------------------*/
/*------------------------  COMMAND    ------------------------*/
/*-------------------------------------------------------------*/

#define HBCP_CMD_HSHF_ENTER_PAIR 			((unsigned char)0x01)
#define HBCP_CMD_HSHF_EXIT_PAIR				((unsigned char)0x02)
#define HBCP_CMD_HSHF_SET_PIN				((unsigned char)0x03)
#define HBCP_CMD_HSHF_CALL_ACCEPT 			((unsigned char)0x05)
#define HBCP_CMD_HSHF_CALL_REJECT			((unsigned char)0x06)
#define HBCP_CMD_HSHF_VOL_UP				((unsigned char)0x07)
#define HBCP_CMD_HSHF_VOL_DOWN				((unsigned char)0x08)
#define HBCP_CMD_HSHF_DIAL_LAST				((unsigned char)0x09)
#define HBCP_CMD_HSHF_DIAL_NUM 				((unsigned char)0x0A)
#define HBCP_CMD_HSHF_DIAL_VOICE			((unsigned char)0x0B)
#define HBCP_CMD_HSHF_CALL_END				((unsigned char)0x0C)
#define HBCP_CMD_HSHF_GET_LIST				((unsigned char)0x0D)
#define HBCP_CMD_HSHF_SET_STORAGE			((unsigned char)0x11)
#define HBCP_CMD_HSHF_SEND_DTMF	  			((unsigned char)0x12)
#define HBCP_CMD_HSHF_INQUIRY 	  		((unsigned char)0x16)/*sxg 20070105*/
#define HBCP_CMD_HSHF_CANCEL_INQUIRY	((unsigned char)0x17)/*sxg 20070105*/
#define HBCP_CMD_HSHF_GET_STATUS			((unsigned char)0x18)
#define HBCP_CMD_HSHF_GET_CONNECTED_DEVICE_NAME		((unsigned char)0x19)/*sxg 20070105*/

#define HBCP_CMD_SYNC						((unsigned char)0x25)
#define HBCP_CMD_HSHF_BLUECORE_ON_OFF			((unsigned char)0x27)
#define HBCP_CMD_HSHF_CONNECT				((unsigned char)0x2A)
#define HBCP_CMD_HSHF_DISCONNECT			((unsigned char)0x2B)
#define HBCP_CMD_HSHF_CALL_TRANS_TO_AG		((unsigned char)0x2C)
#define HBCP_CMD_HSHF_CALL_TRANS_TO_HF		((unsigned char)0x2D)
#define HBCP_CMD_HSHF_DELETE_PAIR			((unsigned char)0x31)
#define HBCP_CMD_HSHF_CONNECT_AG_BY_ADDR 	((unsigned char)0x13)
#define HBCP_CMD_HSHF_CALL_SWAP 			((unsigned char)0x10)
#define HBCP_CMD_HSHF_REL_ACT_ACC_OTH 		((unsigned char)0x2E)
#define HBCP_CMD_HSHF_REL_HOL_REJ_WAIT		((unsigned char)0x2F)
#define HBCP_CMD_HSHF_ADD_HOLD_CALL			((unsigned char)0x30)
#define HBCP_CMD_HSHF_MUTE 					((unsigned char)0x3A)
#define HBCP_CMD_HSHF_GET_PAIR_DEV_NAME		((unsigned char)0x3C)
#define HBCP_CMD_HSHF_RESTORE_FACTORY	 	((unsigned char)0x3D)

#define HBCP_CMD_AV_STOP_AUDIO				((unsigned char)0x44)
#define HBCP_CMD_AV_PLAY_PAUSE_AUDIO		((unsigned char)0x45)
#define HBCP_CMD_AV_PREVIOUS_AUDIO			((unsigned char)0x46)
#define HBCP_CMD_AV_NEXT_AUDIO				((unsigned char)0x47)

#define HBCP_CMD_SET_BC_NAME		((unsigned char)0x50)
#define HBCP_CMD_SET_BC_FNAME	((unsigned char)0x51)
#define HBCP_CMD_SET_BC_TITLE		((unsigned char)0x52)
#define HBCP_CMD_SET_BC_BIRTH		((unsigned char)0x53)
#define HBCP_CMD_SET_BC_ADDR		((unsigned char)0x54)
#define HBCP_CMD_SET_BC_ORG		((unsigned char)0x55)
#define HBCP_CMD_SET_BC_URL		((unsigned char)0x56)
#define HBCP_CMD_SET_BC_TEL		((unsigned char)0x57)
#define HBCP_CMD_SET_BC_MOBILE	((unsigned char)0x58)
#define HBCP_CMD_SET_BC_FAX		((unsigned char)0x59)
#define HBCP_CMD_SET_BC_EMAIL		((unsigned char)0x5a)
#define HBCP_CMD_SET_BC_TZ		((unsigned char)0x5b)
#define HBCP_CMD_SET_BC_REV		((unsigned char)0x5c)

#define HBCP_CMD_CONNECT_SPP_BY_ADDR		((unsigned char)0x60)
#define HBCP_CMD_CONNECT_LAST_SPP	((unsigned char)0x61)
#define HBCP_CMD_DISCONNECT_SPP		((unsigned char)0x62)
#define HBCP_CMD_SPP_DATA		((unsigned char)0x63)

/*------------------------	EVENT    --------------------------*/
#define HBCP_EVT_PAIR_STATUS				((unsigned char)0x81)
#define HBCP_EVT_HSHF_IND					((unsigned char)0x82)
#define HBCP_EVT_RING_IND					((unsigned char)0x83)
#define HBCP_EVT_STORAGE					((unsigned char)0x84)
#define HBCP_EVT_VOL_LEVEL					((unsigned char)0x85)
#define HBCP_EVT_CALL_STATUS				((unsigned char)0x86)
#define HBCP_EVT_CALL_LIST					((unsigned char)0x87)
#define HBCP_EVT_SYNC						((unsigned char)0xA4)
#define HBCP_EVT_PAI_LIST_COMPLETE			((unsigned char)0xA6)//add
#define HBCP_EVT_SCO_IND					((unsigned char)0xA8)
#define HBCP_EVT_CALL_WAITING_IND			((unsigned char)0xAB)
#define HBCP_EVT_PAIRED_DEV_NAME 			((unsigned char)0xB4)
#define HBCP_EVT_NO_PAIRED_DEVICE			((unsigned char)0xA5)
#define HBCP_EVT_CONNECTED_DEV_NAME		((unsigned char)0xB5)/*sxg 20070105*/
#define HBCP_EVT_INITIALIZED_COMPLETE		((unsigned char)0xB6)/*sxg 20070105*/
#define HBCP_EVT_SET_PIN_ACK				((unsigned char)0xB7)/*sxg 20070108*/
#define HBCP_EVT_HSHF_GET_STATUS			((unsigned char)0xB8)/*sxg 20070108*/
#define HBCP_EVT_HSHF_INQUIRY_RESULT		((unsigned char)0xB9)/*sxg 20070108*/


#define HBCP_EVT_CALLER_ID					((unsigned char)0xB1)
#define HBCP_EVT_CALLER_NAME				((unsigned char)0xB2)
#define HBCP_EVT_BLUECORE_ON_OFF				((unsigned char)0xB3)
#define HBCP_EVT_AV_STATUS					((unsigned char)0xD0)
#define HBCP_EVT_HSHF_STATUS				((unsigned char)0xD1)
#define HBCP_EVT_CHKSUM_ERR					((unsigned char)0xFD)/* Add uart checksum error event */
#define HBCP_EVT_DEBUG						((unsigned char)0xFE)
#define HBCP_EVT_ACK						((unsigned char)0xFF)

#define HBCP_EVT_SPP_CONNECTED			((unsigned char)0xe1)
#define HBCP_EVT_SPP_DATA				((unsigned char)0xe2)
#define HBCP_EVT_SPP_DISCONNECTED			((unsigned char)0xe3)

#define HBCP_EVT_BC_PULL_BEGIN	((unsigned char)0xf6)
#define HBCP_EVT_BC_PULL_END		((unsigned char)0xf7)
#define HBCP_EVT_OBJ_PUSH_BEGIN	((unsigned char)0xf8)
#define HBCP_EVT_OBJ_PUSH_END		((unsigned char)0xf9)
#define HBCP_EVT_OBJ_PUSH_FNAME	((unsigned char)0xfa)
#define HBCP_EVT_OBJ_PUSH_FLEN	((unsigned char)0xfb)
#define HBCP_EVT_OBJ_PUSH_DATA	((unsigned char)0xfc)


// Debug
#define HBCP_DBG							((unsigned char)0xFE)
// ACK
#define HBCP_ACK							((unsigned char)0xFF)

/*-----------------------------------------------------------*/
/*------------------------	Event status	 ----------------*/
/*-----------------------------------------------------------*/

/*--------------	HSHF pair status	  -------------------*/
#define HBCP_STATUS_PAIR_SUCCESS			(unsigned char)0x00
#define HBCP_STATUS_PAIR_TIMEOUT			(unsigned char)0x01
#define HBCP_STATUS_PAIR_CANCELLED			(unsigned char)0x02
#define HBCP_STATUS_PAIR_FAILED 			(unsigned char)0x03
#define HBCP_STATUS_PAIRING_IN_PROGRESS 	(unsigned char)0x04
/*--------------	HSHF profile    	  -------------------*/
#define HBCP_IND_HS_PROFILE					(unsigned char)0x01
#define HBCP_IND_HF_PROFILE					(unsigned char)0x02
/*--------------	HSHF set storage	  -------------------*/
#define HBCP_STATUS_STORAGE_FAILED			(unsigned char)0x00
#define HBCP_STATUS_STORAGE_MISSED_CALL 	(unsigned char)0x01
#define HBCP_STATUS_STORAGE_RECEIVED_CALL	(unsigned char)0x02
#define HBCP_STATUS_STORAGE_DIALED_CALL 	(unsigned char)0x03
/*--------------	HSHF call status	   -------------------*/
#define HBCP_STATUS_CALL_IN_PROGRESS		(unsigned char)0x00
#define HBCP_STATUS_CALL_ESTABLISHED		(unsigned char)0x01
#define HBCP_STATUS_CALL_OFFHOOK			(unsigned char)0x02
#define HBCP_STATUS_CALL_FAILED 			(unsigned char)0x03
/*--------------	HSHF Sco status	   -------------------*/
#define HBCP_STATUS_SCO_DISCONNECTED		(unsigned char)0x00
#define HBCP_STATUS_SCO_CONNECTED			(unsigned char)0x01
/*-----------------------------------------------------------*/
#define HBCP_STATUS_POWER_OFF				(unsigned char)0x00
#define HBCP_STATUS_POWER_ON				(unsigned char)0x01
/*--------------	AV status			 --------------------*/
#define HBCP_STATUS_AV_INITIALISING 		(unsigned char)0x00
#define HBCP_STATUS_AV_READY				(unsigned char)0x01
#define HBCP_STATUS_AV_INITIATING			(unsigned char)0x02
#define HBCP_STATUS_AV_CONNECTED			(unsigned char)0x03
#define HBCP_STATUS_AV_STREAMING			(unsigned char)0x04
/*--------------	HSHF status 		   ------------------*/
#define HBCP_STATUS_HSHF_INITIALISING		(unsigned char)0x00
#define HBCP_STATUS_HSHF_READY				(unsigned char)0x01
#define HBCP_STATUS_HSHF_CONNECTING 		(unsigned char)0x02
#define HBCP_STATUS_HSHF_CONNECTED			(unsigned char)0x03
#define HBCP_STATUS_HSHF_OUTGOING_CALL		(unsigned char)0x04
#define HBCP_STATUS_HSHF_INCOMING_CALL		(unsigned char)0x05
#define HBCP_STATUS_HSHF_ACTIVE_CALL		(unsigned char)0x06
/*--------------	Volume status		---------------------*/
#define HBCP_STATUS_HFP_VOLUME				(unsigned char)0x00
#define HBCP_STATUS_AV_VOLUME				(unsigned char)0x01

/*---------------------- HBCP prototype ----------------------*/
/*------------------------------------------------------------*/
/*---------------------- HBCP prototype ----------------------*/

void Init_HBCP(void);
void HBCP_Receive(int	ucLen, unsigned char *pStr);
void HBCP_Send_Command(unsigned char ucCmd, int ucLen, unsigned char *parameter);
void Close_Sync_Thread(void);
#ifdef	HBCP_DEBUG_CHANNEL
void HBCP_Debug(char *pStr);	// print a NULL terminated string on debug channel
#endif
void PRINT(unsigned char *str);


/*------------------------------------------------------------*/
/*---------------------- command prototype -------------------*/
/*------------------------------------------------------------*/

//void HBCP_Enter_Pair( unsigned char ucTimeoutSec);
void HBCP_Set_Pin(unsigned char ucLen,	unsigned char * pStr);
void HBCP_Dial_Number(unsigned char ucLen, unsigned char * pStr);
void HBCP_Dial_NumberExtra(unsigned char * pStr);
void HBCP_Get_CallList(unsigned char ucLen, unsigned char * pStr);
void HBCP_Set_storage(unsigned char ucListType);
//void HBCP_Send_DTMF(unsigned char dtmf);
void HBCP_Connect_AG_By_Addr( unsigned char ucIndex);
void HBCP_Send_Sync(void);
void HBCP_Set_Inquiry(unsigned char ucTimeoutSec);/*sxg 20070105*/
void HBCP_Set_BcName(unsigned char ucLen, unsigned char *pStr);
//++++++add by sunwave
#define HBCP_Sync()					HBCP_Send_Command(HBCP_CMD_SYNC, 0, 0)
#define HBCP_Enter_Pair()			HBCP_Send_Command(HBCP_CMD_HSHF_ENTER_PAIR, 0, 0)
//#define HBCP_Dial_Num()			HBCP_Send_Command(HBCP_CMD_HSHF_DIAL_NUM, 5, (unsigned char *)"10086")

//++++++end add
#define HBCP_Exit_Pair()			HBCP_Send_Command(HBCP_CMD_HSHF_EXIT_PAIR, 0, 0)
#define HBCP_Call_Accept()		 	HBCP_Send_Command(HBCP_CMD_HSHF_CALL_ACCEPT, 0, 0)
#define HBCP_Call_Reject()		 	HBCP_Send_Command(HBCP_CMD_HSHF_CALL_REJECT, 0, 0)
#define HBCP_Volume_Up()		 	HBCP_Send_Command(HBCP_CMD_HSHF_VOL_UP, 0, 0)
#define HBCP_Volume_Down()		 	HBCP_Send_Command(HBCP_CMD_HSHF_VOL_DOWN, 0, 0)
#define HBCP_Dial_Last()		 	HBCP_Send_Command(HBCP_CMD_HSHF_DIAL_LAST, 0, 0)
#define HBCP_Dial_Voice()		 	HBCP_Send_Command(HBCP_CMD_HSHF_DIAL_VOICE, 0, 0)
#define HBCP_End_Call() 		 	HBCP_Send_Command(HBCP_CMD_HSHF_CALL_END, 0, 0)
#define HBCP_POWER_ON_OFF() 		HBCP_Send_Command(HBCP_CMD_HSHF_BLUECORE_ON_OFF, 0, 0)
#define HBCP_HSHF_Connect() 	 	HBCP_Send_Command(HBCP_CMD_HSHF_CONNECT, 0, 0)
#define HBCP_HSHF_Disconnect()	 	HBCP_Send_Command(HBCP_CMD_HSHF_DISCONNECT, 0, 0)
#define HBCP_Trans_Call_to_AG()	 	HBCP_Send_Command(HBCP_CMD_HSHF_CALL_TRANS_TO_AG, 0, 0)
#define HBCP_Trans_Call_to_HF()	 	HBCP_Send_Command(HBCP_CMD_HSHF_CALL_TRANS_TO_HF, 0, 0)
#define HBCP_Delete_Pair()		 	HBCP_Send_Command(HBCP_CMD_HSHF_DELETE_PAIR, 0, 0)
#define HBCP_Call_Swap()	 	 	HBCP_Send_Command(HBCP_CMD_HSHF_CALL_SWAP, 0, 0)
#define HBCP_Rel_Act_Acc_Wait()  	HBCP_Send_Command(HBCP_CMD_HSHF_REL_ACT_ACC_OTH, 0, 0)
#define HBCP_Rel_Hold_Rej_Wait() 	HBCP_Send_Command(HBCP_CMD_HSHF_REL_HOL_REJ_WAIT, 0, 0)
#define HBCP_Add_Hold() 			HBCP_Send_Command(HBCP_CMD_HSHF_ADD_HOLD_CALL, 0, 0)
#define HBCP_Mute_Mic() 		 	HBCP_Send_Command(HBCP_CMD_HSHF_MUTE, 0, 0)
#define HBCP_Get_Paired_Dev_Name()	HBCP_Send_Command(HBCP_CMD_HSHF_GET_PAIR_DEV_NAME, 0, 0)
#define HBCP_Restore_Factory()     	HBCP_Send_Command(HBCP_CMD_HSHF_RESTORE_FACTORY, 0, 0)
/*sxg 20070103*/
#define HBCP_HSHF_Get_Status()     	HBCP_Send_Command(HBCP_CMD_HSHF_GET_STATUS, 0, 0)
#define HBCP_Cancel_Inquiry()			 HBCP_Send_Command(HBCP_CMD_HSHF_CANCEL_INQUIRY, 0, 0)
#define HBCP_Get_Connected_DEV_Name()  HBCP_Send_Command(HBCP_CMD_HSHF_GET_CONNECTED_DEVICE_NAME, 0, 0)

#define HBCP_Stop_Audio() 			HBCP_Send_Command(HBCP_CMD_AV_STOP_AUDIO, 0, 0)
#define HBCP_Play_Pause_Audio() 	HBCP_Send_Command(HBCP_CMD_AV_PLAY_PAUSE_AUDIO, 0, 0)
#define HBCP_Previous_Audio()		HBCP_Send_Command(HBCP_CMD_AV_PREVIOUS_AUDIO, 0, 0)
#define HBCP_Next_Audio()			HBCP_Send_Command(HBCP_CMD_AV_NEXT_AUDIO, 0, 0)

#define HBCP_SPP_Connect() 	 		HBCP_Send_Command(HBCP_CMD_CONNECT_LAST_SPP, 0, 0)
#define HBCP_SPP_Disconnect()	 	HBCP_Send_Command(HBCP_CMD_DISCONNECT_SPP, 0, 0)




void HBCP_Connect_SPP_By_Addr( unsigned char ucIndex);


#endif //_HBCP_H_

