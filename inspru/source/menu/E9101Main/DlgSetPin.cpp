// DlgDial.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
//#include "DlgDial.h"
//#include "../DriveLib/ak4642api.h"
#include "E9101MainDlg.h"
#include "PBookMain.h"
#include "SysConfig.h"
//#include "DialLog.h"
//#include "PhoneLogMng.h"
//#include "BlueToothDialOut.h"
#include "DlgDiaIn.h"
#include "..\BlueToothPhone\HBCP.h"
#include "..\BlueToothPhone\Command.h"
#include "DlgSetPin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDial dialog

CDlgSetPinCode::CDlgSetPinCode(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgSetPinCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pin_type = TRUE;
	m_strNum = _T("");
	m_nRstState = IDCANCEL;
}


void CDlgSetPinCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDial)
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


BEGIN_MESSAGE_MAP(CDlgSetPinCode, CDialogBase)
	//{{AFX_MSG_MAP(CDlgDial)
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
	ON_BN_CLICKED(IDC_BTN_DiaIn, OnBTNDiaIn)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_STATIC_NUM, OnStaticNum)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_VOL_UP, OnBtnVolUp)
	ON_BN_CLICKED(IDC_BTN_VOL_DOWN, OnBtnVolDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HBCPOFFHOOK, OnBuleToothoffHook)
//	ON_MESSAGE(WM_HBCPCALLIN, OnBlueToothDialIn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDial message handlers


BOOL CDlgSetPinCode::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	OnLanguageChange();
	OnSkinChange();

	InitGdi();
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgSetPinCode::OnLanguageChange()
{
	CResString str;
	if (m_pin_type)
 		str.LoadString(RES_BUTTON_SETPIN);
	else
		str.LoadString(RES_BUTTON_USER_NUMBER);
 	
	m_stTitle = str;
	SetWindowText(str);
	return TRUE;
}

	//变皮肤
BOOL CDlgSetPinCode::OnSkinChange()
{
	return TRUE;
}

void CDlgSetPinCode::OnBtnExit() 
{
	m_ItemNum.chChar = _T("");
	m_nRstState = IDCANCEL;
	OnExit();
}

void CDlgSetPinCode::OnBtn1() 
{
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("1");

}

void CDlgSetPinCode::OnBtn2() 
{
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("2");

}

void CDlgSetPinCode::OnBtn3() 
{
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("3");

}

void CDlgSetPinCode::OnBtn4() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("4");
//	m_StcNum.SetWindowText(str);

}

void CDlgSetPinCode::OnBtn5() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("5");
//	m_StcNum.SetWindowText(str);	


}

void CDlgSetPinCode::OnBtn6() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("6");
//	m_StcNum.SetWindowText(str);

}

void CDlgSetPinCode::OnBtn7() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("7");
//	m_StcNum.SetWindowText(str);
	

}

void CDlgSetPinCode::OnBtn8() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("8");
//	m_StcNum.SetWindowText(str);

}

void CDlgSetPinCode::OnBtn9() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("9");
//	m_StcNum.SetWindowText(str);
	
}

void CDlgSetPinCode::OnBtn10() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);

//	m_ItemNum.chChar += _T("*");
//	m_StcNum.SetWindowText(str);	

}

void CDlgSetPinCode::OnBtn0() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);
	int n = 20;
	if ( m_pin_type )
		n = 3;
	if(m_ItemNum.chChar.GetLength() > n)
		return;
	m_ItemNum.chChar += _T("0");
//	m_StcNum.SetWindowText(str);	
}

void CDlgSetPinCode::OnBtn11() 
{
	// TODO: Add your control notification handler code here
//	CString str;
//	m_StcNum.GetWindowText(str);

//	m_ItemNum.chChar += _T("#");
//	m_StcNum.SetWindowText(str);

	m_nRstState = IDOK;
	if ( m_pin_type )
	{
		if(m_ItemNum.chChar.GetLength() == 0)
			return;
		CSysConfig::Instance().SetBtPin(m_ItemNum.chChar);
		
		char strnum[MAX_PATH];
		wcstombs(strnum, (LPCTSTR)m_ItemNum.chChar, m_ItemNum.chChar.GetLength());
		strnum[m_ItemNum.chChar.GetLength()] = '\0';
		//HBCP_Set_BcName(strlen(strnum), (unsigned char *)strnum);
		
		// 	HBCP_Set_Pin(0, NULL);
		HBCP_Set_Pin(strlen(strnum), (unsigned char *)strnum);
	}else
	{
		m_strNum = m_ItemNum.chChar;
	}
	OnExit();
}


