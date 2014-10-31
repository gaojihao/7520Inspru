#pragma once

#include "../DataParser/DataType.h"
#include "../Lib/Location.h"
#define HEX2BCD(x) (((x) % 10) + ((((x) / 10) % 10) << 4))  /*20 -> 20H*/

class CMsgProc;

extern const POS_INFO*	g_pGpsPosInfo;

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

	HWND				m_hMenuMainWnd;		//menu.exe�����ھ��
	HWND				m_hGNSSMainWnd;		//GNSSTerminal.exe�����ھ��
	CWnd*				m_pGNSSMainWnd;		//�����������ָ��
	BOOL				m_bIsLogin;			//�Ƿ��¼�ɹ�

	int AuthenticateTimes; //��Ȩ�Ĵ��������ܳ�������
	

	CMsgProc(void);
	~CMsgProc(void);

public:

	//Singletonģʽʵ������
	static CMsgProc* Instance();

	//���������ھ����ָ��
	BOOL	SetMainWnd(HWND hMainWnd, CWnd* pMainWnd);
	//������Ϣ��������(GNSSTerminalDlg)
	LRESULT SendServerMsg( WPARAM wParam, LPARAM lParam, BOOL bSynchronous = TRUE );
	//������Ϣ��Menu.exe��������
	LRESULT SendMsg2Menu( WPARAM wParam, LPARAM lParam = 0, BOOL bSynchronous = FALSE );

	//��Ϣ������
	LRESULT	MsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	//������ָ�����
	LRESULT	OnServerCommand(WPARAM wParam, LPARAM lParam);

	//UI����������Ϣ����
	LRESULT	OnUIMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnGpsMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnGuider4uMsg(WPARAM wParam, LPARAM lParam);

	//����socket���ӷ�����
	BOOL	ConnectSocket();
	void	DisConnectSocket();
	int		GetSocketStatus();

	VOID    OnTerimalAuthenticate();

	//ע��
	VOID	Register();
	//����������Ϣ
	VOID	LoadConfig();
	//���õ�¼״̬
	VOID	SetLoginState(BOOL bIsLogin);
	//��ȡ��¼״̬
	BOOL	GetLoginState();
	//�����߳�
	static DWORD	ThrdHeartBeat( LPVOID lpParameter );
	//���������߳�
	VOID	BeginHeartBeat();

	BOOL	ReConnectServer();

	//������ƽ̨(SerVeR)��Ϣ������
	BOOL	OnCOMMON_ACK(enResult Result);								//2.ƽ̨ͨ��Ӧ��0x8001
	BOOL	OnSUBCONTRACT(enResult Result);								//3.�����ְ�����0x8003
	BOOL	OnREGISTER_ACK(enResult Result);							//5.�ն�ע��Ӧ��0x8100
	BOOL	OnPARAM_SET(enResult Result);								//8.�����ն˲���0x8103
	BOOL	OnPARAM_GET(enResult Result);								//9.��ѯ�ն˲���0x8104
	BOOL	OnTMN_CONTROL(enResult Result, enTMN_CONTROL enControl);	//11.�ն˿���0x8105
	BOOL	OnTMN_APPOINTPARAM(enResult Result);						//12.��ѯָ���ն˲���
	BOOL	OnTMN_ATTRIBUTE(enResult Result);							//...��ѯ�ն�����
	BOOL	OnPOS_GET(enResult Result);									//13.λ����Ϣ��ѯ0x8201
	BOOL	OnTEMP_POS_TRACE(enResult Result);							//15.��ʱλ�ø��ٿ���0x8202
	BOOL	OnTEMP_ALARM_SMS(enResult Result);							//...�˹�ȷ�ϱ�����Ϣ
	BOOL	OnTEXT_SMS(enResult Result);								//16.�ı���Ϣ�·�0x8300
	BOOL	OnEVENT_SET(enResult Result);								//17.�¼�����0x8301
	BOOL	OnQUESTION(enResult Result);								//19.�����·�0x8302
	BOOL	OnINFO_MENU_SET(enResult Result);							//21.��Ϣ�㲥�˵�����0x8303
	BOOL	OnINFO_SERVICE(enResult Result);							//23.��Ϣ����0x8304
	BOOL	OnCALL_BACK(enResult Result);								//24.�绰�ز�0x8400
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
	BOOL	OnCAMERA_PHOTOGRAPH(enResult Result);						//44.����ͷ������������0x8801
	BOOL	OnMULTIMEDIA_SEARCH(enResult Result);						//45.�洢��ý�����ݼ���0x8802
	BOOL	OnMULTIMEDIA_UPLOAD(enResult Result);						//47.�洢��ý�������ϴ�����0x8803
	BOOL	OnSOUND_RECORD(DWORD dwSoundRecording);						//48.¼����ʼ����0x8804
	BOOL	OnDATA_DOWNLOAD(enResult Result);							//49.��������͸��0x8900
	BOOL	OnRSA_PUBLIC_KEY(enResult Result);							//52.ƽ̨RSA��Կ0x8A00
	BOOL	OnDESTINATION_NAVI(DWORD dwLastUID);						//��2.ƽ̨�·�"Ŀ�ĵص���"ָ��0xFF00
	BOOL    OnAudioUpload(enResult Result);
};
