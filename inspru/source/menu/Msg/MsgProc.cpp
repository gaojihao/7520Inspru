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

PROCESS_INFORMATION		g_ProcessInfo;			//����������Ϣ
extern UINT	g_nWM_MENU_GNSSTERMINAL;
void OnKeyNavi(CString strPosInfo);
extern void TTSSpeaking(CString csText);
//��ʼ����̬��Ա����
CMsgProc*	CMsgProc::m_pInstance	= NULL;

MSG_PROC	g_msg_proc[] = 
{
	g_nWM_MENU_GNSSTERMINAL,		&CMsgProc::OnGNSSTerminal,			//GNSSTERMINAL.exe���̷��͵���Ϣ
	WM_HARDWARE_BUTTON,				&CMsgProc::OnHardButton,			//Ӳ����
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
* ������: Instance
* ��  ��: 
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: singleton
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
* ������:	SetMainWnd
* ��  ��:	���������ھ���������ָ��
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	MsgProc
* ��  ��:	��Ϣ������
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
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

//GNSS ����������Ϣ����
LRESULT	CMsgProc::OnGNSSTerminal(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case MSG_SVR_TEXT_SMS:					//16.�ı���Ϣ�·�0x8300
		//return OnTEXT_SMS();
	case MSG_TMN_DESTINATION_NAVI:			//��2.ƽ̨�·�"Ŀ�ĵص���"ָ��0x8F01
//		return OnDESTINATION_NAVI(lParam);
 	case MSG_SVR_COMMON_ACK:				//2.ƽ̨ͨ��Ӧ��0x8001
 		return OnCOMMON_ACK(lParam);
// 	case MSG_SVR_REGISTER_ACK:				//5.�ն�ע��Ӧ��0x8100
// 		return OnREGISTER_ACK((enResult)lParam);
// 	case MSG_SVR_PARAM_SET:					//8.�����ն˲���0x8103
// 		return OnPARAM_SET((enResult)lParam);
// 	case MSG_SVR_PARAM_GET:					//9.��ѯ�ն˲���0x8104
// 		return OnPARAM_GET((enResult)lParam);
// 	case MSG_SVR_TMN_CONTROL:				//11.�ն˿���0x8105
// 		return OnTMN_CONTROL( (enResult)(lParam>>16), (enTMN_CONTROL)(lParam&0x0000FFFF) );
// 	case MSG_SVR_POS_GET:					//13.λ����Ϣ��ѯ0x8201
// 		return OnPOS_GET((enResult)lParam);
// 	case MSG_SVR_TEMP_POS_TRACE:			//15.��ʱλ�ø��ٿ���0x8202
// 		return OnTEMP_POS_TRACE((enResult)lParam);
 	case MSG_SVR_EVENT_SET:					//17.�¼�����0x8301
 		//return OnEVENT_SET(/*(enResult)*/lParam);
	case MSG_SVR_QUESTION:					//19.�����·�0x8302
		//return OnQUESTION(/*(enResult)*/lParam);
	case MSG_SVR_INFO_MENU_SET:				//21.��Ϣ�㲥�˵�����0x8303
		//return OnINFO_MENU_SET(/*(enResult)*/lParam);
// 	case MSG_SVR_INFO_SERVICE:				//23.��Ϣ����0x8304
// 		return OnINFO_SERVICE((enResult)lParam);
 	case MSG_SVR_CALL_BACK:					//24.�绰�ز�0x8400
 		return OnCALL_BACK((BYTE)lParam);
// 	case MSG_SVR_PHONE_BOOK_SET:			//25.���õ绰��0x8401
// 		return OnPHONE_BOOK_SET((enResult)lParam);
// 	case MSG_SVR_CAR_CONTROL:				//26.��������0x8500
// 		return OnCAR_CONTROL((BYTE)lParam);
// 	case MSG_SVR_CIRC_REGION_SET:			//28.����Բ������0x8600
// 		return OnCIRC_REGION_SET( (enResult)lParam );
// 	case MSG_SVR_CIRC_REGION_DEL:			//29.ɾ��Բ������0x8601
// 		return OnCIRC_REGION_DEL( (enResult)lParam );
// 	case MSG_SVR_RECT_REGION_SET:			//30.���þ�������0x8602
// 		return OnRECT_REGION_SET( (enResult)lParam );
// 	case MSG_SVR_RECT_REGION_DEL:			//31.ɾ����������0x8603
// 		return OnRECT_REGION_DEL( (enResult)lParam );
// 	case MSG_SVR_NGON_REGION_SET:			//32.���ö��������0x8604
// 		return OnNGON_REGION_SET( (enResult)lParam );
// 	case MSG_SVR_NGON_REGION_DEL:			//33.ɾ�����������0x8605
// 		return OnNGON_REGION_DEL( (enResult)lParam );
// 	case MSG_SVR_ROUTE_SET:					//34.����·��0x8606
// 		return OnROUTE_SET( (enResult)lParam );
// 	case MSG_SVR_ROUTE_DEL:					//35.ɾ��·��0x8607
// 		return OnROUTE_DEL( (enResult)lParam );
// 	case MSG_SVR_DRIVE_RECORD_COLLECT:		//36.��ʻ��¼���ݲɼ�����0x8700
// 		return OnDRIVE_RECORD_COLLECT( (BYTE)lParam );
// 	case MSG_SVR_DRIVE_RECORD_PARAM_SET:	//38.��ʻ��¼�����´�����0x8701
// 		return OnDRIVE_RECORD_PARAM_SET( (BYTE)lParam );
// 	case MSG_SVR_MULTIMEDIA_UPLOAD_ACK:		//43.��ý�������ϴ�Ӧ��0x8800
// 		return OnMULTIMEDIA_UPLOAD_ACK( (enResult)lParam );
	case MSG_SVR_CAMERA_PHOTOGRAPH:			//44.����ͷ������������0x8801
//		return OnCAMERA_PHOTOGRAPH(/* (enResult)*/lParam );
// 	case MSG_SVR_MULTIMEDIA_SEARCH:			//45.�洢��ý�����ݼ���0x8802
// 		return OnMULTIMEDIA_SEARCH( (enResult)lParam );
// 	case MSG_SVR_MULTIMEDIA_UPLOAD:			//47.�洢��ý�������ϴ�����0x8803
// 		return OnMULTIMEDIA_UPLOAD( (enResult)lParam );
// 	case MSG_SVR_SOUND_RECORD:				//48.¼����ʼ����0x8804
// 		return OnSOUND_RECORD(lParam);
// 	case MSG_SVR_DATA_DOWNLOAD:				//49.��������͸��0x8900
// 		return OnDATA_DOWNLOAD( (enResult)lParam );
// 	case MSG_SVR_RSA_PUBLIC_KEY:			//52.ƽ̨RSA��Կ0x8A00
// 		return OnRSA_PUBLIC_KEY( (enResult)lParam );
	default:	return FALSE;
	}

	return 1;
}

