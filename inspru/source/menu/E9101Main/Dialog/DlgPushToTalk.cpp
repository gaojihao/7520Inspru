// SMSNew.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgPushToTalk.h"
#include "../GSMDial.h"
#include "../NumSetDlg.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../E9101MainDlg.h"
#include "../DlgConfirm2.h"
#include "../Msg/MsgQueue.h"
#include "../DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog
extern UINT	g_nWM_MENU_GNSSTERMINAL;

CDlgPushToTalk::CDlgPushToTalk(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgPushToTalk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgPushToTalk::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgPushToTalk, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgPushToTalk::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	return TRUE;
}
//变换语言和单位
BOOL CDlgPushToTalk::OnLanguageChange()//变皮肤
{
	CResString str;
	
	//str.LoadString(RES_BUTTON_SMS_INBOX);
	m_csTitile = _T("一键通");
	
	return TRUE;
}

BOOL CDlgPushToTalk::OnSkinChange()
{
	return TRUE;
}


void CDlgPushToTalk::InitGdi()
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
	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_PUSH_TO_TALK_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_PUSH_TO_TALK_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgPushToTalk::ReleaseGdi()
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

void CDlgPushToTalk::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CDlgPushToTalk::InitControl()
{
	for (int i =0;  i < enCtrCount; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	//设置
	m_Item[0].rect.left		= 196;
	m_Item[0].rect.top		= 87;
	m_Item[0].rect.right	= 316;
	m_Item[0].rect.bottom	= 206;
	m_Item[0].chChar		= _T("设置");
	//服务中心
	m_Item[1].rect.left		= 338;
	m_Item[1].rect.top		= 87;
	m_Item[1].rect.right	= 462;
	m_Item[1].rect.bottom	= 206;
	m_Item[1].chChar		= _T("服务中心");
	//医院
	m_Item[2].rect.left		= 482;
	m_Item[2].rect.top		= 87;
	m_Item[2].rect.right	= 605;
	m_Item[2].rect.bottom	= 206;
	m_Item[2].chChar		= _T("医院");
	//消防
	m_Item[3].rect.left		= 268;
	m_Item[3].rect.top		= 264;
	m_Item[3].rect.right	= 391;
	m_Item[3].rect.bottom	= 388;
	m_Item[3].chChar		= _T("消防");
	//报警
	m_Item[4].rect.left		= 412;
	m_Item[4].rect.top		= 264;
	m_Item[4].rect.right	= 536;
	m_Item[4].rect.bottom	= 388;
	m_Item[4].chChar		= _T("报警");

	//Exit
	m_Item[5].rect.left		= 720;
	m_Item[5].rect.top		= 0;
	m_Item[5].rect.right	= 800;
	m_Item[5].rect.bottom	= 60;
}

void CDlgPushToTalk::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgPushToTalk::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgPushToTalk::OnPaint() 
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
		Rect.bottom += 25;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}
	
	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgPushToTalk::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i=0;i<enCtrCount;i++)
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

void CDlgPushToTalk::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<enCtrCount;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				OnSetNum();
				break;
			case 1:
				OnCenter();
				break;
			case 2:
				OnHospital();
				break;
			case 3:
				OnFire();
				break;
			case 4:
				OnAlarm();
				break;
			case 5: 
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

VOID CDlgPushToTalk::OnSetNum()
{
	//电话短信功能开关
	if( !CE9101MainDlg::m_bPhone )
	{
		CDlgConfirm2::m_stTitle = _T("无权限使用该功能!");
		CDlgConfirm2 dlg;
		dlg.DoModal();
		return;
	}
	
	CNumSetDlg dlg;
	dlg.DoModal();
}

VOID CDlgPushToTalk::OnCenter()
{
	if( 0 == g_GsmLogic.GetGSMNetType() )
	{
		CDlgConfirm2::m_stTitle = _T("系统正在查找网络......");
		CDlgConfirm2 dlg(3);		  
		dlg.DoModal();
		return;
	}

	CString csNum;
	csNum = CSysConfig::Instance().GetCenterNum();
	if(csNum.IsEmpty())
	{
		return;
	}
	CGSMDial dlg;
	CGSMDial::m_UserNum = csNum;
	CGSMDial::m_bAutoDial = TRUE;
	dlg.DoModal();
}

VOID CDlgPushToTalk::OnHospital()
{
	if( 0 == g_GsmLogic.GetGSMNetType() )
	{
		CDlgConfirm2::m_stTitle = _T("系统正在查找网络......");
		CDlgConfirm2 dlg(3);		  
		dlg.DoModal();
		return;
	}

	CString csNum;
	csNum = CSysConfig::Instance().GetHospitalNum();
	if(csNum.IsEmpty())
	{
		return;
	}
	CGSMDial dlg;
	CGSMDial::m_UserNum = csNum;
	CGSMDial::m_bAutoDial = TRUE;
	dlg.DoModal();
}

VOID CDlgPushToTalk::OnFire()
{
	if( 0 == g_GsmLogic.GetGSMNetType() )
	{
		CDlgConfirm2::m_stTitle = _T("系统正在查找网络......");
		CDlgConfirm2 dlg(3);		  
		dlg.DoModal();
		return;
	}

	CString csNum;
	csNum = CSysConfig::Instance().GetFireNum();
	if(csNum.IsEmpty())
	{
		return;
	}
	CGSMDial dlg;
	CGSMDial::m_UserNum = csNum;
	CGSMDial::m_bAutoDial = TRUE;
	dlg.DoModal();
}

VOID CDlgPushToTalk::OnAlarm()
{
	if( 0 == g_GsmLogic.GetGSMNetType() )
	{
		CDlgConfirm2::m_stTitle = _T("系统正在查找网络......");
		CDlgConfirm2 dlg(3);		  
		dlg.DoModal();
		return;
	}

	CString csNum;
	csNum = CSysConfig::Instance().GetAlarmNum();
	if(csNum.IsEmpty())
	{
		return;
	}
	CGSMDial dlg;
	CGSMDial::m_UserNum = csNum;
	CGSMDial::m_bAutoDial = TRUE;
	dlg.DoModal();
	
}