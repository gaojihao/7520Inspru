// DlgDiaIn.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgDiaIn.h"
//#include "../DriveLib/ak4642api.h"
#include "SysConfig.h"
#include "..\BlueToothPhone\HBCP.h"
#include "..\BlueToothPhone\Command.h"
#include "E9101MainDlg.h"
#include "PhoneLogMng.h"
#include "PhoneBookMng.h"
#include "PhoneInfoMng.h"
#include "DlgDialNumber.h"
#include "../imm/InputShl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDiaIn dialog

extern int g_VolLev;

HWND g_hDialIn;
CDlgDialNumber* g_DialNum = NULL;

#define TIMER_RINGIND  100
#define TIMER_AUTO_ACCEPT  101
extern UINT g_nDialIn;
CString CDlgDiaIn::m_strNumber = _T("");
CString CDlgDiaIn::m_strName = _T("");
CString CDlgDiaIn::m_strCityr = _T("");
BOOL  CDlgDiaIn::m_bIsAcc = FALSE;
extern UINT g_PhoneDialOut;

extern BOOL g_bLight;

CDlgDiaIn::CDlgDiaIn(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgDiaIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDiaIn)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bExit = FALSE;
	m_EnablePress = TRUE;
}


// void CDlgDiaIn::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialogBase:DoDataExchange(pDX);
// 	//{{AFX_DATA_MAP(CDlgDiaIn)
// //	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
// //	DDX_Control(pDX, IDC_BUTTON_REJECT, m_btnReject);
// //	DDX_Control(pDX, IDC_BUTTON_ACCEPT, m_btnAccept);
// //	DDX_Control(pDX, IDC_BTN_VOLUP, m_btnVolUp);
// //	DDX_Control(pDX, IDC_BTN_VOLDOWN, m_btnVolDown);
// //	DDX_Control(pDX, IDC_STATIC_NUM, m_stcNum);
// 	//}}AFX_DATA_MAP
//}


