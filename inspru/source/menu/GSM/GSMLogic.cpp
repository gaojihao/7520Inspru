// GSMLogic.cpp: implementation of the CGSMLogic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../E9101Main/e9101main.h"
#include "../E9101Main/E9101MainDlg.h"
#include "GSMLogic.h"
#include "../ExTool/ex_basics.h"
#include "../ExTool/ex_debug.h"
#include "../ExTool/ex_file.h"
#include "../E9101Main/SMSInfo.h"
#include "../SQLite3/CppSQLite3.h"
#include "../ConfigFile/CIniFile.h"
#include "../E9101Main/DataType.h"

extern void TTSSpeaking(CString csText);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HWND hMainWnd;


#if defined(UNICODE) || defined(_UNICODE)
	#define   CLD_WND_NAME     L"CARELAND NAVI"
	#define   CLD_CLS_NAME     L"CARELAND_NAVIGATION"
#else
	#define   CLD_WND_NAME     "CARELAND NAVI"
	#define   CLD_CLS_NAME     "CARELAND_NAVIGATION"
#endif

#define PATH_NEW_CALL_INCOMING   ("\\Windows\\incalling.dat")
#define PATH_NEW_CALL_OUT        ("\\Windows\\outing.dat")
#define PATH_NEW_CALL_OUT_EX     ("\\Windows\\outingex.dat")
#define PATH_SEND_SMS_TEXT       ("\\Windows\\sendsms.dat")
#define PATH_NEW_SVR_MSG         ("\\Windows\\svrmsg.dat")


BOOL CGSMLogic::IsOnlyASSIC(CString str)
{
	int nLen = str.GetLength();
	unsigned short UniChar = 0;
	for( int i = 0; i < nLen; i++ )
	{
		UniChar = str.GetAt( i );
		if ( UniChar > 127 || UniChar < 0 )
			return FALSE;
	}
	return TRUE;
}


CGSMLogic::CGSMLogic()
{
	m_bSimState = FALSE;
	m_GsmCardState = sqr_count;  //没有接收到任何信号
	m_bSystemState = TRUE;
	m_nSimSMSUsed = 0;  //SIM卡短消息个数
	m_nSimSMSMax = 0;
	m_nPBookUsed = 0;
	m_nPbookMax = 0;
	m_SMSList = 0;         //SIM卡短消息内容
	m_PBookList = 0;       //SIM卡电话薄内容
	m_nGSMNet = 0;
	memset( m_CallingNum, 0, sizeof(m_CallingNum) );
	memset( m_tnc, 0, sizeof(m_tnc) );
}


CGSMLogic::~CGSMLogic()
{
	if( m_SMSList )
		delete []m_SMSList;
	m_SMSList = 0;
	m_nSimSMSUsed = 0;
}

//////////////////////////////////////////////////////////////////////////

