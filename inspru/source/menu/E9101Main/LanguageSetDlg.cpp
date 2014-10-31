// LanguageSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "E9101Main.h"
#include "LanguageSetDlg.h"
#include "SysConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLanguageSetDlg dialog
#define WM_LANGUAGE_CHANGE  		WM_USER + 14

CLanguageSetDlg::CLanguageSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CLanguageSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLanguageSetDlg)
	//}}AFX_DATA_INIT
}



void CLanguageSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLanguageSetDlg)
//	DDX_Control(pDX, IDC_STA_LANGUAGE_COUNTRY, m_stcContoury);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BTN_LANGUAGE_F, m_BtnF);
	DDX_Control(pDX, IDC_BTN_LANGUAGE_B, m_BtnB);
//	DDX_Control(pDX, IDC_STA_LANGUAGE_COUNTRY_ICO, m_stcLanguage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLanguageSetDlg, CDialogBase)
	//{{AFX_MSG_MAP(CLanguageSetDlg)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE_F, OnBtnF)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE_B, OnBtnB)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLanguageSetDlg message handlers

BOOL CLanguageSetDlg::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_BTN_LANGUAGE_SET);
	m_csTitile = str;
	

	switch(CSysConfig::Instance().GetLanguage())
	{
	case ID_CHINESE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_CHINESE);
			m_Item[1].chChar = str;
			break;
		}
	case ID_ENGLISH:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ENGLISH);
			m_Item[1].chChar = str;
			break;
		}
	default:
		break;
	}
	m_Item[1].chChar = str;

	switch(CSysConfig::Instance().GetSkinType())
	{
	case ID_SKIN1:
		{
			str.LoadString(RES_BTN_SKIN1);
			m_Item[4].chChar = str;
			break;
		}	
	case ID_SKIN2:
		{
			str.LoadString(RES_BTN_SKIN2);
			m_Item[4].chChar = str;
			break;
		}
	default:
		break;
	}


	return TRUE;
}
	//变皮肤
BOOL CLanguageSetDlg::OnSkinChange()
{

	return TRUE;	
}

//////////////////////////////////////////////////////////////////////////

BOOL CLanguageSetDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();
	
	//确定当前语言列表位置
	m_eLan = CSysConfig::Instance().GetLanguage();

	for( UINT i=1; i<=CResString::GetLanguageCount(); i++)
	{
		if(m_eLan == CResString::GetValidLanguage(i))
				m_ListInitPos = i;
	}

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();

	return TRUE;
}


void CLanguageSetDlg::OnBtnB() 
{
	// TODO: Add your control notification handler code here

	CResString str;
	m_ListInitPos--;

	LONG LanPos = CResString::GetLanguageCount();
	
	if(m_ListInitPos <= 0)
		m_ListInitPos = LanPos;

	m_eLan = CResString::GetValidLanguage(m_ListInitPos);

	switch(m_eLan)
	{
	case ID_CHINESE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_CHINESE);
			break;
		}
	case ID_ENGLISH:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ENGLISH);
			break;
		}
	case ID_GERMANY:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_GERMAN);
			break;
		}	
	case ID_FRANCE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_FRANK);
			break;
		}
	case ID_PORTUGAL:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_PORTUGAL);
			break;
		}	
	case ID_ITALY:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ITALY);
			break;
		}
	case ID_RUSSIA:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_RUSSIA);
			break;
		}
	default:
		break;
	}
	
	m_Item[1].chChar = str;

	CSysConfig::Instance().SetLanguage(m_eLan);
	Invalidate();

	//修改子进程配置文件
	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();

	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();
	
	::PostMessage(HWND_BROADCAST, WM_LANGUAGE_CHANGE , (WPARAM)ID_CHINESE, 0  );
	return;
}

