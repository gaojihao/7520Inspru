/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gsm\gsm_api.h

 $Reversion History:

        (1) Created Liuhl@2008-03-05 05:30 PM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  GSM_API_H
#define  GSM_API_H

#include "ex_basics.h"
#include "gsm_ctrl.h"
//////////////////////////////////////////////////////////////////////////

// GSM initialize function 
bool      gsm_api_start(unsigned n_com, unsigned n_baud);
void      gsm_api_close();
bool      gsm_api_config(bool b_check_gsn);
bool      gsm_api_config2();
char*     gsm_api_imei();

bool      gsm_api_check_sim();
bool      gsm_api_register_gsm();
bool      gsm_api_register_gprs();
//int       gsm_api_query_net_isp();
int       gsm_api_query_signal();


void      gsm_api_signal();
void      gsm_api_scmd(const char* p_cmd, int n_len );
void      gsm_api_set_vol( int n_value ); // 0~100
int       gsm_api_call_start(const char* p_number );
void      gsm_api_call_ext(const char* sz_ext);
void      gsm_api_handup();
void      gsm_api_incoming_accept();


int       gprs_api_connect_ip(const char* sz_type, const char* sz_ip, unsigned n_port);
void      gprs_api_shutdown();
bool      gprs_send_data(const char* p_data, int n_size );

bool      gsm_api_sms_send(const char* p_phone_number, const char* p_sms_msg);
//int       gsm_api_sms_read(int idx_msg, unsigned msg_type, SM_PARAM* p_msg);
bool      gsm_api_sms_delete(int idx_sms );
unsigned  gsm_api_check_sms_idx( const char* sz_msg );
bool      gsm_api_sms_sim_count( int& n_max, int& n_cur );
bool      gms_api_pbook_sim_count(int& n_max, int& n_cur);
//bool      gsm_api_phook_sim_read(unsigned nIdx, PBOOK_ITEM* pItem);
//bool      gsm_api_pbook_write(PBOOK_ITEM* pItem);
bool      gsm_api_pbook_del(int nIdx);
void      gsm_api_decode_mode(const char* p_str);

bool      gsm_api_get_imsi(char* p_str);
bool      gsm_api_get_imei(char* p_str);
bool	  gsm_api_get_connectstatus();

#endif //GSM_API_H