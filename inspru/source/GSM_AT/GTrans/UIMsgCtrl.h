

#ifndef  UIMSGCTRL_H
#define  UIMSGCTRL_H

#define WND_NAME_GSM_AT			_T("WND_GSM_AT")

#define   MSG_GPRS2NAVI    ( WM_USER + 0x7623 )
#define   MSG_NAVI2GPRS    ( MSG_GPRS2NAVI + 1 )

enum {
	eMTYPE_GPRS_CONNECT      = 100,		//GPRS连接情况,1:登录到服务 0:没有登录到服务器
	eMTYPE_DOWNLOAD_NAVIFILE,			//GPRS下载导航文件情况1:成功 0:失败
	eMTYPE_DOWNLOADING,					//第一个参数是总包，第二个参数是分包数
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