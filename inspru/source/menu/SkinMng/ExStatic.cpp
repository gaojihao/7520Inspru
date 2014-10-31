// ExStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ExStatic.h"
#include "ResMng.h"
#include "SysConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExStatic
IResMng*  CExStatic::s_pResMng = CResMng::Instance();

enum  slider_e{
	sld_offset_nor = 0,
	sld_offset_dwn,
	sld_offset_bgn,
	sld_offset_fill,
	sld_offset_unfill,
	sld_offset_end
};


CExStatic::CExStatic()
{
	m_bFmt12 = FALSE;

	m_DrawItem.stSkinID    = SKIN_ID_NONE;
	m_DrawItem.stPntSkinID = SKIN_ID_NONE;
	m_DrawItem.nNumber     = 0;
	m_DrawItem.nFontID     = sf_18;
	m_DrawItem.nFontCol    = sc_white;
	m_DrawItem.stcStyle    = STC_TEXT_SINGLE;
	m_DrawItem.nBkCol      = sc_transparent;
	m_bMouseDown           = FALSE;
	m_nCurPos              = 0;
}

CExStatic::~CExStatic()
{
}


BEGIN_MESSAGE_MAP(CExStatic, CStatic)
	//{{AFX_MSG_MAP(CExStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void  CExStatic::SkinSetStcID(BYTE byStyle, DWORD dwSkinID, DWORD dwPntResID /*= SKIN_ID_NONE*/)
{
	m_DrawItem.stcStyle = byStyle;
	m_DrawItem.stSkinID = dwSkinID;
	m_DrawItem.stPntSkinID = dwPntResID;

	switch(m_DrawItem.stcStyle) 
	{
	case STC_DIGITAL_TIME:
		{
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	//		ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0,0, pSkin->nWidthPerCell * 5,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
		break;
	case STC_DIGITAL_NUM : 
		{
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
			ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0, 0, pSkin->nWidthPerCell * 2,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
		break;
	case STC_BMP_BATTERY:
		{
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
			ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0, 0, pSkin->nWidthPerCell,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
		break;
	case STC_BMP:
		{
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,false );
			ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0, 0, pSkin->nWidthPerCell,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
		break;
	case  STC_DIGITAL_FM:
		{
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,false );
			ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0, 0, pSkin->nWidthPerCell * 8,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
		break;
	case  STC_DIGITAL_CH:
		{
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,false );
			ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0, 0, pSkin->nWidthPerCell * 5,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
	case STC_SLIDER:
		{
			CRect rt;
			GetClientRect(&rt);
			PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
			ASSERT(pSkin != NULL );
			SetWindowPos(NULL,0, 0, rt.Width(),pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
		break;
	default:
		break;
	}
}

void  CExStatic::SkinSetDigitalTime(int nSec, BOOL bFmt12 /*= FALSE*/)
{	
	m_bFmt12 = bFmt12;
	m_DrawItem.nNumber = nSec;
	Invalidate();

}

void  CExStatic::SkinSetTxtAttri(BYTE byFontID, BYTE byColID, BYTE byBkCol /*sc_transparent*/)
{
	m_DrawItem.nFontID = byFontID;
	m_DrawItem.nFontCol = byColID;
	m_DrawItem.nBkCol = byBkCol;
	Invalidate();

}	
void  CExStatic::SkinSetCH(DWORD dwCh)
{
	m_DrawItem.nNumber = dwCh;
	Invalidate();
}

void  CExStatic::SkinSetNum(DWORD dwNum)
{
	m_DrawItem.nNumber = dwNum;
	Invalidate();
}

void  CExStatic::SkinSetBatterNum(DWORD dwBattery)
{
	m_DrawItem.nNumber = dwBattery;
	Invalidate();
}

void  CExStatic::SkinSetFm(DWORD dwFm)
{
	m_DrawItem.nNumber = dwFm;
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void  CExStatic::SKinSetMapOffset(DWORD dwOffset)
{
	m_DrawItem.nNumber = dwOffset;
}


void  CExStatic::SkinSetPos(int nPos)
{
	if ( !m_bMouseDown )
	{
		m_DrawItem.nNumber = nPos;
		Invalidate();
	}
}

int CExStatic::SkinGetPos()
{
	return (m_DrawItem.nNumber % 101);
}
/////////////////////////////////////////////////////////////////////////////
// CExStatic message handlers

void CExStatic::OnPaint() 
{	
	CPaintDC  dc(this);

	CRect  rt;
	GetClientRect(&rt);
	HDC     hMemDC = CreateCompatibleDC( dc.GetSafeHdc() );
	HBITMAP hMemBmp = CreateCompatibleBitmap(dc.GetSafeHdc(), rt.Width(), rt.Height() );
	HBITMAP hPreBmp = (HBITMAP)SelectObject( hMemDC, hMemBmp );

//#define  STC_TEXT_MULTIPLE     (0x02)    //多行文本

	
	switch( m_DrawItem.stcStyle )
	{
	case STC_TEXT_SINGLE:       SkinDrawSingleLine(hMemDC, &rt,DT_CENTER);  break;
	case STC_TEXT_SINGLE_LEFT:  SkinDrawSingleLine(hMemDC, &rt,DT_LEFT);    break;
	case STC_TEXT_SINGLE_RIGHT: SkinDrawSingleLine(hMemDC, &rt,DT_RIGHT);   break;
	case STC_TEXT_SINGLE_ELIDE: SkinDrawSingleLineElide(hMemDC, &rt);       break;
	case STC_BMP        :       SkinDrawOnlyBmp(hMemDC, &rt);	  break;
	case STC_BMP_BATTERY:       SkinDrawBattery(hMemDC);          break;
	case STC_DIGITAL_TIME:      SkinDrawDigitalTime(hMemDC,&rt);  break;
	case STC_DIGITAL_NUM:       SkinDrawDigitalNum(hMemDC, &rt);  break;
	case STC_DIGITAL_CH:        SkinDrawDigitalCH(hMemDC, &rt);   break;
	case STC_DIGITAL_FM:        SkinDrawDigitalFm(hMemDC, &rt);   break;
	case STC_TEXT_MULTIPLE:     SkinDrawMultipleText(hMemDC, &rt);break;
	case STC_MAP_ZONE:          SkinDrawZoneMap(hMemDC, &rt);     break; 
	case STC_SLIDER:            SkinDrawSlider(hMemDC, &rt);      break;
	default:
		CStatic::OnPaint();
		return;
	}


	::BitBlt(dc.GetSafeHdc(), 0,0, rt.Width(), rt.Height(),
		hMemDC, 0, 0,  SRCCOPY );
	SelectObject(hMemDC, hPreBmp);
	DeleteObject( hMemBmp );
	DeleteDC( hMemDC );

}

// Nonzero if it erase background; otherwise,it is zero
BOOL CExStatic::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	return CStatic::OnEraseBkgnd(pDC);
}

void  CExStatic::SkinDrawMultipleText(HDC hDC, LPRECT lpRect)
{
	CRect  rt;

	if ( m_DrawItem.nBkCol ==sc_transparent)
	{
		SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);
	}

	// Draw multiple line text
	COLORREF  TextCol  = s_pResMng->RequestCol(m_DrawItem.nFontCol);
	HFONT     hFont    = s_pResMng->RequestFont(m_DrawItem.nFontID );
	COLORREF  PrevCol  = SetTextColor(hDC, TextCol);
	HFONT     PrevFont = (HFONT)SelectObject(hDC, hFont );

	GetClientRect(&rt);

	// Fill the back with solid color
	if ( m_DrawItem.nBkCol != sc_transparent )
	{
		COLORREF bkCol = s_pResMng->RequestCol(m_DrawItem.nBkCol);
		::SetBkColor(hDC, bkCol);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rt, NULL, 0, NULL);
	} 

	SetBkMode(hDC, TRANSPARENT);
	
	TCHAR szTxt[MAX_PATH]={0};
	GetWindowText(szTxt,MAX_PATH);
	::DrawText(hDC,szTxt,_tcslen(szTxt), lpRect ,DT_TOP | DT_LEFT | DT_WORDBREAK);
	SelectObject(hDC, PrevFont);
	SetTextColor(hDC, PrevCol );	
}

///
void  CExStatic::SkinDrawSingleLine(HDC hDC,LPRECT lpRect, DWORD dwStyle /* = DT_CENTER*/)
{
	CRect  rt;

	if ( m_DrawItem.nBkCol ==sc_transparent )
	{
		SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);
	}

	// Draw single line text
	COLORREF  TextCol  = s_pResMng->RequestCol(m_DrawItem.nFontCol);
	HFONT     hFont    = s_pResMng->RequestFont(m_DrawItem.nFontID );
	COLORREF  PrevCol  = SetTextColor(hDC, TextCol);
	HFONT     PrevFont = (HFONT)SelectObject(hDC, hFont );

	GetClientRect(&rt);
	if ( m_DrawItem.nBkCol != sc_transparent )
	{
		COLORREF bkCol = s_pResMng->RequestCol(m_DrawItem.nBkCol);
		CBrush br(bkCol);
		FillRect(hDC, &rt, br);
	} 

	SetBkMode(hDC, TRANSPARENT);

	
	TCHAR szTxt[MAX_PATH]={0};
	GetWindowText(szTxt,MAX_PATH);

	::DrawText(hDC,szTxt,_tcslen(szTxt), lpRect ,dwStyle | DT_VCENTER );

	SelectObject(hDC, PrevFont);
	SetTextColor(hDC, PrevCol );
		
}


void  CExStatic::SkinDrawSingleLineElide(HDC hDC, LPRECT lpRect)
{	
	if ( m_DrawItem.nBkCol ==sc_transparent )
	{
		SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);
	} else
	{
		COLORREF bkCol = s_pResMng->RequestCol(m_DrawItem.nBkCol);
		CBrush br(bkCol);
		FillRect(hDC, lpRect, br);
	}

	// Draw single line text
	COLORREF  TextCol  = s_pResMng->RequestCol(m_DrawItem.nFontCol);
	HFONT     hFont    = s_pResMng->RequestFont(m_DrawItem.nFontID );
	COLORREF  PrevCol  = SetTextColor(hDC, TextCol);
	HFONT     PrevFont = (HFONT)SelectObject(hDC, hFont );

	SetBkMode(hDC, TRANSPARENT);

	CString strText;
	GetWindowText(strText);
	CRect rtText(0,0,0,0);	

	CDC *pDC = CDC::FromHandle(hDC);
	pDC->DrawText(strText,&rtText,DT_CALCRECT);
	int nDCWidth = lpRect->right - lpRect->left;
	if ( rtText.Width() > nDCWidth && rtText.Width() > 0 )
	{
		int nCount =  (nDCWidth * strText.GetLength() / rtText.Width() );
		strText.Format(_T("%s..."), strText.Left(nCount-3));
	}

	pDC->DrawText(strText,lpRect,DT_LEFT);

	SelectObject(hDC, PrevFont);
	SetTextColor(hDC, PrevCol );

}

//////////////////////////////////////////////////////////////////////////

void  CExStatic::SkinDrawDigitalNum(HDC hDC, LPRECT lpRect)
{
	SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);
	
	PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT(pSkin != NULL );

	m_DrawItem.nNumber %= 100;

	TransparentImage(hDC,
		             0, 0, pSkin->nWidthPerCell,pSkin->nHeightPerCell,
					 pSkin->hBitmap,
					 pSkin->nWidthPerCell * (m_DrawItem.nNumber/10), 0,
					 pSkin->nWidthPerCell,pSkin->nHeightPerCell,SKIN_TRANS_COLOR );
	
	TransparentImage(hDC,
		             pSkin->nWidthPerCell, 0, 
					 pSkin->nWidthPerCell,pSkin->nHeightPerCell,
					 pSkin->hBitmap,
					 pSkin->nWidthPerCell * (m_DrawItem.nNumber%10), 0,
					 pSkin->nWidthPerCell,pSkin->nHeightPerCell,SKIN_TRANS_COLOR );

	
}


