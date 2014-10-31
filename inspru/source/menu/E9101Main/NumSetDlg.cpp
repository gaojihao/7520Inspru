// NumSetDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "NumSetDlg.h"
#include "PinSetDlg.h"

#include "../MutiLanguage/CGDICommon.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumSetDlg dialog

CNumSetDlg::CNumSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CNumSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNumSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNumSetDlg, CDialog)
	//{{AFX_MSG_MAP(CNumSetDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumSetDlg message handlers

BOOL CNumSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	MoveWindow(0,0,800,480);
	InitGdi();
	InitControl();

	enSel = NOTHING;

	m_strCenterNum		= CSysConfig::Instance().GetCenterNum();
	m_strHospitalNum	= CSysConfig::Instance().GetHospitalNum();
	m_strFireNum		= CSysConfig::Instance().GetFireNum();
	m_strAlarmNum		= CSysConfig::Instance().GetAlarmNum();
	
//	if( m_IniFile.Load( "\\Flashdrv Storage\\menu\\GPRS.ini" ) )
//	{
//		m_IniFile.GetString(_T("Server"),_T("SIMNo"),m_strLocalNum);
// 	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNumSetDlg::InitGdi()
{
	CRect rc;
	m_pDC=GetDC();
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//
  		smallfont.CreateFont(
			-18,					    // nHeight
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
			_T("宋体\0"));             // lpszFacename
		
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
		
		bk_normal_dc.CreateCompatibleDC(m_pDC);
		bk_press_dc.CreateCompatibleDC(m_pDC);
		
		
		PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_INPUT4_N,true);
		m_stBtnNormalBMP   = pSkin->hBitmap;
		
		pSkin = g_pResMng->RequestBmp(BG_INPUT4_P, true);
		m_stBtnPressBMP = pSkin->hBitmap;
		
		
		m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
		m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
		
		ReleaseDC(m_pDC);
}

void CNumSetDlg::InitControl()
{
	int i;
	for(i=0; i<19; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}

	//OK button.
	m_item[0].rect.left   = 574;
	m_item[0].rect.top    = 214;
	m_item[0].rect.right  = 715;
	m_item[0].rect.bottom = 275;

	//Input.
	//1
	m_item[1].rect.left		= 5;
	m_item[1].rect.top		= 287;
	m_item[1].rect.right	= 117;
	m_item[1].rect.bottom	= 382;
	//2
	m_item[2].rect.left		= 118;
	m_item[2].rect.top		= 287;
	m_item[2].rect.right	= 230;
	m_item[2].rect.bottom	= 382;
	//3
	m_item[3].rect.left		= 231;
	m_item[3].rect.top		= 287;
	m_item[3].rect.right	= 343;
	m_item[3].rect.bottom	= 382;
	//4
	m_item[4].rect.left		= 344;
	m_item[4].rect.top		= 287;
	m_item[4].rect.right	= 456;
	m_item[4].rect.bottom	= 382;
	//5
	m_item[5].rect.left		= 457;
	m_item[5].rect.top		= 287;
	m_item[5].rect.right	= 569;
	m_item[5].rect.bottom	= 382;
	//6
	m_item[6].rect.left		= 5;
	m_item[6].rect.top		= 383;
	m_item[6].rect.right	= 117;
	m_item[6].rect.bottom	= 480;
	//7
	m_item[7].rect.left		= 118;
	m_item[7].rect.top		= 383;
	m_item[7].rect.right	= 230;
	m_item[7].rect.bottom	= 480;
	//8
	m_item[8].rect.left		= 231;
	m_item[8].rect.top		= 383;
	m_item[8].rect.right	= 343;
	m_item[8].rect.bottom	= 480;
	//9
	m_item[9].rect.left		= 344;
	m_item[9].rect.top		= 383;
	m_item[9].rect.right	= 456;
	m_item[9].rect.bottom	= 480;
	//0
	m_item[10].rect.left	= 457;
	m_item[10].rect.top		= 383;
	m_item[10].rect.right	= 569;
	m_item[10].rect.bottom	= 480;
	//*
	m_item[11].rect.left	= 570;
	m_item[11].rect.top		= 287;
	m_item[11].rect.right	= 682;
	m_item[11].rect.bottom	= 382;
	//.
	m_item[12].rect.left	= 683;
	m_item[12].rect.top		= 287;
	m_item[12].rect.right	= 795;
	m_item[12].rect.bottom	= 382;
	//←
	m_item[13].rect.left	= 570;
	m_item[13].rect.top		= 383;
	m_item[13].rect.right	= 795;
	m_item[13].rect.bottom	= 480;
	
	//Edit area. Center Num.
	m_item[14].rect.left   = 283;
	m_item[14].rect.top    = 81;
	m_item[14].rect.right  = 548;
	m_item[14].rect.bottom = 127;

	//Edit area. Hospital Num.
	m_item[15].rect.left   = 283; 
	m_item[15].rect.top    = 130;
	m_item[15].rect.right  = 548;
	m_item[15].rect.bottom = 176;

	//Edit area. Fire Num.
	m_item[16].rect.left   = 283;
	m_item[16].rect.top    = 179;
	m_item[16].rect.right  = 548;
	m_item[16].rect.bottom = 225;

	//Edit area. Alarm Num.
	m_item[17].rect.left   = 283;
	m_item[17].rect.top    = 228;
	m_item[17].rect.right  = 548;
	m_item[17].rect.bottom = 274;

	//Exit.
	m_item[18].rect.left   = 740;
	m_item[18].rect.top    = 0;
	m_item[18].rect.right  = 800;
	m_item[18].rect.bottom = 60;

	m_csTitile = _T("一键通号码设置");
}

void CNumSetDlg::OnExit()
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
	
	CDialog::EndDialog(0);
}

void CNumSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CNumSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CNumSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	int i;
	for(i=0; i<19; i++)
	{
		//绘制背景
		DrawItems(&memDC, m_item[i].rect, m_item[i].nState);
	}

	CString str;
	CResString strRes;
	CRect   rcItem;

	rcItem = m_item[14].rect;
	rcItem.left = 120;
	strRes.LoadString(RES_BUTTON_CENTER);
	str = strRes;
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[14].rect.left + 7;
	DrawBtnText( &memDC, rcItem, m_strCenterNum, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	rcItem = m_item[15].rect;
	rcItem.left = 120;
	strRes.LoadString(RES_BUTTON_HOSPITAL);
	str = strRes;
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[15].rect.left + 7;
	DrawBtnText( &memDC, rcItem, m_strHospitalNum, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	rcItem = m_item[16].rect;
	rcItem.left = 120;
	strRes.LoadString(RES_BUTTON_FIRE);
	str = strRes;
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[16].rect.left + 7;
	DrawBtnText( &memDC, rcItem, m_strFireNum, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	
	rcItem = m_item[17].rect;
	rcItem.left = 120;
	strRes.LoadString(RES_BUTTON_ALARM);
	str = strRes;
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[17].rect.left + 7;
	DrawBtnText( &memDC, rcItem, m_strAlarmNum, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	
	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CNumSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	BOOL bRc = false;
	for(i=14; i<18; i++)
	{
		for(int j=1; j<14; j++)
		{
			bRc = bRc || PtInRect(&m_item[j].rect,point);
		}
		if(!bRc)
		{
			if(m_item[i].nState == BTN_STATE_DOWN)
			{
				m_item[i].nState = BTN_STATE_NORMAL;
				enSel = NOTHING;	//Unselect.
			}
		}
	}
	for( i = 0; i < 19; i++ )
	{
		if(PtInRect(&m_item[i].rect,point))
		{
			if(m_item[i].nState == BTN_STATE_NORMAL)
			{
				m_item[i].nState = BTN_STATE_DOWN;
				//::PlaySound(SOUND_KEYCLICK, NULL, SND_ASYNC | SND_NODEFAULT );
				PlayKeySound();
			}
		}
	}
	
	Invalidate();
	//SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}

void CNumSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
 
	for(i=14; i<18; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			//m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 14:	enSel = CENTERNUM;		break;
			case 15:	enSel = HOSPITALNUM;	break;
			case 16:	enSel = FIRENUM;		break;
			case 17:	enSel = ALARMNUM;		break;
			default:							break;
			}
		}
	}

	for(i=0; i<14; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			m_item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:		OnSave();		break;	//OK
			case 1:		OnInput(1);		break;
			case 2:		OnInput(2);		break;
			case 3:		OnInput(3);		break;
			case 4:		OnInput(4);		break;
			case 5:		OnInput(5);		break;
			case 6:		OnInput(6);		break;
			case 7:		OnInput(7);		break;
			case 8:		OnInput(8);		break;
			case 9:		OnInput(9);		break;
			case 10:	OnInput(0);		break;
			case 11:	OnInput(11);	break;
			case 12:	OnInput(12);	break;
			case 13:	OnInput(13);	break;
			default:					break;
			}
		}
	}

	//退出
	if(m_item[18].nState == BTN_STATE_DOWN )
	{
		m_item[18].nState = BTN_STATE_NORMAL;
		OnExit();
	}
	
	Invalidate();
	CDialog::OnLButtonUp(nFlags, point);
}

void CNumSetDlg::PWDSetting()
{
}

void CNumSetDlg::OnInput(int number)
{
	switch(enSel)
	{
	case CENTERNUM:
		{
			if(number == 11)
			{
				m_strCenterNum += "*";
			}
			else if(number == 12)
			{
				m_strCenterNum += ".";
			}
			else if(number == 13)
			{
				if(m_strCenterNum.GetLength() > 0)
				{
					m_strCenterNum.Delete(m_strCenterNum.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strCenterNum += chN;
			}
			break;
		}
	case HOSPITALNUM:
		{
			if(number == 11)
			{
				m_strHospitalNum += ",";
			}
			else if(number == 12)
			{
				m_strHospitalNum += ".";
			}
			else if(number == 13)
			{
				if(m_strHospitalNum.GetLength() > 0)
				{
					m_strHospitalNum.Delete(m_strHospitalNum.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strHospitalNum += chN;
			}
			break;
		}
	case FIRENUM:
		{
			if(number == 11)
			{
				m_strFireNum += ",";
			}
			else if(number == 12)
			{
				m_strFireNum += ".";
			}
			else if(number == 13)
			{
				if(m_strFireNum.GetLength() > 0)
				{
					m_strFireNum.Delete(m_strFireNum.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strFireNum += chN;
			}
			break;
		}
	case ALARMNUM:
		{
			if(number == 11)
			{
				m_strAlarmNum += ",";
			}
			else if(number == 12)
			{
				m_strAlarmNum += ".";
			}
			else if(number == 13)
			{
				if(m_strAlarmNum.GetLength() > 0)
				{
					m_strAlarmNum.Delete(m_strAlarmNum.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strAlarmNum += chN;
			}
			break;
		}
	default:
		break;
	}

	Invalidate();
}

void CNumSetDlg::OnSave()
{
	CSysConfig::Instance().SetCenterNum(m_strCenterNum);
	CSysConfig::Instance().SetHospitalNum(m_strHospitalNum);
	CSysConfig::Instance().SetFireNum(m_strFireNum);
	CSysConfig::Instance().SetAlarmNum(m_strAlarmNum);

	//m_IniFile.SetString(_T("Server"),_T("SIMNo"),m_strLocalNum);

	OnExit();
}

BOOL CNumSetDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}
