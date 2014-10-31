// SMSInfo.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "../ExTool/ex_basics.h"
#include "DispatchInfo.h"
#include "DlgConfirm.h"
#include "DlgSetPin.h"
#include "SMSSending.h"
#include "..\imm\IMMMng.h"
#include "../DriveLib/tts_api.h"
#include "../BrightnessSetDlg.h"
#include "../DriveLib/midware/DllMidware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSInfo dialog

BOOL CDispatchInfo::m_bISspeak	= FALSE;
BOOL CDispatchInfo::m_bIsOpen	= FALSE;

extern BOOL g_bLight;
CDispatchInfo::CDispatchInfo(st_sms *sms, CWnd* pParent /*=NULL*/)
	: CDialogBase(CDispatchInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psms = sms;
	//m_bISspeak = FALSE;
}


void CDispatchInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispatchInfo, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDispatchInfo::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	m_hdtts = tts_open();
	tts_init(m_hdtts,"\\Flashdrv Storage\\Menu\\stubsvr.dat");
	tts_man_role(m_hdtts,false);
	
	SetWindowText(WND_NAME_DISPATCHINFO);

	UpdateRecord();
	m_bIsOpen	= TRUE;	//窗口已经 打开

	SetTimer(TID_CLOSE_WINDOW, 20*1000, NULL);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//变换语言和单位
BOOL CDispatchInfo::OnLanguageChange()//变皮肤
{
	CResString str;
	//str.LoadString(RES_TITLE_SMS_INFO);
	m_csTitile = _T("调度信息");// = str;
	
	for(int i=0;i<6;i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
		m_Item[i].chChar = _T("");
	}

	m_Item[0].chChar = _T("确定");
	m_Item[1].chChar = _T("放弃");

	return TRUE;
}

BOOL CDispatchInfo::OnSkinChange()
{
	return TRUE;
}


void CDispatchInfo::InitGdi()
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
	bk_disable_dc.CreateCompatibleDC(m_pDC);

	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_DISPATCH_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_BG_DISPATCH_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	pSkin = g_pResMng->RequestBmp(BMP_BG_DISPATCH_P, true);
	m_stBtnDisableBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDispatchInfo::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);

	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bigfont.DeleteObject();
	smallfont.DeleteObject();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CDispatchInfo::OnExit()
{
	m_bIsOpen	= FALSE;
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}


void CDispatchInfo::InitControl()
{
	//确定
	m_Item[0].rect.top    = 224;
	m_Item[0].rect.left   = 144;
	m_Item[0].rect.bottom = 265;
	m_Item[0].rect.right  = 224;
	//m_Item[0].chChar      = "回复";

	//放弃 
	m_Item[1].rect.top    = 224;
	m_Item[1].rect.left   = 229;
	m_Item[1].rect.bottom = 265;
	m_Item[1].rect.right  = 309;
	//m_Item[1].chChar      = "转发";

	//删除2X1：225 Y1：227 X2：301 Y2：264
	m_Item[2].rect.top = 0;
	m_Item[2].rect.left = 0;
	m_Item[2].rect.bottom = 0;
	m_Item[2].rect.right = 0;

	//前一个3X1：308 Y1：227 X2：385 Y2：264
	m_Item[3].rect.top = 0;
	m_Item[3].rect.left = 0;
	m_Item[3].rect.bottom = 0;
	m_Item[3].rect.right = 0;

	//下一个4X1：393 Y1：227 X2：467 Y2：264
	m_Item[4].rect.top = 0;
	m_Item[4].rect.left = 0;
	m_Item[4].rect.bottom = 0;
	m_Item[4].rect.right = 0;

	//Exit
	m_Item[5].rect.top = 0;
	m_Item[5].rect.left = 430;
	m_Item[5].rect.bottom = 40;
	m_Item[5].rect.right = 480;


	//电话号码1X1：51 Y1：45  X2：234 Y2:76   
	m_Static[0].rect.left  = 51;
	m_Static[0].rect.top   = 45;
	m_Static[0].rect.right = 234;
	m_Static[0].rect.bottom = 76;

	//时间 2X1：283 Y1：45 X2：467 Y2：76
	m_Static[1].rect.left  = 283;
	m_Static[1].rect.top   = 45;
	m_Static[1].rect.right = 467;
	m_Static[1].rect.bottom = 76;
	
	//内容X1：11 Y1：81  X2：467 Y2：222 
	m_Static[2].rect.left  = 13;
	m_Static[2].rect.top   = 83;
	m_Static[2].rect.right = 467-2;
	m_Static[2].rect.bottom = 222-2;
	
}