//////////////////////////////////////////////////////////////////////////

void  CExStatic::SkinDrawBattery(HDC hDC)
{
	PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT(pSkin != NULL );

	HDC      hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, pSkin->hBitmap);

	BitBlt(hDC,0,0, pSkin->nWidthPerCell,pSkin->nHeightPerCell,
		             hMemDC,
		             pSkin->nWidthPerCell * (m_DrawItem.nNumber%7), 0,
					 SRCCOPY );
	SelectObject(hMemDC, hPreBmp);
	s_pResMng->ReleaseBmp(m_DrawItem.stSkinID);
	DeleteDC(hMemDC);

}

void CExStatic::SkinDrawDigitalTime(HDC hDC, LPRECT lpRect)
{
	PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT(pSkin != NULL );

	int nW = pSkin->nWidthPerCell;
	int nH = pSkin->nHeightPerCell;
	int n = m_bFmt12 ? 7: 5;
	SetWindowPos(NULL,0,0, pSkin->nWidthPerCell * n,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
	
	SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);

	int nCurrentTime = m_DrawItem.nNumber;
	
	int nUnitCount = 0;
	if ( m_bFmt12 )
	{
		int nCharIndex = 11; // the index of 'AM'
		if ( m_DrawItem.nNumber > 3600 * 12 )
		{
			if ( m_DrawItem.nNumber >= 3600 * 13 )
				nCurrentTime = m_DrawItem.nNumber - (12*3600);
			nCharIndex = 13;
		}
		// Draw 'AM' or 'PM'
		TransparentImage(hDC,
		             nW * nUnitCount, 0, nW * 2, nH,
					 pSkin->hBitmap,
					 nW * nCharIndex, 0,
					 nW * 2, nH,SKIN_TRANS_COLOR );
//		BitBlt(hDC,nW * nUnitCount, 0, nW * 2, nH,
//			hMemDC,	nW * nCharIndex, 0,	SRCCOPY );

		nUnitCount = 2;
	}

	int nHour = nCurrentTime /3600;
	int nMin  = nCurrentTime % 3600 ;
	nMin = nMin / 60;

//	TRACE2("%d--%d \n",nHour, nMin );
	
	// Draw hours
	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nHour/10), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	
	//BitBlt(hDC,nW * nUnitCount, 0, nW, nH,
	//		hMemDC,	nW * (nHour/10), 0,	SRCCOPY );

	nUnitCount++;

	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nHour%10), 0,
					 nW, nH,SKIN_TRANS_COLOR );

