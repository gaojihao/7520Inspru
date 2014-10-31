// ExButton.cpp : implementation file
//

#include "stdafx.h"
#include "ExButton.h"

#include "ResMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExButton

#define  PATH_SOUND_KEYCLICK   _T("\\Style\\KeySound.wav\0")
IResMng*  CExButton::s_pResMng = CResMng::Instance();

CExButton::CExButton()
{
	m_BtnDrawItem.dwSkinID = bmp_id_none;
	m_BtnDrawItem.dwStyle  = KDS_BTN_AUTOBMP | KDS_BTN_DFTTEXT;
	m_BtnDrawItem.s_col.FontID = sf_16;
	m_BtnDrawItem.s_col.TxNorCol = sc_normal;
	m_BtnDrawItem.s_col.TxtSelCol = sc_select;
	m_BtnDrawItem.s_col.TxtDisCol = sc_disable;
	
}

CExButton::~CExButton()
{
}



BEGIN_MESSAGE_MAP(CExButton, CButton)
	//{{AFX_MSG_MAP(CExButton)
		ON_WM_ERASEBKGND()
		ON_WM_LBUTTONDOWN()
		ON_WM_CTLCOLOR()
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
#if defined(_ARM_)
	static TCHAR  szPath[MAX_PATH];
	ZeroMemory(szPath,sizeof(szPath));
	GetModuleFileName(NULL, szPath, MAX_PATH);
	_tcsncpy(_tcsrchr(szPath, 92), PATH_SOUND_KEYCLICK, sizeof(PATH_SOUND_KEYCLICK) ); 
//
//	ioc_audmute(0);
//	Sleep(100);
	::PlaySound(szPath, NULL, SND_ASYNC | SND_NODEFAULT );

//	ioc_audmute(1);

#endif
	
/*
		HWND hPrevHwnd = ::SetFocus(GetSafeHwnd());
		
		if ( ::IsWindow(hPrevHwnd) &&  
			 hPrevHwnd != GetSafeHwnd() && 
			 ::IsWindowVisible(hPrevHwnd) )
		{
			TCHAR szBuf[64];
			ZeroMemory(szBuf, sizeof(TCHAR) * 64 );
			GetClassName(hPrevHwnd, szBuf, 64);
			
			if ( _tcscmp(szBuf, _T("Button")) == 0 )
				::RedrawWindow(hPrevHwnd,NULL,NULL,RDW_INTERNALPAINT|RDW_UPDATENOW );
		} 
		
		int nID = GetWindowLong(GetSafeHwnd(), GWL_ID);
			::SendMessage(::GetParent(GetSafeHwnd()),WM_COMMAND,
				MAKEWPARAM(nID,BN_CLICKED),(LPARAM)GetSafeHwnd() );
*/
		CButton::OnLButtonDown(nFlags, point);

}

BOOL CExButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}

