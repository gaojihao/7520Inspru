#pragma once

#include "../DataParser/DataType.h"
#include "../Lib/Location.h"
#define HEX2BCD(x) (((x) % 10) + ((((x) / 10) % 10) << 4))  /*20 -> 20H*/

class CMsgProc;

extern const POS_INFO*	g_pGpsPosInfo;

//主消息结构体
typedef struct _MSG_PROC
{
	UINT		nMsgID;
	LRESULT		(CMsgProc::*pFunc)(WPARAM, LPARAM);
} MSG_PROC, *PMSG_PROC;

class CMsgProc
{
private:
	static CMsgProc*	m_pInstance;		//Singleton模式实例指针

	HWND				m_hMenuMainWnd;		//menu.exe主窗口句柄
	HWND				m_hGNSSMainWnd;		//GNSSTerminal.exe主窗口句柄
	CWnd*				m_pGNSSMainWnd;		//主窗口类对象指针
	BOOL				m_bIsLogin;			//是否登录成功

	int AuthenticateTimes; //鉴权的次数，不能超过三次
	

	CMsgProc(void);
	~CMsgProc(void);

public:

	//Singleton模式实例函数
	static CMsgProc* Instance();

	//设置主窗口句柄和指针
	BOOL	SetMainWnd(HWND hMainWnd, CWnd* pMainWnd);
	//发送消息至主窗口(GNSSTerminalDlg)
	LRESULT SendServerMsg( WPARAM wParam, LPARAM lParam, BOOL bSynchronous = TRUE );
	//发送消息至Menu.exe的主窗口
	LRESULT SendMsg2Menu( WPARAM wParam, LPARAM lParam = 0, BOOL bSynchronous = FALSE );

	//消息处理函数
	LRESULT	MsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	//服务器指令处理函数
	LRESULT	OnServerCommand(WPARAM wParam, LPARAM lParam);

	//UI发送来的消息处理
	LRESULT	OnUIMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnGpsMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnGuider4uMsg(WPARAM wParam, LPARAM lParam);

	//进行socket连接服务器
	BOOL	ConnectSocket();
	void	DisConnectSocket();
	int		GetSocketStatus();

	VOID    OnTerimalAuthenticate();

	//注册
	VOID	Register();
	//加载配置信息
	VOID	LoadConfig();
	//设置登录状态
	VOID	SetLoginState(BOOL bIsLogin);
	//获取登录状态
	BOOL	GetLoginState();
	//心跳线程
	static DWORD	ThrdHeartBeat( LPVOID lpParameter );
	//启动心跳线程
	VOID	BeginHeartBeat();

	BOOL	ReConnectServer();

	//服务器平台(SerVeR)消息处理函数
	BOOL	OnCOMMON_ACK(enResult Result);								//2.平台通用应答0x8001
	BOOL	OnSUBCONTRACT(enResult Result);								//3.补传分包请求0x8003
	BOOL	OnREGISTER_ACK(enResult Result);							//5.终端注册应答0x8100
	BOOL	OnPARAM_SET(enResult Result);								//8.设置终端参数0x8103
	BOOL	OnPARAM_GET(enResult Result);								//9.查询终端参数0x8104
	BOOL	OnTMN_CONTROL(enResult Result, enTMN_CONTROL enControl);	//11.终端控制0x8105
	BOOL	OnTMN_APPOINTPARAM(enResult Result);						//12.查询指定终端参数
	BOOL	OnTMN_ATTRIBUTE(enResult Result);							//...查询终端属性
	BOOL	OnPOS_GET(enResult Result);									//13.位置信息查询0x8201
	BOOL	OnTEMP_POS_TRACE(enResult Result);							//15.临时位置跟踪控制0x8202
	BOOL	OnTEMP_ALARM_SMS(enResult Result);							//...人工确认报警消息
	BOOL	OnTEXT_SMS(enResult Result);								//16.文本信息下发0x8300
	BOOL	OnEVENT_SET(enResult Result);								//17.事件设置0x8301
	BOOL	OnQUESTION(enResult Result);								//19.提问下发0x8302
	BOOL	OnINFO_MENU_SET(enResult Result);							//21.信息点播菜单设置0x8303
	BOOL	OnINFO_SERVICE(enResult Result);							//23.信息服务0x8304
	BOOL	OnCALL_BACK(enResult Result);								//24.电话回拨0x8400
	BOOL	OnPHONE_BOOK_SET(enResult Result);							//25.设置电话本0x8401
	BOOL	OnCAR_CONTROL(BYTE cControlFlag);							//26.车辆控制0x8500
	BOOL	OnCIRC_REGION_SET(enResult Result);							//28.设置圆形区域0x8600
	BOOL	OnCIRC_REGION_DEL(enResult Result);							//29.删除圆形区域0x8601
	BOOL	OnRECT_REGION_SET(enResult Result);							//30.设置矩形区域0x8602
	BOOL	OnRECT_REGION_DEL(enResult Result);							//31.删除矩形区域0x8603
	BOOL	OnNGON_REGION_SET(enResult Result);							//32.设置多边形区域0x8604
	BOOL	OnNGON_REGION_DEL(enResult Result);							//33.删除多边形区域0x8605
	BOOL	OnROUTE_SET(enResult Result);								//34.设置路线0x8606
	BOOL	OnROUTE_DEL(enResult Result);								//35.删除路线0x8607
	BOOL	OnDRIVE_RECORD_COLLECT(BYTE cCMD);							//36.行驶记录数据采集命令0x8700
	BOOL	OnDRIVE_RECORD_PARAM_SET(BYTE cCMD);						//38.行驶记录参数下传命令0x8701
	BOOL	OnMULTIMEDIA_UPLOAD_ACK(enResult Result);					//43.多媒体数据上传应答0x8800
	BOOL	OnCAMERA_PHOTOGRAPH(enResult Result);						//44.摄像头立即拍摄命令0x8801
	BOOL	OnMULTIMEDIA_SEARCH(enResult Result);						//45.存储多媒体数据检索0x8802
	BOOL	OnMULTIMEDIA_UPLOAD(enResult Result);						//47.存储多媒体数据上传命令0x8803
	BOOL	OnSOUND_RECORD(DWORD dwSoundRecording);						//48.录音开始命令0x8804
	BOOL	OnDATA_DOWNLOAD(enResult Result);							//49.数据下行透传0x8900
	BOOL	OnRSA_PUBLIC_KEY(enResult Result);							//52.平台RSA公钥0x8A00
	BOOL	OnDESTINATION_NAVI(DWORD dwLastUID);						//附2.平台下发"目的地导航"指令0xFF00
	BOOL    OnAudioUpload(enResult Result);
};