//Ӳ����
LRESULT	CMsgProc::OnHardButton(WPARAM wParam, LPARAM lParam)
{
	//HWND hWnd = ::FindWindow(NULL, WNDTITLE_CAMERA_VIDEO);
	//if(NULL != hWnd || CDlgReportState::IsOpen())
	//{
	//	return 0;
	//}

	//if (WPARAM_BTN_2 == wParam)//����1
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 101);
	//}
	//else if (WPARAM_BTN_3 == wParam)//����2
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 102);
	//}
	//else if (WPARAM_BTN_4 == wParam)//����3
	//{
	//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_EVENT_REPORT, 103);
	//}
	//else if (WPARAM_BTN_5 == wParam)//����4
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
	//	pDlg->ReportState(wParam);	//����״̬
	//}

	return 1;
}

/************************************************************************
* ������:	OnTEXT_SMS
* ��  ��:	16.�ı���Ϣ�·�0x8300
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnTEXT_SMS()
{
	if( CDlgTextInfo::m_bIsOpen )
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgTextInfo_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:Ϊ���µļ�¼index
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
			db.open(PATH_SQLITE_DB_808);	//�����ݿ�

			//��ѯ��¼������
			int m_nRecordCount = db.execScalar("select count(*) from text_info WHERE text_type = 0;");

			int nRecordIndex = 0;
			//�����ݿ��в�ѯ��nRecordIndex������
			char szSqlBuffer[512];
			sprintf(szSqlBuffer, "Select * From text_info WHERE text_type = 0 ORDER BY text_datatime DESC limit %d, 1;", nRecordIndex);
			CppSQLite3Query q = db.execQuery(szSqlBuffer);

			int nFlag = 0;

			if ( !q.eof() )
			{

				TCHAR  szBuf[300];

				nFlag	= q.getIntField("flag");	//��־

				
				//��������
				memset(szBuf, 0, sizeof(szBuf) );
				ex_char2uni( q.fieldValue("text_content"), szBuf,300 );
				strTxt.Format(_T("%s"), szBuf);
			}
			//�ͷ�statement
			q.finalize();

			db.close();	//�ر����ݿ�

			//����Ϊ����ʱ����ҪTTS����
			if(nFlag & 0x08)
				TTSSpeaking( strTxt );
		}
	}

	return TRUE;
}

/************************************************************************
* ������:	OnDESTINATION_NAVI
* ��  ��:	��2.ƽ̨�·�"Ŀ�ĵص���"ָ��0x8F01
* ��  ��:	1.Result:	�ظ��Ƿ�ɹ�
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnDESTINATION_NAVI(DWORD dwLastUID)
{
	//��ѯ���ݿ�
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//�����ݿ�
	char szSql[512];
	memset(szSql, 0, sizeof(szSql));
	CString strPosInfo;
	CHAR cInfo[512];
	memset(cInfo, NULL, sizeof(cInfo));

	//�����ݿ��в�ѯ����
	sprintf(szSql, "Select * From text_info where UID = %d;", dwLastUID);
	CppSQLite3Query q = db.execQuery(szSql);

	if ( !q.eof() )
	{
		strPosInfo	= q.fieldValue("text_content");
		strcpy(cInfo,  q.fieldValue("text_content"));
	}
	//�ͷ�statement
	q.finalize();
	db.close();	//�ر����ݿ�

	strPosInfo = cInfo;

	OnKeyNavi(strPosInfo);
	return TRUE;
}

/************************************************************************
* ������:	OnCOMMON_ACK
* ��  ��:	1.ͨ��Ӧ�� 0x0301
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	OnEVENT_SET
* ��  ��:	17.�¼�����0x8301
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnEVENT_SET(DWORD dwLastUID/*enResult Result*/)
{
	if (CDlgEventList::IsOpen())
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgEventInfo_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:Ϊ���µļ�¼index
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
* ������:	OnQUESTION
* ��  ��:	19.�����·�0x8302
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnQUESTION(DWORD dwLastUID/*enResult Result*/)
{
// 	CString str = _T("");
	CppSQLite3Query q;
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//�����ݿ�
	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));

	int nRecordIndex = 0;
	//sprintf(szSqlBuffer, "SELECT * FROM question WHERE UID = '%d';", dwLastUID);
	sprintf(szSqlBuffer, "Select * From question WHERE read_status = 0 ORDER BY question_datatime DESC limit %d, 1;", nRecordIndex);
	q = db.execQuery(szSqlBuffer);

	int nID = q.getIntField("UID");	//ID