//	BitBlt(hDC,nW * nUnitCount, 0, nW, nH,
//			hMemDC,	nW * (nHour%10), 0,	SRCCOPY );
	nUnitCount++;

	// Draw dot
	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * 10, 0,
					 nW, nH, SKIN_TRANS_COLOR );
//	BitBlt(hDC,nW * nUnitCount, 0, nW, nH,
//			hMemDC,	nW * 10, 0, SRCCOPY );
	nUnitCount++;

	// Draw minute
	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nMin/10), 0,
					 nW, nH,SKIN_TRANS_COLOR );
//	BitBlt(hDC,nW * nUnitCount, 0, nW, nH,
//			hMemDC,	nW * (nMin/10), 0, SRCCOPY );
	nUnitCount++;


	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nMin%10), 0,
					 nW, nH,SKIN_TRANS_COLOR );
//	BitBlt(hDC,nW * nUnitCount, 0, nW, nH,
//			hMemDC,	nW * (nMin%10), 0, SRCCOPY );

//	nUnitCount++;
//	SelectObject(hMemDC, hPrevBmp);
//	DeleteDC(hMemDC);

}

//////////////////////////////////////////////////////////////////////////

void  CExStatic::SkinDrawDigitalFm(HDC hDC, LPRECT lpRect)
{

	SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);

	int nInt = m_DrawItem.nNumber / 100;
	int nDot = m_DrawItem.nNumber % 100;

	PSKINBMP_ST pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT( pSkin != NULL );

	int nUnitCount = 0;
	int nW = pSkin->nWidthPerCell;
	int nH = pSkin->nHeightPerCell;


	// Draw inter
	if ( nInt >= 100 )
	{
		TransparentImage(hDC,
			nW * nUnitCount, 0, nW, nH,
			pSkin->hBitmap,
			nW * (nInt/100), 0,
			nW, nH,SKIN_TRANS_COLOR );
		nUnitCount++;
	}

	nInt %= 100;

	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nInt/10), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	nInt %= 10;
	nUnitCount++;

	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nInt), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	nUnitCount++;
	
	// Draw dot
	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * 10, 0,
					 nW, nH, SKIN_TRANS_COLOR );
	nUnitCount++;


	// Draw inter
	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nDot/10), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	nDot %= 10;
	nUnitCount++;

	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nDot), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	nUnitCount++;

	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW * 2, nH,
					 pSkin->hBitmap,
					 nW * 11, 0, nW * 2, nH,
					 SKIN_TRANS_COLOR );

}

