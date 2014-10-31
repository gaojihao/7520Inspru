// DlgPbookBrw.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgPbookBrw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// CDlgPbookBrw dialog

CString CDlgPbookBrw::s_csPath = PATH_SD;
BOOL  CDlgPbookBrw::s_bOk = FALSE;


CDlgPbookBrw::CDlgPbookBrw(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgPbookBrw::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPbookBrw)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	 m_DirCnt = 0;
	 m_FileCnt = 0;
	 m_PageCnt = 1;
	 m_iCurPage = 1;
	 m_iListCnt = 0;

	 m_pItemList = NULL;
}


void CDlgPbookBrw::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPbookBrw)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPbookBrw, CDialogBase)
	//{{AFX_MSG_MAP(CDlgPbookBrw)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPbookBrw message handlers

BOOL CDlgPbookBrw::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	s_csPath = PATH_SD;
	s_bOk = FALSE;

	InitGdi();
	LoadDir( s_csPath );
	InitControl();

	OnLanguageChange();
	OnSkinChange();


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgPbookBrw::OnLanguageChange()
{
	 s_csPath= PATH_SD;

	CResString str;
	str.LoadString(RES_BUTTON_IN);

	CResString str1;
	str1.LoadString(RES_BLUETOOTH_PHONEBOOK);

	m_csTitile = str + str1;



	return TRUE;
}

	//±äÆ¤·ô
BOOL CDlgPbookBrw::OnSkinChange()
{

	return TRUE;
}

void CDlgPbookBrw::LoadDir(CString str)
{

	if( m_pItemList != NULL )
	{
		delete[] m_pItemList;
		m_pItemList = NULL;
	}

	int i;
	int less = 0;

	m_dirref.Load(str,0,_T(".TXT"));

	m_DirCnt = m_dirref.ReadDirCnt();
	m_FileCnt = m_dirref.ReadFileCnt();


	m_iListCnt = ( m_DirCnt + m_FileCnt )%8;

	if( m_iListCnt > 0 )
	{
		m_iListCnt = (( m_DirCnt + m_FileCnt )/8 + 1)*8;
	}
	else
	{
		m_iListCnt = m_DirCnt + m_FileCnt;
	}

	m_PageCnt = m_iListCnt/8;
	m_iCurPage = 1;
	m_Item[0].nState = BTN_STATE_DISABLE;
	if(m_PageCnt == 1)
		m_Item[1].nState = BTN_STATE_DISABLE;
	else
		m_Item[1].nState = BTN_STATE_NORMAL;

	m_pItemList = new  ItemListInfo[m_iListCnt];
	memset(m_pItemList,0,sizeof(m_pItemList));

	int x = 0;
	for(i=0;i<m_iListCnt;i++)
	{
		if( i<m_DirCnt )
		{
			if( i != 0)
				m_pItemList[i].chChar = m_dirref.ReadOneDirName(i);
			else
				m_pItemList[i].chChar = _T("..");
			m_pItemList[i].nState =BTN_STATE_NORMAL;
			m_pItemList[i].nListType = LIST_DIR;
	//		less = i%8;
	//		m_pItemList[i].rect = m_ItemList[less].rect;
		}
		else if( i<m_DirCnt+m_FileCnt )
		{
			m_pItemList[i].chChar = m_dirref.ReadOneFileName(x);
			x++;
			m_pItemList[i].nState =BTN_STATE_NORMAL;
			m_pItemList[i].nListType = LIST_FILE;
		}
		else if( i<m_iListCnt )
		{
			m_pItemList[i].chChar = _T("");
			m_pItemList[i].nState =BTN_STATE_DISABLE;
			m_pItemList[i].nListType = LIST_NONE;
		}
	}

	for(i =0;  i < 8; i++)
	{
		m_ItemList[i].nState = m_pItemList[i].nState;
		m_ItemList[i].chChar = m_pItemList[i].chChar;
		m_ItemList[i].nListType = m_pItemList[i].nListType;
	}


}

