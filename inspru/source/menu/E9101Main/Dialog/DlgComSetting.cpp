// DlgComSetting.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgComSetting.h"
#include "SysConfig.h"
#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgComSetting::CDlgComSetting(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgComSetting::IDD, pParent)
{
	m_nComICCard	= CSysConfig::Instance().GetComICCard();
	m_nComScanner	= CSysConfig::Instance().GetComScanner();
	m_nComRFID		= CSysConfig::Instance().GetComRFID();
}

void CDlgComSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgComSetting)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgComSetting, CDialogBase)
	//{{AFX_MSG_MAP(CDlgComSetting)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgComSetting message handlers

BOOL CDlgComSetting::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	UpdateBnStatus();

	return TRUE;
}

BOOL CDlgComSetting::OnLanguageChange()
{
	CResString str;
	m_csTitile = _T("串口设置");
	return TRUE;
}

//变皮肤
BOOL CDlgComSetting::OnSkinChange()
{
	return TRUE;
}

void CDlgComSetting::OnBtnExit() 
{
	OnExit();
}

void CDlgComSetting::InitGdi()
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
	//bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_COM_SETTING_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_COM_SETTING_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	//pSkin = g_pResMng->RequestBmp(BG_BRIGHTNESS_D, true);
	//m_stBtnDisableBMP = pSkin->hBitmap; 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgComSetting::OnExit( )
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

void CDlgComSetting::InitControl()
{
	for (int i =0;  i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	//ICCard-
	m_Item[0].rect.left		= 233;
	m_Item[0].rect.top		= 86;
	m_Item[0].rect.right	= 360;
	m_Item[0].rect.bottom	= 206;
	//
	m_Item[1].rect.left		= 361;
	m_Item[1].rect.top		= 86;
	m_Item[1].rect.right	= 559;
	m_Item[1].rect.bottom	= 206;
	//ICCard+
	m_Item[2].rect.left		= 560;
	m_Item[2].rect.top		= 86;
	m_Item[2].rect.right	= 680;
	m_Item[2].rect.bottom	= 206;
	//////////////////////////////////////////////////////////////////////////
	//Scanner-
	m_Item[3].rect.left		= 233;
	m_Item[3].rect.top		= 207;
	m_Item[3].rect.right	= 360;
	m_Item[3].rect.bottom	= 318;
	//
	m_Item[4].rect.left		= 361;
	m_Item[4].rect.top		= 207;
	m_Item[4].rect.right	= 559;
	m_Item[4].rect.bottom	= 318;
	//Scanner+
	m_Item[5].rect.left		= 560;
	m_Item[5].rect.top		= 207;
	m_Item[5].rect.right	= 680;
	m_Item[5].rect.bottom	= 318;
	//////////////////////////////////////////////////////////////////////////
	//RFID-
	m_Item[6].rect.left		= 233;
	m_Item[6].rect.top		= 319;
	m_Item[6].rect.right	= 360;
	m_Item[6].rect.bottom	= 432;
	//
	m_Item[7].rect.left		= 361;
	m_Item[7].rect.top		= 319;
	m_Item[7].rect.right	= 559;
	m_Item[7].rect.bottom	= 432;
	//RFID+
	m_Item[8].rect.left		= 560;
	m_Item[8].rect.top		= 319;
	m_Item[8].rect.right	= 680;
	m_Item[8].rect.bottom	= 432;
	//Exit
	m_Item[9].rect.left		= 740;
	m_Item[9].rect.top		= 0;
	m_Item[9].rect.right	= 800;
	m_Item[9].rect.bottom	= 60;
}

void CDlgComSetting::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgComSetting::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgComSetting::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<BTN_COUNT;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),
			RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}

	str = _T("IC卡串口：");
	Rect = m_Item[0].rect;
	Rect.left = 40;
	DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),
		RGB(255,255,255),DT_VCENTER|DT_LEFT);
	str = _T("条形码扫描串口：");
	Rect = m_Item[3].rect;
	Rect.left = 40;
	DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),
		RGB(255,255,255),DT_VCENTER|DT_LEFT);
	str = _T("RFID串口：");
	Rect = m_Item[6].rect;
	Rect.left = 40;
	DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),
		RGB(255,255,255),DT_VCENTER|DT_LEFT);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CDlgComSetting::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0;i<BTN_COUNT;i++)
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

