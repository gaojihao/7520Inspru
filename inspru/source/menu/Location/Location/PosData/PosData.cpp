#include "StdAfx.h"
#include "PosData.h"
#include "../Tools/LogTool.h"

//��ʼ����̬��Ա����
CPosData*	CPosData::m_pInstance	= NULL;

#define		MSG_EURP_START			( WM_USER + 0x1000 )
#define		MSG_SYS_BEGIN			( MSG_EURP_START + 1 )
#define		MSG_LOCATION_DATA		( MSG_SYS_BEGIN  + 2 ) // location message.

//��BD2����������
#define CMD_SGL_BD2			"$CCSIR,1,0*49\r\n"
//��GPS����������
#define CMD_SGL_GPS			"$CCSIR,2,0*4A\r\n"
//BD2/GPS˫ģ����������
#define CMD_DBL_BD2_GPS		"$CCSIR,3,0*4B\r\n"

//
#define MODE_BD				"$BD"
#define MODE_GP				"$GP"
#define MODE_GN				"$GN"
#define DATA_RMC			"RMC"
#define DATA_GGA			"GGA"
#define DATA_GLL			"GLL"
#define DATA_GSA			"GSA"
#define DATA_GSV			"GSV"

extern PPOS_INFO			g_pPosInfo;				//�����ڴ���ָ��

CPosData::CPosData(void)
{
	m_pPosCom = new CSerialCom(&CPosData::ErrorInfo, &CPosData::CatenatePosData);
	m_pConvertCom	= NULL;

	m_nMsgIDPosRefresh	= 0;
	memset(m_hDestWnd, NULL, sizeof(m_hDestWnd));
	memset(m_cGPS, NULL, sizeof(m_cGPS));
	m_wGPSOffset	= 0;
}

CPosData::~CPosData(void)
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	delete m_pPosCom;
	m_pPosCom = NULL;
}

/************************************************************************
* ������: Instance
* ��  ��: 
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: singleton
************************************************************************/
CPosData* CPosData::Instance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CPosData;
	}
	return m_pInstance;
}

