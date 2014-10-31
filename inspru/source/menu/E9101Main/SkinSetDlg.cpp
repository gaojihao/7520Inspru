// SkinSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "E9101Main.h"
//#include "SkinSetDlg.h"
//#include "../DriveLib/ak4642api.h"
#include "E9101MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SysConfig.h"
/////////////////////////////////////////////////////////////////////////////
// CSkinSetDlg dialog
BOOL CSkinSetDlg::g_bFmOpen = FALSE;


void ReMoveZero(CString  &str)					
{	    
	int   nIndex;   
	nIndex=str.Find('.');   
	if   (nIndex>=0)   
	{   
		str.TrimRight('0');   
        if   (str.GetLength()==nIndex+1)   
        {   
			str=str.Left(nIndex);   
            if   (str.IsEmpty())   
                  str='0';   
        }   
	} 
}

CSkinSetDlg::CSkinSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSkinSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinSetDlg)
	//}}AFX_DATA_INIT

}


void CSkinSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinSetDlg)
	DDX_Control(pDX, IDC_BTN_SUB, m_FreqSub);
	DDX_Control(pDX, IDC_BTN_ADD, m_FreqAdd);
	DDX_Control(pDX, IDC_STATIC_FREQ, m_Static);
	DDX_Control(pDX, IDC_BTN_SWITCH, m_BtnSwith);
	DDX_Control(pDX, IDC_BTN_SKINSET_OK, m_BtnOk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinSetDlg, CDialogBase)
	//{{AFX_MSG_MAP(CSkinSetDlg)
	ON_BN_CLICKED(IDC_BTN_SWITCH, OnBtnSwitch)
	ON_BN_CLICKED(IDC_BTN_SKINSET_OK, OnBtnSkinsetOk)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SUB, OnBtnSub)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinSetDlg message handlers

BOOL CSkinSetDlg::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_STA_SKIN_SET_TITILE);
	m_csTitile = str;
	
	if( CSysConfig::Instance().GetIsSpareMode() )
	{
		str.LoadString(RES_BTN_SKIN_SET_FM_OPEN);
	}
	else
	{
		str.LoadString(RES_BTN_SKIN_SET_FM_CLOSE);
	}
	m_Item[4].chChar = str;
	return TRUE;
}

	//±‰∆§∑Ù
BOOL CSkinSetDlg::OnSkinChange()
{
	return TRUE;
}

BOOL CSkinSetDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();


	UINT uFreq = CSysConfig::Instance().GetFmFreq();		//Dealing with wrong data from .ini file
	if((uFreq<82750)||(uFreq>108000)) 
	{
		CSysConfig::Instance().SetFmFreq(88500);			//Default Frequency: 88.5
		uFreq = 88500;
	}

	double dFreq = (double)uFreq/1000;
	CString strTemp;
	strTemp.Format(_T("%f"), dFreq);
	ReMoveZero(strTemp);
	strTemp += "MHZ";
	m_Item[1].chChar = strTemp;

	InitGdi();
	InitControl();

	if(uFreq == 83000)
		m_Item[0].nState = BTN_STATE_DISABLE;
	else if(uFreq == 107500)
		m_Item[2].nState = BTN_STATE_DISABLE;

	OnLanguageChange();
	OnSkinChange();
	return TRUE;
}

void CSkinSetDlg::OnBtnSwitch() 
{
	CE9101MainDlg* pMainDlg = (CE9101MainDlg*)(AfxGetApp()->m_pMainWnd);
	if(g_bFmOpen == FALSE)				
	{
		::PostMessage( pMainDlg->m_hWnd,WM_FM_CHANGE , 1 , 0 );
		//set_FM_power( 1 );
		if(g_bFmInit)
		{
			g_bFmInit = FALSE;
			//FM_init();
		}
		//set_FM_freq( CSysConfig::Instance().GetFmFreq() );
		

		g_bFmOpen = TRUE;
		m_BtnSwith.SetBtnResID(bmp_file_FM_state_open);

		CResString str;
		str.LoadString(RES_BTN_SKIN_SET_FM_OPEN);
		m_BtnSwith.SetWindowText(str);

		CSysConfig::Instance().SetIsSpareMode(1);
	}
	else
	{
		::PostMessage( pMainDlg->m_hWnd,WM_FM_CHANGE , 0 , 0 );
		//set_FM_power( 0 );
		m_BtnSwith.SetBtnResID(bmp_file_FM_state_close);
		g_bFmOpen = FALSE;
		CResString str;
		str.LoadString(RES_BTN_SKIN_SET_FM_CLOSE);
		m_BtnSwith.SetWindowText(str);
		CSysConfig::Instance().SetIsSpareMode(0);
	}	
	m_BtnSwith.Invalidate();
}



