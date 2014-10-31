#ifndef  MSG_CUNION_H
#define  MSG_CUNION_H

#include "msg_mid.h"

enum eDevType
{
	eDev_GPS        = 0x01,   //GPS�����ն�һ���
	eDev_GETWAY     = 0x02,   //GPS�ն�����
	eDev_MOBILE     = 0x03,   //�ֻ��豸
};

enum eMsgType
{                        //һ��        //����       //����        
	ec_heart_beat        = 0x00, //C_CSHeartBeat	    �ն�����
	ec_order_confirm     = 0x01, //C_CSOrderConfirm	    �ն�����ظ�
	ec_register_client   = 0x02, //C_CSReg              �ն��������ע�� 
	ec_connect           = 0X03, //C_CSConnect	        �ն��������غ������������
	ec_disconnect        = 0x04, //C_CSDisconnect	    �ն˶Ͽ�����������
    ec_position_info     = 0x05, //C_CSPositionInfo	    �ն�λ����Ϣ�ϱ�
    ec_alert_notify      = 0x06, //C_CSalertNotify	    ���ͽ���������Ϣ
	ec_get_para          = 0x07, //C_CSGetPara          �ն˽��յ�S_SCGetPara������ȡ��Ӧ����
	ec_file_rcv          = 0x0E, //C_CSFileRcv	        �ļ�����ظ�
	ec_file_req          = 0x0F, //C_CSFileReq    	    �����ļ�
	ec_trans_file        = 0x10, //C_CSTransFile        �ն˷������������ļ�
	ec_certification	 = 0x11, //C_CSCertification	
	ec_vehicle_status	 = 0x12, //C_CSVehicleStatus	


	//�����·�
	es_heart_beat        = 0x20, //S_SCCHeartBeat	    �����ظ�
	es_order_confirm     = 0x21, //S_SCOrderConfirm	    ����ظ�
	es_alert_release     = 0x22, //S_SCalertRelease	    ȡ����������==>C_CSOrderConfirm
	es_param_time        = 0x23, //S_SCSetPara_Time	    �����ն�ʱ�����==>C_CSOrderConfirm

	es_param_net         = 0x24, //S_SCSetPara_Net	   �����ն��������==>C_CSOrderConfirm
	es_setparam_alert    = 0x25, //S_SCSetPara_Alert  �������ն˷���==>>C_CSOrderConfirm
	es_setparam_adress   = 0x26, //S_SCSetParam_Adress  ���������ն˷���==>>C_CSOrderConfirm
	es_get_param         = 0x27, //S_SCGetPara �ɹ���C_CSGetPara,ʧ����C_CSOrderConfirm�ظ�
	es_setoil_elec       = 0x28, //S_SCSetOil_Elec      �������ն˷���,��C_CSorderConfirm�ظ�
	es_listen            = 0x29, //S_SCListen           �������ն˷���,��ظ�C_CSOrderConfirm�ظ�
	es_get_photo         = 0x2A, //S_SCPhoto            ��ظ�C_CSOrderConfirm�ظ�
	es_send_msg          = 0x2B, //S_SCSendMsg          �������ն˷���,C_CSOrderConfirm
	es_file_rcv          = 0x2E, //S_SCFileRcv          ���������յ��ն��ϴ����ļ�������ն˻�Ӧ
	es_trans_file        = 0x2F, //S_SCTransFile
	es_pos_request		 = 0x30, //S_SCPosRequest		�����������ն�λ����Ϣ //@z
};

//��������ѯ�����Ĳ���
//1��ʱ�������2�����������3������������4������汾
enum svr_param_type
{ svr_type_time  = 1,   //ʱ�����
  svr_type_net   = 2,   //�������
  svr_type_alarm = 3,   //��������
  svr_type_soft  = 4,   //����汾
  svr_type_error
};



//FHT��������Ϣ����
enum fth_svr_cmd{

	fth_svr_modify_gps_slice = 0x01,  //�޸ļ��Ƶ��
	fth_svr_view_gps_slice   = 0x02,  //�鿴GPS���Ƶ��
	fth_svr_report_pos_one   = 0x03,  //�����鿴��ǰ��λ����Ϣ
	fth_svr_searching_device = 0x04,  //���������ڵĻ���
	fth_svr_set_overspeed    = 0x05,  //���ó����ٶ�
	fth_svr_view_overspeed   = 0x06,  //�鿴�����ٶ�
	fth_svr_cancel_overspeed = 0x07,  //ȡ�������ٶ�
	fth_svr_tip_overspeed    = 0xFF,  //����������ʾ

	fth_svr_set_listen_phone     = 0x18,   //���ü�������
	fth_svr_view_listen_phone    = 0x19,   //�鿴��������
	fth_svr_cancel_listen_phone  = 0x1A,   //ȡ����������
	fth_svr_modify_ip            = 0x1B,   //�����µ�IP��ַ
	fth_svr_view_ip              = 0x1C,   //�鿴��ǰIP��ַ
	fth_svr_modify_udp_port      = 0x1D,   //�޸�UDP�Ķ˿ں�
	fth_svr_view_udp_port        = 0x1E,   //�鿴UDP�Ķ˿ں�
	fth_svr_set_heart            = 0x21,   //��������ʱ��
	fth_svr_view_heart           = 0x22,   //�鿴����ʱ��
	fth_svr_set_apn              = 0x23,   //����APN����
	fth_svr_view_apn             = 0x24,   //�鿴APN����
	fth_svr_set_ppp_name         = 0x25,   //����PPP�û���
	fth_svr_view_ppp_name        = 0x26,   //�鿴PPP�û���
	fth_svr_set_ppp_pwd          = 0x27,   //����PPP����
	fth_svr_view_ppp_pwd         = 0x28,   //�鿴PPP����
	fth_svr_null
};

