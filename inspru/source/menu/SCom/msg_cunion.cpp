
#include "StdAfx.h"
#include "msg_cunion.h"
#include "../ExTool/ex_debug.h"




char msg_cunion::g_device_id[15];
unsigned  char   g_getway_id[9];
unsigned  char   g_svr_order_id[4];
unsigned  char   g_certification_order_id[4];	//@z  store the orderid of message 0x11 from 0x21
unsigned  msg_cunion::g_uuid_count = 0x01020304;
unsigned  msg_cunion::g_certification_ID;


msg_cunion::msg_cunion(char* sz_buf, int n_size) : msg_mid(sz_buf, n_size)
{

}

void msg_cunion::set_device_id(const char* p_device_id)
{
	memset( g_getway_id, 0, sizeof(g_getway_id) );
	memset( g_device_id, 0, sizeof(g_device_id) );
	int n_len = strlen(p_device_id);
	if ( n_len > sizeof(g_device_id) ) n_len = sizeof(g_device_id);
	memcpy(g_device_id, p_device_id, n_len );
}



void   msg_cunion::pack_download_file_req()
{
	//0	MessageID	BYTE	1	0xAF
	//1	FileType	BYTE	1	0x01：导航文件
	//2..5	OrderID	DWORD	4	命令序列号
	//6..20	TerminalID	CHAR	15	终端ID
	char sz_buf[128];
	int  n_offset = 0;
	memset(sz_buf, 0, sizeof(sz_buf));

	sz_buf[n_offset] = 0x01; 
	n_offset += 1;

	sz_buf[n_offset] = (g_uuid_count ) & 0xFF ;
	sz_buf[n_offset+1] = (g_uuid_count >> 8 ) & 0xFF ;
	sz_buf[n_offset+2] = (g_uuid_count >> 16 ) & 0xFF ;
	sz_buf[n_offset+3] = (g_uuid_count >> 24 ) & 0xFF ;
	g_uuid_count++;
	n_offset += 4;

	
	//GetWayID为0
	memcpy( &sz_buf[n_offset], g_getway_id, sizeof(g_getway_id) );
	n_offset += sizeof(g_getway_id);

	//Device ID
	memcpy( &sz_buf[n_offset], g_device_id, sizeof(g_device_id) );
	n_offset += sizeof(g_device_id);

	build_payload(ec_file_req,sz_buf,n_offset );
	build_msg();
}

//@z
void msg_cunion::pack_certification(unsigned char chDeviceID)
{
	//0			MessageID		BYTE	1	0x11
	//1..4		OrderID			DWORD	4	
	//5..19		DeviceID		CHAR	15	报警器设备ID
	//20..28	GatewayID		CHAR	9	网关ID
	//29..43	TerminalID		CHAR	15	终端ID
	char sz_buf[128];
	int  offset = 0;
	memset(sz_buf, 0, sizeof(sz_buf) );

	//OrderID
	sz_buf[offset] = g_uuid_count & 0xFF;
	sz_buf[offset+1] = (g_uuid_count >> 8 ) & 0xFF;
	sz_buf[offset+2] = (g_uuid_count >> 16 ) & 0xFF;
	sz_buf[offset+3] = (g_uuid_count >> 24 ) & 0xFF;
	g_certification_ID = g_uuid_count;
	offset += 4;
	g_uuid_count++;

	//DeviceID 报警器设备ID
	memcpy( &sz_buf[offset], &chDeviceID, 1 );
	offset += 15;

	//GetWayID为0
	memcpy( &sz_buf[offset], g_getway_id, sizeof(g_getway_id) );
	offset += sizeof(g_getway_id);

	//TerminalID
	memcpy( &sz_buf[offset], g_device_id, sizeof(g_device_id) );
	offset += sizeof(g_device_id);

	build_payload(ec_certification,sz_buf,offset);
	build_msg();
}

