// UIMsgCtrl.cpp: implementation of the CUIMsgCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIMsgCtrl.h"
#include "./GPRS/GPRSLogic.h"
#include "./GPRS/gsm_reset.h"
#include "./GSM/gsm_msg_def.h"
#include "./GPRS/gsm_reset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern void   ShowTip( CString strTip );


CUIMsgCtrl::CUIMsgCtrl()
{

}

CUIMsgCtrl::~CUIMsgCtrl()
{

}


void   CUIMsgCtrl::OnMsgCtrl(WPARAM nParam, LPARAM lParam)
{
	switch ( nParam )
	{
	case eGSM_SIGNAL_VALUE       : GSMSignalQuality(lParam);   break;
	case eGSM_CONFIG_RST         : GSMConfigRst(lParam);       break;
	case eGSM_SIM_CARD_RST       : CheckSimCardRst(lParam);    break;
	case eGSM_REGISTER_GSM_RST   : RegisterGSMRst(lParam);     break;
	case eGSM_QUERY_GSM_ISP      : GSMNetworkOperator(lParam); break;
	case eGSM_DIAL_CONNECT_RST   : GSMDialConnectRst(lParam);  break;
	case eGSM_NEW_CALLING_RST : GSMHasNewCall( (const char*)lParam ); break;
	case eGSM_READ_SIM_SMS_RST   : GSMReadSMSCount(lParam);    break;
	case eGSM_READ_SIM_PBOOK_RST : GSMReadPBookCount(lParam);  break;


	case eGSM_SIM_CARD         : CheckSimCard(lParam);      break;
	case eGSM_REGISTER_GSM     : RegisterGSM( lParam );     break;
		
//GPRS information
	case eGSM_REGISTER_GPRS    : RegisterGPRS( lParam );    break;
	case eGSM_REGISTER_GPRS_RST: RegisterGPRSRst( lParam ); break;
	case eGSM_CONNECT_IP       : Connect2IP( lParam );      break;
	case eGSM_CONNECT_IP_RST   : Connect2IPRst( lParam );   break;

// 
	case eGSM_LOGIN_RST        : LoginSvrRst( lParam );     break;
	case eGSM_REPORT_POS       : ReportPosRst( lParam );    break;
#if 0

	case eGSM_SET_TIME         : SetTimerParam( lParam);    break;
	case eGSM_SET_NET          : SetNetParam(lParam);       break;
	case eGSM_SVR_MSG          : SetSvrNotice(lParam );     break;
#endif
	default: 
		break;
	}
}

void   CUIMsgCtrl::GSMReadSMSCount(unsigned nParam)
{
	int nUsed = LOWORD(nParam);
	int nMax = HIWORD(nParam);
	CString str;
	str.Format( _T("Max SMS count:%d  Used SMS count:%d!"), nMax, nUsed );
	ShowTip( str );
}


void   CUIMsgCtrl::GSMReadPBookCount( unsigned nParam)
{
	int nUsed = LOWORD(nParam);
	int nMax = HIWORD(nParam);
	CString str;
	str.Format( _T("Max PhoneBook count:%d  Used PhoneBook count:%d!"), nMax, nUsed );
	ShowTip( str );
}


void   CUIMsgCtrl::GSMDialConnectRst(int nRst)
{
	CString str;
	if ( nRst == 1 )
		str.Format(_T("User call is through!") );
	else
		str.Format( _T("User call is not through!") );
	ShowTip( str );
}

void   CUIMsgCtrl::GSMHasNewCall(const char* p_num)
{
	CString str;
	if ( p_num )
		str.Format(_T("New Call number is  %s!"), CString(p_num) );
	else
		str.Format( _T("New Call number is empty!") );
	ShowTip( str );
}


void   CUIMsgCtrl::GSMSignalQuality( int nVal )
{
	CString str;
	str.Format(_T("GSM��ǰ�ź�ǿ��Ϊ:��%d��[0~32]"), nVal );
	ShowTip( str );
}

void CUIMsgCtrl::GSMNetworkOperator( int nType )
{
	CString str;
	if ( nType == gsm_net_china_mobile )
		str = _T("The GSM network operator is CHINA MOBILE!");
	else if ( nType == gsm_net_china_unicom )
		str = _T("The GSM network operator is CHINA UNICOM!");
	else if ( nType == gsm_net_china_CUGSM )
		str = _T("The GSM network operator is CHINA CUGSM!");
	else
		str = _T("The GSM network operator is unknow!");
	ShowTip( str );
}


void  CUIMsgCtrl::GSMConfigRst( BOOL bSuc )
{
	CGPRSLogic* pGPRS = CGPRSLogic::Instance();
	if ( bSuc )
	{
		ShowTip( _T("����GSMģ�������OK����") );
		char* pUUID = pGPRS->GCMD_IMEI();
		if ( strlen(pUUID) == 0 )
		{
			ShowTip( _T("Get Device's IMEI ��ERROR��!") );
		}else
		{
			TCHAR szBuf[64];
			memset(szBuf, 0, sizeof(szBuf) );
			
			int rst = MultiByteToWideChar(CP_ACP,NULL, pUUID, -1, szBuf, sizeof(szBuf)/sizeof(szBuf[0]) );
			if ( rst == 0 )
				rst = MultiByteToWideChar(936,NULL, pUUID, -1, szBuf, sizeof(szBuf)/sizeof(szBuf[0]) );
			CString str;
			str.Format(_T("GSM Device's IMEI is [%s]"), szBuf );
			ShowTip( str );
			//msg_cimm::set_device_id( NULL );
		}
		ShowTip( _T("ϵͳ���ڼ��SIM��----->") );
	}else
	{
		ShowTip( _T("config gsm module is ok!") );
	}
}


