#include "StdAfx.h"
#include "PosData.h"
#include "../Tools/LogTool.h"

//初始化静态成员变量
CPosData*	CPosData::m_pInstance	= NULL;

#define		MSG_EURP_START			( WM_USER + 0x1000 )
#define		MSG_SYS_BEGIN			( MSG_EURP_START + 1 )
#define		MSG_LOCATION_DATA		( MSG_SYS_BEGIN  + 2 ) // location message.

//单BD2（热启动）
#define CMD_SGL_BD2			"$CCSIR,1,0*49\r\n"
//单GPS（热启动）
#define CMD_SGL_GPS			"$CCSIR,2,0*4A\r\n"
//BD2/GPS双模（热启动）
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

extern PPOS_INFO			g_pPosInfo;				//共享内存区指针

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
* 函数名: Instance
* 描  述: 
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: singleton
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
* 函数名:	InitGPSCom
* 描  述:	监控GPS串口
* 入  参:	wGPSCom		wGPSCom
* 出  参: 
* 返  回: 
* 备  注: 
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

	//设置BD2/GPS模块数据模式
	return SetDataMode(data_mode);
}

/************************************************************************
* 函数名:	CloseCom
* 描  述:	关闭BD2/GPS串口
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CPosData::CloseCom()
{
	BOOL bSuccess = m_pPosCom->ClosePort();
	return bSuccess;
}

/************************************************************************
* 函数名:	SetDataMode
* 描  述:	设置BD2/GPS模块数据模式
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
BOOL CPosData::SetDataMode(enPOS_DATA_MODE data_mode)
{
	if( !m_pPosCom->IsPortOpen() )
		return FALSE;

	char* pModeCMD = NULL;
	m_data_mode	= data_mode;
	switch(m_data_mode)
	{
	case MODE_SGL_BD2:		//单BD2
		pModeCMD	= CMD_SGL_BD2;
		break;
	case MODE_SGL_GPS:		//单GPS
		pModeCMD	= CMD_SGL_GPS;
		break;
	case MODE_DBL_BD2:		//BD2/GPS双模, 选择处理BD2数据
	case MODE_DBL_GPS:		//BD2/GPS双模, 选择处理GPS数据
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
* 函数名:	GetDataMode
* 描  述:	获取BD2/GPS模块数据模式
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
enPOS_DATA_MODE	CPosData::GetDataMode()
{
	return m_data_mode;
}

/************************************************************************
* 函数名:	AddHWND
* 描  述:	添加窗口句柄，以获取位置信息消息
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
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
* 函数名:	RemoveHWND
* 描  述:	删除窗口句柄
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
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
* 函数名:	RemoveHWND
* 描  述:	使能转换成GPS数据
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
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
* 函数名:	RemoveHWND
* 描  述:	Disable转换GPS数据
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
void CPosData::DisableConvertGPS()
{
	if(m_pConvertCom != NULL)
		m_pConvertCom->ClosePort();
}

/************************************************************************
* 函数名:	ParseRMC
* 描  述:	解析RMC
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss(时分秒)格式
<2> 定位状态，A=有效定位，V=无效定位
<3> 纬度ddmm.mmmm(度分)格式(前面的0也将被传输)
<4> 纬度半球N(北半球)或S(南半球)
<5> 经度dddmm.mmmm(度分)格式(前面的0也将被传输)  
<6> 经度半球E(东经)或W(西经)
<7> 地面速率(000.0~999.9节，前面的0也将被传输)
<8> 地面航向(000.0~359.9度，以真北为参考基准，前面的0也将被传输)
<9> UTC日期，ddmmyy(日月年)格式
<10> 磁偏角(000.0~180.0度，前面的0也将被传输)
<11> 磁偏角方向，E(东)或W(西)
<12> 模式指示(仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效)
************************************************************************/
void CPosData::ParseRMC(PCSTR pcRMC)
{
	//test
	//pcGPRMC = "$GPRMC,101904.000,A,3110.4104,N,12123.8126,E,0.00,,220707,,,A*71";

	WORD wFieldLen	= 0;	//数据段(两个','之间的字符串)的长度
	int		nValue	= 0;	//整型数据段的值
	double	fValue	= 0;	//浮点型数据段的值

	//检查GPRMC的数据段是否为12个
	//PCSTR pcField	= ScanField(pcGPRMC, 12, wFieldLen);
	//if(NULL == pcField)
	//	return;

	//<1> UTC时间，hhmmss(时分秒)格式
	PCSTR pcField	= ScanField(pcRMC, 1, wFieldLen);
	if(NULL == pcField)
		return;
	nValue = atoi( pcField );
	g_pPosInfo->rmc_data.UtcTime.wHour		= nValue / 10000;
	g_pPosInfo->rmc_data.UtcTime.wMinute	= nValue / 100 % 100;
	g_pPosInfo->rmc_data.UtcTime.wSecond	= nValue % 100;

	//<2> 定位状态，A=有效定位，V=无效定位
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	if('A' == pcField[0])
	{
		g_pPosInfo->rmc_data.pos_state.cFix	= 1;	//定位状态
	}
	else if('V' == pcField[0])
	{
		g_pPosInfo->rmc_data.pos_state.cFix	= 0;	//定位状态
	}
	else
	{
		return;
	}

	//<3> 纬度ddmm.mmmm(度分)格式(前面的0也将被传输)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue	= atof( pcField );
	nValue	= (int)fValue / 100;	//计算度
	g_pPosInfo->rmc_data.dbLatitude	= (double)(nValue + (fValue - nValue*100)/60);//纬度 去掉10的6次方，取原值

	//<4> 纬度半球N(北半球)或S(南半球)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	if('N' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLatitudeType	= 0;
	else if('S' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLatitudeType	= 1;
	else
		g_pPosInfo->rmc_data.pos_state.cLatitudeType	= 0;

	//<5> 经度dddmm.mmmm(度分)格式(前面的0也将被传输)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue	= atof( pcField );
	nValue	= (int)fValue / 100;	//计算度
	g_pPosInfo->rmc_data.dbLongitude = (double)(nValue + (fValue - nValue*100)/60);//经度  去掉10的6次方，取原值

	//<6> 经度半球E(东经)或W(西经)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	if('E' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLongitudeType	= 0;
	else if('W' == pcField[0])
		g_pPosInfo->rmc_data.pos_state.cLongitudeType	= 1;
	else
		g_pPosInfo->rmc_data.pos_state.cLongitudeType	= 0;

	//<7> 地面速率(000.0~999.9节，前面的0也将被传输)	1节(海里) = 1.852km/h
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue	= atof( pcField );
	g_pPosInfo->rmc_data.dbSpeed = (fValue*1.852*10+0.5);	//地面速率1.851985

	//<8> 地面航向(000.0~359.9度，以真北为参考基准，前面的0也将被传输)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	//fValue	= atof( pcField ) + 0.5;	//四舍五入
	fValue	= atof( pcField );
	g_pPosInfo->rmc_data.dbAzimuth	= fValue; //方位角

	//<9> UTC日期，ddmmyy(日月年)格式
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
* 函数名:	ParseGGA
* 描  述:	解析GGA
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss（时分秒）格式
<2> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<3> 纬度半球N（北半球）或S（南半球）
<4> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<5> 经度半球E（东经）或W（西经）
<6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算
<7> 正在使用解算位置的卫星数量（00~12）（前面的0也将被传输）
<8> HDOP水平精度因子（0.5~99.9）
<9> 海拔高度（-9999.9~99999.9）
<10> 地球椭球面相对大地水准面的高度
<11> 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）
<12> 差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）
************************************************************************/
void CPosData::ParseGGA(PCSTR pcGGA)
{
	//Test
	//pcGPGGA	= "$GPGGA,101904.000,3110.4104,N,12123.8126,E,1,04,2.7,91.0,M,8.0,M,,0000*5C";

	WORD wFieldLen	= 0;	//数据段(两个','之间的字符串)的长度
	int		nValue	= 0;	//整型数据段的值
	double	fValue	= 0;	//浮点型数据段的值

	if (NULL == pcGGA)
	{
		return;
	}
	//检查GGA的数据段是否为14个
	//PCSTR pcField	= ScanField(pcGPGGA, 14, wFieldLen);
	//if(NULL == pcField)
	//	return;

	//<9> 海拔高度（-9999.9~99999.9）
	PCSTR pcField	= ScanField(pcGGA, 9, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->rmc_data.dbAltitude	= fValue;

	Convert2GPS(pcGGA);
}

/************************************************************************
* 函数名:	ParseGSA
* 描  述:	解析GSA
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
$GPGSA, <1>,<2>,<3>,<3>,,,,,<3>,<3>,<3>,<4>,<5>,<6>,<7><CR><LF>
<1>模式 ：M = 手动， A = 自动。
<2>定位型式 1 = 未定位， 2 = 二维定位， 3 = 三维定位。
<3>PRN 数字：01 至 32 表天空使用中的卫星编号，最多可接收12颗卫星信息。
<4> PDOP位置精度因子（0.5~99.9）
<5> HDOP水平精度因子（0.5~99.9）
<6> VDOP垂直精度因子（0.5~99.9）
<7> Checksum.(检查位).
************************************************************************/
void CPosData::ParseGSA(PCSTR pcGSA)
{
	//Test
	//GPGSA	= "$GPGSA,A,3,14,15,05,22,18,26,,,,,,,2.1,1.2,1.7*3D";

	WORD wFieldLen	= 0;	//数据段(两个','之间的字符串)的长度
	int		nValue	= 0;	//整型数据段的值
	double	fValue	= 0;	//浮点型数据段的值
	PCSTR pcField	= NULL;

	//移至第一个卫星编号
	pcField	= ScanField(pcGSA, 3, wFieldLen);

	//天空使用中的卫星编号
	memset(g_pPosInfo->gsa_data.wUsedID, NULL, sizeof(g_pPosInfo->gsa_data.wUsedID));
	for ( int i = 0; i < MAX_SATE_NUM; i++ )
	{
		if(NULL == pcField || 0 == wFieldLen)
		{
			g_pPosInfo->gsa_data.wSatNum	= i;	//卫星个数
			break;
		}

		g_pPosInfo->gsa_data.wUsedID[i] = atoi( pcField );
		pcField	= ScanField(pcField, 1, wFieldLen);
	}

	//PDOP位置精度因子(0.5~99.9)
	//MAX_SATE_NUM-g_pPosInfo->gsa_data.wSatNum,跳过剩余空白的卫星编号数
	pcField	= ScanField(pcField, MAX_SATE_NUM-g_pPosInfo->gsa_data.wSatNum, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->gsa_data.dbPdop = fValue;

	//HDOP水平精度因子(0.5~99.9)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->gsa_data.dbHdop = fValue;

	//VDOP垂直精度因子(0.5~99.9)
	pcField	= ScanField(pcField, 1, wFieldLen);
	if(NULL == pcField)
		return;
	fValue = atof( pcField );
	g_pPosInfo->gsa_data.dbVdop = fValue;

	Convert2GPS(pcGSA);
}

/************************************************************************
* 函数名:	ParseGSV
* 描  述:	解析GSV
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
$GPGSV, <1>,<2>,<3>,<4>,<5>,<6>,<7>,?<4>,<5>,<6>,<7>,<8><CR><LF>
<1> GSV语句的总数
<2> 本句GSV的编号
<3> 可见卫星的总数，00 至 12。
<4> 卫星编号， 01 至 32。
<5>卫星仰角， 00 至 90 度。
<6>卫星方位角， 000 至 359 度。实际值。
<7>讯号噪声比（C/No）， 00 至 99 dB；无表未接收到讯号。
<8>Checksum.(检查位).
************************************************************************/
void CPosData::ParseGSV(PCSTR pcGSV, PCSTR pcSectionGSV)
{
	//Test
	//GPGSV	= "$GPGSV,3,1,10,18,84,067,23,09,67,067,27,22,49,312,28,15,47,231,30*70
	//         $GPGSV,3,2,10,21,32,199,23,14,25,272,24,05,21,140,32,26,14,070,20*7E
	//	       $GPGSV,3,3,10,29,07,074,,30,07,163,28*7D";

	WORD  wFieldLen = 0;	//数据段(两个','之间的字符串)的长度
	int		nValue	= 0;	//整型数据段的值

	WORD	wGSVNum	= 0;	//GSV语句总数
	WORD	wGSVIndex	= 0;	//当前GSV语句序号
	//WORD	wSateNum = 0;	//视野内卫星数
	const WORD	GSV_SATE_NUM	= 4;	//一条GSV语句中最大卫星数

	//memset(g_pPosInfo->gsv_data, NULL, sizeof(g_pPosInfo->gsv_data));

	BOOL	bFirstGSV = TRUE;
	PCSTR	pcField	= pcGSV;

	Convert2GPS(pcField);

	do
	{
		//GSV语句总数
		pcField	= ScanField(pcField, 1, wFieldLen);
		if(NULL == pcField)
			return;
		if(bFirstGSV)
			wGSVNum = atoi( pcField );
		//当前GSV语句序号
		pcField	= ScanField(pcField, 1, wFieldLen);
		if(NULL == pcField)
			return;
		wGSVIndex = atoi( pcField );
		//视野内卫星数
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

			//最后一颗卫星信息
			if(j == g_pPosInfo->rmc_data.pos_state.dwSateNum)
				break;
			//卫星编号
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wSatID	= atoi( pcField );
			//卫星仰角:度
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wElevation = atoi( pcField );
			//卫星方位角:度
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wAzimuth = atoi( pcField );
			//信噪比
			pcField	= ScanField(pcField, 1, wFieldLen);
			if(NULL == pcField)
				return;
			g_pPosInfo->gsv_data[j].wSNR = atoi( pcField );
			g_pPosInfo->gsv_data[j].bAvailible	= TRUE;
		}
		//最后一条GSV语句
		if(wGSVIndex == wGSVNum)
			break;
		//移至下一条GSV语句
		pcField = strstr(pcField, pcSectionGSV);
		if(NULL == pcField)
			break;
		Convert2GPS(pcField);
		bFirstGSV	= FALSE;
	} while ( TRUE );

	//不使用memset g_pPosInfo->gsv_data
	for(int i=g_pPosInfo->rmc_data.pos_state.dwSateNum; i<MAX_SATE_NUM; i++)
		g_pPosInfo->gsv_data[i].bAvailible	= FALSE;

	return;
}

/************************************************************************
* 函数名:	Convert2GPS
* 描  述:	转换GPS
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
void CPosData::Convert2GPS(PCSTR pcSentence)
{
	if(NULL == m_pConvertCom)
		return;
	if( !m_pConvertCom->IsPortOpen() )
		return;

	//GPS模式则不需要转换
	if(MODE_SGL_GPS == m_data_mode)
		return;
	
	PCSTR pcBuffer	= pcSentence;

	//模式为MODE_DBL_GPS时，GPGSA、GPGSV数据直接拷贝到m_cGPS中
	//$GPGSA,A,3,03,06,14,21,23,19,16,32,,,,,1.3,0.9,0.9,0.7*39
	//"$GPGSV,3,3,9,32,21,242,14*73\r\n"
	if( strncmp(pcSentence, MODE_GP, strlen(MODE_GP)) == 0 )
	{
		do
		{
			m_cGPS[m_wGPSOffset++]	= *pcBuffer++;
		} while ( pcBuffer != NULL && *pcBuffer != '\n' && m_wGPSOffset<NUM_1K-1);
		m_cGPS[m_wGPSOffset++]	= *pcBuffer;	//最后一个字符为'\n'
		return;
	}

	//其它数据
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
* 函数名:	Convert2GPS
* 描  述:	转换GPS
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
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
* 函数名:	GPSErrorInfo
* 描  述:	GPS串口报错信息
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
************************************************************************/
int CPosData::ErrorInfo(LPCTSTR lpText)
{
	MessageBox(NULL, lpText, TEXT("串口出错！"), MB_OK | MB_SETFOREGROUND);
	return 0;
}

/************************************************************************
* 函数名:	CatenateGPSData
* 描  述:	拼接完整GPS数据
* 入  参:
* 出  参: 
* 返  回: 
* 备  注: 
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
* 函数名:	GPSDataParser
* 描  述:	解析NEMA-0183协议数据
* 入  参:
* 出  参: 
* 返  回: 
* 备  注:	一秒钟接收一次
单BD2（热启动）$CCSIR,1,0*49<CR><LF>
$BDRMC,075654.00,A,3109.731130,N,12123.330827,E,0.0,,250413,,,A*67
$BDGGA,075654.00,3109.731130,N,12123.330827,E,1,07,4.3,86.7,M,8.0,M,,,2.5*7E
$BDGLL,3109.731130,N,12123.330827,E,075654.00,A,0*0E
$BDGSA,A,3,06,13,08,01,09,11,03,,,,,,5.0,4.3,2.5,3.6*26
$BDGSV,2,1,7,06,02,191,25,13,10,202,33,08,58,185,26,01,49,145,32*55
$BDGSV,2,2,7,09,09,216,24,11,57,111,25,03,53,200,32*6E

单GPS（热启动）$CCSIR,2,0*4A<CR><LF> 
$GPRMC,075819.00,A,3109.717233,N,12123.341973,E,0.0,,250413,,,A*73
$GPGGA,075819.00,3109.717233,N,12123.341973,E,1,07,1.0,66.7,M,8.0,M,,,2.1*66
$GPGLL,3109.717233,N,12123.341973,E,075819.00,A,0*1A
$GPGSA,A,3,03,06,14,21,23,32,19,,,,,,2.3,1.0,2.1,0.9*31
$GPGSV,2,1,8,03,25,199,41,06,41,183,41,14,17,151,38,21,06,103,31*46
$GPGSV,2,2,8,23,25,316,33,32,21,243,35,19,02,198,41,30,66,344,26*49

BD2/GPS双模（热启动）$CCSIR,3,0*4B<CR><LF> 
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

	//判断BD2/GPS数据头
	if( (pcHead = strstr(pcPosData, MODE_BD DATA_RMC)) != NULL )		//单BD2
	{
		if(m_data_mode != MODE_SGL_BD2)
			m_data_mode = MODE_SGL_BD2;
	}
	else if( (pcHead = strstr(pcPosData, MODE_GP DATA_RMC)) != NULL )	//单GPS
	{
		if(m_data_mode != MODE_SGL_GPS)
			m_data_mode = MODE_SGL_GPS;
	}
	else if( (pcHead = strstr(pcPosData, MODE_GN DATA_RMC)) != NULL )	//BD2/GPS双模
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
	//解析RMC数据
	pcSection	= strstr(pcHead, pcSectionRMC);
	if (pcSection != NULL)
		ParseRMC(pcSection);

	//解析GGA数据
	pcSection	= strstr(pcHead, pcSectionGGA);
	if (pcSection != NULL)
		ParseGGA(pcSection);

	//转换GLL数据
	pcSection	= strstr(pcHead, DATA_GLL);
	if (pcSection != NULL)
		Convert2GPS(pcSection-3);

	//解析GSA数据
	pcSection	= strstr(pcHead, pcSectionGSA);
	if (pcSection != NULL)
		ParseGSA(pcSection);

	//解析GSV数据
	pcSection	= strstr(pcHead, pcSectionGSV);
	if (pcSection != NULL)
		ParseGSV(pcSection, pcSectionGSV);

	PostMessage(HWND_BROADCAST, m_nMsgIDPosRefresh, 0, 0);

	//转发GPS数据
	SendGPS(pcPosData, dwLen);
}
