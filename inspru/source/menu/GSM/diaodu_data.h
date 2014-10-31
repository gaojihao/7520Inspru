
#ifndef  DIAODU_DATA_H
#define  DIAODU_DATA_H

#include "../ExTool/ex_file.h"

//中心信息文件
#define PATH_TZ_BUFFER_SMS_CENTER			"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_center.txt"
//救援任务信息文件
#define PATH_TZ_BUFFER_SMS_TASK				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_task.txt"
//定期定点信息
#define PATH_TZ_BUFFER_SMS_TASK_POINT		"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_task_node.txt"
//用于回复:定期定点的开始,结束  救援的开始,结束   节点的到达,离开
#define PATH_TZ_BUFFER_SMS_TASK_STYLE		"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_task_style.txt"
//IP,port文件
#define PATH_TZ_BUFFER_IP_PORT				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\ip_port.txt"
//IC卡信息文件
#define PATH_TZ_BUFFER_IC_CARD				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\ic_card.txt"
//RFID文件，发送消息WPARAM_SEND_RFID_CMD时使用
#define PATH_TZ_BUFFER_RFID					"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\rfid.txt"
//条形码文件
#define PATH_TZ_BUFFER_BAR_CODE				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\bar_code.txt"
//IMEI文件
#define PATH_TZ_BUFFER_IMEI					"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\imei.txt"
//(下)取消、终止任务文件，包括救援任务和定期定点任务
#define PATH_TZ_BUFFER_CABCLE_CUT_TASK		"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\task_cancle_cut.txt"
//照片路径
#define PATH_TZ_PIC							"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\pic\\"

//中心信息
#define PATH_SMS_CENTER						"\\Flashdrv Storage\\menu\\SMS\\sms_center.sms"
//救援任务信息
#define PATH_SMS_TASK_SAVING				"\\Flashdrv Storage\\menu\\SMS\\sms_task_saving.sms"
//救援任务地点信息
#define PATH_SMS_SAVING_POINT				"\\Flashdrv Storage\\menu\\SMS\\sms_saving_point.sms"
//定期定点任务单
#define PATH_SMS_TASK_DING					"\\Flashdrv Storage\\menu\\SMS\\sms_task_ding.sms"
//定期定点任务单中每点的信息
#define PATH_SMS_DING_POINT					"\\Flashdrv Storage\\menu\\SMS\\sms_ding_point.sms"

enum enSMS_TYPE
{
		SMS_TASK_SAVING = 0,			//救援任务
		SMS_TASK_DING	= 1,				//定期定点任务
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
//中心信息(下发,上传)(点击回复按钮时发送)
typedef struct tagText
{
	unsigned		m_infoNumber;			//信息序号
	char			m_infoContext[300];		//信息内容
} st_Text;
//(文件存储结构体)PATH_SMS_CENTER
typedef struct tagSMSCenter 
{
	char		m_time[14];			//080319121015 2008-03-19 12:10:15
	char		m_replyType;		//0:不需要回复  1:需要回复
	char		m_status;			//0:未回复  1:已回复
	st_Text		m_Text;				//中心信息
} st_SMSCenter;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//紧急救援信息(下发)(件存储结构体)PATH_SMS_TASK
typedef struct tagSMSTask 
{
	char		m_taskID[12];			//任务编号:123456789
	char		m_time[15];				//下发时间:"YYMMDDHHMMSS"
	char		m_status;				//任务完成状态:	0 未开始   1救援开始、2救援结束(已完成)  3取消     4终止
	char		m_licensePlate [32];	//救援车牌号
	char		m_guestName[60];		//客户名称:江苏天泽
	//char		m_address[60];			//联系方式:南京市建邺区云龙山路80号
	unsigned	m_longitude;			//救援地点经度:12345678 (12.345678)
	unsigned	m_latitude;				//救援地点纬度:123456789 (123.456789)
	char		m_rescue_address[60];	//救援地点名称:江苏天泽
	char		m_context[160];			//备注:这是备注！
} st_SMSTask;
//救援开始结束信息(上传)(点击救援开始，结束按钮时发送)
//废弃
typedef struct tagHelpStartEnd 
{
	char			m_taskID[12];			//任务编号:123456789
	unsigned		m_style;				//类型:0救援开始、1救援结束
} st_HelpStartEnd;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//ip,port(下发)
typedef struct tagIP_Port
{
	char		m_IP[4];		//IP地址
	unsigned	m_port;			//端口
} st_IP_Port;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//IC卡(上传)(刷卡后点击确认按钮时发送)
typedef struct tagIC_Card
{
	char		m_ID[32];		//IC卡ID号
	char		m_PWD[64];		//密码
} st_IC_Card;


/////////////////////////////////////////////////////////////////////////////////////////////////////
//定点定期和节点信息(下发)
typedef struct tagSMSTaskNode 
{
	char		m_taskID[12];					//任务编号
	char		m_timeStart[6];					//任务计划开始时间
	char		m_timeEnd[6];					//任务计划结束时间
	char		m_nodeCurCustomerName[60];		//本节点客户名称
	char		m_nodeCurArriveTime[8];			//本节点计划到达时间
	char		m_nodeCurCustomerInfo[60];		//本节点客户信息
	unsigned		m_longitude;				//本节点经度
	unsigned		m_latitude;					//本节点纬度
	char		m_nodeCurAddress[60];			//本节点定点地址名称
	char		m_nodeCurContext[150];			//备注:这是备注！
	unsigned short m_nodeNumber;				//节点数量
	unsigned  m_nodeCurIndex;					//本节点编号
} st_SMSTaskNode;
//定期定点和紧急救援的终止和取消(下发)
typedef struct _CancleTask
{
	char			m_taskID[10];		//任务编号
	unsigned char	ucTaskStyle;		//任务类型：0：定期定点任务		1：紧急救援任务
	unsigned char	ucOperateStyle;		//操作类型：0：取消				1：终止
}T_CancleTask;
//到达、离开节点信息(上传)(点击到达，离开按钮时发送)
//使用情况: 救援开始，结束   定点定期任务的开始,结束   定点定期节点到达、离开   
typedef struct tagArriveLeaveNode 
{
	char			m_taskID[12];			//任务编号
	unsigned		m_longitude;			//本节点经度
	unsigned		m_latitude;				//本节点纬度
	unsigned int	m_nodeCurIndex;			//本节点编号
	unsigned char	m_isBlindArea;			//是否盲区
	unsigned char	m_speed;				//速度
	unsigned char	m_direction;			//方向
	unsigned char	m_style;				//类型：救援开始0、结束1   节点到达0、离开1   定点定期任务开始0、结束1
} st_ArriveLeaveNode;
//(文件存储结构体)	定点定期任务文件
typedef struct tagTaskPoint
{
	char			m_taskID[12];			//任务编号
	//char			m_time[4];				//下发时间:YYMMDD
	char			m_timeStart[6];			//任务计划开始时间
	char			m_timeEnd[6];			//任务计划结束时间
	short			m_nodeNumber;			//节点数量
	short			m_state;				//状态：0未开始   1执行中    2已完成    3取消     4终止
} st_TaskPoint;
//(文件存储结构体)	定点定期节点信息文件
typedef struct tagPointInfo
{
	char			m_taskID[12];			//任务编号
	unsigned int	m_nodeCurIndex;			//本节点编号
	unsigned int	m_state;				//状态：0未到达  1到达  2离开
	char			m_nodeCurCustomerName[60];	//本节点客户名称
	char			m_nodeCurArriveTime[8];		//本节点计划到达时间
	char			m_nodeCurCustomerInfo[60];	//本节点客户信息
	unsigned		m_longitude;			//本节点经度
	unsigned		m_latitude;				//本节点纬度
	char			m_nodeCurAddress[60];	//本节点定点地址名称
	char			m_nodeCurContext[160];	//备注:这是备注！
} st_PointInfo;
/////////////////////////////////////////////////////////////////////////////////////////////////////



//中心信息
class CSMSCenter_data
{
public:
	CSMSCenter_data();
	~CSMSCenter_data();
	static CSMSCenter_data* Instance();

