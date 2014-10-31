// SMSNew.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgSecurityMonitor.h"
#include "../../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog

CDlgSecurityMonitor::CDlgSecurityMonitor(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgSecurityMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgSecurityMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSecurityMonitor, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDlgSecurityMonitor::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	return TRUE;
}
//变换语言和单位
BOOL CDlgSecurityMonitor::OnLanguageChange()//变皮肤
{
	
	CResString str;
	
	//str.LoadString(RES_BUTTON_SMS_INBOX);
	m_csTitile = _T("安全监控");
	
	return TRUE;
}

BOOL CDlgSecurityMonitor::OnSkinChange()
{
	return TRUE;
}


void CDlgSecurityMonitor::InitGdi()
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
	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_SECURITY_MONITOR_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_SECURITY_MONITOR_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgSecurityMonitor::ReleaseGdi()
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

void CDlgSecurityMonitor::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}


void CDlgSecurityMonitor::InitControl()
{
	for (int i =0;  i < enCtrCount; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	//视频选择
	m_Item[0].rect.left		= 0;
	m_Item[0].rect.top		= 148;
	m_Item[0].rect.right	= 0;
	m_Item[0].rect.bottom	= 272;
	m_Item[0].chChar		= _T("视频");
	//监控回放
	m_Item[1].rect.left		= 194;
	m_Item[1].rect.top		= 148;
	m_Item[1].rect.right	= 317;
	m_Item[1].rect.bottom	= 272;
	m_Item[1].chChar		= _T("监控回放");
	//查看抓拍
	m_Item[2].rect.left		= 337;
	m_Item[2].rect.top		= 148;
	m_Item[2].rect.right	= 464;
	m_Item[2].rect.bottom	= 272;
	m_Item[2].chChar		= _T("查看抓拍");
	//设置
	m_Item[3].rect.left		= 479;
	m_Item[3].rect.top		= 148;
	m_Item[3].rect.right	= 605;
	m_Item[3].rect.bottom	= 272;
	m_Item[3].chChar		= _T("设置");
	//Exit
	m_Item[4].rect.left		= 720;
	m_Item[4].rect.top		= 0;
	m_Item[4].rect.right	= 800;
	m_Item[4].rect.bottom	= 60;
}

void CDlgSecurityMonitor::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgSecurityMonitor::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgSecurityMonitor::OnPaint() 
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
		Rect.bottom += 35;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgSecurityMonitor::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
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

void CDlgSecurityMonitor::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<enCtrCount;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				{
					CString strTemp;
					GetModuleFileName(NULL,strTemp.GetBuffer(60),60);
					strTemp.ReleaseBuffer();
					strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
					CString strPath = strTemp + _T("\\Video.exe");
					CreateProcess(strPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				}
				break;
			case 1:
				CreateProcess(g_csMovieName, NULL, NULL, NULL, 
					FALSE, 0, NULL, NULL, NULL, NULL);
				break;
			case 2:
				break;
			case 3:
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
	CDialogBase::OnLButtonUp(nFlags, point);
}