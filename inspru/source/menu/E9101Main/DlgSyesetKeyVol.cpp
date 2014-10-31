// DlgSyesetKeyVol.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgSyesetKeyVol.h"
#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSyesetKeyVol dialog
int CDlgSyesetKeyVol::m_nVolLev = 0;	//初始化音量等级

CDlgSyesetKeyVol::CDlgSyesetKeyVol(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgSyesetKeyVol::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSyesetKeyVol)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSyesetKeyVol::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSyesetKeyVol)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSyesetKeyVol, CDialogBase)
	//{{AFX_MSG_MAP(CDlgSyesetKeyVol)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSyesetKeyVol message handlers

//变换语言和单位
BOOL CDlgSyesetKeyVol::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_BUTTON_KEYVOL_TITLE);
	m_csTitile = str;

	str.LoadString( RES_BUTTON_SYSVOL );
	m_Item[4].chChar =  str;

	switch( g_enKeySnd )
	{
	case KeySnd_none:
		str.LoadString(RES_BUTTON_KEYVOL_0);
		break;
	case KeySnd_1:
		str.LoadString(RES_BUTTON_KEYVOL_1);
		break;
	case KeySnd_2:
		str.LoadString(RES_BUTTON_KEYVOL_2);
		break;
	case KeySnd_3:
		str.LoadString(RES_BUTTON_KEYVOL_3);
		break;
	case KeySnd_4:
		str.LoadString(RES_BUTTON_KEYVOL_4);
		break;
	default:
		str.LoadString(RES_BUTTON_KEYVOL_0);
		break;
	}
	m_Item[1].chChar = str;
	
	return TRUE; 
}

void CDlgSyesetKeyVol::OnLeft()
{
	int ileft  = (int)g_enKeySnd;

	ileft--;
	g_enKeySnd = enKeySound(ileft);

	CSysConfig::Instance().SetKeySoundType( g_enKeySnd );

	CResString str;
	switch( g_enKeySnd )
	{
	case KeySnd_none:
		str.LoadString(RES_BUTTON_KEYVOL_0);
		break;
	case KeySnd_1:
		str.LoadString(RES_BUTTON_KEYVOL_1);
		break;
	case KeySnd_2:
		str.LoadString(RES_BUTTON_KEYVOL_2);
		break;
	case KeySnd_3:
		str.LoadString(RES_BUTTON_KEYVOL_3);
		break;
	case KeySnd_4:
		str.LoadString(RES_BUTTON_KEYVOL_4);
		break;
	default:
		str.LoadString(RES_BUTTON_KEYVOL_0);
		break;
	}
	m_Item[1].chChar = str;
	UpdateBnStatus();
/*
	m_Item[2].nState = BTN_STATE_NORMAL;
	m_Item[0].nState = BTN_STATE_NORMAL;
	if( ileft == 0 )
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		m_Item[2].nState = BTN_STATE_NORMAL;
	}
	Invalidate();
	
	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();
	
	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();	
	*/
}

void CDlgSyesetKeyVol::OnRight()
{
	int iRight  = (int)g_enKeySnd;

	iRight++;
	g_enKeySnd = enKeySound(iRight);

	CSysConfig::Instance().SetKeySoundType( g_enKeySnd );

	CResString str;
	switch( g_enKeySnd )
	{
	case KeySnd_none:
		str.LoadString(RES_BUTTON_KEYVOL_0);
		break;
	case KeySnd_1:
		str.LoadString(RES_BUTTON_KEYVOL_1);
		break;
	case KeySnd_2:
		str.LoadString(RES_BUTTON_KEYVOL_2);
		break;
	case KeySnd_3:
		str.LoadString(RES_BUTTON_KEYVOL_3);
		break;
	case KeySnd_4:
		str.LoadString(RES_BUTTON_KEYVOL_4);
		break;
	default:
		str.LoadString(RES_BUTTON_KEYVOL_0);
		break;
	}
	m_Item[1].chChar = str;
	UpdateBnStatus();
/*
	m_Item[2].nState = BTN_STATE_NORMAL;
	m_Item[0].nState = BTN_STATE_NORMAL;
	if( iRight == 4 )
	{
		m_Item[2].nState = BTN_STATE_DISABLE;
		m_Item[0].nState = BTN_STATE_NORMAL;
	}
	Invalidate();

	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();
	
	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("Set"),_T("KeyVol"),int(g_enKeySnd));
	iniFile.Flush();	
*/
}

	//变皮肤
