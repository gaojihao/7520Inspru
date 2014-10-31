// GSMDial.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "GSMDial.h"
#include "GSMDialing.h"
#include "E9101MainDlg.h"
#include "../ExTool/ex_file.h"

#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGSMDial dialog

#define  TID_CHEK_EXTNO   3011
#define  TID_AUTO_DIAL    3012
#define  TID_HANGUP_GSM   3013
#define  TID_COUNT_SEC    3014

CString    CGSMDial::m_UserNum = _T("");
BOOL       CGSMDial::m_bAutoDial = FALSE;
BOOL		CGSMDial::m_bIsOpen = FALSE;	//窗口是否已经打开

CGSMDial::CGSMDial(CWnd* pParent /*=NULL*/)
	: CDialogBase(CGSMDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGSMDial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGSMDial::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGSMDial)
	DDX_Control(pDX, IDC_BTN_EXIT, m_BtnExit);
	DDX_Control(pDX,IDC_BTN_1, m_Btn1);
	DDX_Control(pDX,IDC_BTN_2, m_Btn2);
	DDX_Control(pDX,IDC_BTN_3, m_Btn3);
	DDX_Control(pDX,IDC_BTN_4, m_Btn4);
	DDX_Control(pDX,IDC_BTN_5, m_Btn5);
	DDX_Control(pDX,IDC_BTN_6, m_Btn6);
	DDX_Control(pDX,IDC_BTN_7, m_Btn7);
	DDX_Control(pDX,IDC_BTN_8, m_Btn8);
	DDX_Control(pDX,IDC_BTN_9, m_Btn9);
	DDX_Control(pDX,IDC_BTN_10, m_Btnstar);
	DDX_Control(pDX,IDC_BTN_0, m_Btn0);
	DDX_Control(pDX,IDC_BTN_11, m_Btnshar);
	DDX_Control(pDX,IDC_BTN_DiaOut, m_BtnOut);
	DDX_Control(pDX,IDC_BTN_BACK, m_BtnBack);
	DDX_Control(pDX,IDC_BTN_DiaIn, m_BtnIn);
	DDX_Control(pDX,IDC_STATIC_NUM, m_StcNum);
	DDX_Control(pDX,IDC_BTN_VOL_DOWN, m_BtnVolDown);
	DDX_Control(pDX,IDC_BTN_VOL_UP, m_BtnVolUp);
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGSMDial, CDialogBase)
	//{{AFX_MSG_MAP(CGSMDial)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
	ON_BN_CLICKED(IDC_BTN_2, OnBtn2)
	ON_BN_CLICKED(IDC_BTN_3, OnBtn3)
	ON_BN_CLICKED(IDC_BTN_4, OnBtn4)
	ON_BN_CLICKED(IDC_BTN_5, OnBtn5)
	ON_BN_CLICKED(IDC_BTN_6, OnBtn6)
	ON_BN_CLICKED(IDC_BTN_7, OnBtn7)
	ON_BN_CLICKED(IDC_BTN_8, OnBtn8)
	ON_BN_CLICKED(IDC_BTN_9, OnBtn9)
	ON_BN_CLICKED(IDC_BTN_10, OnBtn10)
	ON_BN_CLICKED(IDC_BTN_0, OnBtn0)
	ON_BN_CLICKED(IDC_BTN_11, OnBtn11)
	ON_BN_CLICKED(IDC_BTN_DiaOut, OnBTNDiaOut)
	ON_BN_CLICKED(IDC_BTN_BACK, OnBtnBack)
	ON_BN_CLICKED(IDC_BTN_DiaIn, OnBTNHangup)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_STATIC_NUM, OnStaticNum)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_VOL_UP, OnBtnVolUp)
	ON_BN_CLICKED(IDC_BTN_VOL_DOWN, OnBtnVolDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGSMDial message handlers


BOOL CGSMDial::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	//CUiMng::Instance().Detach(this);

	m_bIMeShow = FALSE;
	m_bIsDialing = FALSE;
	m_bDialOut = FALSE;
	m_strExtNo = _T("");

	OnLanguageChange();
	OnSkinChange();

	InitGdi();
	InitControl();

	UINT nLev = CSysConfig::Instance().GetGSMVol();
	if ( nLev > 11 ) 
		nLev = 11;
	if ( nLev < 0 ) 
		nLev = 0;
	if ( nLev )
		g_GsmLogic.GSMSetVol( nLev * 8 + 1  );

	m_ItemNum.chChar = m_UserNum;
	if ( m_bAutoDial )
	{
		SetTimer(TID_AUTO_DIAL, 100, NULL);
	}
	m_bIsOpen	= TRUE;
	SetTimer(TID_SET_TOP, 500, NULL);
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CGSMDial::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_BUTTON_SMS_DIALING_TITLE);
	m_stTitle = str;

	SetWindowText(str);
	str.LoadString( RES_BUTTON_PHONE_CITY ) ;
	m_Info = str;
	return TRUE;
}