//@z
void msg_cunion::pack_vehicle_status(unsigned char ucStatus)
{
	//0			MessageID		BYTE	1	0x11
	//1..4		OrderID			DWORD	4	
	//5..19		DeviceID		CHAR	15	报警器设备ID
	//20..28	GatewayID		CHAR	9	网关ID
	//29..43	TerminalID		CHAR	15	终端ID
	char sz_buf[128];
	int  offset = 0;
	memset(sz_buf, 0, sizeof(sz_buf) );

	//OrderID
	sz_buf[offset] = g_uuid_count & 0xFF;
	sz_buf[offset+1] = (g_uuid_count >> 8 ) & 0xFF;
	sz_buf[offset+2] = (g_uuid_count >> 16 ) & 0xFF;
	sz_buf[offset+3] = (g_uuid_count >> 24 ) & 0xFF;
	g_certification_ID = g_uuid_count;
	offset += 4;
	g_uuid_count++;

	//GetWayID为0
	memcpy( &sz_buf[offset], g_getway_id, sizeof(g_getway_id) );
	offset += sizeof(g_getway_id);

	//TerminalID
	memcpy( &sz_buf[offset], g_device_id, sizeof(g_device_id) );
	offset += sizeof(g_device_id);

	//DeviceID 报警器设备ID
	memcpy( &sz_buf[offset], &ucStatus, 1 );

	build_payload(ec_vehicle_status,sz_buf,offset);
	build_msg();
}

//////////////////////////////////////////////////////////////////////////

void   msg_cunion::set_ack_msg(const char* p_msg, int n_len)
{
	m_data = (unsigned char*)p_msg;
	m_msg_len = n_len;
	m_max_len = n_len;

	/*
	memset(m_buf, 0, sizeof(m_buf) );
	if ( n_len > sizeof(m_buf) )
		n_len = sizeof(m_buf);
	memcpy( m_buf, p_msg, n_len );

	m_msg_len = n_len;
	*/
}

//////////////////////////////////////////////////////////////////////////

int   msg_cunion::check_ack_flag()
{
	if ( m_data == 0 || m_max_len == 0 ) 
		return 0;
	unsigned char* p_ack = (unsigned char*)m_data;

	// (1) check head information
	if ( p_ack[OFFSET_HEAD] != 0xFF )
	{
		EXDBG_TRACE1("check_ack_flag:: head value is : %d!\n", p_ack[OFFSET_HEAD]);
		return 0;
	}

	//(2) check device type
	if ( p_ack[OFFSET_DEVICE_TYPE] != eDev_GPS )
	{
		EXDBG_TRACE1("check_ack_flag:: device type error : %d!\n", p_ack[OFFSET_DEVICE_TYPE]);
		return 0;
	}

	// (3) check message length
	int n_len = m_data[OFFSET_MSGLEN] << 8;
	n_len |= m_data[OFFSET_MSGLEN+1];
	if ( m_msg_len < n_len + 4 )
	{
		EXDBG_TRACE2("check_ack_flag:: msg length is error!%d_%d!\n", n_len,m_msg_len );
		return 0;
	}

	// (4) check tail flag
	if ( p_ack[n_len + 4 -1] != 0xFF )
	{
		EXDBG_TRACE1("check_ack_flag:: tail value is : %d!\n",  p_ack[m_msg_len-1] );
		return 0;
	}

	m_msg_len = n_len + 4;
	return m_msg_len;
}

bool   msg_cunion::check_ack_result()
{
	if ( m_data == 0 || m_max_len == 0 ) 
		return false;
	unsigned char* p_str = &m_data[OFFSET_PAYLOAD];
	//OrderID
	p_str += 4;

	//OrderMessageID
	p_str += 1;

	// GetWayID
	if ( memcmp(p_str, g_getway_id, sizeof(g_getway_id) ) != 0 )
	{
		EXDBG_TRACE2("ack error : %s___%d is not same getway!\n",p_str, g_getway_id );
		//return false;
	}
	p_str += sizeof(g_getway_id);

	// Device ID
	if ( memcmp(p_str, g_device_id, sizeof(g_device_id) ) != 0 )
	{
		EXDBG_TRACE2("ack error : %s___%d is not same device!\n",p_str, g_device_id );
		return false;
	}
	p_str += sizeof(g_device_id);
	
	if ( p_str[0] == 1 )
	{
		return true;
	}
	else
	{
		EXDBG_TRACE1("ack error : %d is failed\n",p_str[0]  );
		return false;
	}
	
}