BEGIN_MESSAGE_MAP(CDlgDiaIn, CDialogBase)
//{{AFX_MSG_MAP(CDlgDiaIn)
ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnButtonAccept)
ON_BN_CLICKED(IDC_BUTTON_REJECT, OnButtonReject)
ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
ON_WM_PAINT()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BTN_VOLUP, OnBtnVolup)
ON_BN_CLICKED(IDC_BTN_VOLDOWN, OnBtnVoldown)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
//ON_MESSAGE(WM_HBCPCALLER_NUMBER, OnBlueToothCallNumber)
ON_MESSAGE(WM_HBCPRINGIND, OnBlueToothRingInd)
ON_MESSAGE(WM_HBCPOFFHOOK, OnBlueToothOffHook)
ON_MESSAGE(WM_HBCPACTIVECALL, OnBlueToothActive)
ON_MESSAGE(WM_IMM_DIAL_NUMBER_MESSAGE, OnDialNum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiaIn message handlers

void CDlgDiaIn::OnButtonAccept() 
{
	// TODO: Add your control notification handler code here
	//::PostMessage(HWND_BROADCAST,g_nDialIn,0,0);
	
	// 	switch(m_DialInState)
	// 	{
	// 		case Start:
	//			{
	//				ak4642_linein_enable();
				if(m_bIsAcc)
				{
					g_PhoneDialOut = 2;
					char strnum[MAX_PATH];
					wcstombs(strnum, (LPCTSTR)m_strNumber, m_strNumber.GetLength());
					strnum[m_strNumber.GetLength()] = '\0';
					HBCP_Dial_Number(strlen(strnum), (unsigned char *)strnum);
				}
				else
					HBCP_Call_Accept();
				CResString ResStr;
				ResStr.LoadString(RES_BLUETOOTH_END);
				m_Item[0].nState = BTN_STATE_DISABLE;
				
				//				m_btnReject.SetWindowText(ResStr);
				//				m_btnReject.Invalidate(FALSE);
				m_DialInState = Accept_Call;
				//				m_btnAccept.EnableWindow(FALSE);
				// 			}
				// 			break;
				// 		default:
				// 			break;
				// 	}
				Invalidate();
				if(m_strNumber.IsEmpty() == FALSE)
				{
					CDialLog plog;
					CPhoneBookFile *pPhoneBook; 
					
					PhoneLogMng::Instance().LoadAll();
					plog.m_csNum = m_strNumber;
					if(m_bIsAcc)
						plog.m_iLogFlag = 1;//call out
					else
						plog.m_iLogFlag = 2;//call accept
					plog.m_csCity = m_pItemCity.chChar;
					
					PhoneBookMng::Instance().Load();
					
					UINT Count = PhoneBookMng::Instance().GetCount();
					
					for(UINT i=0; i<Count; i++)
					{
						pPhoneBook = PhoneBookMng::Instance().GetPhoneBookItem(i);
						
						if(pPhoneBook->m_csNum == plog.m_csNum)
						{
							plog.m_csName = pPhoneBook->m_csName;
							plog.m_csNum = pPhoneBook->m_csNum;
							plog.m_csCity= pPhoneBook->m_csCity;
						}
					}
					
					PhoneLogMng::Instance().Delete(&plog);
					PhoneLogMng::Instance().AddLog(&plog); 
					PhoneLogMng::Instance().Write();
				}
}

BOOL CDlgDiaIn::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	CUiMng::Instance().Detach(this);
	
	g_hDialIn = m_hWnd;
	m_pItemCity.chChar = m_strCityr;
	m_DialExtNum = FALSE;

	
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, DLG_WIDTH, DLG_HIGHT, SWP_SHOWWINDOW);
	m_DialInState = Start;
	
	m_pItemCity.chChar = PhoneInfoMng::Instance().FindPhoneNum( m_strNumber );
	m_strCityr = m_pItemCity.chChar;
	
	m_Info += m_strCityr;
	
	
	//	ak4642_linein_enable();	
	SetTimer(TIMER_AUTO_ACCEPT, 1000, NULL);
	SetTimer(1031,500,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiaIn::OnButtonReject() 
{
	// TODO: Add your control notification handler code here
	switch(m_DialInState)
	{
	case Start:
		HBCP_Call_Reject();
		m_DialInState = Reject_Call;
		
		break;
	case Accept_Call:
		HBCP_End_Call();
		// m_DialInState = End_Call;
		
		break;
	default:
		break;
	}
	HBCP_End_Call();
	
	if(m_strNumber.IsEmpty() == FALSE)
	{
		CDialLog plog;
		CPhoneBookFile *pPhoneBook = NULL; 
		
		if( !m_bIsAcc )
		{
			if(m_DialInState != Accept_Call)
			{
				PhoneLogMng::Instance().LoadAll();
				plog.m_csNum = m_strNumber;
				plog.m_iLogFlag = 3;// no call  accept
				
				PhoneBookMng::Instance().Load();
				
				UINT Count = PhoneBookMng::Instance().GetCount();
				
				BOOL bIsPB = FALSE;
				for(UINT i=0; i<Count; i++)
				{
					pPhoneBook = PhoneBookMng::Instance().GetPhoneBookItem(i);
					
					if(pPhoneBook->m_csNum == plog.m_csNum)
					{
						plog.m_csName = pPhoneBook->m_csName;
						plog.m_csNum = pPhoneBook->m_csNum;
						plog.m_csCity= pPhoneBook->m_csCity;
						
						bIsPB = TRUE;
					}
				}
				
				if( !bIsPB )
				{
					plog.m_csName = _T("");
					plog.m_csNum = m_strNumber;
					plog.m_csCity = PhoneInfoMng::Instance().FindPhoneNum( m_strNumber );
				}
				
				PhoneLogMng::Instance().Delete(&plog);
				PhoneLogMng::Instance().AddLog(&plog); 
				PhoneLogMng::Instance().Write();
			}
			
		}
		
	}
	OnExit();
}

void CDlgDiaIn::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	//CDialogBase::EndDialog(0);
	
	switch(m_DialInState)
	{
	case Start:
		HBCP_Call_Reject();
		break;
	case Accept_Call:
		HBCP_End_Call();
		m_DialInState = End_Call;
		break;
	default:
		break;
	}
	
	if(CSysConfig::Instance().GetAutoBkLight())
	{
		g_bLight = TRUE;
	}
	
	//  		AfxBeginThread(	ak4642Disable,
	//  			NULL,
	//  			THREAD_PRIORITY_BELOW_NORMAL,
	//			CREATE_SUSPENDED);
	// 
	
	// 	if( CSysConfig::Instance().GetIsSpareMode() )
	// 	{
	// 		ns73_power( 1 );
	//   		ns73_init ();
	// 		FreqSet( CSysConfig::Instance().GetFmFreq() );
	//	}
	
	OnExit();
}

