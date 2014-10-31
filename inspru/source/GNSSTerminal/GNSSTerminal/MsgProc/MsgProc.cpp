#include "StdAfx.h"
#include "MsgProc.h"
#include "../DataParser/DataType.h"
#include "../DataParser/DataParser.h"
#include "../Tools/LogTool.h"
#include "../DataParser/DataPack.h"
#include "../TerminalConfig/TerminalConfig.h"
#include "../GPSData/GPSData.h"
#include "../DataParser/ComManager.h"

//menu.exeͬGNSSTerminal.exe����֮�佻������ϢID
UINT	g_nWM_MENU_GNSSTERMINAL	= RegisterWindowMessage(_T("MENU_GNSSTERMINAL"));
UINT    g_nWM_MsgIDPosRefresh	= RegisterWindowMessage( WM_POS_DATA_REFRESH );


const POS_INFO*	g_pGpsPosInfo;
//��ʼ����̬��Ա����
CMsgProc*	CMsgProc::m_pInstance	= NULL;

MSG_PROC	g_msg_proc[] = 
{
	WM_SERVER,					&CMsgProc::OnServerCommand,		//���յ�������socket���ݴ�����Ϣ
	g_nWM_MENU_GNSSTERMINAL,	&CMsgProc::OnUIMsg,				//UI����Ϣ
	g_nWM_MsgIDPosRefresh,      &CMsgProc::OnGpsMsg,
	MSG_TO_OUTSIDEPROCESS,		&CMsgProc::OnGuider4uMsg,		//���������Ϣ
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

	m_hGNSSMainWnd	= hMainWnd;
	m_pGNSSMainWnd	= pMainWnd;

	m_hMenuMainWnd = ::FindWindow ( NULL, MenuApp );

	g_pGpsPosInfo = GetSharePosInfo();
	return TRUE;
}

/************************************************************************
* ������:	SendServerMsg
* ��  ��:	������Ϣ����ǰ����������
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
LRESULT CMsgProc::SendServerMsg( WPARAM wParam, LPARAM lParam, BOOL bSynchronous /*= TRUE*/ )
{
	if(NULL == m_hGNSSMainWnd)
		return 0;

	if(!bSynchronous)	//ͬ����Ϣ
		return ::SendMessage(m_hGNSSMainWnd, WM_SERVER, wParam, lParam);
	else
		return ::PostMessage(m_hGNSSMainWnd, WM_SERVER, wParam, lParam);
}

