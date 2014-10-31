
#include "stdafx.h"
#include "../e9101main.h"
#include "DlgOilConsum.h"
#include "../E9101MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgOilConsum::CDlgOilConsum(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgOilConsum::IDD, pParent)
{
}

void CDlgOilConsum::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgOilConsum, CDialogBase)
//{{AFX_MSG_MAP(CDlgOilConsum)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgOilConsum::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();
	
	SetTimer(TID_REFRESHPAGE, 1000, NULL);
	
	return TRUE;
}

BOOL  CDlgOilConsum::OnLanguageChange()
{
	m_csTitile = _T("油耗信息");
	return TRUE;
}

BOOL  CDlgOilConsum::OnSkinChange()
{
	return TRUE;
}

void CDlgOilConsum::OnExit()
{
	KillTimer(TID_REFRESHPAGE);
	
	//bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	//DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	//bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
	
	CDialogBase::EndDialog(0);
}

void CDlgOilConsum::InitGdi()
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
	//bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_OIL_CONSUM, true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	//pSkin = g_pResMng->RequestBmp(BMP_VEHICLE_INFO_P, true);
	//m_stBtnPressBMP = pSkin->hBitmap;
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	//m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgOilConsum::InitControl()
{
	int i;
	for(i=0;i<22;i++)
		m_Item[i].nState = BTN_STATE_NORMAL;
	
	//参数名
	for(i=0; i<7; i++)
	{
		m_Item[i].rect.left		= 90;
		m_Item[i].rect.top		= 89+56*i;
		m_Item[i].rect.right	= 90+180;
		m_Item[i].rect.bottom	= 89+49+56*i;
	}
	m_Item[0].chChar	= _T("参数名");
	m_Item[1].chChar	= _T("燃油油量");
	m_Item[2].chChar	= _T("累计里程");
	m_Item[3].chChar	= _T("瞬时油耗");
	m_Item[4].chChar	= _T("总油耗");
	//m_Item[5].chChar	= _T("");
	//m_Item[6].chChar	= _T("");
	
	//参数值
	for(i=7; i<14; i++)
	{
		m_Item[i].rect.left		= 400;
		m_Item[i].rect.top		= 89+56*(i-7);
		m_Item[i].rect.right	= 400+180;
		m_Item[i].rect.bottom	= 89+49+56*(i-7);
	}
	m_Item[7].chChar	= _T("参数值");

	//单位
	for(i=14; i<21; i++)
	{
		m_Item[i].rect.left		= 620;
		m_Item[i].rect.top		= 89+56*(i-14);
		m_Item[i].rect.right	= 620+180;
		m_Item[i].rect.bottom	= 89+49+56*(i-14);
	}
	m_Item[14].chChar	= _T("单位");
	m_Item[15].chChar	= _T("%");		//燃油油量
	m_Item[16].chChar	= _T("Km");		//累计里程
	m_Item[17].chChar	= _T("L/h");	//瞬时油耗
	m_Item[18].chChar	= _T("L");		//总油耗
	//m_Item[19].chChar	= _T("KPa");
	//m_Item[20].chChar	= _T("KPa");
	
	//Exit
	m_Item[21].rect.left	= 720;
	m_Item[21].rect.top		= 0;
	m_Item[21].rect.right	= 800;
	m_Item[21].rect.bottom	= 60;
}

void CDlgOilConsum::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DISABLE:
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgOilConsum::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgOilConsum::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	
	//绘制背景
	for(int i=0;i<22;i++)
	{
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		str		= m_Item[i].chChar;
		DrawItems(&memDC, Rect, state);
		if(i%7 != 0)
			DrawBtnText(&memDC, Rect, str, smallfont, RGB(255,255,255), DT_LEFT|DT_VCENTER);
		else
			DrawBtnText(&memDC, Rect, str, bigfont, RGB(255,255,255), DT_LEFT|DT_VCENTER);
	}
	//标题
	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	
	//str = _T("主菜单");
	//DrawBtnText(&memDC, g_retTitile, str, bigfont, RGB(255,255,255),DT_CENTER|DT_VCENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgOilConsum::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0; i<22; i++)
	{
		if(PtInRect(&m_Item[i].rect, point))
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

void CDlgOilConsum::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i =0; i<22; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch(i)
			{
			case 21:
				OnExit();
				break;
			}
		}
	}
	
	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

LRESULT CDlgOilConsum::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgOilConsum::OnTimer(UINT nIDEvent) 
{
	if ( TID_REFRESHPAGE == nIDEvent )
	{
		RefreshVehicleInfo();
		Invalidate();
	}
	CDialogBase::OnTimer(nIDEvent);
}

VOID CDlgOilConsum::RefreshVehicleInfo()
{
	//主菜单
	CString str;
	VEHICLE_INFO* pstVehicle = &g_pstShareMem->stVehicle;
	//燃油油量
	if(pstVehicle->stTimeOut.bit.ucCluster1_Timeout == 0)
		str.Format(_T("%d"), pstVehicle->stCluster1.ucFuelLevel);
	else
		str = _T("");
	m_Item[8].chChar	= str;
	
	//累计里程
	if(pstVehicle->stTimeOut.bit.ucCluster1_Timeout == 0)
		str.Format(_T("%d"), pstVehicle->stCluster1.dwODO);
	else
		str = _T("");
	m_Item[9].chChar	= str;
	
	//瞬时油耗
	if(pstVehicle->stTimeOut.bit.ucInstantOilConsumTimeout == 0)	
		str.Format(_T("%d"), pstVehicle->usInstantOilConsum / 20 );
	else
		str = _T("");
	m_Item[10].chChar	= str;
	
	//总油耗
	if(pstVehicle->stTimeOut.bit.ucTotalOilConsumTimeout == 0)
		str.Format(_T("%d"), pstVehicle->dwTotalOilConsum / 2 );
	else
		str = _T("");
	m_Item[11].chChar	= str;
}
