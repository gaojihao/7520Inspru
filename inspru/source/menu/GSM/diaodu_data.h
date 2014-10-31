
#ifndef  DIAODU_DATA_H
#define  DIAODU_DATA_H

#include "../ExTool/ex_file.h"

//������Ϣ�ļ�
#define PATH_TZ_BUFFER_SMS_CENTER			"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_center.txt"
//��Ԯ������Ϣ�ļ�
#define PATH_TZ_BUFFER_SMS_TASK				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_task.txt"
//���ڶ�����Ϣ
#define PATH_TZ_BUFFER_SMS_TASK_POINT		"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_task_node.txt"
//���ڻظ�:���ڶ���Ŀ�ʼ,����  ��Ԯ�Ŀ�ʼ,����   �ڵ�ĵ���,�뿪
#define PATH_TZ_BUFFER_SMS_TASK_STYLE		"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\sms_task_style.txt"
//IP,port�ļ�
#define PATH_TZ_BUFFER_IP_PORT				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\ip_port.txt"
//IC����Ϣ�ļ�
#define PATH_TZ_BUFFER_IC_CARD				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\ic_card.txt"
//RFID�ļ���������ϢWPARAM_SEND_RFID_CMDʱʹ��
#define PATH_TZ_BUFFER_RFID					"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\rfid.txt"
//�������ļ�
#define PATH_TZ_BUFFER_BAR_CODE				"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\bar_code.txt"
//IMEI�ļ�
#define PATH_TZ_BUFFER_IMEI					"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\imei.txt"
//(��)ȡ������ֹ�����ļ���������Ԯ����Ͷ��ڶ�������
#define PATH_TZ_BUFFER_CABCLE_CUT_TASK		"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\buffer_data\\task_cancle_cut.txt"
//��Ƭ·��
#define PATH_TZ_PIC							"\\Flashdrv Storage\\TZ_GPS_VEHICLE\\pic\\"

//������Ϣ
#define PATH_SMS_CENTER						"\\Flashdrv Storage\\menu\\SMS\\sms_center.sms"
//��Ԯ������Ϣ
#define PATH_SMS_TASK_SAVING				"\\Flashdrv Storage\\menu\\SMS\\sms_task_saving.sms"
//��Ԯ����ص���Ϣ
#define PATH_SMS_SAVING_POINT				"\\Flashdrv Storage\\menu\\SMS\\sms_saving_point.sms"
//���ڶ�������
#define PATH_SMS_TASK_DING					"\\Flashdrv Storage\\menu\\SMS\\sms_task_ding.sms"
//���ڶ���������ÿ�����Ϣ
#define PATH_SMS_DING_POINT					"\\Flashdrv Storage\\menu\\SMS\\sms_ding_point.sms"