BOOL CDlgDiaIn::OnLanguageChange()
{
	
	CResString str;
	//	m_btnExit.InvalidateRect(NULL,FALSE);
	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
	
	
	if( CE9101MainDlg::g_Bluetooth_state )
		str.LoadString(RES_BT_STATE_CONNECTED);
	else
		str.LoadString(RES_BT_STATE_DISCONNECTED);
	
	//	SetWindowText(str);
	m_stTitle = str;
	
	if(m_bIsAcc)
		str.LoadString(RES_BLUETOOTH_ACCEPT_ISACC);
	else
		str.LoadString(RES_BLUETOOTH_ACCEPT);
	//	SetDlgItemText(IDC_BUTTON_ACCEPT, str);
	m_Item[0].chChar = str;
	
	if(m_bIsAcc)
		str.LoadString(RES_BLUETOOTH_REJECT_ISACC);
	else
		str.LoadString(RES_BLUETOOTH_REJECT);
	//	SetDlgItemText(IDC_BUTTON_REJECT, str);
	m_Item[1].chChar = str;
	
	str.LoadString( RES_BUTTON_PHONE_CITY ) ;
	m_Info = str;
	
	return TRUE;
}

BOOL CDlgDiaIn::OnSkinChange()
{
	// TODO: Add extra initialization here
	//	SkinSetDlgStyle(KDS_CTRL_FKTEXT|KDS_BTN_AUTOBMP);
	//	SkinSetTitlePos(CPoint(180,6));
	//	SkinSetResID(bmp_bg_mainmenu);
	
    // bmp_file_btn_OutA,
	// 	m_btnAccept.SetWindowPos(&CWnd::wndTop,108,100,0,0,NULL);
	// 	m_btnAccept.SettextLeft(13);
	//     m_btnAccept.SetBtnFontID(SF_BUTTON_NORMAL_CHINESE);
	// 	m_btnAccept.SetBtnResID(bmp_btn_accept);
	// 	
	// 	m_btnReject.SetWindowPos(&CWnd::wndTop,108,200,0,0,NULL);
	// 	m_btnReject.SettextLeft(13);
	// 	m_btnReject.SetBtnResID(bmp_btn_reject);
	// 	m_btnReject.SetBtnFontID(SF_BUTTON_NORMAL_CHINESE);
	// 
	// 	m_btnExit.SetWindowPos(&CWnd::wndTop,0,0,0,0,NULL);
	// 	m_btnExit.SetBtnResID(bmp_file_back);
	// 
	// 	m_btnVolUp.SetWindowPos(&CWnd::wndTop,300,35,0,0,NULL); 
	// 	m_btnVolDown.SetWindowPos(&CWnd::wndTop,380,35,0,0,NULL); 
	// 	m_btnVolUp.SetBtnResID(bmp_calcu_btn_ADD);
	// 	m_btnVolDown.SetBtnResID(bmp_calcu_btn_SUB);
	// 
	// 	m_stcNum.MoveWindow(35,35,280,40); 
	//	m_stcNum.SkinSetStcID(STC_TEXT_SINGLE,SKIN_ID_NONE,bmp_bg_mainmenu);
	//	m_stcNum.SkinSetTxtAttri(SF_BUTTON_BIG_CHINESE,sc_ZoneWords);
	
	//	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, DLG_WIDTH, DLG_HIGHT, SWP_SHOWWINDOW);
	
	return TRUE;
}

