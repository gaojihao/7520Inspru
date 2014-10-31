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
	static CPosData*		m_pInstance;			//Singletonģʽʵ��ָ��
	CSerialCom*				m_pPosCom;				//BD2/GPS����
	CSerialCom*				m_pConvertCom;			//ת��GPS����
	WORD					m_wBaudRate;			//���ڲ�����
	POS_INFO				m_pos_info;				//λ����Ϣ
	enPOS_DATA_MODE			m_data_mode;			//��BD2	��GPS  BD2/GPS˫ģ
	UINT					m_nMsgIDPosRefresh;		//ģ��λ�����ݸ�����ϢID
	HWND					m_hDestWnd[10];
	char					m_cGPS[NUM_1K];
	WORD					m_wGPSOffset;

public:
	CPosData(void);
	~CPosData(void);

	//Singletonģʽʵ������
	static CPosData* Instance();
	//���GPS����
	BOOL	InitPosCom(WORD wPosCom, WORD wBaudRate, enPOS_DATA_MODE data_mode);
	//�ر�GPS����
	BOOL	CloseCom();
	//����BD2/GPSģ������ģʽ
	BOOL	SetDataMode(enPOS_DATA_MODE data_mode);
	//��ȡBD2/GPSģ������ģʽ
	enPOS_DATA_MODE	GetDataMode();
	//��Ӵ��ھ�����Ի�ȡλ����Ϣ��Ϣ
//	BOOL	AddHWND(HWND hWnd);
	//ɾ�����ھ��
//	void	RemoveHWND(HWND hWnd);
	//ʹ��ת��GPS����
	BOOL	EnableConvertGPS(WORD wConvertCom);
	//Disableת��GPS����
	void	DisableConvertGPS();

private:
	//GPS���ڱ�����Ϣ
	static int		ErrorInfo(LPCTSTR lpText);
	//ƴ������GPS����
	static void		CatenatePosData(BYTE *pcBuffer, DWORD dwLen);
	//����BD2/GPSЭ������
	void	PosDataParser(PCSTR pcPosData, DWORD dwLen);
	//����Pos����
	void	ParseRMC(PCSTR pcRMC);
	void	ParseGGA(PCSTR pcGGA);
	void    ParseGSA(PCSTR pcGSA);
	void	ParseGSV(PCSTR pcGSV, PCSTR pcSectionGSV);
	//ת��GPS
	void	Convert2GPS(PCSTR pcSentence);
	void	SendGPS(PCSTR pcPosData, DWORD dwLen);
};
