#pragma once

//#include "../DataParser/DataType.h"

class CMsgProc;

//����Ϣ�ṹ��
typedef struct _MSG_PROC
{
	UINT		nMsgID;
	LRESULT		(CMsgProc::*pFunc)(WPARAM, LPARAM);
} MSG_PROC, *PMSG_PROC;

class CMsgProc
{
private:
	static CMsgProc*	m_pInstance;		//Singletonģʽʵ��ָ��

	HWND				m_hMainWnd;			//menu.exe�����ھ��
	CWnd*				m_pMainWnd;			//menu.exe�����������ָ��

public:
	CMsgProc(void);
	~CMsgProc(void);

	//Singletonģʽʵ������
	static CMsgProc* Instance();

	//���������ھ����ָ��
	BOOL	SetMainWnd(HWND hMainWnd, CWnd* pMainWnd);

	//��Ϣ������
	LRESULT	MsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	//GNSSTERMINAL.exe���̷���������Ϣ����
	LRESULT	OnGNSSTerminal(WPARAM wParam, LPARAM lParam);
	//Ӳ������Ϣ����
	LRESULT	OnHardButton(WPARAM wParam, LPARAM lParam);

	//�����߳�
	//static DWORD	ThrdHeartBeat( LPVOID lpParameter );
	//���������߳�
	//VOID	BeginHeartBeat();

	//������ƽ̨(SerVeR)��Ϣ������
	BOOL	OnTEXT_SMS();												//16.�ı���Ϣ�·�0x8300
	BOOL	OnDESTINATION_NAVI(DWORD dwLastUID);						//��2.ƽ̨�·�"Ŀ�ĵص���"ָ��0x8F01
	BOOL	OnCOMMON_ACK(DWORD dwAck_result);							//ͨ��Ӧ��
	/*
	BOOL	OnREGISTER_ACK(enResult Result);							//5.�ն�ע��Ӧ��0x8100
	BOOL	OnPARAM_SET(enResult Result);								//8.�����ն˲���0x8103
	BOOL	OnPARAM_GET(enResult Result);								//9.��ѯ�ն˲���0x8104
	BOOL	OnTMN_CONTROL(enResult Result, enTMN_CONTROL enControl);	//11.�ն˿���0x8105
	BOOL	OnPOS_GET(enResult Result);									//13.λ����Ϣ��ѯ0x8201
	BOOL	OnTEMP_POS_TRACE(enResult Result);							//15.��ʱλ�ø��ٿ���0x8202
*/	
	BOOL	OnEVENT_SET(DWORD dwLastUID/*enResult Result*/);			//17.�¼�����0x8301
	BOOL	OnQUESTION(DWORD dwLastUID/*enResult Result*/);				//19.�����·�0x8302
	BOOL	OnINFO_MENU_SET(DWORD dwLastUID/*enResult Result*/);		//21.��Ϣ�㲥�˵�����0x8303
/*	BOOL	OnINFO_SERVICE(enResult Result);							//23.��Ϣ����0x8304
	
	BOOL	OnPHONE_BOOK_SET(enResult Result);							//25.���õ绰��0x8401
	BOOL	OnCAR_CONTROL(BYTE cControlFlag);							//26.��������0x8500
	BOOL	OnCIRC_REGION_SET(enResult Result);							//28.����Բ������0x8600
	BOOL	OnCIRC_REGION_DEL(enResult Result);							//29.ɾ��Բ������0x8601
	BOOL	OnRECT_REGION_SET(enResult Result);							//30.���þ�������0x8602
	BOOL	OnRECT_REGION_DEL(enResult Result);							//31.ɾ����������0x8603
	BOOL	OnNGON_REGION_SET(enResult Result);							//32.���ö��������0x8604
	BOOL	OnNGON_REGION_DEL(enResult Result);							//33.ɾ�����������0x8605
	BOOL	OnROUTE_SET(enResult Result);								//34.����·��0x8606
	BOOL	OnROUTE_DEL(enResult Result);								//35.ɾ��·��0x8607
	BOOL	OnDRIVE_RECORD_COLLECT(BYTE cCMD);							//36.��ʻ��¼���ݲɼ�����0x8700
	BOOL	OnDRIVE_RECORD_PARAM_SET(BYTE cCMD);						//38.��ʻ��¼�����´�����0x8701
	BOOL	OnMULTIMEDIA_UPLOAD_ACK(enResult Result);					//43.��ý�������ϴ�Ӧ��0x8800
*/	BOOL	OnCAMERA_PHOTOGRAPH(DWORD dwLastUID/*enResult Result*/);	//44.����ͷ������������0x8801
/*	BOOL	OnMULTIMEDIA_SEARCH(enResult Result);						//45.�洢��ý�����ݼ���0x8802
	BOOL	OnMULTIMEDIA_UPLOAD(enResult Result);						//47.�洢��ý�������ϴ�����0x8803
	BOOL	OnSOUND_RECORD(DWORD dwSoundRecording);						//48.¼����ʼ����0x8804
	BOOL	OnDATA_DOWNLOAD(enResult Result);							//49.��������͸��0x8900
	BOOL	OnRSA_PUBLIC_KEY(enResult Result);							//52.ƽ̨RSA��Կ0x8A00
	*/

	BOOL	OnCALL_BACK(BYTE byType);								//24.�绰�ز�0x8400
};