LRESULT CDlgDiaIn::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// 	if( message == WM_CREATE_SET )
	// 	{
	// 		if( wParam == 0x03 )
	// 		{
	// 			EndDialog(0);
	// 		}
	//	}
	
	if( message == BLUETOOTH_STATE )
	{
		CResString str;
		
		if( (int)wParam == 1 )
		{
			str.LoadString(RES_BT_STATE_CONNECTED);
			SetWindowText(str);
			Invalidate();
		}
		else if( (int)wParam == 0 )
		{
			str.LoadString(RES_BT_STATE_DISCONNECTED);	
			SetWindowText(str);
			Invalidate();
			
		}
		
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}



LRESULT CDlgDiaIn::OnBlueToothRingInd(WPARAM wParam, LPARAM lParam)
{
	//	CString str;
	//	GetWindowText(str);
	//	if(str.IsEmpty() == TRUE)
	//	{
	// 		if(m_strNumber.IsEmpty() == FALSE)
	// 		{
	// 			SetWindowText( m_strNumber);
	// 			::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
	//		}
	if(m_strNumber.IsEmpty() == FALSE)
	{
		Invalidate();
		
		//	m_stcNum.SetWindowText( m_strNumber);
		//	m_stcNum.Invalidate();
		//	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
		//	::RedrawWindow(m_stcNum.GetSafeHwnd(), NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
	}
	
	
	// 	if(m_strNumber.IsEmpty() != TRUE)
	//		m_stcNum.SetWindowText(m_strNumber);	
	//	}
	
	//	if(m_DialInState == Start)
	//	{
	//		m_bExit = FALSE;
	//		m_DialInState = Accept_Call;
	//		KillTimer(TIMER_RINGIND);
	//		SetTimer(TIMER_RINGIND, 5000, NULL);
	//	}
	return 0;
}

LRESULT CDlgDiaIn::OnBlueToothOffHook(WPARAM wParam, LPARAM lParam)
{
	// 	if(m_DialInState == Accept_Call)
	//	{
	//		ak4642_linein_disable();
	
	if(m_strNumber.IsEmpty() == FALSE)
	{
		CDialLog plog;
		CPhoneBookFile *pPhoneBook = NULL; 
		
		if( !m_bIsAcc )
		{
			PhoneLogMng::Instance().LoadAll();
			plog.m_csNum = m_strNumber;
			if(m_Item[0].nState == BTN_STATE_DISABLE)
				plog.m_iLogFlag = 2;// call  accept
			else
				plog.m_iLogFlag = 3;// no call  accept
			
			PhoneBookMng::Instance().Load();
			
			UINT Count = PhoneBookMng::Instance().GetCount();
			
			BOOL bIsPB = FALSE;
			for(UINT i=0; i<Count; i++)
			{
				pPhoneBook = PhoneBookMng::Instance().GetPhoneBookItem(i);
				
				if(pPhoneBook->m_csNum == plog.m_csNum)
				{
					plog.m_csName = pPhoneBook->m_csName;
					plog.m_csNum = pPhoneBook->m_csNum;
					plog.m_csCity= pPhoneBook->m_csCity;
					
					bIsPB = TRUE;
				}
			}
			
			if( !bIsPB )
			{
				plog.m_csName = _T("");
				plog.m_csNum = m_strNumber;
				plog.m_csCity = PhoneInfoMng::Instance().FindPhoneNum( m_strNumber );
			}
			
			PhoneLogMng::Instance().Delete(&plog);	
			PhoneLogMng::Instance().AddLog(&plog); 
			PhoneLogMng::Instance().Write();
		}
	}
	
	m_DialExtNum = FALSE;
	if(g_DialNum != NULL)
	{
		g_DialNum->OnExit();
		m_DialExtNum = TRUE;
	}
	OnBtnExit();
	return 0;
}

