// SMSNew.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPushToTalkSetting.h"
#include "../../SysConfig/SysConfig.h"
#include "../../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog
BOOL CDlgPushToTalkSetting::m_bIsGearSwitch = FALSE;

CDlgPushToTalkSetting::CDlgPushToTalkSetting(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgPushToTalkSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgPushToTalkSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPushToTalkSetting, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDlgPushToTalkSetting::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	return TRUE;
}
//变换语言和单位
BOOL CDlgPushToTalkSetting::OnLanguageChange()//变皮肤
{
	CResString str;
	
	//str.LoadString(RES_BUTTON_SMS_INBOX);
	m_csTitile = _T("一键通设置");
	
	return TRUE;
}

BOOL CDlgPushToTalkSetting::OnSkinChange()
{
	return TRUE;
}


void CDlgPushToTalkSetting::InitGdi()
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
	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_DRIVE_ECONOMY_SETTING_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_DRIVE_ECONOMY_SETTING_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgPushToTalkSetting::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgPushToTalkSetting::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}


void CDlgPushToTalkSetting::InitControl()
{
	for (int i =0;  i < enCtrCount; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[0].rect.left		= 172;
	m_Item[0].rect.top		= 200;
	m_Item[0].rect.right	= 263;
	m_Item[0].rect.bottom	= 288;
	m_Item[0].chChar		= _T("");
	if (1 == CSysConfig::Instance().GetPushToTalk())
		m_Item[0].nState = BTN_STATE_DOWN;
	else
		m_Item[0].nState = BTN_STATE_NORMAL;
	
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

void CDlgPushToTalkSetting::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgPushToTalkSetting::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgPushToTalkSetting::OnPaint() 
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
		DrawItems(&memDC, Rect, state);
		DrawBtnText(&memDC, Rect, str, CGDICommon::Instance()->smallfont(),RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}
	str = _T("设置一键通为拨打中心号码");
	Rect = m_Item[0].rect;
	Rect.left = Rect.right + 15;
	Rect.right = 800;
	DrawBtnText(&memDC, Rect, str, CGDICommon::Instance()->smallfont(), 
		RGB(255,255,255), DT_VCENTER|DT_LEFT);
	DrawBtnText(&memDC, g_retTitile, m_csTitile, CGDICommon::Instance()->bigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgPushToTalkSetting::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	if(PtInRect(&m_Item[3].rect,point))
//	{
//		OnExit();
//		return;
//	}

	for(int i=1;i<enCtrCount;i++)
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

void CDlgPushToTalkSetting::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=1;i<enCtrCount;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
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
			CSysConfig::Instance().SetPushToTalk(FALSE);
			m_bIsGearSwitch = TRUE;
		}
		else if( m_Item[0].nState == BTN_STATE_NORMAL )
		{
			m_Item[0].nState = BTN_STATE_DOWN;
			CSysConfig::Instance().SetPushToTalk(TRUE);
			m_bIsGearSwitch = FALSE;
		}
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}
