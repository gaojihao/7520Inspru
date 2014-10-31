// TimeSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "TimeSetDlg.h"
#include "SysConfig.h"
#include "CUnitAndFormatSwitch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeSetDlg dialog


CTimeSetDlg::CTimeSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CTimeSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ZonEexcursion.RemoveAll();

	m_ZonEexcursion.SetAt(-12,0);
	m_ZonEexcursion.SetAt(-11,11);
	m_ZonEexcursion.SetAt(-10,22);
	m_ZonEexcursion.SetAt(-9,33);
	m_ZonEexcursion.SetAt(-8,44);
	m_ZonEexcursion.SetAt(-7,55);
	m_ZonEexcursion.SetAt(-6,65);
	m_ZonEexcursion.SetAt(-5,76);
	m_ZonEexcursion.SetAt(-4,87);
	m_ZonEexcursion.SetAt(-3,93);
	m_ZonEexcursion.SetAt(-2,109);
	m_ZonEexcursion.SetAt(-1,121);
	m_ZonEexcursion.SetAt(0,131);
	m_ZonEexcursion.SetAt(1,141);
	m_ZonEexcursion.SetAt(2,152);
	m_ZonEexcursion.SetAt(3,163);
	m_ZonEexcursion.SetAt(4,175);
	m_ZonEexcursion.SetAt(5,186);
	m_ZonEexcursion.SetAt(6,201);
	m_ZonEexcursion.SetAt(7,207);
	m_ZonEexcursion.SetAt(8,218);
	m_ZonEexcursion.SetAt(9,229);
	m_ZonEexcursion.SetAt(10,240);
	m_ZonEexcursion.SetAt(11,252);
	m_ZonEexcursion.SetAt(12,263);
	m_ZonEexcursion.SetAt(13,274);
}



void CTimeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeSetDlg)
	DDX_Control(pDX, IDC_STA_TIMEZONE, m_stcZoneBmp);
	DDX_Control(pDX, IDC_STA_ICO, m_stcZoneInfo);
	DDX_Control(pDX, IDC_STA_CUR_SECOND, m_stcSec);
	DDX_Control(pDX, IDC_STA_CUR_MINUTE, m_stcMin);
	DDX_Control(pDX, IDC_STA_CUR_HOUR, m_stcHour);
	DDX_Control(pDX, IDC_BTN_ZONESET, m_BtnZoneSet);
	DDX_Control(pDX, IDC_BTN_ZONE_F, m_ZoneF);
	DDX_Control(pDX, IDC_BTN_ZONE_B, m_BtnZoneB);
	DDX_Control(pDX, IDC_BTN_TIMEUNIT_24, m_BtnTimeUnit24);
	DDX_Control(pDX, IDC_BTN_TIMEUNIT_12, m_BtnTimeUnit12);
	DDX_Control(pDX, IDC_BTN_TIMESET_OK, m_BtnOk);
	DDX_Control(pDX, IDC_BTN_TIMESET_BACK, m_BtnCancel);
	DDX_Control(pDX, IDC_BTN_TIMESET, m_BtnTimeSet);
	DDX_Control(pDX, IDC_BTN_TIMEFORMAT, m_BtnTimeFormat);
	DDX_Control(pDX, IDC_BTN_MINUTE_SUB, m_BtnMinuteSub);
	DDX_Control(pDX, IDC_BTN_MINUTE_ADD, m_BtnMinuteAdd);
	DDX_Control(pDX, IDC_BTN_SECOND_ADD, m_BtnSecondAdd);
	DDX_Control(pDX, IDC_BTN_SECOND_SUB, m_BtnSecondSub);
	DDX_Control(pDX, IDC_BTN_HOUR_SUB, m_BtnHourSub);
	DDX_Control(pDX, IDC_BTN_HOUR_ADD, m_BtnHourAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeSetDlg, CDialogBase)
	//{{AFX_MSG_MAP(CTimeSetDlg)
	ON_BN_CLICKED(IDC_BTN_TIMESET_OK, OnBtnTimesetOk)
	ON_BN_CLICKED(IDC_BTN_TIMESET_BACK, OnBtnTimesetBack)
	ON_BN_CLICKED(IDC_BTN_TIMESET, OnBtnTimeset)
	ON_BN_CLICKED(IDC_BTN_TIMEFORMAT, OnBtnTimeformat)
	ON_BN_CLICKED(IDC_BTN_ZONESET, OnBtnZoneset)
	ON_BN_CLICKED(IDC_BTN_ZONE_B, OnBtnZoneB)
	ON_BN_CLICKED(IDC_BTN_ZONE_F, OnBtnZoneF)
	ON_BN_CLICKED(IDC_BTN_HOUR_ADD, OnBtnHourAdd)
	ON_BN_CLICKED(IDC_BTN_HOUR_SUB, OnBtnHourSub)
	ON_BN_CLICKED(IDC_BTN_MINUTE_ADD, OnBtnMinuteAdd)
	ON_BN_CLICKED(IDC_BTN_MINUTE_SUB, OnBtnMinuteSub)
	ON_BN_CLICKED(IDC_BTN_SECOND_ADD, OnBtnSecondAdd)
	ON_BN_CLICKED(IDC_BTN_SECOND_SUB, OnBtnSecondSub)
	ON_BN_CLICKED(IDC_BTN_TIMEUNIT_24, OnBtnTimeunit24)
	ON_BN_CLICKED(IDC_BTN_TIMEUNIT_12, OnBtnTimeunit12)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSetDlg message handlers

