
#include "stdafx.h"
#include "e9101main.h"
#include "DlgICCard.h"

#include "DlgICLogin.h"
#include "DlgBarCodeScan.h"
#include "DlgComSetting.h"
#include "../../MutiLanguage/CGDICommon.h"
#include "../../SCom/scom_api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

serial_win32	g_serial_ic;

CDlgICCard::CDlgICCard(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgICCard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgICCard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgICCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgICCard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgICCard, CDialogBase)
	//{{AFX_MSG_MAP(CDlgICCard)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgICCard message handlers

	//变换语言和单位
BOOL CDlgICCard::OnLanguageChange()
{
	CResString str;

	m_csTitile = _T("扩展应用");

	m_Item[0].chChar	= _T("IC卡登录");
	m_Item[1].chChar	= _T("条形码扫描");
	m_Item[2].chChar	= _T("RFID");
	m_Item[3].chChar	= _T("串口设置");

	return TRUE;
}

//变皮肤
BOOL CDlgICCard::OnSkinChange()
{
	return TRUE;
}

void CDlgICCard::InitGdi()
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
	//bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_EXT_APP_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_EXT_APP_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgICCard::OnExit( )
{
	//scom_api_close();
	//g_serial_ic.close();
	//bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	//DeleteObject(m_stBtnDisableBMP);	
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

void CDlgICCard::InitControl()
{
	for (int i =0; i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[0].rect.left		= 124;
	m_Item[0].rect.top		= 150;
	m_Item[0].rect.right	= 250;
	m_Item[0].rect.bottom	= 275;

	m_Item[1].rect.left		= 266;
	m_Item[1].rect.top		= 150;
	m_Item[1].rect.right	= 393;
	m_Item[1].rect.bottom	= 275;

	m_Item[2].rect.left		= 408;
	m_Item[2].rect.top		= 150;
	m_Item[2].rect.right	= 536;
	m_Item[2].rect.bottom	= 275;

	m_Item[3].rect.left		= 550;
	m_Item[3].rect.top		= 150;
	m_Item[3].rect.right	= 677;
	m_Item[3].rect.bottom	= 275;


	//Exit
	m_Item[4].rect.left		= 720;
	m_Item[4].rect.top		= 0;
	m_Item[4].rect.right	= 800;
	m_Item[4].rect.bottom	= 60;
}

void CDlgICCard::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgICCard::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgICCard::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0; i<BTN_COUNT; i++)
	{
		//绘制背景
		str		= m_Item[i].chChar;
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.bottom += 35;
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgICCard::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0; i<BTN_COUNT; i++)
	{
		if( PtInRect(&m_Item[i].rect,point) )
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

void CDlgICCard::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0; i<BTN_COUNT; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				OnICLogin();
				break;
			case 1:
				OnBarCodeScan();
				break;
			case 2:
				OnRFID();
				break;
			case 3://设置串口
				//OnComSetting();
				break;
			case 4:
				OnExit();
				break;
			default:
				break;
			}
		}
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp( nFlags, point );
}

BOOL CDlgICCard::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	//bool bScom = scom_api_start(2, 9600);
	//g_serial_ic.open(2, 9600);
	
	return TRUE;
}

//IC卡登录
VOID	CDlgICCard::OnICLogin()
{
	CDlgICLogin dlg;
	dlg.DoModal();
}

//条形码扫描
VOID	CDlgICCard::OnBarCodeScan()
{
	CDlgBarCodeScan dlg;
	dlg.DoModal();
}

//RFID
VOID	CDlgICCard::OnRFID()
{

}

//设置串口
VOID	CDlgICCard::OnComSetting()
{
	CDlgComSetting dlg;
	dlg.DoModal();
}