void  CExStatic::SkinDrawDigitalCH(HDC hDC, LPRECT lpRect)
{
	SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);

	PSKINBMP_ST pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT( pSkin != NULL );

	int nInt = m_DrawItem.nNumber % 100;
	int nUnitCount = 0;
	int nW = pSkin->nWidthPerCell;
	int nH = pSkin->nHeightPerCell;

	// Draw chanel
	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nInt/10), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	nInt %= 10;
	nUnitCount++;

	TransparentImage(hDC,
		             nW * nUnitCount, 0, nW, nH,
					 pSkin->hBitmap,
					 nW * (nInt), 0,
					 nW, nH,SKIN_TRANS_COLOR );
	nUnitCount++;

	// 需要在 04 CH之间加一个空格

	TransparentImage(hDC,
		             nW * nUnitCount+nW/2, 0, nW * 2, nH,
					 pSkin->hBitmap,
					 nW * 10, 0, nW * 2, nH,
					 SKIN_TRANS_COLOR );
}


void  CExStatic::SkinDrawOnlyBmp(HDC hDC, LPRECT lpRect)
{
	CRect rt;
	PSKINBMP_ST  pSkin = NULL;

	if ( m_DrawItem.stPntSkinID != SKIN_ID_NONE )
	{
		SkinDrawBkFromParent(hDC, m_DrawItem.stPntSkinID);
	}
	
	int nOffset = s_pResMng->CalBmpOffset( m_DrawItem.stSkinID );
	pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,false );
	ASSERT( pSkin != NULL );
	
	TransparentImage(hDC,0,0,pSkin->nWidthPerCell,pSkin->nHeightPerCell,
		pSkin->hBitmap,nOffset*pSkin->nWidthPerCell ,
		0,
		pSkin->nWidthPerCell,
		pSkin->nHeightPerCell, 
		SKIN_TRANS_COLOR );


}

