// E9101MainDlg.h : header file
//

#if !defined(AFX_E9101MAINDLG_H__3D3A7646_846F_429E_8DEE_56C140C38004__INCLUDED_)
#define AFX_E9101MAINDLG_H__3D3A7646_846F_429E_8DEE_56C140C38004__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "DialogBase.h"
#include "MaskCtrl.h"

#include "BtnOfMainMenu.h"
#include "Dialog/DlgVehicleInfo.h"
#include "../gsm/diaodu_data.h"


// #define WM_HARD_BUTTON				(WM_USER+784)
// #define WPARAM_BUTTON1				0xF1F1
// #define WPARAM_BUTTON2				0xF2F2
// #define WPARAM_BUTTON3				0xF3F3
// #define WPARAM_BUTTON4				0xF4F4
// #define WPARAM_BUTTON5				0xF5F5
// #define WPARAM_BUTTON6				0xF6F6

#define BLUETOOTH_STATE				10243

#define TIMER_CURTIME				10240
#define TIMER_POWER_STATE			10241
#define TIMER_NAVI_EXIT_STATE		10242

//ͬ�������ͨѶ����
#define WPARAM_SMS_CENTER			1	//������Ϣ
#define WPARAM_SMS_TASK				2	//������Ԯ������Ϣ��LPARAM:����ID
#define WPARAM_TASK_START_END		12	//���ڶ���������Ϣ��LPARAM:����ID
#define WPARAM_SMS_TASK_POINT		9	//���ڶ���/��Ԯ����  �ڵ�ĵ���,�뿪

#define WPARAM_SNAP					3	//����ָ��
#define WPARAM_IP_SETTING			4	//����ip�˿�
#define WPARAM_GETIMEI_FAILURE		5	//IMEI��ȡʧ��
#define WPARAM_IC_CARD				6	//IC����Ϣ
#define WPARAM_CALL					7	//ͨ����Ϣ   LPARAM:1 ��ʼͨ��   0 ����ͨ��
#define WPARAM_BAR_CODE_CMD			8	//��������Ϣ
#define WPARAM_SERVER_LOGIN			10	//��������¼״̬	LPARAM:1 ��ͨ   0 ����
#define WPARAM_OFFLINE_REQUEST		11	//��������:����δ���ͳɹ�������
#define WPARAM_SEND_RFID_CMD		13	//RFID,��RFID���ŷ������������
#define WPARAM_CANCLE_CUT_TASK		14	//���ڶ���ͽ�����Ԯ LPARAM:1 "��ȡ��"  0 "�������ѱ���ֹ"   δ��ʼ��ȡ����ִ��������ֹ
#define WPARAM_RFID_SCAN			15  //�������ķ���RFIDѲ��ָ�LPARAM��0 ����Ѳ��   >0����ʱѲ��ʱ����(��λ����)
#define WPARAM_LOW_VIL				16	//��ѹ������ֵ
#define WPARAM_TASK_RE_REQ			18	//�����ط�ȱ�ٽڵ������

//ͬ����ͷ����ͨѶ����
#define WPARAM_CAR_REVERSE			100		//LPARAM: 1 ��ʼ����  0 ��������
#define WPARAM_START_VIDEO			101

#define PATH_CAMERA_VIDEO					_T("\\Flashdrv Storage\\Menu\\Video.exe")
#define PATH_BRIGHTNESS_VOLUMN_SETTING		_T("\\Flashdrv Storage\\Menu\\Setting.exe")
//
//����
//160��120
#define		PIXEL_160_120                 									1
//320��240
#define		PIXEL_320_240                 									2
//640��480
#define		PIXEL_640_480                 									3

//ȫ������ͷ
#define		CMR_COUNT_ALL            									0
//����ͷ1
#define		CMR_COUNT_ONE            									1
//����ͷ2
#define		CMR_COUNT_TWO            									2
//����ͷ3
#define		CMR_COUNT_THREE          									3
//����ͷ4
#define		CMR_COUNT_FOUR           									4

const int WM_TIP = 5319;
const int WM_SD  = 5519;
const int WM_KEY = 5520;

typedef enum BRD_KEYMSG_E{
     BACKLIGHT_MSG,
     VOLUME_MSG,
     PWROFF_MSG,
     MENU_MSG,
     SUSPEND_MSG,
     CAMIF_MSG,
};


	
class CE9101MainDlg : public CDialogBase
{
	enum{	BTN_COUNT = 10	};
public:


	static UINT		S_5Minute;
	static int		m_nPic;
	static BOOL		m_bPhone;	//�绰���Ź��ܿ���
	static BOOL		m_bACC_ON;		//ACC״̬
	static BOOL		m_bCanStartNavi;	//���⴮�ڹ��������ӳ�,�Ƿ��������������

	//�任���Ժ͵�λ
	BOOL OnLanguageChange();
	//��Ƥ��
	BOOL OnSkinChange();
	void OnExit();

	void DelayTime(int nSecMil);

	BOOL bTimeOver;

	//�ж��������ڳ����Ƿ����
	BOOL bVol;
	
	static void OnInitConfig();		

private:
	
	BOOL m_bIsSpare;			//�Ƿ�ʡ��ģʽ


	void ReleaseGdi();

	
public:

	ItemInfo   m_Item[10];
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP    m_stBtnPressBMP;
	HBITMAP    m_stGSMSignalBMP;
	HBITMAP    m_stIconFlagBMP;
	BOOL	   PowerTip;

	ItemInfo        m_ItemFm;
	//ItemInfo        m_ItemBt;

	ItemInfo     m_ItemGSM;
	ItemInfo	 m_ItemSMS; //����Ϣ
//	CDlgVehicleInfo* m_pDlgVehicleInfo;

	CString m_csTitle;
	CRect m_MenuRect;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont	bigfont;
	