void CLanguageSetDlg::OnBtnF() 
{
	// TODO: Add your control notification handler code here

	
	CResString str;
	m_ListInitPos++;

	LONG LanPos = CResString::GetLanguageCount();
	
	if(m_ListInitPos > LanPos)
		m_ListInitPos = 1;

	m_eLan = CResString::GetValidLanguage(m_ListInitPos);

	switch(m_eLan)
	{
	case ID_CHINESE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_CHINESE);
			break;
		}
	case ID_ENGLISH:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ENGLISH);
			break;
		}
	case ID_GERMANY:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_GERMAN);
			break;
		}	
	case ID_FRANCE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_FRANK);
			break;
		}
	case ID_PORTUGAL:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_PORTUGAL);
			break;
		}	
	case ID_ITALY:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ITALY);
			break;
		}
	case ID_RUSSIA:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_RUSSIA);
			break;
		}
	default:
		break;
	}
	
	m_Item[1].chChar = str;

	CSysConfig::Instance().SetLanguage(m_eLan);
	Invalidate();

	//修改子进程配置文件
	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();

	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(ID_CHINESE));
	iniFile.Flush();
	
	::PostMessage(HWND_BROADCAST, WM_LANGUAGE_CHANGE , (WPARAM)ID_CHINESE, 0  );
	return;
}



void CLanguageSetDlg::OnBtnCancel() 
{
	CDialogBase::OnCancel();
}

LRESULT CLanguageSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnExit();
		}
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}


void CLanguageSetDlg::InitGdi()
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

void CLanguageSetDlg::ReleaseGdi()
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
}

void CLanguageSetDlg::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CLanguageSetDlg::InitControl()
{
	for (int i =0;  i < 7; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[1].nState = BTN_STATE_NORMAL;
	m_Item[4].nState = BTN_STATE_NORMAL;

	if( ID_SKIN1 == CSysConfig::Instance().GetSkinType() )
	{
		m_Item[5].nState = BTN_STATE_DISABLE;
		m_Item[3].nState = BTN_STATE_DISABLE;
	}
	else if( ID_SKIN2 == CSysConfig::Instance().GetSkinType() )
	{
		m_Item[5].nState = BTN_STATE_DISABLE;
		m_Item[3].nState = BTN_STATE_DISABLE;
	}


	m_Item[0].rect.top    = 170;
	m_Item[0].rect.left   = 115;
	m_Item[0].rect.bottom = 239;
	m_Item[0].rect.right  = 191;

	m_Item[1].rect.top    = 170;
	m_Item[1].rect.left   = 210;
	m_Item[1].rect.bottom = 239;
	m_Item[1].rect.right  = 590;

	m_Item[2].rect.top    = 170;
	m_Item[2].rect.left   = 610;
	m_Item[2].rect.bottom = 239;
	m_Item[2].rect.right  = 685;

	m_Item[3].rect.top = 292;
	m_Item[3].rect.left = 115;
	m_Item[3].rect.bottom = 360;
	m_Item[3].rect.right = 191;

	m_Item[4].rect.top = 292;
	m_Item[4].rect.left = 210;
	m_Item[4].rect.bottom = 360;
	m_Item[4].rect.right = 590;

	m_Item[5].rect.top = 292;
	m_Item[5].rect.left = 610;
	m_Item[5].rect.bottom = 360;
	m_Item[5].rect.right = 685;

	m_Item[6].rect.top = 0;
	m_Item[6].rect.left = 740;
	m_Item[6].rect.bottom = 60;
	m_Item[6].rect.right = 800;
}

void CLanguageSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CLanguageSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CLanguageSetDlg::OnPaint() 
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
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
		str = m_Item[4].chChar;
 		Rect = m_Item[4].rect;
 		state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CLanguageSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
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

void CLanguageSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<7;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			
				
				switch( i )
					{
					case 0:
						OnLeft();
						break;
					case 2:
						OnRight();
						break;
					case 3:
						OnSkinLeft();
						break;
					case 5:
						OnSkinRight();
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
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}



void CLanguageSetDlg::OnLeft()
{
//	CSysConfig::Instance().SetLanguage( ID_CHINESE );
//	m_eLan = ID_CHINESE;
//	m_Item[0].nState = BTN_STATE_DISABLE;
//	m_Item[2].nState = BTN_STATE_NORMAL;


	CResString str;
	m_ListInitPos--;

	LONG LanPos = CResString::GetLanguageCount();
	
	if(m_ListInitPos <= 0)
		m_ListInitPos = LanPos;

	m_eLan = CResString::GetValidLanguage(m_ListInitPos);

	switch(m_eLan)
	{
	case ID_CHINESE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_CHINESE);
			break;
		}
	case ID_ENGLISH:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ENGLISH);
			break;
		}
	}
	
	m_Item[1].chChar = str;
	CSysConfig::Instance().SetLanguage(m_eLan);
	Invalidate();
	
	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();
	
	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();
	::PostMessage(HWND_BROADCAST,WM_SKIN_CHANGE , 0, 0  );
	ReleaseGdi();
	InitGdi();
}