//////////////////////////////////////////////////////////////////////////

void  CExStatic::SkinDrawZoneMap(HDC hDC, LPRECT lpRect)
{
	PSKINBMP_ST  pSkin = NULL;

	SkinDrawBkFromParent(hDC,m_DrawItem.stPntSkinID);
		
	// Draw background for static control
	//TransparentImage(hDC,
	//		0,0, rt.Width(), rt.Height(),
	//		pSkin->hBitmap,
	//		rt.left,rt.top, rt.Width(), rt.Height(),
	//		SKIN_TRANS_COLOR 
	//		);

	CRect rt;
	GetClientRect(&rt);

	pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT( pSkin != NULL );

	m_DrawItem.nNumber = m_DrawItem.nNumber % (pSkin->nWidthPerCell + 1 );

	TRACE1("User input offset of the map : %d \n",m_DrawItem.nNumber);

	//COLORREF  colTrans = RGB(46,151,254);
	COLORREF colTrans = s_pResMng->RequestCol(sc_ZoneTransitionCol);
	int nOffset = 0;
	int nDlt = pSkin->nWidthPerCell - (lpRect->right - lpRect->left);
	if ( m_DrawItem.nNumber >= 0 && m_DrawItem.nNumber <= nDlt / 2)
	{

		nOffset = m_DrawItem.nNumber + nDlt/2;
		TRACE1("First Offset : %d \n",nOffset);
		TransparentImage(hDC,
			0,0, rt.Width(), rt.Height(),
			pSkin->hBitmap,
			nOffset,0, rt.Width(), rt.Height(),
			colTrans );

	} else  if ( m_DrawItem.nNumber > nDlt/ 2 && 
		         m_DrawItem.nNumber <= nDlt/2 + (lpRect->right - lpRect->left)
				 )
	{
		nOffset = m_DrawItem.nNumber + nDlt/2;
		int nLeft = pSkin->nWidthPerCell - nOffset;
		TRACE2("Second Offset : %d-%d \n",nOffset,nLeft);

		TransparentImage(hDC,
			0,0, nLeft, rt.Height(),
			pSkin->hBitmap,
			nOffset,0, nLeft, rt.Height(),
			colTrans );
	
		TransparentImage(hDC,
			nLeft-1,0, (lpRect->right - lpRect->left) - nLeft, rt.Height(),
			pSkin->hBitmap,
			0,0, (lpRect->right - lpRect->left) - nLeft, rt.Height(),
			colTrans );

	} else  if (m_DrawItem.nNumber <= pSkin->nWidthPerCell )
	{
		nOffset = m_DrawItem.nNumber - nDlt/2 - (lpRect->right - lpRect->left);
		TRACE1("Third Offset : %d \n",nOffset);
		TransparentImage(hDC,
			0,0, rt.Width(), rt.Height(),
			pSkin->hBitmap,
			nOffset,0, rt.Width(), rt.Height(),
			colTrans );
	}
}

