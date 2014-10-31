// DlgPowerStatus.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgPowerStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPowerStatus dialog

int CDlgPowerStatus::m_nPic = 0;
//extern m_hMainWnd;

CDlgPowerStatus::CDlgPowerStatus(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgPowerStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPowerStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPowerStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPowerStatus)
	DDX_Control(pDX, IDC_STATIC_POWERSTATUS, m_stcPowerStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPowerStatus, CDialogBase)
	//{{AFX_MSG_MAP(CDlgPowerStatus)
	ON_WM_TIMER()
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POWER_EVENT, OnPowerStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPowerStatus message handlers

BOOL CDlgPowerStatus::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	MoveWindow(435,9,34,17);

	m_stcPowerStatus.SetWindowPos(&CWnd::wndTop, 0,0,34,17, NULL);
	OnSkinChange();
	
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 34, 17, SWP_NOMOVE | SWP_SHOWWINDOW);
	
	SetTimer(44, 500, NULL);
	m_bVisible = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CDlgPowerStatus::OnPowerStatus(WPARAM wParam, LPARAM lParam)
{
	static int Percent = 100;

	m_PowerState = (POWERSTAT_E)wParam;

// 	CString cs;
// 	cs.Format(_T("Enter Pwr Thread... : %d \n", (int)wParam));
// 	RETAILMSG(1,(TEXT("%s"),cs));
// 	cs.Format(_T("Pwr Pre is : %d \n", (int)lParam));
//	RETAILMSG(1,(TEXT("%s"),cs));
	
	switch( m_PowerState )
	{
	case BATTERY_CHARGING:
		{
			m_PowerState = BATTERY_CHARGING;
			Percent = 100;
		}
		break;
	case BATTERY_FULL:
		{
			m_PowerState = BATTERY_FULL;
			break;
		}

	case BATTERY_ONLINE:
		m_PowerState = BATTERY_ONLINE;
		m_nPercent = (int)lParam;
		m_nPic = m_nPercent/20;
		{
			//电量0时可以使主页面电量显示回跳
			if( m_nPercent != 0 )
			{
				if ( m_nPercent > Percent )
				{
					m_nPercent = Percent;
				}
				else
				{
					Percent = m_nPercent;
				}

			}
			switch( m_nPercent )
			{
			case 0:
				{
					m_stcPowerStatus.SkinSetBatterNum(battery_empy);
					break;
				}
			case 20:
				m_stcPowerStatus.SkinSetBatterNum(battery_20);
				break;
			case 40:
				m_stcPowerStatus.SkinSetBatterNum(battery_40);	
				break;
			case 60:
				m_stcPowerStatus.SkinSetBatterNum(battery_60);
				break;
			case 80:
				m_stcPowerStatus.SkinSetBatterNum(battery_80);
				break;
			case 100:
				{
					CString str(_T("Offline Batter FULL!!!" ));
//					RETAILMSG(1,(TEXT("%s"),str));
					m_stcPowerStatus.SkinSetBatterNum(battery_100);
				}
				break;
			default:
				m_stcPowerStatus.SkinSetBatterNum(battery_20);
				break;
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	static int bFirst = TRUE;
	if(bFirst)
	{
		bFirst = FALSE;
		m_stcPowerStatus.ShowWindow(SW_SHOW);
	}
	return 1L;
}

BOOL CDlgPowerStatus::OnLanguageChange()
{
	return TRUE;
}

BOOL CDlgPowerStatus::OnSkinChange()
{
	m_stcPowerStatus.SkinSetStcID(STC_BMP_BATTERY, bmp_battery );
	m_stcPowerStatus.SkinSetBatterNum(battery_empy);
	return TRUE;
}

void CDlgPowerStatus::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogBase::PostNcDestroy();
	delete this;
}

void CDlgPowerStatus::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//BringWindowToTop();
	//::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 34, 17, SWP_NOMOVE | SWP_SHOWWINDOW);
	if(::IsWindowVisible(m_stcPowerStatus.GetSafeHwnd()) && BATTERY_CHARGING == m_PowerState)
	{
//		if( s_pTipDlg->m_hWnd != NULL )
//		{
//			::PostMessage( s_pTipDlg->m_hWnd , WM_QUIT , 0 , 0 );
//		}
		m_stcPowerStatus.SkinSetStcID(STC_BMP_BATTERY, bmp_battery);
//		TRACE0("BATTERY_CHARGING change pic\n");
		m_nPic++;
		if(m_nPic > 5)m_nPic = 1;
		
		switch(m_nPic)
		{
		case 0:
			m_stcPowerStatus.SkinSetBatterNum(battery_empy);
			break;
		case 1:
			m_stcPowerStatus.SkinSetBatterNum(battery_20);
			break;
		case 2:
			m_stcPowerStatus.SkinSetBatterNum(battery_40);	
			break;
		case 3:
			m_stcPowerStatus.SkinSetBatterNum(battery_60);
			break;
		case 4:
			m_stcPowerStatus.SkinSetBatterNum(battery_80);
			break;
		case 5:
			m_stcPowerStatus.SkinSetBatterNum(battery_100);
			break;
		default:
			m_stcPowerStatus.SkinSetBatterNum(battery_20);

			break;
		}
	}

	if(m_nPercent == 0 && BATTERY_CHARGING != m_PowerState)
	{
// 		static bool bVsibleFlag = FALSE;
// 		MoveWindow(410, 0, 50, 30);
// 		m_stcPowerStatus.SetWindowPos(&CWnd::wndTop, 0,0,50,30, NULL);
// 		m_stcPowerStatus.SkinSetStcID(STC_BMP_BATTERY, bmp_battery3 );
// 		ShowWindow(bVsibleFlag ? SW_HIDE: SW_SHOW);
// 		m_stcPowerStatus.SkinSetBatterNum(battery_empy);
// 		RedrawWindow(NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
// 		bVsibleFlag = !bVsibleFlag;
		ShowWindow(SW_HIDE);
	}
	else if(TRUE == m_bVisible)
	{
		m_stcPowerStatus.SkinSetStcID(STC_BMP_BATTERY, bmp_battery);
		MoveWindow(435, 9, 34, 17);
		ShowWindow(SW_SHOW);
	}
	else
	{
		m_stcPowerStatus.SkinSetStcID(STC_BMP_BATTERY, bmp_battery);
		MoveWindow(435, 9, 34, 17);
		ShowWindow(SW_HIDE);
	}
	CDialogBase::OnTimer(nIDEvent);
}

void CDlgPowerStatus::ShowPowerStatus()
{
	m_bVisible = TRUE;
	ShowWindow(SW_SHOW);
}

void CDlgPowerStatus::HidePowerStatus()
{
	m_bVisible = FALSE;
	ShowWindow(SW_HIDE);
}

LRESULT CDlgPowerStatus::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x03 )
		{
			ShowWindow(SW_HIDE);		
		}
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
