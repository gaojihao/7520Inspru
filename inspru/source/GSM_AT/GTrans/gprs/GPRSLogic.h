// GPRSLogic.h: interface for the CGPRSLogic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPRSLOGIC_H__7026C9CC_0498_44BA_B3CC_3905A925AF09__INCLUDED_)
#define AFX_GPRSLOGIC_H__7026C9CC_0498_44BA_B3CC_3905A925AF09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ex_basics.h"
#include "gsm_parse.h"
#include "../GSM/msg_ipc.h"

class CGPRSLogic  
{
public:
	virtual ~CGPRSLogic();
	static CGPRSLogic* Instance();

	// Init function
public:

	void     CloseGSM();
	void     ResetGSM();
	

	BOOL     InitWathing( HWND hNotifyWnd , msg_ipc* p_ipc, int nGSMPort=0);
	void     UnInit();
	void     SetServerInfo( const char* pIP, int nPort, const char* IpType,int nSlice = 10 );

	void       SetState(unsigned nState) { m_state = nState; }
	unsigned    GetState()                { return m_state;   }
	BOOL       IsLogin()                 { return m_state == eGState_LoginedSvr; }

	void   GSMSetCalling(BOOL bCall);//     { m_gsm_state.gsm_is_dialing = bCall; }
	BOOL   GSMIsCalling()                { return m_gsm_state.gsm_is_dialing;  }

public:
	void   GCDM_QuerySignal();
	char*  GCMD_IMEI();
	void   GCMD_ShutConnect();

	BOOL   StartConfig();
	bool	DetectGSM();

	// Send Message
public:
	BOOL       GPRS_Send(const char* p_data, int n_size );
	char*      GPRS_Buf();
 	int        GPRS_BufSize();
	gsm_parse  m_parse;
	GSM_STATE  m_gsm_state;
	msg_ipc*   m_ipc;
	HANDLE		m_hThrdConfigGSM;
	bool        m_bdialing;


protected:
	unsigned   m_state;   //system state

protected:
	CGPRSLogic();
	static CGPRSLogic  m_instance;

};

#endif // !defined(AFX_GPRSLOGIC_H__7026C9CC_0498_44BA_B3CC_3905A925AF09__INCLUDED_)
