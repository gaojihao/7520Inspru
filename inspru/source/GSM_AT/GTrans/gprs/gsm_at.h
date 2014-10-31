
/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\gsm_api.h

 $Reversion History:

        (1) Created Liuhl@2008-03-05 05:30 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  GSM_AT_H
#define  GSM_AT_H


const char*     gsm_api_cmd( unsigned n_type );
const char*     gsm_api_at_cmd( unsigned n_idx, const char* p_param);

enum msg_type_e
{

	CMD_AT_TEXT_ENCODE = 0,   //  set TEXT encode
	CMD_AT_ATE0,              //  'ate0\r'
	CMD_QUERY_SIM,            //  "AT+CPIN?\r"
	CMD_AT_CLIP1,             //  "AT+CLIP=1\r"
	CMD_AT_QSP,               //  "AT+CSQ=?"

	CMD_ACK_STR_OK,           // "OK"
	CMD_ACK_QUERY_SIM,        // "CPIN: READY"
	CMD_ACK_CLCC,             // "+CLCC:"

	
	// ack for calling 
	CMD_ACK_CALL_NO_ANSWER,    // "No Answer"     用户没有接听
	CMD_ACK_CALL_NO_DIALTONE, // "NO DIALTONE"   假如没有拨号音
	CMD_ACK_CALL_NO_CARRIER,  // "NO CARRIER"    如果没有信号
	CMD_ACK_CALL_DISCONNECT,  // "Disconnecting", 被动时,仅在接通后挂断有此消息
	CMD_ACK_CALL_RING,        // "RING\r\n"     , 有新电话进来了

	CMD_ACK_AT_QSP,           // +CSQ: 16,0 ;查询GSM信号的ACK

	
	CMD_NUM
};


//////////////////////////////////////////////////////////////////////////
// All AT COMMAND
//////////////////////////////////////////////////////////////////////////

typedef enum
{
    ATNULL = 0,
	ATCMGD,
    ATCMGF,
	ATCMGL,
	ATCMGR,
	ATCMGS,
	ATCMGW,
	ATCMSS,
	ATCMGC,
	ATCNMI,
	ATCPMS,
	ATCRES,
	ATCSAS,
	ATCSCA, //
	ATCSCB,
	ATCSDH,
	ATCSMP,
	ATCSMS,
	ATSMALPHAID,
	ATSMEXTRAINFO,
	ATSMEXTRAUNSOL,
	ATE,
}enAtcmd;

//////////////////////////////////////////////////////////////////////////

typedef  enum
{
    ATCIPSTART = 22,
    ATCSTT,
	ATCCTT,
	ATCIICR,
	ATCIPSHUT,
	ATCIPC,
	ATCIPCLOSE,
	ATCIPSEND,
	ATCPING,
	ATCIFSR,
	ATCLPORT,
	ATCIPFLP,
	ATCIPSRIP,
	ATCIPDPDP,
	ATCIPSTATUS,
	ATCRESTART,
	ATCIPACT,
	ATCIPCN,
	ATCRCFM,
	ATCIPRET,
}enGprs;

//////////////////////////////////////////////////////////////////////////

typedef enum
{
	ATA = 42,
	ATD,
	ATW,
	ATVTS,
	ATDL,
	ATH,
	ATM,
	ATP,
	ATQ,
	ATS0,
	ATCR,
	ATCRC,

	//The current states of all calls can be easily checked at any time by 
	// using the AT+CLCC command.
	ATCLCC,     
	ATCLVL,
	ATCLIP,     // calling line identification presentation
	ATCOLP,
}enDial;

//////////////////////////////////////////////////////////////////////////

typedef  enum
{
    FIRST_UNREAD_RECORD,
	NEXT_UNREAD_RECORD,
	
	FIRST_READ_RECORD,
	NEXT_READ_RECORD,

	FIRST_STORED_UNSENT_RECORD,
	NEXT_STORED_UNSENT_RECORD,

	FIRST_STORED_SENT_RECORD,
	NEXT_STORED_SENT_RECORD,

	FIRST_USED_RECORD,
	NEXT_USED_RECORD,

	FIRST_NOTMARKED_AS_READ,
	NEXT_NOTMARKED_AS_READ,

	SPECIFIED_SMS_RECORD,
	WRITE_SPECIFIED_SMS,

	SPECIFIED_RECORD_NOTMARKED_AS_READ,
	FIRST_STORED_SENT,
	NEXT_STORED_SENT,

	FIRST_UNREAD_NOTMARKED_AS_READ,
	NEXT_UNREAD_NOTMARKED_AS_READ,			
}enSmsAll;

//////////////////////////////////////////////////////////////////////////

typedef  enum
{
    UNREAD,
	READ,
	UNSENT,
	SENT,
	ALL,
}enSmsType;

//////////////////////////////////////////////////////////////////////////

typedef  enum
{
	GSMMODE,
	GPRSMODE,
}enWorkMode;



#endif //GSM_AT_H