/*******************************************************************************
FILE: HBCP.c

DESCRIPTION: APIs for each HBCP command. Event data decoding and reassembling.
*******************************************************************************/

#include "HBCP.h"
#include "command.h"
#include <windows.h>	//for RETAILMSG()
#include <stdio.h>
#include <string.h>     // for memcmp()

#ifdef	WIN32
#pragma pack (1)
#endif

#define SYNC_ENABLE

#define CHECK_SUM
#ifdef CHECK_SUM 
unsigned short gRx_chk_sum =0;		/* sum of all rx data */	
unsigned short gRx_data_sum =0;   /* the sum data sent from BlueCore */
#endif
/*****************************************************************
	External Function & Variable
******************************************************************/
void HardUart_send(unsigned char a)
{
	//UInt32	tx_num=0;
	#ifdef	WIN32
//	UartDrv_Tx(&a,1, &tx_num);
    #endif
}

/*****************************************************************
	Internal Function & Variable
******************************************************************/
typedef /*__packed*/ struct _THBCP_RX_INFO
{
	unsigned int	bSync:1;	// Syncronized flag
	unsigned int	bEsc:1;		// Escaped flag
	unsigned char	ucSeqNum;	// current received sequence number
	unsigned char	nLen;		// package length
}HBCP_RX_INFO;

HBCP_RX_INFO	gHBCP_Rx;
unsigned char	gTxSeq=0;	// tx sequencial number
unsigned char	gRxSeq=0;	// rx sequencial number

//Only increase to certain level
#define TXSEQnum	((++gTxSeq)&0x3f)

unsigned char	gHBCP_Rx_Buf[HBCP_RX_BUF_SIZE];

TX_BUF tx_buf;
unsigned char sync_send, sync_send_num;
unsigned char gHBCP_FLAG = HBCP_FLAG;
unsigned char gHBCP_ESCAPE = HBCP_ESCAPE;
extern HANDLE ReceDoneEvent;
extern unsigned char g_ReceBuffer[];
static int g_init_flag = 0;

void FATAL_ERROR(void);
void HBCP_RcvPacket(void);
void SyncThread(DWORD dwPrameter);
void HBCP_Sync_Ack(void);

/*****************************************************************
	Common Function Definition
******************************************************************/

/* 	Function: 	HBCP_Physical_Send
	Params:		pStr	The pionter of content to be sent
				ucLen	The data length to be escaped and sent out
	return:		none
*/
#define FLAG_START	0x01	/* first bit */
#define FLAG_END	0x02	/* second bit*/
#define FLAG_FULL	0x03
#define FLAG_MID	0x00

#define HBCP_Physical_Send(str,len)			HBCP_Physical_Send_by_flag(str,len,FLAG_FULL);
#define HBCP_Physical_Send_Start(str,len)	HBCP_Physical_Send_by_flag(str,len,FLAG_START);
#define HBCP_Physical_Send_End(str,len)		HBCP_Physical_Send_by_flag(str,len,FLAG_END);
#define HBCP_Physical_Send_Content(str,len)	HBCP_Physical_Send_by_flag(str,len,FLAG_MID);

void HBCP_Physical_Send_Txbuffer(unsigned char *pStr, unsigned char ucLen);
void HBCP_Send_TxBuffer(unsigned char *pStr, unsigned char ucLen);
									
