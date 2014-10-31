// MediaSMS.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "SMSMain.h"
#include "SMSInbox.h"
#include "SMSOutbox.h"
#include "SMSDraft.h"
#include "SMSSending.h"
#include "..\imm\IMMMng.h"
#include "DlgSetPin.h"
#include "SMSSending.h"
#include "../BrightnessSetDlg.h"

#include "../MutiLanguage/CGDICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CString g_csPicName;
extern CString g_csPicIconName;
extern CString g_csMusicName;
extern CString g_csMusicIconName;
extern CString g_csMovieName;
extern CString g_csMovieIconName;
extern CString g_csTxtName;
extern CString g_csTxtIconName;
extern BOOL g_bLight;

CSMSMain::CSMSMain(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSMSMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSMSMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSMain, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CSMSMain::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//变换语言和单位
BOOL CSMSMain::OnLanguageChange()//变皮肤
{
	CResString str;
	
	str.LoadString(RES_BUTTON_SMS);
	m_csTitile = str;
	
	str.LoadString(RES_BUTTON_SMS_NEW);
	m_Item[0].chChar = str;
	
	str.LoadString(RES_BUTTON_SMS_INBOX);
	m_Item[1].chChar = str;
	
	str.LoadString(RES_BUTTON_SMS_OUTOBX);
	m_Item[2].chChar = str;
	
	//str.LoadString(RES_BUTTON_SMS_DRAFT);
	str.LoadString(RES_BUTTON_DISPATCH);
	m_Item[3].chChar = _T("草稿箱");

	m_Item[4].chChar = _T("");

	return TRUE;
}

BOOL CSMSMain::OnSkinChange()
{
	return TRUE;
}


void CSMSMain::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//
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
	
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_SMS_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(SKIN_BG_SMS_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CSMSMain::ReleaseGdi()
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

void CSMSMain::OnExit()
{
	ReleaseGdi();
	//g_GsmLogic.m_inbox.save_data( SMS_PATH_INBOX );
	//g_GsmLogic.m_outbox.save_data( SMS_PATH_OUTBOX );
	//g_GsmLogic.m_draft.save_data( SMS_PATH_DRAFT );
	CDialogBase::EndDialog(0);
}


void CSMSMain::InitControl()
{
 	for (int i =0;  i < 5; i++)
 	{
 		m_Item[i].nState = BTN_STATE_NORMAL;
 	}
	m_Item[0].rect.left		= 124;
	m_Item[0].rect.top		= 150;
	m_Item[0].rect.right	= 250;
	m_Item[0].rect.bottom	= 275;

	m_Item[1].rect.left		= 266;
	m_Item[1].rect.top		= 150;
	m_Item[1].rect.right	= 393;
	m_Item[1].rect.bottom	= 275;

	m_Item[2].rect.left		= 408;
	m_Item[2].rect.top		= 150;
	m_Item[2].rect.right	= 536;
	m_Item[2].rect.bottom	= 275;

	m_Item[3].rect.left		= 550;
	m_Item[3].rect.top		= 150;
	m_Item[3].rect.right	= 677;
	m_Item[3].rect.bottom	= 275;

	m_Item[4].rect.top = 0;
	m_Item[4].rect.left = 740;
	m_Item[4].rect.bottom = 60;
	m_Item[4].rect.right = 800;

}

void CSMSMain::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CSMSMain::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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



void CSMSMain::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<5;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.bottom += 30;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_CENTER | DT_BOTTOM);
	}
	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CSMSMain::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<5;i++)
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
	SetCapture();
	
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CSMSMain::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<5;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:	OnBtnNewSMS();break;
			case 1:	OnBtnInBox();break; //收件箱
			case 2:	OnBtnOutBox();break;//发件箱
			case 3: OnBtnDraft();break; //草稿箱
			case 4: EndDialog(0);break;
			default:
				break;
			}
		}
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
BOOL  CSMSMain::LoadSMSContext(CString& strContext)
{
	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	
	ImmReturn ret;
	IMMRESULT res;
	enSkinType enSkin = ID_SKIN2; //CSysConfig::Instance().GetSkinType();
	if(CIMMMng::Instance().Load() == TRUE)
	{
		CIMMMng::Instance().ImmSetKeySound( g_enKeySnd );
		CIMMMng::Instance().IMMInit(FourPoint3);
		if(CSysConfig::Instance().GetLanguage() == ID_ENGLISH)
			ret = CIMMMng::Instance().IMMCreateMessage(ID_ENGLISH);
		else
			ret = CIMMMng::Instance().IMMCreateMessage(ID_CHINESE);
	}else
	{
		MessageBox( _T("Load IMM has Failed!\n"), _T("System Tip"), MB_OK );
		return FALSE;
	}
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();

	res = CIMMMng::Instance().IMMGetResult();
	res.strName = res.strName.Left(70);
	//14 确认
	if( ret == 14 )
	{
		if( res.strName.GetLength() > 0 )
		{
			strContext = res.strName;
			return TRUE;
		}
	}
	//15 save
	else if ( ret == 15 )
	{
		if( res.strName.GetLength() > 0 )
		{
			g_GsmLogic.UserSaveMsg( res.strName );
			g_GsmLogic.m_draft.save_data( SMS_PATH_DRAFT );
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL  CSMSMain::LoadUserPhone(CString& strPhone)
{
	int value;
	CDlgSetPinCode dlg;
	dlg.SetType(FALSE);
	value = dlg.DoModal();
	if ( value == IDOK && dlg.m_ItemNum.chChar.GetLength() > 0 )
	{
		strPhone = dlg.m_ItemNum.chChar;
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////

void  CSMSMain::OnBtnNewSMS()
{
	CString strSMS;
	if ( LoadSMSContext(strSMS) )
	{
		CString strPhone;
		if ( LoadUserPhone(strPhone) )
		{
			CSMSSending Smsdlg;
			Smsdlg.SetCallPhone(strPhone);
			Smsdlg.SetDestoryHwnd(NULL);
			Smsdlg.SetMsgParam( strPhone, strSMS, TRUE );
			Smsdlg.DoModal();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void  CSMSMain::OnBtnInBox()
{
	CSMSInbox dlg;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////

void  CSMSMain::OnBtnOutBox()
{
	CSMSOutbox dlg;
	dlg.DoModal();
}

void  CSMSMain::OnBtnDraft()
{
	CSMSDraft dlg;
 	dlg.DoModal();
}