void CDlgSetPinCode::OnBTNDiaOut() 
{

}

void CDlgSetPinCode::OnBtnBack() 
{

	m_ItemNum.chChar.Delete(m_ItemNum.chChar.GetLength()-1,1);


}

void CDlgSetPinCode::OnBTNDiaIn() 
{

}

void CDlgSetPinCode::OnBtnReset() 
{
	//HBCP_HSHF_Get_Status();
}

void CDlgSetPinCode::OnStaticNum() 
{
	// TODO: Add your control notification handler code here
	m_StcNum.SetWindowText(_T(""));
}

LRESULT CDlgSetPinCode::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgSetPinCode::OnTimer(UINT nIDEvent) 
{
	CDialogBase::OnTimer(nIDEvent);
}


LRESULT CDlgSetPinCode::OnBuleToothoffHook(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CDlgSetPinCode::OnBtnVolUp() 
{
}

void CDlgSetPinCode::OnBtnVolDown() 
{
}
//
//LRESULT CDlgSetPinCode::OnBlueToothDialIn(WPARAM, LPARAM)
//{
//	return TRUE;
// }

void CDlgSetPinCode::OnExit()
{
//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

//	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
//	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(m_nRstState);
}


void CDlgSetPinCode::InitGdi()
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

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_PHONE_NUM_INPUT_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BG_PHONE_NUM_INPUT_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;    

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgSetPinCode::InitControl()
{
	int i;
	for(i=0; i<BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	//OK button.
	m_Item[0].rect.left		= 570;
	m_Item[0].rect.top		= 287;
	m_Item[0].rect.right	= 682;
	m_Item[0].rect.bottom	= 382;
	//1
	m_Item[1].rect.left		= 5;
	m_Item[1].rect.top		= 287;
	m_Item[1].rect.right	= 117;
	m_Item[1].rect.bottom	= 382;
	//2
	m_Item[2].rect.left		= 118;
	m_Item[2].rect.top		= 287;
	m_Item[2].rect.right	= 230;
	m_Item[2].rect.bottom	= 382;
	//3
	m_Item[3].rect.left		= 231;
	m_Item[3].rect.top		= 287;
	m_Item[3].rect.right	= 343;
	m_Item[3].rect.bottom	= 382;
	//4
	m_Item[4].rect.left		= 344;
	m_Item[4].rect.top		= 287;
	m_Item[4].rect.right	= 456;
	m_Item[4].rect.bottom	= 382;
	//5
	m_Item[5].rect.left		= 457;
	m_Item[5].rect.top		= 287;
	m_Item[5].rect.right	= 569;
	m_Item[5].rect.bottom	= 382;
	//6
	m_Item[6].rect.left		= 5;
	m_Item[6].rect.top		= 383;
	m_Item[6].rect.right	= 117;
	m_Item[6].rect.bottom	= 480;
	//7
	m_Item[7].rect.left		= 118;
	m_Item[7].rect.top		= 383;
	m_Item[7].rect.right	= 230;
	m_Item[7].rect.bottom	= 480;
	//8
	m_Item[8].rect.left		= 231;
	m_Item[8].rect.top		= 383;
	m_Item[8].rect.right	= 343;
	m_Item[8].rect.bottom	= 480;
	//9
	m_Item[9].rect.left		= 344;
	m_Item[9].rect.top		= 383;
	m_Item[9].rect.right	= 456;
	m_Item[9].rect.bottom	= 480;
	//0
	m_Item[10].rect.left	= 457;
	m_Item[10].rect.top		= 383;
	m_Item[10].rect.right	= 569;
	m_Item[10].rect.bottom	= 480;
	//*
	m_Item[11].rect.left	= 0;
	m_Item[11].rect.top		= 0;
	m_Item[11].rect.right	= 0;
	m_Item[11].rect.bottom	= 0;
	//从电话簿选择号码
	m_Item[12].rect.left	= 683;
	m_Item[12].rect.top		= 287;
	m_Item[12].rect.right	= 795;
	m_Item[12].rect.bottom	= 382;
	//←
	m_Item[13].rect.left	= 570;
	m_Item[13].rect.top		= 383;
	m_Item[13].rect.right	= 795;
	m_Item[13].rect.bottom	= 480;
	
	//Edit area. PWD.
	m_Item[14].rect.left   = 243;
	m_Item[14].rect.top    = 202;
	m_Item[14].rect.right  = 568;
	m_Item[14].rect.bottom = 255;

	//Exit.
	m_Item[15].rect.left	= 720;
	m_Item[15].rect.top		= 0;
	m_Item[15].rect.right	= 800;
	m_Item[15].rect.bottom	= 60;
}


void  CDlgSetPinCode::LoadPookBook()
{
	CPBookMain dlg;
	dlg.m_IsGetItem = TRUE;
	if ( dlg.DoModal()== IDOK )
		m_ItemNum.chChar = dlg.m_strSel;
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CDlgSetPinCode::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgSetPinCode::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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


void CDlgSetPinCode::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<BTN_COUNT;i++)
	{
		//绘制背景
			str = m_Item[i].chChar;
			Rect = m_Item[i].rect;
			state = m_Item[i].nState;
			DrawItems(&memDC,Rect,state);
//			Rect.top = Rect.top + 54;
//			DrawBtnText(&memDC,Rect,str,FALSE);
	}

	DrawBtnText(&memDC,g_retTitile,m_stTitle,bigfont,RGB(255,255,255),DT_LEFT|DT_VCENTER);

	Rect = m_Item[14].rect;
	DrawBtnText(&memDC,Rect,m_ItemNum.chChar,bigfont,RGB(255,255,255),DT_RIGHT | DT_TOP);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CDlgSetPinCode::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<BTN_COUNT;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
				if(m_Item[i].nState == BTN_STATE_NORMAL)
				{
					m_Item[i].nState = BTN_STATE_DOWN;
				}

				if( i == 0)
				{
					PlayKeySound();
				}
				else if( i == 1) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\1.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\1.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 2) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\2.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\2.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 3) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\3.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\3.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 4) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\4.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\4.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 5) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\5.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\5.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 6) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\6.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\6.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 7)
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\7.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\7.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 8) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\8.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\8.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 9 )
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\9.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\9.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 10) 
					if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\0.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
					else
						::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\0.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
						//PlayKeySound();
				else if( i == 11) 
				//	if( ID_CHINESE == CSysConfig::Instance().GetLanguage() )
				//		::PlaySound(_T("\\Flashdrv Storage\\menu\\Style\\VoiceKey\\well.wav"), NULL, SND_ASYNC | SND_NODEFAULT );
				//	else
						PlayKeySound();
				else if(  11 <= i < BTN_COUNT) 
				{
					PlayKeySound();
				}

		}
	}
	
	Invalidate();
	SetCapture();

	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgSetPinCode::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<BTN_COUNT;i++)
	{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			
				switch( i )
					{
 					case 0:
 						OnBtn11();
 						break;
 					case 1:
 						OnBtn1();
 						break;
 					case 2:
 						OnBtn2();
 						break;
 					case 3:
 						OnBtn3();
 						break;
 					case 4:
 						OnBtn4();
 						break;
 					case 5:
 						OnBtn5();
 						break;
 					case 6:
 						OnBtn6();
 						break;
					case 7:
						OnBtn7();
						break;
 					case 8:
 						OnBtn8();
 						break;
 					case 9:
 						OnBtn9();
 						break;
 					case 10:
 						OnBtn0();
 						break;
					case 12:
						LoadPookBook();
						break;
					case 13:
						OnBtnBack();
						break;
					case 15:
						OnBtnExit();
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