	CDC  bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_gsm_dc;
	CDC  bk_icon_dc;

	CBitmap*  m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_gsm_bmp;
	CBitmap*  m_pold_bk_icon_bmp;

//////////////////////////////////////////////////////////////////////////

	HBITMAP    m_stBtnNormalBMP1;
	HBITMAP    m_stBtnPressBMP1;
	HBITMAP    m_stGSMSignalBMP1;
	HBITMAP    m_stIconFlagBMP1;

	HBITMAP    m_stBtnNormalBMP2;
	HBITMAP    m_stBtnPressBMP2;
	HBITMAP    m_stGSMSignalBMP2;
	HBITMAP    m_stIconFlagBMP2;


	CDC  bk_normal_dc1;
	CDC  bk_press_dc1;
	CDC  bk_gsm_dc1;
	CDC  bk_icon_dc1;

	CDC  bk_normal_dc2;
	CDC  bk_press_dc2;
	CDC  bk_gsm_dc2;
	CDC  bk_icon_dc2;

	CBitmap*  m_pold_bk_normal_bmp1;
	CBitmap*  m_pold_bk_press_bmp1;
	CBitmap*  m_pold_bk_gsm_bmp1;
	CBitmap*  m_pold_bk_icon_bmp1;

	CBitmap*  m_pold_bk_normal_bmp2;
	CBitmap*  m_pold_bk_press_bmp2;
	CBitmap*  m_pold_bk_gsm_bmp2;
	CBitmap*  m_pold_bk_icon_bmp2;
//////////////////////////////////////////////////////////////////////////

	//����״̬: ��¼������:	0 ����  1 ��ͨ  RAS����:  2 �Ͽ�    3 ����
	DWORD m_nRasState;	//RAS����:  2 �Ͽ�    3 ����
	DWORD m_nLoginServer;	//��¼������:	0 ����  1 ��ͨ

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(198,203,210),UINT Format = DT_CENTER | DT_BOTTOM,BOOL bDisable=FALSE);

public:
	CE9101MainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CE9101MainDlg)
	enum { IDD = IDD_E9101MAIN_DIALOG };
	CExStatic	m_stcBlueTooth;
	CExStatic	m_stcBattery;
	CExStatic	m_stcTime;
	CExStatic	m_stSet;
	CExStatic	m_stAdjust;
	CBtnOfMainMenu	m_BtnSysSet;
	CBtnOfMainMenu	m_BtnImage;
	CBtnOfMainMenu	m_BtnMusic;
	CBtnOfMainMenu	m_BtnMovie;
	CBtnOfMainMenu	m_BtnAdjust;
	CBtnOfMainMenu	m_Navi;
	CBtnOfMainMenu	m_BtnBlueTooth;
	CBtnOfMainMenu	m_BtnGame;
	CBtnOfMainMenu	m_BtnEbook;
	//}}AFX_DATA

	static BOOL g_Bluetooth_state;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE9101MainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
private:
	VOID	OnBtnNavi();			//��������
	VOID	OnBtnPushToTalk();		//һ��ͨ
	VOID	OnBtnVideo();			//��Ƶ����
	VOID	OnBtnMP3();				//���ֲ���
	VOID	OnBtnCamera();			//����ͷ
	VOID	OnBtnPicture();			//ͼƬ���
	VOID	OnBtnDial();			//�绰����
	VOID	OnBtnDiaoDu();			//������Ϣ
	VOID	OnBtnBook();			//�鼮�Ķ�
	VOID	OnBtnReportState();		//״̬�ϴ�
	VOID	OnBtnSysset();			//ϵͳ����
	
	VOID	DrawDataTime(CDC* pmemDC);

	VOID	OnCmdTianze(WPARAM wParam, LPARAM lParam);
	VOID	OnCmdCarReverse(WPARAM wParam, LPARAM lParam);
	VOID	OnCmdGSM(WPARAM wParam, LPARAM lParam);
	VOID	OnCmdCOPYDATA(WPARAM wParam, LPARAM lParam);
	
	VOID	WriteSMSCenter();		//������Ϣ
	VOID	WriteSMSTask();			//������Ϣ(��Ԯ��Ϣ)
	VOID	CheckSMSReadStatus();	//��������Ϣ���Ѷ�/δ��״̬
	VOID	CancelTask();			//ȡ����Ԯ������ڶ���
	VOID	CheckRFID(LPARAM lParam);
	VOID	PicSnap(WPARAM wParam, LPARAM lParam);//������������ָ��

	VOID	OnDestinationNavi(LPARAM lParam);

	void	ResetOS();


	static DWORD THDOpenRegDialog( LPVOID lpParameter );


	
// Implementation
protected:
	CDialogBase*  m_pMain;
	// Generated message map functions
	//{{AFX_MSG(CE9101MainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnBtnDlgGps();
	afx_msg void OnBtnGPRS();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBtnNewSms();
	afx_msg void OnBtnCenterDial();
	//}}AFX_MSG
	afx_msg LRESULT OnTip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildExit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSdCardMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPowerEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBlueToothStateTrans(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnBlueToothDialIn(WPARAM, LPARAM);
	afx_msg LRESULT OnBlueToothCallNumber(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPowerLowDlgDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHideWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOutGoingCall(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOffHookCall(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBlueToothConnected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRingInd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnActiveCall(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void  OnBtnSMS();
	void  LoadGSMNetType();

private:
	CString		m_strNetTip;
	UINT		m_nTimerCount;
	POWERSTAT_E m_PowerState;
	int			m_nPercent;
	bool		m_ResumeGPRS;
	BOOL		m_bEnableInput;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E9101MAINDLG_H__3D3A7646_846F_429E_8DEE_56C140C38004__INCLUDED_)