void HBCP_Physical_Send_by_flag(/*__packed*/ unsigned char * pStr, unsigned char	ucLen, unsigned char flag)
{
	unsigned char	ch;
#ifdef CHECK_SUM
	unsigned short sum=0;
	unsigned char sum_higher_byte=0;
	unsigned char sum_lower_byte=0;
#endif
	if (flag & FLAG_START)
		//HardUart_send(HBCP_FLAG);
		Transmit(&gHBCP_FLAG, 1);
	while (ucLen--)
	{
		ch = *(pStr++);
		if ( (ch == HBCP_ESCAPE) || (ch == HBCP_FLAG) )
		{
			//HardUart_send(HBCP_ESCAPE);
			Transmit(&gHBCP_ESCAPE, 1);
			ch ^= 0x20;
		}
#ifdef CHECK_SUM
		sum+=ch;
#endif
		//HardUart_send(ch);	
		Transmit(&ch, 1);	
	}
	
#ifdef CHECK_SUM
	sum = ~sum + 1;/*checksum = ~(Sum of data bytes in data package) + 1*/
	/*
	HardUart_send((unsigned char)((sum&0xff00)>>8));		
	HardUart_send((unsigned char)(sum&0x00ff)); 
	*/
	sum_higher_byte = (unsigned char)((sum&0xff00)>>8);
	sum_lower_byte = (unsigned char)(sum&0x00ff);	
	
	if((sum_higher_byte == HBCP_ESCAPE)||(sum_higher_byte == HBCP_FLAG))
	{
		//HardUart_send(HBCP_ESCAPE);
		Transmit(&gHBCP_ESCAPE, 1);
		sum_higher_byte ^= 0x20;
		//HardUart_send(sum_higher_byte);
		Transmit(&sum_higher_byte, 1);	
	}
	else
	{
		//HardUart_send(sum_higher_byte);	
		Transmit(&sum_higher_byte, 1);	
	}
	
	if((sum_lower_byte == HBCP_ESCAPE)||(sum_lower_byte == HBCP_FLAG))
	{
		//HardUart_send(HBCP_ESCAPE);
		Transmit(&gHBCP_ESCAPE, 1);
		sum_lower_byte ^= 0x20;
		//HardUart_send(sum_lower_byte);	
		Transmit(&sum_lower_byte, 1);	
	}
	else
	{
		//HardUart_send(sum_lower_byte);
		Transmit(&sum_lower_byte, 1);		
	}
	
#endif

	if (flag & FLAG_END)
		//HardUart_send(HBCP_FLAG);
		Transmit(&gHBCP_FLAG, 1);
}

/*****************************************************************
	Host Function Definition
******************************************************************/

/* 	Function: 	HBCP_Send_Command
	Params:		ucCmd	command code with parameters
	return:		none
*/
void HBCP_Send_Command(unsigned char ucCmd, int ucLen, unsigned char *parameter)
{
	/*__packed*/ struct {
		HBCP_HEAD	head;
		char			pbuf[MAX_TX_BUF_LENGTH];
	}data;

	unsigned char		i;

	if(ucLen > MAX_TX_BUF_LENGTH)  ucLen = MAX_TX_BUF_LENGTH;
	
	data.head.ucType	= ucCmd;
	data.head.ucSeqnum	= TXSEQnum;
	data.head.ucLen		= ucLen;

	for (i=0; i<ucLen; i++)
		data.pbuf[i]	= *(parameter++);
	
	// Wake-up: HBCP_Physical_Send((unsigned char *)&data, HBCP_HEAD_SIZE+ucLen);
	HBCP_Send_TxBuffer((unsigned char *)&data, (unsigned char)(HBCP_HEAD_SIZE + ucLen));
}

/* Wake-up */
/* 	Function: 	HBCP_Send_TxBuffer
	Params:		
	return:		none
    Description: Sends command to tx buffer first
*/

void HBCP_Send_TxBuffer(unsigned char *pStr, unsigned char ucLen)
{
	unsigned char	ch;
	unsigned int	i, cnt=0;
#ifdef CHECK_SUM 
	unsigned short sum=0;
	unsigned char sum_higher_byte=0;
	unsigned char sum_lower_byte=0;
#endif
#ifdef SYNC_ENABLE

	DWORD			dwID;
	HANDLE			thread_handle;
	
    /*Fixed problem:Press cmd key more than 30times pc tool no response;Ver 0.82*/
    if (sync_send == 0)
	{
		sync_send = 1;
		thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SyncThread, 0, 0, &dwID);
		if (thread_handle == INVALID_HANDLE_VALUE)
		{
//		  RETAILMSG(TRUE, (TEXT("\n\nCreateThread failed.\n\n")));
		  sync_send = 0;
		  return;
		}
		else
		{
			CloseHandle(thread_handle);
		}
	}

