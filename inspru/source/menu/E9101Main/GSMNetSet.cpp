// GSMNetSet.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "GSMNetSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGSMNetSet dialog

CGSMNetSet::CGSMNetSet(CWnd* pParent /*=NULL*/)
	: CDialogBase(CGSMNetSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGSMNetSet)
	//}}AFX_DATA_INIT
}



void CGSMNetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGSMNetSet)
	DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BTN_LANGUAGE_F, m_BtnF);
	DDX_Control(pDX, IDC_BTN_LANGUAGE_B, m_BtnB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGSMNetSet, CDialogBase)
	//{{AFX_MSG_MAP(CGSMNetSet)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGSMNetSet message handlers

BOOL CGSMNetSet::OnLanguageChange()
{
	CResString str;

	str.LoadString(RES_DIAL_NET_SET);
	m_csTitile = str;
	
	switch( CSysConfig::Instance().GetGSMSelect() )
	{
		//AutoSelected
	case 0:
		{
			str.LoadString(RES_GSM_NET_AUTO);
			m_Item[5].nState = BTN_STATE_DISABLE;
			m_Item[3].nState = BTN_STATE_DISABLE;
			m_Item[1].chChar = str;
			m_Item[0].nState = BTN_STATE_DISABLE;
			//m_Item[2].nState = BTN_STATE_NORMAL;
			m_Item[2].nState = BTN_STATE_DISABLE;
			break;
		}
		//手动选择
	case 1:
		{
			str.LoadString(RES_GSM_NET_USER_SET);
			m_Item[1].chChar = str;
			m_Item[2].nState = BTN_STATE_DISABLE;
			m_Item[0].nState = BTN_STATE_NORMAL;
			if (  CSysConfig::Instance().GetGSMSelect() == 0 )
			{
				m_Item[3].nState = BTN_STATE_DISABLE;
				m_Item[5].nState = BTN_STATE_NORMAL;
			}else
			{
				m_Item[5].nState = BTN_STATE_DISABLE;
				m_Item[3].nState = BTN_STATE_NORMAL;
			}
			break;
		}
	default:
		break;
	}


	switch( g_GsmLogic.GetGSMNetType() )
	{
	case gsm_net_china_mobile:
		{
			str.LoadString(RES_GSM_TYPE_CMCC);
			m_Item[4].chChar = str;
			break;
		}
	case gsm_net_china_unicom:
		{
			str.LoadString(RES_GSM_TYPE_CUC);
			m_Item[4].chChar = str;
			break;
		}
	case gsm_net_china_CUGSM:
		{
			//str.LoadString(RES_GSM_TYPE_CUC);
			m_Item[4].chChar = _T("中国联通3G");
			break;
		}
	default:
		break;
	}


	return TRUE;
}
	//变皮肤
BOOL CGSMNetSet::OnSkinChange()
{
	return TRUE;	
}

//////////////////////////////////////////////////////////////////////////

BOOL CGSMNetSet::OnInitDialog()
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	return TRUE;
}





void CGSMNetSet::OnBtnCancel() 
{
	CDialogBase::OnCancel();
}

LRESULT CGSMNetSet::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnExit();
		}
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}


