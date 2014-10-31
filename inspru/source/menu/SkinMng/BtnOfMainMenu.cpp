// BtnOfMainMenu.cpp : implementation file
//

#include "stdafx.h"
#include "BtnOfMainMenu.h"
#include "ResMng.h"
#include "LanguageSetType.h"
#include "CUiMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBtnOfMainMenu

CBtnOfMainMenu::CBtnOfMainMenu()
{
	m_iLeft = 75;
}

CBtnOfMainMenu::~CBtnOfMainMenu()
{

}



BEGIN_MESSAGE_MAP(CBtnOfMainMenu, CExButton)
	//{{AFX_MSG_MAP(CBtnOfMainMenu)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtnOfMainMenu message handlers

void CBtnOfMainMenu::SettextLeft(UINT iLeft)
{

	m_iLeft = iLeft;
}

void CBtnOfMainMenu::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//���뷽ʽ�ǲ��ñ��,���������໯��ʱ����һ�μ���
 	DWORD dwStyle;
 	GetBtnStyle(dwStyle);
 	dwStyle |= KDS_TEXT_HCENTER;
 	dwStyle &= ~KDS_TEXT_VBOTTOM; 
	SetBtnStyle(dwStyle);
	
	CExButton::PreSubclassWindow();
}

BOOL CBtnOfMainMenu::OnSkinChange()
{
//	CConfig* pConfig = AfxGetMainGroup()->GetConfig();
//	ASSERT(pConfig);
////////////////////////////////////////////////////////////////////////////
//	//����Ƥ�������������ɫ(to be add because of day/night mode not finish)
//	{
//	
//		BYTE NorCol,SelCol,  DisCol; 
//		GetBtnCol(NorCol,SelCol,  DisCol);
//		SetBtnCol(sc_white,SelCol,  DisCol);
//	}

	return TRUE;
}

BOOL CBtnOfMainMenu::OnLanguageChange()
{
//	CConfig* pConfig = AfxGetMainGroup()->GetConfig();
//	ASSERT(pConfig);
////////////////////////////////////////////////////////////////////////////
//	//����������������Ĵ�С(��Ҫ�����ĵİ�ť�����ĵİ�ť��С�᲻ͬ)
	enLanguage eLanguage = CUiMng::Instance().GetCurLanguage();
	{
		if (ID_CHINESE == eLanguage)
		{
			//��������������
			SetBtnFontID(SF_BUTTON_BIG_CHINESE);
		}
		else
		{
			//������������������
			SetBtnFontID(SF_BUTTON_BIG);
		}

	}
 	return TRUE;
}

void CBtnOfMainMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
	// TODO: Add your code to draw the specified item
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
	 
	// ������
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

	// ������
	if ( (m_BtnDrawItem.dwStyle & KDS_CTRL_FKTEXT) == KDS_CTRL_FKTEXT )
	{
		
		COLORREF PreCol = SetTextColor(hMemDC, nTextCol );
		BYTE     byFontID = m_BtnDrawItem.s_col.FontID;
		HFONT    hTxtFont = s_pResMng->RequestFont(byFontID);
		
		HFONT   hPreFont = (HFONT)SelectObject(hMemDC, hTxtFont );
		::SetBkMode(hMemDC,TRANSPARENT);
		
		
		//ȡ�ô�������
		TCHAR szTxt[MAX_PATH]={0};
		GetWindowText(szTxt,MAX_PATH);
		
		RECT rtTxt;
		rtTxt.left = rtClient.left ;
		rtTxt.top = rtClient.top+ m_iLeft;
		rtTxt.right = rtClient.right;
		rtTxt.bottom = rtClient.bottom;
		
		::DrawText(hMemDC,szTxt,_tcslen(szTxt),&rtTxt,DT_CENTER );
		
		SelectObject(hMemDC, hPreFont );
		SetTextColor(hMemDC, PreCol);
	}
	
	::BitBlt(lpDrawItemStruct->hDC, 0,0, rtClient.Width(), rtClient.Height(),
		hMemDC, 0, 0,  SRCCOPY );
	SelectObject(hMemDC, hPreBmp);
	DeleteObject( hMemBmp );
	DeleteDC( hMemDC );
	
//////////////////////////////////////////////////////////////////////////
//�˴�����ʱ������Ϊ��ʵ�ְ���ʱ������Ч��.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED )
	{
		Sleep(80);
	}
//////////////////////////////////////////////////////////////////////////
	
}