void  CExStatic::SkinDrawSlider(HDC hDC, LPRECT lpRect)
{	
	PSKINBMP_ST  pSkin = NULL;

	pSkin = s_pResMng->RequestBmp(m_DrawItem.stSkinID,true );
	ASSERT( pSkin != NULL );

	CRect rt;
	GetClientRect( &rt );

	// Draw Head
	TransparentImage(hDC,
			0,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			pSkin->hBitmap,
			sld_offset_bgn * pSkin->nWidthPerCell ,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			SKIN_TRANS_COLOR);	

	// Draw Fill
	int nFillWidth = (m_DrawItem.nNumber) * (rt.Width() - pSkin->nWidthPerCell) / 100;
	int n = 0;
	while ( nFillWidth >= n )
	{
		TransparentImage(hDC,
			n+pSkin->nWidthPerCell/2,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			pSkin->hBitmap,
			sld_offset_fill * pSkin->nWidthPerCell ,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			SKIN_TRANS_COLOR);
		n+= pSkin->nWidthPerCell;
	}

	// Draw nufill
	n = nFillWidth+pSkin->nWidthPerCell/2;
	while ( n<=rt.Width()-pSkin->nWidthPerCell/2)
	{
		TransparentImage(hDC,
			n,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			pSkin->hBitmap,
			sld_offset_unfill * pSkin->nWidthPerCell ,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			SKIN_TRANS_COLOR);
		n+= pSkin->nWidthPerCell;
	}

	// Draw End
	TransparentImage(hDC,
			rt.Width()-pSkin->nWidthPerCell,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			pSkin->hBitmap,
			sld_offset_end * pSkin->nWidthPerCell ,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			SKIN_TRANS_COLOR);
		n+= pSkin->nWidthPerCell;
	
		
	// Draw slider
	TransparentImage(hDC,
			nFillWidth,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			pSkin->hBitmap,
			(m_bMouseDown ? sld_offset_dwn:sld_offset_nor) * pSkin->nWidthPerCell ,0, pSkin->nWidthPerCell, pSkin->nHeightPerCell,
			SKIN_TRANS_COLOR);


/*
	sld_offset_nor = 0,
	sld_offset_dwn,
	sld_offset_bgn,
	sld_offset_fill,
	sld_offset_unfill,
	sld_offset_end
*/	
}


