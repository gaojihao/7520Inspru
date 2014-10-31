// BrightnessSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "BrightnessSetDlg.h"
#include "SysConfig.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../DriveLib/IOControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bLight;

int CBrightnessSetDlg::m_nBrightness = 10;	//初始化音量等级

CBrightnessSetDlg::CBrightnessSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CBrightnessSetDlg::IDD, pParent)
{
	//亮度等级
	m_nBrightness = CSysConfig::Instance().GetBrightness();
	if(m_nBrightness < 1)
		m_nBrightness = 1;
	else if(m_nBrightness > 10)
		m_nBrightness = 10;
	//背光时间关闭时间等级
	m_nSpareTimeLevel = CSysConfig::Instance().GetSpareTime();
	if(m_nSpareTimeLevel < 0)
		m_nSpareTimeLevel = 0;
	else if(m_nSpareTimeLevel > 5)
		m_nSpareTimeLevel = 5;
}

void CBrightnessSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrightnessSetDlg)
	DDX_Control(pDX, IDC_BTN_HIGH, m_BtnLightHigh);
	DDX_Control(pDX, IDC_BTN_MID, m_BtnLightMid);
	DDX_Control(pDX, IDC_BTN_LOW, m_BtnLightLow);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BtnExit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrightnessSetDlg, CDialogBase)
	//{{AFX_MSG_MAP(CBrightnessSetDlg)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrightnessSetDlg message handlers

BOOL CBrightnessSetDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	UpdateBnStatus();

	SetWindowText(_T("Brightness_Setting"));

	return TRUE;
}

BOOL CBrightnessSetDlg::OnLanguageChange()
{
	CResString str;
	
	str.LoadString(RES_STA_BRIGHTSET_TITILE);
	m_csTitile = str;


	return TRUE;
}

//变皮肤
BOOL CBrightnessSetDlg::OnSkinChange()
{
	return TRUE;
}

void CBrightnessSetDlg::OnBtnExit() 
{
	OnExit();
}

void CBrightnessSetDlg::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

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

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_BRIGHTNESS_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BG_BRIGHTNESS_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	pSkin = g_pResMng->RequestBmp(BG_BRIGHTNESS_D, true);
	m_stBtnDisableBMP = pSkin->hBitmap; 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CBrightnessSetDlg::OnExit( )
{
	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(0);

}

void CBrightnessSetDlg::InitControl()
{
	for (int i =0;  i < 7; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
//亮度左
	m_Item[0].rect.left   = 127;
	m_Item[0].rect.top    = 123;
	m_Item[0].rect.right  = 285;
	m_Item[0].rect.bottom = 269;
//亮度值
	m_Item[1].rect.left   = 286;
	m_Item[1].rect.top    = 123;
	m_Item[1].rect.right  = 519;
	m_Item[1].rect.bottom = 269;
//亮度右
	m_Item[2].rect.left   = 520;
	m_Item[2].rect.top    = 123;
	m_Item[2].rect.right  = 664;
	m_Item[2].rect.bottom = 269;
//黑屏时间左
	m_Item[3].rect.left		= 127;
	m_Item[3].rect.top		= 276;
	m_Item[3].rect.right	= 285;
	m_Item[3].rect.bottom	= 404;
//黑屏时间
	m_Item[4].rect.left		= 286;
	m_Item[4].rect.top		= 276;
	m_Item[4].rect.right	= 519;
	m_Item[4].rect.bottom	= 404;
//黑屏时间右
	m_Item[5].rect.left		= 520;
	m_Item[5].rect.top		= 276;
	m_Item[5].rect.right	= 664;
	m_Item[5].rect.bottom	= 404;
//Exit
	m_Item[6].rect.left = 740;
	m_Item[6].rect.top = 0;
	m_Item[6].rect.right = 800;
	m_Item[6].rect.bottom = 60;
}

void CBrightnessSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CBrightnessSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CBrightnessSetDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<7;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
//		Rect.top = Rect.top + 54;
//		DrawBtnText(&memDC,Rect,str,RGB(255,125,0));
	}
	
	str = m_Item[1].chChar;
	Rect = m_Item[1].rect;
	state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	
	str = m_Item[4].chChar;
	Rect = m_Item[4].rect;
	state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);


	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CBrightnessSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0;i<7;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}
	
	Invalidate();
	SetCapture();

	CDialogBase::OnLButtonDown(nFlags, point);
}

void CBrightnessSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<7;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0://亮度-
				OnAdjustBrightness(-1);
				break;
			case 2://亮度+
				OnAdjustBrightness(1);
				break;
			case 3:
				OnSpareleft();
				break;
			case 5:
				OnSpareright();
				break;
			case 6:
				OnBtnExit();
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