BOOL CDlgSyesetKeyVol::OnSkinChange()
{
	return TRUE; 
}

void CDlgSyesetKeyVol::InitGdi()
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

void CDlgSyesetKeyVol::OnExit()
{
	SetKeyType();

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

void CDlgSyesetKeyVol::InitControl()
{
	for (int i =0;  i < 7; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
//声音左
	m_Item[0].rect.left   = 127;
	m_Item[0].rect.top    = 123;
	m_Item[0].rect.right  = 285;
	m_Item[0].rect.bottom = 269;
//声音
	m_Item[1].rect.left   = 286;
	m_Item[1].rect.top    = 123;
	m_Item[1].rect.right  = 519;
	m_Item[1].rect.bottom = 269;
//声音右
	m_Item[2].rect.left   = 520;
	m_Item[2].rect.top    = 123;
	m_Item[2].rect.right  = 664;
	m_Item[2].rect.bottom = 269;
//音量左
	m_Item[3].rect.left		= 127;
	m_Item[3].rect.top		= 276;
	m_Item[3].rect.right	= 285;
	m_Item[3].rect.bottom	= 404;
//音量
	m_Item[4].rect.left		= 286;
	m_Item[4].rect.top		= 276;
	m_Item[4].rect.right	= 519;
	m_Item[4].rect.bottom	= 404;
//音量右
	m_Item[5].rect.left		= 520;
	m_Item[5].rect.top		= 276;
	m_Item[5].rect.right	= 664;
	m_Item[5].rect.bottom	= 404;
//Exit
	m_Item[6].rect.left = 740;
	m_Item[6].rect.top = 0;
	m_Item[6].rect.right = 800;
	m_Item[6].rect.bottom = 60;
/*
	DWORD   dwVol;
	if(::waveOutGetVolume(0, &dwVol) != MMSYSERR_NOERROR)
	{
		return;
	}
	m_nVolLev = LOWORD(dwVol);

	m_nVolLev = m_nVolLev * 10 / 0x7FFF;
	if ( m_nVolLev <= 0) 
		m_nVolLev = 0;
	if ( m_nVolLev >= 10 ) 
		m_nVolLev = 10;

	CSysConfig::Instance().SetSysVol( m_nVolLev );

	if ( m_nVolLev <= 0 )
	{
		m_Item[3].nState = BTN_STATE_DISABLE;
		m_Item[5].nState = BTN_STATE_NORMAL;
	}
	else if ( m_nVolLev >= 10 )
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[5].nState = BTN_STATE_DISABLE;
	}
	else
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[5].nState = BTN_STATE_NORMAL;
	}*/
}

void CDlgSyesetKeyVol::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgSyesetKeyVol::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgSyesetKeyVol::OnPaint() 
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

	str.Format(_T("%s%d"), m_Item[4].chChar, m_nVolLev );
	Rect = m_Item[4].rect;
	state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_CENTER | DT_VCENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CDlgSyesetKeyVol::OnLButtonDown(UINT nFlags, CPoint point) 
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

	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgSyesetKeyVol::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<7;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0://声音类型
				OnLeft();
				break;
			case 2://声音类型
				OnRight();
				break;
			case 3: //音量-
				OnAdjustVolumn(-1);
				break;
			case 5:	//音量+
				OnAdjustVolumn(1);
				break;
			case 6:
				OnExit();
				break;
			default:
				break;
			}
		}
	}

	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

