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

//同天泽进程通讯参数
#define WPARAM_SMS_CENTER			1	//中心信息
#define WPARAM_SMS_TASK				2	//紧急救援任务信息：LPARAM:任务ID
#define WPARAM_TASK_START_END		12	//定期定点任务信息：LPARAM:任务ID
#define WPARAM_SMS_TASK_POINT		9	//定期定点/救援任务  节点的到达,离开

#define WPARAM_SNAP					3	//拍照指令
#define WPARAM_IP_SETTING			4	//设置ip端口
#define WPARAM_GETIMEI_FAILURE		5	//IMEI获取失败
#define WPARAM_IC_CARD				6	//IC卡信息
#define WPARAM_CALL					7	//通话消息   LPARAM:1 开始通话   0 结束通话
#define WPARAM_BAR_CODE_CMD			8	//条形码消息
#define WPARAM_SERVER_LOGIN			10	//服务器登录状态	LPARAM:1 接通   0 掉线
#define WPARAM_OFFLINE_REQUEST		11	//离线请求:发送未发送成功的数据
#define WPARAM_SEND_RFID_CMD		13	//RFID,将RFID卡号发送至天泽进程
#define WPARAM_CANCLE_CUT_TASK		14	//定期定点和紧急救援 LPARAM:1 "已取消"  0 "该任务已被终止"   未开始则取消，执行中则终止
#define WPARAM_RFID_SCAN			15  //服务中心发送RFID巡检指令：LPARAM：0 立即巡检   >0：定时巡检时间间隔(单位：秒)
#define WPARAM_LOW_VIL				16	//低压报警阀值
#define WPARAM_TASK_RE_REQ			18	//请求重发缺少节点的任务

//同摄像头进程通讯参数
#define WPARAM_CAR_REVERSE			100		//LPARAM: 1 开始倒车  0 结束倒车
#define WPARAM_START_VIDEO			101

#define PATH_CAMERA_VIDEO					_T("\\Flashdrv Storage\\Menu\\Video.exe")
#define PATH_BRIGHTNESS_VOLUMN_SETTING		_T("\\Flashdrv Storage\\Menu\\Setting.exe")
//
//像素
//160×120
#define		PIXEL_160_120                 									1
//320×240
#define		PIXEL_320_240                 									2
//640×480
#define		PIXEL_640_480                 									3

//全部摄像头
#define		CMR_COUNT_ALL            									0
//摄像头1
#define		CMR_COUNT_ONE            									1
//摄像头2
#define		CMR_COUNT_TWO            									2
//摄像头3
#define		CMR_COUNT_THREE          									3
//摄像头4
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
	static BOOL		m_bPhone;	//电话短信功能开关
	static BOOL		m_bACC_ON;		//ACC状态
	static BOOL		m_bCanStartNavi;	//虚拟串口工具启动延迟,是否能启动导航软件

	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	void OnExit();

	void DelayTime(int nSecMil);

	BOOL bTimeOver;

	//判断音量调节程序是否存在
	BOOL bVol;
	
	static void OnInitConfig();		

private:
	
	BOOL m_bIsSpare;			//是否省电模式


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
	ItemInfo	 m_ItemSMS; //短消息
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

	//网络状态: 登录服务器:	0 掉线  1 接通  RAS拨号:  2 断开    3 连接
	DWORD m_nRasState;	//RAS拨号:  2 断开    3 连接
	DWORD m_nLoginServer;	//登录服务器:	0 掉线  1 接通

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
	VOID	OnBtnNavi();			//汽车导航
	VOID	OnBtnPushToTalk();		//一键通
	VOID	OnBtnVideo();			//视频播放
	VOID	OnBtnMP3();				//音乐播放
	VOID	OnBtnCamera();			//摄像头
	VOID	OnBtnPicture();			//图片浏览
	VOID	OnBtnDial();			//电话短信
	VOID	OnBtnDiaoDu();			//调度信息
	VOID	OnBtnBook();			//书籍阅读
	VOID	OnBtnReportState();		//状态上传
	VOID	OnBtnSysset();			//系统设置
	
	VOID	DrawDataTime(CDC* pmemDC);

	VOID	OnCmdTianze(WPARAM wParam, LPARAM lParam);
	VOID	OnCmdCarReverse(WPARAM wParam, LPARAM lParam);
	VOID	OnCmdGSM(WPARAM wParam, LPARAM lParam);
	VOID	OnCmdCOPYDATA(WPARAM wParam, LPARAM lParam);
	
	VOID	WriteSMSCenter();		//中心信息
	VOID	WriteSMSTask();			//任务信息(救援信息)
	VOID	CheckSMSReadStatus();	//检查调度信息的已读/未读状态
	VOID	CancelTask();			//取消救援任务或定期定点
	VOID	CheckRFID(LPARAM lParam);
	VOID	PicSnap(WPARAM wParam, LPARAM lParam);//服务中心拍照指令

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