void CDlgComSetting::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<BTN_COUNT;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0://ICCard
				OnAdjustICCard(COM_FRONT);
				break;
			case 2:
				OnAdjustICCard(COM_BACK);
				break;
			case 3://Scanner
				OnAdjustScanner(COM_FRONT);
				break;
			case 5:
				OnAdjustScanner(COM_BACK);
				break;
			case 6://RFID
				OnAdjustRFID(COM_FRONT);
				break;
			case 8:
				OnAdjustRFID(COM_BACK);		
				break;
			case 9://Exit
				OnBtnExit();
				break;
			default:
				break;
			}
		}
	}

	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

//ICCard
void CDlgComSetting::OnAdjustICCard(int nDifference)
{
	if( (COM_FRONT == nDifference && COM_FRONT == m_nComICCard)			//已经是前串口且设为前串口
		|| (COM_BACK == nDifference && COM_BACK == m_nComICCard) )		//已经是后串口且设为后串口
	{
		return;
	}

	m_nComICCard = nDifference;
	CSysConfig::Instance().SetComICCard(m_nComICCard);
	UpdateBnStatus();
}
//扫描枪
void CDlgComSetting::OnAdjustScanner(int nDifference)
{
	if( (COM_FRONT == nDifference && COM_FRONT == m_nComScanner)			//已经是前串口且设为前串口
		|| (COM_BACK == nDifference && COM_BACK == m_nComScanner) )		//已经是后串口且设为后串口
	{
		return;
	}

	m_nComScanner = nDifference;

	CSysConfig::Instance().SetComScanner(m_nComScanner);
	UpdateBnStatus();
}
//RFID
void CDlgComSetting::OnAdjustRFID(int nDifference)
{
	if( (COM_FRONT == nDifference && COM_FRONT == m_nComRFID)			//已经是前串口且设为前串口
		|| (COM_BACK == nDifference && COM_BACK == m_nComRFID) )		//已经是后串口且设为后串口
	{
		return;
	}

	m_nComRFID = nDifference;

	CSysConfig::Instance().SetComRFID(m_nComRFID);
	UpdateBnStatus();
}

LRESULT CDlgComSetting::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			//OnBtnExit();
		}
 	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
//更新按钮状态
void CDlgComSetting::UpdateBnStatus()
{
	//ICCard
	if(m_nComICCard == COM_FRONT)
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		m_Item[1].chChar = _T("前串口");
		m_Item[2].nState = BTN_STATE_NORMAL;
	}
	else if(m_nComICCard == COM_BACK)
	{
		m_Item[0].nState = BTN_STATE_NORMAL;
		m_Item[1].chChar = _T("后串口");
		m_Item[2].nState = BTN_STATE_DISABLE;
	}
	//////////////////////////////////////////////////////////////////////////
	//扫描枪
	if(m_nComScanner == COM_FRONT)
	{
		m_Item[3].nState = BTN_STATE_DISABLE;
		m_Item[4].chChar = _T("前串口");
		m_Item[5].nState = BTN_STATE_NORMAL;
	}
	else if(m_nComScanner == COM_BACK)
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[4].chChar = _T("后串口");
		m_Item[5].nState = BTN_STATE_DISABLE;
	}
	//////////////////////////////////////////////////////////////////////////
	//RFID
	if(m_nComRFID == COM_FRONT)
	{
		m_Item[6].nState = BTN_STATE_DISABLE;
		m_Item[7].chChar = _T("前串口");
		m_Item[8].nState = BTN_STATE_NORMAL;
	}
	else if(m_nComRFID == COM_BACK)
	{
		m_Item[6].nState = BTN_STATE_NORMAL;
		m_Item[7].chChar = _T("后串口");
		m_Item[8].nState = BTN_STATE_DISABLE;
	}
}