/************************************************************************
* ������:	SendMsg2Menu
* ��  ��:	������Ϣ��Menu.exe��������
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
LRESULT CMsgProc::SendMsg2Menu( WPARAM wParam, LPARAM lParam/* = 0*/, BOOL bSynchronous /*= TRUE*/ )
{
	//menu.exe�����ھ��
	if( IsWindow(m_hMenuMainWnd) == 0 )
	{
		m_hMenuMainWnd = ::FindWindow ( NULL, MenuApp );
		if(NULL == m_hMenuMainWnd)
			return 0;
	}

	//������Ϣ
	if(bSynchronous)	//ͬ����Ϣ
		return ::SendMessage(m_hMenuMainWnd, g_nWM_MENU_GNSSTERMINAL, wParam, lParam);
	else
		return ::PostMessage(m_hMenuMainWnd, g_nWM_MENU_GNSSTERMINAL, wParam, lParam);
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

/************************************************************************
* ������:	OnServerCommand
* ��  ��:	������ָ�����
* ��  ��:	1.wParam: ��2�ֽڣ��ظ�����     ��2�ֽڣ��ظ��Ƿ�ɹ�
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
LRESULT CMsgProc::OnServerCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case MSG_SVR_COMMON_ACK:				//2.ƽ̨ͨ��Ӧ��0x8001
		return OnCOMMON_ACK((enResult)lParam);
	case MSG_SVR_SUBCONTRACT:				//3.�����ְ�����0x8003
		return OnSUBCONTRACT((enResult)lParam);	
	case MSG_SVR_REGISTER_ACK:				//5.�ն�ע��Ӧ��0x8100
		return OnREGISTER_ACK((enResult)lParam);
	case MSG_SVR_PARAM_SET:					//8.�����ն˲���0x8103
		return OnPARAM_SET((enResult)lParam);
	case MSG_SVR_PARAM_GET:					//9.��ѯ�ն˲���0x8104
		return OnPARAM_GET((enResult)lParam);
	case MSG_SVR_TMN_CONTROL:				//11.�ն˿���0x8105
		return OnTMN_CONTROL( (enResult)lParam, (enTMN_CONTROL)(lParam&0x0000FFFF) );
	case MSG_SVR_TMN_APPOINT_PARAMETER:		//12.��ѯָ���ն˲���
		return OnTMN_APPOINTPARAM((enResult)lParam);
	case MSG_SVR_TMN_ATTRIBUTE:				//.��ѯ�ն�����
		return OnTMN_ATTRIBUTE((enResult)lParam);
	case MSG_SVR_POS_GET:					//13.λ����Ϣ��ѯ0x8201
		return OnPOS_GET((enResult)lParam);
	case MSG_SVR_TEMP_POS_TRACE:			//15.��ʱλ�ø��ٿ���0x8202
		return OnTEMP_POS_TRACE((enResult)lParam);
	case MSG_SVR_ALARM_SMS:					//�˹�ȷ�ϱ�����Ϣ
		return OnTEMP_ALARM_SMS((enResult)lParam);
	case MSG_SVR_TEXT_SMS:					//16.�ı���Ϣ�·�0x8300
		return OnTEXT_SMS((enResult)lParam);
	case MSG_SVR_EVENT_SET:					//17.�¼�����0x8301
		return OnEVENT_SET((enResult)lParam);
	case MSG_SVR_QUESTION:					//19.�����·�0x8302
		return OnQUESTION((enResult)lParam);
	case MSG_SVR_INFO_MENU_SET:				//21.��Ϣ�㲥�˵�����0x8303
		return OnINFO_MENU_SET((enResult)lParam);
	case MSG_SVR_INFO_SERVICE:				//23.��Ϣ����0x8304
		return OnINFO_SERVICE((enResult)lParam);
	case MSG_SVR_CALL_BACK:					//24.�绰�ز�0x8400
		return OnCALL_BACK((enResult)lParam);
	case MSG_SVR_PHONE_BOOK_SET:			//25.���õ绰��0x8401
		return OnPHONE_BOOK_SET((enResult)lParam);
	case MSG_SVR_CAR_CONTROL:				//26.��������0x8500
		return OnCAR_CONTROL((BYTE)lParam);
	case MSG_SVR_CIRC_REGION_SET:			//28.����Բ������0x8600
		return OnCIRC_REGION_SET( (enResult)lParam );
	case MSG_SVR_CIRC_REGION_DEL:			//29.ɾ��Բ������0x8601
		return OnCIRC_REGION_DEL( (enResult)lParam );
	case MSG_SVR_RECT_REGION_SET:			//30.���þ�������0x8602
		return OnRECT_REGION_SET( (enResult)lParam );
	case MSG_SVR_RECT_REGION_DEL:			//31.ɾ����������0x8603
		return OnRECT_REGION_DEL( (enResult)lParam );
	case MSG_SVR_NGON_REGION_SET:			//32.���ö��������0x8604
		return OnNGON_REGION_SET( (enResult)lParam );
	case MSG_SVR_NGON_REGION_DEL:			//33.ɾ�����������0x8605
		return OnNGON_REGION_DEL( (enResult)lParam );
	case MSG_SVR_ROUTE_SET:					//34.����·��0x8606
		return OnROUTE_SET( (enResult)lParam );
	case MSG_SVR_ROUTE_DEL:					//35.ɾ��·��0x8607
		return OnROUTE_DEL( (enResult)lParam );
	case MSG_SVR_DRIVE_RECORD_COLLECT:		//36.��ʻ��¼���ݲɼ�����0x8700
		return OnDRIVE_RECORD_COLLECT( (BYTE)lParam );
	case MSG_SVR_DRIVE_RECORD_PARAM_SET:	//38.��ʻ��¼�����´�����0x8701
		return OnDRIVE_RECORD_PARAM_SET( (BYTE)lParam );
	case MSG_SVR_MULTIMEDIA_UPLOAD_ACK:		//43.��ý�������ϴ�Ӧ��0x8800
		return OnMULTIMEDIA_UPLOAD_ACK( (enResult)lParam );
	case MSG_SVR_CAMERA_PHOTOGRAPH:			//44.����ͷ������������0x8801
		return OnCAMERA_PHOTOGRAPH( (enResult)lParam );
	case MSG_SVR_MULTIMEDIA_SEARCH:			//45.�洢��ý�����ݼ���0x8802
		return OnMULTIMEDIA_SEARCH( (enResult)lParam );
	case MSG_SVR_MULTIMEDIA_UPLOAD:			//47.�洢��ý�������ϴ�����0x8803
		return OnMULTIMEDIA_UPLOAD( (enResult)lParam );
	case MSG_SVR_SOUND_RECORD:				//48.¼����ʼ����0x8804
		return OnSOUND_RECORD(lParam);
	case MSG_SVR_DATA_DOWNLOAD:				//49.��������͸��0x8900
		return OnDATA_DOWNLOAD( (enResult)lParam );
	case MSG_SVR_RSA_PUBLIC_KEY:			//52.ƽ̨RSA��Կ0x8A00
		return OnRSA_PUBLIC_KEY( (enResult)lParam );
	case MSG_TMN_DESTINATION_NAVI:			//��2.ƽ̨�·�"Ŀ�ĵص���"ָ��0xFF00
		return OnDESTINATION_NAVI(lParam);
	case MSG_SVR_SINGLE_MULT_SEARCH:
		return OnAudioUpload((enResult)lParam);
	default:	return FALSE;
	}

	return 1;
}

//UI����������Ϣ����
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
* ������:	OnCOMMON_ACK
* ��  ��:	2.ƽ̨ͨ��Ӧ��0x8001
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnCOMMON_ACK(enResult Result)
{
	const PCOMMON_ACK	pCommonAck = CDataParser::Instance()->GetCommonAck();
	if( RESULT_SUCCESS == Result )
	{
		switch( pCommonAck->wAckMsgID )
		{
		case MSG_TMN_AUTHENTICATION:		//7.�ն˼�Ȩ0x0102
			{
				//��¼״̬ΪTRUE
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
		case MSG_TMN_LOGOUT:				//6.�ն�ע��0x0003
			//��¼״̬ΪFALSE
			SetLoginState(FALSE);
			break;
		case MSG_TMN_EVENT_REPORT:			//18.�¼�����0x0301
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
		case MSG_TMN_AUTHENTICATION:		//7.�ն˼�Ȩ0x0102				
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
* ������: OnREGISTER_ACK
* ��  ��: 5.�ն�ע��Ӧ��0x8100
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnREGISTER_ACK(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		//��Ȩ
		CDataPack::Instance()->PackAUTHENTICATION();
	}
	else
	{
		CDataPack::Instance()->PackCOMMON_ACK(Result);
	}

	return TRUE;
}

/************************************************************************
* ������:	OnPARAM_SET
* ��  ��:	8.�����ն˲���0x8103
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnPARAM_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnPARAM_GET
* ��  ��:	9.��ѯ�ն˲���0x8104
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	OnTMN_CONTROL
* ��  ��:	11.�ն˿���0x8105
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnTMN_CONTROL(enResult Result, enTMN_CONTROL enControl)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnPOS_GET
* ��  ��:	13.λ����Ϣ��ѯ0x8201
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	OnPOS_GET
* ��  ��:	15.��ʱλ�ø��ٿ���0x8202
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	OnTEXT_SMS
* ��  ��:	16.�ı���Ϣ�·�0x8300
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnTEXT_SMS(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
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
BOOL CMsgProc::OnEVENT_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
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
BOOL CMsgProc::OnQUESTION(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
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
BOOL CMsgProc::OnINFO_MENU_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
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
BOOL CMsgProc::OnINFO_SERVICE(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnCALL_BACK
* ��  ��:	24.�绰�ز�0x8400
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnCALL_BACK(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnPHONE_BOOK_SET
* ��  ��:	25.���õ绰��0x8401
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnPHONE_BOOK_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	SendMsg2Menu(MSG_SVR_PHONE_BOOK_SET);
	return TRUE;
}

/************************************************************************
* ������:	OnCAR_CONTROL
* ��  ��:	26.��������0x8500
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnCAR_CONTROL(BYTE cControlFlag)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}


/************************************************************************
* ������:	OnCIRC_REGION_SET
* ��  ��:	28.����Բ������0x8600
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnCIRC_REGION_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnCIRC_REGION_DEL
* ��  ��:	29.ɾ��Բ������0x8601
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnCIRC_REGION_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnRECT_REGION_SET
* ��  ��:	30.���þ�������0x8602
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnRECT_REGION_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnRECT_REGION_DEL
* ��  ��:	31.ɾ����������0x8603
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnRECT_REGION_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnNGON_REGION_SET
* ��  ��:	32.���ö��������0x8604
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnNGON_REGION_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnNGON_REGION_DEL
* ��  ��:	33.ɾ�����������0x8605
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnNGON_REGION_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
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
BOOL CMsgProc::OnROUTE_SET(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnROUTE_DEL
* ��  ��:	35.ɾ��·��0x8607
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnROUTE_DEL(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnDRIVE_RECORD_COLLECT
* ��  ��:	36.��ʻ��¼���ݲɼ�����0x8700
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnDRIVE_RECORD_COLLECT(BYTE cCMD)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnDRIVE_RECORD_PARAM_SET
* ��  ��:	38.��ʻ��¼�����´�����0x8701
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnDRIVE_RECORD_PARAM_SET(BYTE cCMD)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}
/************************************************************************
* ������:	OnMULTIMEDIA_UPLOAD_ACK
* ��  ��:	43.��ý�������ϴ�Ӧ��0x8800
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnMULTIMEDIA_UPLOAD_ACK(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnCAMERA_PHOTOGRAPH
* ��  ��:	44.����ͷ������������0x8801
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnCAMERA_PHOTOGRAPH(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	SendMsg2Menu(MSG_SVR_CAMERA_PHOTOGRAPH);
	return TRUE;
}

/************************************************************************
* ������:	OnMULTIMEDIA_SEARCH
* ��  ��:	45.�洢��ý�����ݼ���0x8802
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnMULTIMEDIA_SEARCH(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnMULTIMEDIA_UPLOAD
* ��  ��:	47.�洢��ý�������ϴ�����0x8803
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnMULTIMEDIA_UPLOAD(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnSOUND_RECORD
* ��  ��:	48.¼����ʼ����0x8804
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnSOUND_RECORD(DWORD dwSoundRecording)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);
	return TRUE;
}

/************************************************************************
* ������:	OnDATA_DOWNLOAD
* ��  ��:	49.��������͸��0x8900
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	OnRSA_PUBLIC_KEY
* ��  ��:	52.ƽ̨RSA��Կ0x8A00
* ��  ��:	1.bSuccess:	�ظ��Ƿ�ɹ�
*			2.lParam:	Ӧ����ˮ��
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnRSA_PUBLIC_KEY(enResult Result)
{
	CDataPack::Instance()->PackCOMMON_ACK(Result);
	return TRUE;
}

/************************************************************************
* ������:	OnDESTINATION_NAVI
* ��  ��:	��2.ƽ̨�·�"Ŀ�ĵص���"ָ��0xFF00
* ��  ��:	1.Result:	�ظ��Ƿ�ɹ�
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnDESTINATION_NAVI(DWORD dwLastUID)
{
	CDataPack::Instance()->PackCOMMON_ACK(RESULT_SUCCESS);

	return TRUE;
}
/************************************************************************
* ������:	ConnectSocket
* ��  ��:	����socket
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
************************************************************************/
BOOL CMsgProc::ConnectSocket()
{
	//��ȡ�ն˲����ṹ��
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();

	

	CLogTool::Instance()->WriteLogFile("Connecting to IP:%s:%d...", pParam->szMAIN_SVR_IP, pParam->dwSVR_TCP_PORT );

	AuthenticateTimes = 0;

	BOOL bRet = CComManager::Instance()->OpenSocketConnect(pParam->szMAIN_SVR_IP, (WORD)pParam->dwSVR_TCP_PORT);

	if (bRet == TRUE)
	{
		CLogTool::Instance()->WriteLogFile("success!");
		

		//ע���Ȩ
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
	//�����ļ�ʧ��
	if( !IniFile.Load( PATH_CONFIG_INI ) )
	{
//		AfxMessageBox(_T("����config.ini�ļ�ʧ�ܣ�"));
		return;
	}

	CHAR	szAuthenCode[24] = {0};			//��Ȩ��

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
* ������:	Register
* ��  ��:	ע��
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
************************************************************************/
VOID CMsgProc::Register()
{
	CIniFile	IniFile;
	//�����ļ�ʧ��
	if( !IniFile.Load( PATH_CONFIG_INI ) )
	{
//		AfxMessageBox(_T("����config.ini�ļ�ʧ�ܣ�"));
		return;
	}

	CHAR	szPhoneNum[24] = {0};		//�ն�SIM������
	CHAR	szManufacturerID[24] = {0};	//������ID��5byte
	CHAR	szTerminalType[24] = {0};	//�ն��ͺţ�8byte
	CHAR	szTerminalID[24] = {0};		//�ն�ID��7byte
	WORD	wEncryptionType = 0;		//���ݼ��ܷ�ʽ 0:������ 1:RSA����
		
	IniFile.GetString(_T("Set"), _T("PhoneNum"), szPhoneNum, sizeof(szPhoneNum));
	IniFile.GetString(_T("Set"), _T("ManufacturerID"), szManufacturerID, sizeof(szManufacturerID));
	IniFile.GetString(_T("Set"), _T("TerminalType"), szTerminalType, sizeof(szTerminalType));
	IniFile.GetString(_T("Set"), _T("TerminalID"), szTerminalID, sizeof(szTerminalID));
	IniFile.GetInt(_T("Set"), _T("EncryptionType"), wEncryptionType);
//	IniFile.Release();

	//�������ݰ����Զ�
	CDataPack::Instance()->SetMsgProperty(szPhoneNum, (BYTE)wEncryptionType);
	//ע��
	const PTERMINAL_PARAM	pParam	= CTerminalConfig::Instance()->GetTerminalParam();
	CDataPack::Instance()->PackREGISTER(pParam->wVEHICLE_PROVINCE_ID, pParam->wVEHICLE_CITY_ID, szManufacturerID, szTerminalType, 
		szTerminalID, (BYTE)pParam->dwPLATE_COLOR, pParam->szVEHICLE_ID, strlen(pParam->szVEHICLE_ID));
}

/************************************************************************
* ������:	Register
* ��  ��:	����������Ϣ
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
************************************************************************/
VOID CMsgProc::LoadConfig()
{
	CIniFile	IniFile;
	//�����ļ�ʧ��
	if( !IniFile.Load( PATH_CONFIG_INI ) )
	{
//		AfxMessageBox(_T("����config.ini�ļ�ʧ�ܣ�"));
		return;
	}

	CHAR	szPhoneNum[24] = {0};		//�ն�SIM������
// 	CHAR	szManufacturerID[24] = {0};	//������ID��5byte
// 	CHAR	szTerminalType[24] = {0};	//�ն��ͺţ�8byte
// 	CHAR	szTerminalID[24] = {0};		//�ն�ID��7byte
	WORD	wEncryptionType = 0;		//���ݼ��ܷ�ʽ 0:������ 1:RSA����

	IniFile.GetString(_T("Set"), _T("PhoneNum"), szPhoneNum, sizeof(szPhoneNum));
// 	IniFile.GetString(_T("Set"), _T("ManufacturerID"), szManufacturerID, sizeof(szManufacturerID));
// 	IniFile.GetString(_T("Set"), _T("TerminalType"), szTerminalType, sizeof(szTerminalType));
// 	IniFile.GetString(_T("Set"), _T("TerminalID"), szTerminalID, sizeof(szTerminalID));
	IniFile.GetInt(_T("Set"), _T("EncryptionType"), wEncryptionType);
//	IniFile.Release();

	//�������ݰ����Զ�
	CDataPack::Instance()->SetMsgProperty(szPhoneNum, (BYTE)wEncryptionType);
}

/************************************************************************
* ������:	SetLoginState
* ��  ��:	���õ�¼״̬
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
************************************************************************/
VOID CMsgProc::SetLoginState(BOOL bIsLogin)
{

	//������Ϣ��menu.exe

	HWND hWndSend = ::FindWindow (NULL,MenuApp );
	//������Ϣ
	if (hWndSend != NULL)
	{
		::PostMessage(hWndSend, WM_COMMAND_TIANZE, WPARAM_SERVER_LOGIN, (LPARAM)bIsLogin);
	}

	m_bIsLogin	= bIsLogin;
}

/************************************************************************
* ������:	GetLoginState
* ��  ��:	��ȡ��¼״̬
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
************************************************************************/
BOOL CMsgProc::GetLoginState()
{

	return m_bIsLogin;
}

/************************************************************************
* ������:	ThrdHeartBeat
* ��  ��:	�����߳�
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
************************************************************************/
DWORD CMsgProc::ThrdHeartBeat( LPVOID lpParameter )
{

	CMsgProc *Proc = (CMsgProc *)lpParameter;

	const PTERMINAL_PARAM pParam = CTerminalConfig::Instance()->GetTerminalParam();

	while ( TRUE )
	{
		Sleep( pParam->dwHEART_BEAT_SLICE * 1000 );

		//��¼�Ϸ�������ʼ����
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

	::PostMessage(HWND_BROADCAST, MSG_RAS_RECONNECT, 2, 0); //����ras
	return FALSE;
}

/************************************************************************
* ������:	BeginHeartBeat
* ��  ��:	���������߳�
* ��  ��:	
* ��  ��:	
* ��  ��:	
* ��  ע:
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
* ������:	OnSUBCONTRACT
* ��  ��:	3.�����ְ�����0x8003
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnSUBCONTRACT(enResult Result)
{
	return TRUE;
}

/************************************************************************
* ������:	OnTMN_APPOINTPARAM
* ��  ��:	��ѯָ���ն˲���
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
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
* ������:	OnTMN_ATTRIBUTE
* ��  ��:	��ѯ�ն�����
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnTMN_ATTRIBUTE(enResult Result)
{
	if(RESULT_SUCCESS == Result)
	{
		CIniFile	IniFile;
		//�����ļ�ʧ��
		if( !IniFile.Load( PATH_CONFIG_INI ) )
		{
//			AfxMessageBox(_T("����config.ini�ļ�ʧ�ܣ�"));
			return FALSE;
		}

		CHAR	szManufacturerID[24] = {0};		//������ID��5byte
		CHAR	szTerminalType[24] = {0};		//�ն��ͺţ�8byte
		CHAR	szTerminalID[24] = {0};			//�ն�ID��7byte
		CHAR	szHardwareVersion[256] = {0};	//Ӳ���汾
		CHAR	szFirmwareVersion[256] = {0};	//�̼��汾

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
* ������:	OnTEMP_ALARM_SMS
* ��  ��:	�˹�ȷ�ϱ�����Ϣ
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CMsgProc::OnTEMP_ALARM_SMS(enResult Result)
{
	return TRUE;
}