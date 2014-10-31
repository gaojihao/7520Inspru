#include "stdafx.h"
#include "e9101main.h"
#include "DlgRFIDCheck.h"
#include "../../imm/IMMMng.h"
#include "../../MutiLanguage/CGDICommon.h"
#include "../DataType.h"
#include "../../ConfigFile/CIniFile.h"


CDlgRFIDCheck::CDlgRFIDCheck(BOOL bIsModelDlg /*= TRUE*/, CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgRFIDCheck::IDD, pParent)
{
	m_bIsModelDlg	= bIsModelDlg;	//当前窗口是否为模态窗口
}

void CDlgRFIDCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgRFIDCheck, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgRFIDCheck::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	return TRUE;
}
//变换语言和单位
BOOL CDlgRFIDCheck::OnLanguageChange()//变皮肤
{
	CResString str;
	m_csTitile = _T("车辆注册");
	return TRUE;
}

BOOL CDlgRFIDCheck::OnSkinChange()
{
	return TRUE;
}

void CDlgRFIDCheck::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	if( CSysConfig::Instance().GetLanguage() != ID_CHINESE &&  CSysConfig::Instance().GetLanguage() != ID_ENGLISH )
	{
		smallfont.CreateFont(
			-12,					   // nHeight
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
			ANTIALIASED_QUALITY,       // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体\0")); 
	}
	else
	{
		smallfont.CreateFont(
			-18,					   // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_SEMIBOLD,               // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			ANTIALIASED_QUALITY,       // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体\0"));             // lpszFacename
	}
	
	bigfont.CreateFont(
		32,					       // nHeight
		11,                        // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,               // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("黑体\0"));
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_RFID_CHECK_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_RFID_CHECK_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgRFIDCheck::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CDlgRFIDCheck::OnExit()
{
	ReleaseGdi();
	if(m_bIsModelDlg)
		CDialogBase::EndDialog(0);		//模态对话框
	else
		CDialogBase::DestroyWindow();	//非模态对话框
}

void CDlgRFIDCheck::InitControl()
{
	for (int i = 6; i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	m_Item[0].rect.left     = 33;
	m_Item[0].rect.top      = 138;
	m_Item[0].rect.right    = 145;
	m_Item[0].rect.bottom   = 178;
	m_Item[0].chChar        = _T("SIM卡号");

	m_Item[1].rect.left     = 33;
	m_Item[1].rect.top      = 188;
	m_Item[1].rect.right    = 145;
	m_Item[1].rect.bottom   = 228;
	m_Item[1].chChar        = _T("终端ID");

	m_Item[2].rect.left     = 33;
	m_Item[2].rect.top      = 238;
	m_Item[2].rect.right    = 145;
	m_Item[2].rect.bottom   = 278;
	m_Item[2].chChar        = _T("车牌号");
	/////////////////////////////////////////////

	m_Item[3].rect.left     = 424;
	m_Item[3].rect.top      = 138;
	m_Item[3].rect.right    = 560;
	m_Item[3].rect.bottom   = 178;
	m_Item[3].chChar        = _T("车牌颜色");

	m_Item[4].rect.left     = 424;
	m_Item[4].rect.top      = 188;
	m_Item[4].rect.right    = 560;
	m_Item[4].rect.bottom   = 228;
	m_Item[4].chChar        = _T("省域");

	m_Item[5].rect.left     = 424;
	m_Item[5].rect.top      = 238;
	m_Item[5].rect.right    = 560;
	m_Item[5].rect.bottom   = 278;
	m_Item[5].chChar        = _T("市域");
	///////////////////////////////////////////////
	//按钮
	m_Item[6].rect.left     = 150;
	m_Item[6].rect.top      = 138;
	m_Item[6].rect.right    = 375;
	m_Item[6].rect.bottom   = 178;
	m_Item[6].chChar        = _T("");

	
	m_Item[7].rect.left     = 150;
	m_Item[7].rect.top      = 188;
	m_Item[7].rect.right    = 375;
	m_Item[7].rect.bottom   = 228;
	m_Item[7].chChar        = _T("");

	m_Item[8].rect.left     = 150;
	m_Item[8].rect.top      = 238;
	m_Item[8].rect.right    = 375;
	m_Item[8].rect.bottom   = 278;
	m_Item[8].chChar        = _T("");
	//////////////////////////////////////////////

	m_Item[9].rect.left     = 565;
	m_Item[9].rect.top      = 138;
	m_Item[9].rect.right    = 765;
	m_Item[9].rect.bottom   = 178;
	m_Item[9].chChar        = _T("");

	m_Item[10].rect.left     = 565;
	m_Item[10].rect.top      = 188;
	m_Item[10].rect.right    = 765;
	m_Item[10].rect.bottom   = 228;
	m_Item[10].chChar        = _T("");

	m_Item[11].rect.left     = 565;
	m_Item[11].rect.top      = 238;
	m_Item[11].rect.right    = 765;
	m_Item[11].rect.bottom   = 278;
	m_Item[11].chChar        = _T("");
////////////////////////////////////////////////////////////
	//开户
	m_Item[12].rect.left		= 639;
	m_Item[12].rect.top			= 396;
	m_Item[12].rect.right		= 782;
	m_Item[12].rect.bottom		= 475;
	m_Item[12].chChar            = _T("注册");
}

void CDlgRFIDCheck::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CDlgRFIDCheck::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
{
	COLORREF  nOldRgb;
	nOldRgb = pDC->SetTextColor( col );
	int      nOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont* def_font = pDC->SelectObject(&font);

	pDC->DrawText(str, &rt, Format );

	pDC->SelectObject(def_font);
	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(nOldRgb);
}

void CDlgRFIDCheck::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	int i=0;
	
	//绘制按钮状态和文字
	for(i=0;i<BTN_COUNT;i++)
	{
		str		= m_Item[i].chChar;
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}

	//标题
	DrawBtnText(&memDC, g_retTitile, m_csTitile, CGDICommon::Instance()->bigbigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgRFIDCheck::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=6;i<BTN_COUNT;i++)
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
	
	Invalidate();
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgRFIDCheck::OnLButtonUp(UINT nFlags, CPoint point)
{
	for(int i=6;i<BTN_COUNT;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;

			switch( i )
			{
			case 6:
				OnButtonInput(6);
				break;
			case 7:
				OnButtonInput(7);
				break;
			case 8:
				OnButtonInput(8);
				break;
			case 9:
				OnButtonInput(9);
				break;
			case 10:
				OnButtonInput(10);
				break;
			case 11:
				OnButtonInput(11);
				break;
			case 12://开户
				OnRegister();
				break;
			//case 13://退出
				//OnExit();
			//	break;
			default:
				break;
			}
		}
	}	
	//Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CDlgRFIDCheck::OnTimer(UINT nIDEvent) 
{
	CDialogBase::OnTimer(nIDEvent);
}

void CDlgRFIDCheck::OnButtonInput(int inum)
{
	ImmReturn ret;
	IMMRESULT res;
	CString strName;
	strName = _T("");


	enSkinType enSkin = ID_SKIN2;
	if(CIMMMng::Instance().Load() == TRUE)
	{
		CIMMMng::Instance().ImmSetKeySound( g_enKeySnd );
		CIMMMng::Instance().IMMInit(FourPoint3);
		ret = CIMMMng::Instance().IMMCreateMessage(ID_CHINESE);
	}
	res = CIMMMng::Instance().IMMGetResult();


	if(res.strName == _T(""))
	{
	}
	else
	{
		m_Item[inum].chChar =  res.strName;
	}
	Invalidate();
}

void CDlgRFIDCheck::OnRegister()
{
	CIniFile inifile;

	if( !inifile.Load( PATH_CONFIG_INI ) )
	{
		AfxMessageBox(_T("加载config.ini文件失败！"));
		return;
	}


	inifile.SetString(_T("Set"), _T("PhoneNum"), m_Item[6].chChar);
	inifile.SetString(_T("Set"), _T("TerminalID"), m_Item[7].chChar);
	inifile.SetString(_T("TerminalParam"), _T("VEHICLE_ID"), m_Item[8].chChar);
	inifile.SetString(_T("TerminalParam"), _T("PLATE_COLOR"), m_Item[9].chChar);
	inifile.SetString(_T("TerminalParam"), _T("VEHICLE_PROVINCE_ID"), m_Item[10].chChar);
	inifile.SetString(_T("TerminalParam"), _T("VEHICLE_CITY_ID"), m_Item[11].chChar);

	inifile.Flush();

	Sleep(1000);

	OnExit();
}


LRESULT CDlgRFIDCheck::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}

void CDlgRFIDCheck::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogBase::PostNcDestroy();

	if( !m_bIsModelDlg )	//非模态对话框下才能删除对话框对象
		delete this;
}