CExButton&    CExButton::SetBtnCol(BYTE NorCol, BYTE SelCol, BYTE DisCol)
{
	m_BtnDrawItem.s_col.TxNorCol = NorCol;
	m_BtnDrawItem.s_col.TxtSelCol = SelCol;
	m_BtnDrawItem.s_col.TxtDisCol = DisCol;
	return (*this);
}
CExButton&    CExButton::GetBtnCol(BYTE& NorCol, BYTE& SelCol, BYTE& DisCol)
{
	NorCol	= m_BtnDrawItem.s_col.TxNorCol	;
	SelCol	= m_BtnDrawItem.s_col.TxtSelCol	;
	DisCol	= m_BtnDrawItem.s_col.TxtDisCol	;
	return (*this);
}
/////////////////////////////////////////////////////////////////////////////
// CExButton message handlers
CExButton&    CExButton::SetBtnResID(unsigned nResID)
{
//	if ( m_BtnDrawItem.dwSkinID == nResID )
//		return *this;
	
	m_BtnDrawItem.dwSkinID = nResID;

	SetButtonStyle( GetButtonStyle() | BS_OWNERDRAW );

	if ( ((m_BtnDrawItem.dwStyle & KDS_CTRL_BKGBMP) == KDS_CTRL_BKGBMP) &&
		 ((m_BtnDrawItem.dwStyle & KDS_AUTOSIZE) == KDS_AUTOSIZE) )
	{
		PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(nResID, false);
		ASSERT( pSkin != NULL);
		if ( pSkin != NULL )
		{
			CRect rt;
			GetClientRect(&rt);
			SetWindowPos(NULL,0,0, pSkin->nWidthPerCell,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
	}
	return *this;
}

CExButton& CExButton::SetBtnStyle(DWORD dwStyle)
{
	m_BtnDrawItem.dwStyle = dwStyle;
	return *this;
}
CExButton& CExButton::GetBtnStyle(DWORD& dwStyle)
{
	dwStyle = m_BtnDrawItem.dwStyle;
	return *this;
}

CExButton&  CExButton::SetBtnFontID(BYTE nFontID)
{
	m_BtnDrawItem.s_col.FontID = nFontID;
	return *this;
}

HBRUSH CExButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

void CExButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	CRect  rtClient;
	GetClientRect(&rtClient);

	
	HDC     hMemDC = CreateCompatibleDC( lpDrawItemStruct->hDC );
	HBITMAP hMemBmp = CreateCompatibleBitmap(lpDrawItemStruct->hDC, 
		rtClient.Width(), rtClient.Height()	);

	HBITMAP hPreBmp = (HBITMAP)SelectObject( hMemDC, hMemBmp );


	PSKINBMP_ST  pSkin = s_pResMng->RequestBmp(m_BtnDrawItem.dwSkinID,false);
	ASSERT( pSkin != NULL);

	int       nBmpState = 0;
	COLORREF  nTextCol  = s_pResMng->RequestCol(m_BtnDrawItem.s_col.TxNorCol);
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED )
	{
		nBmpState = 1;
		nTextCol = s_pResMng->RequestCol(m_BtnDrawItem.s_col.TxtSelCol);
	}
	else if ( (lpDrawItemStruct->itemState & ODS_DISABLED) == ODS_DISABLED )
	{	
		nBmpState = 2;
		nTextCol = s_pResMng->RequestCol(m_BtnDrawItem.s_col.TxtDisCol);
	}
	 
	// 画背景
	if ( (m_BtnDrawItem.dwStyle & KDS_CTRL_BKGBMP) == KDS_CTRL_BKGBMP )
	{

		int nOffset = s_pResMng->CalBmpOffset( m_BtnDrawItem.dwSkinID );
		HDC      hTempDC = CreateCompatibleDC( hMemDC );
		HBITMAP  hTempBmp = (HBITMAP)SelectObject(hTempDC, pSkin->hBitmap);

		BitBlt(hMemDC,0,0, pSkin->nWidthPerCell,pSkin->nHeightPerCell,
		             hTempDC,
		             pSkin->nWidthPerCell * nOffset , nBmpState*pSkin->nHeightPerCell,
					 SRCCOPY );
		SelectObject(hTempDC, hTempBmp);
		DeleteDC(hTempDC);
	}
//
//	
//		TransparentImage(hMemDC,0,0,pSkin->nWidthPerCell,pSkin->nHe2ightPerCell,
//		pSkin->hBitmap,nOffset*pSkin->nWidthPerCell ,
//		    nBmpState*pSkin->nHeightPerCell,
//		    pSkin->nWidthPerCell,
//			pSkin->nHeightPerCell, 
//			SKIN_TRANS_COLOR );
//	}


	// 画文字
	if ( (m_BtnDrawItem.dwStyle & KDS_CTRL_FKTEXT) == KDS_CTRL_FKTEXT )
	{
		//CString strText;
		//GetWindowText(strText);
		
		COLORREF PreCol = SetTextColor(hMemDC, nTextCol );
		BYTE     byFontID = m_BtnDrawItem.s_col.FontID;
		HFONT    hTxtFont = s_pResMng->RequestFont(byFontID);
		
		HFONT   hPreFont = (HFONT)SelectObject(hMemDC, hTxtFont );
		::SetBkMode(hMemDC,TRANSPARENT);
		
		
		if ( (m_BtnDrawItem.dwStyle & KDS_TEXT_HCENTER) == KDS_TEXT_HCENTER )
		{
			
			//取得窗口文字
			TCHAR szTxt[MAX_PATH]={0};
			GetWindowText(szTxt,MAX_PATH);
			
			RECT rtTxt;
			rtTxt.left = rtTxt.top = rtTxt.right = rtTxt.bottom = 0;

			::DrawText(hMemDC,szTxt,_tcslen(szTxt),&rtTxt,DT_CALCRECT);
			//::DrawText(hMemDC,(LPCTSTR)m_strTitle,m_strTitle.GetLength(),&rtTxt,DT_CALCRECT);
			
				
			if( (m_BtnDrawItem.dwStyle & KDS_TEXT_VBOTTOM) == KDS_TEXT_VBOTTOM )
			{
				
				int nTxtHight = rtTxt.bottom - rtTxt.top;
				int nTxtWidth = rtTxt.right - rtTxt.left;
				
				rtTxt.left	=  (rtClient.right-nTxtWidth)/2;
				rtTxt.right =  rtTxt.left + nTxtWidth;
				rtTxt.bottom = rtClient.bottom - 4;

				if ( nBmpState == 1 )  rtTxt.bottom += 2;

				rtTxt.top    = rtTxt.bottom - nTxtHight;

			} 
			else
			{
				rtTxt.left = rtClient.left;
				rtTxt.top = rtClient.top;
				rtTxt.right = rtClient.right;
				rtTxt.bottom = rtClient.bottom;
			}
			::DrawText(hMemDC,szTxt,_tcslen(szTxt),&rtTxt,DT_CENTER | DT_VCENTER );
			//::DrawText(hMemDC,(LPCTSTR)m_strTitle,m_strTitle.GetLength(),&rtTxt,DT_CENTER | DT_VCENTER);

		}

		SelectObject(hMemDC, hPreFont );
		SetTextColor(hMemDC, PreCol);
	}
	
	::BitBlt(lpDrawItemStruct->hDC, 0,0, rtClient.Width(), rtClient.Height(),
		hMemDC, 0, 0,  SRCCOPY );
	SelectObject(hMemDC, hPreBmp);
	DeleteObject( hMemBmp );
	DeleteDC( hMemDC );
	
//////////////////////////////////////////////////////////////////////////
//此处加延时处理是为了实现按键时的柔软效果.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED )
	{
		Sleep(100);
	}
//////////////////////////////////////////////////////////////////////////

	
}

LRESULT CExButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if ( message == WM_SETTEXT )
	{
		m_strTitle.Format(_T("%s"), (LPCTSTR)(lParam ) );
//		TRACE1("BUTTON : %s\n", (LPCTSTR)(lParam ) );
		// return TRUE;

	} else if ( message == WM_LBUTTONDBLCLK )
	{
		message = WM_LBUTTONDOWN;
	} 
	
	return CButton::DefWindowProc(message, wParam, lParam);
}