#endif

    for (i=0;i<cnt;i++)
    {
    	if ((tx_buf.data[i]==HBCP_ESCAPE) || (tx_buf.data[i]==HBCP_FLAG))
			cnt ++;	// counting characters needs to be escaped
    }
	
#ifdef CHECK_SUM 
	cnt+=2; /* add 2 byte length for chksum*/
#endif

    if ((tx_buf.length + ucLen + cnt + 2)> MAX_TX_BUF_LENGTH)
    {   
        return;
    }
    
    tx_buf.data[tx_buf.length++]=HBCP_FLAG;

	while (ucLen--)
	{
		ch = *(pStr++);
		if ( (ch == HBCP_ESCAPE) || (ch == HBCP_FLAG) )
		{
			tx_buf.data[tx_buf.length++]=HBCP_ESCAPE;
			ch ^= 0x20;
		}
		tx_buf.data[tx_buf.length++] = ch;	
#ifdef CHECK_SUM 
		sum+=ch; /* data sum */
#endif
	}   
	
#ifdef CHECK_SUM 
	sum = ~sum + 1;/*checksum = ~(Sum of data bytes in data package) + 1*/

	//tx_buf.data[tx_buf.length++]=(unsigned char)((sum & 0xff00)>>8);/* send the higher 8bits firstly*/
	//tx_buf.data[tx_buf.length++]=(unsigned char)(sum & 0x00ff);/* send the lower 8bits */
	
	sum_higher_byte = (unsigned char)((sum&0xff00)>>8);
	sum_lower_byte = (unsigned char)(sum&0x00ff);	
	
	if((sum_higher_byte == HBCP_ESCAPE)||(sum_higher_byte == HBCP_FLAG))
	{
		tx_buf.data[tx_buf.length++]=HBCP_ESCAPE;
		sum_higher_byte ^= 0x20;
		tx_buf.data[tx_buf.length++] = sum_higher_byte;	
	}
	else
	{
		tx_buf.data[tx_buf.length++] = sum_higher_byte;	
	}
	
	if((sum_lower_byte == HBCP_ESCAPE)||(sum_lower_byte == HBCP_FLAG))
	{
		tx_buf.data[tx_buf.length++]=HBCP_ESCAPE;
		sum_lower_byte ^= 0x20;
		tx_buf.data[tx_buf.length++] = sum_lower_byte;	
	}	
	else
	{
		tx_buf.data[tx_buf.length++] = sum_lower_byte;	
	}
	
#endif
    tx_buf.data[tx_buf.length++]=HBCP_FLAG;	

#ifndef SYNC_ENABLE
	HBCP_Sync_Ack();
#endif
}

void HBCP_Physical_Send_Txbuffer(unsigned char *pStr, unsigned char ucLen)
{
    while (ucLen --)
    {
    	//HardUart_send(*(pStr++));
		Transmit(pStr++, 1);
    }
}

void HBCP_Send_Sync(void)
{
	HBCP_HEAD		head;
	
	head.ucType		= HBCP_CMD_SYNC;
	head.ucSeqnum	= TXSEQnum;
	head.ucLen		= 0;
	
	HBCP_Physical_Send((unsigned char *)&head, HBCP_HEAD_SIZE);	
}