void   CExStatic::SkinDrawBkFromParent(HDC hDC, DWORD  dwSkinPntID)
{
	PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(dwSkinPntID,true );
	ASSERT( pSkin != NULL );

	CRect rt;
	GetWindowRect(&rt);
	GetOwner()->ScreenToClient(&rt);
	
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP hPrevBmp = (HBITMAP)::SelectObject(hMemDC, pSkin->hBitmap);

	// Draw background for static control
	::BitBlt(hDC, 0,0,rt.Width(),rt.Height(),
		hMemDC, rt.left, rt.top, SRCCOPY );
	
	//TransparentImage(hDC,0,0, rt.Width(), rt.Height(),
	//	             pSkin->hBitmap,
	//	             rt.left,rt.top, rt.Width(), rt.Height(),
	//				 SKIN_TRANS_COLOR );

	SelectObject(hMemDC, hPrevBmp);
	::DeleteDC( hMemDC );
	

}


void  CExStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( m_DrawItem.stcStyle == STC_SLIDER )
	{
		CRect rt;
		GetWindowRect(&rt);
		
		SetCapture();
		
		if ( abs( point.x * 100 / rt.Width() - m_DrawItem.nNumber) < 3)
		{
			m_bMouseDown = TRUE;
		}else
		{
			m_bMouseDown = FALSE;
		}
		Invalidate();
	} else 
	{
		CStatic::OnLButtonDown(nFlags, point);
	}

}

void  CExStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_DrawItem.stcStyle == STC_SLIDER )
	{
		if ( m_bMouseDown )
		{
			CRect rt;
			GetWindowRect(&rt);
			m_bMouseDown = TRUE;
			int nPercent = (point.x * 100.0 / rt.Width()) + 0.5;
			m_DrawItem.nNumber = nPercent;
			if ( m_DrawItem.nNumber > 100 )
				m_DrawItem.nNumber = 100;
			if ( m_DrawItem.nNumber <0 )
				m_DrawItem.nNumber = 0;

			Invalidate();
		}
	}
	
	CStatic::OnMouseMove(nFlags, point);
	
}

void  CExStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( m_DrawItem.stcStyle == STC_SLIDER )
	{
		ReleaseCapture();
		
		if ( m_bMouseDown )
		{
			m_bMouseDown = FALSE;
		} else {
			CRect rt;
			GetWindowRect(&rt);
			
			int nPercent = (point.x * 100.0 / rt.Width()) + 0.5;
			m_DrawItem.nNumber = nPercent;
			
			if ( m_DrawItem.nNumber > 100 )
				m_DrawItem.nNumber = 100;
			if ( m_DrawItem.nNumber <0 )
				m_DrawItem.nNumber = 0;
		}
		Invalidate();
				
		int nID = GetWindowLong(GetSafeHwnd(), GWL_ID);
		::SendMessage(::GetParent(GetSafeHwnd()),WM_COMMAND,
			MAKEWPARAM(nID,BN_CLICKED),(LPARAM)GetSafeHwnd() );
	} else {
		
		CStatic::OnLButtonUp(nFlags, point);
	}
}
