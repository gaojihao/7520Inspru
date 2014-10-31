// DlgGameManual.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgGameManual.h" 
#include "E9101MainDlg.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGameManual dialog


CDlgGameManual::CDlgGameManual(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgGameManual::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGameManual)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGameManual::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGameManual)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGameManual, CDialogBase)
//{{AFX_MSG_MAP(CDlgGameManual)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGameManual message handlers

LRESULT CDlgGameManual::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	if( message == WM_CREATE_SET )
// 	{
// 		if( wParam == 0x03 )
// 		{
// 			EndDialog(0);
// 		}
//	}	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

BOOL CDlgGameManual::OnLanguageChange()
{
	CResString str;
/*
	str.LoadString(RES_BTN_INFO);
	m_stTitle = str;

	str.LoadString(RES_BUTTON_SCREEN);
//	m_BtnCarlibra.SetWindowText(str);
	m_Item[0].chChar = str;

	str.LoadString(RES_BTN_DEFAULT_SET);
//	m_BtnGps.SetWindowText(str);
	m_Item[1].chChar = str;
*/
	return TRUE;
}
	//±äÆ¤·ô
BOOL CDlgGameManual::OnSkinChange()
{
// 	SkinSetDlgStyle(KDS_CTRL_FKTEXT|KDS_BTN_AUTOBMP);
// 	SkinSetTitlePos(CPoint(170,6));
// 	SkinSetResID(bmp_bg_language);
// 
// 	m_BtnExit.SetBtnStyle(KDS_BTN_AUTOBMP |KDS_TEXT_VCENTER | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT)
// 		.SetBtnResID(bmp_file_back);
// 	m_BtnCarlibra.SetBtnResID(bmp_file_SetChild_Calibrater);
// 	m_BtnCarlibra.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );
// 	m_BtnGps.SetBtnResID(bmp_file_SetChild_GpsInfo);
//	m_BtnGps.SetBtnFontID( SF_BUTTON_NORMAL_CHINESE );

	return TRUE;
}


BOOL CDlgGameManual::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here

// 	m_BtnExit.SetWindowPos(&CWnd::wndTop,0,0,0,0,NULL);
// 	m_BtnGps.SetWindowPos(&CWnd::wndTop,48,70,0,0,SWP_NOSIZE|SWP_NOZORDER);
//	m_BtnCarlibra.SetWindowPos(&CWnd::wndTop,240,70,0,0,SWP_NOSIZE|SWP_NOZORDER);

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();

	if(m_IniFile.Load( _T("\\Flashdrv Storage\\menu\\GameManual.ini") ))
	{
 		m_IniFile.GetString(_T("GameInfo"),_T("MANUAL"),m_ItemSysInfo[0].chChar);
	}
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CDlgGameManual::OnButtonInfoExit() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CDlgGameManual::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

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
		_T("ËÎÌå\0")); 


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
		_T("ËÎÌå\0"));             // lpszFacename
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
		_T("ËÎÌå\0"));  

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	//bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_GAME_MANUAL,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	//pSkin = g_pResMng->RequestBmp(BMP_BG_GAME_MANUAL, true);
	//m_stBtnPressBMP = pSkin->hBitmap;    

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	//m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgGameManual::ReleaseGdi()
{
//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	//bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

//	DeleteObject(m_stBtnDisBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

//	bk_disable_dc.DeleteDC();
	//bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgGameManual::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);

}

void CDlgGameManual::InitControl()
{
	for (int i =0;  i < 1; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	for (int j =0;  j < 1; j++)
	{
		m_ItemSysInfo[j].nState = BTN_STATE_DISABLE;
	}

	m_ItemSysInfo[0].rect.top    = 53;
	m_ItemSysInfo[0].rect.left   = 22;
	m_ItemSysInfo[0].rect.bottom = 244;
	m_ItemSysInfo[0].rect.right  = 456;
	m_ItemSysInfo[0].chChar      = _T("");
	
	m_Item[0].rect.top = 0;
	m_Item[0].rect.left = 430;
	m_Item[0].rect.bottom = 40;
	m_Item[0].rect.right = 480;
}

void CDlgGameManual::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgGameManual::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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
void CDlgGameManual::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<1;i++)
	{
		//»æÖÆ±³¾°
			str = m_Item[i].chChar;
			Rect = m_Item[i].rect;
			state = m_Item[i].nState;
			DrawItems(&memDC,Rect,state);
			Rect.top = Rect.top ;
			DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255));
	}

	for(int j=0;j<1;j++)
	{
		//»æÖÆ±³¾°
		//if(j==1)
			//str = m_ItemSysInfo[j].chChar.Left(29);
		//else
			str = m_ItemSysInfo[j].chChar;
			Rect = m_ItemSysInfo[j].rect;
			DrawItems(&memDC,Rect,state);
			DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255), DT_LEFT | DT_TOP | DT_WORDBREAK);
	}

	//Rect.top = 125;
	//Rect.left = 157;
	//Rect.bottom = 190;
	//Rect.right = 400;
	//str = m_ItemSysInfo[1].chChar.Right(m_ItemSysInfo[1].chChar.GetLength()-29);

	//DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255), DT_LEFT | DT_TOP );


	//DrawBtnText(&memDC,g_retTitile,m_stTitle,bigfont,RGB(255,255,255),DT_LEFT | DT_TOP);


	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CDlgGameManual::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<1;i++)
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

void CDlgGameManual::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<1;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			
				switch( i )
					{
 					case 0:
 						EndDialog(0);
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