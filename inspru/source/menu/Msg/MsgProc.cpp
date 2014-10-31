#include "StdAfx.h"
#include "MsgProc.h"
//#include "../Tools/LogTool.h"
#include "../Msg/MsgQueue.h"

#include "../E9101Main/e9101main.h"
#include "../E9101Main/E9101MainDlg.h"
#include "../Dialog/DlgReportState.h"
#include "../E9101Main/DataType.h"
#include "../SQLite3/CppSQLite3.h"
#include "../E9101Main/Dialog/DlgTextInfo.h"
#include "../E9101Main/Dialog/DlgInfoMenu.h"
#include "../E9101Main/Dialog/DlgAnswerList.h"
#include "../E9101Main/Dialog/DlgEventList.h"
#include "../E9101Main/Dialog/DlgDemand.h"
#include "../E9101Main/DataType.h"
#include "../E9101Main/GSMDial.h"
#include "../GSM/GSMLogic.h"

#define WNDTITLE_CAMERA_VIDEO				_T("CAMERA_VIDEO_20110411")

PROCESS_INFORMATION		g_ProcessInfo;			//进程启动信息
extern UINT	g_nWM_MENU_GNSSTERMINAL;
void OnKeyNavi(CString strPosInfo);
extern void TTSSpeaking(CString csText);
//初始化静态成员变量
CMsgProc*	CMsgProc::m_pInstance	= NULL;

MSG_PROC	g_msg_proc[] = 
{
	g_nWM_MENU_GNSSTERMINAL,		&CMsgProc::OnGNSSTerminal,			//GNSSTERMINAL.exe进程发送的消息
	WM_HARDWARE_BUTTON,				&CMsgProc::OnHardButton,			//硬按键
};

