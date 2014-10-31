#include "StdAfx.h"
#include "MsgProc.h"
#include "../DataParser/DataType.h"
#include "../DataParser/DataParser.h"
#include "../Tools/LogTool.h"
#include "../DataParser/DataPack.h"
#include "../TerminalConfig/TerminalConfig.h"
#include "../GPSData/GPSData.h"
#include "../DataParser/ComManager.h"

//menu.exe同GNSSTerminal.exe进程之间交互的消息ID
UINT	g_nWM_MENU_GNSSTERMINAL	= RegisterWindowMessage(_T("MENU_GNSSTERMINAL"));
UINT    g_nWM_MsgIDPosRefresh	= RegisterWindowMessage( WM_POS_DATA_REFRESH );


const POS_INFO*	g_pGpsPosInfo;
//初始化静态成员变量
CMsgProc*	CMsgProc::m_pInstance	= NULL;

MSG_PROC	g_msg_proc[] = 
{
	WM_SERVER,					&CMsgProc::OnServerCommand,		//接收到服务器socket数据处理消息
	g_nWM_MENU_GNSSTERMINAL,	&CMsgProc::OnUIMsg,				//UI的消息
	g_nWM_MsgIDPosRefresh,      &CMsgProc::OnGpsMsg,
	MSG_TO_OUTSIDEPROCESS,		&CMsgProc::OnGuider4uMsg,		//导航软件消息
};

CMsgProc::CMsgProc(void)
{
	m_hGNSSMainWnd	= NULL;
	m_hMenuMainWnd	= NULL;
	m_pGNSSMainWnd	= NULL;
	m_bIsLogin		= FALSE;
}

CMsgProc::~CMsgProc(void)
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

	//delete [] m_pmsg_proc;
}

/************************************************************************
* 函数名: Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: singleton
************************************************************************/
CMsgProc* CMsgProc::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CMsgProc;
	}
	return m_pInstance;
}

/************************************************************************
* 函数名:	SetMainWnd
* 描  述:	设置主窗口句柄和类对象指针
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::SetMainWnd(HWND hMainWnd, CWnd* pMainWnd)
{
	if(NULL == hMainWnd || NULL == pMainWnd)
		return FALSE;

	m_hGNSSMainWnd	= hMainWnd;
	m_pGNSSMainWnd	= pMainWnd;

	m_hMenuMainWnd = ::FindWindow ( NULL, MenuApp );

	g_pGpsPosInfo = GetSharePosInfo();
	return TRUE;
}

/************************************************************************
* 函数名:	SendServerMsg
* 描  述:	发送消息到当前进程主窗口
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
LRESULT CMsgProc::SendServerMsg( WPARAM wParam, LPARAM lParam, BOOL bSynchronous /*= TRUE*/ )
{
	if(NULL == m_hGNSSMainWnd)
		return 0;

	if(!bSynchronous)	//同步消息
		return ::SendMessage(m_hGNSSMainWnd, WM_SERVER, wParam, lParam);
	else
		return ::PostMessage(m_hGNSSMainWnd, WM_SERVER, wParam, lParam);
}

/************************************************************************
* 函数名:	SendMsg2Menu
* 描  述:	发送消息至Menu.exe的主窗口
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
LRESULT CMsgProc::SendMsg2Menu( WPARAM wParam, LPARAM lParam/* = 0*/, BOOL bSynchronous /*= TRUE*/ )
{
	//menu.exe主窗口句柄
	if( IsWindow(m_hMenuMainWnd) == 0 )
	{
		m_hMenuMainWnd = ::FindWindow ( NULL, MenuApp );
		if(NULL == m_hMenuMainWnd)
			return 0;
	}

	//发送消息
	if(bSynchronous)	//同步消息
		return ::SendMessage(m_hMenuMainWnd, g_nWM_MENU_GNSSTERMINAL, wParam, lParam);
	else
		return ::PostMessage(m_hMenuMainWnd, g_nWM_MENU_GNSSTERMINAL, wParam, lParam);
}