BOOL CGSMDial::OnSkinChange()
{
	return TRUE;
}

void CGSMDial::OnBtnExit() 
{
	m_ItemNum.chChar.Empty();
	g_GsmLogic.GSMHangup();
	m_bIsDialing = FALSE;

	if( GetModal() )
		OnExit();
	else
		SetTimer(TID_CLOSE, 1000, NULL);
}

void CGSMDial::OnBtn1() 
{
	AddPhoneNum(_T('1'));
}

void CGSMDial::OnBtn2() 
{
	AddPhoneNum(_T('2'));
}

void CGSMDial::OnBtn3() 
{
	AddPhoneNum(_T('3'));
}

void CGSMDial::OnBtn4()
{
	AddPhoneNum(_T('4'));
}

void CGSMDial::OnBtn5() 
{
	AddPhoneNum(_T('5'));
}

void CGSMDial::OnBtn6() 
{
	AddPhoneNum(_T('6'));
}

void CGSMDial::OnBtn7() 
{
	AddPhoneNum(_T('7'));
}

void CGSMDial::OnBtn8() 
{
	AddPhoneNum(_T('8'));
}

void CGSMDial::OnBtn9() 
{
	AddPhoneNum(_T('9'));
}

void CGSMDial::OnBtn10() 
{
	AddPhoneNum(_T('*'));
}

void CGSMDial::OnBtn0() 
{
	AddPhoneNum(_T('0'));
}

void CGSMDial::OnBtn11() 
{
	AddPhoneNum(_T('#'));
}

void   CGSMDial::AddPhoneNum(TCHAR chNum, BOOL bAdd /*= TRUE*/ )
{
	//电话短信功能开关，当电话功能关闭时，在一键导航界面进入时不能点击数字按钮
	if( !CE9101MainDlg::m_bPhone )
		return;

	if ( bAdd )
	{
		if ( chNum == _T('0') )
		{
			if ( GetTickCount() - m_nZeroTick > 500 )
				chNum = _T('+');
		}

		if ( m_bIsDialing )
		{
			if ( m_strExtNo.GetLength() < 32 )
			{
				m_strExtNo += chNum;
				KillTimer( TID_CHEK_EXTNO );
				SetTimer( TID_CHEK_EXTNO, 1000, 0 );
			}
		}
		else
		{
			if ( m_ItemNum.chChar.GetLength() < 32 )
				m_ItemNum.chChar += chNum;
		}
	}
	else
	{
		if ( m_bIsDialing )
		{
			if ( m_strExtNo.GetLength() > 0 )
				m_strExtNo.Delete(m_strExtNo.GetLength()-1,1);
		}else
		{
			if ( m_ItemNum.chChar.GetLength() >0 )
				m_ItemNum.chChar.Delete(m_ItemNum.chChar.GetLength()-1,1);
		}
	}
}

