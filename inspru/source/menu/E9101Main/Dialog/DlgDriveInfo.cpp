
#include "stdafx.h"
#include "e9101main.h"
#include "DlgDriveInfo.h"

//#include "DlgDriveSecurity.h"
#include "DlgDriveEconomy.h"
//#include "DlgDriveRecord.h"
#include "../../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgDriveInfo::CDlgDriveInfo(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgDriveInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDriveInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgDriveInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDriveInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDriveInfo, CDialogBase)
	//{{AFX_MSG_MAP(CDlgDriveInfo)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDriveInfo message handlers

	//变换语言和单位
BOOL CDlgDriveInfo::OnLanguageChange()
{
	CResString str;

	m_csTitile = _T("驾驶信息");

	m_Item[0].chChar	= _T("驾驶安全管理");
	m_Item[1].chChar	= _T("驾驶经济管理");
	m_Item[2].chChar	= _T("行驶记录仪");

	return TRUE;
}

//变皮肤
BOOL CDlgDriveInfo::OnSkinChange()
{
	return TRUE;
}

void CDlgDriveInfo::InitGdi()
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
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_DRIVE_INFO_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_DRIVE_INFO_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgDriveInfo::OnExit( )
{
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

void CDlgDriveInfo::InitControl()
{
	for (int i =0; i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[0].rect.left		= 194;
	m_Item[0].rect.top		= 150;
	m_Item[0].rect.right	= 319;
	m_Item[0].rect.bottom	= 273;

	m_Item[1].rect.left		= 338;
	m_Item[1].rect.top		= 150;
	m_Item[1].rect.right	= 463;
	m_Item[1].rect.bottom	= 273;

	m_Item[2].rect.left		= 479;
	m_Item[2].rect.top		= 150;
	m_Item[2].rect.right	= 606;
	m_Item[2].rect.bottom	= 273;


	//Exit
	m_Item[3].rect.left		= 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;
}

void CDlgDriveInfo::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgDriveInfo::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgDriveInfo::OnPaint() 
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
	
	if( !CSysConfig::Instance().GetICCard() )
	{
		str = _T("Can总线未连接!");
		DrawBtnText(&memDC, CRect(214,342,587,418), str, CGDICommon::Instance()->bigbigfont(), 
			RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgDriveInfo::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CDlgDriveInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0; i<BTN_COUNT; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				OnDriveSecurity();
				break;
			case 1:
				//OnDriveEconomy();
				break;
			case 2:
				//OnDriveRecord();
				break;
			case 3:
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

BOOL CDlgDriveInfo::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	
	return TRUE;
}

//驾驶安全管理
VOID	CDlgDriveInfo::OnDriveSecurity()
{
	if( !CSysConfig::Instance().GetICCard() )
		return;

	//CDlgDriveSecurity dlg;
	//dlg.DoModal();
}

//驾驶经济管理
VOID	CDlgDriveInfo::OnDriveEconomy()
{
	CDlgDriveEconomy dlg;
	dlg.DoModal();
}

//行驶记录仪
VOID	CDlgDriveInfo::OnDriveRecord()
{
	//CDlgDriveRecord dlg;
	//dlg.DoModal();
}