/*******************************************************************
	HBCP RX Handler
*******************************************************************/
// BUGHBCP HBCP_EXCEPTION
void InitHBCPState(void)
{
	// Initial HBCP
	gHBCP_Rx.bSync		= 0;	// loss of syn
	gHBCP_Rx.bEsc		= 0;	// not escaped
	gHBCP_Rx.ucSeqNum	= 0xff; // unknow
	gHBCP_Rx.nLen		= 0;
#ifdef CHECK_SUM
	gRx_chk_sum = 0;
	gRx_data_sum = 0;
#endif		
}

_inline void ReceSyncThrd(unsigned int itimeout)
{
	int ret, timeout = 2000;
	
	while(1)
	{
//		RETAILMSG(1,(TEXT("bt:ReceSyncThrd running++++++++++++++++\n")));
		int w = ::WaitForSingleObject(ReceDoneEvent, 0);
		if (w == WAIT_OBJECT_0) 
		{
			return;
		}
		else
		{
			ret = Receive(g_ReceBuffer, timeout);
			if (ret != FALSE)
			{
				HBCP_Receive(ret, g_ReceBuffer);
	//			break;
			}
			//else
			//	g_rece_count++;
		}

		
	}
}

void Init_HBCP(void)
{
	int timeout = 5000;

	//++++++add by sunwave
	if (g_init_flag == 1)
		return;
	HBCPInit();
	ResetEvent(ReceDoneEvent);
	HANDLE hsync = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceSyncThrd, (LPVOID)timeout, 0, NULL);
	if(hsync) {
		CloseHandle(hsync);
	}
	//++++++end add
	// BUGHBCP HBCP_EXCEPTION
	InitHBCPState();

    //initialise transmit buffer
    tx_buf.length = 0;
    memset(tx_buf.data, 0, MAX_TX_BUF_LENGTH);//clear tx buffer to 0
    
    sync_send = 0;
	sync_send_num = 0;
	g_init_flag = 1;
    
}

void Close_Sync_Thread(void)
{
	sync_send = 0;
	sync_send_num = 0;
}