enum enSMS_TYPE
{
		SMS_TASK_SAVING = 0,			//��Ԯ����
		SMS_TASK_DING	= 1,				//���ڶ�������
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ(�·�,�ϴ�)(����ظ���ťʱ����)
typedef struct tagText
{
	unsigned		m_infoNumber;			//��Ϣ���
	char			m_infoContext[300];		//��Ϣ����
} st_Text;
//(�ļ��洢�ṹ��)PATH_SMS_CENTER
typedef struct tagSMSCenter 
{
	char		m_time[14];			//080319121015 2008-03-19 12:10:15
	char		m_replyType;		//0:����Ҫ�ظ�  1:��Ҫ�ظ�
	char		m_status;			//0:δ�ظ�  1:�ѻظ�
	st_Text		m_Text;				//������Ϣ
} st_SMSCenter;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//������Ԯ��Ϣ(�·�)(���洢�ṹ��)PATH_SMS_TASK
typedef struct tagSMSTask 
{
	char		m_taskID[12];			//������:123456789
	char		m_time[15];				//�·�ʱ��:"YYMMDDHHMMSS"
	char		m_status;				//�������״̬:	0 δ��ʼ   1��Ԯ��ʼ��2��Ԯ����(�����)  3ȡ��     4��ֹ
	char		m_licensePlate [32];	//��Ԯ���ƺ�
	char		m_guestName[60];		//�ͻ�����:��������
	//char		m_address[60];			//��ϵ��ʽ:�Ͼ��н���������ɽ·80��
	unsigned	m_longitude;			//��Ԯ�ص㾭��:12345678 (12.345678)
	unsigned	m_latitude;				//��Ԯ�ص�γ��:123456789 (123.456789)
	char		m_rescue_address[60];	//��Ԯ�ص�����:��������
	char		m_context[160];			//��ע:���Ǳ�ע��
} st_SMSTask;
//��Ԯ��ʼ������Ϣ(�ϴ�)(�����Ԯ��ʼ��������ťʱ����)
//����
typedef struct tagHelpStartEnd 
{
	char			m_taskID[12];			//������:123456789
	unsigned		m_style;				//����:0��Ԯ��ʼ��1��Ԯ����
} st_HelpStartEnd;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//ip,port(�·�)
typedef struct tagIP_Port
{
	char		m_IP[4];		//IP��ַ
	unsigned	m_port;			//�˿�
} st_IP_Port;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//IC��(�ϴ�)(ˢ������ȷ�ϰ�ťʱ����)
typedef struct tagIC_Card
{
	char		m_ID[32];		//IC��ID��
	char		m_PWD[64];		//����
} st_IC_Card;


/////////////////////////////////////////////////////////////////////////////////////////////////////
//���㶨�ںͽڵ���Ϣ(�·�)
typedef struct tagSMSTaskNode 
{
	char		m_taskID[12];					//������
	char		m_timeStart[6];					//����ƻ���ʼʱ��
	char		m_timeEnd[6];					//����ƻ�����ʱ��
	char		m_nodeCurCustomerName[60];		//���ڵ�ͻ�����
	char		m_nodeCurArriveTime[8];			//���ڵ�ƻ�����ʱ��
	char		m_nodeCurCustomerInfo[60];		//���ڵ�ͻ���Ϣ
	unsigned		m_longitude;				//���ڵ㾭��
	unsigned		m_latitude;					//���ڵ�γ��
	char		m_nodeCurAddress[60];			//���ڵ㶨���ַ����
	char		m_nodeCurContext[150];			//��ע:���Ǳ�ע��
	unsigned short m_nodeNumber;				//�ڵ�����
	unsigned  m_nodeCurIndex;					//���ڵ���
} st_SMSTaskNode;
//���ڶ���ͽ�����Ԯ����ֹ��ȡ��(�·�)
typedef struct _CancleTask
{
	char			m_taskID[10];		//������
	unsigned char	ucTaskStyle;		//�������ͣ�0�����ڶ�������		1��������Ԯ����
	unsigned char	ucOperateStyle;		//�������ͣ�0��ȡ��				1����ֹ
}T_CancleTask;
//����뿪�ڵ���Ϣ(�ϴ�)(�������뿪��ťʱ����)
//ʹ�����: ��Ԯ��ʼ������   ���㶨������Ŀ�ʼ,����   ���㶨�ڽڵ㵽��뿪   
typedef struct tagArriveLeaveNode 
{
	char			m_taskID[12];			//������
	unsigned		m_longitude;			//���ڵ㾭��
	unsigned		m_latitude;				//���ڵ�γ��
	unsigned int	m_nodeCurIndex;			//���ڵ���
	unsigned char	m_isBlindArea;			//�Ƿ�ä��
	unsigned char	m_speed;				//�ٶ�
	unsigned char	m_direction;			//����
	unsigned char	m_style;				//���ͣ���Ԯ��ʼ0������1   �ڵ㵽��0���뿪1   ���㶨������ʼ0������1
} st_ArriveLeaveNode;
//(�ļ��洢�ṹ��)	���㶨�������ļ�
typedef struct tagTaskPoint
{
	char			m_taskID[12];			//������
	//char			m_time[4];				//�·�ʱ��:YYMMDD
	char			m_timeStart[6];			//����ƻ���ʼʱ��
	char			m_timeEnd[6];			//����ƻ�����ʱ��
	short			m_nodeNumber;			//�ڵ�����
	short			m_state;				//״̬��0δ��ʼ   1ִ����    2�����    3ȡ��     4��ֹ
} st_TaskPoint;
//(�ļ��洢�ṹ��)	���㶨�ڽڵ���Ϣ�ļ�
typedef struct tagPointInfo
{
	char			m_taskID[12];			//������
	unsigned int	m_nodeCurIndex;			//���ڵ���
	unsigned int	m_state;				//״̬��0δ����  1����  2�뿪
	char			m_nodeCurCustomerName[60];	//���ڵ�ͻ�����
	char			m_nodeCurArriveTime[8];		//���ڵ�ƻ�����ʱ��
	char			m_nodeCurCustomerInfo[60];	//���ڵ�ͻ���Ϣ
	unsigned		m_longitude;			//���ڵ㾭��
	unsigned		m_latitude;				//���ڵ�γ��
	char			m_nodeCurAddress[60];	//���ڵ㶨���ַ����
	char			m_nodeCurContext[160];	//��ע:���Ǳ�ע��
} st_PointInfo;
/////////////////////////////////////////////////////////////////////////////////////////////////////



//������Ϣ
class CSMSCenter_data
{
public:
	CSMSCenter_data();
	~CSMSCenter_data();
	static CSMSCenter_data* Instance();

