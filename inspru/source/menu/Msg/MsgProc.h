#pragma once

//#include "../DataParser/DataType.h"

class CMsgProc;

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

	HWND				m_hMainWnd;			//menu.exe主窗口句柄
	CWnd*				m_pMainWnd;			//menu.exe主窗口类对象指针

public:
	CMsgProc(void);
	~CMsgProc(void);

	//Singleton模式实例函数
	static CMsgProc* Instance();

	//设置主窗口句柄和指针
	BOOL	SetMainWnd(HWND hMainWnd, CWnd* pMainWnd);

	//消息处理函数
	LRESULT	MsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	//GNSSTERMINAL.exe进程发送来的消息处理
	LRESULT	OnGNSSTerminal(WPARAM wParam, LPARAM lParam);
	//硬按键消息处理
	LRESULT	OnHardButton(WPARAM wParam, LPARAM lParam);

	//心跳线程
	//static DWORD	ThrdHeartBeat( LPVOID lpParameter );
	//启动心跳线程
	//VOID	BeginHeartBeat();

	//服务器平台(SerVeR)消息处理函数
	BOOL	OnTEXT_SMS();												//16.文本信息下发0x8300
	BOOL	OnDESTINATION_NAVI(DWORD dwLastUID);						//附2.平台下发"目的地导航"指令0x8F01
	BOOL	OnCOMMON_ACK(DWORD dwAck_result);							//通用应答
	/*
	BOOL	OnREGISTER_ACK(enResult Result);							//5.终端注册应答0x8100
	BOOL	OnPARAM_SET(enResult Result);								//8.设置终端参数0x8103
	BOOL	OnPARAM_GET(enResult Result);								//9.查询终端参数0x8104
	BOOL	OnTMN_CONTROL(enResult Result, enTMN_CONTROL enControl);	//11.终端控制0x8105
	BOOL	OnPOS_GET(enResult Result);									//13.位置信息查询0x8201
	BOOL	OnTEMP_POS_TRACE(enResult Result);							//15.临时位置跟踪控制0x8202
*/	
	BOOL	OnEVENT_SET(DWORD dwLastUID/*enResult Result*/);			//17.事件设置0x8301
	BOOL	OnQUESTION(DWORD dwLastUID/*enResult Result*/);				//19.提问下发0x8302
	BOOL	OnINFO_MENU_SET(DWORD dwLastUID/*enResult Result*/);		//21.信息点播菜单设置0x8303
/*	BOOL	OnINFO_SERVICE(enResult Result);							//23.信息服务0x8304
	
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
*/	BOOL	OnCAMERA_PHOTOGRAPH(DWORD dwLastUID/*enResult Result*/);	//44.摄像头立即拍摄命令0x8801
/*	BOOL	OnMULTIMEDIA_SEARCH(enResult Result);						//45.存储多媒体数据检索0x8802
	BOOL	OnMULTIMEDIA_UPLOAD(enResult Result);						//47.存储多媒体数据上传命令0x8803
	BOOL	OnSOUND_RECORD(DWORD dwSoundRecording);						//48.录音开始命令0x8804
	BOOL	OnDATA_DOWNLOAD(enResult Result);							//49.数据下行透传0x8900
	BOOL	OnRSA_PUBLIC_KEY(enResult Result);							//52.平台RSA公钥0x8A00
	*/

	BOOL	OnCALL_BACK(BYTE byType);								//24.电话回拨0x8400
};
