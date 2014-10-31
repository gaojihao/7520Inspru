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
	char      m_number[16];    //�绰����8613818763854
	char      m_context[160];  // max number is 160
}st_sms;

//typedef struct
//{
//	BYTE		ucSMSType;          //A  B  C  D  E
//	BYTE		ucVoiceType;		//0: ������   1:������
//	BYTE		ucTime[16];			//080319121015 2008-03-19 12:10:15
//	char		ucContent[160];		//140
// }st_dispatch;

typedef struct {
	char SCA[16];       // ����Ϣ�������ĺ���(SMSC��ַ)
	char TPA[16];       // Ŀ������ظ�����(TP-DA��TP-RA)
	char TP_PID;        // �û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];   // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	char index;         // ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

typedef struct
{
	short   nIdx;
	short   state;    // 0,�޼�¼��1���м�¼
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
	bool   load_data( const char* p_path );	  //���ļ��ж������ݵ��ṹ��
	bool   save_data( const char* p_path );	  //
	bool   delte_record( int n_idx );		  //ɾ����¼����m_typeΪ0
	bool   add_recourd(st_sms* p_msg );		  //�ļ��������һ�ṹ��

	//ɾ�����еļ�¼
	void   remove_all() { m_count = 0; m_is_change = true; };

	int       max_number();		              //���ض��ŵ�����m_count
	st_sms*   get_recourd( int n_idx );		  //�����������ؽṹ���ָ��
	bool      read_sms(int n_idx);			  //�������ŵ���Ϣ����m_type:1
	bool      is_open() { return m_is_open; }
	void      user_change(bool b_change) { m_is_change = b_change; }

	static  bool  add_2file(const char* p_path, st_sms* p_msg);   //����������ļ�
	
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
	bool   load_data( const char* p_path );	  //���ļ��ж������ݵ��ṹ��
	bool   save_data( const char* p_path );	  //
	bool   delte_record( int n_idx );		  //ɾ����¼����m_typeΪ0
	bool   add_recourd(PBOOK_ITEM* p_msg );		  //�ļ��������һ�ṹ��

	PBOOK_ITEM* sch_item(const char* p_phone);
	
	//ɾ�����еļ�¼
	void   remove_all() { m_count = 0; m_is_change = true; };
	
	int           max_number();
	PBOOK_ITEM*   get_recourd( int n_idx );		      //�����������ؽṹ���ָ��
	bool          is_open() { return m_is_open; }
	void          user_change(bool b_change) { m_is_change = b_change; }
	static  bool  add_2file(const char* p_path, PBOOK_ITEM* p_msg);   //����������ļ�
	
protected:
	bool            m_is_open;
	bool            m_is_change;
	PBOOK_ITEM*		m_list;
	int		  	    m_count;
	int             m_max;
};

#endif //LOGIC_DATA_H