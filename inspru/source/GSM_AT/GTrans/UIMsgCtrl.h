

#ifndef  UIMSGCTRL_H
#define  UIMSGCTRL_H

#define WND_NAME_GSM_AT			_T("WND_GSM_AT")

#define   MSG_GPRS2NAVI    ( WM_USER + 0x7623 )
#define   MSG_NAVI2GPRS    ( MSG_GPRS2NAVI + 1 )

enum {
	eMTYPE_GPRS_CONNECT      = 100,		//GPRS�������,1:��¼������ 0:û�е�¼��������
	eMTYPE_DOWNLOAD_NAVIFILE,			//GPRS���ص����ļ����1:�ɹ� 0:ʧ��
	eMTYPE_DOWNLOADING,					//��һ���������ܰ����ڶ��������Ƿְ���
};

class CUIMsgCtrl  
{
public:
	CUIMsgCtrl();
	virtual ~CUIMsgCtrl();

	void   OnMsgCtrl(WPARAM nParam, LPARAM lParam);

protected:
	void   GSMConfigRst( BOOL bSuc );
	void   CheckSimCardRst( BOOL bSuc );
	void   GSMSignalQuality( int nVal );
	void   GSMNetworkOperator(int nType);
	void   GSMDialConnectRst(int nRst);
	void   GSMHasNewCall(const char* p_num);

	void   GSMReadSMSCount(unsigned nParam);
	void   GSMReadPBookCount( unsigned nParam);



	void   CheckSimCard( BOOL bSuc );

	void   RegisterGSM( BOOL bSuc );
	void   RegisterGSMRst( BOOL bSuc );
	void   RegisterGPRS( BOOL bSuc );
	void   RegisterGPRSRst( BOOL bSuc );
	void   Connect2IP( bool bSuc );
	void   Connect2IPRst( bool bSuc );

	void   LoginSvrRst( bool bSuc );
	void   ReportPosRst( unsigned nState );

	void   SetTimerParam( BOOL bSuc );
	void   SetNetParam( BOOL bSuc );
	void   SetSvrNotice( BOOL bSuc );
};

#endif //UIMSGCTRL_H