LRESULT CDlgDiaIn::OnBlueToothActive(WPARAM wParam, LPARAM lParam)
{
	m_DialInState = Accept_Call;
	CDialLog plog;
	CPhoneBookFile *pPhoneBook = NULL; 
	
	if( !m_bIsAcc )
	{
		PhoneLogMng::Instance().LoadAll();
		plog.m_csNum = m_strNumber;
		plog.m_iLogFlag = 2;// call  accept
		PhoneBookMng::Instance().Load();
		UINT Count = PhoneBookMng::Instance().GetCount();
		BOOL bIsPB = FALSE;
		for(UINT i=0; i<Count; i++)
		{
			pPhoneBook = PhoneBookMng::Instance().GetPhoneBookItem(i);
			
			if(pPhoneBook->m_csNum == plog.m_csNum)
			{
				plog.m_csName = pPhoneBook->m_csName;
				plog.m_csNum = pPhoneBook->m_csNum;
				plog.m_csCity= pPhoneBook->m_csCity;
				
				bIsPB = TRUE;
			}
		}
		
		if( !bIsPB )
		{
			plog.m_csName = _T("");
			plog.m_csNum = m_strNumber;
			plog.m_csCity = PhoneInfoMng::Instance().FindPhoneNum( m_strNumber );
		}
		PhoneLogMng::Instance().Delete(&plog);	
		PhoneLogMng::Instance().AddLog(&plog); 
		PhoneLogMng::Instance().Write();
	}
	m_Item[0].nState = BTN_STATE_DISABLE;
	Invalidate();
	return 0;
}
void CDlgDiaIn::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_RINGIND)
	{
		m_bExit = TRUE;
		if (m_DialInState == Start)
		{
			OnButtonAccept();
			
			
			
			// 			if( CSysConfig::Instance().GetIsSpareMode() )
			// 			{
			// 				ns73_power( 1 );
			//   				ns73_init ();
			// 				FreqSet( CSysConfig::Instance().GetFmFreq() );
			//			}
			//  		AfxBeginThread(	ak4642Disable,
			//  			NULL,
			//  			THREAD_PRIORITY_BELOW_NORMAL,
			//			CREATE_SUSPENDED);
			OnExit();
		}
	}
	else if( 1032 == nIDEvent )
	{
		KillTimer(1032);
		m_EnablePress = TRUE;
		//		m_btnVolUp.EnableWindow(TRUE);
		//		m_btnVolDown.EnableWindow(TRUE);
		//m_Item[2].nState = BTN_STATE_NORMAL;
		//m_Item[3].nState = BTN_STATE_NORMAL;
		//Invalidate();
		
	}
	else if( TIMER_AUTO_ACCEPT == nIDEvent )
	{
		
		KillTimer(TIMER_AUTO_ACCEPT);
		if( !m_bIsAcc &&CSysConfig::Instance().GetAutoAccept() && m_DialInState == Start)	
			OnButtonAccept();
		else if( CDlgDiaIn::m_bIsAcc && CDlgDiaIn::m_strNumber != _T("***********"))
			OnButtonAccept();
		else
		{
			if(m_bIsAcc)
				m_Item[0].nState = BTN_STATE_DISABLE;
			else
				m_Item[0].nState = BTN_STATE_NORMAL;
			Invalidate();
		}
	}
	else if( 1031 == nIDEvent )
	{
		Invalidate();
	}
	CDialogBase::OnTimer(nIDEvent);
}


void CDlgDiaIn::OnBtnVolup() 
{
	// TODO: Add your control notification handler code here
	//	m_btnVolUp.EnableWindow(FALSE);
	
	if(m_EnablePress)
	{
		m_EnablePress = FALSE;
		//m_Item[15].nState = BTN_STATE_DISABLE;
		SetTimer(1032,500,NULL);
		HBCP_Volume_Up();
	}
	
}

