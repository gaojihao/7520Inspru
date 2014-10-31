// GSMLogic.h: interface for the CGSMLogic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GSMLOGIC_H__3676D002_A87B_4889_8011_F16FB050C982__INCLUDED_)
#define AFX_GSMLOGIC_H__3676D002_A87B_4889_8011_F16FB050C982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gsm_msg_def.h"
#include "msg_ipc.h"
#include "logic_data.h"

#define  SMS_PATH_INBOX          "\\Flashdrv Storage\\menu\\inbox.sms"
#define  SMS_PATH_DISPATCH_INFO  "\\Flashdrv Storage\\menu\\dispatch.sms"
#define  SMS_PATH_OUTBOX         "\\Flashdrv Storage\\menu\\outbox.sms"
#define  SMS_PATH_DRAFT          "\\Flashdrv Storage\\menu\\draft.sms"
#define  PBOOK_PATH_DEVICE       "\\Flashdrv Storage\\menu\\pbook.sms"

class CGSMLogic  
{
public:
	CGSMLogic();
	virtual ~CGSMLogic();

	void    InitFindSrv( HWND hOwner );
	void    GSMSrvShutDown( bool b_sync );
	
	void    GSMOpen()          { SendGSMCMD( GSM_CMD_OPEN, 0 );    }
	void    GSMInit()          { SendGSMCMD( GSM_CMD_INIT, 0, TRUE); }
	void    GSMClose()         { SendGSMCMD( GSM_CMD_CLOSE, 1 );   }
	void    GSMSuspend()       { SendGSMCMD( GSM_CMD_SUSPEND, FALSE);  }
	void    GSMResume()        { SendGSMCMD( GSM_CMD_RESUME, 0 ); }
	void    GSMReFindHwnd()    { SendGSMCMD( GSM_CMD_REFIND_WND,TRUE);}
	void    GSMWritePbook()    { SendGSMCMD( GSM_CMD_ADD_SIM_PBOOK,0,FALSE); }

	void    SetPhoneNum( CString& strPhone );
	void    SetPhoneExt( CString& strExt );
	void    SetMsgBuf( CString&  strMsg );
	void    SetSMSList(SM_PARAM* p_list, int nCount);
	void    SetPBookList(PBOOK_ITEM* p_list, int nCount);

	void    GSMSendSMS()        { SendGSMCMD(GSM_CMD_SENDMSG, 0,false );    }
	void    GSMDial()           { SendGSMCMD(GSM_CMD_DIAL, 0, TRUE);       }
	void    GSMListen()         { SendGSMCMD(GSM_CMD_LISTEN,0,TRUE);       }
	void    GSMDialExt()        { SendGSMCMD(GSM_CMD_DIAL_EXT, 0, false);   }
	void    GSMHangup()         { SendGSMCMD(GSM_CMD_HANGUP, 0, false);     }
	void    GSMCheckLine()      { SendGSMCMD(GSM_CMD_CHECK_LINE, 0, false); }
	void    GSMSetVol(int nVel);
	void    GSMAccepted()       { SendGSMCMD(GSM_CMD_ACCEPTED,0, false);    }

	void    GSMReadSIMSMS()       { SendGSMCMD(GSM_SIM_SMS_READ, 0, FALSE); }
	void    GSMQueryGSMNet()      { SendGSMCMD(GSM_QUERY_NET,0, TRUE);}
	bool    GSMIsReadSIM();
	void    GSMDelSIMSMS(int idx);
	void    GSMDelSIMSMSAll();
	void    GSMSetNet(int nSel, int nType);


	int     GSMGetSIMUsed()        { return m_nSimSMSUsed; }
	BOOL    IsSMSHasMaxOnSIM();  
	SM_PARAM*  GSMGetList()        { return m_SMSList;     }

	PBOOK_ITEM* GSMGetSIMPBookList() { return m_PBookList; }
	PBOOK_ITEM* GSMGetSIMPBookRd(int nRdx);
	BOOL        SchPBook(CString& strPhone, CString& strName);

	int     GSMGetPBookUsed()     { return m_nPBookUsed; }
	void    GSMReadSIMPBook()     {  SendGSMCMD(GSM_SIM_PBOOK_READ, 0, FALSE); }
	BOOL    GSMIsReadSIMPBook();
	BOOL    GSMAddSIMPBook( const char* p_name, const char* p_phone );
	BOOL    GSMUpdateSIMPBook(PBOOK_ITEM* pItem);
	void    GSMDelSIMPBook(int idx);
	void    GSMDelSIMPBookAll( );
	int     GSMFindUnUsedIdx();

	
	void    onGsmAckCmd(UINT GsmCmd, int nParam);
	int     onGsmDataCopy(WPARAM wParam, LPARAM lParam);
	PCSTR	ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator = ',');
	
	UINT    GetSMSSignal() { return m_GsmCardState; }
	BOOL    GetSysState()  { return m_bSystemState; }
	BOOL    GetSIMState()  { return m_bSimState;    }
	UINT    GetGSMNetType(){ return m_nGSMNet;      }
	logic_data  m_inbox;
	logic_data  m_outbox;
	logic_data  m_draft;
	logic_data	m_dispatch;	//调度信息
	logic_pbook  m_pbook; //本机电话薄
	int		m_iUnRdMsg;
	void	SetLoginState(int iState);
	int		m_iState;

	void     UserSaveMsg(CString& strMsg); //保存到草稿箱
	void     UserSendMsg(CString& strPhone, CString& strMsg);
	void     UserNavi()  { send_2_navi(m_LongSec, m_LatSec); }
	void     SMSNavi()   { Send2TNC(m_tnc); } 
	void     OnKeyNaviForHuaWei( );

	CString  GetCallingPhone();

	void	 GetUnRdMsg();  //获取未读短信条数
	void     GSMReportSignal(int nVal);

protected:

	static BOOL IsOnlyASSIC(CString str);
	void   Send2TNC(char *destpos);
	void   past_cld_TNC (const char* p_data, int n_len );
	

	void   SaveUserLongLat(const char* pStr);
	HWND   find_kld_hwnd();
	void   send_2_navi(int n_long, int n_lat);
	void   past_cld_navigation(const char* p_data, int n_len );


protected:
	void    SendGSMCMD(unsigned n_cmd, int n_param = 0, bool b_sync = true );

	short     m_nSimSMSUsed;  //SIM卡短消息个数
	short     m_nSimSMSMax;
	short     m_nPBookUsed;
	short     m_nPbookMax;
	SM_PARAM*    m_SMSList;       //SIM卡短消息内容
	PBOOK_ITEM*  m_PBookList;     //SIM上的电话薄
	bool      m_bSendMsgOK;
	msg_ipc   m_ipc;
	UINT      m_GsmCardState;
	BOOL      m_bSimState;    // TRUE,有SIM卡； FALSE,没有SIM卡
	BOOL      m_bSystemState; //系统初始化标志，默认为成功
	int       m_LongSec;
	int       m_LatSec;
	int       m_nGSMNet;
	char      m_CallingNum[32];
	char      m_tnc[256];
};

#endif // !defined(AFX_GSMLOGIC_H__3676D002_A87B_4889_8011_F16FB050C982__INCLUDED_)