/************************************************************************
* 函数名:	MsgProc
* 描  述:	消息处理函数
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
LRESULT CMsgProc::MsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	for (INT i = 0; i < dim(g_msg_proc); i++)
	{
		if(message == g_msg_proc[i].nMsgID)
		{
			return (this->*g_msg_proc[i].pFunc)(wParam, lParam);
		}
	}

	return 0;
}

/************************************************************************
* 函数名:	OnServerCommand
* 描  述:	服务器指令处理函数
* 入  参:	1.wParam: 高2字节：回复类型     低2字节：回复是否成功
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
LRESULT CMsgProc::OnServerCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case MSG_SVR_COMMON_ACK:				//2.平台通用应答0x8001
		return OnCOMMON_ACK((enResult)lParam);
	case MSG_SVR_SUBCONTRACT:				//3.补传分包请求0x8003
		return OnSUBCONTRACT((enResult)lParam);	
	case MSG_SVR_REGISTER_ACK:				//5.终端注册应答0x8100
		return OnREGISTER_ACK((enResult)lParam);
	case MSG_SVR_PARAM_SET:					//8.设置终端参数0x8103
		return OnPARAM_SET((enResult)lParam);
	case MSG_SVR_PARAM_GET:					//9.查询终端参数0x8104
		return OnPARAM_GET((enResult)lParam);
	case MSG_SVR_TMN_CONTROL:				//11.终端控制0x8105
		return OnTMN_CONTROL( (enResult)lParam, (enTMN_CONTROL)(lParam&0x0000FFFF) );
	case MSG_SVR_TMN_APPOINT_PARAMETER:		//12.查询指定终端参数
		return OnTMN_APPOINTPARAM((enResult)lParam);
	case MSG_SVR_TMN_ATTRIBUTE:				//.查询终端属性
		return OnTMN_ATTRIBUTE((enResult)lParam);
	case MSG_SVR_POS_GET:					//13.位置信息查询0x8201
		return OnPOS_GET((enResult)lParam);
	case MSG_SVR_TEMP_POS_TRACE:			//15.临时位置跟踪控制0x8202
		return OnTEMP_POS_TRACE((enResult)lParam);
	case MSG_SVR_ALARM_SMS:					//人工确认报警消息
		return OnTEMP_ALARM_SMS((enResult)lParam);
	case MSG_SVR_TEXT_SMS:					//16.文本信息下发0x8300
		return OnTEXT_SMS((enResult)lParam);
	case MSG_SVR_EVENT_SET:					//17.事件设置0x8301
		return OnEVENT_SET((enResult)lParam);
	case MSG_SVR_QUESTION:					//19.提问下发0x8302
		return OnQUESTION((enResult)lParam);
	case MSG_SVR_INFO_MENU_SET:				//21.信息点播菜单设置0x8303
		return OnINFO_MENU_SET((enResult)lParam);
	case MSG_SVR_INFO_SERVICE:				//23.信息服务0x8304
		return OnINFO_SERVICE((enResult)lParam);
	case MSG_SVR_CALL_BACK:					//24.电话回拨0x8400
		return OnCALL_BACK((enResult)lParam);
	case MSG_SVR_PHONE_BOOK_SET:			//25.设置电话本0x8401
		return OnPHONE_BOOK_SET((enResult)lParam);
	case MSG_SVR_CAR_CONTROL:				//26.车辆控制0x8500
		return OnCAR_CONTROL((BYTE)lParam);
	case MSG_SVR_CIRC_REGION_SET:			//28.设置圆形区域0x8600
		return OnCIRC_REGION_SET( (enResult)lParam );
	case MSG_SVR_CIRC_REGION_DEL:			//29.删除圆形区域0x8601
		return OnCIRC_REGION_DEL( (enResult)lParam );
	case MSG_SVR_RECT_REGION_SET:			//30.设置矩形区域0x8602
		return OnRECT_REGION_SET( (enResult)lParam );
	case MSG_SVR_RECT_REGION_DEL:			//31.删除矩形区域0x8603
		return OnRECT_REGION_DEL( (enResult)lParam );
	case MSG_SVR_NGON_REGION_SET:			//32.设置多边形区域0x8604
		return OnNGON_REGION_SET( (enResult)lParam );
	case MSG_SVR_NGON_REGION_DEL:			//33.删除多边形区域0x8605
		return OnNGON_REGION_DEL( (enResult)lParam );
	case MSG_SVR_ROUTE_SET:					//34.设置路线0x8606
		return OnROUTE_SET( (enResult)lParam );
	case MSG_SVR_ROUTE_DEL:					//35.删除路线0x8607
		return OnROUTE_DEL( (enResult)lParam );
	case MSG_SVR_DRIVE_RECORD_COLLECT:		//36.行驶记录数据采集命令0x8700
		return OnDRIVE_RECORD_COLLECT( (BYTE)lParam );
	case MSG_SVR_DRIVE_RECORD_PARAM_SET:	//38.行驶记录参数下传命令0x8701
		return OnDRIVE_RECORD_PARAM_SET( (BYTE)lParam );
	case MSG_SVR_MULTIMEDIA_UPLOAD_ACK:		//43.多媒体数据上传应答0x8800
		return OnMULTIMEDIA_UPLOAD_ACK( (enResult)lParam );
	case MSG_SVR_CAMERA_PHOTOGRAPH:			//44.摄像头立即拍摄命令0x8801
		return OnCAMERA_PHOTOGRAPH( (enResult)lParam );
	case MSG_SVR_MULTIMEDIA_SEARCH:			//45.存储多媒体数据检索0x8802
		return OnMULTIMEDIA_SEARCH( (enResult)lParam );
	case MSG_SVR_MULTIMEDIA_UPLOAD:			//47.存储多媒体数据上传命令0x8803
		return OnMULTIMEDIA_UPLOAD( (enResult)lParam );
	case MSG_SVR_SOUND_RECORD:				//48.录音开始命令0x8804
		return OnSOUND_RECORD(lParam);
	case MSG_SVR_DATA_DOWNLOAD:				//49.数据下行透传0x8900
		return OnDATA_DOWNLOAD( (enResult)lParam );
	case MSG_SVR_RSA_PUBLIC_KEY:			//52.平台RSA公钥0x8A00
		return OnRSA_PUBLIC_KEY( (enResult)lParam );
	case MSG_TMN_DESTINATION_NAVI:			//附2.平台下发"目的地导航"指令0xFF00
		return OnDESTINATION_NAVI(lParam);
	case MSG_SVR_SINGLE_MULT_SEARCH:
		return OnAudioUpload((enResult)lParam);
	default:	return FALSE;
	}

	return 1;
}

//UI发送来的消息处理
LRESULT	CMsgProc::OnUIMsg(WPARAM wParam, LPARAM lParam)
{
	if(MSG_TMN_TEXT_SMS == wParam)
	{
		CDataPack::Instance()->PackTextSMS((INT)lParam);
	}
	else if (MSG_TMN_EVENT_REPORT == wParam)
	{
		CDataPack::Instance()->PackEVENT_REPORT((BYTE)lParam);
	}
	else if (MSG_TMN_QUESTION_ACK == wParam)
	{
		CDataPack::Instance()->PackQUESTION_ACK((BYTE)lParam);
	}
	else if (MSG_TMN_INFO_DEMAND_CANCEL == wParam)
	{
		CDataPack::Instance()->PackINFO_DEMAND_CANCEL((DWORD)lParam);
	}

	return 0;
}

LRESULT CMsgProc::OnGpsMsg(WPARAM wParam, LPARAM lParam)
{
	CGPSData::Instance()->GPSReportAndAdjusttime();
	return 0;
}

LRESULT CMsgProc::OnGuider4uMsg(WPARAM wParam, LPARAM lParam)
{
	WORD dwparam = (WORD)wParam;
	switch(dwparam)
	{
	case PARAM_ROUTE_NORMAL:
		g_alarm_state.unAlarm.alarm_flag.cDriftAlarm = 0x00;
		break;
	case PARAM_ROUTE_YEBOND:
		g_alarm_state.unAlarm.alarm_flag.cDriftAlarm = 0x01;
		break;
	case PARAM_ARRIVE_TERMINI:
		g_alarm_state.unAlarm.alarm_flag.cGPSSignIn	 = 0x01;
		break;
	default:
		break;
	}
	return 0;
}

/************************************************************************
* 函数名:	OnCOMMON_ACK
* 描  述:	2.平台通用应答0x8001
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCOMMON_ACK(enResult Result)
{
	const PCOMMON_ACK	pCommonAck = CDataParser::Instance()->GetCommonAck();
	if( RESULT_SUCCESS == Result )
	{
		switch( pCommonAck->wAckMsgID )
		{
		case MSG_TMN_AUTHENTICATION:		//7.终端鉴权0x0102
			{
				//登录状态为TRUE
				SetLoginState(TRUE);
				CDataPack::Instance()->PackTimeRequest();
				Sleep(20);

				CDataPack::Instance()->SplitBlindAreaData();
				Sleep(2);
				CDataPack::Instance()->PackPOS_REPORT();
				Sleep(100);
				CDataParser::Instance()->LoadPhotoNum();
			}
			break;
		case MSG_TMN_LOGOUT:				//6.终端注销0x0003
			//登录状态为FALSE
			SetLoginState(FALSE);
			break;
		case MSG_TMN_EVENT_REPORT:			//18.事件报告0x0301
			{
				unCOMMON_ACK_RESULT common_result;
				common_result.ack_result.wMsgID	= MSG_TMN_EVENT_REPORT;
				common_result.ack_result.wResult	= Result;
				SendMsg2Menu( MSG_SVR_COMMON_ACK, common_result.dwAck_result );
			}
			break;
		}
	}
	else
	{
		switch( pCommonAck->wAckMsgID )
		{
		case MSG_TMN_AUTHENTICATION:		//7.终端鉴权0x0102				
			AuthenticateTimes++;
			if (AuthenticateTimes < 3)
			{
				OnTerimalAuthenticate();
			}
			else
			{
				SetLoginState(FALSE);
			}
			break;
		}
	}
	
	return TRUE;
}

/************************************************************************
* 函数名: OnREGISTER_ACK
* 描  述: 5.终端注册应答0x8100
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnREGISTER_ACK(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		//鉴权
		CDataPack::Instance()->PackAUTHENTICATION();
	}
	else
	{
		CDataPack::Instance()->PackCOMMON_ACK(Result);
	}

	return TRUE;
}

/************************************************************************
* 函数名:	OnPARAM_SET
* 描  述:	8.设置终端参数0x8103
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnPARAM_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnPARAM_GET
* 描  述:	9.查询终端参数0x8104
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnPARAM_GET(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		CDataPack::Instance()->PackPARAM_GET_ACK();
	}
	else
	{
		CDataPack::Instance()->PackCOMMON_ACK(Result);
	}

	return TRUE;
}

/************************************************************************
* 函数名:	OnTMN_CONTROL
* 描  述:	11.终端控制0x8105
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnTMN_CONTROL(enResult Result, enTMN_CONTROL enControl)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnPOS_GET
* 描  述:	13.位置信息查询0x8201
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnPOS_GET(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		CDataPack::Instance()->PackPOS_GET_ACK();
	}
	else
	{
		CDataPack::Instance()->PackCOMMON_ACK(Result);
	}

	return TRUE;
}

/************************************************************************
* 函数名:	OnPOS_GET
* 描  述:	15.临时位置跟踪控制0x8202
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnTEMP_POS_TRACE(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		//CDataPack::Instance()->PackPOS_GET_ACK();
		CDataPack::Instance()->PackCOMMON_ACK(Result);
	}

	return TRUE;
}

/************************************************************************
* 函数名:	OnTEXT_SMS
* 描  述:	16.文本信息下发0x8300
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnTEXT_SMS(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnEVENT_SET
* 描  述:	17.事件设置0x8301
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnEVENT_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnQUESTION
* 描  述:	19.提问下发0x8302
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnQUESTION(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}


/************************************************************************
* 函数名:	OnINFO_MENU_SET
* 描  述:	21.信息点播菜单设置0x8303
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnINFO_MENU_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnINFO_SERVICE
* 描  述:	23.信息服务0x8304
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnINFO_SERVICE(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnCALL_BACK
* 描  述:	24.电话回拨0x8400
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCALL_BACK(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnPHONE_BOOK_SET
* 描  述:	25.设置电话本0x8401
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnPHONE_BOOK_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	SendMsg2Menu(MSG_SVR_PHONE_BOOK_SET);
	return TRUE;
}

/************************************************************************
* 函数名:	OnCAR_CONTROL
* 描  述:	26.车辆控制0x8500
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCAR_CONTROL(BYTE cControlFlag)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}


/************************************************************************
* 函数名:	OnCIRC_REGION_SET
* 描  述:	28.设置圆形区域0x8600
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCIRC_REGION_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnCIRC_REGION_DEL
* 描  述:	29.删除圆形区域0x8601
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCIRC_REGION_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnRECT_REGION_SET
* 描  述:	30.设置矩形区域0x8602
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnRECT_REGION_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnRECT_REGION_DEL
* 描  述:	31.删除矩形区域0x8603
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnRECT_REGION_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnNGON_REGION_SET
* 描  述:	32.设置多边形区域0x8604
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnNGON_REGION_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnNGON_REGION_DEL
* 描  述:	33.删除多边形区域0x8605
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnNGON_REGION_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnROUTE_SET
* 描  述:	34.设置路线0x8606
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnROUTE_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnROUTE_DEL
* 描  述:	35.删除路线0x8607
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnROUTE_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnDRIVE_RECORD_COLLECT
* 描  述:	36.行驶记录数据采集命令0x8700
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnDRIVE_RECORD_COLLECT(BYTE cCMD)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnDRIVE_RECORD_PARAM_SET
* 描  述:	38.行驶记录参数下传命令0x8701
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnDRIVE_RECORD_PARAM_SET(BYTE cCMD)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}
/************************************************************************
* 函数名:	OnMULTIMEDIA_UPLOAD_ACK
* 描  述:	43.多媒体数据上传应答0x8800
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnMULTIMEDIA_UPLOAD_ACK(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnCAMERA_PHOTOGRAPH
* 描  述:	44.摄像头立即拍摄命令0x8801
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCAMERA_PHOTOGRAPH(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	SendMsg2Menu(MSG_SVR_CAMERA_PHOTOGRAPH);
	return TRUE;
}

/************************************************************************
* 函数名:	OnMULTIMEDIA_SEARCH
* 描  述:	45.存储多媒体数据检索0x8802
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnMULTIMEDIA_SEARCH(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnMULTIMEDIA_UPLOAD
* 描  述:	47.存储多媒体数据上传命令0x8803
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnMULTIMEDIA_UPLOAD(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnSOUND_RECORD
* 描  述:	48.录音开始命令0x8804
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnSOUND_RECORD(DWORD dwSoundRecording)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* 函数名:	OnDATA_DOWNLOAD
* 描  述:	49.数据下行透传0x8900
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnDATA_DOWNLOAD(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

BOOL CMsgProc::OnAudioUpload(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnRSA_PUBLIC_KEY
* 描  述:	52.平台RSA公钥0x8A00
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnRSA_PUBLIC_KEY(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* 函数名:	OnDESTINATION_NAVI
* 描  述:	附2.平台下发"目的地导航"指令0xFF00
* 入  参:	1.Result:	回复是否成功
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnDESTINATION_NAVI(DWORD dwLastUID)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);

	return TRUE;
}
/************************************************************************
* 函数名:	ConnectSocket
* 描  述:	连接socket
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
BOOL CMsgProc::ConnectSocket()
{
	//获取终端参数结构体
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();

	

	CLogTool::Instance()->WriteLogFile("Connecting to IP:%s:%d...", pParam->szMAIN_SVR_IP, pParam->dwSVR_TCP_PORT );

	AuthenticateTimes = 0;

	BOOL bRet = CComManager::Instance()->OpenSocketConnect(pParam->szMAIN_SVR_IP, (WORD)pParam->dwSVR_TCP_PORT);

	if (bRet == TRUE)
	{
		CLogTool::Instance()->WriteLogFile("success!");
		

		//注册鉴权
		OnTerimalAuthenticate();
		
		return TRUE;
	}
	else
	{
		CLogTool::Instance()->WriteLogFile("failure!");
		return FALSE;
	}
}

void CMsgProc::DisConnectSocket()
{
	 CComManager::Instance()->CloseSocketConnect();
}

int CMsgProc::GetSocketStatus()
{
	return CComManager::Instance()->GetConnectStatus();
}

VOID CMsgProc::OnTerimalAuthenticate()
{
	CIniFile	IniFile;
	//加载文件失败
	if( !IniFile.Load( PATH_CONFIG_INI ) )
	{
//		AfxMessageBox(_T("加载config.ini文件失败！"));
		return;
	}

	CHAR	szAuthenCode[24] = {0};			//鉴权码

	IniFile.GetString(_T("Set"), _T("AuthenCode"), szAuthenCode, sizeof(szAuthenCode));
//	IniFile.Release();

	LoadConfig();

	if (strlen(szAuthenCode) == 0)
	{	
		Register();
	}
	else
	{			
		CDataPack::Instance()->PackAUTHENTICATION((PCSTR)szAuthenCode);
	}


}

/************************************************************************
* 函数名:	Register
* 描  述:	注册
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
VOID CMsgProc::Register()
{
	CIniFile	IniFile;
	//加载文件失败
	if( !IniFile.Load( PATH_CONFIG_INI ) )
	{
//		AfxMessageBox(_T("加载config.ini文件失败！"));
		return;
	}

	CHAR	szPhoneNum[24] = {0};		//终端SIM卡号码
	CHAR	szManufacturerID[24] = {0};	//制造商ID，5byte
	CHAR	szTerminalType[24] = {0};	//终端型号，8byte
	CHAR	szTerminalID[24] = {0};		//终端ID，7byte
	WORD	wEncryptionType = 0;		//数据加密方式 0:不加密 1:RSA加密
		
	IniFile.GetString(_T("Set"), _T("PhoneNum"), szPhoneNum, sizeof(szPhoneNum));
	IniFile.GetString(_T("Set"), _T("ManufacturerID"), szManufacturerID, sizeof(szManufacturerID));
	IniFile.GetString(_T("Set"), _T("TerminalType"), szTerminalType, sizeof(szTerminalType));
	IniFile.GetString(_T("Set"), _T("TerminalID"), szTerminalID, sizeof(szTerminalID));
	IniFile.GetInt(_T("Set"), _T("EncryptionType"), wEncryptionType);
//	IniFile.Release();

	//设置数据包属性段
	CDataPack::Instance()->SetMsgProperty(szPhoneNum, (BYTE)wEncryptionType);
	//注册
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();
	CDataPack::Instance()->PackREGISTER(pParam->wVEHICLE_PROVINCE_ID, pParam->wVEHICLE_CITY_ID, szManufacturerID, szTerminalType, 
		szTerminalID, (BYTE)pParam->dwPLATE_COLOR, pParam->szVEHICLE_ID, strlen(pParam->szVEHICLE_ID));
}

/************************************************************************
* 函数名:	Register
* 描  述:	加载配置信息
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
VOID CMsgProc::LoadConfig()
{
	CIniFile	IniFile;
	//加载文件失败
	if( !IniFile.Load( PATH_CONFIG_INI ) )
	{
//		AfxMessageBox(_T("加载config.ini文件失败！"));
		return;
	}

	CHAR	szPhoneNum[24] = {0};		//终端SIM卡号码
// 	CHAR	szManufacturerID[24] = {0};	//制造商ID，5byte
// 	CHAR	szTerminalType[24] = {0};	//终端型号，8byte
// 	CHAR	szTerminalID[24] = {0};		//终端ID，7byte
	WORD	wEncryptionType = 0;		//数据加密方式 0:不加密 1:RSA加密

	IniFile.GetString(_T("Set"), _T("PhoneNum"), szPhoneNum, sizeof(szPhoneNum));
// 	IniFile.GetString(_T("Set"), _T("ManufacturerID"), szManufacturerID, sizeof(szManufacturerID));
// 	IniFile.GetString(_T("Set"), _T("TerminalType"), szTerminalType, sizeof(szTerminalType));
// 	IniFile.GetString(_T("Set"), _T("TerminalID"), szTerminalID, sizeof(szTerminalID));
	IniFile.GetInt(_T("Set"), _T("EncryptionType"), wEncryptionType);
//	IniFile.Release();

	//设置数据包属性段
	CDataPack::Instance()->SetMsgProperty(szPhoneNum, (BYTE)wEncryptionType);
}

/************************************************************************
* 函数名:	SetLoginState
* 描  述:	设置登录状态
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
VOID CMsgProc::SetLoginState(BOOL bIsLogin)
{

	//发送消息至menu.exe

	HWND hWndSend = ::FindWindow (NULL,MenuApp );
	//发送消息
	if (hWndSend != NULL)
	{
		::PostMessage(hWndSend, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, (LPARAM)bIsLogin);
	}

	m_bIsLogin	= bIsLogin;
}

/************************************************************************
* 函数名:	GetLoginState
* 描  述:	获取登录状态
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
BOOL CMsgProc::GetLoginState()
{

	return m_bIsLogin;
}

/************************************************************************
* 函数名:	ThrdHeartBeat
* 描  述:	心跳线程
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
DWORD CMsgProc::ThrdHeartBeat( LPVOID lpParameter )
{

	CMsgProc *Proc = (CMsgProc *)lpParameter;

	const PTERMINAL_PARAM pParam = CTerminalConfig::Instance()->GetTerminalParam();

	while ( TRUE )
	{
		Sleep( pParam->dwHEART_BEAT_SLICE * 1000 );

		//登录上服务器则开始心跳
		if( !CMsgProc::Instance()->GetLoginState() )
			continue;

		CDataPack::Instance()->PackHEARTBEAT();
	}

	return 1;
}


BOOL CMsgProc::ReConnectServer()
{

	SetLoginState(FALSE);

	DisConnectSocket();

	Sleep(2000);

	for (int i = 0; i < 10; i++)
	{
		if (ConnectSocket())
		{
			return TRUE;
		}
		Sleep(2000);
	}

	::PostMessage(HWND_BROADCAST, MSG_RAS_RECONNECT, 2, 0); //重连ras
	return FALSE;
}

/************************************************************************
* 函数名:	BeginHeartBeat
* 描  述:	启动心跳线程
* 入  参:	
* 出  参:	
* 返  回:	
* 备  注:
************************************************************************/
VOID CMsgProc::BeginHeartBeat()
{
	static HANDLE hThrdHeartBeat = NULL;
	if(hThrdHeartBeat != NULL)
		return;

	hThrdHeartBeat = ::CreateThread(NULL, 0, ThrdHeartBeat, this, 0, NULL);
	SetThreadPriority(hThrdHeartBeat, THREAD_PRIORITY_IDLE);
}

