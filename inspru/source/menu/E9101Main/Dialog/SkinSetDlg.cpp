// SkinSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "E9101Main.h"
#include "SkinSetDlg.h"
#include "ak4642api.h"
#include "E9101MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SysConfig.h"
/////////////////////////////////////////////////////////////////////////////
// CSkinSetDlg dialog
BOOL CSkinSetDlg::g_bFmOpen = FALSE;
extern BOOL g_timedatedsp;

CSkinSetDlg::CSkinSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSkinSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinSetDlg)
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


void CSkinSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinSetDlg)
	DDX_Control(pDX, IDC_BTN_SUB, m_FreqSub);
	DDX_Control(pDX, IDC_BTN_ADD, m_FreqAdd);
	DDX_Control(pDX, IDC_STATIC_FREQ, m_Static);
	DDX_Control(pDX, IDC_BTN_SWITCH, m_BtnSwith);
	DDX_Control(pDX, IDC_BTN_SKINSET_OK, m_BtnOk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinSetDlg, CDialogBase)
	//{{AFX_MSG_MAP(CSkinSetDlg)
//	ON_BN_CLICKED(IDC_BTN_SWITCH, OnBtnSwitch)
//	ON_BN_CLICKED(IDC_BTN_SKINSET_OK, OnBtnSkinsetOk)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SUB, OnBtnSub)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinSetDlg message handlers

BOOL CSkinSetDlg::OnLanguageChange()
{

	CResString str;

	str.LoadString(RES_BTN_DATE_TIME);
//	SetWindowText(str);
	m_strdatetime = str;
	
// 	if( CSysConfig::Instance().GetIsSpareMode() )
// 	{
// 		str.LoadString(RES_BTN_SKIN_SET_FM_OPEN);
// 	}
// 	else
// 	{
// 		str.LoadString(RES_BTN_SKIN_SET_FM_CLOSE);
// 	}
//	m_Item[4].chChar = str;

//	m_BtnSwith.SetWindowText(str);

	return TRUE;
}

	//±äÆ¤·ô
BOOL CSkinSetDlg::OnSkinChange()
{

	return TRUE;
}

BOOL CSkinSetDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();


	GetLocalTime(&m_systime);

	m_iZone = CSysConfig::Instance().GetTimeZone();
	CUnitAndFormatSwitch::SetTimeZone(m_iZone);
	m_strZone = CUnitAndFormatSwitch::GetTimeZoneInfo();

	m_stryear.Format(_T("%d"),m_systime.wYear);
	m_strmonth.Format(_T("%d"),m_systime.wMonth);
	m_strday.Format(_T("%d"),m_systime.wDay);
	m_strhour.Format(_T("%d"),m_systime.wHour);
	m_strminute.Format(_T("%d"),m_systime.wMinute);

	OnLanguageChange();
	OnSkinChange();



	return TRUE;
}


void CSkinSetDlg::OnBtnAdd() 
{

}

void CSkinSetDlg::OnBtnSub() 
{	

}



BOOL CSkinSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	
		return CDialogBase::PreTranslateMessage(pMsg);
}
 
LRESULT CSkinSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	if(WM_EXIT2MAIN_MENU == message)
	{
		OnExit();	
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}


void CSkinSetDlg::InitGdi()
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
		_T("ËÎÌå\0")); 

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
		_T("ËÎÌå\0"));             // lpszFacename
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
		_T("ËÎÌå\0")); 

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	//bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_TIME_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_BG_TIME_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	//pSkin = g_pResMng->RequestBmp(BMP_BG_FM_D, true);
	//m_stBtnDisableBMP = pSkin->hBitmap; 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CSkinSetDlg::OnExit()
{
	SetLocalTime(&m_systime);
	//bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	//bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(0);

}