void CSkinSetDlg::OnBtnAdd() 
{
	m_Item[0].nState = BTN_STATE_NORMAL;
	m_Item[2].nState = BTN_STATE_NORMAL;

	CString strTemp;
	strTemp = m_Item[1].chChar;

	int   nIndex;   
	nIndex=strTemp.Find('M');   
	strTemp=strTemp.Left(nIndex); 

	double numTemp;
	numTemp =wcstod((LPCTSTR)strTemp,_T('\0'));
	numTemp += FM_ADJUST_STEP;
	
	if((numTemp+FM_ADJUST_STEP)>=108) 
	{
		if(((numTemp+FM_ADJUST_STEP)>=108)&&(numTemp<108)) 	m_Item[2].nState = BTN_STATE_DISABLE;
		else {m_Item[2].nState = BTN_STATE_DISABLE; return;}
	}

	if((numTemp<=108)&&(numTemp>=82.75)) m_FreqSub.EnableWindow(TRUE);

	strTemp.Format(_T("%f"), numTemp);
	ReMoveZero(strTemp);
	strTemp += "MHZ";
	m_Item[1].chChar = strTemp;
	Invalidate();

	//set_FM_freq(DWORD(1000*numTemp));
	CSysConfig::Instance().SetFmFreq(DWORD(1000*numTemp));
}

void CSkinSetDlg::OnBtnSub() 
{	
	m_Item[0].nState = BTN_STATE_NORMAL;
	m_Item[2].nState = BTN_STATE_NORMAL;

	CString strTemp;
	strTemp = m_Item[1].chChar;

	int   nIndex;   
	nIndex=strTemp.Find('M');   
	strTemp=strTemp.Left(nIndex); 

	double numTemp;
	numTemp =wcstod((LPCTSTR)strTemp,_T('\0'));
	numTemp -= FM_ADJUST_STEP;

	if((numTemp-FM_ADJUST_STEP)<=82.75) 
	{
		if(((numTemp-FM_ADJUST_STEP)<=82.75)&&(numTemp>82.75))	m_Item[0].nState = BTN_STATE_DISABLE;
		else{m_Item[0].nState = BTN_STATE_DISABLE; return;} 
	}

	if((numTemp<=108)&&(numTemp>=82.75)) m_FreqAdd.EnableWindow(TRUE);

	strTemp.Format(_T("%f"), numTemp);
	ReMoveZero(strTemp);
	strTemp += "MHZ";
	m_Item[1].chChar = strTemp;
	Invalidate();

	//set_FM_freq(DWORD(1000*numTemp));
	CSysConfig::Instance().SetFmFreq(DWORD(1000*numTemp));
}



void CSkinSetDlg::OnBtnSkinsetOk() 
{
	CDialogBase::OnOK();
}

BOOL CSkinSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialogBase::PreTranslateMessage(pMsg);
}
 
LRESULT CSkinSetDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
// TODO: Add your specialized code here and/or call the base class
 	if( message == WM_CREATE_SET )
 	{
 		if( wParam == 0x06 )
 		{
 			OnExit();
 		}
	}		
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}


void CSkinSetDlg::InitGdi()
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
		_T("ÀŒÃÂ\0")); 

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
		_T("ÀŒÃÂ\0"));             // lpszFacename
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
		_T("ÀŒÃÂ\0")); 

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

void CSkinSetDlg::OnExit()
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

	CDialogBase::EndDialog(0);

}