void CLanguageSetDlg::OnRight()
{

//	CSysConfig::Instance().SetLanguage( ID_ENGLISH );
//	m_eLan = ID_ENGLISH;
//	m_Item[0].nState = BTN_STATE_NORMAL;
//	m_Item[2].nState = BTN_STATE_DISABLE;

	CResString str;
	m_ListInitPos++;

	LONG LanPos = CResString::GetLanguageCount();
	
	if(m_ListInitPos > LanPos)
		m_ListInitPos = 1;

	m_eLan = CResString::GetValidLanguage(m_ListInitPos);

	switch(m_eLan)
	{
	case ID_CHINESE:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_CHINESE);
			break;
		}
	case ID_ENGLISH:
		{
			str.LoadString(RES_SYSSET_LANGUAGE_ENGLISH);
			break;
		}
	default:
		break;
	}
	
	m_Item[1].chChar = str;
	CSysConfig::Instance().SetLanguage(m_eLan);
	Invalidate();


	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();
	
	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Language"),int(m_eLan));
	iniFile.Flush();
	::PostMessage(HWND_BROADCAST,WM_SKIN_CHANGE , 0, 0  );
	ReleaseGdi();
	InitGdi();
}

void CLanguageSetDlg::OnSkinLeft()
{
	CSysConfig::Instance().SetSkinType( ID_SKIN1 );
	m_eLan = ID_CHINESE;
	m_Item[3].nState = BTN_STATE_DISABLE;
	m_Item[5].nState = BTN_STATE_DISABLE;

	CResString str;
	str.LoadString(RES_BTN_SKIN1);
	m_Item[4].chChar = str;


	Invalidate();

	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN1));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN1));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN1));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN1));
	iniFile.Flush();
	
	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN1));
	iniFile.Flush();
	
	::PostMessage(HWND_BROADCAST,WM_SKIN_CHANGE , (WPARAM)ID_SKIN1, 0  );

	ReleaseGdi();
	InitGdi();
}

void CLanguageSetDlg::OnSkinRight()
{
	CSysConfig::Instance().SetSkinType( ID_SKIN2 );
	m_eLan = ID_CHINESE;
	m_Item[3].nState = BTN_STATE_DISABLE;
	m_Item[5].nState = BTN_STATE_DISABLE;

	CResString str;
	str.LoadString(RES_BTN_SKIN2);
	m_Item[4].chChar = str;

	Invalidate();

	CIniFile iniFile;
	iniFile.Load(g_csVolumeIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN2));
	iniFile.Flush();	

	iniFile.Load(g_csPicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN2));
	iniFile.Flush();

	iniFile.Load(g_csMusicIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN2));
	iniFile.Flush();

	iniFile.Load(g_csMovieIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN2));
	iniFile.Flush();
	
	iniFile.Load(g_csTxtIconName);
	iniFile.SetInt(_T("CommonSet"),_T("Skin"),int(ID_SKIN2));
	iniFile.Flush();
	
	::PostMessage(HWND_BROADCAST,WM_SKIN_CHANGE , (WPARAM)ID_SKIN2, 0  );

	ReleaseGdi();
	InitGdi();
}