void CGSMNetSet::InitGdi()
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
	bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_BRIGHTNESS_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BG_BRIGHTNESS_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	pSkin = g_pResMng->RequestBmp(BG_BRIGHTNESS_D, true);
	m_stBtnDisableBMP = pSkin->hBitmap; 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CGSMNetSet::ReleaseGdi()
{

	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CGSMNetSet::OnExit()
{
	//int nSel = CSysConfig::Instance().GetGSMSelect();
	//int nType = CSysConfig::Instance().GetGSMType();
	//g_GsmLogic.GSMSetNet(nSel, nType);
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CGSMNetSet::InitControl()
{
	for (int i =0;  i < 7; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[1].nState = BTN_STATE_NORMAL;
	m_Item[4].nState = BTN_STATE_NORMAL;
	
	m_Item[5].nState = BTN_STATE_DISABLE;
	m_Item[3].nState = BTN_STATE_DISABLE;

	m_Item[0].rect.top = 169;
	m_Item[0].rect.left = 114;
	m_Item[0].rect.bottom = 238;
	m_Item[0].rect.right = 191;

	m_Item[1].rect.top = 169;
	m_Item[1].rect.left = 210;
	m_Item[1].rect.bottom = 238;
	m_Item[1].rect.right = 590;

	m_Item[2].rect.top = 169;
	m_Item[2].rect.left = 610;
	m_Item[2].rect.bottom = 238;
	m_Item[2].rect.right = 685;

	m_Item[3].rect.top = 292;
	m_Item[3].rect.left = 114;
	m_Item[3].rect.bottom = 361;
	m_Item[3].rect.right = 116;

	m_Item[4].rect.top = 292;
	m_Item[4].rect.left = 210;
	m_Item[4].rect.bottom = 361;
	m_Item[4].rect.right = 590;

	m_Item[5].rect.top = 292;
	m_Item[5].rect.left = 610;
	m_Item[5].rect.bottom = 361;
	m_Item[5].rect.right = 685;

	m_Item[6].rect.top = 0;
	m_Item[6].rect.left = 740;
	m_Item[6].rect.bottom = 60;
	m_Item[6].rect.right = 800;
}

void CGSMNetSet::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CGSMNetSet::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CGSMNetSet::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<7;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

		str = m_Item[1].chChar;
 		Rect = m_Item[1].rect;
 		state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
		str = m_Item[4].chChar;
 		Rect = m_Item[4].rect;
 		state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CGSMNetSet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0;i<7;i++)
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

void CGSMNetSet::OnLButtonUp(UINT nFlags, CPoint point) 
{                       
	for(int i=0;i<7;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
				switch( i )
					{
					case 0:
						OnLeft();
						break;
					case 2:
						OnRight();
						break;
					case 3:
						OnSkinLeft();
						break;
					case 5:
						OnSkinRight();
						break;
					case 6:
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



void CGSMNetSet::OnLeft()
{
	CResString str;
	int nSel = CSysConfig::Instance().GetGSMSelect();
	if ( nSel == 1 ) nSel = 0;
	switch( nSel )
	{
	case 0:
		{
			str.LoadString(RES_GSM_NET_AUTO);
			m_Item[5].nState = BTN_STATE_DISABLE;
			m_Item[3].nState = BTN_STATE_DISABLE;
			break;
		}	
	case 1:
		{
			str.LoadString(RES_GSM_NET_USER_SET);
			if (  CSysConfig::Instance().GetGSMSelect() == 0 )
			{
				m_Item[3].nState = BTN_STATE_DISABLE;
				m_Item[5].nState = BTN_STATE_NORMAL;
			}else
			{
				m_Item[5].nState = BTN_STATE_DISABLE;
				m_Item[3].nState = BTN_STATE_NORMAL;
			}

			break;
		}
	default:
		break;
	}
	
	m_Item[1].chChar = str;
	CSysConfig::Instance().SetGSMSelect( nSel );
	m_Item[0].nState = BTN_STATE_DISABLE;
	m_Item[2].nState = BTN_STATE_NORMAL;

	Invalidate();
}

void CGSMNetSet::OnRight()
{
	CResString str;
	int nSel = CSysConfig::Instance().GetGSMSelect();
	if ( nSel == 0 ) nSel = 1;
	switch( nSel )
	{
	case 0:
		{
			str.LoadString(RES_GSM_NET_AUTO);
   			m_Item[5].nState = BTN_STATE_DISABLE;
			m_Item[3].nState = BTN_STATE_DISABLE;
			break;
		}	
	case 1:
		{
			str.LoadString(RES_GSM_NET_USER_SET);
			if (  CSysConfig::Instance().GetGSMSelect() == 0 )
			{
				m_Item[3].nState = BTN_STATE_DISABLE;
				m_Item[5].nState = BTN_STATE_NORMAL;
			}else
			{
				m_Item[5].nState = BTN_STATE_DISABLE;
				m_Item[3].nState = BTN_STATE_NORMAL;
			}

			break;
		}
	default:
		break;
	}
	
	m_Item[1].chChar = str;
	m_Item[2].nState = BTN_STATE_DISABLE;
	m_Item[0].nState = BTN_STATE_NORMAL;

	CSysConfig::Instance().SetGSMSelect( nSel );
	Invalidate();
}

void CGSMNetSet::OnSkinLeft()
{
	CResString str;
	int nSel = CSysConfig::Instance().GetGSMType();
	if ( nSel == 1 ) nSel = 0;
	switch( nSel )
	{
	case 0:	str.LoadString(RES_GSM_TYPE_CMCC); 	break;
	case 1: str.LoadString(RES_GSM_TYPE_CUC);   break;
	default: break;
	}

	m_Item[4].chChar = str;	
	CSysConfig::Instance().SetGSMType( nSel );
	m_Item[3].nState = BTN_STATE_DISABLE;
	m_Item[5].nState = BTN_STATE_NORMAL;
	
	Invalidate();
}

void CGSMNetSet::OnSkinRight()
{
	CResString str;
	int nSel = CSysConfig::Instance().GetGSMType();
	if ( nSel == 0 ) nSel = 1;
	switch( nSel )
	{
	case 0: str.LoadString(RES_GSM_TYPE_CMCC);break;
	case 1:	str.LoadString(RES_GSM_TYPE_CUC);break;
	default:break;
	}
	m_Item[4].chChar = str;
	
	CSysConfig::Instance().SetGSMType( nSel );
	m_Item[5].nState = BTN_STATE_DISABLE;
	m_Item[3].nState = BTN_STATE_NORMAL;
	Invalidate();
}