void CDlgDiaIn::OnBtnVoldown() 
{
	// TODO: Add your control notification handler code here
	//	m_btnVolDown.EnableWindow(FALSE);
	if(m_EnablePress)
	{
		m_EnablePress = FALSE;
		//m_Item[16].nState = BTN_STATE_DISABLE;
		SetTimer(1032,500,NULL);
		HBCP_Volume_Down();
	}
	
	
}

void CDlgDiaIn::OnExit()
{
	g_PhoneDialOut = 1;
	
	//bk_dis_dc.SelectObject(m_pold_bk_dis_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	//DeleteObject(m_stBtnDisBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	//bk_dis_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	midfont.DeleteObject();
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
	
	KillTimer(1031);
	EndDialog(0);
	::PostMessage(HWND_BROADCAST, g_nDialIn, 0, 0);
}


void CDlgDiaIn::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
	{
		smallfont.CreateFont(
			-12,					    // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			ANTIALIASED_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体\0")); 
		
		
	}
	else
	{
		smallfont.CreateFont(
			-18,					    // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_SEMIBOLD,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			ANTIALIASED_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体\0"));             // lpszFacename
	}
	
	bigfont.CreateFont(
		32,					    // nHeight
		12,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0"));  
	
	midfont.CreateFont(
		25,					    // nHeight
		9,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0")); 
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	//bk_dis_dc.CreateCompatibleDC(m_pDC);	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_DIAL_IN_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_BG_DIAL_IN_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    
	
	//pSkin = g_pResMng->RequestBmp(BMP_BG_DIAL_IN_D, true);
	m_stBtnDisBMP = pSkin->hBitmap;   
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_dis_bmp    = bk_dis_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisBMP));
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgDiaIn::InitControl()
{
	for (int i =1;  i < 6; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	
	m_Item[0].rect.top = 125;
	m_Item[0].rect.left = 100;
	m_Item[0].rect.bottom = 244;
	m_Item[0].rect.right = 235;
	
	m_Item[1].rect.top = 125;
	m_Item[1].rect.left = 251;
	m_Item[1].rect.bottom = 244;
	m_Item[1].rect.right = 383;
	
	m_Item[2].rect.top = 100;
	m_Item[2].rect.left = 392;
	m_Item[2].rect.bottom = 150;
	m_Item[2].rect.right = 438;
	
	m_Item[3].rect.top = 194;
	m_Item[3].rect.left = 392;
	m_Item[3].rect.bottom = 240;
	m_Item[3].rect.right = 438;
	
	
	m_Item[4].rect.top = 0;
	m_Item[4].rect.left = 430;
	m_Item[4].rect.bottom = 40;
	m_Item[4].rect.right = 480;
	
	m_Item[5].rect.top = 158;
	m_Item[5].rect.left = 25;
	m_Item[5].rect.bottom = 208;
	m_Item[5].rect.right = 90;
	
	
	m_pItemCity.nState = BTN_STATE_DISABLE;
	
	m_pItemCity.rect.top = 247;
	m_pItemCity.rect.left = 42;
	m_pItemCity.rect.bottom = 272;
	m_pItemCity.rect.right = 442;
	
}

void CDlgDiaIn::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
{
	ASSERT(pDC);
	int x = 0;
	int y = 0;
	switch(state)
	{
	case BTN_STATE_NORMAL:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DOWN:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DISABLE:
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_dis_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgDiaIn::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
{
	COLORREF  nOldRgb;
	nOldRgb=pDC->SetTextColor( col );
	int      nOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont* def_font = pDC->SelectObject(&font);
	
	pDC->DrawText(str, &rt, Format );
	
	pDC->SelectObject(def_font);
	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(nOldRgb);
}

void CDlgDiaIn::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	
	for(int i=0;i<6;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.top = Rect.top + 54;
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255));
	}
	
	Rect =  m_pItemCity.rect;
	DrawBtnText(&memDC,Rect,m_Info,midfont,RGB(255,255,255),DT_TOP|DT_LEFT);
	
	
	DrawBtnText(&memDC,g_retTitile,m_stTitle,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	
	if(m_strNumber.IsEmpty() != TRUE)
	{
		Rect.top = 54;
		Rect.bottom = 90;
		Rect.left = 26;
		Rect.right = 453;
		DrawBtnText(&memDC,Rect,m_strNumber,bigfont,RGB(255,255,255),DT_RIGHT|DT_VCENTER);
		DrawBtnText(&memDC,Rect,m_strName,bigfont,RGB(255,255,255),DT_LEFT|DT_VCENTER);
	}
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);	
	
}

void CDlgDiaIn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_DialExtNum = FALSE;
	if( g_DialNum != NULL )
	{
		g_DialNum->OnExit();
		m_DialExtNum = TRUE;
	}
	for(int i=0;i<6;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			PlayKeySound();
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
			}
		}
	}
	
	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgDiaIn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<6;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				OnButtonAccept();
				break;
			case 1:
				OnButtonReject();
				break;
			case 2:
				OnBtnVolup();
				break;
			case 3:
				OnBtnVoldown();
				break;
			case 4:
				OnBtnExit();
				break;
			case 5:
				OnOpenNumber();
				break;
			default:
				break;
			}
		}
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CDlgDiaIn::OnOpenNumber()
{
	if(g_DialNum == NULL&&!m_DialExtNum)
	{
		g_DialNum = new CDlgDialNumber;
		g_DialNum->Create(IDD_DIALOG_DIAL_NUMBER, this);
	}
	//	m_DialExtNum = !m_DialExtNum;
	//	else
	//		g_DialNum->OnExit();
	
	//	CDlgDialNumber dlg;
	//	dlg.DoModal();
}