void   CUIMsgCtrl::CheckSimCard( BOOL bSuc )
{
	CString str;
	if ( bSuc == 1 )
	{
		str.Format(_T("�Ѿ���鵽SIM�����롾OK��"));
	}
	else
		str.Format(_T("ϵͳ��鲻��SIM����ERROR��"));
	ShowTip( str );
}


void   CUIMsgCtrl::CheckSimCardRst( BOOL bSuc )
{
	CString str;
	if ( bSuc == 1 )
	{
		str.Format(_T("���SIM����ɹ�����OK��"));
	}
	else
		str.Format(_T("ϵͳ��鲻��SIM����ERROR��������SIM�Ƿ����ã�"));
	ShowTip( str );
}


//////////////////////////////////////////////////////////////////////////

void   CUIMsgCtrl::LoginSvrRst( bool bSuc )
{
	CString str;
	if ( bSuc == 1 )
	{
		str.Format(_T("��¼����������ɹ�����OK��"));
	}
	else
		str.Format(_T("��¼�����������ERROR������������ֹ��½��"));
	ShowTip( str );
}

//////////////////////////////////////////////////////////////////////////

void   CUIMsgCtrl::RegisterGSM( BOOL bSuc )
{
	CString str;
	if ( bSuc == 1 )
	{
		CGPRSLogic::Instance()->SetState( eGState_InitGSM_OK );
		str.Format(_T("GSM����ע�᡾OK��"));
	}
	else
	{
		CGPRSLogic::Instance()->SetState( eGState_Error );
		str.Format(_T("GSM����ע�᡾ERROR��"));
	}
	ShowTip( str );
}

//////////////////////////////////////////////////////////////////////////

void   CUIMsgCtrl::RegisterGSMRst( BOOL bSuc )
{
	CString str;
	if ( bSuc == 1 )
	{
		CGPRSLogic::Instance()->SetState( eGState_InitGSM_OK );
		str.Format(_T("GSM����ע��������OK��"));
	}
	else
	{
		CGPRSLogic::Instance()->SetState( eGState_Error );
		str.Format(_T("GSM����ע��������ERROR��"));
	}
	ShowTip( str );
}


void  CUIMsgCtrl::RegisterGPRS( BOOL bSuc )
{
	CString str;
	if ( bSuc )
	{
		CGPRSLogic::Instance()->SetState( eGState_ConnectingIP );
		str.Format(_T("GPRS����ע�᡾OK��"));
	}
	else
	{
		//�������3�β������ӵ�����������Resetģ��
		str.Format(_T("GPRS����ע�᡾ERROR��"));
		CGPRSLogic::Instance()->SetState( eGState_Error );
	}
	ShowTip( str );
}

void  CUIMsgCtrl::RegisterGPRSRst( BOOL bSuc )
{
	CString str;
	if ( bSuc )
	{
		CGPRSLogic::Instance()->SetState( eGState_ConnectingIP );
		str.Format(_T("GPRS����ע��������OK��"));
	}
	else
	{
		str.Format(_T("GPRS����ע��������ERROR��"));
		CGPRSLogic::Instance()->SetState( eGState_Error );
	}
	ShowTip( str );
}


void  CUIMsgCtrl::Connect2IP(bool bSuc )
{
	CString str;
	if ( bSuc )
	{
		CGPRSLogic::Instance()->SetState( eGState_ConnectedIP );
		str = _T("�Ѿ���Զ�̷���������TCP���ӡ�OK��");
	}
	else
	{
		CGPRSLogic::Instance()->SetState( eGState_Error );
		str = _T("��Զ�̷���������TCP���ӡ�ERROR��");
	}
	ShowTip( str );
}

void  CUIMsgCtrl::Connect2IPRst( bool bSuc  )
{
	CString str;
	if ( bSuc )
	{
		CGPRSLogic::Instance()->SetState( eGState_ConnectedIP );
		str = _T("����TCP���ӽ��:��OK��");
	}
	else
	{
		CGPRSLogic::Instance()->SetState( eGState_Error );
		str = _T("����TCP���ӽ��:��ERROR��");
	}
	ShowTip( str );
}



void   CUIMsgCtrl::ReportPosRst( unsigned nState )
{
	BOOL bSuc = (nState >> 16) & 0x01;
	int nCount = (nState & 0xFFFF);
	
	CString str;
	if ( bSuc )
		str.Format( _T("λ����Ϣ��OK��") );
	else
		str.Format( _T("λ����Ϣ��ERROR��") );
	ShowTip( str );
}

void   CUIMsgCtrl::SetTimerParam( BOOL bSuc )
{
	if ( bSuc )
		ShowTip( _T("\r\n����ϵͳ�·��� [ʱ�����] �ɹ���OK��\r\n") );
	else
		ShowTip( _T("\r\n����ϵͳ�·��� [ʱ�����] ʧ�ܡ�OK��\r\n") );
}


void   CUIMsgCtrl::SetNetParam( BOOL bSuc )
{
	if ( bSuc )
		ShowTip( _T("����ϵͳ�·��� [�������] �ɹ���OK��") );
	else
		ShowTip( _T("����ϵͳ�·��� [�������] ʧ�ܡ�ERROR��") );
}

void   CUIMsgCtrl::SetSvrNotice( BOOL bSuc )
{
	if ( bSuc )
		ShowTip( _T("����ϵͳ�·��� [��������Ϣ] �ɹ���OK��") );
	else
		ShowTip( _T("����ϵͳ�·��� [��������Ϣ] ʧ�ܡ�ERROR��") );
}