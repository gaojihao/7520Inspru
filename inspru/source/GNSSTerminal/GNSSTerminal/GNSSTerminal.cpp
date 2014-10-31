// GNSSTerminal.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GNSSTerminal.h"
#include "GNSSTerminalDlg.h"
#include "./SQLite3/SQLite3Class.h"
#include "Lib/tts_api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE			g_hTTS;							//tts���
#define	PATH_TTS_DATA		"\\Flashdrv Storage\\Menu\\stubsvr.dat"



void TTSSpeaking(CString csText)
{
	if(g_hTTS == NULL)
		return;

	const unsigned short* pszText = (unsigned short*)csText.GetBuffer(csText.GetLength());
	if ( tts_is_speaking(g_hTTS) )
	{
		tts_cancle(g_hTTS);
	}
	tts_speaking(g_hTTS, pszText, csText.GetLength()+1 );
}


// CGNSSTerminalApp

BEGIN_MESSAGE_MAP(CGNSSTerminalApp, CWinApp)
END_MESSAGE_MAP()


// CGNSSTerminalApp construction
CGNSSTerminalApp::CGNSSTerminalApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGNSSTerminalApp object
CGNSSTerminalApp theApp;

// CGNSSTerminalApp initialization

BOOL CGNSSTerminalApp::InitInstance()
{
	//������ʵ������
	HANDLE   hMutex = CreateMutex(NULL, TRUE, _T("GNSS_PROCESS_MUTEX"));  
	if(hMutex != NULL)   
	{
		//�����Ѵ���
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseMutex(hMutex);
			return  FALSE;   
		}
	}

	g_hTTS = tts_open();
	tts_init(g_hTTS, PATH_TTS_DATA);
	tts_man_role(g_hTTS, false);


	//��ʼ�����ݿ�
	InitSQLite();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	CGNSSTerminalDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	tts_close(g_hTTS);
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CGNSSTerminalApp::InitSQLite()
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//�����ݿ�
	//�ı���Ϣ�·������ϴ�
	if( !db.tableExists("text_info") )
	{
		//UID					��Ϣ���
		//text_type				���ͣ�0:�·�  1:�ϴ�  2:Ŀ�ĵص���(text_contentΪ��γ��)
		//flag					��־����808Э���27
		//text_content			��Ϣ����(1k�ֽ�)
		//read_status			0:δ��  1:�Ѷ�
		//text_datatime			2008-03-19 12:10:15
		db.execDML("CREATE TABLE text_info(							\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   text_type				INTEGER,				\
				   flag						INTEGER,				\
				   text_content				TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   text_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	//�����ݿ��������100�����ݱ������ϵ�ɾ����
	db.execDML("DELETE FROM text_info WHERE UID NOT IN (SELECT UID FROM text_info ORDER BY UID DESC LIMIT 100);");

	//�¼���Ϣ
	if( !db.tableExists("event_info") )
	{
		//event_ID					�¼�ID
		//event_content				�¼�����
		db.execDML("CREATE TABLE event_info(						\
				   event_ID					INTEGER PRIMARY KEY,	\
				   event_content			TEXT);");
	}

	//������Ϣ
	if( !db.tableExists("question") )
	{
		//UID					������
		//flag					��־����808Э���32
		//question_content		��������
		//read_status			0:δ��  1:�Ѷ�
		//question_datatime		����ʱ��
		db.execDML("CREATE TABLE question(							\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,				\
				   question_content			TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   question_datatime		NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	//�����ݿ��������100�����ݱ������ϵ�ɾ����
	db.execDML("DELETE FROM question WHERE UID NOT IN (SELECT UID FROM question ORDER BY UID DESC LIMIT 100);");

	//��ѡ���б�
	if( !db.tableExists("answer") )
	{
		//UID					����
		//serial_num			�����·���Ϣ����ˮ�ţ�����Ӧ��
		//answer_ID				��ID
		//answer_content		������
		db.execDML("CREATE TABLE answer(				\
				   UID						INTEGER,	\
				   serial_num				INTEGER,	\
				   answer_ID				INTEGER,	\
				   answer_content			TEXT,		\
				   PRIMARY KEY (UID, answer_ID));");
	}
	db.execDML("DELETE FROM answer WHERE UID NOT IN (SELECT UID FROM answer ORDER BY UID DESC LIMIT 100);");

	//��Ϣ�㲥�˵�
	if( !db.tableExists("info_menu") )
	{
		//UID					�˵����
		//type					��Ϣ����
		//info_name				��Ϣ����
		//read_status			0:δ��  1:�Ѷ�
		//info_datatime         ��Ϣ�·�ʱ��
		db.execDML("CREATE TABLE info_menu(							\
				   UID						INTEGER,				\
				   type						INTEGER,				\
				   info_name				TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   info_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM info_menu WHERE type IN (SELECT type FROM info_menu LIMIT (SELECT count(*) FROM info_menu)-100);");

	//��Ϣ����
	if( !db.tableExists("info_service") )
	{
		//type					��Ϣ����
		//info_content			��Ϣ����
		//read_status			0:δ��  1:�Ѷ�
		db.execDML("CREATE TABLE info_service(						\
				   type						INTEGER PRIMARY KEY,	\
				   info_content				TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   text_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM info_service WHERE type IN (SELECT type FROM info_service LIMIT (SELECT count(*) FROM info_service)-100);");

	//�绰�ز�
	if( !db.tableExists("call_back") )
	{
		//UID					����
		//flag					0:��ͨͨ��; 1:����
		//phone_num				�绰���룬�20�ֽ�
		//call_datatime			ʱ��
		db.execDML("CREATE TABLE call_back(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,	\
				   phone_number				TEXT,		\
				   call_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM call_back WHERE UID NOT IN (SELECT UID FROM call_back ORDER BY UID DESC LIMIT 100);");

	//�����˵�
	if( !db.tableExists("e_waybill") )
	{
		//UID					�����˵����
		//content				�����˵�����
		//waybill_datatime		�����˵��ϱ�ʱ��2008-03-19 12:10:15
		db.execDML("CREATE TABLE e_waybill(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   content					TEXT,		\
				   waybill_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM e_waybill WHERE UID NOT IN (SELECT UID FROM e_waybill ORDER BY UID DESC LIMIT 100);");

	//��ʻԱ�����Ϣ
	if( !db.tableExists("driver_info") )
	{
		//UID					��ʻԱ�����Ϣ���
		//driver_name			��ʻԱ����
		//driver_ID				��ʻԱ���֤����
		//certificate_num		��ҵ�ʸ�֤����
		//agency				��֤��������
		db.execDML("CREATE TABLE driver_info(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   driver_name				TEXT,			\
				   driver_ID				TEXT,			\
				   certificate_num			TEXT,			\
				   agency					TEXT);");
	}
	
	//�绰��
	if( !db.tableExists("phone_book") )
	{
		//UID					����
		//flag					��־��1������   2������   3������/����
		//name					��ϵ������
		//phone_number			��ϵ�˺���
		db.execDML("CREATE TABLE phone_book(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,		\
				   name						TEXT,			\
				   phone_number				TEXT);");
	}
	db.execDML("DELETE FROM phone_book WHERE UID NOT IN (SELECT UID FROM phone_book ORDER BY UID DESC LIMIT 100);");

	//Բ������
	if( !db.tableExists("region_circular") )
	{
		//region_ID				����ID
		//region_property		��������
		//centerpoint_latitude	���ĵ�γ��
		//centerpoint_longitude	���ĵ㾭��
		//radius				�뾶
		//datetime_begin		��ʼʱ��
		//datetime_end			����ʱ��
		//max_speed				����ٶ�
		//overspeed_time		���ٳ���ʱ��
		db.execDML("CREATE TABLE region_circular(				\
				   region_ID				INTEGER PRIMARY KEY,	\
				   region_property			INTEGER,			\
				   centerpoint_latitude		INTEGER,			\
				   centerpoint_longitude	INTEGER,			\
				   radius					INTEGER,			\
				   datetime_begin			DATETIME,			\
				   datetime_end				DATETIME,			\
				   max_speed				INTEGER,			\
				   overspeed_time			INTEGER);");
	}
	db.execDML("DELETE FROM region_circular WHERE region_ID IN (SELECT region_ID FROM region_circular LIMIT (SELECT count(*) FROM region_circular)-200);");

	//��������
	if( !db.tableExists("region_rectangle") )
	{
		//region_ID				����ID
		//region_property		��������
		//topleft_latitude		���ϵ�γ��
		//topleft_longitude		���ϵ㾭��
		//bottomright_latitude	���µ�γ��
		//bottomright_longitude	���µ㾭��
		//datetime_begin		��ʼʱ��
		//datetime_end			����ʱ��
		//max_speed				����ٶ�
		//overspeed_time		���ٳ���ʱ��
		db.execDML("CREATE TABLE region_rectangle(				\
				   region_ID				INTEGER PRIMARY KEY,\
				   region_property			INTEGER,			\
				   topleft_latitude			INTEGER,			\
				   topleft_longitude		INTEGER,			\
				   bottomright_latitude		INTEGER,			\
				   bottomright_longitude	INTEGER,			\
				   datetime_begin			DATETIME,			\
				   datetime_end				DATETIME,			\
				   max_speed				INTEGER,			\
				   overspeed_time			INTEGER);");
	}
	db.execDML("DELETE FROM region_rectangle WHERE region_ID IN (SELECT region_ID FROM region_rectangle LIMIT (SELECT count(*) FROM region_rectangle)-200);");

	//���������
	if( !db.tableExists("region_polygon") )
	{
		//region_ID				����ID
		//region_property		��������
		//datetime_begin		��ʼʱ��
		//datetime_end			����ʱ��
		//max_speed				����ٶ�
		//overspeed_time		���ٳ���ʱ��
		db.execDML("CREATE TABLE region_polygon(		\
				   region_ID				INTEGER PRIMARY KEY,	\
				   region_property			INTEGER,	\
				   datetime_begin			DATETIME,	\
				   datetime_end				DATETIME,	\
				   max_speed				INTEGER,	\
				   overspeed_time			INTEGER);");
	}
	db.execDML("DELETE FROM region_polygon WHERE region_ID IN (SELECT region_ID FROM region_polygon LIMIT (SELECT count(*) FROM region_polygon)-200);");

	//���������Ķ���
	if( !db.tableExists("polygon_vertex") )
	{
		//region_ID				����ID
		//vertex_latitude		����γ��
		//vertex_longitude		���㾭��
		db.execDML("CREATE TABLE polygon_vertex(		\
				   region_ID				INTEGER,	\
				   vertex_latitude			INTEGER,	\
				   vertex_longitude			INTEGER);");
	}
	db.execDML("DELETE FROM polygon_vertex WHERE region_ID IN (SELECT region_ID FROM polygon_vertex LIMIT (SELECT count(*) FROM polygon_vertex)-200);");

	//·��
	if( !db.tableExists("line") )
	{
		//line_ID				·��ID
		//line_property			·������
		//datetime_begin		��ʼʱ��
		//datetime_end			����ʱ��
		db.execDML("CREATE TABLE line(					\
				   line_ID					INTEGER PRIMARY KEY, \
				   line_property			INTEGER,	\
				   datetime_begin			DATETIME,	\
				   datetime_end				DATETIME);");
	}
	db.execDML("DELETE FROM line WHERE line_ID IN (SELECT line_ID FROM line LIMIT (SELECT count(*) FROM line)-200);");

	//·�߹յ�
	if( !db.tableExists("line_inflexion") )
	{
		//line_ID				·��ID
		//inflexion_ID			�յ�ID
		//inflexion_latitude	�յ�γ��
		//inflexion_longitude	�յ㾭��
		//section_width			·�ο��
		//section_property		·������
		//drive_threshold_max	·����ʻ������ֵ
		//drive_threshold_min	·����ʻ������ֵ
		//max_speed				����ٶ�
		//overspeed_time		���ٳ���ʱ��
		db.execDML("CREATE TABLE line_inflexion(			\
				   line_ID					INTEGER,		\
				   inflexion_ID				INTEGER,		\
				   inflexion_latitude		INTEGER,		\
				   inflexion_longitude		INTEGER,		\
				   section_width			INTEGER,		\
				   section_property			INTEGER,		\
				   drive_threshold_max		INTEGER,		\
				   drive_threshold_min		INTEGER,		\
				   max_speed				INTEGER,		\
				   overspeed_time			INTEGER);");
	}
	db.execDML("DELETE FROM line_inflexion WHERE line_ID IN (SELECT line_ID FROM line_inflexion LIMIT (SELECT count(*) FROM line_inflexion)-200);");

	//����
	if( !db.tableExists("sms") )
	{
		//UID					��Ϣ���
		//sms_type				���ͣ�1:�ռ���  2:������  3:�ݸ���	4:ɾ��
		//phone_number			�绰����
		//sms_content			��Ϣ����(140�ֽ�)
		//read_status			0:δ��  1:�Ѷ�
		//sms_datatime			2008-03-19 12:10:15
		db.execDML("CREATE TABLE sms(						\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   sms_type					INTEGER,			\
				   phone_number				TEXT, 				\
				   sms_content				TEXT,				\
				   read_status				BOOLEAN DEFAULT 0,	\
				   sms_datatime				NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM sms WHERE UID NOT IN (SELECT UID FROM sms ORDER BY UID DESC LIMIT 100);");

	
	//ͨ����¼
	if( !db.tableExists("phone_log") )
	{
		//UID					����
		//flag					��־��1���Ѳ�   2���ѽ�   3��δ��
		//phone_number			��ϵ�˺���	��ϵ���������ݺ��뵽phone_book���в�ѯ
		//call_duration			ͨ��ʱ������λs(��)
		//log_datatime
		db.execDML("CREATE TABLE phone_log(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,	\
				   phone_number				TEXT,		\
				   call_duration			INTEGER DEFAULT 0,	\
				   log_datatime				NOT NULL DEFAULT (datetime('now','localtime')));");
	}

	//��ý��洢��Ϣ
	if( !db.tableExists("multimedia_Info") )
	{
		//multimedia_ID					��ý��ID
		// multimedia_type				��ý������
		//event_code					�¼������
		//multimedia_path				��ý��·��
		//position_info					λ����Ϣ
		db.execDML("CREATE TABLE multimedia_Info(			\
				   multimedia_ID				INTEGER,	\
				   multimedia_type				INTEGER,	\
				   event_code				    INTEGER,	\
				   multimedia_path				TEXT,		\
				   position_info				TEXT,       \
				   log_datatime				    NOT NULL DEFAULT (datetime('now','localtime')));");
	}

	//�����ϴ���Ƭ��Ŀ����·��
	if( !db.tableExists("photo_Info") )
	{
		//photo_num					�����δ������Ƭ��Ŀ
		db.execDML("CREATE TABLE photo_Info(			\
				   photo_num				INTEGER,	\
				   photo_time				INTEGER,    \
				   log_datatime				NOT NULL DEFAULT (datetime('now','localtime')));");

		db.execDML("INSERT INTO photo_Info(photo_num,photo_time) VALUES(0,0);");
	}


	//�����ݿ���һ����֮ǰ������ɾ����
	db.execDML("DELETE FROM text_info WHERE text_datatime < (SELECT datetime('now','localtime', '-1 month'));");
	//�����ݿ��������100�����ݱ������ϵ�ɾ����
	db.execDML("DELETE FROM text_info WHERE UID NOT IN (SELECT UID FROM text_info ORDER BY UID DESC LIMIT 100);");
	//DELETE FROM event_info WHERE event_ID IN (SELECT event_ID FROM event_info LIMIT (SELECT count(*) FROM event_info)-100);
	
	db.close();	//�ر����ݿ�
}
