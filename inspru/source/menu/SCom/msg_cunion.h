#ifndef  MSG_CUNION_H
#define  MSG_CUNION_H

#include "msg_mid.h"

enum eDevType
{
	eDev_GPS        = 0x01,   //GPS车载终端一体机
	eDev_GETWAY     = 0x02,   //GPS终端网关
	eDev_MOBILE     = 0x03,   //手机设备
};

enum eMsgType
{                        //一期        //二期       //三期        
	ec_heart_beat        = 0x00, //C_CSHeartBeat	    终端心跳
	ec_order_confirm     = 0x01, //C_CSOrderConfirm	    终端命令回复
	ec_register_client   = 0x02, //C_CSReg              终端向服务器注册 
	ec_connect           = 0X03, //C_CSConnect	        终端连接网关后向服务器发送
	ec_disconnect        = 0x04, //C_CSDisconnect	    终端断开服务器连接
    ec_position_info     = 0x05, //C_CSPositionInfo	    终端位置信息上报
    ec_alert_notify      = 0x06, //C_CSalertNotify	    发送紧急报警消息
	ec_get_para          = 0x07, //C_CSGetPara          终端接收到S_SCGetPara命令后获取相应参数
	ec_file_rcv          = 0x0E, //C_CSFileRcv	        文件传输回复
	ec_file_req          = 0x0F, //C_CSFileReq    	    请求文件
	ec_trans_file        = 0x10, //C_CSTransFile        终端发服务器发送文件
	ec_certification	 = 0x11, //C_CSCertification	
	ec_vehicle_status	 = 0x12, //C_CSVehicleStatus	


	//服务下发
	es_heart_beat        = 0x20, //S_SCCHeartBeat	    心跳回复
	es_order_confirm     = 0x21, //S_SCOrderConfirm	    命令回复
	es_alert_release     = 0x22, //S_SCalertRelease	    取消紧急报警==>C_CSOrderConfirm
	es_param_time        = 0x23, //S_SCSetPara_Time	    设置终端时间参数==>C_CSOrderConfirm

	es_param_net         = 0x24, //S_SCSetPara_Net	   设置终端网络参数==>C_CSOrderConfirm
	es_setparam_alert    = 0x25, //S_SCSetPara_Alert  服务向终端发送==>>C_CSOrderConfirm
	es_setparam_adress   = 0x26, //S_SCSetParam_Adress  服务器向终端发送==>>C_CSOrderConfirm
	es_get_param         = 0x27, //S_SCGetPara 成功用C_CSGetPara,失败用C_CSOrderConfirm回复
	es_setoil_elec       = 0x28, //S_SCSetOil_Elec      服务向终端发送,需C_CSorderConfirm回复
	es_listen            = 0x29, //S_SCListen           服务向终端发送,需回复C_CSOrderConfirm回复
	es_get_photo         = 0x2A, //S_SCPhoto            需回复C_CSOrderConfirm回复
	es_send_msg          = 0x2B, //S_SCSendMsg          服务向终端发送,C_CSOrderConfirm
	es_file_rcv          = 0x2E, //S_SCFileRcv          服务器接收到终端上传的文件快后向终端回应
	es_trans_file        = 0x2F, //S_SCTransFile
	es_pos_request		 = 0x30, //S_SCPosRequest		服务器请求终端位置信息 //@z
};

//服务器查询机器的参数
//1：时间参数；2：网络参数；3：报警参数；4：软件版本
enum svr_param_type
{ svr_type_time  = 1,   //时间参数
  svr_type_net   = 2,   //网络参数
  svr_type_alarm = 3,   //报警参数
  svr_type_soft  = 4,   //软件版本
  svr_type_error
};



//FHT的命令消息定义
enum fth_svr_cmd{

	fth_svr_modify_gps_slice = 0x01,  //修改监控频率
	fth_svr_view_gps_slice   = 0x02,  //查看GPS监控频率
	fth_svr_report_pos_one   = 0x03,  //立即查看当前的位置信息
	fth_svr_searching_device = 0x04,  //搜索区域内的机器
	fth_svr_set_overspeed    = 0x05,  //设置超速速度
	fth_svr_view_overspeed   = 0x06,  //查看超速速度
	fth_svr_cancel_overspeed = 0x07,  //取消超速速度
	fth_svr_tip_overspeed    = 0xFF,  //超速主动提示