/************************************************************************
* 函数名:	OnSUBCONTRACT
* 描  述:	3.补传分包请求0x8003
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnSUBCONTRACT(enResult Result)
{
	return TRUE;
}

/************************************************************************
* 函数名:	OnTMN_APPOINTPARAM
* 描  述:	查询指定终端参数
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnTMN_APPOINTPARAM(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		CDataPack::Instance()->PackPARAM_GET_ACK();
	}

	return TRUE;
}

/************************************************************************
* 函数名:	OnTMN_ATTRIBUTE
* 描  述:	查询终端属性
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnTMN_ATTRIBUTE(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		CIniFile	IniFile;
		//加载文件失败
		if( !IniFile.Load( PATH_CONFIG_INI ) )
		{
//			AfxMessageBox(_T("加载config.ini文件失败！"));
			return FALSE;
		}

		CHAR	szManufacturerID[24] = {0};		//制造商ID，5byte
		CHAR	szTerminalType[24] = {0};		//终端型号，8byte
		CHAR	szTerminalID[24] = {0};			//终端ID，7byte
		CHAR	szHardwareVersion[256] = {0};	//硬件版本
		CHAR	szFirmwareVersion[256] = {0};	//固件版本

		IniFile.GetString(_T("Set"), _T("ManufacturerID"), szManufacturerID, sizeof(szManufacturerID));
		IniFile.GetString(_T("Set"), _T("TerminalType"), szTerminalType, sizeof(szTerminalType));
		IniFile.GetString(_T("Set"), _T("TerminalID"), szTerminalID, sizeof(szTerminalID));
		IniFile.GetString(_T("WirelessUpgrade"), _T("HardwareVersion"), szHardwareVersion, sizeof(szHardwareVersion));
		IniFile.GetString(_T("WirelessUpgrade"), _T("FirmwareVersion"), szFirmwareVersion, sizeof(szFirmwareVersion));
//		IniFile.Release();

		CDataPack::Instance()->PackTMN_ATTRIBUTE(szManufacturerID, szTerminalType, szTerminalID, szHardwareVersion, szFirmwareVersion);
	}
	return TRUE;
}

/************************************************************************
* 函数名:	OnTEMP_ALARM_SMS
* 描  述:	人工确认报警消息
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnTEMP_ALARM_SMS(enResult Result)
{
	return TRUE;
}