CString  CGSMLogic::GetCallingPhone()
{
	memset(m_CallingNum, 0, sizeof(m_CallingNum) );

	cex_file file;
	file.open( PATH_NEW_CALL_INCOMING, cex_file::m_read );
	int nSize = file.size();
	file.read( m_CallingNum, nSize );
	file.close();

	TCHAR sz[32];
	memset(sz, 0, sizeof(sz) );
	ex_char2uni(m_CallingNum, sz, 32 );
	return (CString)sz;
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::InitFindSrv( HWND hOwner )
{
	if ( hOwner != 0 )
		m_ipc.set_onwer( hOwner );
	m_ipc.init_target( NULL, NULL );
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::SetPhoneNum( CString& strPhone )
{
	TCHAR* pPhone = strPhone.GetBuffer( strPhone.GetLength() );
	char  chPhone[128];
	memset(chPhone, 0, sizeof(chPhone) );

#if defined(UNICODE) || defined(_UNICODE)
	int rst = WideCharToMultiByte( CP_ACP, NULL, pPhone, -1, chPhone, sizeof(chPhone)/sizeof(chPhone[0]), NULL,FALSE );
	if ( rst == 0 )
		rst = WideCharToMultiByte( 936, NULL, pPhone, -1, chPhone, sizeof(chPhone)/sizeof(chPhone[0]), NULL,FALSE );
#else
	memcpy(chPhone, pPhone, nLen );
#endif
	strPhone.ReleaseBuffer();

	int nLen = strlen( chPhone );
	if ( nLen > 32 ) nLen = 31;

	cex_file file;
	cex_file::remove( PATH_NEW_CALL_OUT );
	cex_file::create( PATH_NEW_CALL_OUT );

	if ( file.open( PATH_NEW_CALL_OUT, cex_file::m_write ) )
	{
		file.seek2begin();
		file.write( chPhone, nLen+1 );
		file.flush();
		file.close();
	}
}

//////////////////////////////////////////////////////////////////////////
void    CGSMLogic::SetPhoneExt( CString& strExt )
{
	TCHAR* pPhone = strExt.GetBuffer( strExt.GetLength() );
	char  chPhone[128];
	memset(chPhone, 0, sizeof(chPhone) );
	
#if defined(UNICODE) || defined(_UNICODE)
	int rst = WideCharToMultiByte( CP_ACP, NULL, pPhone, -1, chPhone, sizeof(chPhone)/sizeof(chPhone[0]), NULL,FALSE );
	if ( rst == 0 )
		rst = WideCharToMultiByte( 936, NULL, pPhone, -1, chPhone, sizeof(chPhone)/sizeof(chPhone[0]), NULL,FALSE );
#else
	memcpy(chPhone, pPhone, nLen );
#endif
	strExt.ReleaseBuffer();
	
	int nLen = strlen( chPhone );
	cex_file file;

	cex_file::remove( PATH_NEW_CALL_OUT_EX );
	cex_file::create( PATH_NEW_CALL_OUT_EX );
	
	if ( file.open( PATH_NEW_CALL_OUT_EX, cex_file::m_write ) )
	{
		file.seek2begin();
		file.write( chPhone, nLen+1 );
		file.flush();
		file.close();
	}
}
//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::SetMsgBuf( CString&  strMsg )
{
	TCHAR UniMsg[141];
	memset(UniMsg, 0, sizeof(UniMsg) );
	int nLen = strMsg.GetLength();
	if ( nLen > 140 )
		nLen = 140;

	memcpy(UniMsg, (LPCTSTR)strMsg, nLen * sizeof(TCHAR) );

	char szBuf[282];
	memset(szBuf, 0, sizeof(szBuf));
#if defined(UNICODE) || defined(_UNICODE)
	ex_uni2char(UniMsg, szBuf, sizeof(szBuf) );
#else
	memcpy(szBuf, UniMsg, nLen );
#endif

	nLen = strlen( szBuf );
	if ( nLen > 140 ) nLen = 140;
	szBuf[nLen] = '\0';

	cex_file file;
	cex_file::remove( PATH_SEND_SMS_TEXT );
	cex_file::create( PATH_SEND_SMS_TEXT );
	
	if ( file.open( PATH_SEND_SMS_TEXT, cex_file::m_write ) )
	{
		file.seek2begin();
		file.write( szBuf, sizeof(szBuf) );
		file.flush();
		file.close();
	}


	//m_ipc.past_data(GSM_DCOPY_MSG,szBuf, nLen );
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::SetSMSList( SM_PARAM* p_list, int nCount )
{
	if ( m_SMSList ) delete [] m_SMSList;

	m_nSimSMSUsed = nCount;
	if ( nCount > 0 )
	{
		m_SMSList = new SM_PARAM[nCount];
		if ( m_SMSList == 0 ) return;
		memset(m_SMSList, 0, sizeof(SM_PARAM) * nCount );
		if ( nCount > 0 && p_list != NULL )
			memcpy( m_SMSList, p_list, sizeof(SM_PARAM) * nCount );
	}
}


//////////////////////////////////////////////////////////////////////////
void    CGSMLogic::SetPBookList(PBOOK_ITEM* p_list, int nCount)
{
	if( m_PBookList ) delete [] m_PBookList;
	if ( m_nPbookMax > 0 )
	{
		m_PBookList = new PBOOK_ITEM[m_nPbookMax];
		if ( m_PBookList == 0 ) return;
		memset( m_PBookList, 0, sizeof(PBOOK_ITEM) * m_nPbookMax );
		if ( nCount > 0 && p_list != NULL )
		memcpy( m_PBookList, p_list, sizeof(PBOOK_ITEM) * nCount );
	}

}
//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::SendGSMCMD(unsigned n_cmd, int n_param /* = 0 */, bool b_sync /*= true*/ )
{
	if ( b_sync )
		m_ipc.send_msg( UMSG_GSM_CMD, n_cmd, n_param );
	else
		m_ipc.post_msg( UMSG_GSM_CMD, n_cmd, n_param );
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::GSMSrvShutDown( bool b_sync)
{
	if ( b_sync )
		m_ipc.send_msg(WM_DESTROY, 0, 0 );
	else
		m_ipc.post_msg(WM_DESTROY, 0, 0 );
}

void    CGSMLogic::GSMSetVol(int nVel)
{
	int nVol = nVel;
	if(73 == nVel)
	{
		nVol = 74;
	}
	else if(81 == nVel)
	{
		nVol = 77;
	}
	else if(89 == nVel)
	{
		nVol = 81;
	}
	SendGSMCMD( GSM_CMD_SET_VOL, nVol, false );
}

//////////////////////////////////////////////////////////////////////////


BOOL    CGSMLogic::GSMIsReadSIMPBook()
{
	return TRUE;
	//如果没有SIM卡，则返回，不用在读SIM卡
	if( m_nPbookMax > 0 && m_PBookList != NULL ) 
		return TRUE;
	return FALSE; 
}

//////////////////////////////////////////////////////////////////////////

bool    CGSMLogic::GSMIsReadSIM()
{
	if ( m_nSimSMSMax > 0 ) return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::onGsmAckCmd(UINT GsmCmd, int nParam)
{
	switch ( GsmCmd )
	{
	//发送短消息后的ACK
	case GSM_ACK_MSG_RESULT: 
		if ( GetForegroundWindow() != hMainWnd )
			::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GSM_ACK_MSG_RESULT, nParam );
		break;
	//拨打电话后的ACK
	case GSM_ACK_DIAL_RESULT: 
		if ( GetForegroundWindow() != hMainWnd )
			::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GSM_ACK_DIAL_RESULT, nParam );
		break;
	case GSM_ACK_DIALING_LINE:
		if ( GetForegroundWindow() != hMainWnd )
			::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GSM_ACK_DIALING_LINE, nParam );
		break;
	case GSM_ACK_LINE_DISCONNECT:
	case GSM_ACK_LINE_NOCARRIER:
		{
			if ( GetForegroundWindow() != hMainWnd )
				::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GsmCmd, 0 );
		}
		break;
	//读SIM卡上电话薄的进度HIWORD():总共电话个数 LOWORD():正在读的条数
	case GSM_SIM_PBOOK_READ_PROCESS:
		if ( GetForegroundWindow() != hMainWnd )
			::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GSM_SIM_PBOOK_READ_PROCESS, nParam );
		break;
	//		GSM_SIM_SMS_READ_PROCESS,      //读SIM卡上的SMS进度，HIWORD:总数，LOWORD:当前的个数

	//GSM信号强度
	case GSM_ACK_SIGNAL:
		{
			UINT n_val;
			n_val = nParam;
			//m_GsmCardState = (gsm_signal_e)nParam;
			//TRACE1("%d",nParam);

			if ( n_val >= 0 && n_val <= 7 )        m_GsmCardState = sqr_very_low;
			else if ( n_val > 7 && n_val <= 15 )   m_GsmCardState = sqr_weak;
			else if ( n_val > 15 && n_val <=20 )   m_GsmCardState = sqr_normal;
			else if ( n_val > 20 && n_val < 25 )   m_GsmCardState = sqr_good;
			else if ( n_val >= 25 && n_val <= 31 ) m_GsmCardState = sqr_strength;
			else if ( n_val == 99 ) 			   m_GsmCardState = sqr_count;
			
			GSMReportSignal(n_val);		//store GSM signal
		}
		break;
	case GSM_ACK_QUERY_NET:
		{
			m_nGSMNet = nParam;
		}
		break;
	case GSM_ACK_SIM_STATE:
		{
			if ( nParam == 2 ) m_bSystemState = 0;
			else if ( nParam == 0 )	m_bSimState = FALSE;
			else if ( nParam == 1 )	m_bSimState = 1;
		}
		break;
	case GSM_ACK_SMS_STATE:
		{
			m_nSimSMSMax = HIWORD(nParam);
			m_nSimSMSUsed = LOWORD(nParam);

#if 0	
			CString str;
			str.Format(_T("SMS:%d-%d\n"), m_nSimSMSUsed, m_nSimSMSMax);
			AfxMessageBox( str );
#endif

			TRACE2("SMS_State: %d--%d\n",m_nSimSMSMax, m_nSimSMSUsed );
		}
		break;


	case GSM_ACK_PBOOK_STATE:
		{
			m_nPbookMax = HIWORD(nParam);
			m_nPBookUsed = LOWORD(nParam);
#if 0
			CString str;
			str.Format(_T("PBOOK:%d-%d\n"), m_nPBookUsed, m_nPbookMax);
			AfxMessageBox( str );
#endif
			TRACE2("SMS_State: %d--%d\n",m_nPbookMax, m_nPBookUsed );
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL     CGSMLogic::IsSMSHasMaxOnSIM()
{
	if (m_nSimSMSUsed == m_nSimSMSMax ) 
	{
		if ( m_nSimSMSMax > 0 )
			return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//保存到草稿箱
void     CGSMLogic::UserSaveMsg(CString& strMsg)
{
	//保存到发件邮箱里面
	st_sms  stMsg;
	memset( &stMsg, 0, sizeof(stMsg) );
	
	//如果小于70个字符，则所有都保存
	if ( strMsg.GetLength() <= 70 )
	{
		TCHAR* pChar = strMsg.GetBuffer( strMsg.GetLength() );
		ex_uni2char( pChar, stMsg.m_context, sizeof(stMsg.m_context) );
		strMsg.ReleaseBuffer();
	}else{
		BOOL  bIsAssic = IsOnlyASSIC( strMsg );
		int   nTotalLen = 70;

		if ( bIsAssic )
			nTotalLen = 140;

			if ( strMsg.GetLength() >= nTotalLen )
				strMsg = strMsg.Left( nTotalLen );
       
		//保存当前的消息
		TCHAR* pChar = strMsg.GetBuffer( strMsg.GetLength() );
		ex_uni2char( pChar, stMsg.m_context, sizeof(stMsg.m_context) );
		strMsg.ReleaseBuffer();
	}

	char szSql[512];
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(szSql, "INSERT INTO sms(sms_type,sms_content) VALUES(%d,'%s');", 
		3,stMsg.m_context);

	db.execDML(szSql);
	db.close();
}

//////////////////////////////////////////////////////////////////////////
//保存到发件邮箱里面
void    CGSMLogic::UserSendMsg(CString& strPhone, CString& strMsg)
{
	st_sms  stMsg;
	memset( &stMsg, 0, sizeof(stMsg) );

	if ( strPhone.GetLength() >= 16 )
		strPhone = strPhone.Left( 15 );
	TCHAR* pChar = strPhone.GetBuffer( strPhone.GetLength() );
	ex_uni2char( pChar, stMsg.m_number, sizeof(stMsg.m_number) );
	strPhone.ReleaseBuffer();


	//如果小于70个字符，则所有都保存
	if ( strMsg.GetLength() <= 70 )
	{
		TCHAR* pChar = strMsg.GetBuffer( strMsg.GetLength() );
		ex_uni2char( pChar, stMsg.m_context, sizeof(stMsg.m_context) );
		strMsg.ReleaseBuffer();
	}
	else
	{
		BOOL  bIsAssic = IsOnlyASSIC( strMsg );
		int   nTotalLen = 70;
		
		if ( bIsAssic )
			nTotalLen = 140;
		
		if ( strMsg.GetLength() >= nTotalLen )
			strMsg = strMsg.Left( nTotalLen );
		
		//保存当前的消息
		TCHAR* pChar = strMsg.GetBuffer( strMsg.GetLength() );
		ex_uni2char( pChar, stMsg.m_context, sizeof(stMsg.m_context) );
		strMsg.ReleaseBuffer();
	}

	char szSql[512];
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(szSql, "INSERT INTO sms(sms_type,phone_number,sms_content) VALUES(%d,%s,'%s');", 
		2,stMsg.m_number,stMsg.m_context);

	db.execDML(szSql);
	db.close();
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::GSMDelSIMSMS(int idx) 
{
	if ( idx <= m_nSimSMSUsed-1 )
		SendGSMCMD(GSM_SIM_SMS_DEL,m_SMSList[idx].index, TRUE); 

	if ( idx < m_nSimSMSUsed - 1 )
	{
		for( int i = idx; i < m_nSimSMSUsed-1; i++)
			memcpy(&m_SMSList[i],&m_SMSList[i+1], sizeof(SM_PARAM) );
	}
	m_nSimSMSUsed--;
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::GSMDelSIMPBook(int idx)
{
	int nDel = -1;
	int i = 0;
	for( i = 0; i < m_nPBookUsed; i++ )
	{
		if ( m_PBookList[i].nIdx == idx )
		{
			nDel = i;
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if ( nDel >= 0 )
	{
		if ( nDel != m_nPBookUsed - 1 )
		{	for( int j = nDel; j < m_nPBookUsed -1; j++ )
				memcpy( &m_PBookList[i], &m_PBookList[j+1], sizeof(PBOOK_ITEM) );
		}
		SendGSMCMD(GSM_SIM_PBOOK_DEL,idx, TRUE );
		m_nPBookUsed--;
	}
}

//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::GSMDelSIMPBookAll()
{
	for( int i = 0; i < m_nPBookUsed; i++ )
		SendGSMCMD( GSM_SIM_PBOOK_DEL,m_PBookList[i].nIdx, TRUE );

	//HarryLiu@20090324
	//if ( m_PBookList ) delete [] m_PBookList;
	//m_PBookList = NULL;
	m_nPBookUsed = 0;
}

//////////////////////////////////////////////////////////////////////////

int     CGSMLogic::GSMFindUnUsedIdx()
{
	BOOL bUsed = FALSE;
	for( int i = 1; i <= m_nPbookMax; i++ )
	{
		bUsed = FALSE;
		for( int j = 0; j < m_nPBookUsed; j++ )
		{
			if ( i == m_PBookList[j].nIdx )
			{
				bUsed = TRUE;
				break;
			}
		}
		if ( !bUsed )
			return i;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////

BOOL    CGSMLogic::GSMAddSIMPBook( const char* p_name, const char* p_phone ) 
{
	if ( p_phone == NULL || p_name == NULL )
		return FALSE;
	if ( m_nPBookUsed == m_nPbookMax && m_nPbookMax > 0 ) return FALSE;
	int nIdx = GSMFindUnUsedIdx();
	if ( nIdx == -1 ) return FALSE;

	PBOOK_ITEM Item;
	memset( &Item, 0, sizeof(PBOOK_ITEM) );
	strcpy( Item.name, p_name );
	strcpy( Item.phone, p_phone );
	Item.nIdx = nIdx;
	Item.state = 1;
	memcpy( &m_PBookList[m_nPBookUsed++], &Item, sizeof(PBOOK_ITEM) );

	//发送给Svr进程
	m_ipc.past_data(GSM_DCOPY_PBOOK, (const char*)&Item, sizeof(PBOOK_ITEM) );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CGSMLogic::SchPBook(CString& strPhone, CString& strName )
{
	char sz_pnone[64];
	memset(sz_pnone, 0, sizeof(sz_pnone) );
	ex_uni2char((LPCTSTR)strPhone, sz_pnone, sizeof(sz_pnone) );
	PBOOK_ITEM* p_rst = m_pbook.sch_item( sz_pnone );

	if ( p_rst == NULL ) 
	{	
		//查找SIM卡上的电话薄
		if ( m_PBookList == NULL )
			return FALSE;
		for( int i = 0; i < m_nPBookUsed; i++ )
		{
			if ( strcmp( m_PBookList[i].phone, sz_pnone ) == 0 )
			{
				TCHAR szName[64];
				memset(szName, 0, sizeof(szName) );
				ex_char2uni(m_PBookList[i].name, szName, Ex_MIDOF(szName) );
				strName.Format(_T("%s"), szName);
				return true;
			}
		}
	}
	else
	{
		TCHAR szName[64];
		memset(szName, 0, sizeof(szName) );
		ex_char2uni(p_rst->name, szName, Ex_MIDOF(szName) );
		strName.Format(_T("%s"), szName);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

BOOL    CGSMLogic::GSMUpdateSIMPBook( PBOOK_ITEM* pItem )
{
	PBOOK_ITEM* p_Cur = NULL;
	if ( pItem == NULL ) 
		return FALSE;
	for( int i = 0; i < m_nPBookUsed; i++ )
	{
		p_Cur = &m_PBookList[i];
		if ( p_Cur->nIdx == pItem->nIdx )
			break;
		p_Cur = NULL;
	}

	if ( p_Cur == NULL )
	{
		return FALSE;
	}

	memcpy(p_Cur, pItem, sizeof(PBOOK_ITEM) );
	//发送给Svr进程
	m_ipc.past_data(GSM_DCOPY_PBOOK, (const char*)pItem, sizeof(PBOOK_ITEM) );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
PBOOK_ITEM* CGSMLogic::GSMGetSIMPBookRd(int nRdx)
{
	for( int i = 0; i < m_nPBookUsed; i++ )
	{
		if ( m_PBookList[i].nIdx == nRdx )
			return &m_PBookList[i];
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////

void    CGSMLogic::GSMDelSIMSMSAll()
{
	for( int i = 0; i < m_nSimSMSUsed; i++ )
		SendGSMCMD(GSM_SIM_SMS_DEL,m_SMSList[i].index, TRUE);
	if ( m_SMSList ) delete []m_SMSList;
	m_SMSList = 0;
	m_nSimSMSUsed = 0;
}

//////////////////////////////////////////////////////////////////////////
void    CGSMLogic::GSMSetNet(int nSel, int nType)
{
	int nVal = (nSel << 16) & 0xFFFF0000;
	nVal += ( nType & 0xFFFF );
	SendGSMCMD( GSM_NET_SET, nVal ,FALSE );
	GSMQueryGSMNet();
	m_nGSMNet = 0;
}


void   CGSMLogic::Send2TNC(char *TNCData)
{
	wchar_t sz_buf[512];
	memset(sz_buf, 0, sizeof(sz_buf) );
	ex_char2uni(TNCData, sz_buf, sizeof(sz_buf)/sizeof(sz_buf[0]));
	int len = wcslen(sz_buf) * 2;
	past_cld_TNC( (const char*)sz_buf, len ); 
}

//////////////////////////////////////////////////////////////////////////

void   CGSMLogic::past_cld_TNC (const char* p_data, int n_len )
{
	HWND h_wnd = find_kld_hwnd();
	if ( h_wnd ==0 )
	{
		TCHAR bCmdline[100];
		LPCWSTR srCmdLine = bCmdline;
		
		BOOL bProcessState = CreateProcess( CSysConfig::Instance().GetNaviPath(), NULL, NULL, NULL, 
            FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);	
		if(bProcessState)
			CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
		
		Sleep(8000);
		h_wnd = find_kld_hwnd();
	}
	
	COPYDATASTRUCT  cds;
	cds.cbData = n_len;
	cds.lpData = (PVOID)p_data;
	cds.dwData = 1;
	int n_rst = ::SendMessage( h_wnd, WM_COPYDATA, (WPARAM)h_wnd, (LPARAM)&cds);
}


//////////////////////////////////////////////////////////////////////////

int    CGSMLogic::onGsmDataCopy(WPARAM wParam, LPARAM lParam)
{
	int nRst = 0;
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)(lParam);
	if ( pcds->dwData == GSM_DCOPY_ACK_MSG )
	{
		if ( pcds->cbData == sizeof(SM_PARAM) )
		{
			SM_PARAM  *p_sm = (SM_PARAM*)pcds->lpData;
			//#T,ASN:
			if ( memcmp(p_sm->TP_UD,"#RESET", 5) == 0 )
			{
				
				nRst = 999;
				return nRst;
			}
			else if ( memcmp(p_sm->TP_UD,"#SMSSET:", 7) == 0 )
			{
				CString strSMSbody = _T("SET OK");
				CString	strSMSTA = _T("");
				TCHAR  szBuf[160];
				memset(szBuf, 0, sizeof(szBuf) );
				ex_char2uni((const char*)p_sm->TPA, szBuf,160 );

				strSMSTA = szBuf;

				g_GsmLogic.SetPhoneNum( strSMSTA );
				g_GsmLogic.SetMsgBuf(strSMSbody);//发短信字符
				g_GsmLogic.GSMSendSMS();
				Sleep(500);

				HWND hWnd = ::FindWindow(NULL, _T("WND_GNSSTERMINAL"));
				if(hWnd != NULL)
				{
					::SendMessage( hWnd, WM_COPYDATA, GSM_DCOPY_ACK_MSG, lParam);
				}

				return 0;
			}
			else if ( memcmp(p_sm->TP_UD,"#SMSQUERY", 9) == 0 )
			{
				CString strSMSbody = _T("");
				CString	strSMSTA = _T("");
				TCHAR  szBuf[160];
				memset(szBuf, 0, sizeof(szBuf) );
				ex_char2uni((const char*)p_sm->TPA, szBuf,160 );
				strSMSTA = szBuf;
				CString strtemp = _T("");
				CIniFile inifile;

				inifile.Load(PATH_CONFIG_INI);

				strtemp = _T("");
				inifile.GetString(_T("TerminalParam"), _T("MAIN_SVR_IP"), strtemp);
				strSMSbody += _T("IP:")+strtemp+_T(";");

				strtemp = _T("");
				inifile.GetString(_T("TerminalParam"), _T("SVR_TCP_PORT"), strtemp);
				strSMSbody += _T("PORT:")+strtemp+_T(";");

				strtemp = _T("");
				inifile.GetString(_T("Set"), _T("PhoneNum"), strtemp);
				strSMSbody += _T("ID:")+strtemp+_T(";");

				inifile.Load(_T("\\Flashdrv Storage\\Version.ini"));
				inifile.GetString(_T("VERSION"),_T("SWVER"),strtemp);
				strSMSbody += strtemp+_T(";");

				strtemp = _T("");
				strtemp.Format(_T("GPS:%d;"),m_pPosInfo->rmc_data.pos_state.dwSateNum);

				strSMSbody += strtemp;

				strtemp = _T("");
				strtemp.Format(_T("GSM:%d;"),m_iState);

				strSMSbody += strtemp;

				strSMSbody += _T("APN:cmnet;");
			
				g_GsmLogic.SetPhoneNum( strSMSTA );
				g_GsmLogic.SetMsgBuf(strSMSbody);//发短信字符
				g_GsmLogic.GSMSendSMS();

				return 0;
			}



 			st_sms st;
 			st.m_type = 2;
 			memcpy(st.m_time, p_sm->TP_SCTS, sizeof(p_sm->TP_SCTS) );
 			memcpy(st.m_number, p_sm->TPA, sizeof(p_sm->TPA) );
 			memcpy(st.m_context, p_sm->TP_UD, sizeof(p_sm->TP_UD));
 			if ( m_inbox.is_open() == 0 )
 			{
 				m_inbox.add_2file( SMS_PATH_INBOX, &st);
 			}else
 			{
 				m_inbox.add_recourd( &st );
 			}

			char szSql[512];
			CppSQLite3DB db;
			db.open(PATH_SQLITE_DB_808);	//打开数据库
			sprintf(szSql, "INSERT INTO sms(sms_type,phone_number,sms_content) VALUES(%d,%s,'%s');", 
				1,p_sm->TPA,p_sm->TP_UD);

			db.execDML(szSql);
			db.close();

			TCHAR tchar[NUM_1K];
			memset(tchar, 0, sizeof(tchar));

			MultiByteToWideChar( CP_ACP, 0, p_sm->TP_UD,strlen(p_sm->TP_UD)+1, tchar, sizeof(tchar)/sizeof(tchar[0]) );

			CString strtemp = _T("");
			strtemp.Format(_T("%s"),tchar);


			TTSSpeaking(strtemp);

			//GetUnRdMsg();
			m_iUnRdMsg++;
			//Invalidate();
		}
		::PlaySound(_T("\\Flashdrv Storage\\menu\\style\\sms_msg.wav"),NULL, SND_FILENAME | SND_ASYNC );
	} //新电话的电话号码
	else if ( pcds->dwData == GSM_DCOPY_ACK_PHONE )
	{
		memset( m_CallingNum, 0 , sizeof(m_CallingNum) );
		int n_size = pcds->cbData;
		if ( n_size > sizeof(m_CallingNum) )
			n_size = sizeof(m_CallingNum) - 1;
		memcpy( m_CallingNum, pcds->lpData, n_size );
	}
	else if ( pcds->dwData == GSM_DCOPY_SIM_SMS )
	{
		SetSMSList( (SM_PARAM*)pcds->lpData, pcds->cbData/sizeof(SM_PARAM) );
		::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GSM_SIM_SMS_READ, 0 );
	}else if ( pcds->dwData == GSM_DCOPY_SIM_PHONEBOOK )
	{
		SetPBookList( (PBOOK_ITEM*)pcds->lpData, pcds->cbData/sizeof(PBOOK_ITEM) );
		::PostMessage( GetForegroundWindow(), UMSG_GSM_CMD, GSM_SIM_PBOOK_READ, 0 );
	}
	return nRst;
}

PCSTR CGSMLogic::ScanField(PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator)
{
	PCSTR pcFieldBegin = pcBuffer;

	while( nFieldNO-- > 0 )
	{
		while ( pcFieldBegin && *pcFieldBegin++ != cSeparator && *pcFieldBegin != NULL )
			;
	}
	//搜索的节为空
	if(NULL == *pcFieldBegin)
	{
		wLen = 0;
		return NULL;
	}

	//节的最后一个字符指针的后一位
	PCSTR pcFieldEnd = pcFieldBegin;
	while( pcFieldEnd && *pcFieldEnd != cSeparator && *pcFieldEnd != NULL )
	{
		pcFieldEnd++;
	}

	//计算节的长度
	wLen = pcFieldEnd - pcFieldBegin;

	return pcFieldBegin;
}

//"#T,NAV:114.025187,22.540043"
void   CGSMLogic::SaveUserLongLat(const char* pStr)
{
	char szBuf[15];
	memset(szBuf,0,sizeof(szBuf) );
	memcpy(szBuf, pStr, 10 );
	double ft = atof(szBuf);
	m_LongSec = ( ft * 10000 ) * 60;

	memset(szBuf,0,sizeof(szBuf) );
	memcpy(szBuf, &pStr[11], 9 );
	ft = atof(szBuf);
	m_LatSec = ( ft * 10000 ) * 60;
}
//#,136.36366,49.4856,人民大会堂
//"#T,NAV:114.025187,22.540043"
void CGSMLogic::OnKeyNaviForHuaWei()
{
	#define MSG_WND_NAME L"Guider_MsgWnd"	// 消息接收窗口名称
	HWND hwnd = ::FindWindow(NULL, MSG_WND_NAME);
	if (NULL == hwnd)
	{
		TCHAR bCmdline[100];
		LPCWSTR srCmdLine = bCmdline;

		BOOL bProcessState = CreateProcess( CSysConfig::Instance().GetNaviPath(), NULL, NULL, NULL, 
            FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo);	
		if(bProcessState)
			CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);
		
		//StartProgress("Storage Card\\Guider4u\\320240\\Guider.exe")
		for(int i = 0; i < 1 * 60; i++ )
		{
			Sleep(1000);
			hwnd = ::FindWindow(NULL, MSG_WND_NAME);
			if ( hwnd ) 
				break;
		}
	}
	
	if (hwnd)
	{
		wchar_t buf[256] = {0};
		wsprintf(buf, L"SET_DESTINATION,%s",  CString(m_tnc) );
		COPYDATASTRUCT cpd;
		cpd.dwData = 0;
		cpd.cbData = wcslen(buf)*sizeof(wchar_t) + 2;
		cpd.lpData = (PVOID)buf;
		
		::SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cpd);
	}
}


//////////////////////////////////////////////////////////////////////////
HWND   CGSMLogic::find_kld_hwnd()
{
	HWND h_wnd = 0;
	for( int i = 0; i < 10; i++ )
	{
		h_wnd = ::FindWindow(CLD_CLS_NAME, CLD_WND_NAME );
		if ( h_wnd != 0 )
			break;
		Sleep(10);
	}
	
	return h_wnd;
}

//////////////////////////////////////////////////////////////////////////
//传递的参数必须为毫秒
void  CGSMLogic::send_2_navi(int n_long, int n_lat)
{
	
#if defined(UNICODE)||defined(_UNICODE)
	//#if 0
	wchar_t sz_buf[128];// = "$CLDLOC,2,2, 2232.4026, 11401.5112, 31.19,,#E";
	wchar_t sz_lat[32];
	wchar_t sz_long[32];
	memset( sz_long, 0, sizeof(sz_lat) );
	memset( sz_lat, 0, sizeof(sz_lat) );
	
	int n_deg = ( n_lat / 10000 ) / 60;
	int n_min = ( n_lat / 10000 ) % 60;
	int n_min_float = n_lat % 10000;
	wsprintfW( sz_lat, L"%02d%02d.%04d",n_deg,n_min,n_min_float);
	
	n_deg = ( n_long / 10000 ) / 60;
	n_min = ( n_long / 10000 ) % 60;
	n_min_float = n_long % 10000;
	wsprintfW( sz_long, L"%02d%02d.%04d",n_deg,n_min,n_min_float);
	
	//8343 9261 - 顾安友
	wsprintfW( sz_buf, L"$CLDLOC,2,2,%s,%s,10.0,test,#E\0",sz_lat,sz_long);
	
	past_cld_navigation( (const char*)sz_buf, wcslen(sz_buf) * 2 );
#else
	char sz_buf[128];// = "$CLDLOC,2,2,2232.4026,11401.5112, 31.19,,#E";
	char sz_lat[32];
	char sz_long[32];
	memset( sz_long, 0, sizeof(sz_lat) );
	memset( sz_lat, 0, sizeof(sz_lat) );
	
	int n_deg = ( n_lat / 10000 ) / 60;
	int n_min = ( n_lat / 10000 ) % 60;
	int n_min_float = n_lat % 10000;
	sprintf( sz_lat, "%02d%02d.%04d",n_deg,n_min,n_min_float);
	
	n_deg = ( n_long / 10000 ) / 60;
	n_min = ( n_long / 10000 ) % 60;
	n_min_float = n_long % 10000;
	sprintf( sz_long, "%02d%02d.%04d",n_deg,n_min,n_min_float);
	
	//8343 9261 - 顾安友
	sprintf( sz_buf, "$CLDLOC,2,2,%s,%s,10.0,,#E\0",sz_lat,sz_long);
	
	past_cld_navigation( (const char*)sz_buf, strlen(sz_buf) );
	
#endif
}


void   CGSMLogic::past_cld_navigation(const char* p_data, int n_len )
{
	HWND h_wnd = find_kld_hwnd();
	
	if ( h_wnd ==0 )
	{
		return;
	}

	//char* p="TNC,002,0,15C580DD34165AC131C14C,0,B0,,,,,C0,,,H,A)福州市政府\0";
	
	COPYDATASTRUCT  cds;

#if 0
	cds.cbData = strlen(p);
	cds.lpData = (PVOID)p;
#endif

#if 1
	cds.cbData = n_len;
	cds.lpData = (PVOID)p_data;
#endif
		int n_rst = 0;
		for( int i = 0; i < 0xFFFFFFFF; i++ )
		{
			cds.dwData = i;
			n_rst = ::SendMessage( h_wnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
			if ( n_rst == 1 )
			{
				TRACE1("OK:%d\0", i );
				break;
			}else
			{
				TRACE1("Failed:%d\n",i);
			}
		}
}

void CGSMLogic::GetUnRdMsg()
{
	m_iUnRdMsg = 0;
	if ( !m_inbox.is_open() )
	{
		m_inbox.load_data( SMS_PATH_INBOX );
	}

	logic_data* pInbox = &(m_inbox);
	int nMaxRd = pInbox->max_number();
	st_sms* pSMS;
	for(int i=0; i<nMaxRd; i++)
	{
		pSMS = pInbox->get_recourd(i);
		if(pSMS->m_type == 2)
		{
			m_iUnRdMsg++;
		}
	}
}

void CGSMLogic::GSMReportSignal(int nVal)
{
	static int nSignal = 0;
	if ( nVal > 0 )
		nSignal = nVal;

	HWND hwGSM_SignalUI;
	hwGSM_SignalUI = FindWindow(NULL, _T("GSM_SignalUI"));
	if(hwGSM_SignalUI)
		::PostMessage(hwGSM_SignalUI, UMSG_GSM_CMD, GSM_ACK_SIGNAL, nSignal);
}

void CGSMLogic::SetLoginState(int iState)
{
	m_iState = iState-8;
}