void CDispatchInfo::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDispatchInfo::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDispatchInfo::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	if(!m_bShowReply)
	{
		//m_Item[0].nState = BTN_STATE_DISABLE;
		//m_Item[0].chChar = _T("");
	}

	for(int i=0;i<6;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect, str, smallfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}

	DrawBtnText(&memDC,m_Static[0].rect,m_Static[0].chChar,smallfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	DrawBtnText(&memDC,m_Static[1].rect,m_Static[1].chChar,smallfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	DrawBtnText(&memDC,m_Static[2].rect,m_Static[2].chChar,smallfont,RGB(255,255,255),DT_LEFT|DT_TOP|DT_WORDBREAK|DT_EDITCONTROL);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDispatchInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	for( i = 0; i < 6; i++ )
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

void CDispatchInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<6;i++)
	{
		if(m_Item[i].nState == BTN_STATE_DOWN )
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:	Reply();		break;
			case 1:	SendMsg();		break;
			case 2:	OnBtnDel();		break;
			case 3:	OnBtnPageUp();	break;
			case 4: OnBtnPageDown();break;
			case 5: OnExit();		break;
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

void  CDispatchInfo::OnBtnPageUp()
{
	m_nSelIdx--;
	if ( m_nSelIdx < 0 )
		m_nSelIdx = 0;
	UpdateRecord();
	Invalidate();
}

void  CDispatchInfo::OnBtnPageDown()
{
	m_nSelIdx++;
	int nMaxRd = SMSMaxNum();
	if ( m_nSelIdx > nMaxRd - 1 )
		m_nSelIdx = nMaxRd - 1;
	UpdateRecord();
	Invalidate();
}

void  CDispatchInfo::OnBtnDel()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_SEL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	if( CDlgConfirm::s_bOk )
	{
		if ( m_nSelIdx < m_pData->max_number() )
		{
			m_pData->delte_record( m_pData->max_number() - m_nSelIdx - 1 );
		}else
		{
			g_GsmLogic.GSMDelSIMSMS( m_nSelIdx - m_pData->max_number() );
		}
		OnExit();
	}
}

/////////// ///////////////////////////////////////////////////////////////

void  CDispatchInfo::SendMsg()
{
	OnExit();
	return;
	int nIdx = m_pData->max_number() - m_nSelIdx - 1;
	st_sms* pMsg = m_pData->get_recourd(nIdx);
	if ( pMsg )
	{
		if (  strlen(pMsg->m_context) < 1)
			return;

		int value;
		CDlgSetPinCode dlg;
		dlg.SetType( FALSE );
		value = dlg.DoModal();

		if ( value == IDOK && dlg.m_ItemNum.chChar.GetLength() > 0 )
		{
			CSMSSending Smsdlg;
			TCHAR szBuf[256];
			memset( szBuf, 0, sizeof(szBuf) );
			ex_char2uni(pMsg->m_context, szBuf, sizeof(szBuf)/sizeof(szBuf[0]) );
			CString strName(szBuf);
			Smsdlg.SetMsgParam( dlg.m_ItemNum.chChar, strName, m_bSavedMsg );
			Smsdlg.SetDestoryHwnd(this);
			Smsdlg.DoModal();
		}
	}
}

//tan.c@qq.com

//////////////////////////////////////////////////////////////////////////

//用户选择的当前IDX,但是显示的时候是到序
void  CDispatchInfo::SetLogicData(logic_data* pData, int nIdx,BOOL bSaved /*= FALSE*/ )
{
	m_pData = pData;
	m_nSelIdx = nIdx;
	m_bSavedMsg = bSaved;

	UpdateRecord();
}

void  CDispatchInfo::SetReplyBtn(BOOL bShow)
{
	m_bShowReply = bShow;
}

//////////////////////////////////////////////////////////////////////////

void  CDispatchInfo::UpdateRecord()
{
#if 1
//	if ( m_nSelIdx < m_pData->max_number() )
//	{
//		int nIdx = m_pData->max_number() - m_nSelIdx - 1;
//		st_sms* pMsg = m_pData->get_recourd(nIdx);
//
//		pMsg->m_type = 1;
//		m_pData->user_change( true );

	if (m_bISspeak)
	{
		CString readstr; 
		readstr = m_psms->m_context;
		const unsigned short* pszText = (unsigned short*)readstr.GetBuffer(readstr.GetLength());
		tts_speaking(m_hdtts,pszText, readstr.GetLength()+1 );
	}

		if ( m_psms )
		{
			TCHAR  szBuf[256];
			//电话号码
			memset(szBuf, 0, sizeof(szBuf) );
			ex_char2uni((const char*)m_psms->m_number, szBuf,256 );
			m_Static[0].chChar.Format(_T("%s"), szBuf);
			
			//短信内容
			memset(szBuf, 0, sizeof(szBuf) );
			ex_char2uni((const char*)m_psms->m_context, szBuf,256 );
			m_Static[2].chChar.Format(_T("%s"), szBuf);
			
			//时间
			memset(szBuf, 0, sizeof(szBuf) );
			ex_char2uni((const char*)m_psms->m_time, szBuf,256 );
			
			TCHAR szYear[3];
			TCHAR szMonth[3];
			TCHAR szDay[3];
			TCHAR szHour[3];
			TCHAR szMin[3];
			memset(szYear, 0, sizeof(szYear) );
			memset(szMonth, 0, sizeof(szYear) );
			memset(szDay, 0, sizeof(szYear) );
			memset(szHour, 0, sizeof(szYear) );
			memset(szMin, 0, sizeof(szYear) );
			
			memcpy(szYear, &szBuf[0],4);
			memcpy(szMonth, &szBuf[2],4);
			memcpy(szDay, &szBuf[4],4);
			memcpy(szHour, &szBuf[6],4);
			memcpy(szMin, &szBuf[8],4);
			m_Static[1].chChar.Format(_T("20%s-%s-%s %s:%s"), szYear, szMonth, szDay, szHour, szMin);
			
		}
//	}
//	else
//	{
//	//		int nIdx = m_nSelIdx - m_pData->max_number();
//	//		if ( nIdx < g_GsmLogic.GSMGetSIMUsed() )
////		{
////			TCHAR  szBuf[160];
////			SM_PARAM* pList = &(g_GsmLogic.GSMGetList()[nIdx]);
////			//电话号码
////			memset(szBuf, 0, sizeof(szBuf) );
////			ex_char2uni((const char*)pList->TPA, szBuf,160 );
////			m_Static[0].chChar.Format(_T("%s"), szBuf);
////			
////			//短信内容
////			memset(szBuf, 0, sizeof(szBuf) );
////			ex_char2uni((const char*)pList->TP_UD, szBuf,160 );
////			m_Static[2].chChar.Format(_T("%s"), szBuf);
////			
//// 			//时间
//// 			memset(szBuf, 0, sizeof(szBuf) );
//// 			ex_char2uni((const char*)pList->TP_SCTS, szBuf,160 );
//// 			
//// 			TCHAR szYear[3];
//// 			TCHAR szMonth[3];
//// 			TCHAR szDay[3];
//// 			TCHAR szHour[3];
//// 			TCHAR szMin[3];
//// 			memset(szYear, 0, sizeof(szYear) );
//// 			memset(szMonth, 0, sizeof(szYear) );
//// 			memset(szDay, 0, sizeof(szYear) );
//// 			memset(szHour, 0, sizeof(szYear) );
//// 			memset(szMin, 0, sizeof(szYear) );
//// 			
//// 			memcpy(szYear, &szBuf[0],4);
//// 			memcpy(szMonth, &szBuf[2],4);
////			memcpy(szDay, &szBuf[4],4);
////			memcpy(szHour, &szBuf[6],4);
////			memcpy(szMin, &szBuf[8],4);
////
////			m_Static[1].chChar.Format(_T("20%s-%s-%s %s:%s"), szYear, szMonth, szDay, szHour, szMin);
//
//		}
//	}

#endif

#if 0
	int nIdx = m_pData->max_number() - m_nSelIdx - 1;
	st_sms* pMsg = m_pData->get_recourd(nIdx);
	if ( pMsg )
	{
		TCHAR  szBuf[256];
		//电话号码
		memset(szBuf, 0, sizeof(szBuf) );
		ex_char2uni((const char*)pMsg->m_number, szBuf,256 );
		m_Static[0].chChar.Format(_T("%s"), szBuf);

		//短信内容
		memset(szBuf, 0, sizeof(szBuf) );
		ex_char2uni((const char*)pMsg->m_context, szBuf,256 );
		m_Static[2].chChar.Format(_T("%s"), szBuf);

		//时间
		memset(szBuf, 0, sizeof(szBuf) );
		ex_char2uni((const char*)pMsg->m_time, szBuf,256 );
		
		TCHAR szYear[3];
		TCHAR szMonth[3];
		TCHAR szDay[3];
		TCHAR szHour[3];
		TCHAR szMin[3];
		memset(szYear, 0, sizeof(szYear) );
		memset(szMonth, 0, sizeof(szYear) );
		memset(szDay, 0, sizeof(szYear) );
		memset(szHour, 0, sizeof(szYear) );
		memset(szMin, 0, sizeof(szYear) );
		
		memcpy(szYear, &szBuf[0],4);
		memcpy(szMonth, &szBuf[2],4);
		memcpy(szDay, &szBuf[4],4);
		memcpy(szHour, &szBuf[6],4);
		memcpy(szMin, &szBuf[8],4);
		if (pMsg->m_time != NULL)
		{
			m_Static[1].chChar.Format(_T("20%s-%s-%s %s:%s"), szYear, szMonth, szDay, szHour, szMin);
		}
		
	}
#endif
}

int   CDispatchInfo::SMSMaxNum()
{
	logic_data* pInbox = m_pData;/*&(g_GsmLogic.m_inbox)*/;
	int nMax = pInbox->max_number();
	nMax += g_GsmLogic.GSMGetSIMUsed();
	return nMax;
}

void CDispatchInfo::Reply()
{
	OnExit();
	return;

	CString strSMS;
	if ( LoadSMSContext(strSMS) )
	{
		CSMSSending Smsdlg;

		CString str;
		str = m_Static[0].chChar;
		if(str.Left(2).Compare(_T("86")) == 0)
		{
			m_Static[0].chChar = str.Right(str.GetLength()-2);
		}
		else if(str.Left(3).Compare(_T("+86")) == 0)
		{
			m_Static[0].chChar = str.Right(str.GetLength()-3);
		}

		Smsdlg.SetCallPhone(m_Static[0].chChar);
		Smsdlg.SetDestoryHwnd(NULL);
		Smsdlg.SetMsgParam( m_Static[0].chChar, strSMS, TRUE );
		Smsdlg.DoModal();
	}
}

BOOL  CDispatchInfo::LoadSMSContext(CString& strContext)
{
	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	CDllMidware::Instance().BKliteTimeOutSet(0xFFFFFFFF);

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
		g_bLight = false;
		MessageBox( _T("Load IMM has Failed!\n"), _T("System Tip"), MB_OK );
		return FALSE;
	}
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();

	res = CIMMMng::Instance().IMMGetResult();
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
		g_GsmLogic.UserSaveMsg( res.strName );
		g_GsmLogic.m_draft.save_data( SMS_PATH_DRAFT );
	}

	return FALSE;
}

void CDispatchInfo::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_CLOSE_WINDOW )
	{
		KillTimer( nIDEvent );
		OnExit();
	}
	CDialogBase::OnTimer(nIDEvent);
}
LRESULT CDispatchInfo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (MSG_NEW_DISPATCHINFO == message)
	{
		m_psms = (st_sms*)wParam;
		UpdateRecord();
		KillTimer( TID_CLOSE_WINDOW );
		SetTimer(TID_CLOSE_WINDOW, 20*1000, NULL);
		Invalidate();
	}
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}