void HBCP_Receive(int ucLen, unsigned char *pStr)
{
	unsigned char	ch;
	
	//++++++add by sunwave
	//Init_HBCP();
	//++++++end add
	while (ucLen--)
	{
		ch = *(pStr++);

		if (!gHBCP_Rx.bSync)
		{
			// No Sync
			if ( ch == HBCP_FLAG)
			{
				// Reach Sync
				gHBCP_Rx.bSync	= 1;
				// Initial context
				gHBCP_Rx.nLen	= 0;
#ifdef CHECK_SUM 
				gRx_chk_sum = 0;
				gRx_data_sum = 0;
#endif
				
			}else
			{
				// Discard any data if not in Sync
			}
		}else
		{
			// Sync
			if (!gHBCP_Rx.bEsc)
			{
				// non escaped
				if (ch == HBCP_FLAG)
				{
					if (gHBCP_Rx.nLen==0)
					{
						// no data at all,
						// we received repeat 0xC0
						// to support wakeup package
						// consider it as a start of package
						// instead of an end of package
						// so, do nothing
					}else
					// found Sync flag again
					// a complete packet was in buffer now
					// notify to application
					{
#ifdef CHECK_SUM		
						//printf("rx_sum_Lower=[%x]\n", gHBCP_Rx_Buf[gHBCP_Rx.nLen-1]);
						//printf("rx_sum_Higher=[%x]\n", gHBCP_Rx_Buf[gHBCP_Rx.nLen-2]);
						
						//printf("gHBCP_Rx.nLen=%d\n", gHBCP_Rx.nLen);
						//printf("sum before=[%x]\n", gRx_chk_sum);
						if(gHBCP_Rx.nLen >= 3)
						{
							gRx_chk_sum-=((unsigned short)gHBCP_Rx_Buf[gHBCP_Rx.nLen-1] + (unsigned short)gHBCP_Rx_Buf[gHBCP_Rx.nLen-2]);

							/* checksum = ~(Sum of data bytes in data package) + 1.*/
							gRx_chk_sum = ~gRx_chk_sum +1;

							gRx_data_sum = (unsigned short)gHBCP_Rx_Buf[--gHBCP_Rx.nLen];
							gRx_data_sum |= (unsigned short)(gHBCP_Rx_Buf[--gHBCP_Rx.nLen] << 8);
							
							// note: unknowen error!!!
							//gRx_data_sum=(unsigned short)gHBCP_Rx_Buf[--gHBCP_Rx.nLen]
							//	+(unsigned short)(gHBCP_Rx_Buf[--gHBCP_Rx.nLen] << 8);
							
							if(!(gRx_chk_sum - gRx_data_sum))
							{
								//printf("sum == rx_sum \n");
								HBCP_RcvPacket();
							}
							else
							{	
//								RETAILMSG(TRUE, (TEXT("rx_sum=[%x]\r\n"), gRx_data_sum));
//								RETAILMSG(TRUE, (TEXT("sum=[%x]\r\n"), gRx_chk_sum));
//								RETAILMSG(TRUE, (TEXT("UART RX Check sum error ! \r\n")));
							}
						}
						else
						{
//							RETAILMSG(TRUE, (TEXT("error ! gHBCP_Rx.nLen < 3 ! \r\n")));
							InitHBCPState();
						}
						
						gRx_chk_sum = 0;
						gRx_data_sum = 0;
						
#else
						HBCP_RcvPacket();
#endif			
						
						// reset flags
						gHBCP_Rx.bSync	= 0;	// lost of sync
						gHBCP_Rx.bEsc	= 0;	// no esc
					}
				}else
				if (ch == HBCP_ESCAPE)
				{
/*				
#ifdef CHECK_SUM 
					gRx_chk_sum+=ch;
#endif				
*/
					// set flag, next character is escaped
					gHBCP_Rx.bEsc	= 1;		// escaped
				}else
				{
#ifdef CHECK_SUM 
					gRx_chk_sum+=ch;
#endif				
					gHBCP_Rx_Buf[gHBCP_Rx.nLen++] = ch;
			
					// BUGHBCP HBCP_EXCEPTION
					// Discard data if too many, reset state machine to initial state
					if (gHBCP_Rx.nLen>(HBCP_RX_BUF_SIZE-2))
						InitHBCPState();
				}
			}else
			{
/*			
#ifdef CHECK_SUM 
				gRx_chk_sum+=ch;
#endif				
*/
				// escaped
				ch ^= 0x20;
#ifdef CHECK_SUM 
				gRx_chk_sum+=ch;
#endif				
				gHBCP_Rx_Buf[gHBCP_Rx.nLen++] = ch;

				// BUGHBCP 
				gHBCP_Rx.bEsc = 0;	// Reset ESCAPE flag

				// BUGHBCP HBCP_FATALERROR
				//Prevention purpose only!
				if ( !((ch==HBCP_ESCAPE)||(ch==HBCP_FLAG)))
				{
					//If the byte after the ESCAPE byte is not our escaped code
					//Simple discard the message, and reset state machine to initial state
					InitHBCPState();
				}
				
				// BUGHBCP HBCP_EXCEPTION
				//Discard data if too many, reset state machine to initial state
				if (gHBCP_Rx.nLen>(HBCP_RX_BUF_SIZE-2))
					InitHBCPState();
			}
			
		}
	}//while
}

void FATAL_ERROR(void)
{
	// BUGHBCP BUG_FATALERROR
	printf(0, "FATA ERROR!!!\r\n");

	while (1) 
	{
	}

}

/*void SyncThread(void)
{
}*/
void SyncThread(DWORD dwParameter)
{
	while(sync_send)
	{
		if(sync_send_num <= 10)
		{
			sync_send_num ++;
			HBCP_Send_Sync();
//			RETAILMSG(TRUE, (TEXT("HBCP_Send_Sync\n")));
			Sleep(250);			
		}
		else
		{
			//indicate sync timeout
			HBCP_Sync_Ack();
		}
	}
}

