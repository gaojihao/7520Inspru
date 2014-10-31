// VolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "VolumeDlg.h"
//#include "../DriveLib/ak4642api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg dialog


int CVolumeDlg::m_VolLevel = 10;

CVolumeDlg::CVolumeDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CVolumeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeDlg)

	DDX_Control(pDX,IDC_STC_VOLUME,m_StcVolume);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeDlg, CDialogBase)
	//{{AFX_MSG_MAP(CVolumeDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg message handlers

BOOL CVolumeDlg::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	// TODO: Add extra initialization here
	MoveWindow(0,215,480,57);
//	m_StcVolume.MoveWindow(0,0,480,40);

 	//savtofile_t savefile;
 	//memset(&savefile,0,sizeof(savefile));
 
#ifdef T7006
// 	m_VolLevel = get_vol(&savefile);
 	if(m_VolLevel<0 || m_VolLevel >16)
		m_VolLevel = 10;
#endif
	
	OnSkinChange();
	
	InitGdi();
	InitControl();
	SetTimer( Timer_EVENT , 2000 ,NULL );

	::SetWindowPos( this->m_hWnd , HWND_TOPMOST,0,215,480,57, SWP_SHOWWINDOW );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVolumeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer( Timer_EVENT );
	CDialogBase::OnTimer(nIDEvent);
	OnExit();

// 	MoveWindow(0,215,480,57);
// 	Sleep(500);
// 
// //	bitmap.Detach();
// 	bitmap.DeleteObject();
// 	m_pold_bk_normal_bmp->DeleteObject();
// 	bk_normal_dc.DeleteDC();
// 	//bk_press_dc.DeleteDC();
// 	memDC.DeleteDC();
// 	EndDialog(0);
//	//DestroyWindow();
}

LRESULT CVolumeDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x00 )
			SetTimer( Timer_EVENT , 2000 ,NULL );

//	}
// 	else if( message == WM_CREATE_SET )
// 	{
 	else if( wParam == 0x06 )
 		{
			OnExit();
 		}
	}	
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
return CDialogBase::DefWindowProc(message, wParam, lParam);
}


BOOL CVolumeDlg::OnSkinChange()
{

//	m_StcVolume.SkinSetStcID(STC_BMP,bmp_file_Volume_480x40_1);
//	m_StcVolume.Invalidate();

	return TRUE;
}

void CVolumeDlg::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
//	bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_VOLUME_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

//	pSkin = g_pResMng->RequestBmp(BMP_BG_VOLUME_N, true);
//	m_stBtnPressBMP = pSkin->hBitmap;    

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
//	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

//	memDC.DeleteDC();
	::ReleaseDC(m_hWnd, hDC);
}

void CVolumeDlg::InitControl()
{

	m_Item[0].nState = BTN_STATE_NORMAL;
	m_Item[1].nState = BTN_STATE_NORMAL;

	m_Item[0].rect.top = 0;
	m_Item[0].rect.left = 0;
	m_Item[0].rect.bottom = 66;
	m_Item[0].rect.right = 86;

	m_Item[1].rect.top = 0;
	m_Item[1].rect.left = 375;
	m_Item[1].rect.bottom = 66;
	m_Item[1].rect.right = 461;
}

void CVolumeDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
	//	pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DISABLE:
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CVolumeDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
{
	COLORREF  nOldRgb;
	nOldRgb=pDC->SetTextColor( col );
	int      nOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont* def_font = pDC->SelectObject(&smallfont);

	pDC->DrawText(str, &rt, Format );

	pDC->SelectObject(def_font);
	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(nOldRgb);
}

void CVolumeDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, rc.left, m_VolLevel*57, SRCCOPY);


	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBmp);	


}

void CVolumeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetTimer( Timer_EVENT , 2000 ,NULL );
	for(int i=0;i<2;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			PlayKeySound();
				if(m_Item[i].nState == BTN_STATE_NORMAL)
				{
					m_Item[i].nState = BTN_STATE_DOWN;
				}
		}
	}

	
	Invalidate();
	SetCapture();


	CDialogBase::OnLButtonDown(nFlags, point);
}

void CVolumeDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//savtofile_t savefile;
	//memset(&savefile,0,sizeof(savefile));

	for(int i=0;i<2;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;

			if( i == 0 && m_VolLevel >0 )
			{
				m_VolLevel--;
				//set_vol_down(&savefile);
			}
			else if( i == 1 && m_VolLevel < 16 )
			{
				m_VolLevel++;
				//set_vol_up(&savefile);
			}


		}
	}
		

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);


}

void CVolumeDlg::OnExit()
{
//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
//	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

//	DeleteObject(m_stBtnDisableBMP);	
//	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

//	bk_disable_dc.DeleteDC();
//	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

//	bigfont.DeleteObject();
//	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
	CDialogBase::EndDialog(0);

}