void CGSMDial::OnBTNDiaOut() 
{
	if ( m_ItemNum.chChar.GetLength() <= 0 )
		return;

	if ( m_ItemNum.chChar == _T("*#06#") )
	{
		m_bIMeShow = TRUE;
		LoadIMEFromText();
	}
	else if ( !m_bIsDialing )
	{
		CGSMDialing dlg;
		dlg.SetCallPhone( m_ItemNum.chChar );
		if ( dlg.DoModal() == IDOK )
		{
			KillTimer(TID_COUNT_SEC);
			SetTimer(TID_COUNT_SEC, 1000, NULL);
			m_DialCount = 0;
			m_bIsDialing = TRUE;
			Invalidate();
		}
		else
		{
			m_bIsDialing = FALSE;
			KillTimer( TID_HANGUP_GSM );
			SetTimer(TID_HANGUP_GSM, 10, NULL);
		}
	}
}

void CGSMDial::OnServerDiaOut(CString strphone)
{
	if ( !m_bIsDialing )
	{
		CGSMDialing dlg;
		dlg.SetCallPhone( strphone );
		if ( dlg.DoModal() == IDOK )
		{
			KillTimer(TID_COUNT_SEC);
			SetTimer(TID_COUNT_SEC, 1000, NULL);
			m_DialCount = 0;
			m_bIsDialing = TRUE;
			Invalidate();
		}
		else
		{
			m_bIsDialing = FALSE;
			KillTimer( TID_HANGUP_GSM );
			SetTimer(TID_HANGUP_GSM, 10, NULL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CGSMDial::OnBtnBack() 
{
	AddPhoneNum(_T(''), FALSE);
}

//////////////////////////////////////////////////////////////////////////

void CGSMDial::OnBTNHangup() 
{
	g_GsmLogic.GSMHangup();
	m_bIsDialing = FALSE;
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CGSMDial::OnBtnReset() 
{
}

//////////////////////////////////////////////////////////////////////////

void CGSMDial::OnStaticNum() 
{
	m_StcNum.SetWindowText(_T(""));
}

void CGSMDial::OnTimer( UINT nIDEvent ) 
{
	if ( TID_CHEK_EXTNO == nIDEvent )
	{
		KillTimer( TID_CHEK_EXTNO );
		if ( m_strExtNo.GetLength() > 0 )
		{
			g_GsmLogic.SetPhoneExt( m_strExtNo );
			m_ItemNum.chChar += m_strExtNo;
			m_strExtNo = _T("");
			g_GsmLogic.GSMDialExt();	
		}
	}else if ( TID_AUTO_DIAL == nIDEvent )
	{
		KillTimer( nIDEvent );
		m_UserNum = _T("");
	    m_bAutoDial = FALSE;
		OnBTNDiaOut();
	}else if ( 	TID_HANGUP_GSM == nIDEvent )
	{
		KillTimer( nIDEvent );
		g_GsmLogic.GSMHangup();
	}

	else if ( TID_COUNT_SEC == nIDEvent )
	{
		if ( m_bIsDialing )
		{
			m_DialCount++;
			Invalidate();
		}
	}
	else if(TID_CLOSE == nIDEvent)
	{
		OnExit();
		KillTimer(nIDEvent);
	}
	else if(TID_SET_TOP == nIDEvent)
	{
		SetWindowPos(&wndTopMost, 0, 0, 800, 480, SWP_SHOWWINDOW);
		KillTimer(nIDEvent);
	}
	CDialogBase::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////


void CGSMDial::OnBtnVolUp() 
{
	UINT nVol = CSysConfig::Instance().GetGSMVol();
	if ( nVol < 11 )
	{
		nVol++;
		CSysConfig::Instance().SetGSMVol( nVol );
		g_GsmLogic.GSMSetVol( nVol * 8 + 1  );
		Invalidate( FALSE );
	}
}

//////////////////////////////////////////////////////////////////////////


void CGSMDial::OnBtnVolDown() 
{
	UINT nVol = CSysConfig::Instance().GetGSMVol();
	if ( nVol > 0 )
	{
		nVol--;
		CSysConfig::Instance().SetGSMVol( nVol );
		g_GsmLogic.GSMSetVol( nVol * 8 + 1  );
		Invalidate( FALSE );
	}
}

//////////////////////////////////////////////////////////////////////////

void CGSMDial::OnExit()
{
	g_GsmLogic.GSMHangup();
	m_ItemNum.chChar = _T("");
	m_strExtNo = _T("");
	m_bIsDialing = FALSE;

	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	bk_vol_dc.SelectObject( m_pold_bk_vol_bmp );

	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	DeleteObject( m_stVolBmp );

	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	bk_vol_dc.DeleteDC();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	m_bIsOpen	= FALSE;
	Close();

	return;
}

//窗口是否已经打开
BOOL CGSMDial::IsOpen()
{
	return m_bIsOpen;
}

void CGSMDial::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//

	bk_normal_dc.CreateCompatibleDC( m_pDC );
	bk_press_dc.CreateCompatibleDC( m_pDC );
	bk_vol_dc.CreateCompatibleDC( m_pDC );
	bk_disable_dc.CreateCompatibleDC( m_pDC );
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_DIAOUT_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_DIAOUT_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;   

	pSkin = g_pResMng->RequestBmp(SKIN_BG_DIAOUT_D, true);
	m_stBtnDisableBMP = pSkin->hBitmap;  
	

	pSkin = g_pResMng->RequestBmp(SKIN_BG_VOL_LIST, true);
	m_stVolBmp = pSkin->hBitmap;   

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject( CBitmap::FromHandle(m_stBtnNormalBMP) );
	m_pold_bk_press_bmp   = bk_press_dc.SelectObject( CBitmap::FromHandle(m_stBtnPressBMP) );
	m_pold_bk_disable_bmp = bk_disable_dc.SelectObject( CBitmap::FromHandle(m_stBtnDisableBMP) );
	m_pold_bk_vol_bmp     = bk_vol_dc.SelectObject( CBitmap::FromHandle(m_stVolBmp) );

	::ReleaseDC(m_hWnd, hDC);
}

//////////////////////////////////////////////////////////////////////////

void CGSMDial::InitControl()
{
	int i =0;
	for (i =0;  i < 19; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	for (i = 0; i < 15; i++) 
	{
		if (i < 3)
		{//7,4,1
			m_Item[i].rect.left		= 5;
			m_Item[i].rect.top		= 191+i*96;
			m_Item[i].rect.right	= 5+113;
			m_Item[i].rect.bottom	= 191+i*96+96;
		}
		else if (i > 2 && i < 6)
		{//8,5,2
			m_Item[i].rect.left		= 5+113;
			m_Item[i].rect.top		= 191+(i-3)*96;
			m_Item[i].rect.right	= 5+113*2;
			m_Item[i].rect.bottom	= 191+(i-3)*96+96;
		}
		else if (i > 5 && i < 9)
		{//9,6,3
			m_Item[i].rect.left		= 5+113*2;
			m_Item[i].rect.top		= 191+(i-6)*96;
			m_Item[i].rect.right	= 5+113*3;
			m_Item[i].rect.bottom	= 191+(i-6)*96+96;
		}
		else if (i > 8 && i < 12)
		{//*,0/+,#
			m_Item[i].rect.left		= 5+113*3;
			m_Item[i].rect.top		= 191+(i-9)*96;
			m_Item[i].rect.right	= 5+113*4;
			m_Item[i].rect.bottom	= 191+(i-9)*96+96;
		}
	}
	//拨出
	m_Item[12].rect.left	= 570;
	m_Item[12].rect.top		= 287;
	m_Item[12].rect.right	= 800;
	m_Item[12].rect.bottom	= 382;
	//清空
	m_Item[13].rect.left	= 5+113*4;
	m_Item[13].rect.top		= 191+2*96;
	m_Item[13].rect.right	= 5+113*5;
	m_Item[13].rect.bottom	= 191+2*96+96;
	//挂断
	m_Item[14].rect.left	= 570;
	m_Item[14].rect.top		= 383;
	m_Item[14].rect.right	= 800;
	m_Item[14].rect.bottom	= 480;
	//+
	m_Item[15].rect.left	= 5+113*5;
	m_Item[15].rect.top		= 191;
	m_Item[15].rect.right	= 5+113*6;
	m_Item[15].rect.bottom	= 191+96;
	//-
	m_Item[16].rect.left	= 5+113*4;
	m_Item[16].rect.top		= 191;
	m_Item[16].rect.right	= 5+113*5;
	m_Item[16].rect.bottom	= 191+96;
	//删除钮  <-
	m_Item[17].rect.left	= 5+113*4;
	m_Item[17].rect.top		= 191+96;
	m_Item[17].rect.right	= 5+113*5;
	m_Item[17].rect.bottom	= 191+96+96;
	//Exit
	m_Item[18].rect.left	= 740;
	m_Item[18].rect.top		= 0;
	m_Item[18].rect.right	= 800;
	m_Item[18].rect.bottom	= 56;

	//输入框
	m_ItemNum.rect.left		= 114;
	m_ItemNum.rect.top		= 82;
	m_ItemNum.rect.right	= 686;
	m_ItemNum.rect.bottom	= 163;
}

void CGSMDial::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CGSMDial::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CGSMDial::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	if ( m_bIsDialing )
	{
		m_Item[12].nState = BTN_STATE_DISABLE;
		m_Item[14].nState = BTN_STATE_NORMAL;
	}
	else
	{
		m_Item[14].nState = BTN_STATE_DISABLE;
		m_Item[12].nState = BTN_STATE_NORMAL;
	}

	for(int i=0;i<19;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	if ( m_bIsDialing && m_DialCount > 0 )
	{
		int nHour = m_DialCount / 3600;
		int nMin =  ( m_DialCount - nHour * 3600 ) / 60;
		int nSec = m_DialCount % 60;
		str.Format( _T("%02d:%02d:%02d") , nHour, nMin, nSec );
		Rect = g_retTitile;
		Rect.left = 0;
		Rect.right = 480;
		DrawBtnText(&memDC,CRect(0,145,120,185),str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}

	DrawBtnText(&memDC,g_retTitile,m_stTitle,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_CENTER|DT_VCENTER);

	//电话号码
	Rect = m_ItemNum.rect;
	DrawBtnText(&memDC,Rect,m_ItemNum.chChar + m_strExtNo ,CGDICommon::Instance()->bigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER|DT_WORDBREAK);

	// Draw vol
	{
		//左下角 435 210
		//左下角 X：399 Y：206
		UINT nLev = CSysConfig::Instance().GetGSMVol();
		if ( nLev > 11 ) nLev = 11;
		if ( nLev < 0 ) nLev = 0;

		memDC.BitBlt(720, 240 - 11 * nLev, 59, 11 * nLev, &bk_vol_dc, 0, (11-nLev)*11 ,SRCCOPY);
	}

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CGSMDial::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_nZeroTick = 0xFFFFFF;
	if ( m_bIMeShow )
	{
		m_ItemNum.chChar.Empty();
		m_bIMeShow = FALSE;
	}

	for(int i=0;i<19;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
			}
			if( i == 0)
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\1.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\1.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn1();
			}
			else if( i == 1) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\4.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\4.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn4();
			}
			else if( i == 2) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\7.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\7.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn7();
			}
			else if( i == 3) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\2.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\2.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn2();
			}
			else if( i == 4) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\5.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\5.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn5();
			}
			else if( i == 5) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\8.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\8.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn8();
			}
			else if( i == 6) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\3.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\3.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn3();
			}
			else if( i == 7)
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\6.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\6.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn6();
			}
			else if( i == 8) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\9.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\9.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn9();
			}
			else if( i == 9 )
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\star.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				else
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\star.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				OnBtn10();
			}
			else if( i == 10) 
			{
				m_nZeroTick = ::GetTickCount();
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
				{
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\0.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				}
				else
				{
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\0.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				}
			}
			else if( i == 11) 
			{
				if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
				{
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\well.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				}
				else
				{
					::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\well.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					//PlayKeySound();
				}
			}
			else if(  12 <= i && i <= 17) 
			{
				PlayKeySound();
				if ( i == 15 )
					OnBtnVolUp();
				else if ( i == 16 )
					OnBtnVolDown();
				else if ( i == 17 )
					OnBtnBack();
			}				
		}
	}
	
	Invalidate();
	SetCapture();

	CDialogBase::OnLButtonDown(nFlags, point);
}