CMsgProc::CMsgProc(void)
{
	m_hMainWnd		= NULL;
	m_pMainWnd		= NULL;
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

	m_hMainWnd	= hMainWnd;
	m_pMainWnd	= pMainWnd;
	return TRUE;
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

//GNSS 发送来的消息处理
LRESULT	CMsgProc::OnGNSSTerminal(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case MSG_SVR_TEXT_SMS:					//16.文本信息下发0x8300
		//return OnTEXT_SMS();
	case MSG_TMN_DESTINATION_NAVI:			//附2.平台下发"目的地导航"指令0x8F01
//		return OnDESTINATION_NAVI(lParam);
 	case MSG_SVR_COMMON_ACK:				//2.平台通用应答0x8001
 		return OnCOMMON_ACK(lParam);
// 	case MSG_SVR_REGISTER_ACK:				//5.终端注册应答0x8100
// 		return OnREGISTER_ACK((enResult)lParam);
// 	case MSG_SVR_PARAM_SET:					//8.设置终端参数0x8103
// 		return OnPARAM_SET((enResult)lParam);
// 	case MSG_SVR_PARAM_GET:					//9.查询终端参数0x8104
// 		return OnPARAM_GET((enResult)lParam);
// 	case MSG_SVR_TMN_CONTROL:				//11.终端控制0x8105
// 		return OnTMN_CONTROL( (enResult)(lParam>>16), (enTMN_CONTROL)(lParam&0x0000FFFF) );
// 	case MSG_SVR_POS_GET:					//13.位置信息查询0x8201
// 		return OnPOS_GET((enResult)lParam);
// 	case MSG_SVR_TEMP_POS_TRACE:			//15.临时位置跟踪控制0x8202
// 		return OnTEMP_POS_TRACE((enResult)lParam);
 	case MSG_SVR_EVENT_SET:					//17.事件设置0x8301
 		//return OnEVENT_SET(/*(enResult)*/lParam);
	case MSG_SVR_QUESTION:					//19.提问下发0x8302
		//return OnQUESTION(/*(enResult)*/lParam);
	case MSG_SVR_INFO_MENU_SET:				//21.信息点播菜单设置0x8303
		//return OnINFO_MENU_SET(/*(enResult)*/lParam);
// 	case MSG_SVR_INFO_SERVICE:				//23.信息服务0x8304
// 		return OnINFO_SERVICE((enResult)lParam);
 	case MSG_SVR_CALL_BACK:					//24.电话回拨0x8400
 		return OnCALL_BACK((BYTE)lParam);
// 	case MSG_SVR_PHONE_BOOK_SET:			//25.设置电话本0x8401
// 		return OnPHONE_BOOK_SET((enResult)lParam);
// 	case MSG_SVR_CAR_CONTROL:				//26.车辆控制0x8500
// 		return OnCAR_CONTROL((BYTE)lParam);
// 	case MSG_SVR_CIRC_REGION_SET:			//28.设置圆形区域0x8600
// 		return OnCIRC_REGION_SET( (enResult)lParam );
// 	case MSG_SVR_CIRC_REGION_DEL:			//29.删除圆形区域0x8601
// 		return OnCIRC_REGION_DEL( (enResult)lParam );
// 	case MSG_SVR_RECT_REGION_SET:			//30.设置矩形区域0x8602
// 		return OnRECT_REGION_SET( (enResult)lParam );
// 	case MSG_SVR_RECT_REGION_DEL:			//31.删除矩形区域0x8603
// 		return OnRECT_REGION_DEL( (enResult)lParam );
// 	case MSG_SVR_NGON_REGION_SET:			//32.设置多边形区域0x8604
// 		return OnNGON_REGION_SET( (enResult)lParam );
// 	case MSG_SVR_NGON_REGION_DEL:			//33.删除多边形区域0x8605
// 		return OnNGON_REGION_DEL( (enResult)lParam );
// 	case MSG_SVR_ROUTE_SET:					//34.设置路线0x8606
// 		return OnROUTE_SET( (enResult)lParam );
// 	case MSG_SVR_ROUTE_DEL:					//35.删除路线0x8607
// 		return OnROUTE_DEL( (enResult)lParam );
// 	case MSG_SVR_DRIVE_RECORD_COLLECT:		//36.行驶记录数据采集命令0x8700
// 		return OnDRIVE_RECORD_COLLECT( (BYTE)lParam );
// 	case MSG_SVR_DRIVE_RECORD_PARAM_SET:	//38.行驶记录参数下传命令0x8701
// 		return OnDRIVE_RECORD_PARAM_SET( (BYTE)lParam );
// 	case MSG_SVR_MULTIMEDIA_UPLOAD_ACK:		//43.多媒体数据上传应答0x8800
// 		return OnMULTIMEDIA_UPLOAD_ACK( (enResult)lParam );
	case MSG_SVR_CAMERA_PHOTOGRAPH:			//44.摄像头立即拍摄命令0x8801
//		return OnCAMERA_PHOTOGRAPH(/* (enResult)*/lParam );
// 	case MSG_SVR_MULTIMEDIA_SEARCH:			//45.存储多媒体数据检索0x8802
// 		return OnMULTIMEDIA_SEARCH( (enResult)lParam );
// 	case MSG_SVR_MULTIMEDIA_UPLOAD:			//47.存储多媒体数据上传命令0x8803
// 		return OnMULTIMEDIA_UPLOAD( (enResult)lParam );
// 	case MSG_SVR_SOUND_RECORD:				//48.录音开始命令0x8804
// 		return OnSOUND_RECORD(lParam);
// 	case MSG_SVR_DATA_DOWNLOAD:				//49.数据下行透传0x8900
// 		return OnDATA_DOWNLOAD( (enResult)lParam );
// 	case MSG_SVR_RSA_PUBLIC_KEY:			//52.平台RSA公钥0x8A00
// 		return OnRSA_PUBLIC_KEY( (enResult)lParam );
	default:	return FALSE;
	}

	return 1;
}

