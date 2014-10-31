/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\logic\logic_data.h

 $Reversion History:

        (1) Created Liuhl@2008-03-18 15:50

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  LOGIC_DATA_H
#define  LOGIC_DATA_H

typedef enum
{
	SMS,
	DISPATCH,
};

typedef struct
{
	unsigned  m_type;          //0 : delte ; 1:read, 2 : unread,3:SIM READ, 4SIM:UNREAD
	char      m_time[16];      //080319121015 2008-03-19 12:10:15
	char      m_number[16];    //电话号码8613818763854
	char      m_context[160];  // max number is 160
}st_sms;

//typedef struct
//{
//	BYTE		ucSMSType;          //A  B  C  D  E
//	BYTE		ucVoiceType;		//0: 无语音   1:有语音
//	BYTE		ucTime[16];			//080319121015 2008-03-19 12:10:15
//	char		ucContent[160];		//140
// }st_dispatch;

typedef struct {
	char SCA[16];       // 短消息服务中心号码(SMSC地址)
	char TPA[16];       // 目标号码或回复号码(TP-DA或TP-RA)
	char TP_PID;        // 用户信息协议标识(TP-PID)
	char TP_DCS;        // 用户信息编码方式(TP-DCS)
	char TP_SCTS[16];   // 服务时间戳字符串(TP_SCTS), 接收时用到
	char TP_UD[161];    // 原始用户信息(编码前或解码后的TP-UD)
	char index;         // 短消息序号，在读取时用到
} SM_PARAM;

typedef struct
{
	short   nIdx;
	short   state;    // 0,无记录，1，有记录
	char    phone[64];
	char    name[64];
}PBOOK_ITEM;

class logic_data
{
public:
	logic_data();
	~logic_data();

	enum { reserve_number = 100 };

public:
	bool   load_data( const char* p_path );	  //从文件中读入数据到结构体
	bool   save_data( const char* p_path );	  //
	bool   delte_record( int n_idx );		  //删除记录，置m_type为0
	bool   add_recourd(st_sms* p_msg );		  //文件最后增加一结构体

	//删除所有的记录
	void   remove_all() { m_count = 0; m_is_change = true; };

	int       max_number();		              //返回短信的条数m_count
	st_sms*   get_recourd( int n_idx );		  //按索引，返回结构体的指针
	bool      read_sms(int n_idx);			  //读索引号的消息，置m_type:1
	bool      is_open() { return m_is_open; }
	void      user_change(bool b_change) { m_is_change = b_change; }

	static  bool  add_2file(const char* p_path, st_sms* p_msg);   //添加用例到文件
	
protected:
	bool        m_is_open;
	bool        m_is_change;
	st_sms*		m_list;
	int			m_count;
	int         m_max;
};



class logic_pbook
{
public:
	logic_pbook();
	~logic_pbook();
	
	enum { reserve_number = 100 };
	
public:
	bool   load_data( const char* p_path );	  //从文件中读入数据到结构体
	bool   save_data( const char* p_path );	  //
	bool   delte_record( int n_idx );		  //删除记录，置m_type为0
	bool   add_recourd(PBOOK_ITEM* p_msg );		  //文件最后增加一结构体

	PBOOK_ITEM* sch_item(const char* p_phone);
	
	//删除所有的记录
	void   remove_all() { m_count = 0; m_is_change = true; };
	
	int           max_number();
	PBOOK_ITEM*   get_recourd( int n_idx );		      //按索引，返回结构体的指针
	bool          is_open() { return m_is_open; }
	void          user_change(bool b_change) { m_is_change = b_change; }
	static  bool  add_2file(const char* p_path, PBOOK_ITEM* p_msg);   //添加用例到文件
	
protected:
	bool            m_is_open;
	bool            m_is_change;
	PBOOK_ITEM*		m_list;
	int		  	    m_count;
	int             m_max;
};

#endif //LOGIC_DATA_H