extern "C"
{
//**********************    commual subroutine    ***********************
HANDLE __stdcall  ic_init(__int16 port,unsigned long baud);
HANDLE __stdcall  auto_init(__int16 port,unsigned long baud);
__int16 __stdcall ic_exit(HANDLE icdev);
__int16 __stdcall srd_ver(HANDLE icdev, __int16 len,
						  unsigned char *data_buffer);
__int16 __stdcall set_baud(HANDLE icdev, unsigned long baud);
__int16 __stdcall cmp_dvsc(HANDLE icdev, __int16 len,
						   unsigned char *dta_buffer);
__int16 __stdcall srd_dvsc(HANDLE icdev, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall swr_dvsc(HANDLE icdev, __int16 len,
							 unsigned char *data_buffer);
__int16 __stdcall setsc_md(HANDLE icdev, __int16 mode);
__int16 __stdcall turn_on(HANDLE icdev);
__int16 __stdcall turn_off(HANDLE icdev);
__int16 __stdcall auto_pull(HANDLE icdev);
__int16 __stdcall get_status(HANDLE icdev,__int16 *state);
//hwj
__int16 __stdcall get_status_DPEx(HANDLE icdev,unsigned char CardType,__int16 *state);


__int16 __stdcall dv_beep(HANDLE icdev, __int16 time);
__int16 __stdcall swr_eeprom(HANDLE icdev, __int16 offset,__int16 len,
							   unsigned char *data_buffer);
__int16 __stdcall srd_eeprom(HANDLE icdev, __int16 offset,__int16 len,
							  unsigned char *data_buffer);
__int16 __stdcall srd_snr(HANDLE icdev, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall val_read(HANDLE icdev,unsigned long *p_Value);
__int16 __stdcall val_inc(HANDLE icdev, unsigned long _Value);
__int16 __stdcall val_dec(HANDLE icdev, unsigned long _Value);
__int16 __stdcall val_set(HANDLE icdev, unsigned long _Value);
__int32 __stdcall chk_baud(__int16 port);
__int16 __stdcall chk_card(HANDLE icdev);
__int16 __stdcall ic_encrypt( char *key,char *ptrSource, unsigned short msgLen, char *ptrDest);
__int16 __stdcall ic_decrypt( char *key,char *ptrSource, unsigned short msgLen, char *ptrDest);
unsigned long __stdcall  DES_Encrypt( unsigned char *Key,__int16 KeyLen,
						   unsigned char *Source, unsigned long SrcLen,
							   unsigned char *result);
unsigned long __stdcall  DES_Decrypt( unsigned char *Key,__int16 KeyLen,
						   unsigned char *Source, unsigned long SrcLen,
							   unsigned char *result);
__int16 __stdcall lib_ver(char *VerStr);
__int16 __stdcall asc_hex(unsigned char *asc, unsigned char *hex, long pair_len);
__int16 __stdcall hex_asc(unsigned char *hex,unsigned char *asc,long length);
__int16 __stdcall asc_asc(unsigned char *src,unsigned char *des,long len);
__int16 __stdcall float_uchar(float f,unsigned char *c);
__int16 __stdcall uchar_float(unsigned char *c,float *f);
__int16 __stdcall short_uchar(short i,unsigned char *c);
__int16 __stdcall uchar_short(unsigned char *c,short *i);
__int16 __stdcall long_uchar(long l,unsigned char *c);
__int16 __stdcall uchar_long(unsigned char *c,long *l);
//**********************    operate sle 4404    **************************
__int16 __stdcall srd_4404(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_4404(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall csc_4404(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wsc_4404(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsc_4404(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsct_4404(HANDLE icdev,__int16 *counter);
__int16 __stdcall cesc_4404(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wesc_4404(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resc_4404(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resct_4404(HANDLE icdev,__int16 *counter);
__int16 __stdcall ser_4404(HANDLE icdev,__int16 offset,__int16 len);
__int16 __stdcall fakefus_4404(HANDLE icdev,__int16 mode);
__int16 __stdcall clrpr_4404(HANDLE icdev);
__int16 __stdcall clrrd_4404(HANDLE icdev);
__int16 __stdcall psnl_4404(HANDLE icdev);
__int16 __stdcall chk_4404(HANDLE icdev);
//***********************    operate sle 4406    *************************
__int16 __stdcall srd_4406(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_4406(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall csc_4406(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wsc_4406(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsc_4406(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsct_4406(HANDLE icdev,__int16 *Counter);
__int16 __stdcall eswc_4406(HANDLE icdev,__int16 offset);
__int16 __stdcall psnl_4406(HANDLE icdev);
__int16 __stdcall chk_4406(HANDLE icdev);

//***********************    operate at88sc102    ************************
__int16 __stdcall srd_102(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_102(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall ser_102(HANDLE icdev,__int16 zone,__int16 offset,__int16 len);

__int16 __stdcall csc_102(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsc_102(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wsc_102(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsct_102(HANDLE icdev,__int16 *counter);

__int16 __stdcall cesc_102(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resc_102(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wesc_102(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resct_102(HANDLE icdev,__int16 zone,__int16 *counter);

__int16 __stdcall clrpr_102(HANDLE icdev,__int16 zone);
__int16 __stdcall clrrd_102(HANDLE icdev,__int16 zone);

__int16 __stdcall fakefus_102(HANDLE icdev,__int16 mode);
__int16 __stdcall psnl_102(HANDLE icdev);
__int16 __stdcall chk_102(HANDLE icdev);
//*********************    operate at88sc1604    **************************
__int16 __stdcall srd_1604(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_1604(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall ser_1604(HANDLE icdev,__int16 zone,__int16 offset,__int16 len);

__int16 __stdcall csc_1604(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsc_1604(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wsc_1604(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsct_1604(HANDLE icdev,__int16 zone,__int16 *counter);

__int16 __stdcall cesc_1604(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resc_1604(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wesc_1604(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resct_1604(HANDLE icdev,__int16 zone,__int16 *counter);

__int16 __stdcall clrpr_1604(HANDLE icdev,__int16 zone);
__int16 __stdcall clrrd_1604(HANDLE icdev,__int16 zone);

__int16 __stdcall fakefus_1604(HANDLE icdev,__int16 mode);
__int16 __stdcall psnl_1604(HANDLE icdev);
__int16 __stdcall chk_1604(HANDLE icdev);
//*********************    operate at88sc1604b    **************************
__int16 __stdcall srd_1604b(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_1604b(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall ser_1604b(HANDLE icdev,__int16 zone,__int16 offset,__int16 len);

__int16 __stdcall csc_1604b(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsc_1604b(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wsc_1604b(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsct_1604b(HANDLE icdev,__int16 zone,__int16 *counter);

__int16 __stdcall cesc_1604b(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resc_1604b(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wesc_1604b(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall resct_1604b(HANDLE icdev,__int16 zone,__int16 *counter);

__int16 __stdcall clrpr_1604b(HANDLE icdev,__int16 zone);
__int16 __stdcall clrrd_1604b(HANDLE icdev,__int16 zone);

__int16 __stdcall fakefus_1604b(HANDLE icdev,__int16 mode);
__int16 __stdcall psnl_1604b(HANDLE icdev);
__int16 __stdcall chk_1604b(HANDLE icdev);

//************************    operate at24c01A    ***************************
__int16 __stdcall swr_24c01a(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall srd_24c01a(HANDLE icdev, __int16 offset, __int16 len, 
							unsigned char *data_buffer);
__int16 __stdcall chk_24c01a(HANDLE icdev);
//************************    operate at24c02    ***************************
__int16 __stdcall  swr_24c02(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  srd_24c02(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  chk_24c02(HANDLE icdev);
//************************    operate at24c04    ***************************
__int16 __stdcall  swr_24c04(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  srd_24c04(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  chk_24c04(HANDLE icdev);
//************************    operate at24c08    ***************************
__int16 __stdcall  swr_24c08(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  srd_24c08(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  chk_24c08(HANDLE icdev);
//************************    operate at24c16    ***************************
__int16 __stdcall  swr_24c16(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  srd_24c16(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  chk_24c16(HANDLE icdev);
//************************    operate at24c32    ***************************
__int16 __stdcall  swr_24c32(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  srd_24c32(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  chk_24c32(HANDLE icdev);
//************************    operate at24c64    ***************************
__int16 __stdcall  swr_24c64(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  srd_24c64(HANDLE icdev, __int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall  chk_24c64(HANDLE icdev);
//***********************    operate sle 4418    *************************
__int16 __stdcall srd_4418(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_4418(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rdwpb_4418(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wrwpb_4418(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall pwr_4418(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall chk_4418(HANDLE icdev);
//***********************    operate sle 4428    *************************
__int16 __stdcall srd_4428(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_4428(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rdwpb_4428(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wrwpb_4428(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall pwr_4428(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall csc_4428(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wsc_4428(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsc_4428(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall rsct_4428(HANDLE icdev,__int16 *counter);
__int16 __stdcall chk_4428(HANDLE icdev);
//***********************    operate sle 4442    **************************
__int16 __stdcall srd_4442(HANDLE icdev,__int16 offset,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall swr_4442(HANDLE icdev,__int16 offset,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall prd_4442(HANDLE icdev,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall pwr_4442(HANDLE icdev,__int16 offset,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall csc_4442(HANDLE icdev,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall wsc_4442(HANDLE icdev,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall rsc_4442(HANDLE icdev,__int16 len, 
						   unsigned char *data_buffer);
__int16 __stdcall rsct_4442(HANDLE icdev,__int16 *counter);
__int16 __stdcall chk_4442(HANDLE icdev);

//***********************    operate sle 4432    **************************
__int16 __stdcall srd_4432(HANDLE icdev,__int16 offset,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall swr_4432(HANDLE icdev,__int16 offset,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall prd_4432(HANDLE icdev,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall pwr_4432(HANDLE icdev,__int16 offset,__int16 len,
						   unsigned char *data_buffer);
__int16 __stdcall chk_4432(HANDLE icdev);

//***********************    operate at45d041    *************************
__int16 __stdcall srd_45d041(HANDLE icdev,__int16 page,__int16 offset,unsigned long len,
						   unsigned char *data_buffer);
__int16 __stdcall swr_45d041(HANDLE icdev,__int16 page,__int16 offset,unsigned long len,
						   unsigned char *data_buffer);
__int16 __stdcall rdstrg_45d041(HANDLE icdev,unsigned char *ch);
__int16 __stdcall chk_45d041(HANDLE icdev);
//************************    operate at93c46a    ***************************
__int16 __stdcall swr_93c46a(HANDLE icdev,__int16 offset, __int16 len,
							unsigned char *data_buffer);
__int16 __stdcall srd_93c46a(HANDLE icdev,__int16 offset, __int16 len, 
							unsigned char *data_buffer);
__int16 __stdcall eral_93c46a(HANDLE icdev);
__int16 __stdcall chk_93c46a(HANDLE icdev);
//************************    operate at93c46    ***************************
__int16 __stdcall swr_93c46(HANDLE icdev,__int16 offset, __int16 len, unsigned char *data_buffer);
__int16 __stdcall srd_93c46(HANDLE icdev,__int16 offset, __int16 len, unsigned char *data_buffer);
__int16 __stdcall eral_93c46(HANDLE icdev);
__int16 __stdcall chk_93c46(HANDLE icdev);
//************************* cpu card ***************************************//
__int16 __stdcall cpu_reset(HANDLE icdev,unsigned char *data_buffer);
__int16 __stdcall cpu_protocol(HANDLE icdev,__int16 len, unsigned char *send_cmd, unsigned char *receive_data);
__int16 __stdcall cpu_comres(HANDLE icdev,__int16 len, unsigned char *send_cmd, unsigned char *receive_data);
//************************* sam card ****************************************
__int16 __stdcall set_card_baud(HANDLE icdev, unsigned char CardType,unsigned char BaudCode);
__int16 __stdcall sam_power_on(HANDLE icdev);
__int16 __stdcall sam_power_down(HANDLE icdev);
__int16 __stdcall sam_reset(HANDLE icdev,__int16 *len,unsigned char *receive_data);
__int16 __stdcall sam_select(HANDLE icdev, unsigned char SelectCard);
__int16 __stdcall sam_protocol(HANDLE icdev,__int16 sLen, unsigned char *send_cmd, unsigned char *receive_data);
__int16 __stdcall sam_comres(HANDLE icdev,__int16 sLen, unsigned char *send_cmd, unsigned char *receive_data);
//new three sam card
//************************* sam card ****************************************

__int16 __stdcall sam_slt_power_on(HANDLE icdev,unsigned char CardType);
__int16 __stdcall sam_slt_power_down(HANDLE icdev,unsigned char CardType);


__int16 __stdcall sam_slt_reset(HANDLE icdev,unsigned char CardType,__int16 *len,unsigned char *receive_data);
__int16 __stdcall sam_slt_protocol(HANDLE icdev,unsigned char CardType,__int16 sLen, unsigned char *send_cmd, __int16 *rLen,unsigned char *receive_data);


//*********************    operate at88sc1608    **************************
__int16 __stdcall reset_1608(HANDLE icdev,__int16 len, unsigned char *data_buffer);
__int16 __stdcall init_auth_1608(HANDLE icdev,__int16 len, unsigned char *data_buffer);
//__int16 __stdcall vrf_1608(HANDLE icdev,__int16 len, unsigned char *data_buffer);
__int16 __stdcall callsrd_1608(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall srd_1608(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall srfus_1608(HANDLE icdev,__int16 len,unsigned char *data_buffer);
__int16 __stdcall callswr_1608(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swr_1608(HANDLE icdev,__int16 zone,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall csc_1608(HANDLE icdev,__int16 zone,__int16 len,__int16 rw,unsigned char *data_buffer);
__int16 __stdcall psnl_1608(HANDLE icdev);
__int16 __stdcall wsc_1608(HANDLE icdev,__int16 zone,__int16 len, __int16 rw,unsigned char *data_buffer);
__int16 __stdcall rsc_1608(HANDLE icdev,__int16 zone,__int16 len,__int16 rw,unsigned char *data_buffer);
__int16 __stdcall rsct_1608(HANDLE icdev,__int16 zone,__int16 len, __int16 rw,unsigned char *data_buffer);
__int16 __stdcall rac_1608(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall wac_1608(HANDLE icdev,__int16 zone,__int16 len,unsigned char *data_buffer);
__int16 __stdcall chk_1608(HANDLE icdev);
__int16 __stdcall srdconfig_1608(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
__int16 __stdcall swrconfig_1608(HANDLE icdev,__int16 offset,__int16 len,unsigned char *data_buffer);
void __stdcall Authenticate(unsigned char *q1,unsigned char *q2);
void __stdcall SetInit(unsigned char *crypto, unsigned char *graine,unsigned char *host);

//************************* math calculate ************************************
unsigned char __stdcall xor(unsigned char char1,unsigned char char2);
unsigned char __stdcall ck_bcc(__int16 len, unsigned char *bcc_buffer);
unsigned char __stdcall cr_bcc(__int16 len, unsigned char *bcc_buffer);
unsigned char __stdcall comm_bcc(__int16 len, unsigned char *bcc_buffer);
__int16 __stdcall srd_ssf1101(HANDLE icdev,__int16 page,__int16 offset,long len,
						   unsigned char *data_buffer);
__int16 __stdcall swr_ssf1101(HANDLE icdev,__int16 page,__int16 offset,long len,
						   unsigned char *data_buffer);
__int16 __stdcall chk_ssf1101(HANDLE icdev);

__int16 __stdcall eral_ssf1101(HANDLE icdev);

//************************Demo new define hwj***************
__int16 __stdcall exp_dis(HANDLE icdev,char *filename,__int16 *no);
__int16 __stdcall get_status0(HANDLE icdev,__int16 *state);
__int16 __stdcall set_code(__int16 CardType,__int16 num,__int16 len,unsigned char *code);

__int16 __stdcall swr_snr(HANDLE icdev, __int16 len,unsigned char *data_buffer);

}
