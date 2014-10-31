
#include "stdafx.h"
#include "../e9101main.h"
#include "DlgVehicleInfo.h"
#include "../E9101MainDlg.h"
#include "../Msg/MsgQueue.h"
#include "../DataType.h"

#include "../../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CE9101MainApp theApp;
extern UINT	g_nWM_MENU_GNSSTERMINAL;

CDlgVehicleInfo::CDlgVehicleInfo(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgVehicleInfo::IDD, pParent)
{
}

void CDlgVehicleInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgVehicleInfo, CDialogBase)
//{{AFX_MSG_MAP(CDlgVehicleInfo)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgVehicleInfo::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();
	
//	SetTimer(TID_REFRESHPAGE, 1000, NULL);
	
	return TRUE;
}

BOOL  CDlgVehicleInfo::OnLanguageChange()
{
	return TRUE;
}

BOOL  CDlgVehicleInfo::OnSkinChange()
{
	return TRUE;
}

void CDlgVehicleInfo::OnExit()
{
//	KillTimer(TID_REFRESHPAGE);
	
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
	
	CDialogBase::EndDialog(0);
}

void CDlgVehicleInfo::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_DIAODU_INFO_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_DIAODU_INFO_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgVehicleInfo::InitControl()
{
	int i;
	for(i=0;i<4;i++)
		m_Item[i].nState = BTN_STATE_NORMAL;


	m_Item[0].rect.left		= 190;
	m_Item[0].rect.top		= 145;
	m_Item[0].rect.right	= 325;
	m_Item[0].rect.bottom	= 275;
	m_Item[0].chChar	    = _T("空车");

	m_Item[1].rect.left		= 335;
	m_Item[1].rect.top		= 145;
	m_Item[1].rect.right	= 466;
	m_Item[1].rect.bottom	= 275;
	m_Item[1].chChar	    = _T("半载");

	m_Item[2].rect.left		= 478;
	m_Item[2].rect.top		= 145;
	m_Item[2].rect.right	= 611;
	m_Item[2].rect.bottom	= 275;
	m_Item[2].chChar	    = _T("满载");
	
	//Exit
	m_Item[3].rect.left	    = 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;
}

void CDlgVehicleInfo::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgVehicleInfo::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgVehicleInfo::OnPaint() 
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
	for(int i=0;i<4;i++)
	{
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		str		= m_Item[i].chChar;
		DrawItems(&memDC, Rect, state);
		Rect.bottom += 35;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_CENTER|DT_BOTTOM);

	}
	
	str = _T("电子运单");
	DrawBtnText(&memDC, g_retTitile, str, CGDICommon::Instance()->bigbigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_CENTER);	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgVehicleInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0; i<4; i++)
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

void CDlgVehicleInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i =0; i<4; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch(i)
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
			}
		}
	}
	Invalidate();
	
	CDialogBase::OnLButtonUp(nFlags, point);
}


LRESULT CDlgVehicleInfo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgVehicleInfo::OnTimer(UINT nIDEvent) 
{
	if ( TID_REFRESHPAGE == nIDEvent )
	{
		RefreshVehicleInfo();
		Invalidate();
	}
	else if ( TID_EXIT == nIDEvent )
	{
		KillTimer(nIDEvent);
		OnExit();
	}
	CDialogBase::OnTimer(nIDEvent);
}

VOID CDlgVehicleInfo::RefreshVehicleInfo()
{
	CString str;
	VEHICLE_INFO* pstVehicle = &g_pstShareMem->stVehicle;
	//车速
	if(pstVehicle->stTimeOut.bit.ucCluster1_Timeout == 0)
		str.Format(_T("%d"), pstVehicle->stCluster1.ucVechileSpeed);
	else
		str = _T("");
	m_Item[8].chChar	= str;
	
	//发动机转速
	if(pstVehicle->stTimeOut.bit.ucEngineSpeedTimeout == 0)		
		str.Format(_T("%d"), pstVehicle->usEngineSpeed / 8);
	else
		str = _T("");
	m_Item[9].chChar	= str;
	
	//冷却液温度
	if(pstVehicle->stTimeOut.bit.ucCoolentWaterTempTimeout == 0)	
		str.Format(_T("%d"), pstVehicle->ucCoolentTemp - 40);
	else
		str = _T("");
	m_Item[10].chChar	= str;
	
	//机油压力
	if(pstVehicle->stTimeOut.bit.ucOilPressTimeout == 0)
		str.Format(_T("%d"), pstVehicle->ucOilPress * 4);
	else
		str = _T("");
	m_Item[11].chChar	= str;
	
	//气压1
	if(pstVehicle->stTimeOut.bit.ucCluster1_Timeout == 0)
		str.Format(_T("%d"), pstVehicle->stCluster1.ucAirPresure1 * 4);
	else
		str = _T("");
	m_Item[12].chChar	= str;
	
	//气压2
	if(pstVehicle->stTimeOut.bit.ucCluster1_Timeout == 0)
		str.Format(_T("%d"), pstVehicle->stCluster1.ucAirPresure2 * 4);
	else
		str = _T("");
	m_Item[13].chChar	= str;
}