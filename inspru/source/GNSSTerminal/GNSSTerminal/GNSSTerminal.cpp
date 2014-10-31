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

HANDLE			g_hTTS;							//tts句柄
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
	//创建单实例程序
	HANDLE   hMutex = CreateMutex(NULL, TRUE, _T("GNSS_PROCESS_MUTEX"));  
	if(hMutex != NULL)   
	{
		//窗口已存在
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseMutex(hMutex);
			return  FALSE;   
		}
	}

	g_hTTS = tts_open();
	tts_init(g_hTTS, PATH_TTS_DATA);
	tts_man_role(g_hTTS, false);


	//初始化数据库
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
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	//文本信息下发，或上传
	if( !db.tableExists("text_info") )
	{
		//UID					信息编号
		//text_type				类型，0:下发  1:上传  2:目的地导航(text_content为经纬度)
		//flag					标志，见808协议表27
		//text_content			信息内容(1k字节)
		//read_status			0:未读  1:已读
		//text_datatime			2008-03-19 12:10:15
		db.execDML("CREATE TABLE text_info(							\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   text_type				INTEGER,				\
				   flag						INTEGER,				\
				   text_content				TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   text_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	//将数据库中最近的100条数据保留，老的删除掉
	db.execDML("DELETE FROM text_info WHERE UID NOT IN (SELECT UID FROM text_info ORDER BY UID DESC LIMIT 100);");

	//事件信息
	if( !db.tableExists("event_info") )
	{
		//event_ID					事件ID
		//event_content				事件内容
		db.execDML("CREATE TABLE event_info(						\
				   event_ID					INTEGER PRIMARY KEY,	\
				   event_content			TEXT);");
	}

	//问题信息
	if( !db.tableExists("question") )
	{
		//UID					问题编号
		//flag					标志，见808协议表32
		//question_content		问题内容
		//read_status			0:未读  1:已读
		//question_datatime		提问时间
		db.execDML("CREATE TABLE question(							\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,				\
				   question_content			TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   question_datatime		NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	//将数据库中最近的100条数据保留，老的删除掉
	db.execDML("DELETE FROM question WHERE UID NOT IN (SELECT UID FROM question ORDER BY UID DESC LIMIT 100);");

	//候选答案列表
	if( !db.tableExists("answer") )
	{
		//UID					主键
		//serial_num			提问下发消息的流水号，用于应答
		//answer_ID				答案ID
		//answer_content		答案内容
		db.execDML("CREATE TABLE answer(				\
				   UID						INTEGER,	\
				   serial_num				INTEGER,	\
				   answer_ID				INTEGER,	\
				   answer_content			TEXT,		\
				   PRIMARY KEY (UID, answer_ID));");
	}
	db.execDML("DELETE FROM answer WHERE UID NOT IN (SELECT UID FROM answer ORDER BY UID DESC LIMIT 100);");

	//信息点播菜单
	if( !db.tableExists("info_menu") )
	{
		//UID					菜单编号
		//type					信息类型
		//info_name				信息名称
		//read_status			0:未读  1:已读
		//info_datatime         信息下发时间
		db.execDML("CREATE TABLE info_menu(							\
				   UID						INTEGER,				\
				   type						INTEGER,				\
				   info_name				TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   info_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM info_menu WHERE type IN (SELECT type FROM info_menu LIMIT (SELECT count(*) FROM info_menu)-100);");

	//信息服务
	if( !db.tableExists("info_service") )
	{
		//type					信息类型
		//info_content			信息内容
		//read_status			0:未读  1:已读
		db.execDML("CREATE TABLE info_service(						\
				   type						INTEGER PRIMARY KEY,	\
				   info_content				TEXT,					\
				   read_status				BOOLEAN DEFAULT 0,		\
				   text_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM info_service WHERE type IN (SELECT type FROM info_service LIMIT (SELECT count(*) FROM info_service)-100);");

	//电话回拨
	if( !db.tableExists("call_back") )
	{
		//UID					主键
		//flag					0:普通通话; 1:监听
		//phone_num				电话号码，最长20字节
		//call_datatime			时间
		db.execDML("CREATE TABLE call_back(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,	\
				   phone_number				TEXT,		\
				   call_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM call_back WHERE UID NOT IN (SELECT UID FROM call_back ORDER BY UID DESC LIMIT 100);");

	//电子运单
	if( !db.tableExists("e_waybill") )
	{
		//UID					电子运单编号
		//content				电子运单内容
		//waybill_datatime		电子运单上报时间2008-03-19 12:10:15
		db.execDML("CREATE TABLE e_waybill(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   content					TEXT,		\
				   waybill_datatime			NOT NULL DEFAULT (datetime('now','localtime')));");
	}
	db.execDML("DELETE FROM e_waybill WHERE UID NOT IN (SELECT UID FROM e_waybill ORDER BY UID DESC LIMIT 100);");

	//驾驶员身份信息
	if( !db.tableExists("driver_info") )
	{
		//UID					驾驶员身份信息编号
		//driver_name			驾驶员姓名
		//driver_ID				驾驶员身份证编码
		//certificate_num		从业资格证编码
		//agency				发证机构名称
		db.execDML("CREATE TABLE driver_info(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   driver_name				TEXT,			\
				   driver_ID				TEXT,			\
				   certificate_num			TEXT,			\
				   agency					TEXT);");
	}
	
	//电话本
	if( !db.tableExists("phone_book") )
	{
		//UID					主键
		//flag					标志，1：呼入   2：呼出   3：呼入/呼出
		//name					联系人姓名
		//phone_number			联系人号码
		db.execDML("CREATE TABLE phone_book(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,		\
				   name						TEXT,			\
				   phone_number				TEXT);");
	}
	db.execDML("DELETE FROM phone_book WHERE UID NOT IN (SELECT UID FROM phone_book ORDER BY UID DESC LIMIT 100);");

	//圆形区域
	if( !db.tableExists("region_circular") )
	{
		//region_ID				区域ID
		//region_property		区域属性
		//centerpoint_latitude	中心点纬度
		//centerpoint_longitude	中心点经度
		//radius				半径
		//datetime_begin		起始时间
		//datetime_end			结束时间
		//max_speed				最高速度
		//overspeed_time		超速持续时间
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

	//矩形区域
	if( !db.tableExists("region_rectangle") )
	{
		//region_ID				区域ID
		//region_property		区域属性
		//topleft_latitude		左上点纬度
		//topleft_longitude		左上点经度
		//bottomright_latitude	右下点纬度
		//bottomright_longitude	右下点经度
		//datetime_begin		起始时间
		//datetime_end			结束时间
		//max_speed				最高速度
		//overspeed_time		超速持续时间
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

	//多边形区域
	if( !db.tableExists("region_polygon") )
	{
		//region_ID				区域ID
		//region_property		区域属性
		//datetime_begin		起始时间
		//datetime_end			结束时间
		//max_speed				最高速度
		//overspeed_time		超速持续时间
		db.execDML("CREATE TABLE region_polygon(		\
				   region_ID				INTEGER PRIMARY KEY,	\
				   region_property			INTEGER,	\
				   datetime_begin			DATETIME,	\
				   datetime_end				DATETIME,	\
				   max_speed				INTEGER,	\
				   overspeed_time			INTEGER);");
	}
	db.execDML("DELETE FROM region_polygon WHERE region_ID IN (SELECT region_ID FROM region_polygon LIMIT (SELECT count(*) FROM region_polygon)-200);");

	//多边形区域的顶点
	if( !db.tableExists("polygon_vertex") )
	{
		//region_ID				区域ID
		//vertex_latitude		顶点纬度
		//vertex_longitude		顶点经度
		db.execDML("CREATE TABLE polygon_vertex(		\
				   region_ID				INTEGER,	\
				   vertex_latitude			INTEGER,	\
				   vertex_longitude			INTEGER);");
	}
	db.execDML("DELETE FROM polygon_vertex WHERE region_ID IN (SELECT region_ID FROM polygon_vertex LIMIT (SELECT count(*) FROM polygon_vertex)-200);");

	//路线
	if( !db.tableExists("line") )
	{
		//line_ID				路线ID
		//line_property			路线属性
		//datetime_begin		起始时间
		//datetime_end			结束时间
		db.execDML("CREATE TABLE line(					\
				   line_ID					INTEGER PRIMARY KEY, \
				   line_property			INTEGER,	\
				   datetime_begin			DATETIME,	\
				   datetime_end				DATETIME);");
	}
	db.execDML("DELETE FROM line WHERE line_ID IN (SELECT line_ID FROM line LIMIT (SELECT count(*) FROM line)-200);");

	//路线拐点
	if( !db.tableExists("line_inflexion") )
	{
		//line_ID				路线ID
		//inflexion_ID			拐点ID
		//inflexion_latitude	拐点纬度
		//inflexion_longitude	拐点经度
		//section_width			路段宽度
		//section_property		路段属性
		//drive_threshold_max	路段行驶过长阈值
		//drive_threshold_min	路段行驶不足阈值
		//max_speed				最高速度
		//overspeed_time		超速持续时间
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

	//短信
	if( !db.tableExists("sms") )
	{
		//UID					信息编号
		//sms_type				类型，1:收件箱  2:发件箱  3:草稿箱	4:删除
		//phone_number			电话号码
		//sms_content			信息内容(140字节)
		//read_status			0:未读  1:已读
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

	
	//通话记录
	if( !db.tableExists("phone_log") )
	{
		//UID					主键
		//flag					标志，1：已拨   2：已接   3：未接
		//phone_number			联系人号码	联系人姓名根据号码到phone_book表中查询
		//call_duration			通话时长，单位s(秒)
		//log_datatime
		db.execDML("CREATE TABLE phone_log(				\
				   UID						INTEGER PRIMARY KEY AUTOINCREMENT,	\
				   flag						INTEGER,	\
				   phone_number				TEXT,		\
				   call_duration			INTEGER DEFAULT 0,	\
				   log_datatime				NOT NULL DEFAULT (datetime('now','localtime')));");
	}

	//多媒体存储信息
	if( !db.tableExists("multimedia_Info") )
	{
		//multimedia_ID					多媒体ID
		// multimedia_type				多媒体类型
		//event_code					事件项编码
		//multimedia_path				多媒体路径
		//position_info					位置信息
		db.execDML("CREATE TABLE multimedia_Info(			\
				   multimedia_ID				INTEGER,	\
				   multimedia_type				INTEGER,	\
				   event_code				    INTEGER,	\
				   multimedia_path				TEXT,		\
				   position_info				TEXT,       \
				   log_datatime				    NOT NULL DEFAULT (datetime('now','localtime')));");
	}

	//拍照上传照片数目保存路径
	if( !db.tableExists("photo_Info") )
	{
		//photo_num					保存的未拍摄照片数目
		db.execDML("CREATE TABLE photo_Info(			\
				   photo_num				INTEGER,	\
				   photo_time				INTEGER,    \
				   log_datatime				NOT NULL DEFAULT (datetime('now','localtime')));");

		db.execDML("INSERT INTO photo_Info(photo_num,photo_time) VALUES(0,0);");
	}


	//将数据库中一个月之前的数据删除掉
	db.execDML("DELETE FROM text_info WHERE text_datatime < (SELECT datetime('now','localtime', '-1 month'));");
	//将数据库中最近的100条数据保留，老的删除掉
	db.execDML("DELETE FROM text_info WHERE UID NOT IN (SELECT UID FROM text_info ORDER BY UID DESC LIMIT 100);");
	//DELETE FROM event_info WHERE event_ID IN (SELECT event_ID FROM event_info LIMIT (SELECT count(*) FROM event_info)-100);
	
	db.close();	//关闭数据库
}