	fth_svr_set_listen_phone     = 0x18,   //设置监听号码
	fth_svr_view_listen_phone    = 0x19,   //查看监听号码
	fth_svr_cancel_listen_phone  = 0x1A,   //取消监听号码
	fth_svr_modify_ip            = 0x1B,   //设置新的IP地址
	fth_svr_view_ip              = 0x1C,   //查看当前IP地址
	fth_svr_modify_udp_port      = 0x1D,   //修改UDP的端口号
	fth_svr_view_udp_port        = 0x1E,   //查看UDP的端口号
	fth_svr_set_heart            = 0x21,   //设置心跳时间
	fth_svr_view_heart           = 0x22,   //查看心跳时间
	fth_svr_set_apn              = 0x23,   //设置APN名称
	fth_svr_view_apn             = 0x24,   //查看APN名称
	fth_svr_set_ppp_name         = 0x25,   //设置PPP用户名
	fth_svr_view_ppp_name        = 0x26,   //查看PPP用户名
	fth_svr_set_ppp_pwd          = 0x27,   //设置PPP密码
	fth_svr_view_ppp_pwd         = 0x28,   //查看PPP密码
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
	unsigned int    order_id;		//用来做发送ACK的时候用
	unsigned char   ordermsg_id;	//OrderMessageID
	unsigned int	result;			//0:执行失败; 1:执行成功
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
		unsigned short user_alarm;      //..21	Time1	WORD	2	手动报警时的发送间隔，单位秒，范围1~0xfffe
		unsigned short vehicle_empty;   //time..13	Time2	WORD	2	空车时的发送间隔，单位秒，范围1~0xfffe
		unsigned short vehicle_full;    //..25	Time3	WORD	2	重车时的发送间隔，单位秒，范围1~0xfffe
		unsigned short acc_off;         //..27	Time4	WORD	2	ACC关闭时发送间隔，单位秒，范围1~0xfffe
		unsigned char  over_speed;      //Time5	BYTE	1	设置超时行驶时间，单位小时，范围1-254
		unsigned char  over_time;       //Time6	BYTE	1	设置超时停车时间，单位小时，范围1-254
		unsigned char  acc_off_standby; //Time7	BYTE	1	设置ACC关闭多少时间后休眠，单位分钟，范围1-254
		unsigned char  heart_beat;      //Time8	BYTE	1	设置心跳包的发送间隔，单位秒，范围1-254
}sttime_param;



typedef struct{
	unsigned int   order_id;   //用来做发送ACK的时候使用
	char*          msg_ptr;    //消息内容
	unsigned int   msg_size;   //消息大小
}stsvr_msg;

typedef struct{
	unsigned int   order_id;   //用来做发送ACK的时候用
	unsigned short speed_ltd;     //超速速度
	unsigned short speed_time;    //超速时间
}stspeed_alert;


typedef struct{
	unsigned int    order_id;  //用来做发送ACK的时候用
	unsigned char   ch_type;   //1：监听；2：通话
	char            ch_phone[19];
}stlisten_phone;

typedef struct{
	unsigned int    order_id;  //用来做发送ACK的时候用
	unsigned char   ch_type;   //监控类型：1 点名呼叫；2 定次呼叫；3 定时呼叫
	DWORD			nCount;//定位报文回传次数（只对"定次呼叫"有效）
	DWORD			nTimeInterval;//定位报文回传间隔时间，如果时间为0则停止；（只对"定次呼叫"和"定时呼叫"有效）
}stpos_request;

typedef struct{
	unsigned int    order_id;  //用来做发送ACK的时候用
	unsigned char   ch_type;   //1：断开；2：连接 
}stsetoil_elec;

typedef struct{
	char   user_name[32];
	char   user_num[32];
}stsvr_phone;

typedef  struct{
	unsigned char   action_type;  //1：开始；2：停止
	unsigned        inter_time;   //时间间隔，单位：秒
	unsigned        photo_num;    //拍照数量
}stphoto_capture;

//////////////////////////////////////////////////////////////////////////

typedef struct{
	unsigned    gps_slice;          //GPS时间上传的频率
	unsigned    is_over_speed;      //是否处在超速状态
	unsigned    gps_over_speed;     //超速阀值
	unsigned    svr_heart;          //心跳时间
	char        user_name[16];      //用户名
	unsigned    listen_idx;
	char        listen_phone1[16];  //监听用户名1
	char        listen_phone2[16];  //监听用户名2
	float       Lat1;
	float       Long1;
	float       Lat2;
	float       Long2;
	char        svr_ip[64];         //中心IP地址
	unsigned    svr_port;           //端口号
	char        apn_name[16];        //APN名称
	char        ppp_name[16];        //PPP名称
	char        ppp_pwd[16];        //PPP密码
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