typedef struct{
	int   m_num;
	int   m_cur_idx;
	char* m_data;
	int   m_length;
	int   m_total_size;
}stfile_page; //download file param

//@z
typedef struct
{
	unsigned int    order_id;		//����������ACK��ʱ����
	unsigned char   ordermsg_id;	//OrderMessageID
	unsigned int	result;			//0:ִ��ʧ��; 1:ִ�гɹ�
}storder_confirm;

//////////////////////////////////////////////////////////////////////////
typedef struct
{
	unsigned char  ip_svr[4];
	unsigned short svr_port;
	unsigned char  ip_gate[4];
	unsigned char  ip_dns[4];
	char           dns_address[32];
	char           apn_name[32];
	char           apn_user[32];
	char           apn_pwd[32];
}stnet_param;

//////////////////////////////////////////////////////////////////////////

typedef struct{
		unsigned short user_alarm;      //..21	Time1	WORD	2	�ֶ�����ʱ�ķ��ͼ������λ�룬��Χ1~0xfffe
		unsigned short vehicle_empty;   //time..13	Time2	WORD	2	�ճ�ʱ�ķ��ͼ������λ�룬��Χ1~0xfffe
		unsigned short vehicle_full;    //..25	Time3	WORD	2	�س�ʱ�ķ��ͼ������λ�룬��Χ1~0xfffe
		unsigned short acc_off;         //..27	Time4	WORD	2	ACC�ر�ʱ���ͼ������λ�룬��Χ1~0xfffe
		unsigned char  over_speed;      //Time5	BYTE	1	���ó�ʱ��ʻʱ�䣬��λСʱ����Χ1-254
		unsigned char  over_time;       //Time6	BYTE	1	���ó�ʱͣ��ʱ�䣬��λСʱ����Χ1-254
		unsigned char  acc_off_standby; //Time7	BYTE	1	����ACC�رն���ʱ������ߣ���λ���ӣ���Χ1-254
		unsigned char  heart_beat;      //Time8	BYTE	1	�����������ķ��ͼ������λ�룬��Χ1-254
}sttime_param;



typedef struct{
	unsigned int   order_id;   //����������ACK��ʱ��ʹ��
	char*          msg_ptr;    //��Ϣ����
	unsigned int   msg_size;   //��Ϣ��С
}stsvr_msg;

typedef struct{
	unsigned int   order_id;   //����������ACK��ʱ����
	unsigned short speed_ltd;     //�����ٶ�
	unsigned short speed_time;    //����ʱ��
}stspeed_alert;


typedef struct{
	unsigned int    order_id;  //����������ACK��ʱ����
	unsigned char   ch_type;   //1��������2��ͨ��
	char            ch_phone[19];
}stlisten_phone;

typedef struct{
	unsigned int    order_id;  //����������ACK��ʱ����
	unsigned char   ch_type;   //������ͣ�1 �������У�2 ���κ��У�3 ��ʱ����
	DWORD			nCount;//��λ���Ļش�������ֻ��"���κ���"��Ч��
	DWORD			nTimeInterval;//��λ���Ļش����ʱ�䣬���ʱ��Ϊ0��ֹͣ����ֻ��"���κ���"��"��ʱ����"��Ч��
}stpos_request;

typedef struct{
	unsigned int    order_id;  //����������ACK��ʱ����
	unsigned char   ch_type;   //1���Ͽ���2������ 
}stsetoil_elec;

typedef struct{
	char   user_name[32];
	char   user_num[32];
}stsvr_phone;

typedef  struct{
	unsigned char   action_type;  //1����ʼ��2��ֹͣ
	unsigned        inter_time;   //ʱ��������λ����
	unsigned        photo_num;    //��������
}stphoto_capture;

//////////////////////////////////////////////////////////////////////////

typedef struct{
	unsigned    gps_slice;          //GPSʱ���ϴ���Ƶ��
	unsigned    is_over_speed;      //�Ƿ��ڳ���״̬
	unsigned    gps_over_speed;     //���ٷ�ֵ
	unsigned    svr_heart;          //����ʱ��
	char        user_name[16];      //�û���
	unsigned    listen_idx;
	char        listen_phone1[16];  //�����û���1
	char        listen_phone2[16];  //�����û���2
	float       Lat1;
	float       Long1;
	float       Lat2;
	float       Long2;
	char        svr_ip[64];         //����IP��ַ
	unsigned    svr_port;           //�˿ں�
	char        apn_name[16];        //APN����
	char        ppp_name[16];        //PPP����
	char        ppp_pwd[16];        //PPP����
}stfht_param;

//////////////////////////////////////////////////////////////////////////

class  msg_cunion : public msg_mid
{
public:
	msg_cunion(char* sz_buf, int n_size);
	~msg_cunion() {}


public:
	void   fht_pack_ack_view_ppp_pwd( stfht_param* p_param );
public:
	unsigned   fth_ack_parse(const char* p_buf, int n_size, stfht_param* p_param );

public:
	void   pack_download_file_req();

public:
	bool   check_ack_file_trans(stfile_page* p_page);

	static void  set_device_id(const char* p_device_id );
	static char* parse_gprs_ack_data(const char* p_ack, int* n_data_len);

public:
	static  unsigned    g_certification_ID;

	static  char g_device_id[15];
	static  unsigned  g_uuid_count;
protected:
};


#endif //MSG_CUNION_H