void HBCP_Sync_Ack(void)
{
	if(tx_buf.length)
		HBCP_Physical_Send_Txbuffer(tx_buf.data, tx_buf.length);
	tx_buf.length = 0;
	Close_Sync_Thread();
}

/******************************************************
debug get call info

*/
void debug_f(unsigned char	*pParam)
{
//	unsigned char ucLen;

//	RETAILMSG(TRUE, (TEXT("\r\n#%x#\r\n"), *(pParam-1)));
//	for(ucLen=0; ucLen<*(pParam-1); ucLen++)
//		RETAILMSG(TRUE, (TEXT("[%x]"), pParam[ucLen]));
//	RETAILMSG(TRUE, (TEXT("\r\n")));
}


void HBCP_RcvPacket()
{
	HBCP_HEAD		*head 	= (HBCP_HEAD *)gHBCP_Rx_Buf;
	unsigned char	*pParam	= (unsigned char *)gHBCP_Rx_Buf + HBCP_HEAD_SIZE;
	int len = (int)((pParam[0] << 24) | (pParam[1] << 16) | (pParam[2] << 8) | pParam[3]);
			 
	/*printf("Receive data len[%d],type[%x]\n", head->ucLen, head->ucType);*/
	
	if (gHBCP_Rx.nLen < HBCP_HEAD_SIZE)
	{
		// BUGOBEX BUG_FATALERROR
		//Fatal error here, just now we got a un-expected package (SMALL PACKAGE)
		//Discard it here to prevent Protocol HANG

		//TODO: add some MMI indication to notice user

		//Won't hange device here, discard the message, and give message
#ifdef	DEBUG_HBCP
		{
			char		temp[12];
			sprintf(temp, "%d ERROR", gHBCP_Rx.nLen);
			tmpAppDbgLine(0,temp);
		}
#endif
		return;
		
		// something wrong
		FATAL_ERROR();
		return;
	}
unsigned char type;
type = head->ucType;
	// Handle events from BLUECORE
	switch (type)
	{
		case HBCP_EVT_PAIR_STATUS:
			 HBCP_Pair_Status(*pParam);
			 break;
			 
		case HBCP_EVT_HSHF_IND:
			 HBCP_HsHf_Ind(*pParam);
			 break;
			
		case HBCP_EVT_RING_IND:
			 HBCP_Hshf_Ring_Ind();
			 break;
			 
		case HBCP_EVT_STORAGE:
			 HBCP_Hshf_Set_Storage_Status(*pParam);
			 break;
			 
		case HBCP_EVT_VOL_LEVEL:
			 HBCP_Hshf_Volume_Level(pParam);
			 break;
			 
		case HBCP_EVT_CALL_STATUS://///////////////////86
			 HBCP_Hshf_Call_Status(*pParam);
			 break;
			
		case HBCP_EVT_CALL_LIST:
			 HBCP_Call_List_Display(pParam);
			 break;

		case HBCP_EVT_SYNC:
			 HBCP_Sync_Ack();
//			 RETAILMSG(TRUE, (TEXT("HBCP_EVT_SYNC\r\n")));
			 break;	
			
		case HBCP_EVT_SCO_IND:
//			 HBCP_Hshf_Sco_Ind(*pParam);
			 break;
			
		case HBCP_EVT_CALL_WAITING_IND:
			 HBCP_Hshf_Call_Waiting(head->ucLen, pParam);
			 break;
			
		case HBCP_EVT_PAIRED_DEV_NAME:
			 //HBCP_Paired_Dev_Name(head->ucLen, pParam);
			 HBCP_Paired_Dev_Name(pParam);
			 break;

		case HBCP_EVT_PAI_LIST_COMPLETE:
			 HBCP_Paired_List_Complete();
			 break;	
			 
		case HBCP_EVT_CALLER_ID:
			 HBCP_Hshf_Caller_Id(head->ucLen, pParam);
			 break;

		case HBCP_EVT_CALLER_NAME:
			 HBCP_Hshf_Caller_Name(head->ucLen, pParam);
			 break;
			 
		case HBCP_EVT_BLUECORE_ON_OFF:
			 HBCP_BLUECORE_On_Off(*pParam);
			 break;

		case HBCP_EVT_AV_STATUS:
			 HBCP_AV_Status(*pParam);
			 break;

		case HBCP_EVT_HSHF_STATUS:
			 HBCP_HSHF_Status(*pParam);
			 break;

		case HBCP_EVT_CHKSUM_ERR:
			 HBCP_Checksum_Error();
			 break;
			 
		case HBCP_EVT_DEBUG:
			 HBCP_Debug_Msg(head->ucLen, pParam);
			 break;
			 
		case HBCP_EVT_ACK:
			 /*HBCP_Sync_Ack();*/
			 //post message to application confirm a command was received from host
//			 RETAILMSG(TRUE, (TEXT("HBCP_EVT_ACK\r\n")));
			 break;

		case HBCP_EVT_CONNECTED_DEV_NAME:/*sxg 20070105*/
			 HBCP_Connected_Dev_Name(pParam);
			 break;
			
		case HBCP_EVT_INITIALIZED_COMPLETE:/*sxg 20070105*/
			 HBCP_Iinitialized_Complete();
			 break;

		case HBCP_EVT_SET_PIN_ACK:
			 HBCP_Get_Pin(pParam);
			 break;	 
			
		/*Xiuge 20070103*/

		case HBCP_EVT_HSHF_GET_STATUS:
			 HBCP_Hshf_All_Status(pParam);
			 break;
			
		case HBCP_EVT_HSHF_INQUIRY_RESULT:
			 HBCP_Hshf_Inquiry_Result(pParam);
			 break;

		/*Jim 20070530*/
		case HBCP_EVT_BC_PULL_BEGIN:
			 HBCP_BcPull_Begin();
			 break;

		case HBCP_EVT_BC_PULL_END:
			 HBCP_BcPull_End();
			 break;

		case HBCP_EVT_OBJ_PUSH_BEGIN:
			 HBCP_BcPull_Begin();
			 break;

		case HBCP_EVT_OBJ_PUSH_END:
			 HBCP_BcPull_End();
			 break;

		case HBCP_EVT_OBJ_PUSH_FNAME:
			 HBCP_OPP_Filename(pParam);
			 break;

		case HBCP_EVT_OBJ_PUSH_FLEN:
			 HBCP_OPP_FileLen(len);
			 break;

		case HBCP_EVT_OBJ_PUSH_DATA:
			 HBCP_Save_OPP_Data(pParam);
			 break;

		case HBCP_EVT_SPP_CONNECTED:
			 HBCP_Spp_Connected();
			 break;

		case HBCP_EVT_SPP_DATA:
			 HBCP_Spp_receive_Data(pParam);
			 break;

		case HBCP_EVT_SPP_DISCONNECTED:
			 HBCP_Spp_Disconnected();
			 break;

		//case HBCP_EVT_NO_PAIRED_DEVICE:
//			 HBCP_No_Paired_Dev();
		//	 break;

		default:
			break;
	}
}

#ifdef	HBCP_DEBUG_CHANNEL
void HBCP_Debug(char *pStr)
{
	HBCP_HEAD		head;
	unsigned char	s1;
	s1	= strlen(pStr);
	
	head.ucType		= HBCP_DBG;
	head.ucSeqnum	= TXSEQnum;
	head.ucLen		= s1;
	
	HBCP_Physical_Send_Start((unsigned char *)&head, HBCP_HEAD_SIZE);
	HBCP_Physical_Send_End((unsigned char *)pStr,s1);
}
#endif