/************************************************************************
* ������:	InitGPSCom
* ��  ��:	���GPS����
* ��  ��:	wGPSCom		wGPSCom
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CPosData::InitPosCom(WORD wPosCom, WORD wBaudRate, enPOS_DATA_MODE data_mode)
{
	TCHAR	szPosCom[8] = {0};

	m_wBaudRate	= wBaudRate;

	_stprintf(szPosCom, _T("COM%d:"), wPosCom);
	Port_Parameters tPortParam = {szPosCom, wBaudRate, 8, ONESTOPBIT, NOPARITY, TRUE, TRUE};

	if( !m_pPosCom->OpenPort(tPortParam) )
	{
		CString str;
		str.Format(_T("Open Pos Com Failed:com%s,%d"), szPosCom, wBaudRate);
		AfxMessageBox( str );
		return FALSE;
	}

	m_nMsgIDPosRefresh	= RegisterWindowMessage( WM_POS_DATA_REFRESH );

	//����BD2/GPSģ������ģʽ
	return SetDataMode(data_mode);
}

/************************************************************************
* ������:	CloseCom
* ��  ��:	�ر�BD2/GPS����
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CPosData::CloseCom()
{
	BOOL bSuccess = m_pPosCom->ClosePort();
	return bSuccess;
}

/************************************************************************
* ������:	SetDataMode
* ��  ��:	����BD2/GPSģ������ģʽ
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CPosData::SetDataMode(enPOS_DATA_MODE data_mode)
{
	if( !m_pPosCom->IsPortOpen() )
		return FALSE;

	char* pModeCMD = NULL;
	m_data_mode	= data_mode;
	switch(m_data_mode)
	{
	case MODE_SGL_BD2:		//��BD2
		pModeCMD	= CMD_SGL_BD2;
		break;
	case MODE_SGL_GPS:		//��GPS
		pModeCMD	= CMD_SGL_GPS;
		break;
	case MODE_DBL_BD2:		//BD2/GPS˫ģ, ѡ����BD2����
	case MODE_DBL_GPS:		//BD2/GPS˫ģ, ѡ����GPS����
		pModeCMD	= CMD_DBL_BD2_GPS;
		break;
	default:
		pModeCMD	= CMD_SGL_GPS;
		m_data_mode	= MODE_SGL_GPS;
		break;
	}
	m_pPosCom->SendData((PBYTE)pModeCMD, strlen(pModeCMD));
	return TRUE;
}

/************************************************************************
* ������:	GetDataMode
* ��  ��:	��ȡBD2/GPSģ������ģʽ
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
enPOS_DATA_MODE	CPosData::GetDataMode()
{
	return m_data_mode;
}

/************************************************************************
* ������:	AddHWND
* ��  ��:	��Ӵ��ھ�����Ի�ȡλ����Ϣ��Ϣ
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
//BOOL CPosData::AddHWND(HWND hWnd)
//{
//	for(int i=0; i<10; i++)
//	{
//		if(m_hDestWnd[i] == NULL)
//		{
//			m_hDestWnd[i] = hWnd;
//			return TRUE;
//		}
//		if(hWnd == m_hDestWnd[i])
//			return TRUE;
//	}
//	return FALSE;
//}

/************************************************************************
* ������:	RemoveHWND
* ��  ��:	ɾ�����ھ��
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
//void CPosData::RemoveHWND(HWND hWnd)
//{
//	for(int i=0; i<10; i++)
//	{
//		if(m_hDestWnd[i] == NULL)
//			return;
//		if(m_hDestWnd[i] != hWnd)
//			continue;
//
//		int j = i;
//		for(; j<10-1; j++)
//		{
//			if(m_hDestWnd[j] != NULL)
//				m_hDestWnd[j] = m_hDestWnd[j+1];
//		}
//		if(j == 9)
//			m_hDestWnd[j] = NULL;
//		return;
//	}
//}

/************************************************************************
* ������:	RemoveHWND
* ��  ��:	ʹ��ת����GPS����
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
BOOL CPosData::EnableConvertGPS(WORD wConvertCom)
{
	if(NULL == m_pConvertCom)
		m_pConvertCom	= new CSerialCom(NULL, NULL);

	if( !m_pPosCom->IsPortOpen() )
		return FALSE;

	TCHAR	szPosCom[8] = {0};
	_stprintf(szPosCom, _T("COM%d:"), wConvertCom);
	Port_Parameters tPortParam = {szPosCom, m_wBaudRate, 8, ONESTOPBIT, NOPARITY, TRUE, TRUE};

	if( !m_pConvertCom->OpenPort(tPortParam) )
	{
		CString str;
		str.Format(_T("Open Convert Com Failed:com%s,%d"), szPosCom, m_wBaudRate);
		AfxMessageBox( str );
		return FALSE;
	}

	return TRUE;
}

/************************************************************************
* ������:	RemoveHWND
* ��  ��:	Disableת��GPS����
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
void CPosData::DisableConvertGPS()
{
	if(m_pConvertCom != NULL)
		m_pConvertCom->ClosePort();
}

/************************************************************************
* ������:	ParseRMC
* ��  ��:	����RMC
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTCʱ�䣬hhmmss(ʱ����)��ʽ
<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
<3> γ��ddmm.mmmm(�ȷ�)��ʽ(ǰ���0Ҳ��������)
<4> γ�Ȱ���N(������)��S(�ϰ���)
<5> ����dddmm.mmmm(�ȷ�)��ʽ(ǰ���0Ҳ��������)  
<6> ���Ȱ���E(����)��W(����)
<7> ��������(000.0~999.9�ڣ�ǰ���0Ҳ��������)
<8> ���溽��(000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ��������)
<9> UTC���ڣ�ddmmyy(������)��ʽ
<10> ��ƫ��(000.0~180.0�ȣ�ǰ���0Ҳ��������)
<11> ��ƫ�Ƿ���E(��)��W(��)
<12> ģʽָʾ(��NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч)
************************************************************************/
void CPosData::ParseRMC(PCSTR pcRMC)
{
	//test
	//pcGPRMC = "$GPRMC,101904.000,A,3110.4104,N,12123.8126,E,0.00,,220707,,,A*71";

	WORD wFieldLen	= 0;	//���ݶ�(����','֮����ַ���)�ĳ���
	int		nValue	= 0;	//�������ݶε�ֵ
	double	fValue	= 0;	//���������ݶε�ֵ

	//���GPRMC�����ݶ��Ƿ�Ϊ12��
	//PCSTR pcField	= ScanField(pcGPRMC, 12, wFieldLen);
	//if(NULL == pcField)
	//	return;

	//<1> UTCʱ�䣬hhmmss(ʱ����)��ʽ
	PCSTR pcField	= ScanField(pcRMC, 1, wFieldLen);
	if(NULL == pcField)
		return;
	nValue = atoi( pcField );
	g_pPosInfo->rmc_data.UtcTime.wHour		= nValue / 10000;
	g_pPosInfo->rmc_data.UtcTime.wMinute	= nValue / 100 % 100;
	g_pPosInfo->rmc_data.UtcTime.wSecond	= nValue % 100;

	//<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	if('A' == pcField[0])
	{
		g_pPosInfo->rmc_data.pos_state.cFix	= 1;	//��λ״̬
	}
	else if('V' == pcField[0])
	{
		g_pPosInfo->rmc_data.pos_state.cFix	= 0;	//��λ״̬
	}
	else
	{
		return;
	}

	//<3> γ��ddmm.mmmm(�ȷ�)��ʽ(ǰ���0Ҳ��������)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue	= atof( pcField );
	nValue	= (int)fValue / 100;	//�����
	g_pPosInfo->rmc_data.dbLatitude	= (double)(nValue + (fValue - nValue*100)/60);//γ�� ȥ��10��6�η���ȡԭֵ

	//<4> γ�Ȱ���N(������)��S(�ϰ���)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	if('N' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLatitudeType	= 0;
	else if('S' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLatitudeType	= 1;
	else
		g_pPosInfo->rmc_data.pos_state.cLatitudeType	= 0;

	//<5> ����dddmm.mmmm(�ȷ�)��ʽ(ǰ���0Ҳ��������)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue	= atof( pcField );
	nValue	= (int)fValue / 100;	//�����
	g_pPosInfo->rmc_data.dbLongitude = (double)(nValue + (fValue - nValue*100)/60);//����  ȥ��10��6�η���ȡԭֵ

	//<6> ���Ȱ���E(����)��W(����)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	if('E' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLongitudeType	= 0;
	else if('W' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLongitudeType	= 1;
	else
		g_pPosInfo->rmc_data.pos_state.cLongitudeType	= 0;

	//<7> ��������(000.0~999.9�ڣ�ǰ���0Ҳ��������)	1��(����) = 1.852km/h
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue	= atof( pcField );
	g_pPosInfo->rmc_data.dbSpeed = (fValue*1.852*10+0.5);	//��������1.851985

	//<8> ���溽��(000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ��������)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	//fValue	= atof( pcField ) + 0.5;	//��������
	fValue	= atof( pcField );
	g_pPosInfo->rmc_data.dbAzimuth	= fValue; //��λ��

	//<9> UTC���ڣ�ddmmyy(������)��ʽ
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	nValue = atoi( pcField );
	g_pPosInfo->rmc_data.UtcTime.wYear	= (nValue % 100) + 2000;
	g_pPosInfo->rmc_data.UtcTime.wMonth	= nValue / 100 % 100;
	g_pPosInfo->rmc_data.UtcTime.wDay	= nValue / 10000;

	Convert2GPS(pcRMC);
}

/************************************************************************
* ������:	ParseGGA
* ��  ��:	����GGA
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>
<1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<2> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<3> γ�Ȱ���N�������򣩻�S���ϰ���
<4> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<5> ���Ȱ���E����������W��������
<6> GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
<7> ����ʹ�ý���λ�õ�����������00~12����ǰ���0Ҳ�������䣩
<8> HDOPˮƽ�������ӣ�0.5~99.9��
<9> ���θ߶ȣ�-9999.9~99999.9��
<10> ������������Դ��ˮ׼��ĸ߶�
<11> ���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
<12> ���վID��0000~1023��ǰ���0Ҳ�������䣬������ǲ�ֶ�λ��Ϊ�գ�
************************************************************************/
void CPosData::ParseGGA(PCSTR pcGGA)
{
	//Test
	//pcGPGGA	= "$GPGGA,101904.000,3110.4104,N,12123.8126,E,1,04,2.7,91.0,M,8.0,M,,0000*5C";

	WORD wFieldLen	= 0;	//���ݶ�(����','֮����ַ���)�ĳ���
	int		nValue	= 0;	//�������ݶε�ֵ
	double	fValue	= 0;	//���������ݶε�ֵ

	if (NULL == pcGGA)
	{
		return;
	}
	//���GGA�����ݶ��Ƿ�Ϊ14��
	//PCSTR pcField	= ScanField(pcGPGGA, 14, wFieldLen);
	//if(NULL == pcField)
	//	return;

	//<9> ���θ߶ȣ�-9999.9~99999.9��
	PCSTR pcField	= ScanField(pcGGA, 9, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->rmc_data.dbAltitude	= fValue;

	Convert2GPS(pcGGA);
}

/************************************************************************
* ������:	ParseGSA
* ��  ��:	����GSA
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
$GPGSA, <1>,<2>,<3>,<3>,,,,,<3>,<3>,<3>,<4>,<5>,<6>,<7><CR><LF>
<1>ģʽ ��M = �ֶ��� A = �Զ���
<2>��λ��ʽ 1 = δ��λ�� 2 = ��ά��λ�� 3 = ��ά��λ��
<3>PRN ���֣�01 �� 32 �����ʹ���е����Ǳ�ţ����ɽ���12��������Ϣ��
<4> PDOPλ�þ������ӣ�0.5~99.9��
<5> HDOPˮƽ�������ӣ�0.5~99.9��
<6> VDOP��ֱ�������ӣ�0.5~99.9��
<7> Checksum.(���λ).
************************************************************************/
void CPosData::ParseGSA(PCSTR pcGSA)
{
	//Test
	//GPGSA	= "$GPGSA,A,3,14,15,05,22,18,26,,,,,,,2.1,1.2,1.7*3D";

	WORD wFieldLen	= 0;	//���ݶ�(����','֮����ַ���)�ĳ���
	int		nValue	= 0;	//�������ݶε�ֵ
	double	fValue	= 0;	//���������ݶε�ֵ
	PCSTR pcField	= NULL;

	//������һ�����Ǳ��
	pcField	= ScanField(pcGSA, 3, wFieldLen);

	//���ʹ���е����Ǳ��
	memset(g_pPosInfo->gsa_data.wUsedID, NULL, sizeof(g_pPosInfo->gsa_data.wUsedID));
	for ( int i = 0; i < MAX_SATE_NUM; i++ )
	{
		if(NULL == pcField || 0 == wFieldLen)
		{
			g_pPosInfo->gsa_data.wSatNum	= i;	//���Ǹ���
			break;
		}

		g_pPosInfo->gsa_data.wUsedID[i] = atoi( pcField );
		pcField	= ScanField(pcField, 1, wFieldLen);
	}

	//PDOPλ�þ�������(0.5~99.9)
	//MAX_SATE_NUM-g_pPosInfo->gsa_data.wSatNum,����ʣ��հ׵����Ǳ����
	pcField	= ScanField(pcField, MAX_SATE_NUM-g_pPosInfo->gsa_data.wSatNum, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->gsa_data.dbPdop = fValue;

	//HDOPˮƽ��������(0.5~99.9)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->gsa_data.dbHdop = fValue;

	//VDOP��ֱ��������(0.5~99.9)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->gsa_data.dbVdop = fValue;

	Convert2GPS(pcGSA);
}

/************************************************************************
* ������:	ParseGSV
* ��  ��:	����GSV
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
$GPGSV, <1>,<2>,<3>,<4>,<5>,<6>,<7>,?<4>,<5>,<6>,<7>,<8><CR><LF>
<1> GSV��������
<2> ����GSV�ı��
<3> �ɼ����ǵ�������00 �� 12��
<4> ���Ǳ�ţ� 01 �� 32��
<5>�������ǣ� 00 �� 90 �ȡ�
<6>���Ƿ�λ�ǣ� 000 �� 359 �ȡ�ʵ��ֵ��
<7>Ѷ�������ȣ�C/No���� 00 �� 99 dB���ޱ�δ���յ�Ѷ�š�
<8>Checksum.(���λ).
************************************************************************/
void CPosData::ParseGSV(PCSTR pcGSV, PCSTR pcSectionGSV)
{
	//Test
	//GPGSV	= "$GPGSV,3,1,10,18,84,067,23,09,67,067,27,22,49,312,28,15,47,231,30*70
	//         $GPGSV,3,2,10,21,32,199,23,14,25,272,24,05,21,140,32,26,14,070,20*7E
	//	       $GPGSV,3,3,10,29,07,074,,30,07,163,28*7D";

	WORD  wFieldLen = 0;	//���ݶ�(����','֮����ַ���)�ĳ���
	int		nValue	= 0;	//�������ݶε�ֵ

	WORD	wGSVNum	= 0;	//GSV�������
	WORD	wGSVIndex	= 0;	//��ǰGSV������
	//WORD	wSateNum = 0;	//��Ұ��������
	const WORD	GSV_SATE_NUM	= 4;	//һ��GSV��������������

	//memset(g_pPosInfo->gsv_data, NULL, sizeof(g_pPosInfo->gsv_data));

	BOOL	bFirstGSV = TRUE;
	PCSTR	pcField	= pcGSV;

	Convert2GPS(pcField);

	do
	{
		//GSV�������
		pcField	= ScanField(pcField, 1, wFieldLen);
		if(NULL == pcField)
			return;
		if(bFirstGSV)
			wGSVNum = atoi( pcField );
		//��ǰGSV������
		pcField	= ScanField(pcField, 1, wFieldLen);
		if(NULL == pcField)
			return;
		wGSVIndex = atoi( pcField );
		//��Ұ��������
		pcField	= ScanField(pcField, 1, wFieldLen);
		if(NULL == pcField)
			return;
		if(bFirstGSV)
			g_pPosInfo->rmc_data.pos_state.dwSateNum = atoi( pcField );

		
		for(int i=0; i<GSV_SATE_NUM; i++)
		{
			int j = (wGSVIndex-1)*GSV_SATE_NUM+i;

			if (j >= MAX_SATE_NUM)
			{
				break;
			}

			//���һ��������Ϣ
			if(j == g_pPosInfo->rmc_data.pos_state.dwSateNum)
				break;
			//���Ǳ��
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wSatID	= atoi( pcField );
			//��������:��
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wElevation = atoi( pcField );
			//���Ƿ�λ��:��
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wAzimuth = atoi( pcField );
			//�����
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wSNR = atoi( pcField );
			g_pPosInfo->gsv_data[j].bAvailible	= TRUE;
		}
		//���һ��GSV���
		if(wGSVIndex == wGSVNum)
			break;
		//������һ��GSV���
		pcField = strstr(pcField, pcSectionGSV);
		if(NULL == pcField)
			break;
		Convert2GPS(pcField);
		bFirstGSV	= FALSE;
	} while ( TRUE );

	//��ʹ��memset g_pPosInfo->gsv_data
	for(int i=g_pPosInfo->rmc_data.pos_state.dwSateNum; i<MAX_SATE_NUM; i++)
		g_pPosInfo->gsv_data[i].bAvailible	= FALSE;

	return;
}

/************************************************************************
* ������:	Convert2GPS
* ��  ��:	ת��GPS
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
void CPosData::Convert2GPS(PCSTR pcSentence)
{
	if(NULL == m_pConvertCom)
		return;
	if( !m_pConvertCom->IsPortOpen() )
		return;

	//GPSģʽ����Ҫת��
	if(MODE_SGL_GPS == m_data_mode)
		return;
	
	PCSTR pcBuffer	= pcSentence;

	//ģʽΪMODE_DBL_GPSʱ��GPGSA��GPGSV����ֱ�ӿ�����m_cGPS��
	//$GPGSA,A,3,03,06,14,21,23,19,16,32,,,,,1.3,0.9,0.9,0.7*39
	//"$GPGSV,3,3,9,32,21,242,14*73\r\n"
	if( strncmp(pcSentence, MODE_GP, strlen(MODE_GP)) == 0 )
	{
		do
		{
			m_cGPS[m_wGPSOffset++]	= *pcBuffer++;
		} while ( pcBuffer != NULL && *pcBuffer != '\n' && m_wGPSOffset<NUM_1K-1);
		m_cGPS[m_wGPSOffset++]	= *pcBuffer;	//���һ���ַ�Ϊ'\n'
		return;
	}

	//��������
	//$GNRMC,075956.00,A,3109.716706,N,12123.342776,E,0.0,,250413,,,A*6D
	//$GNGGA,075956.00,3109.716706,N,12123.342776,E,1,15,0.9,62.9,M,8.0,M,,,0.9*73
	//$GNGLL,3109.716706,N,12123.342776,E,075956.00,A,0*04
	//$BDRMC,075654.00,A,3109.731130,N,12123.330827,E,0.0,,250413,,,A*67
	//$BDGGA,075654.00,3109.731130,N,12123.330827,E,1,07,4.3,86.7,M,8.0,M,,,2.5*7E
	//$BDGLL,3109.731130,N,12123.330827,E,075654.00,A,0*0E
	//$BDGSA,A,3,06,13,08,01,09,11,03,,,,,,1.3,0.9,0.9,0.7*23
	//$BDGSV,2,2,7,09,09,216,22,11,57,108,16,03,53,200,32*60
	if( strncmp(pcSentence, MODE_BD, strlen(MODE_BD)) == 0
		|| strncmp(pcSentence, MODE_GN, strlen(MODE_GN)) == 0)
	{
		pcBuffer	+= 3;
		memcpy(&m_cGPS[m_wGPSOffset], MODE_GP, 3);
		m_wGPSOffset	+= 3;
		BYTE cCheckSum	= 'G' ^ 'P';
		do
		{
			cCheckSum	^= *pcBuffer;
			m_cGPS[m_wGPSOffset++]	= *pcBuffer++;
		} while ( pcBuffer != NULL && *pcBuffer != '*' && m_wGPSOffset<NUM_1K-5);
		sprintf(&m_cGPS[m_wGPSOffset], "*%02X\r\n", cCheckSum);
		m_wGPSOffset	+= 5;
		return;
	}
}

/************************************************************************
* ������:	Convert2GPS
* ��  ��:	ת��GPS
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
void CPosData::SendGPS(PCSTR pcPosData, DWORD dwLen)
{
	if(NULL == m_pConvertCom)
		return;
	if( !m_pConvertCom->IsPortOpen() )
		return;

	if(MODE_SGL_GPS == m_data_mode)
		m_pConvertCom->SendData((PBYTE)pcPosData, dwLen);
	else
		m_pConvertCom->SendData((PBYTE)m_cGPS, m_wGPSOffset);

	m_wGPSOffset = 0;
	//m_pConvertCom->SendData((PBYTE)"\r\n", 2);
}

/************************************************************************
* ������:	GPSErrorInfo
* ��  ��:	GPS���ڱ�����Ϣ
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
int CPosData::ErrorInfo(LPCTSTR lpText)
{
	MessageBox(NULL, lpText, TEXT("���ڳ���"), MB_OK | MB_SETFOREGROUND);
	return 0;
}

/************************************************************************
* ������:	CatenateGPSData
* ��  ��:	ƴ������GPS����
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע: 
************************************************************************/
void CPosData::CatenatePosData(BYTE *pcBuffer, DWORD dwLen)
{
	if ((NULL == pcBuffer) || (dwLen == 0))
	{
		return;
	}

	m_pInstance->PosDataParser((PCSTR)pcBuffer, dwLen);
	

	return;
}

/************************************************************************
* ������:	GPSDataParser
* ��  ��:	����NEMA-0183Э������
* ��  ��:
* ��  ��: 
* ��  ��: 
* ��  ע:	һ���ӽ���һ��
��BD2����������$CCSIR,1,0*49<CR><LF>
$BDRMC,075654.00,A,3109.731130,N,12123.330827,E,0.0,,250413,,,A*67
$BDGGA,075654.00,3109.731130,N,12123.330827,E,1,07,4.3,86.7,M,8.0,M,,,2.5*7E
$BDGLL,3109.731130,N,12123.330827,E,075654.00,A,0*0E
$BDGSA,A,3,06,13,08,01,09,11,03,,,,,,5.0,4.3,2.5,3.6*26
$BDGSV,2,1,7,06,02,191,25,13,10,202,33,08,58,185,26,01,49,145,32*55
$BDGSV,2,2,7,09,09,216,24,11,57,111,25,03,53,200,32*6E

��GPS����������$CCSIR,2,0*4A<CR><LF> 
$GPRMC,075819.00,A,3109.717233,N,12123.341973,E,0.0,,250413,,,A*73
$GPGGA,075819.00,3109.717233,N,12123.341973,E,1,07,1.0,66.7,M,8.0,M,,,2.1*66
$GPGLL,3109.717233,N,12123.341973,E,075819.00,A,0*1A
$GPGSA,A,3,03,06,14,21,23,32,19,,,,,,2.3,1.0,2.1,0.9*31
$GPGSV,2,1,8,03,25,199,41,06,41,183,41,14,17,151,38,21,06,103,31*46
$GPGSV,2,2,8,23,25,316,33,32,21,243,35,19,02,198,41,30,66,344,26*49

BD2/GPS˫ģ����������$CCSIR,3,0*4B<CR><LF> 
$GNRMC,075955.00,A,3109.716702,N,12123.342782,E,0.0,,250413,,,A*61
$GNGGA,075955.00,3109.716702,N,12123.342782,E,1,15,0.9,62.9,M,8.0,M,,,0.9*7F
$GNGLL,3109.716702,N,12123.342782,E,075955.00,A,0*08
$GPGSA,A,3,03,06,14,21,23,19,16,32,,,,,1.3,0.9,0.9,0.7*39
$BDGSA,A,3,06,13,08,01,09,11,03,,,,,,1.3,0.9,0.9,0.7*23
$GPGSV,3,1,9,03,25,199,39,06,42,183,41,14,17,151,39,21,06,102,33*48
$GPGSV,3,2,9,23,26,316,24,19,03,198,40,30,65,346,21,16,59,276,12*40
$GPGSV,3,3,9,32,21,242,14*73
$BDGSV,2,1,7,06,01,191,21,13,09,201,35,08,58,186,26,01,49,145,32*5C
$BDGSV,2,2,7,09,09,216,22,11,57,108,16,03,53,200,32*60
************************************************************************/
void CPosData::PosDataParser(PCSTR pcPosData, DWORD dwLen)
{
	CLogTool::Instance()->WriteLogFile(pcPosData);
#ifdef DEBUG_0
	//32.4J = 60km/h
	pcPosData = 
		"$BDRMC,075654.00,A,3109.731130,N,12123.330827,E,0.0,,250413,,,A*67\r\n"
		"$BDGGA,075654.00,3109.731130,N,12123.330827,E,1,07,4.3,86.7,M,8.0,M,,,2.5*7E\r\n"
		"$BDGLL,3109.731130,N,12123.330827,E,075654.00,A,0*0E\r\n"
		"$BDGSA,A,3,06,13,08,01,09,11,03,,,,,,5.0,4.3,2.5,3.6*26\r\n"
		"$BDGSV,2,1,7,06,02,191,25,13,10,202,33,08,58,185,26,01,49,145,32*55\r\n"
		"$BDGSV,2,2,7,09,09,216,24,11,57,111,25,03,53,200,32*6E\r\n";

	/*pCPosData = 
		"\r\n"
		"\r\n";*/
#endif

	if(NULL == g_pPosInfo)
		return;

	PCSTR pcHead	= NULL;
	PCSTR pcSection	= NULL;

	//�ж�BD2/GPS����ͷ
	if( (pcHead = strstr(pcPosData, MODE_BD DATA_RMC)) != NULL )		//��BD2
	{
		if(m_data_mode != MODE_SGL_BD2)
			m_data_mode = MODE_SGL_BD2;
	}
	else if( (pcHead = strstr(pcPosData, MODE_GP DATA_RMC)) != NULL )	//��GPS
	{
		if(m_data_mode != MODE_SGL_GPS)
			m_data_mode = MODE_SGL_GPS;
	}
	else if( (pcHead = strstr(pcPosData, MODE_GN DATA_RMC)) != NULL )	//BD2/GPS˫ģ
	{
		if(m_data_mode != MODE_DBL_BD2 && m_data_mode != MODE_DBL_GPS)
			m_data_mode = MODE_DBL_GPS;
	}
	else
		return;

	char* pcSectionRMC = NULL;
	char* pcSectionGGA = NULL;
	char* pcSectionGSA = NULL;
	char* pcSectionGSV = NULL;

	switch(m_data_mode)
	{
	case MODE_SGL_BD2:
		pcSectionRMC	= MODE_BD DATA_RMC;
		pcSectionGGA	= MODE_BD DATA_GGA;
		pcSectionGSA	= MODE_BD DATA_GSA;
		pcSectionGSV	= MODE_BD DATA_GSV;
		break;
	case MODE_SGL_GPS:
		pcSectionRMC	= MODE_GP DATA_RMC;
		pcSectionGGA	= MODE_GP DATA_GGA;
		pcSectionGSA	= MODE_GP DATA_GSA;
		pcSectionGSV	= MODE_GP DATA_GSV;
		break;
	case MODE_DBL_BD2:	
		pcSectionRMC	= MODE_GN DATA_RMC;
		pcSectionGGA	= MODE_GN DATA_GGA;
		pcSectionGSA	= MODE_BD DATA_GSA;
		pcSectionGSV	= MODE_BD DATA_GSV;
		break;
	case MODE_DBL_GPS:
		pcSectionRMC	= MODE_GN DATA_RMC;
		pcSectionGGA	= MODE_GN DATA_GGA;
		pcSectionGSA	= MODE_GP DATA_GSA;
		pcSectionGSV	= MODE_GP DATA_GSV;
		break;
	}
	//����RMC����
	pcSection	= strstr(pcHead, pcSectionRMC);
	if (pcSection != NULL)
		ParseRMC(pcSection);

	//����GGA����
	pcSection	= strstr(pcHead, pcSectionGGA);
	if (pcSection != NULL)
		ParseGGA(pcSection);

	//ת��GLL����
	pcSection	= strstr(pcHead, DATA_GLL);
	if (pcSection != NULL)
		Convert2GPS(pcSection-3);

	//����GSA����
	pcSection	= strstr(pcHead, pcSectionGSA);
	if (pcSection != NULL)
		ParseGSA(pcSection);

	//����GSV����
	pcSection	= strstr(pcHead, pcSectionGSV);
	if (pcSection != NULL)
		ParseGSV(pcSection, pcSectionGSV);

	PostMessage(HWND_BROADCAST, m_nMsgIDPosRefresh, 0, 0);

	//ת��GPS����
	SendGPS(pcPosData, dwLen);
}
