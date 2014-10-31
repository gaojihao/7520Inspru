#pragma once

#include "SerialCom.h"
#include "MsgBase.h"
#include "../Location.h"

#define MSG_BUFFER_LEN					1280
#define NUM_256B						256
#define NUM_512B						512
#define NUM_1K							1024
#define NUM_2K							(1024*2)
#define NUM_3K							(1024*3)
#define NUM_4K							(1024*4)

class CPosData : public CMsgBase
{
private:
	static CPosData*		m_pInstance;			//Singleton模式实例指针
	CSerialCom*				m_pPosCom;				//BD2/GPS串口
	CSerialCom*				m_pConvertCom;			//转换GPS串口
	WORD					m_wBaudRate;			//串口波特率
	POS_INFO				m_pos_info;				//位置信息
	enPOS_DATA_MODE			m_data_mode;			//单BD2	单GPS  BD2/GPS双模
	UINT					m_nMsgIDPosRefresh;		//模块位置数据更新消息ID
	HWND					m_hDestWnd[10];
	char					m_cGPS[NUM_1K];
	WORD					m_wGPSOffset;

public:
	CPosData(void);
	~CPosData(void);

	//Singleton模式实例函数
	static CPosData* Instance();
	//监控GPS串口
	BOOL	InitPosCom(WORD wPosCom, WORD wBaudRate, enPOS_DATA_MODE data_mode);
	//关闭GPS串口
	BOOL	CloseCom();
	//设置BD2/GPS模块数据模式
	BOOL	SetDataMode(enPOS_DATA_MODE data_mode);
	//获取BD2/GPS模块数据模式
	enPOS_DATA_MODE	GetDataMode();
	//添加窗口句柄，以获取位置信息消息
//	BOOL	AddHWND(HWND hWnd);
	//删除窗口句柄
//	void	RemoveHWND(HWND hWnd);
	//使能转换GPS数据
	BOOL	EnableConvertGPS(WORD wConvertCom);
	//Disable转换GPS数据
	void	DisableConvertGPS();

private:
	//GPS串口报错信息
	static int		ErrorInfo(LPCTSTR lpText);
	//拼接完整GPS数据
	static void		CatenatePosData(BYTE *pcBuffer, DWORD dwLen);
	//解析BD2/GPS协议数据
	void	PosDataParser(PCSTR pcPosData, DWORD dwLen);
	//解析Pos数据
	void	ParseRMC(PCSTR pcRMC);
	void	ParseGGA(PCSTR pcGGA);
	void    ParseGSA(PCSTR pcGSA);
	void	ParseGSV(PCSTR pcGSV, PCSTR pcSectionGSV);
	//转换GPS
	void	Convert2GPS(PCSTR pcSentence);
	void	SendGPS(PCSTR pcPosData, DWORD dwLen);
};