//硬按键
LRESULT	CMsgProc::OnHardButton(WPARAM wParam, LPARAM lParam)
{
	//HWND hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
	//if(NULL != hWnd || CDlgReportState::IsOpen())
	//{
	//	return 0;
	//}

	//if (WPARAM_BTN_2 == wParam)//按键1
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 101);
	//}
	//else if (WPARAM_BTN_3 == wParam)//按键2
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 102);
	//}
	//else if (WPARAM_BTN_4 == wParam)//按键3
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 103);
	//}
	//else if (WPARAM_BTN_5 == wParam)//按键4
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 104);
	//}

	//if (WPARAM_BTN_1 == wParam)
	//{
	//	return 0;
	//}

	//if( !CDlgReportState::IsOpen())
	//{
	//	CDlgReportState* pDlg = new CDlgReportState(m_pMainWnd);
	//	pDlg->SetModal(FALSE);
	//	pDlg->Open();
	//	pDlg->ReportState(wParam);	//发送状态
	//}

	return 1;
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
BOOL CMsgProc::OnTEXT_SMS()
{
	if( CDlgTextInfo::m_bIsOpen )
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgTextInfo_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:为最新的记录index
			::PostMessage(hWnd, CDlgTextInfo::WM_DlgTextInfo_Refresh, 0, 0);
	}
	else
	{

		HWND hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
		if(NULL == hWnd)
		{
			CDlgTextInfo dlg(0);
			dlg.DoModal();
		}
		else
		{
			CString strTxt =_T("");

			CppSQLite3DB db;
			db.open(PATH_SQLITE_DB_808);	//打开数据库

			//查询记录总数量
			int m_nRecordCount = db.execScalar("select count(*) from text_info WHERE text_type = 0;");

			int nRecordIndex = 0;
			//在数据库中查询第nRecordIndex条数据
			char szSqlBuffer[512];
			sprintf(szSqlBuffer, "Select * From text_info WHERE text_type = 0 ORDER BY text_datatime DESC limit %d, 1;", nRecordIndex);
			CppSQLite3Query q = db.execQuery(szSqlBuffer);

			int nFlag = 0;

			if ( !q.eof() )
			{

				TCHAR  szBuf[300];

				nFlag	= q.getIntField("flag");	//标志

				
				//短信内容
				memset(szBuf, 0, sizeof(szBuf) );
				ex_char2uni( q.fieldValue("text_content"), szBuf,300 );
				strTxt.Format(_T("%s"), szBuf);
			}
			//释放statement
			q.finalize();

			db.close();	//关闭数据库

			//类型为短信时，需要TTS播报
			if(nFlag & 0x08)
				TTSSpeaking( strTxt );
		}
	}

	return TRUE;
}

/************************************************************************
* 函数名:	OnDESTINATION_NAVI
* 描  述:	附2.平台下发"目的地导航"指令0x8F01
* 入  参:	1.Result:	回复是否成功
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnDESTINATION_NAVI(DWORD dwLastUID)
{
	//查询数据库
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	char szSql[512];
	memset(szSql, 0, sizeof(szSql));
	CString strPosInfo;
	CHAR cInfo[512];
	memset(cInfo, NULL, sizeof(cInfo));

	//在数据库中查询数据
	sprintf(szSql, "Select * From text_info where UID = %d;", dwLastUID);
	CppSQLite3Query q = db.execQuery(szSql);

	if ( !q.eof() )
	{
		strPosInfo	= q.fieldValue("text_content");
		strcpy(cInfo,  q.fieldValue("text_content"));
	}
	//释放statement
	q.finalize();
	db.close();	//关闭数据库

	strPosInfo = cInfo;

	OnKeyNavi(strPosInfo);
	return TRUE;
}

/************************************************************************
* 函数名:	OnCOMMON_ACK
* 描  述:	1.通用应答 0x0301
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCOMMON_ACK(DWORD dwAck_result)
{
	unCOMMON_ACK_RESULT common_result;
	common_result.dwAck_result = dwAck_result;

	if( CDlgReportState::IsOpen() )
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgReportState_Refresh"));
		if(hWnd != NULL)
		{
			::PostMessage(hWnd, CDlgReportState::WM_DlgReportState_Refresh, 
				common_result.ack_result.wMsgID, common_result.ack_result.wResult);
		}
	}

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
BOOL CMsgProc::OnEVENT_SET(DWORD dwLastUID/*enResult Result*/)
{
	if (CDlgEventList::IsOpen())
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgEventInfo_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:为最新的记录index
			::PostMessage(hWnd, CDlgEventList::WM_DlgEventInfo_Refresh, WM_REFRESH_PAGE, 0);
	}
	else
	{
		CDlgEventList dlg;
		dlg.DoModal();
	}
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
BOOL CMsgProc::OnQUESTION(DWORD dwLastUID/*enResult Result*/)
{
// 	CString str = _T("");
	CppSQLite3Query q;
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));

	int nRecordIndex = 0;
	//sprintf(szSqlBuffer, "SELECT * FROM question WHERE UID = '%d';", dwLastUID);
	sprintf(szSqlBuffer, "Select * From question WHERE read_status = 0 ORDER BY question_datatime DESC limit %d, 1;", nRecordIndex);
	q = db.execQuery(szSqlBuffer);

	int nID = q.getIntField("UID");	//ID