BOOL CDlgSyesetKeyVol::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	
	UpdateBnStatus();

	SetTimer(100, 2000, NULL);

	SetWindowText(_T("Sound_Setting"));
	
	return TRUE;
}
/*
void CDlgSyesetKeyVol::OnLeftVol()
{
	m_nVolLev--;
	if ( m_nVolLev <= 0 ) m_nVolLev = 0;
	CSysConfig::Instance().SetSysVol( m_nVolLev );
	unsigned  long nVal = 0x7FFF * m_nVolLev / 10;
	DWORD dwVol = MAKELONG( nVal, nVal );
	::waveOutSetVolume(0, dwVol );
	if ( m_nVolLev == 0 )
	{
		m_Item[3].nState = BTN_STATE_DISABLE;
		m_Item[5].nState = BTN_STATE_NORMAL;
	}
	else if ( m_nVolLev == 10 )
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[5].nState = BTN_STATE_DISABLE;
	}else
	{
		m_Item[5].nState = BTN_STATE_NORMAL;
		m_Item[3].nState = BTN_STATE_NORMAL;
	}
	Invalidate();
}

void CDlgSyesetKeyVol::OnRightVol()
{
	m_nVolLev++;
	if ( m_nVolLev >= 10 ) m_nVolLev = 10;
	CSysConfig::Instance().SetSysVol( m_nVolLev );
	unsigned  long nVal = 0x7FFF * m_nVolLev / 10;
	DWORD dwVol = MAKELONG( nVal, nVal );
	::waveOutSetVolume(0, dwVol );
	if ( m_nVolLev == 0 )
	{
		m_Item[3].nState = BTN_STATE_DISABLE;
		m_Item[5].nState = BTN_STATE_NORMAL;
	}
	else if ( m_nVolLev == 10 )
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[5].nState = BTN_STATE_DISABLE;
	}else
	{
		m_Item[5].nState = BTN_STATE_NORMAL;
		m_Item[3].nState = BTN_STATE_NORMAL;
	}
	Invalidate();
}
*/
//音量
void CDlgSyesetKeyVol::OnAdjustVolumn(int nDifference)
{
	//音量等级0~10
	if(m_nVolLev+nDifference < 0 || m_nVolLev+nDifference > 10)
		return;

	m_nVolLev += nDifference;
	SetValue();	//设置数值
	UpdateBnStatus();//更新按钮状态
}
//设置数值
void CDlgSyesetKeyVol::SetValue()
{
	CSysConfig::Instance().SetSysVol( m_nVolLev );
	//unsigned  long nVal = 0x7FFF * m_nVolLev / 10;
	unsigned  long nVal = 0xFFFF * m_nVolLev / 10;
	DWORD dwVol = MAKELONG( nVal, nVal );
	::waveOutSetVolume(0, dwVol );
}

//更新按钮状态
void CDlgSyesetKeyVol::UpdateBnStatus()
{
	//声音类型
	int iRight  = (int)g_enKeySnd;
	if( iRight == 0 )
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		m_Item[2].nState = BTN_STATE_NORMAL;
	}
	if( iRight == 4 )
	{
		m_Item[0].nState = BTN_STATE_NORMAL;
		m_Item[2].nState = BTN_STATE_DISABLE;
	}

	//音量调节按钮
	if ( m_nVolLev == 0 )
	{
		m_Item[3].nState = BTN_STATE_DISABLE;
		m_Item[5].nState = BTN_STATE_NORMAL;
	}
	else if ( m_nVolLev == 10 )
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[5].nState = BTN_STATE_DISABLE;
	}else
	{
		m_Item[5].nState = BTN_STATE_NORMAL;
		m_Item[3].nState = BTN_STATE_NORMAL;
	}
}

void CDlgSyesetKeyVol::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(100 != nIDEvent)
	{
		return;
	}
	CDialogBase::OnTimer(nIDEvent);
}
LRESULT CDlgSyesetKeyVol::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			//OnExit();
		}
	}
	else if(WM_ADJUST_BRIGHTNESS_VOLUMN == message)
	{
		UpdateBnStatus();
		Invalidate();
	}
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
void CDlgSyesetKeyVol::SetKeyType()
{
	CIniFile	IniFile;
	IniFile.Load(_T("\\Flashdrv Storage\\Mp3\\Config.ini"));
	IniFile.SetInt(_T("Set"), _T("KeyVol"), g_enKeySnd);
	IniFile.Flush();

	IniFile.Load(_T("\\Flashdrv Storage\\Mp4\\Config.ini"));
	IniFile.SetInt(_T("Set"), _T("KeyVol"), g_enKeySnd);
	IniFile.Flush();

	IniFile.Load(_T("\\Flashdrv Storage\\JP\\Config.ini"));
	IniFile.SetInt(_T("Set"), _T("KeyVol"), g_enKeySnd);
	IniFile.Flush();

	IniFile.Load(_T("\\Flashdrv Storage\\Txt\\Config.ini"));
	IniFile.SetInt(_T("Set"), _T("KeyVol"), g_enKeySnd);
	IniFile.Flush();
}