//	str = q.fieldValue("question_content");	//��ȡ�ı�����
//	int nFlag = q.getIntField("flag");	//��־

// 	if (nFlag)
// 	{
// 		TTSSpeaking( str );
// 	}

	q.finalize();
	db.close();	//�ر����ݿ�

	if (CDlgAnswerList::IsOpen())
	{
		HWND hWnd = ::FindWindow(NULL, _T("DlgAnswerList_WINDOW_NAME"));
		if(hWnd != NULL)	//wParam:Ϊ���µļ�¼index
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
* ������:	OnINFO_MENU_SET
* ��  ��:	21.��Ϣ�㲥�˵�����0x8303
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnINFO_MENU_SET(DWORD dwLastUID/*enResult Result*/)
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//�����ݿ�

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
		if(hWnd != NULL)	//wParam:Ϊ���µļ�¼index
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
* ������:	OnINFO_SERVICE
* ��  ��:	23.��Ϣ����0x8304
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
// BOOL CMsgProc::OnINFO_SERVICE(enResult Result)
// {
// 	return TRUE;
// }

/************************************************************************
* ������:	OnCALL_BACK
* ��  ��:	24.�绰�ز�0x8400
* ��  ��:	1.byType:	�ظ��Ƿ�ɹ�
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
 BOOL CMsgProc::OnCALL_BACK(BYTE byType)
 {
	 CppSQLite3DB db;
	 char szSql[NUM_1K];
	 db.open(PATH_SQLITE_DB_808);	//�����ݿ�

	 sprintf(szSql, "SELECT * FROM call_back;");
	 CppSQLite3Query q = db.execQuery(szSql);
	 CString chChar	= _T("");
	 chChar = q.fieldValue("phone_number");

	 db.execDML( "DELETE FROM call_back;" );
	 db.close();

	 if (0 == byType)//��ͨͨ��
	 {
		 CGSMDial dlg;
		 CGSMDial::m_UserNum = chChar;
		 CGSMDial::m_bAutoDial = TRUE;
		 dlg.DoModal();
	 }
	 else if (1 == byType)//����
	 {
		 g_GsmLogic.SetPhoneNum( chChar );
		 g_GsmLogic.GSMListen();
	 }

	return TRUE;
 }

/************************************************************************
* ������:	OnROUTE_SET
* ��  ��:	34.����·��0x8606
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
// BOOL CMsgProc::OnROUTE_SET(enResult Result)
// {
// 	return TRUE;
// }

/************************************************************************
* ������:	OnROUTE_DEL
* ��  ��:	35.ɾ��·��0x8607
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
// BOOL CMsgProc::OnROUTE_DEL(enResult Result)
// {
// 	return TRUE;
// }



/************************************************************************
* ������:	OnCAMERA_PHOTOGRAPH
* ��  ��:	44.����ͷ������������0x8801
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
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