	enum { reserve_number = 100 };

public:
	bool			load_data();	  //���ļ��ж������ݵ��ṹ��
	bool			save_data(bool bClear);	  //bClear:�Ƿ��ڴ��е��������
	bool			delete_record( int n_idx );		  //ɾ����¼����m_typeΪ0
	bool			add_recourd(st_SMSCenter* p_msg );		  //�ļ��������һ�ṹ��

	//ɾ�����еļ�¼
	void			remove_all() { m_count = 0; m_is_change = true; };
	int				max_number();		              //���ض��ŵ�����m_count
	st_SMSCenter*	get_recourd( int n_idx );		  //�����������ؽṹ���ָ��
	bool			read_sms(int n_idx);			  //�������ŵ���Ϣ����m_type:1
	bool			is_open() { return m_is_open; }
	void			user_change(bool b_change) { m_is_change = b_change; }

	static bool		add_2file_end( st_SMSCenter* p_msg );   //����������ļ�

protected:
	static CSMSCenter_data	m_instance;
	static bool				m_is_open;
	bool					m_is_change;
	st_SMSCenter*			m_list;
	int						m_count;
	int						m_max;
	static const char*		m_path;
};

//��Ԯ��Ϣ
class CSMSTask_data
{
public:
	CSMSTask_data();
	~CSMSTask_data();
	static CSMSTask_data* Instance();

	enum { reserve_number = 100 };

public:
	bool			load_data();	  //���ļ��ж������ݵ��ṹ��
	bool			save_data(bool bClear);	  //bClear:�Ƿ��ڴ��е��������
	bool			delete_record( int n_idx );		  //ɾ����¼����m_typeΪ0
	bool			add_recourd(st_SMSTask* p_msg );		  //�ļ��������һ�ṹ��

	//ɾ�����еļ�¼
	void			remove_all() { m_count = 0; m_is_change = true; };
	int				max_number();		              //���ض��ŵ�����m_count
	st_SMSTask*	get_recourd( int n_idx );		  //�����������ؽṹ���ָ��
	bool			read_sms(int n_idx);			  //�������ŵ���Ϣ����m_type:1
	bool			is_open() { return m_is_open; }
	bool			cancel_task(char* pTaskID, int nLen);	//
	void			user_change(bool b_change) { m_is_change = b_change; }

	static bool		add_2file_end( st_SMSTask* p_msg );   //����������ļ�

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