//亮度
void CBrightnessSetDlg::OnAdjustBrightness(int nDifference)
{
	//亮度等级1~10
	if(m_nBrightness+nDifference < 1 || m_nBrightness+nDifference > 10)
		return;

	m_nBrightness += nDifference;
	SetValue();
	UpdateBnStatus();
}

void CBrightnessSetDlg::SetValue()
{
	IO_SetLcdBackLight(m_nBrightness);
	CSysConfig::Instance().SetBrightness(enBrightness(m_nBrightness));
}

void CBrightnessSetDlg::SetbkliteTimeOuts()
{
}

void CBrightnessSetDlg::OnSpareleft()
{
	if(m_nSpareTimeLevel > 0)
		m_nSpareTimeLevel--;
	else
		return;

	UpdateBnStatus();
}

void CBrightnessSetDlg::OnSpareright()
{
	if(m_nSpareTimeLevel < 5)
		m_nSpareTimeLevel++;
	else
		return;
	
	UpdateBnStatus();
}

LRESULT CBrightnessSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			//OnBtnExit();
		}
 	}
	else if(WM_ADJUST_BRIGHTNESS_VOLUMN == message)
	{
		UpdateBnStatus();
		Invalidate();
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
//更新按钮状态
void CBrightnessSetDlg::UpdateBnStatus()
{
	//亮度
	CString strPercent;
	if(m_nBrightness == 1)
	{
		m_Item[0].nState = BTN_STATE_DISABLE;	//1.
		m_Item[2].nState = BTN_STATE_NORMAL;
	}
	else if(m_nBrightness == 10)
	{
		m_Item[0].nState = BTN_STATE_NORMAL;	//10.
		m_Item[2].nState = BTN_STATE_DISABLE;
	}
	else
	{
		m_Item[0].nState = BTN_STATE_NORMAL;	//1~10.
		m_Item[2].nState = BTN_STATE_NORMAL;
	}

	strPercent.Format(_T("%d%%"), m_nBrightness*10);
	m_Item[1].chChar = strPercent;

	//背光时间
	CResString str;
	switch(m_nSpareTimeLevel)
	{
	case 0:
		{
			str.LoadString(RES_BUTTON_LCD_1);
			m_Item[4].chChar = str;
			m_Item[3].nState = BTN_STATE_DISABLE;
			m_Item[5].nState = BTN_STATE_NORMAL;
			
			g_dwSpareTime = 30000;
			g_bLight = TRUE;
			CSysConfig::Instance().SetSpareTime(0);
			CSysConfig::Instance().SetAutoBkLight(1);
		}
		break;
	case 1:
		{
			str.LoadString(RES_BUTTON_LCD_2);
			m_Item[4].chChar = str;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[5].nState = BTN_STATE_NORMAL;
			
			g_dwSpareTime = 60000*2;
			g_bLight = TRUE;
			CSysConfig::Instance().SetSpareTime(1);
			CSysConfig::Instance().SetAutoBkLight(1);
		}
		break;
	case 2:
		{
			str.LoadString(RES_BUTTON_LCD_3);
			m_Item[4].chChar = str;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[5].nState = BTN_STATE_NORMAL;
			
			g_dwSpareTime = 60000*5;
			g_bLight = TRUE;
			CSysConfig::Instance().SetSpareTime(2);
			CSysConfig::Instance().SetAutoBkLight(1);
		}
		break;
	case 3:
		{
			str.LoadString(RES_BUTTON_LCD_4);
			m_Item[4].chChar = str;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[5].nState = BTN_STATE_NORMAL;
			
			g_dwSpareTime = 60000*10;
			g_bLight = TRUE;
			CSysConfig::Instance().SetSpareTime(3);
			CSysConfig::Instance().SetAutoBkLight(1);
		}
		break;
	case 4:
		{
			str.LoadString(RES_BUTTON_LCD_5);
			m_Item[4].chChar = str;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[5].nState = BTN_STATE_NORMAL;
			
			g_dwSpareTime = 60000*15;
			g_bLight = TRUE;
			CSysConfig::Instance().SetSpareTime(4);
			CSysConfig::Instance().SetAutoBkLight(1);
		}
		break;
	case 5:
		{
			str.LoadString(RES_BUTTON_LCD_6);
			m_Item[4].chChar = str;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[5].nState = BTN_STATE_DISABLE;

			g_bLight = FALSE;
			CSysConfig::Instance().SetSpareTime(5);
			::KillTimer(0, CE9101MainApp::s_SpareTimer);
			CSysConfig::Instance().SetAutoBkLight(0);
		}
		break;
	default:
		break;
	}
}