void CDlgPbookBrw::InitGdi()
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
		_T("ËÎÌå\0")); 


	}
	else
	{
  		smallfont.CreateFont(
		16,					    // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_THIN,                 // nWeight
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

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	bk_disable_dc.CreateCompatibleDC(m_pDC);

	bk_list_dc.CreateCompatibleDC(m_pDC);

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_NAVIBRO_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_NAVIBRO_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 

	pSkin = g_pResMng->RequestBmp(SKIN_BG_NAVIBRO_D, true);
	m_stBtnDisableBMP = pSkin->hBitmap; 
	
	pSkin = g_pResMng->RequestBmp(SKIN_BTN_BROWER, true);
	m_stBtnListBMP = pSkin->hBitmap;  

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_bk_disable_bmp    = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	m_bk_list_bmp    = bk_list_dc.SelectObject(CBitmap::FromHandle(m_stBtnListBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgPbookBrw::OnExit()
{
	bk_list_dc.SelectObject(m_bk_list_bmp);
	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnListBMP);	
	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_list_dc.DeleteDC();
	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(0);

}

void CDlgPbookBrw::InitControl()
{
	m_Item[0].rect.top = 59;
	m_Item[0].rect.left = 434;
	m_Item[0].rect.bottom = 132;
	m_Item[0].rect.right = 480;

	m_Item[1].rect.top = 146;
	m_Item[1].rect.left = 438;
	m_Item[1].rect.bottom = 224;
	m_Item[1].rect.right = 480;

	m_Item[2].rect.top = 44;
	m_Item[2].rect.left = 386;
	m_Item[2].rect.bottom = 81;
	m_Item[2].rect.right = 433;

	m_Item[3].rect.top = 0;
	m_Item[3].rect.left = 420;
	m_Item[3].rect.bottom = 40;
	m_Item[3].rect.right = 480;


	m_ItemList[0].rect.top = 100;
	m_ItemList[0].rect.left = 46;
	m_ItemList[0].rect.bottom = 145;
	m_ItemList[0].rect.right = 91;

	m_ItemList[1].rect.top = 100;
	m_ItemList[1].rect.left = 146;
	m_ItemList[1].rect.bottom = 145;
	m_ItemList[1].rect.right = 191;


	m_ItemList[2].rect.top = 100;
	m_ItemList[2].rect.left = 246;
	m_ItemList[2].rect.bottom = 145;
	m_ItemList[2].rect.right = 291;

	m_ItemList[3].rect.top = 100;
	m_ItemList[3].rect.left = 346;
	m_ItemList[3].rect.bottom = 145;
	m_ItemList[3].rect.right = 391;


	m_ItemList[4].rect.top = 185;
	m_ItemList[4].rect.left = 46;
	m_ItemList[4].rect.bottom = 230;
	m_ItemList[4].rect.right = 91;

	m_ItemList[5].rect.top = 185;
	m_ItemList[5].rect.left = 146;
	m_ItemList[5].rect.bottom = 230;
	m_ItemList[5].rect.right = 191;

	m_ItemList[6].rect.top = 185;
	m_ItemList[6].rect.left = 246;
	m_ItemList[6].rect.bottom = 230;
	m_ItemList[6].rect.right = 291;

	m_ItemList[7].rect.top = 185;
	m_ItemList[7].rect.left = 346;
	m_ItemList[7].rect.bottom = 230;
	m_ItemList[7].rect.right = 391;


	for (int i =2;  i < 4; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
}

void CDlgPbookBrw::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgPbookBrw::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgPbookBrw::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
	int i;

	for( i=0;i<4;i++ )
	{
		//»æÖÆ±³¾°
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
//		Rect.top = Rect.top + 54;
//		DrawBtnText(&memDC,Rect,str,RGB(255,125,0));
	}

	for( i=0;i<8;i++ )
	{
		//»æÖÆ±³¾°
		str = m_ItemList[i].chChar;
		Rect = m_ItemList[i].rect;

		if( m_ItemList[i].nState == BTN_STATE_NORMAL )
		{
			if(m_ItemList[i].nListType == LIST_DIR)
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,0,0,SRCCOPY);
			else if( m_ItemList[i].nListType == LIST_FILE )
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,45,0,SRCCOPY);

			Rect.left -= 20;
			Rect.right += 20;
			Rect.bottom += 20;
			DrawBtnText(&memDC,Rect,m_ItemList[i].chChar,smallfont,RGB(255,255,255),DT_CENTER | DT_BOTTOM);
		}
		else if( m_ItemList[i].nState == BTN_STATE_DOWN )
		{
			if(m_ItemList[i].nListType == LIST_DIR)
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,0,45,SRCCOPY);
			else if( m_ItemList[i].nListType == LIST_FILE )
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,45,45,SRCCOPY);

			Rect.left -= 20;
			Rect.right += 20;
			Rect.bottom += 20;
			DrawBtnText(&memDC,Rect,m_ItemList[i].chChar,smallfont,RGB(255,255,255),DT_CENTER | DT_BOTTOM);
		}
		else if( m_ItemList[i].nState == BTN_STATE_DISABLE && m_ItemList[i].nListType == LIST_NONE )
		{
			memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,90,0,SRCCOPY);
		}
		else if( m_ItemList[i].nState == BTN_STATE_DISABLE && m_ItemList[i].nListType == LIST_FILE )
		{
			memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,45,90,SRCCOPY);
			Rect.left -= 20;
			Rect.right += 20;
			Rect.bottom += 20;
			DrawBtnText(&memDC,Rect,m_ItemList[i].chChar,smallfont,RGB(255,255,255),DT_CENTER | DT_BOTTOM);
		}
	}


	Rect.top = 35;
	Rect.left = 13;
	Rect.bottom = 65;
	Rect.right = 382;
	DrawBtnText(&memDC,Rect,s_csPath,smallfont,RGB(255,255,255),DT_LEFT | DT_BOTTOM);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CDlgPbookBrw::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<4;i++)
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

	for(int j=0;j<8;j++)
	{
		if(PtInRect(&m_ItemList[j].rect,point))
		{
				if(m_ItemList[j].nState == BTN_STATE_NORMAL)
				{
					m_ItemList[j].nState = BTN_STATE_DOWN;
					PlayKeySound();
				}
		}
	}
	
	Invalidate();
	SetCapture();


	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgPbookBrw::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<4;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			
				
				switch( i )
					{
					case 0:
						OnUp();
						break;
					case 1:
						OnDown();
						break;
					case 2:
						OnOK();
						break;
					case 3:
						OnExit();
						break;
					default:
						break;
					}
					
			}
		
	}

	for(int j=0;j<8;j++)
	{
			if(m_ItemList[j].nState==BTN_STATE_DOWN)
			{
				m_ItemList[j].nState = BTN_STATE_NORMAL;
			
				if( j == 0 && m_ItemList[j].nListType == LIST_DIR && m_ItemList[j].chChar == _T(".."))
					OnUpDir();
				else
					OnBtnList(j);
							
			}
			else if( m_ItemList[j].nListType == LIST_FILE && m_ItemList[j].nState == BTN_STATE_DISABLE )
			{
				m_ItemList[j].nState = BTN_STATE_NORMAL;
			}
		
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CDlgPbookBrw::OnUpDir()
{


	TCHAR ch = '\\';
	int pos = s_csPath.ReverseFind(ch);
	s_csPath.Delete(pos,s_csPath.GetLength()-pos);

	if( !s_csPath.Compare(_T("")) )
	{
		s_csPath = PATH_SD;
		return;
	}
	else
	{
		LoadDir(s_csPath);
	}
	
}

void CDlgPbookBrw::OnBtnList( int j )
{
	if( m_ItemList[j].nListType == LIST_DIR )
	{
		s_csPath += _T("\\") + m_ItemList[j].chChar;
		LoadDir(s_csPath);
	}
	else if( m_ItemList[j].nListType == LIST_FILE )
	{
		m_ItemList[j].nState = BTN_STATE_DISABLE;
	}
}

void CDlgPbookBrw::OnUp()
{

	m_iCurPage--;

	m_Item[0].nState = BTN_STATE_NORMAL;
	m_Item[1].nState = BTN_STATE_NORMAL;
	if( m_iCurPage == 1 )
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		//m_Item[1].nState = BTN_STATE_NORMAL;
	}


	int j = 0;
	for( int i = (m_iCurPage-1)*8;  i < (m_iCurPage*8); i++ )
	{
		m_ItemList[j].nState = m_pItemList[i].nState;
		m_ItemList[j].chChar = m_pItemList[i].chChar;
		m_ItemList[j].nListType = m_pItemList[i].nListType;
		j++;
	}
	Invalidate();
}