	enum { reserve_number = 100 };

public:
	bool			load_data();	  //从文件中读入数据到结构体
	bool			save_data(bool bClear);	  //bClear:是否将内存中的数据清空
	bool			delete_record( int n_idx );		  //删除记录，置m_type为0
	bool			add_recourd(st_SMSCenter* p_msg );		  //文件最后增加一结构体

	//删除所有的记录
	void			remove_all() { m_count = 0; m_is_change = true; };
	int				max_number();		              //返回短信的条数m_count
	st_SMSCenter*	get_recourd( int n_idx );		  //按索引，返回结构体的指针
	bool			read_sms(int n_idx);			  //读索引号的消息，置m_type:1
	bool			is_open() { return m_is_open; }
	void			user_change(bool b_change) { m_is_change = b_change; }

	static bool		add_2file_end( st_SMSCenter* p_msg );   //添加用例到文件

protected:
	static CSMSCenter_data	m_instance;
	static bool				m_is_open;
	bool					m_is_change;
	st_SMSCenter*			m_list;
	int						m_count;
	int						m_max;
	static const char*		m_path;
};

//救援信息
class CSMSTask_data
{
public:
	CSMSTask_data();
	~CSMSTask_data();
	static CSMSTask_data* Instance();

	enum { reserve_number = 100 };

public:
	bool			load_data();	  //从文件中读入数据到结构体
	bool			save_data(bool bClear);	  //bClear:是否将内存中的数据清空
	bool			delete_record( int n_idx );		  //删除记录，置m_type为0
	bool			add_recourd(st_SMSTask* p_msg );		  //文件最后增加一结构体

	//删除所有的记录
	void			remove_all() { m_count = 0; m_is_change = true; };
	int				max_number();		              //返回短信的条数m_count
	st_SMSTask*	get_recourd( int n_idx );		  //按索引，返回结构体的指针
	bool			read_sms(int n_idx);			  //读索引号的消息，置m_type:1
	bool			is_open() { return m_is_open; }
	bool			cancel_task(char* pTaskID, int nLen);	//
	void			user_change(bool b_change) { m_is_change = b_change; }

	static bool		add_2file_end( st_SMSTask* p_msg );   //添加用例到文件

protected:
	static CSMSTask_data	m_instance;
	static bool				m_is_open;
	bool					m_is_change;
	st_SMSTask*				m_list;
	int						m_count;
	int						m_max;
	static const char*		m_path;
};
#endif //DIAODU_DATA_H