//	str = q.fieldValue("question_content");	//获取文本内容
//	int nFlag = q.getIntField("flag");	//标志

// 	if (nFlag)
// 	{
// 		TTSSpeaking( str );
// 	}

	q.finalize();
	db.close();	//关闭数据库

	if (CDlgAnswerList::IsOpen())
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgAnswerList_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:为最新的记录index
			::PostMessage(hWnd, CDlgAnswerList::WM_DlgAnswerList_Refresh, WM_REFRESH_PAGE, nID);
	}
	else
	{
		CDlgAnswerList dlg(nID);
		dlg.DoModal();
	}
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
BOOL CMsgProc::OnINFO_MENU_SET(DWORD dwLastUID/*enResult Result*/)
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	CppSQLite3Query q;
	sprintf(szSqlBuffer, "SELECT * FROM info_menu where read_status ORDER BY info_datatime DESC LIMIT %d, %d;", 0, 1);
	q = db.execQuery(szSqlBuffer);
	int nID = q.getIntField("UID");
	q.finalize();
	db.close();

	if (CDlgDemand::IsOpen())
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgDemand_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:为最新的记录index
			::PostMessage(hWnd, CDlgDemand::WM_DlgDemand_Refresh, WM_REFRESH_PAGE, dwLastUID);
	}
	else
	{
		CDlgDemand dlg(dwLastUID);
		dlg.DoModal();
	}

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
// BOOL CMsgProc::OnINFO_SERVICE(enResult Result)
// {
// 	return TRUE;
// }

/************************************************************************
* 函数名:	OnCALL_BACK
* 描  述:	24.电话回拨0x8400
* 入  参:	1.byType:	回复是否成功
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
 BOOL CMsgProc::OnCALL_BACK(BYTE byType)
 {
	 CppSQLite3DB db;
	 char szSql[NUM_1K];
	 db.open(PATH_SQLITE_DB_808);	//打开数据库

	 sprintf(szSql, "SELECT * FROM call_back;");
	 CppSQLite3Query q = db.execQuery(szSql);
	 CString chChar	= _T("");
	 chChar = q.fieldValue("phone_number");

	 db.execDML( "DELETE FROM call_back;" );
	 db.close();

	 if (0 == byType)//普通通话
	 {
		 CGSMDial dlg;
		 CGSMDial::m_UserNum = chChar;
		 CGSMDial::m_bAutoDial = TRUE;
		 dlg.DoModal();
	 }
	 else if (1 == byType)//监听
	 {
		 g_GsmLogic.SetPhoneNum( chChar );
		 g_GsmLogic.GSMListen();
	 }

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
// BOOL CMsgProc::OnROUTE_SET(enResult Result)
// {
// 	return TRUE;
// }

/************************************************************************
* 函数名:	OnROUTE_DEL
* 描  述:	35.删除路线0x8607
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
// BOOL CMsgProc::OnROUTE_DEL(enResult Result)
// {
// 	return TRUE;
// }



/************************************************************************
* 函数名:	OnCAMERA_PHOTOGRAPH
* 描  述:	44.摄像头立即拍摄命令0x8801
* 入  参:	1.bSuccess:	回复是否成功
*			2.lParam:	应答流水号
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CMsgProc::OnCAMERA_PHOTOGRAPH(DWORD dwLastUID/*enResult Result*/)
{
	CreateProcess(PATH_CAMERA_VIDEO, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);

	Sleep(1000);
	HWND hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
	if(NULL != hWnd)
	{
		::PostMessage(hWnd, WM_COMMAND_PHOTO, WPARAM_SNAP, 0);
	}

	return TRUE;
}