void CSkinSetDlg::InitControl()
{
	for (int i =0;  i < 14; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}


	m_Item[0].rect.top = 16;
	m_Item[0].rect.left = 60;
	m_Item[0].rect.bottom = 59;
	m_Item[0].rect.right = 100;

	m_Item[1].rect.top = 99;
	m_Item[1].rect.left = 60;
	m_Item[1].rect.bottom = 137;
	m_Item[1].rect.right = 100;

	m_Item[2].rect.top = 16;
	m_Item[2].rect.left = 142;
	m_Item[2].rect.bottom = 59;
	m_Item[2].rect.right = 182;

	m_Item[3].rect.top = 99;
	m_Item[3].rect.left = 142;
	m_Item[3].rect.bottom = 137;
	m_Item[3].rect.right = 182;

	m_Item[4].rect.top = 16;
	m_Item[4].rect.left = 221;
	m_Item[4].rect.bottom = 59;
	m_Item[4].rect.right = 261;

	m_Item[5].rect.top = 99;
	m_Item[5].rect.left = 221;
	m_Item[5].rect.bottom = 137;
	m_Item[5].rect.right = 261;

	m_Item[6].rect.top = 16;
	m_Item[6].rect.left = 302;
	m_Item[6].rect.bottom = 59;
	m_Item[6].rect.right = 342;

	m_Item[7].rect.top = 99;
	m_Item[7].rect.left = 302;
	m_Item[7].rect.bottom = 137;
	m_Item[7].rect.right = 342;

	m_Item[8].rect.top = 24;
	m_Item[8].rect.left = 383;
	m_Item[8].rect.bottom = 59;
	m_Item[8].rect.right = 423;

	m_Item[9].rect.top = 99;
	m_Item[9].rect.left = 383;
	m_Item[9].rect.bottom = 137;
	m_Item[9].rect.right = 423;

	m_Item[10].rect.top = 158;
	m_Item[10].rect.left = 35;
	m_Item[10].rect.bottom = 195;
	m_Item[10].rect.right = 76;

	m_Item[11].rect.top = 158;
	m_Item[11].rect.left = 407;
	m_Item[11].rect.bottom = 195;
	m_Item[11].rect.right = 447;


	m_Item[12].rect.top = 225;
	m_Item[12].rect.left = 395;
	m_Item[12].rect.bottom = 271;
	m_Item[12].rect.right = 477;

	m_Item[13].rect.top = 211;
	m_Item[13].rect.left = 31;
	m_Item[13].rect.bottom = 254;
	m_Item[13].rect.right = 85;

	if( 1 == CSysConfig::Instance().GetVerVol() )
	{
		m_Item[13].nState = BTN_STATE_DOWN;
	}
	else
	{
		m_Item[13].nState = BTN_STATE_NORMAL;
	}
}

void CSkinSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CSkinSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CSkinSetDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<14;i++)
	{
		//»æÖÆ±³¾°
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
//		Rect.top = Rect.top + 54;
//		DrawBtnText(&memDC,Rect,str,RGB(255,125,0));
	}

// 		str = m_Item[1].chChar;
//  		Rect = m_Item[1].rect;
//  		state = BTN_STATE_DISABLE;
// 	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
// 		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
// 	else
// 		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
// 
// 		str = m_Item[4].chChar;
//  		Rect = m_Item[4].rect;
//  		state = BTN_STATE_DISABLE;
// 	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
// 		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
// 	else
//		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);


	DrawBtnText(&memDC,CRect(48,59,113,94),m_stryear,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);	
	DrawBtnText(&memDC,CRect(130,59,193,94),m_strmonth,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);	
	DrawBtnText(&memDC,CRect(210,59,273,94),m_strday,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);	
	DrawBtnText(&memDC,CRect(290,59,362,94),m_strhour,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	DrawBtnText(&memDC,CRect(369,59,432,94),m_strminute,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);	
	DrawBtnText(&memDC,CRect(78,158,403,193),m_strZone,smallfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	DrawBtnText(&memDC,CRect(95,214,345,250),m_strdatetime,smallfont,RGB(255,255,255),DT_LEFT|DT_VCENTER);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);


	memDC.SelectObject(pOldBmp);	

}

void CSkinSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<14;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			if(i != 13)
			{
				if(m_Item[i].nState == BTN_STATE_NORMAL)
				{
					m_Item[i].nState = BTN_STATE_DOWN;
					PlayKeySound();
				}
			}
			else
			{
				PlayKeySound();
				g_timedatedsp = !g_timedatedsp;
				CSysConfig::Instance().SetVerVol(g_timedatedsp);
				if( 1 == CSysConfig::Instance().GetVerVol() )
				{
					m_Item[13].nState = BTN_STATE_DOWN;
				}
				else
				{
					m_Item[13].nState = BTN_STATE_NORMAL;
				}
			}

		}
	}
	
	Invalidate();

	CDialogBase::OnLButtonDown(nFlags, point);
}

void CSkinSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<14;i++)
	{
		if(i == 13)
			break;
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			
				
				switch( i )
					{
					case 0:
						OnAddyear();
						break;
					case 1:
						OnSubyear();
						break;
					case 2:
						OnAddMonth();
						break;
					case 3:
						OnSubMonth();
						break;
					case 4:
						OnAddday();
						break;
					case 5:
						OnSubday();
						break;
					case 6:
						OnAddhour();
						break;
					case 7:
						OnSubhour();
						break;
					case 8:
						OnAddminute();
						break;
					case 9:
						OnSubminute();
						break;
					case 10:
						OnLeftZone();
						break;
					case 11:
						OnRightZone();
						break;
					case 12:
						OnExit();
						break;
					case 13:
						{
							
						}
						
						break;						
					default:
						break;
					}
					
			}
		
	}

	Invalidate();

	CDialogBase::OnLButtonUp(nFlags, point);
}



