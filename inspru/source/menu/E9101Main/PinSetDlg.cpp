// PinSetDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "PinSetDlg.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog

CPinSetDlg::CPinSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CPinSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPinSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPinSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPinSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPinSetDlg, CDialog)
	//{{AFX_MSG_MAP(CPinSetDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg message handlers

BOOL CPinSetDlg::OnInitDialog()
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
	enOBJ = DIALDLG;

	//m_strCenterNum	  = CSysConfig::Instance().GetCenterNum();
	//m_strEmergencyNum = CSysConfig::Instance().GetEmergencyNum();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPinSetDlg::InitGdi()
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

void CPinSetDlg::InitControl()
{
	int i;
	for(i=0; i<19; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}
	//OK button.
	m_item[0].rect.left   = 600;
	m_item[0].rect.top    = 316;
	m_item[0].rect.right  = 711;
	m_item[0].rect.bottom = 387;

	//Input.
	for(i=1; i<=13; i++)
	{
		m_item[i].rect.left   = 60 * (i-1);
		m_item[i].rect.top    = 418;
		m_item[i].rect.right  = 60 * i;
		m_item[i].rect.bottom = 480;
	}

	//Edit area. Center Num.
	m_item[14].rect.left   = 150;
	m_item[14].rect.top    = 138;
	m_item[14].rect.right  = 474;
	m_item[14].rect.bottom = 195;

	//Edit area. Emergency Num.
	m_item[15].rect.left   = 150;
	m_item[15].rect.top    = 209;
	m_item[15].rect.right  = 474;
	m_item[15].rect.bottom = 269;

	//Edit area. Local Num.
	m_item[16].rect.left   = 150;
	m_item[16].rect.top    = 281;
	m_item[16].rect.right  = 474;
	m_item[16].rect.bottom = 341;

	//PWD Setting.
	m_item[17].rect.left   = 600;
	m_item[17].rect.top    = 196;
	m_item[17].rect.right  = 730;
	m_item[17].rect.bottom = 255;

	//Exit.
	m_item[18].rect.left   = 740;
	m_item[18].rect.top    = 0;
	m_item[18].rect.right  = 800;
	m_item[18].rect.bottom = 60;

	m_item[14].nState = BTN_STATE_DOWN;
}

void CPinSetDlg::OnExit()
{
	//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	//	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	//bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
	
	CDialog::EndDialog(0);
}

void CPinSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CPinSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CPinSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
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
	rcItem.left = 45;
	strRes.LoadString(RES_BUTTON_CENTER);

	if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
		str = TEXT("old pwd:");
	else
		str = TEXT("旧密码：");

	
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[14].rect.left + 7;	
	m_strPWDShow = TEXT("");
	for(i=0; i<m_strOldPWDInput.GetLength(); i++)
	{
		m_strPWDShow += TEXT("*");
	}
	DrawBtnText( &memDC, rcItem, m_strPWDShow, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	rcItem = m_item[15].rect;
	rcItem.left = 45;
	strRes.LoadString(RES_BUTTON_HOSPITAL);
	if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
		str = TEXT("new pwd");
	else
		str = TEXT("新密码：");

	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[15].rect.left + 7;
	m_strPWDShow = TEXT("");
	for(i=0; i<m_strNewPWD.GetLength(); i++)
	{
		m_strPWDShow += TEXT("*");
	}
	DrawBtnText( &memDC, rcItem, m_strPWDShow, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	rcItem = m_item[16].rect;
	rcItem.left = 45;
	strRes.LoadString(RES_BUTTON_FIRE);

	if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
		str = TEXT("Confirm:");
	else
		str = TEXT("确认密码：");


	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[16].rect.left + 7;
	m_strPWDShow = TEXT("");
	for(i=0; i<m_strConfirmPWD.GetLength(); i++)
	{
		m_strPWDShow += TEXT("*");
	}
	DrawBtnText( &memDC, rcItem, m_strPWDShow, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	rcItem = m_item[17].rect;
	if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
		str = TEXT("Other Set");
	else
		str = TEXT("其他设置");
	
	DrawBtnText( &memDC, rcItem, str, smallfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);

	// Do not call CDialog::OnPaint() for painting messages

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CPinSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	BOOL bRc = false;
	for(i=14; i<17; i++)
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

void CPinSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
 
	for(i=14; i<17; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			//m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 14:	enSel = OLDPWD;		break;
			case 15:	enSel = NEWPWD;		break;
			case 16:	enSel = CFMPWD;		break;
			default:						break;
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

	//密码设置
	if(m_item[17].nState == BTN_STATE_DOWN )
	{
		m_item[17].nState = BTN_STATE_NORMAL;
		ChangeObject();
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

void CPinSetDlg::ChangeObject()
{
	
}

void CPinSetDlg::OnInput(int number)
{
	switch(enSel)
	{
	case OLDPWD:
		{
			if(number == 11)
			{
				m_strOldPWDInput += ",";
			}
			else if(number == 12)
			{
				m_strOldPWDInput += ".";
			}
			else if(number == 13)
			{
				if(m_strOldPWDInput.GetLength() > 0)
				{
					m_strOldPWDInput.Delete(m_strOldPWDInput.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strOldPWDInput += chN;
			}
			break;
		}
	case NEWPWD:
		{
			if(number == 11)
			{
				m_strNewPWD += ",";
			}
			else if(number == 12)
			{
				m_strNewPWD += ".";
			}
			else if(number == 13)
			{
				if(m_strNewPWD.GetLength() > 0)
				{
					m_strNewPWD.Delete(m_strNewPWD.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strNewPWD += chN;
			}
			break;
		}
	case CFMPWD:
		{
			if(number == 11)
			{
				m_strConfirmPWD += ",";
			}
			else if(number == 12)
			{
				m_strConfirmPWD += ".";
			}
			else if(number == 13)
			{
				if(m_strConfirmPWD.GetLength() > 0)
				{
					m_strConfirmPWD.Delete(m_strConfirmPWD.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strConfirmPWD += chN;
			}
			break;
		}
	default:
		break;
	}

	Invalidate();
}

void CPinSetDlg::OnSave()
{
	if(!CheckPWD())
		return;

	switch(enOBJ)
	{
	case IPSETDLG:
		m_IniFile.SetString(_T("PWD_IPSet"),_T("PWD"), m_strNewPWD);
		break;
	case DIALDLG:
		m_IniFile.SetString(_T("PWD_Dial"),_T("PWD"), m_strNewPWD);
		break;
	}

	OnExit();
}

bool CPinSetDlg::CheckPWD()
{
	if( !m_IniFile.Load( _T("\\Flashdrv Storage\\menu\\GSM.dll") ) )
		return false;
	
	switch(enOBJ)
	{
	case IPSETDLG:
		m_IniFile.GetString(_T("PWD_IPSet"),_T("PWD"), m_strOldPWD);
		break;
	case DIALDLG:
		m_IniFile.GetString(_T("PWD_Dial"),_T("PWD"), m_strOldPWD);
		break;
	}
		
	if(m_strOldPWD != m_strOldPWDInput)
	{
		if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
			MessageBox(_T("Password Error!"), _T("WARNING"), MB_OK);
		else
			MessageBox(_T("密码输入错误!"), _T("WARNING"), MB_OK);


		m_strOldPWDInput = TEXT("");
		m_item[14].nState = BTN_STATE_DOWN;
		enSel = OLDPWD;
		return false;
	}

	if(m_strNewPWD != m_strConfirmPWD)
	{
		if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
			MessageBox(_T("The two PWD is not same!"), _T("WARNING"), MB_OK);
		else
			MessageBox(_T("密码输入不一致，请重新输入!"), _T("WARNING"), MB_OK);


		m_strNewPWD		= TEXT("");
		m_strConfirmPWD	= TEXT("");
		m_item[15].nState = BTN_STATE_DOWN;
		enSel = NEWPWD;
		return false;
	}

	return true;
}

BOOL CPinSetDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}