void CDlgPbookBrw::OnDown()
{

	m_iCurPage++;

	m_Item[0].nState = BTN_STATE_NORMAL;
	m_Item[1].nState = BTN_STATE_NORMAL;
	if( m_iCurPage == m_PageCnt )
	{
		m_Item[1].nState = BTN_STATE_DISABLE;
		//m_Item[0].nState = BTN_STATE_NORMAL;
	}


	int j = 0;
	for( int i = (m_iCurPage-1)*8;  i < (m_iCurPage*8); i++ )
	{
		m_ItemList[j].nState = m_pItemList[i].nState;
		m_ItemList[j].chChar = m_pItemList[i].chChar;
		m_ItemList[j].nListType = m_pItemList[i].nListType;
		j++;
	}		
	Invalidate();
}

void CDlgPbookBrw::OnOK()
{
	for( int i = 0;  i < 8; i++ )
	{
		if( m_ItemList[i].nListType == LIST_FILE && m_ItemList[i].nState == BTN_STATE_DISABLE )
		{
			s_csPath += _T("\\") + m_ItemList[i].chChar;
			s_bOk = TRUE;	
			EndDialog(0);
		}
	}
}



LRESULT CDlgPbookBrw::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnExit();
		}
	}
	else if( wParam == STORAGE_EVENT_SD_PLUGIN )
 	{
		Sleep(600);
		s_csPath =PATH_SD;
		LoadDir(s_csPath);
		Invalidate();
	}
	else if ( wParam == STORAGE_EVENT_SD_PULLOUT )
	{
		s_csPath = PATH_SD;
		LoadDir(s_csPath);
		Invalidate();
	}

	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgPbookBrw::OnTimer(UINT nIDEvent) 
{
	CDialogBase::OnTimer( nIDEvent );
}

BOOL CDlgPbookBrw::PreTranslateMessage(MSG* pMsg) 
{

	return CDialogBase::PreTranslateMessage(pMsg);
}