void CSkinSetDlg::OnAddyear()
{

	m_systime.wYear++;

	if ( m_systime.wYear <= 1979 )
		m_systime.wYear = 1980;
	m_stryear.Format(_T("%d"),m_systime.wYear);
	Invalidate();
}

void CSkinSetDlg::OnSubyear()
{
	m_systime.wYear--;
	if ( m_systime.wYear <= 1979 )
		m_systime.wYear = 1980;
	m_stryear.Format(_T("%d"),m_systime.wYear);
	Invalidate();
}

void CSkinSetDlg::OnAddMonth()
{

	if ( m_systime.wMonth >= 12 )
		m_systime.wMonth =  1;
	else
		m_systime.wMonth++;
	m_strmonth.Format(_T("%d"),m_systime.wMonth);
	Invalidate();
}

void CSkinSetDlg::OnSubMonth()
{

	if ( m_systime.wMonth == 1 )
		m_systime.wMonth = 12;
	else
		m_systime.wMonth--;
	m_strmonth.Format(_T("%d"),m_systime.wMonth);
	Invalidate();
}

void CSkinSetDlg::OnAddday()
{
	int n = max_day(m_systime.wYear, m_systime.wMonth );

	if ( m_systime.wDay >= n )
		m_systime.wDay = 1;
	else
		m_systime.wDay++;
	m_strday.Format(_T("%d"),m_systime.wDay);

	Invalidate();
}

void CSkinSetDlg::OnSubday()
{
	int n = max_day(m_systime.wYear, m_systime.wMonth );

	if ( m_systime.wDay <= 1 )
		m_systime.wDay = n;
	else
		m_systime.wDay--;
	m_strday.Format(_T("%d"),m_systime.wDay);
}

void CSkinSetDlg::OnAddhour()
{
	if(m_systime.wHour >= 23)
		m_systime.wHour = 0;
	else
		m_systime.wHour += 1;

	m_strhour.Format(_T("%d"),m_systime.wHour);

	Invalidate();
}

void CSkinSetDlg::OnSubhour()
{
	if(m_systime.wHour <= 0 )
		m_systime.wHour = 23;
	else
		m_systime.wHour = m_systime.wHour - 1;

	m_strhour.Format(_T("%d"),m_systime.wHour);
	Invalidate();
}

void CSkinSetDlg::OnAddminute()
{
	if(m_systime.wMinute >= 59)
		m_systime.wMinute = 0;
	else
		m_systime.wMinute += 1;
	m_strminute.Format(_T("%d"),m_systime.wMinute);
	Invalidate();
}

void CSkinSetDlg::OnSubminute()
{
	if(m_systime.wMinute <= 0)
		m_systime.wMinute = 59;
	else
		m_systime.wMinute = m_systime.wMinute - 1;
	m_strminute.Format(_T("%d"),m_systime.wMinute);
	Invalidate();
}

void CSkinSetDlg::OnLeftZone()
{
		m_iZone = m_iZone - 1;
			
		if (m_iZone > 13 || m_iZone < -12)
			m_iZone = 13;
			
		CSysConfig::Instance().SetTimeZone(m_iZone);
		CUnitAndFormatSwitch::SetTimeZone(m_iZone);
		
		m_strZone = CUnitAndFormatSwitch::GetTimeZoneInfo();

		GetLocalTime(&m_systime);

		m_stryear.Format(_T("%d"),m_systime.wYear);
		m_strmonth.Format(_T("%d"),m_systime.wMonth);
		m_strday.Format(_T("%d"),m_systime.wDay);
		m_strhour.Format(_T("%d"),m_systime.wHour);
		m_strminute.Format(_T("%d"),m_systime.wMinute);
		Invalidate();
}

void CSkinSetDlg::OnRightZone()
{
		m_iZone += 1;

		if (m_iZone > 13 || m_iZone < -12)
			m_iZone = -12;
		
		CSysConfig::Instance().SetTimeZone(m_iZone);
		CUnitAndFormatSwitch::SetTimeZone(m_iZone);

		m_strZone = CUnitAndFormatSwitch::GetTimeZoneInfo();

		GetLocalTime(&m_systime);

		m_stryear.Format(_T("%d"),m_systime.wYear);
		m_strmonth.Format(_T("%d"),m_systime.wMonth);
		m_strday.Format(_T("%d"),m_systime.wDay);
		m_strhour.Format(_T("%d"),m_systime.wHour);
		m_strminute.Format(_T("%d"),m_systime.wMinute);
		Invalidate();
}

int CSkinSetDlg::max_day( int n_year, int m_month )
{
	switch( m_month )
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		{	if ( (n_year%400 == 0) || (n_year%4==0 && n_year%100 != 0 ) )
				return 29;
			else
				return 28;
		}
		break;
	}
	return 30;
}







