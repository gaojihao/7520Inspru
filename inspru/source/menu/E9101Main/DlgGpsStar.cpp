// DlgGpsStar.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgGpsStar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGpsStar dialog 
#define		MSG_EURP_START			( WM_USER + 0x1000 )
#define		MSG_SYS_BEGIN			( MSG_EURP_START + 1 )
#define		MSG_LOCATION_DATA		( MSG_SYS_BEGIN  + 2 ) // location message.


DlgGpsStar::DlgGpsStar(CWnd* pParent /*=NULL*/)
	: CDialogBase(DlgGpsStar::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGpsStar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hbkBrush = ::CreateSolidBrush(RGB(255, 255, 255));
	m_hbkGpsState = ::CreateSolidBrush(RGB(65, 146, 73));
}


void DlgGpsStar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGpsStar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_GPSSTAR_EXIT, m_BtnExit);
	DDX_Control(pDX, IDC_STATIC_SKYVIEW, m_stcAlmanac);
	DDX_Control(pDX, IDC_STATIC_SATELLITESIG, m_stcSatellite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGpsStar, CDialogBase)
	//{{AFX_MSG_MAP(DlgGpsStar)
	ON_BN_CLICKED(IDC_BTN_GPSSTAR_EXIT, OnBtnGpsstarExit)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGpsStar message handlers

BOOL DlgGpsStar::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	m_stcAlmanac.MoveWindow(17,80,307,380); 
	m_stcSatellite.MoveWindow(324,80,456,380);

	m_nMsgIDPosRefresh	= RegisterWindowMessage( WM_POS_DATA_REFRESH );



	
	OnLanguageChange();
	OnSkinChange();

	InitGdi();
	InitControl();

	UserRefresh();
	
	return TRUE;
}

BOOL DlgGpsStar::OnLanguageChange()
{

	smallfont.CreateFont(
		-18,							// nHeight
		0,								// nWidth
		0,								// nEscapement
		0,								// nOrientation
		FW_SEMIBOLD,					// nWeight
		FALSE,							// bItalic
		FALSE,							// bUnderline
		0,								// cStrikeOut
		ANSI_CHARSET,					// nCharSet
		OUT_DEFAULT_PRECIS,				// nOutPrecision
		CLIP_DEFAULT_PRECIS,			// nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,		// nPitchAndFamily
		_T("宋体\0"));					// lpszFacename

		bigfont.CreateFont(
		32,								// nHeight
		12,								// nWidth
		0,								// nEscapement
		0,								// nOrientation
		FW_SEMIBOLD,					// nWeight
		FALSE,							// bItalic
		FALSE,							// bUnderline
		0,								// cStrikeOut
		ANSI_CHARSET,					// nCharSet
		OUT_DEFAULT_PRECIS,				// nOutPrecision
		CLIP_DEFAULT_PRECIS,			// nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,		// nPitchAndFamily
		_T("宋体\0"));					// lpszFacename
		
	CResString str;
	
	str.LoadString(RES_BUTTON_BLUETOOTH);
	m_stTitle = str;


	return TRUE;
}

	//变皮肤
BOOL DlgGpsStar::OnSkinChange()
{
	return TRUE;
}

void DlgGpsStar::OnBtnGpsstarExit() 
{
	EndDialog(0);
}

LRESULT DlgGpsStar::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void DlgGpsStar::HandleStardata()
{
	SStarData starData[13];
	memset(&starData, 0, sizeof(starData));

	int numLock = 0;
	int numView = 0;
	int i = 0;

	for(int i=0; i<MAX_SATE_NUM; i++)
	{
		if (m_pPosInfo->gsv_data[i].wSatID != 0 )
		{
			starData[numView].id = m_pPosInfo->gsv_data[i].wSatID;
			starData[numView].signal = m_pPosInfo->gsv_data[i].wSNR;
			starData[numView].horizon = m_pPosInfo->gsv_data[i].wElevation;
			starData[numView].direction = m_pPosInfo->gsv_data[i].wAzimuth;
			starData[numView].tracked = 0;
			numView++;
		}	
	}


	//找出定位的卫星
	SStarData starView[12];
	memset(&starView, 0, sizeof(starView));

	for (i = 0; i < MAX_SATE_NUM; i++)
	{
		for (int k = 0; k < MAX_SATE_NUM; k++)
		{
			if (m_pPosInfo->gsa_data.wUsedID[i] == starData[k].id)
			{
				starData[k].tracked = 1;
			}
		}
	}

	// 对卫星序号排序
	for (i=0; i<numView; i++)
	{
		for (int j=i+1; j<numView; j++)
		{
			if (starData[i].id > starData[j].id)
			{
				SStarData data;
				data = starData[i];
				starData[i] = starData[j];
				starData[j] = data;
			}
		}
	}

	//过滤掉重复的卫星
	int isatellitenum = 0;
	for (i = 0; i < numView-1; i++)
	{
		while (starData[i].id == starData[i+1].id)
		{
			i++;
		}

		starData[isatellitenum].id			= starData[i].id;
		starData[isatellitenum].signal		= starData[i].signal;
		starData[isatellitenum].direction	= starData[i].direction;
		starData[isatellitenum].horizon	= starData[i].horizon;
		starData[isatellitenum].tracked	= starData[i].tracked;
		isatellitenum++;
	}

	

	// 过滤掉没有位置的卫星
	SStarData viewstar[12];
	int num = 0;
	for (i=0; i<isatellitenum; i++)
	{
		if ((starData[i].direction==-1 || starData[i].horizon==-1) ||
			(starData[i].direction==0 || starData[i].horizon==0))
		{
			continue;
		}
		viewstar[num] = starData[i];
		num++;
	}

	
	m_stcSatellite.SetStarData(starData, isatellitenum);
	m_stcAlmanac.SetStarData(viewstar, num);
}

void DlgGpsStar::UserRefresh()
{	
	HandleStardata();
	m_stcAlmanac.Invalidate();
	m_stcSatellite.Invalidate();
}

HBRUSH DlgGpsStar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);
	if (m_hbkBrush != NULL)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_hbkBrush;
	}
	return hbr;
}

void DlgGpsStar::OnTimer(UINT nIDEvent) 
{
	CDialogBase::OnTimer(nIDEvent);
}

BOOL DlgGpsStar::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == m_nMsgIDPosRefresh )
	{
		UserRefresh();
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}

void DlgGpsStar::OnExit()
{
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnNormalBMP);
	
	bk_normal_dc.DeleteDC();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(0);
}

void DlgGpsStar::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
   	bk_normal_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_BACKGROUND,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	::ReleaseDC(m_hWnd, hDC);
}


void DlgGpsStar::InitControl()
{

	m_Item.nState = BTN_STATE_NORMAL;

	m_Item.rect.top = 0;
	m_Item.rect.left = 740;
	m_Item.rect.bottom = 60;
	m_Item.rect.right = 800;
}

void DlgGpsStar::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void DlgGpsStar::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void DlgGpsStar::OnPaint() 
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
	Rect = m_Item.rect;
	state = m_Item.nState;
	DrawItems(&memDC,Rect,state);

	DrawBtnText(&memDC,g_retTitile,m_stTitle,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void DlgGpsStar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PlayKeySound();
	if(m_Item.nState == BTN_STATE_NORMAL)
	{
		m_Item.nState = BTN_STATE_DOWN;
	}

	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void DlgGpsStar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_Item.nState==BTN_STATE_DOWN)
	{
		m_Item.nState = BTN_STATE_NORMAL;
	}
	OnExit();
	CDialogBase::OnLButtonUp(nFlags, point);
}