void CSkinSetDlg::InitControl()
{
	for (int i =0;  i < 7; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	m_Item[1].nState = BTN_STATE_NORMAL;
	m_Item[4].nState = BTN_STATE_NORMAL;

	if( CSysConfig::Instance().GetIsSpareMode() )
	{
		m_Item[3].nState = BTN_STATE_NORMAL;
		m_Item[5].nState = BTN_STATE_DISABLE;
	}
	else
	{
		m_Item[3].nState = BTN_STATE_DISABLE;
		m_Item[5].nState = BTN_STATE_NORMAL;
	}


	m_Item[0].rect.top = 96;
	m_Item[0].rect.left = 69;
	m_Item[0].rect.bottom = 135;
	m_Item[0].rect.right = 116;

	m_Item[1].rect.top = 96;
	m_Item[1].rect.left = 128;
	m_Item[1].rect.bottom = 132;
	m_Item[1].rect.right = 330;

	m_Item[2].rect.top = 96;
	m_Item[2].rect.left = 365;
	m_Item[2].rect.bottom = 135;
	m_Item[2].rect.right = 414;

	m_Item[3].rect.top = 164;
	m_Item[3].rect.left = 69;
	m_Item[3].rect.bottom = 208;
	m_Item[3].rect.right = 116;

	m_Item[4].rect.top = 167;
	m_Item[4].rect.left = 128;
	m_Item[4].rect.bottom = 203;
	m_Item[4].rect.right = 330;

	m_Item[5].rect.top = 164;
	m_Item[5].rect.left = 365;
	m_Item[5].rect.bottom = 208;
	m_Item[5].rect.right = 414;

	m_Item[6].rect.top = 0;
	m_Item[6].rect.left = 430;
	m_Item[6].rect.bottom = 40;
	m_Item[6].rect.right = 480;
}

void CSkinSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CSkinSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CSkinSetDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;

	//ªÊ÷∆±≥æ∞
	for(int i=0;i<7;i++)
	{	
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}
	
	str = m_Item[1].chChar;
	Rect = m_Item[1].rect;
	state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  
		CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	
	str = m_Item[4].chChar;
	Rect = m_Item[4].rect;
	state = BTN_STATE_DISABLE;
	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  
		CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	else
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	
	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_LEFT|DT_VCENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);	

}

void CSkinSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
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

void CSkinSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<7;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;			
			switch( i )
			{
			case 0:
				OnBtnSub();
				break;
			case 2:
				OnBtnAdd();
				break;
			case 3:
				OnLeft();
				break;
			case 5:
				OnRight();
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



void CSkinSetDlg::OnLeft()
{
	CE9101MainDlg* pMainDlg = (CE9101MainDlg*)(AfxGetApp()->m_pMainWnd);
	m_Item[3].nState = BTN_STATE_DISABLE;
	m_Item[5].nState = BTN_STATE_NORMAL;

	::PostMessage( pMainDlg->m_hWnd,WM_FM_CHANGE , 0 , 0 );
	// set_FM_power( 0 );
	g_bFmOpen = FALSE;

	CResString str;
	str.LoadString(RES_BTN_SKIN_SET_FM_CLOSE);
	m_Item[4].chChar = str;
	CSysConfig::Instance().SetIsSpareMode(0);
	Invalidate();
}

void CSkinSetDlg::OnRight()
{
	CE9101MainDlg* pMainDlg = (CE9101MainDlg*)(AfxGetApp()->m_pMainWnd);

	m_Item[3].nState = BTN_STATE_NORMAL;
	m_Item[5].nState = BTN_STATE_DISABLE;

	::PostMessage( pMainDlg->m_hWnd,WM_FM_CHANGE , 1 , 0 );
	//set_FM_power( 1 );
	if(g_bFmInit)
	{
		g_bFmInit = FALSE;
		//FM_init();
	}
	//set_FM_freq( CSysConfig::Instance().GetFmFreq() );
	g_bFmOpen = TRUE;

	CResString str;
	str.LoadString(RES_BTN_SKIN_SET_FM_OPEN);
	m_Item[4].chChar = str;
	CSysConfig::Instance().SetIsSpareMode(1);
	Invalidate();
}