bool   msg_cunion::check_ack_type(int n_type)
{
	if ( (unsigned char)m_data[OFFSET_MSGID] == n_type )
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////

bool  msg_cunion::check_ack_file_req()
{
	if ( m_data == 0 ) return false;
	unsigned char* p_str = &m_data[OFFSET_PAYLOAD];
	int nValu= 0;
	nValu = (p_str[0] ) | (p_str[1] << 8 ) | (p_str[2] << 16) | (p_str[3] << 24);
	if ( nValu != g_uuid_count )
	{
		EXDBG_TRACE2("ack error : %d-%d is not same uuid!\n",nValu, g_uuid_count );
		return false;
	}
	p_str += 4;

	if ( (unsigned char)p_str[0] != ec_file_req)
	{
		EXDBG_TRACE1("ack error : %s is not client cmd!\n",p_str[0] );
		return false;
	}
	p_str += 1;

	// GetWayID
	if ( memcmp(p_str, g_getway_id, sizeof(g_getway_id) ) != 0 )
	{
		EXDBG_TRACE2("ack error : %s___%d is not same getway!\n",p_str, g_getway_id );
		//return false;
	}
	p_str += sizeof(g_getway_id);



	if ( memcmp(p_str, g_device_id, sizeof(g_device_id) ) != 0 )
	{
		EXDBG_TRACE2("ack error : %s___%d is not same device!\n",p_str, g_device_id );
		return false;
	}
	p_str += sizeof(g_device_id);

	if ( p_str[0] == 1 )
	{
		return true;
	}
	else
	{
		EXDBG_TRACE1("ack error : %d is failed\n",p_str[0]  );
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
bool msg_cunion::check_ack_order_confirm(storder_confirm* p_order_confirm)
{
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// server message order id
	memcpy(g_certification_order_id, p_str, 4 );
	p_order_confirm->order_id = p_str[offset];
	p_order_confirm->order_id |= p_str[offset+1] << 8;
	p_order_confirm->order_id |= p_str[offset+2] << 16;
	p_order_confirm->order_id |= p_str[offset+3] << 24;
	offset += 4;
	
	//OrderMessageID
	p_order_confirm->ordermsg_id = p_str[offset];
	offset++;

	// getway id
	if ( memcmp(&p_str[offset], g_getway_id, sizeof(g_getway_id) ) != 0 )
	{
		EXDBG_TRACE2("check_ack_order_confirm: %s___%d is not same getway!\n",p_str, g_getway_id );
		//return false;
	}
	offset += 9;
	
	// terminal id
	if ( memcmp(&p_str[offset], g_device_id, sizeof(g_device_id) ) != 0 )
	{
		//EXDBG_TRACE2("check_ack_order_confirm: %s___%d is not same device!\n",p_str, g_device_id );
		return false;
	}
	offset += 15;

	//result
	p_order_confirm->result = p_str[offset];
	return true;
}

bool   msg_cunion::check_ack_alert_release()
{
	//0	MessageID	BYTE	1	0x22
	//1..4	OrderID	DWORD	4	命令序列号
	//5..13	GatewayID	CHAR	9	网关ID
	//14..28	TerminalID	CHAR  15	终端ID
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	int nOrderID = 0;
	nOrderID = p_str[offset];
	nOrderID |= p_str[offset+1] << 8;
	nOrderID |= p_str[offset+2] << 16;
	nOrderID |= p_str[offset+3] << 24;
	offset += 4;
	
	// GetWay id
	offset += 9;
	// terminal id
	offset += 15;


	
	return true;
}


bool   msg_cunion::check_ack_file_trans(stfile_page* p_page)
{
	//0	MessageID	BYTE	1	0xBF
	//1	FileType	BYTE	1	0x01：导航文件
	//2..5	OrderID	DWORD	4	命令序列号 (同一文件序列号相同)
	//6..20	TerminalID	CHAR	15	终端ID
	//                         4 文件总长度
	//21..24	TotalCount	DWORD	4	文件总包数
	//25..29	CurrentCount	DWORD	4	当前包序号
	//30	文件数据	BYTE	n	文件数据
	if ( p_page == 0 )
	{
		EXDBG_TRACE0("check_ack_file_trans: stfile_page is NULL!\n"  );
		return false;
	}
	int n_val = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	if ( p_str[0] != 0x01 )
	{
		EXDBG_TRACE1("check_ack_file_trans is not 0x01__%d\n",p_str[0]  );
		return false;
	}

	p_str += 1;
	n_val = (p_str[0] ) | (p_str[1] << 8) | (p_str[2] << 16) | (p_str[3] << 24);
	if ( n_val != g_uuid_count )
	{
		EXDBG_TRACE2("check_ack_file_trans : %d-%d is not same uuid!\n",n_val, g_uuid_count );
		return false;
	}
	p_str += 4;

	// GetWayID
#if 0
	if ( memcmp(p_str, g_getway_id, sizeof(g_getway_id) ) != 0 )
	{
		EXDBG_TRACE2("ack error : %s___%d is not same getway!\n",p_str, g_getway_id );
		//return false;
	}
#endif
	p_str += sizeof(g_getway_id);


	if ( memcmp(p_str, g_device_id, sizeof(g_device_id) ) != 0 )
	{
		EXDBG_TRACE2("check_ack_file_trans : %s___%d is not same device!\n",p_str, g_device_id );
		return false;
	}

	p_str += sizeof(g_device_id);

	//文件总长度
	p_page->m_total_size = (p_str[0]<< 24) | (p_str[1] << 16) | (p_str[2] << 8) | (p_str[3]);
	p_str += 4;

	//TotalCount	DWORD	4	文件总包数
	p_page->m_num = (p_str[0]<< 24) | (p_str[1] << 16) | (p_str[2] << 8) | (p_str[3]);
	p_str += 4;
	//CurrentCount	DWORD	4	当前包序号
	p_page->m_cur_idx = (p_str[0]<< 24) | (p_str[1] << 16) | (p_str[2] << 8) | (p_str[3]);
	p_str += 4;

	int nValue = p_str - &m_data[OFFSET_PAYLOAD] + 2;
	p_page->m_data = (char*)p_str;
	p_page->m_length = ( m_msg_len - 4 ) - nValue;


	return true;
}

//////////////////////////////////////////////////////////////////////////
bool   msg_cunion::check_ack_set_time(sttime_param* p_time)
{
	int n_val = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	

	// save order ID
	memcpy( g_svr_order_id, p_str, 4 );
	p_str += 4;

	if ( memcmp(p_str, g_getway_id, sizeof(g_getway_id) ) != 0 )
	{
		EXDBG_TRACE2("check_ack_set_time: %s___%d is not same getway!\n",p_str, g_getway_id );
		//return false;
	}
	p_str += sizeof(g_getway_id);
	
	if ( memcmp(p_str, g_device_id, sizeof(g_device_id) ) != 0 )
	{
		EXDBG_TRACE2("check_ack_set_time: %s___%d is not same device!\n",p_str, g_device_id );
		return false;
	}
	
	p_str += sizeof(g_device_id);

	//..21	Time1	WORD	2	手动报警时的发送间隔，单位秒，范围1~0xfffe
	p_time->user_alarm = (p_str[0] << 8) + p_str[1];
	p_str += 2;
   
	//time..13	Time2	WORD	2空车时的发送间隔，单位秒，范围1~0xfffe
	p_time->vehicle_empty= (p_str[0] << 8) + p_str[1];
	p_str += 2;

	//..25	Time3	WORD	2	重车时的发送间隔，单位秒，范围1~0xfffe
	p_time->vehicle_full= (p_str[0] << 8) + p_str[1];
	p_str += 2;

     //..27	Time4	WORD	2	ACC关闭时发送间隔，单位秒，范围1~0xfffe
	p_time->acc_off= (p_str[0] << 8) + p_str[1];
	p_str += 2;

	//Time5	BYTE	1	设置超时行驶时间，单位小时，范围1-254
	p_time->over_speed= p_str[0];
	p_str++;

	//Time6	BYTE	1	设置超时停车时间，单位小时，范围1-254
	p_time->over_time= p_str[0];
	p_str++;

	//Time7	BYTE	1	设置ACC关闭多少时间后休眠，单位分钟，范围1-254
	p_time->acc_off_standby= p_str[0];
	p_str++;

	//Time8	BYTE	1	设置心跳包的发送间隔，单位秒，范围1-254
	p_time->heart_beat= p_str[0];

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool   msg_cunion::check_ack_set_net(stnet_param* p_net)
{
	int n_val = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// save order ID
	memcpy( g_svr_order_id, p_str, 4 );
	p_str += 4;

	if ( memcmp(p_str, g_getway_id, sizeof(g_getway_id) ) != 0 )
	{
		EXDBG_TRACE2("check_ack_set_net: %s___%d is not same getway!\n",p_str, g_getway_id );
		//return false;
	}
	p_str += sizeof(g_getway_id);

	
	if ( memcmp(p_str, g_device_id, sizeof(g_device_id) ) != 0 )
	{
		EXDBG_TRACE2("check_ack_set_net: %s___%d is not same device!\n",p_str, g_device_id );
		return false;
	}
	
	p_str += sizeof(g_device_id);
	
	//20..23	ServerIP	BYTE	4	服务中心IP地址，例：218.108.39.38
	memcpy(p_net->ip_svr, p_str, 4 );
	p_str += 4;

	//24..25	ServerPort	WORD	2	服务中心侦听端口
	p_net->svr_port = ( p_str[0] << 8 ) + p_str[1]; 
	p_str += 2;

	//26..29	GateIP	BYTE	4	网关地址
	memcpy( p_net->ip_gate, p_str, 4 );
	p_str += 4;

	//30..33	DNSServer	BYTE	4	域名解析服务器
	memcpy( p_net->ip_dns, p_str, 4 );
	p_str += 4;

	//34..65	Address	CHAR	32	域名地址
	memcpy( p_net->dns_address, p_str, 32 );
	p_str += 32;

	//66..97	APNName	CHAR	32	APN名称
	memcpy( p_net->apn_name, p_str, 32 );
	p_str += 32;

	//98..129	APNUser	CHAR	32	APN用户名
	memcpy( p_net->apn_user, p_str, 32 );
	p_str += 32;

	//130..161	APNPwd	CHAR	32	APN密码
	memcpy( p_net->apn_pwd, p_str, 32 );
	p_str += 32;

	return true;

}

bool   msg_cunion::check_ack_svr_msg(stsvr_msg*  p_msg)
{
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);

	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	p_msg->order_id = p_str[offset];
	p_msg->order_id |= p_str[offset+1] << 8;
	p_msg->order_id |= p_str[offset+2] << 16;
	p_msg->order_id |= p_str[offset+3] << 24;
	offset += 4;

	// getway id
	offset += 9;
	// terminal id
	offset += 15;

	int msg_size =  m_msg_len - 4 - 2 - offset;
	if ( p_msg->msg_ptr != NULL )
		delete [] p_msg->msg_ptr;

	p_msg->msg_ptr = new char[msg_size+2];
	memset( p_msg->msg_ptr, 0, msg_size + 2 );
	memcpy( p_msg->msg_ptr, &p_str[offset], msg_size );
	return true;
}

bool   msg_cunion::check_ack_set_alert(stspeed_alert* p_speed)
{
/*
		1..4	OrderID	DWORD	4	命令序列号
		5..13	GatewayID	CHAR	9	网关ID
		14..28	TerminalID	CHAR	15	终端ID
		29..30	SpeedLimit	WORD	2	超速速度
		31..32	Time	WORD	2	超速时间，单位：分钟
*/
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	p_speed->order_id = p_str[offset];
	p_speed->order_id |= p_str[offset+1] << 8;
	p_speed->order_id |= p_str[offset+2] << 16;
	p_speed->order_id |= p_str[offset+3] << 24;
	offset += 4;
	
	// getway id
	offset += 9;
	// terminal id
	offset += 15;

	p_speed->speed_ltd  = p_str[offset] << 8;
	p_speed->speed_ltd |= p_str[offset+1];
	offset += 2;

	p_speed->speed_time  = p_str[offset] << 8;
	p_speed->speed_time |= p_str[offset+1];

	return true;
}


bool   msg_cunion::check_ack_phone_book( stsvr_phone* p_pbook, unsigned& n_num, bool& b_add )
{
/*
	0	MessageID	BYTE	1	0x26
	1..4	OrderID	DWORD	4	命令序列号
	5..13	GatewayID	CHAR	9	网关ID
	14..28	TerminalID	CHAR	15	终端ID
	29	OperType	BYTE	1	1：增加；2：清除
	30..31	AddressBookCount	WORD	2	电话本条数
	32..	Phone	CHAR	n	形如："人姓名1，人电话1；人姓名2，人电话2"
*/
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	int n_order_id = 0;
	n_order_id = p_str[offset];
	n_order_id |= p_str[offset+1] << 8;
	n_order_id |= p_str[offset+2] << 16;
	n_order_id |= p_str[offset+3] << 24;
	offset += 4;
	
	// GetWay id
	offset += 9;
	// terminal id
	offset += 15;

	// OrderType
	if ( p_str[offset] == 1 )
		b_add = true;
	else if ( p_str[offset] == 2 )
		b_add = false;
	else
	{
		EXDBG_TRACE1("check_ack_phone_book has error: order type:%d!", p_str[offset] );
		return false;
	}
	offset++;

	//phone book count
	n_num = p_str[offset] + (p_str[offset+1] << 8);
	if ( n_num > 256 )
	{
		EXDBG_TRACE1("check_ack_phone_book has error: phone num:%d!", n_num );
		return false;
	}
	offset += 2;

	char* p_begin = (char*)&p_str[offset];
	char* p_end;
	int  nCount = 0, nChSize;
	while( p_begin )
	{
		if( nCount >= n_num )
			break;
		p_end = strstr(p_begin,",");
		if ( p_end == 0 ) 
			return false;
		
		//用户名的个数
		nChSize = p_end - p_begin;
		if ( nChSize >= 32 ) nChSize = 31;
		memcpy( p_pbook[nCount].user_name, p_begin, nChSize );

		//用户电话号码
		p_end++; //跳过","
		p_begin = p_end;
		p_end = strstr( p_begin,";" );
		if ( p_end == 0 )
			return false;
		
		nChSize = p_end - p_begin;
		if ( nChSize >= 32 ) nChSize = 31;
		memcpy( p_pbook[nCount].user_num, p_begin, nChSize );

		p_end++; //跳过";"
		p_begin = p_end;
	}
	return true;
}


bool   msg_cunion::check_ack_query_param( svr_param_type& n_type )
{
/* 	0	消息标示	BYTE	1	0x27
	1..4	OrderID	DWORD	4	命令序列号
	5..13	GatewayID	CHAR	9	网关ID
	14..28	TerminalID	CHAR	15	终端ID
	29	参数类型	BYTE	1	1：时间参数；2：网络参数；3：报警参数；4：软件版本
	
*/
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	int order_id = p_str[offset];
	order_id |= p_str[offset+1] << 8;
	order_id |= p_str[offset+2] << 16;
	order_id |= p_str[offset+3] << 24;
	offset += 4;
	
	// getway id
	offset += 9;
	// terminal id
	offset += 15;
	// listen type
	n_type = (svr_param_type)p_str[offset];
	return true;
}

/* 	0  MessageID  BYTE         1   0x29 
	1..4  OrderID  DWORD       4   命令序列号 
	5..13  GatewayID  CHAR     9   网关 ID 
	14..28  TerminalID  CHAR  15   终端 ID 
	29  ListenType  BYTE       1   1：监听；2：通话 
	30..  Phone  CHAR         19   电话号码 	
*/
bool   msg_cunion::check_ack_svr_listen(stlisten_phone* p_listen)
{
	int offset = 0;
	unsigned char* p_str =(&m_data[OFFSET_PAYLOAD]);
	
	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	p_listen->order_id = p_str[offset];
	p_listen->order_id |= p_str[offset+1] << 8;
	p_listen->order_id |= p_str[offset+2] << 16;
	p_listen->order_id |= p_str[offset+3] << 24;
	offset += 4;
	
	// getway id
	offset += 9;
	// terminal id
	offset += 15;

	// listen type
	p_listen->ch_type  = p_str[offset];
	offset += 1;
	
	//30..  Phone  CHAR  19  电话号码 
	memcpy( p_listen->ch_phone, &p_str[offset], 19 );
	return true;
}

/*
0	MessageID	BYTE	1	0x2A
1..4	OrderID	DWORD	4	命令序列号
5..13	GatewayID	CHAR	9	网关ID
14..28	TerminalID	CHAR	15	终端ID
29	OperType	BYTE	1	1：开始；2：停止 
30..33	TimeInterval	DWORD	4	时间间隔，单位：秒
34..35	Count	WORD	2	拍照数量 
*/
bool   msg_cunion::check_ack_capture_photo( stphoto_capture* p_capture )
{
	int offset = 0;
	unsigned char* p_str =( &m_data[OFFSET_PAYLOAD] );
	
	// server message order id
	memcpy(g_svr_order_id, p_str, 4 );
	int order_id = p_str[offset];
	order_id |= p_str[offset+1] << 8;
	order_id |= p_str[offset+2] << 16;
	order_id |= p_str[offset+3] << 24;
	offset += 4;

	// Getway id
	offset += 9;
	// Terminal id
	offset += 15;


	//开始 or 停止
	if ( p_str[offset] == 1 || p_str[offset] == 2 )
		p_capture->action_type = p_str[offset];
	else
	{
		EXDBG_TRACE1("check_ack_capture_photo::%d\n",p_str[offset] );
		return false;
	}
	offset += 1;
	//时间间隔
	p_capture->inter_time = p_str[offset] | p_str[offset+1] << 8 | p_str[offset+2] << 16 | p_str[offset+3] << 24;
	offset += 4;

	p_capture->photo_num = p_str[offset] | p_str[offset+1] << 8;
	offset += 2;

	return true;
}