BOOL CTimeSetDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_enTimeSet = ID_TIMESET;
	m_bIsTimeAdjust = FALSE;

//	GetDlgItem(IDC_STA_ICO)->CenterWindow(GetParent());

	m_stcSec.SetWindowPos(&CWnd::wndTop,345,135,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_stcMin.SetWindowPos(&CWnd::wndTop,213,135,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_stcHour.SetWindowPos(&CWnd::wndTop,75,135,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_BtnZoneSet.SetWindowPos(&CWnd::wndTop,214,0,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_ZoneF.SetWindowPos(&CWnd::wndTop,240,206,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtnZoneB.SetWindowPos(&CWnd::wndTop,160,206,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
	m_BtnTimeFormat.SetWindowPos(&CWnd::wndTop,106,0,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_BtnTimeUnit24.SetWindowPos(&CWnd::wndTop,49,125,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtnTimeUnit12.SetWindowPos(&CWnd::wndTop,49,52,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
	m_BtnTimeSet.SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_BtnHourAdd.SetWindowPos(&CWnd::wndTop,65,54,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	m_BtnHourSub.SetWindowPos(&CWnd::wndTop,65,200,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtnMinuteAdd.SetWindowPos(&CWnd::wndTop,205,54,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtnMinuteSub.SetWindowPos(&CWnd::wndTop,205,200,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtnSecondAdd.SetWindowPos(&CWnd::wndTop,340,54,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtnSecondSub.SetWindowPos(&CWnd::wndTop,340,200,0,0,SWP_NOSIZE|SWP_NOZORDER);


	m_BtnTimeSet.ShowWindow(SW_HIDE);
	m_BtnTimeFormat.ShowWindow(SW_HIDE);
	m_BtnZoneSet.ShowWindow(SW_HIDE);

//	m_BtnOk.SetWindowPos(&CWnd::wndTop,44,204,0,0,SWP_NOSIZE|SWP_NOZORDER);
// 	m_BtnCancel.SetWindowPos(&CWnd::wndTop,169,204,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
	m_BtnOk.ShowWindow(SW_HIDE);
	m_BtnCancel.SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_stcZoneBmp.MoveWindow(6,69,308,130);
	
	//button状态
	m_BtnTimeSet.SetBtnResID(bmp_file_TimeAdjustBtn1)
		.SetBtnStyle(KDS_BTN_AUTOBMP |KDS_TEXT_VCENTER | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT);
	m_BtnTimeFormat.SetBtnResID(bmp_file_TimeFmtBtn1)
		.SetBtnStyle(KDS_BTN_AUTOBMP |KDS_TEXT_VCENTER | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT);
	m_BtnZoneSet.SetBtnResID(bmp_file_TimeZoneBtn1)
		.SetBtnStyle(KDS_BTN_AUTOBMP |KDS_TEXT_VCENTER | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT);

	m_BtnTimeSet.SetBtnCol(sc_Time_Titile_normal,sc_Time_Titile_select,sc_Time_Titile_disable);
	m_BtnTimeFormat.SetBtnCol(sc_Time_Titile_normal,sc_Time_Titile_select,sc_Time_Titile_disable);
	m_BtnZoneSet.SetBtnCol(sc_Time_Titile_normal,sc_Time_Titile_select,sc_Time_Titile_disable);

	m_BtnTimeSet.EnableWindow(FALSE);
	m_BtnTimeFormat.EnableWindow(TRUE);
 	m_BtnZoneSet.EnableWindow(TRUE);

	//配置文件时区
	m_enTimeUnit = CSysConfig::Instance().GetTimeUnit();
	//系统时间
	GetLocalTime(&m_systime);
	//取配置文件时区
	m_iZone = CSysConfig::Instance().GetTimeZone();
	CUnitAndFormatSwitch::SetTimeZone(m_iZone);


	m_BtnMinuteSub.ShowWindow(SW_SHOW);
	m_BtnMinuteAdd.ShowWindow(SW_SHOW);
	m_BtnSecondAdd.ShowWindow(SW_SHOW);
	m_BtnSecondSub.ShowWindow(SW_SHOW);
	m_BtnHourSub.ShowWindow(SW_SHOW);
	m_BtnHourAdd.ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STA_CUR_HOUR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STA_CUR_MINUTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STA_CUR_SECOND)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STA_ICO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_TIMEUNIT_12)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_TIMEUNIT_24)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ZONE_B)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ZONE_F)->ShowWindow(SW_HIDE);
	m_stcZoneBmp.ShowWindow(SW_HIDE);

	GetLocalTime(&m_systime);
	m_stcHour.SkinSetNum(m_systime.wHour);
	m_stcMin.SkinSetNum(m_systime.wMinute);
	m_stcSec.SkinSetNum(m_systime.wSecond);

//	CString str;
//	str.Format(_T("%d"),m_systime.wHour);
//	SetDlgItemText(IDC_STA_CUR_HOUR,str);
//	str.Format(_T("%d"),m_systime.wMinute);
//	SetDlgItemText(IDC_STA_CUR_MINUTE,str);
//	str.Format(_T("%d"),m_systime.wSecond);
//	SetDlgItemText(IDC_STA_CUR_SECOND,str);

	OnLanguageChange();

	OnSkinChange();

	return TRUE;
}

BOOL CTimeSetDlg::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_BTN_TIMESET);
	SetWindowText(str);
	
	GetDlgItem(IDC_BTN_TIMESET)->SetWindowText(str);

	str.LoadString(RES_BTN_TIMEFORMAT);
	GetDlgItem(IDC_BTN_TIMEFORMAT)->SetWindowText(str);

	str.LoadString(RES_BTN_ZONESET);
	GetDlgItem(IDC_BTN_ZONESET)->SetWindowText(str);

	str.LoadString(RES_BTN_TIMESET_OK);
	GetDlgItem(IDC_BTN_TIMESET_OK)->SetWindowText(str);


//	m_enTimeUnit = CSysConfig::Instance().GetTimeUnit();
//	if(m_enTimeUnit == ID_12_TYPE)
//	{
		str.LoadString(RES_BTN_12_TIMEFORMAT);
		GetDlgItem(IDC_BTN_TIMEUNIT_12)->SetWindowText(str);
//	}
//	else if(m_enTimeUnit == ID_24_TYPE)
//	{
		str.LoadString(RES_BTN_24_TIMEFORMAT);
		GetDlgItem(IDC_BTN_TIMEUNIT_24)->SetWindowText(str);
//	}

	return TRUE;
}

	//变皮肤
BOOL CTimeSetDlg::OnSkinChange()
{
// 	switch(m_enTimeSet)
// 	{
// 	case ID_TIMESET:
// 		{
// 			SkinSetResID(bmp_bg_timeadjust);
// 			m_BtnMinuteSub.SetBtnResID(bmp_file_TimeAdjArrow_62_54_down);
// 			m_BtnMinuteAdd.SetBtnResID(bmp_file_TimeAdjArrow_62_54_up);
// 			m_BtnSecondAdd.SetBtnResID(bmp_file_TimeAdjArrow_62_54_up);
// 			m_BtnSecondSub.SetBtnResID(bmp_file_TimeAdjArrow_62_54_down);
// 			m_BtnHourSub.SetBtnResID(bmp_file_TimeAdjArrow_62_54_down);
// 			m_BtnHourAdd.SetBtnResID(bmp_file_TimeAdjArrow_62_54_up);
// 		}
// 		break;
// 	case ID_TIMEFORMAT:
// 		{
// 			SkinSetResID(bmp_bg_mainmenu);
// 		}
// 		break;
// 	case ID_ZONESET:
// 		{
// 			SkinSetResID(bmp_bg_timezone);
// 		}
// 		break;
// 	default:
// 		break;
// 	}
// 	
// 	SkinSetDlgStyle(KDS_CTRL_FKTEXT|KDS_BTN_AUTOBMP);
// 	SkinSetTitlePos(CPoint(195,6));
// 
// 	m_BtnCancel.SetBtnStyle(KDS_BTN_AUTOBMP)
// 		.SetBtnResID(bmp_file_back);
// 	m_BtnOk.SetBtnStyle(KDS_BTN_AUTOBMP)
// 		.SetBtnResID(bmp_file_back);
// 
// 	m_stcSec.SkinSetStcID(STC_DIGITAL_NUM,bmp_dgt_num,bmp_bg_timeadjust);
// 	m_stcHour.SkinSetStcID(STC_DIGITAL_NUM,bmp_dgt_num,bmp_bg_timeadjust);
// 	m_stcMin.SkinSetStcID(STC_DIGITAL_NUM,bmp_dgt_num,bmp_bg_timeadjust);
// 	
// 
// 	m_stcZoneInfo.SkinSetTxtAttri(sf_18, sc_ZoneWords);
// 	m_stcZoneInfo.SkinSetStcID(STC_TEXT_SINGLE, SKIN_ID_NONE, bmp_bg_timezone );
// 
 	return TRUE;
}

void CTimeSetDlg::InitConfig()
{
//	m_enTimeUnit = CSysConfig::Instance().GetTimeUnit();

	m_TimeZone = CSysConfig::Instance().GetTimeZone();	
}

void CTimeSetDlg::OnBtnTimesetOk() 
{
	// TODO: Add your control notification handler code here

//	CSysConfig::Instance().SetTimeUnit( m_enTimeUnit );
//	CUnitAndFormatSwitch::SetTimeZone(m_iZone);
//	CSysConfig::Instance().SetTimeZone( m_iZone );

	if(m_bIsTimeAdjust)
		SetLocalTime(&m_systime);

	
//	EndDialog(IDCANCEL);
}

void CTimeSetDlg::OnBtnTimesetBack() 
{
	// TODO: Add your control notification handler code here
	
//	CSysConfig::Instance().SetTimeUnit( m_enTimeUnit );
//	CUnitAndFormatSwitch::SetTimeZone(m_iZone);
//	CSysConfig::Instance().SetTimeZone( m_iZone );

	if(m_bIsTimeAdjust)
		SetLocalTime(&m_systime);

	EndDialog(IDCANCEL);
}

void CTimeSetDlg::OnBtnTimeset() 
{
	// TODO: Add your control notification handler code here
// 	m_enTimeSet = ID_TIMESET;
// 	
// 	//button状态
// 	m_BtnTimeSet.EnableWindow(FALSE);
// 	m_BtnTimeFormat.EnableWindow(TRUE);
// 	m_BtnZoneSet.EnableWindow(TRUE);
// 	
// 	GetDlgItem(IDC_BTN_HOUR_ADD)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_HOUR_SUB)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_MINUTE_ADD)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_MINUTE_SUB)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STA_CUR_HOUR)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STA_CUR_MINUTE)->ShowWindow(SW_SHOW);
// //	GetDlgItem(IDC_STA_TIME_UNIT)->ShowWindow(SW_SHOW);
// 
// //	GetDlgItem(IDC_STA_TIME_UNIT2)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STA_CUR_SECOND)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_SECOND_ADD)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_SECOND_SUB)->ShowWindow(SW_SHOW);
// 
// 	GetDlgItem(IDC_BTN_TIMEUNIT_24)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_TIMEUNIT_12)->ShowWindow(SW_HIDE);
// 	
// 	GetDlgItem(IDC_STA_ICO)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_TIMEUNIT)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_ZONE_B)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_ZONE_F)->ShowWindow(SW_HIDE);
// 	m_stcZoneBmp.ShowWindow(SW_HIDE);
// 
// 	DWORD s = m_systime.wHour;
// 	CString str;
// 	str.Format(_T("%d"),m_systime.wHour);
// 	SetDlgItemText(IDC_STA_CUR_HOUR,str);
// 	str.Format(_T("%d"),m_systime.wMinute);
// 	SetDlgItemText(IDC_STA_CUR_MINUTE,str);
// 	str.Format(_T("%d"),m_systime.wSecond);
// 	SetDlgItemText(IDC_STA_CUR_SECOND,str);
// 
// 	SkinSetResID(bmp_bg_timeadjust);
// 	m_BtnMinuteSub.SetBtnResID(bmp_file_TimeAdjArrow_62_54_down);
// 	m_BtnMinuteAdd.SetBtnResID(bmp_file_TimeAdjArrow_62_54_up);
// 	m_BtnSecondAdd.SetBtnResID(bmp_file_TimeAdjArrow_62_54_up);
// 	m_BtnSecondSub.SetBtnResID(bmp_file_TimeAdjArrow_62_54_down);
// 	m_BtnHourSub.SetBtnResID(bmp_file_TimeAdjArrow_62_54_down);
//	m_BtnHourAdd.SetBtnResID(bmp_file_TimeAdjArrow_62_54_up);

	//Invalidate();
//	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
	
}

void CTimeSetDlg::OnBtnTimeformat() 
{
// 	// TODO: Add your control notification handler code here
// 	m_enTimeSet = ID_TIMEFORMAT;
// 
// 	//button状态
// 	m_BtnTimeSet.EnableWindow(TRUE);
// 	m_BtnTimeFormat.EnableWindow(FALSE);
// 	m_BtnZoneSet.EnableWindow(TRUE);
// 
// 	GetDlgItem(IDC_BTN_HOUR_ADD)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_HOUR_SUB)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_MINUTE_ADD)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_MINUTE_SUB)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_CUR_HOUR)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_CUR_MINUTE)->ShowWindow(SW_HIDE);
// //	GetDlgItem(IDC_STA_TIME_UNIT)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_ICO)->ShowWindow(SW_HIDE);
// 	
// //	GetDlgItem(IDC_STA_TIME_UNIT2)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_CUR_SECOND)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_SECOND_ADD)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_SECOND_SUB)->ShowWindow(SW_HIDE);
// 
// 	GetDlgItem(IDC_BTN_ZONE_B)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_ZONE_F)->ShowWindow(SW_HIDE);
// 	m_stcZoneBmp.ShowWindow(SW_HIDE);
// 
// 	GetDlgItem(IDC_BTN_TIMEUNIT_12)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_TIMEUNIT_24)->ShowWindow(SW_SHOW);
// 
// 
// 	CResString str;
// 	str.LoadString(RES_BTN_12_TIMEFORMAT);
// 	GetDlgItem(IDC_BTN_TIMEUNIT_12)->SetWindowText(str);
// 	str.LoadString(RES_BTN_24_TIMEFORMAT);
// 	GetDlgItem(IDC_BTN_TIMEUNIT_24)->SetWindowText(str);
// 
// 
// 	SkinSetResID(bmp_bg_language);
// 
// 	m_BtnTimeUnit24.SetBtnStyle(KDS_BTN_AUTOBMP )
// 		           .SetBtnResID(bmp_file_SetTimeFmt_24fmt);
// 	m_BtnTimeUnit12.SetBtnStyle(KDS_BTN_AUTOBMP )
//  		           .SetBtnResID(bmp_file_SetTimeFmt_12fmt);
// 	
// 	if(m_enTimeUnit == ID_12_TYPE)
// 	{
// 
// 		m_BtnTimeUnit24.EnableWindow(TRUE);
// 		m_BtnTimeUnit12.EnableWindow(FALSE);
// 
// 	}	
// 	else if(m_enTimeUnit == ID_24_TYPE)
// 	{
// 		m_BtnTimeUnit24.EnableWindow(FALSE);
// 		m_BtnTimeUnit12.EnableWindow(TRUE);
// 	}
// 
//	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);


}

void CTimeSetDlg::OnBtnZoneset() 
{
	// TODO: Add your control notification handler code here
// 	m_enTimeSet = ID_ZONESET;
// 
// 	//button状态
// 	m_BtnTimeSet.EnableWindow(TRUE);
// 	m_BtnTimeFormat.EnableWindow(TRUE);
// 	m_BtnZoneSet.EnableWindow(FALSE);
// 
// 	GetDlgItem(IDC_BTN_HOUR_ADD)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_HOUR_SUB)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_MINUTE_ADD)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_MINUTE_SUB)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_CUR_HOUR)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_CUR_MINUTE)->ShowWindow(SW_HIDE);
// //	GetDlgItem(IDC_STA_TIME_UNIT)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_TIMEUNIT)->ShowWindow(SW_HIDE);
// //	GetDlgItem(IDC_STA_TIME_UNIT2)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STA_CUR_SECOND)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_SECOND_ADD)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_SECOND_SUB)->ShowWindow(SW_HIDE);
// 	
// 	GetDlgItem(IDC_BTN_TIMEUNIT_24)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_BTN_TIMEUNIT_12)->ShowWindow(SW_HIDE);
// 
// 	CString str;
// 	str = CUnitAndFormatSwitch::GetTimeZoneInfo();
// 
// 	GetDlgItem(IDC_STA_ICO)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_ZONE_B)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_ZONE_F)->ShowWindow(SW_SHOW);
// 	m_stcZoneBmp.ShowWindow(SW_SHOW);
// 
// 	SetDlgItemText(IDC_STA_ICO,str);
// 
// 	SkinSetResID(bmp_bg_timezone);
// 
// 	m_ZoneF.SetBtnResID(bmp_file_DownBtn);
// 	m_BtnZoneB.SetBtnResID(bmp_file_UpBtn);
// 
// 	m_stcZoneBmp.SkinSetStcID(STC_MAP_ZONE, bmp_map_zone, bmp_bg_timezone );
// 
// 	UINT ZoneMapSkinPos = 0;
// 	m_ZonEexcursion.Lookup(m_iZone,ZoneMapSkinPos);
// 	m_stcZoneBmp.SKinSetMapOffset(ZoneMapSkinPos);
// 
// 	::RedrawWindow(this->GetSafeHwnd(),NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
}


void CTimeSetDlg::OnBtnZoneB() 
{
	// TODO: Add your control notification handler code here
	switch(m_enTimeSet)
	{
//	case ID_TIMEFORMAT:
//		{
//			CResString str;
//			if(m_enTimeUnit == ID_12_TYPE)
//			{
//				m_enTimeUnit = ID_24_TYPE;
//				str.LoadString(RES_STA_24_TIMEFORMAT);
//			}
//			else if(m_enTimeUnit == ID_24_TYPE)
//			{
//				m_enTimeUnit = ID_12_TYPE;
//				str.LoadString(RES_STA_12_TIMEFORMAT);
//			}
// 			GetDlgItem(IDC_STA_TIMEFORMAT)->SetWindowText(str);
//
//			break;
// 		}
	case ID_ZONESET:
		{
			m_iZone += 1;

			if (m_iZone > 13 || m_iZone < -12)
				m_iZone = -12;

			CUnitAndFormatSwitch::SetTimeZone(m_iZone);

			CString str;
			str = CUnitAndFormatSwitch::GetTimeZoneInfo();

			SetDlgItemText(IDC_STA_ICO,str); 
			
			UINT ZoneMapSkinPos = 0;
			m_ZonEexcursion.Lookup(m_iZone,ZoneMapSkinPos);
			
			m_stcZoneBmp.SKinSetMapOffset(ZoneMapSkinPos);

			m_stcZoneInfo.RedrawWindow(NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
			m_stcZoneBmp.RedrawWindow(NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);

			break;
		}
	default:
		break;
	}
	
}

void CTimeSetDlg::OnBtnZoneF() 
{
	// TODO: Add your control notification handler code here
	switch(m_enTimeSet)
	{
//	case ID_TIMEFORMAT:
//		{
//			CResString str;
//			if(m_enTimeUnit == ID_12_TYPE)
//			{
//				m_enTimeUnit = ID_24_TYPE;
//				str.LoadString(RES_STA_24_TIMEFORMAT);
//			}
//			else if(m_enTimeUnit == ID_24_TYPE)
//			{
//				m_enTimeUnit = ID_12_TYPE;
//				str.LoadString(RES_STA_12_TIMEFORMAT);
//			}
// 			GetDlgItem(IDC_STA_TIMEFORMAT)->SetWindowText(str);
//			break;
// 		}
	case ID_ZONESET:
		{
			m_iZone = m_iZone - 1;
			
			if (m_iZone > 13 || m_iZone < -12)
				m_iZone = 13;
				
			CUnitAndFormatSwitch::SetTimeZone(m_iZone);
	

			CString str;
			str = CUnitAndFormatSwitch::GetTimeZoneInfo();


			SetDlgItemText(IDC_STA_ICO,str);

			UINT ZoneMapSkinPos = 0;
			m_ZonEexcursion.Lookup(m_iZone,ZoneMapSkinPos);
			
			m_stcZoneBmp.SKinSetMapOffset(ZoneMapSkinPos);

			TRACE2("\n\n------- %d:%s \n\n",ZoneMapSkinPos,str);

			m_stcZoneInfo.RedrawWindow(NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);
			m_stcZoneBmp.RedrawWindow(NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW);

	
			break;
		}
	default:
		break;
	}
}

void CTimeSetDlg::OnBtnHourAdd() 
{
	// TODO: Add your control notification handler code here
	m_bIsTimeAdjust = TRUE;

	if(m_systime.wHour >= 23)
		m_systime.wHour = 0;
	else
		m_systime.wHour += 1;

	m_stcHour.SkinSetNum(m_systime.wHour);
	
//	CString str;
//	str.Format(_T("%d"),m_systime.wHour);
//	SetDlgItemText(IDC_STA_CUR_HOUR,str);
}

void CTimeSetDlg::OnBtnHourSub() 
{
	// TODO: Add your control notification handler code here
	m_bIsTimeAdjust = TRUE;

	if(m_systime.wHour <= 0 )
		m_systime.wHour = 23;
	else
		m_systime.wHour = m_systime.wHour - 1;

	m_stcHour.SkinSetNum(m_systime.wHour);
	
//	CString str;
//	str.Format(_T("%d"),m_systime.wHour);
//	SetDlgItemText(IDC_STA_CUR_HOUR,str);
}

void CTimeSetDlg::OnBtnMinuteAdd() 
{
	// TODO: Add your control notification handler code here
	m_bIsTimeAdjust = TRUE;

	if(m_systime.wMinute >= 59)
		m_systime.wMinute = 0;
	else
		m_systime.wMinute += 1;

	m_stcMin.SkinSetNum(m_systime.wMinute);
//	CString str;
//	str.Format(_T("%d"),m_systime.wMinute);
//	SetDlgItemText(IDC_STA_CUR_MINUTE,str);
}

void CTimeSetDlg::OnBtnMinuteSub() 
{
	// TODO: Add your control notification handler code here
	m_bIsTimeAdjust = TRUE;

	if(m_systime.wMinute <= 0)
		m_systime.wMinute = 59;
	else
		m_systime.wMinute = m_systime.wMinute - 1;
	m_stcMin.SkinSetNum(m_systime.wMinute);
//	CString str;
//	str.Format(_T("%d"),m_systime.wMinute);
//	SetDlgItemText(IDC_STA_CUR_MINUTE,str);
}

void CTimeSetDlg::OnBtnSecondAdd() 
{
	// TODO: Add your control notification handler code here
	m_bIsTimeAdjust = TRUE;

	if(m_systime.wSecond >= 59)
		m_systime.wSecond = 0;
	else
		m_systime.wSecond += 1;

	m_stcSec.SkinSetNum(m_systime.wSecond);
	
//	CString str;
//	str.Format(_T("%d"),m_systime.wSecond);
//	SetDlgItemText(IDC_STA_CUR_SECOND,str);
}

void CTimeSetDlg::OnBtnSecondSub() 
{
	// TODO: Add your control notification handler code here
	m_bIsTimeAdjust = TRUE;
	
	if(m_systime.wSecond <= 0)
		m_systime.wSecond = 59;
	else
		m_systime.wSecond = m_systime.wSecond - 1;

	m_stcSec.SkinSetNum(m_systime.wSecond);
//	CString str;
//	str.Format(_T("%d"),m_systime.wSecond);
//	SetDlgItemText(IDC_STA_CUR_SECOND,str);
}

//void CTimeSetDlg::OnBtnTimeunit() 
//{
//	// TODO: Add your control notification handler code here
//	m_enTimeUnit  = ID_12_TYPE;
// }

void CTimeSetDlg::OnBtnTimeunit24() 
{
	// TODO: Add your control notification handler code here
	m_enTimeUnit  = ID_24_TYPE;
	m_BtnTimeUnit24.EnableWindow(FALSE);
	m_BtnTimeUnit12.EnableWindow(TRUE);

}

void CTimeSetDlg::OnBtnTimeunit12() 
{
	// TODO: Add your control notification handler code here
	m_enTimeUnit  = ID_12_TYPE;
	m_BtnTimeUnit24.EnableWindow(TRUE);
	m_BtnTimeUnit12.EnableWindow(FALSE);

}

LRESULT CTimeSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
// 	if( message == WM_CREATE_SET )
// 	{
// 		if( wParam == 0x03 )
// 		{
// 			EndDialog(0);
// 		}
// 	}	// TODO: Add your specialized code here and/or call the base class
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