void CGSMDial::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<19;i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
		if(PtInRect(&m_Item[i].rect,point))
		{
			switch( i )
			{
#if 0				
			case 0:
				OnBtn7();
				break;
			case 1:
				OnBtn4();
				break;
			case 2:
				OnBtn1();
				break;
			case 3:
				OnBtn8();
				break;
			case 4:
				OnBtn5();
				break;
			case 5:
				OnBtn2();
				break;
			case 6:
				OnBtn9();
				break;
			case 7:
				OnBtn6();
				break;
			case 8:
				OnBtn3();
				break;
			case 9:
				OnBtn10();
				break;
#endif
			case 10://0
				OnBtn0();
				break;
			case 11://#
				OnBtn11();
				break;
			case 12:
				OnBTNDiaOut();
				break;
			case 13:
				DelAllInput();
				//OnBtnBack();
				break;
			case 14:
				OnBTNHangup();
				break;
#if 0				
			case 15:
				OnBtnVolUp();
				break;
			case 16:
				OnBtnVolDown();
				break;
			case 17:
				OnBtnBack();
				break;
#endif				
			case 18:
				OnBtnExit();
				break;
			default:
				break;
			}
		}
	}
	m_nZeroTick = 0xFFFFFFFF;
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////

BOOL CGSMDial::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
	 if ( pMsg->wParam == GSM_ACK_LINE_DISCONNECT ||
	  	  pMsg->wParam == GSM_ACK_LINE_NOCARRIER )
		{
			OnBTNHangup();
		}
	}
	else if( pMsg->message == WM_CREATE_SET )
	{
		if( pMsg->wParam == 0x06 )
		{
			OnBtnExit();
		}
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
void   CGSMDial::DelAllInput()
{
	if ( m_bIsDialing )
	{
		if ( m_strExtNo.GetLength() > 0 )
			m_strExtNo.Empty();
	}else
	{
		if ( m_ItemNum.chChar.GetLength() >0 )
			m_ItemNum.chChar.Empty();
	}

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void   CGSMDial::LoadIMEFromText( )
{
	char  imei[16];
	memset(imei, 0, sizeof(imei) );
	char* p_path = "\\Flashdrv Storage\\IMEI.txt";
	cex_file file;
	if ( file.open(p_path, cex_file::m_readwrite ) )
	{
		file.read(imei, 15);
		file.close();
		TCHAR szBuf[16];
		memset( szBuf, 0, sizeof(szBuf) );
		ex_char2uni(imei, szBuf, 16 );
		m_ItemNum.chChar.Format( _T("IMEI: %s !"), szBuf );
		Invalidate();
	}
}

void CGSMDial::PostNcDestroy()
{
	//非模态窗口才需要处理
	if( !GetModal() )
	{
		CDialogBase::PostNcDestroy();
		delete this;
	}
}
