// DialogBase.cpp: implementation of the CDialogBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogBase.h"
#include "CUiMng.h"
//#include "resource.h"
#include "../SkinMng/ResMng.h"

#include "../SysConfig/SysConfig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IResMng*  CDialogBase::g_pResMng = CResMng::Instance();

CDialogBase::CDialogBase(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
	m_bModal = TRUE;	//默认为模态对话框
	//{{AFX_DATA_INIT(CDialogBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_DlgDrawItem.dwSkinID = bmp_id_none;
	m_DlgDrawItem.TitlePt.x = 0;
	m_DlgDrawItem.TitlePt.y = 0;
	m_DlgDrawItem.dwStyle = KDS_BTN_AUTOBMP;
	m_DlgDrawItem.s_Text.FontID = SF_BUTTON_NORMAL_CHINESE;
	m_DlgDrawItem.s_Text.TitleCol = sc_normal;
	m_nIDTemplate = nIDTemplate;	//当前dialog资源的IDD
}

CDialogBase::CDialogBase()
	:CDialog()
{
	m_bModal = TRUE;
}

void CDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBase)
	
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

LRESULT CDialogBase::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_ACTIVATE:
		if (WA_ACTIVE == LOWORD(wParam) )
		{
			if ( IsWindow( GetSafeHwnd() ) &&  IsWindowVisible() )
				RedrawWindow(NULL,NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
		}
		break;
	default: 
		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

CDialogBase& CDialogBase::SkinSetResID(DWORD dwSkinID)
{
	m_DlgDrawItem.dwSkinID = dwSkinID;

	if ( ((m_DlgDrawItem.dwStyle & KDS_CTRL_BKGBMP) == KDS_CTRL_BKGBMP) &&
		 ((m_DlgDrawItem.dwStyle & KDS_AUTOSIZE) == KDS_AUTOSIZE) )
	{
		PSKINBMP_ST  pSkin = g_pResMng->RequestBmp(dwSkinID,true);
		if ( pSkin != NULL )
		{
			SetWindowPos(NULL,0,0, pSkin->nWidthPerCell,pSkin->nHeightPerCell, SWP_NOMOVE | SWP_NOZORDER );
		}
	}
	return *this;
}

CDialogBase& CDialogBase::SkinSetDlgStyle(DWORD dwStyle)
{
	m_DlgDrawItem.dwStyle = dwStyle;
	return *this;
}


CDialogBase&  CDialogBase::SkinSetTitlePos(CPoint pt)
{
	m_DlgDrawItem.TitlePt.x = pt.x;
	m_DlgDrawItem.TitlePt.y = pt.y;
	return *this;
}

BOOL CDialogBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	MoveWindow(0,0,DLG_WIDTH,DLG_HIGHT);
	CenterWindow(GetDesktopWindow());

//	PositionControl();
//
//	KSKINITEM SkinItem;
//	SkinItem.CtlType = KCT_DIALOG;
//	SkinItem.DrawStyle = KDS_BKGRGB;
//	SkinItem.BkNorCol = COLOR1;
// 	SetCtlSkin( NULL, m_hWnd, &SkinItem );
	
	//CSysConfig::Instance().SetLastDialogID(m_nIDTemplate);

	return TRUE;
}

BOOL CDialogBase::OnLanguageChange()
{
	return TRUE;
}

BOOL CDialogBase::OnSkinChange()
{
	return TRUE;
}

void CDialogBase::OnExit()
{
	EndDialog(0);
}
//BOOL CDialogBase::OnUnitChange()
//{
//	return TRUE;
// }

void CDialogBase::SetTemplateID( UINT nID )
{
	AFX_ZERO_INIT_OBJECT(CWnd);

	m_lpszTemplateName = MAKEINTRESOURCE(nID);
	m_nIDHelp = nID;
}

UINT CDialogBase::GetTemplateID()
{
	return m_nIDHelp;
}

UINT CDialogBase::Open()
{
	if (m_hWnd)		//已经打开
		return 1;	//就不要重复打开

	BOOL rts = TRUE;
	if( GetModal() )
	{
		return DoModal();
	}
	else
	{
		CWnd* pParWnd = CWnd::GetActiveWindow();
		Create( m_nIDHelp ,pParWnd );
		if(ShowWindow( SW_SHOW ))
			return 1;
		else
			return 0;
	}
}

