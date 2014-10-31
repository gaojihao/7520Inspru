// SMSNew.cpp : implementation file
//

#include "stdafx.h"
#include "DlgDriveEconomySetting.h"
#include "../../SysConfig/SysConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog
BOOL CDlgDriveEconomySetting::m_bIsGearSwitch = FALSE;

CDlgDriveEconomySetting::CDlgDriveEconomySetting(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgDriveEconomySetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgDriveEconomySetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDriveEconomySetting, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDlgDriveEconomySetting::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	return TRUE;
}
//变换语言和单位
BOOL CDlgDriveEconomySetting::OnLanguageChange()//变皮肤
{
	CResString str;
	
	//str.LoadString(RES_BUTTON_SMS_INBOX);
	m_csTitile = _T("驾驶经济管理");
	
	return TRUE;
}

BOOL CDlgDriveEconomySetting::OnSkinChange()
{
	return TRUE;
}


void CDlgDriveEconomySetting::InitGdi()
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
	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_DRIVE_ECONOMY_SETTING_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_DRIVE_ECONOMY_SETTING_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgDriveEconomySetting::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgDriveEconomySetting::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}


void CDlgDriveEconomySetting::InitControl()
{
	for (int i =0;  i < enCtrCount; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[0].rect.left		= 472;
	m_Item[0].rect.top		= 222;
	m_Item[0].rect.right	= 572;
	m_Item[0].rect.bottom	= 280;
	m_Item[0].chChar		= _T("");
	if (1 == CSysConfig::Instance().GetGearSwitch())
		m_Item[0].nState = BTN_STATE_NORMAL;
	else
		m_Item[0].nState = BTN_STATE_DOWN;
	

	m_Item[1].rect.left		= 0;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 0;
	m_Item[1].rect.bottom	= 0;
	m_Item[1].chChar		= _T("");

	m_Item[2].rect.left		= 0;
	m_Item[2].rect.top		= 0;
	m_Item[2].rect.right	= 0;
	m_Item[2].rect.bottom	= 0;
	m_Item[2].chChar		= _T("");
	//Exit
	m_Item[3].rect.left		= 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;
}

void CDlgDriveEconomySetting::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgDriveEconomySetting::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgDriveEconomySetting::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	
	for(int i=0;i<enCtrCount;i++)
	{
		//绘制背景
		str		= m_Item[i].chChar;
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}
	str = _T("驾驶行为指导");
	DrawBtnText(&memDC,CRect(46,212,442,289),str,smallfont,RGB(255,255,255),DT_VCENTER|DT_RIGHT);
	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgDriveEconomySetting::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(PtInRect(&m_Item[3].rect,point))
	{
		OnExit();
		return;
	}

	for(int i=0;i<enCtrCount;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			if(m_Item[3].nState == BTN_STATE_NORMAL)
			{
				m_Item[3].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}
	
	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgDriveEconomySetting::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<enCtrCount;i++)
	{
		if(m_Item[3].nState==BTN_STATE_DOWN)
		{
			m_Item[3].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3: 
				OnExit();
				break;
			default:
				break;
			}
		}
	}
	if(PtInRect(&m_Item[0].rect,point))
	{
		if( m_Item[0].nState == BTN_STATE_DOWN )
		{
			m_Item[0].nState = BTN_STATE_NORMAL;
			CSysConfig::Instance().SetGearSwitch(TRUE);
			m_bIsGearSwitch = TRUE;
		}
		else if( m_Item[0].nState == BTN_STATE_NORMAL )
		{
			m_Item[0].nState = BTN_STATE_DOWN;
			CSysConfig::Instance().SetGearSwitch(FALSE );
			m_bIsGearSwitch = FALSE;
		}
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}