LRESULT CDlgDiaIn::OnDialNum(WPARAM wParam , LPARAM lParam)
{
	switch(wParam)
	{
	case 0:
		HBCP_Dial_NumberExtra((unsigned char *)"0");
		m_strNumber+=_T("0");
		Invalidate();
		break;
	case 1:
		HBCP_Dial_NumberExtra((unsigned char *)"1");
		m_strNumber+=_T("1");
		Invalidate();
		break;
	case 2:
		HBCP_Dial_NumberExtra((unsigned char *)"2");
		m_strNumber+=_T("2");
		Invalidate();
		break;
	case 3:
		HBCP_Dial_NumberExtra((unsigned char *)"3");
		m_strNumber+=_T("3");
		Invalidate();
		break;
	case 4:
		HBCP_Dial_NumberExtra((unsigned char *)"4");
		m_strNumber+=_T("4");
		Invalidate();
		break;
	case 5:
		HBCP_Dial_NumberExtra((unsigned char *)"5");
		m_strNumber+=_T("5");
		Invalidate();
		break;
	case 6:
		HBCP_Dial_NumberExtra((unsigned char *)"6");
		m_strNumber+=_T("6");
		Invalidate();
		break;
	case 7:
		HBCP_Dial_NumberExtra((unsigned char *)"7");
		m_strNumber+=_T("7");
		Invalidate();
		break;
	case 8:
		HBCP_Dial_NumberExtra((unsigned char *)"8");
		m_strNumber+=_T("8");
		Invalidate();
		break;
	case 9:
		HBCP_Dial_NumberExtra((unsigned char *)"9");
		m_strNumber+=_T("9");
		Invalidate();
		break;
	case 10:
		HBCP_Dial_NumberExtra((unsigned char *)"#");
		m_strNumber+=_T("#");
		Invalidate();
		break;
	case 11:
		HBCP_Dial_NumberExtra((unsigned char *)"*");
		m_strNumber+=_T("*");
		Invalidate();
		break;
	case 12:
		m_strNumber = m_strNumber.Left(m_strNumber.GetLength()-1);
		Invalidate();
		break;
	}
	return 1;
}