BOOL CDialogBase::Close()
{
	if (!m_hWnd)		//已经关闭
		return FALSE;	//就不要重复关闭

	if(GetModal())
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}
	else
	{
		return DestroyWindow();
	}

	//VERIFY(::ReleaseDlgSkin(m_hWnd));	//关闭窗口时释放资源
}

void CDialogBase::SetModal( BOOL bModal )
{
	m_bModal = bModal;
}


BOOL CDialogBase::Create( UINT nIDTemplate, CWnd* pParentWnd)
{
	CDialog::Create(nIDTemplate, pParentWnd);
	return CUiMng::Instance().Attach(this);
}

int CDialogBase::DoModal( )
{
	CUiMng::Instance().Attach(this);
	return 	CDialog::DoModal();
}

void CDialogBase::EndDialog(int nResult )
{
	CUiMng::Instance().Detach(this);
	CDialog::EndDialog(nResult);
}

BOOL CDialogBase::DestroyWindow()
{
	 CUiMng::Instance().Detach(this);
	 return CDialog::DestroyWindow();
}

void CDialogBase::OnPaint() 
{
	CPaintDC  dc(this);
	RECT  rtClient;
	GetClientRect(&rtClient);

	PSKINBMP_ST  pSkin = g_pResMng->RequestBmp(m_DlgDrawItem.dwSkinID,true);
//	ASSERT( pSkin != NULL );
	if ( pSkin == NULL )
	{
		dc.FillSolidRect(&rtClient, RGB(255,255,255));
	}
	else 
	{
		
		if ( (m_DlgDrawItem.dwStyle & KDS_CTRL_BKGBMP) == KDS_CTRL_BKGBMP )
		{
			HDC     hMemDC = CreateCompatibleDC( dc.GetSafeHdc() );
			HBITMAP hPreBmp = (HBITMAP)SelectObject( hMemDC, pSkin->hBitmap );
			::BitBlt(dc.GetSafeHdc(),
				0,0,pSkin->nWidthPerCell,pSkin->nHeightPerCell,
				hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC, hPreBmp);
			::DeleteDC(hMemDC);
		}
		
		// 画文字
		if ( (m_DlgDrawItem.dwStyle & KDS_CTRL_FKTEXT) == KDS_CTRL_FKTEXT )
		{
			
			CString strText;
			GetWindowText(strText);
			
			COLORREF  nTextCol  = g_pResMng->RequestCol(m_DlgDrawItem.s_Text.TitleCol);
			COLORREF PreCol = SetTextColor(dc.GetSafeHdc(), nTextCol );
			
			DWORD     byFontID = m_DlgDrawItem.s_Text.FontID;
			HFONT    hTxtFont = g_pResMng->RequestFont(byFontID);
			
			HFONT   hPreFont = (HFONT)SelectObject(dc.GetSafeHdc(), hTxtFont );
			::SetBkMode(dc.GetSafeHdc(),TRANSPARENT);
			
			rtClient.left = m_DlgDrawItem.TitlePt.x;
			rtClient.top = m_DlgDrawItem.TitlePt.y;
			
			dc.DrawText(strText, &rtClient, DT_TOP | DT_LEFT | DT_SINGLELINE );
			
			SelectObject( dc.GetSafeHdc(), hPreFont );
			SetTextColor( dc.GetSafeHdc(), PreCol);
		}
	}
}

BOOL CDialogBase::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDialogBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->IsKindOf(RUNTIME_CLASS(CButton)))
	{
		return (HBRUSH)GetStockObject(NULL_BRUSH);	
	}

	return hbr;
}



BOOL CDialogBase::GetModal()
{
	return m_bModal;
}

//BOOL CDialogBase::ReflushData()
//{
//	return PostMessage(WM_RealTimeData);
//}

BEGIN_MESSAGE_MAP(CDialogBase, CDialog)
	//{{AFX_MSG_MAP(CDialogBase)
//		ON_COMMAND(ID_E9200N_ENTER,OnEnter)
		ON_WM_PAINT()
		ON_WM_ERASEBKGND()
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// void CDialogBase::PostNcDestroy()
// {
// 	CDialog::PostNcDestroy();
// 	delete this;
// }
