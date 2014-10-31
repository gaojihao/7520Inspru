// DlgPair.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgPhoneEdit.h"
#include "E9101MainDlg.h"
#include "HBCP.h"
#include "command.h"
#include "PhoneBookMng.h"
//#include "DlgAddPhone.h"

//#include "DlgDial.h"
#include "E9101MainDlg.h"
#include "..\imm\IMMMng.h"
#include "DlgConfirm.h"
#include "DlgDiaIn.h"
#include "DlgNumber.h"
#include "PhoneInfoMng.h"
#include "GSMDial.h"
#include "../ExTool/ex_basics.h"
#include "../BrightnessSetDlg.h"
#include "../SQLite3/CppSQLite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CDlgPhoneEdit::m_PhoneName;
CString CDlgPhoneEdit::m_PhoneNum;
CString CDlgPhoneEdit::m_PhoneCity;
UINT CDlgPhoneEdit::m_PhonePos;
BOOL CDlgPhoneEdit::m_AddPhone;
HWND g_PhoneEdit;
/////////////////////////////////////////////////////////////////////////////
// CDlgPhoneEdit dialog
extern BOOL g_PowerOn;

extern BOOL g_bLight;

CDlgPhoneEdit::CDlgPhoneEdit(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgPhoneEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPair)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_NewPhone = TRUE;
	tempPhoneNum = _T("");
	m_PrevDlg = NULL;
	m_bSIM = FALSE;
	m_nIdx = -1;
}


void CDlgPhoneEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPhoneEdit)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPhoneEdit, CDialogBase)
	//{{AFX_MSG_MAP(CDlgPair)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IMM_MESSAGE, OnIMMMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPair message handlers

BOOL CDlgPhoneEdit::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	// TODO: Add extra initialization here
	g_PhoneEdit = m_hWnd;
	InitControl();
	InitGdi();

	OnLanguageChange();
	OnSkinChange();
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL  CDlgPhoneEdit::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_BTN_BT_PHONEBOOK);
	m_stTitle = str;

	str.LoadString(RES_BTN_BT_Name);
	m_stName = str;

	str.LoadString(RES_BTN_BT_Num);
	m_stNum = str;	

	str.LoadString(RES_BTN_BT_DIAL);
	m_stEditCall = str;	

	str.LoadString(RES_BUTTON_DEL);
	m_stEditDel = str;	

	str.LoadString( RES_BUTTON_PHONE_CITY ) ;
	m_Info = str;
	//m_Info+= m_PhoneCity;

	if ( m_AddPhone )
	{
		str.LoadString( RES_SAVE_TO_SIM );
		m_Item[5].chChar = str;
		str.LoadString( RES_SAVE_TO_DEVICE );
		m_Item[6].chChar = str;
	}

	return TRUE;
}

BOOL  CDlgPhoneEdit::OnSkinChange()
{
	return TRUE;
}
//GetWindowThreadProcessId
void CDlgPhoneEdit::OnExit()
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


	CDialogBase::EndDialog(0);
}

void CDlgPhoneEdit::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	// lpszFacename
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


	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_PHONEBOOKEDIT_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_PHONEBOOKEDIT_N, true);
	m_stBtnPressBMP = pSkin->hBitmap;    

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));


	if ( m_AddPhone )
	{
		CDC dcTemp;
		dcTemp.CreateCompatibleDC( m_pDC );
		pSkin = g_pResMng->RequestBmp(SKIN_BTN_SAVE, true);
		HBITMAP TmpBmp = pSkin->hBitmap;
		CBitmap* pOld = dcTemp.SelectObject( CBitmap::FromHandle(TmpBmp) );

		//CRect rt(m_Item[5].rect);
		//bk_normal_dc.BitBlt( rt.left,rt.top,rt.Width(),rt.Height(),&dcTemp,0,0,SRCCOPY );
		//bk_press_dc.BitBlt( rt.left,rt.top,rt.Width(),rt.Height(),&dcTemp,0,rt.Height(),SRCCOPY);

		CRect rt1(m_Item[6].rect);
		bk_normal_dc.BitBlt(rt1.left,rt1.top,rt1.Width(),rt1.Height(),&dcTemp,0,0,SRCCOPY);
		bk_press_dc.BitBlt(rt1.left,rt1.top,rt1.Width(),rt1.Height(),&dcTemp,0,rt1.Height(),SRCCOPY);

		dcTemp.SelectObject( pOld );
		::DeleteObject( TmpBmp );
		TmpBmp = NULL;
		dcTemp.DeleteDC();
	}
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgPhoneEdit::InitControl()
{
	//Exit
	m_Item[0].rect.left		= 740;
	m_Item[0].rect.top		= 0;
	m_Item[0].rect.right	= 800;
	m_Item[0].rect.bottom	= 60;
	m_Item[0].nState = BTN_STATE_NORMAL;
	//拨号
	m_Item[1].rect.left		= 185;
	m_Item[1].rect.top		= 397;
	m_Item[1].rect.right	= 328;
	m_Item[1].rect.bottom	= 478;
	m_Item[1].nState = BTN_STATE_NORMAL;
	//删除
	m_Item[2].rect.left		= 329;
	m_Item[2].rect.top		= 397;
	m_Item[2].rect.right	= 472;
	m_Item[2].rect.bottom	= 478;
	m_Item[2].nState = BTN_STATE_NORMAL;
	//名字
	m_Item[3].rect.left		= 174;
	m_Item[3].rect.top		= 170;
	m_Item[3].rect.right	= 628;
	m_Item[3].rect.bottom	= 209;
	m_Item[3].nState = BTN_STATE_NORMAL;
	//号码
	m_Item[4].rect.left		= 174;
	m_Item[4].rect.top		= 271;
	m_Item[4].rect.right	= 628;
	m_Item[4].rect.bottom	= 312;
	m_Item[4].nState = BTN_STATE_NORMAL;


	//钮1X1：47 Y1：169  X2：221 Y2：232
	//钮2X1：235 Y1：169  X2：410 Y2：232
	//Save to SIM
	m_Item[5].nState = BTN_STATE_NORMAL;
	m_Item[5].rect.top = 0;
	m_Item[5].rect.left = 0;
	m_Item[5].rect.bottom = 0 + 0;
	m_Item[5].rect.right = 0;

	// Save to Device
	m_Item[6].nState = BTN_STATE_NORMAL;
	m_Item[6].rect.left		= 473;
	m_Item[6].rect.top		= 397;
	m_Item[6].rect.right	= m_Item[6].rect.left+144;
	m_Item[6].rect.bottom	= m_Item[6].rect.top+82;
}

void CDlgPhoneEdit::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
void CDlgPhoneEdit::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgPhoneEdit::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
	int nList = 5;
	if ( m_AddPhone ) nList = 7;
	for(int i=0;i<nList;i++)
	{
		if ( i == 5 ) continue;
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	DrawBtnText(&memDC, g_retTitile, m_stTitle, CGDICommon::Instance()->bigbigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_CENTER);
	//名字
	str.Format(_T("  %s:%s"), m_stName, m_PhoneName);
	DrawBtnText(&memDC, m_Item[3].rect, str, CGDICommon::Instance()->bigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_LEFT);
	//号码
	str.Format(_T("  %s:%s"), m_stNum, m_PhoneNum);
	DrawBtnText(&memDC, m_Item[4].rect, str, CGDICommon::Instance()->bigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_LEFT);

	//DrawBtnText(&memDC,CRect(546,107,680,167),m_stEditCall,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	//DrawBtnText(&memDC,CRect(546,207,680,267),m_stEditDel,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);

	if ( m_AddPhone )
	{
		//DrawBtnText(&memDC,m_Item[5].rect,m_Item[5].chChar,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
		//保存至本机
		//DrawBtnText(&memDC,m_Item[6].rect,m_Item[6].chChar,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	}

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	
}

void CDlgPhoneEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
// 	int nList = 5;
// 	if ( m_AddPhone ) nList = 7;
	for(int i=0; i<7; i++)
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

void CDlgPhoneEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
// 	int nList = 5;
// 	if ( m_AddPhone ) nList = 7;
	for(int i=0; i<7; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch(i)
			{
			case 0:	OnExit();break;
			case 1: OnPhoneCallout();break;
			case 2:
				{//删除
					if( !m_AddPhone )
					{
						CResString str;
						str.LoadString( RES_BUTTON_DEL_PB );
						CDlgConfirm::m_stTitle = str;
						CDlgConfirm dlg;
						dlg.DoModal();
						if( CDlgConfirm::s_bOk )
						{
							if ( m_bSIM )
								DelSIM( m_nIdx );
							else
								DelDevice( m_nIdx );
							OnExit();
						}
					}
				}
				break;
			case 3: OnEditPhoneName();break;
			case 4: OnEditPhoneNum();break;
				//case 5: SaveToSIM(); break;
			case 6: SaveToDevice(); break;
			default:
				break;
			}
		}
	}	

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}


void CDlgPhoneEdit::OnTimer(UINT nIDEvent) 
{
	CDialogBase::OnTimer(nIDEvent);
}

void CDlgPhoneEdit::OnEditPhoneNum()
{
	ImmReturn ret;
	IMMRESULT res;
	CString strName;
	strName = _T("");

	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	
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
		m_PhoneNum =  res.strName;
	}
	Invalidate();
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();

	/*
	CDlgNumber dlg;
	tempPhoneNum = m_PhoneNum;	
	dlg.DoModal();
	*/
}

void CDlgPhoneEdit::OnEditPhoneName()
{
	ImmReturn ret;
	IMMRESULT res;
	CString strName;
	strName = _T("");

	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	
	enSkinType enSkin = ID_SKIN2;
	if(CIMMMng::Instance().Load() == TRUE)
	{
		CIMMMng::Instance().ImmSetKeySound( g_enKeySnd );
		CIMMMng::Instance().IMMInit(FourPoint3);
		ret = CIMMMng::Instance().IMMCreateMessage(ID_CHINESE);
	}
	res = CIMMMng::Instance().IMMGetResult();
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();

	if( res.strName != _T("") )
	{
		if(m_AddPhone)
		{
			m_PhoneName = res.strName;
			m_NewPhone = FALSE;
		}
		else
		{
			m_PhoneName = res.strName;
		}

		if ( m_PhoneName.GetLength() > 8 )
			m_PhoneName = res.strName.Left( 8 );
	}
}

LRESULT CDlgPhoneEdit::OnIMMMsg(WPARAM wParam , LPARAM lParam)
{
	CString str;
	str.Format(_T("%d"), wParam);
	if ( m_PhoneNum.GetLength() >= 16 && wParam != 10 )
		return 1;
	switch(wParam)
	{
	case 0:
		m_PhoneNum += str;
		break;
	case 1:
		m_PhoneNum += str;
		break;
	case 2:
		m_PhoneNum += str;
		break;
	case 3:
		m_PhoneNum += str;
		break;
	case 4:
		m_PhoneNum += str;
		break;
	case 5:
		m_PhoneNum += str;
		break;
	case 6:
		m_PhoneNum += str;
		break;
	case 7:
		m_PhoneNum += str;
		break;
	case 8:
		m_PhoneNum += str;
		break;
	case 9:
		m_PhoneNum += str;
		break;
	case 10:
		m_PhoneNum = m_PhoneNum.Left(m_PhoneNum.GetLength()-1);
		break;
	case 11:
		{
			//修改电话薄，则保存电话
			// 			if ( !m_AddPhone )
			// 			{
			// 				if( !m_PhoneNum.IsEmpty() && !m_PhoneName.IsEmpty() )
			// 				{
			// 					if( m_bSIM )
			// 						ModifySIM( m_nIdx );
			// 					else
			// 						ModifyDevice( m_nIdx );						
			// 				}
			// 			}
		}
		break;
	case 12:
		m_PhoneNum = tempPhoneNum;
		break;
	}

	Invalidate();
	return 1;
}

void CDlgPhoneEdit::OnPhoneCallout()
{
	if( m_PhoneNum.IsEmpty() )
		return;

	CGSMDial dlg;
	CGSMDial::m_UserNum = m_PhoneNum;
	CGSMDial::m_bAutoDial = TRUE;
	if ( m_PrevDlg )
		m_PrevDlg->OnExit();
	OnExit();
	dlg.DoModal();
}


//////////////////////////////////////////////////////////////////////////

void   CDlgPhoneEdit::SaveToSIM( )
{
	return;

	if ( m_AddPhone )
	{
		if( !m_PhoneNum.IsEmpty() && !m_PhoneName.IsEmpty() )
		{
			PBOOK_ITEM Item;
			memset( &Item, 0, sizeof(PBOOK_ITEM));
			ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneNum, Item.phone, sizeof(Item.phone)/sizeof(Item.phone[0]));
			ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneName, Item.name, sizeof(Item.name)/sizeof(Item.name[0]));
			Item.state = 1;
			Item.nIdx = 0;
			//需要到SIM卡上
			//..........
			g_GsmLogic.GSMAddSIMPBook( Item.name, Item.phone );
			g_GsmLogic.GSMWritePbook();
			OnExit();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void   CDlgPhoneEdit::SaveToDevice( )
{
	if( m_PhoneNum.IsEmpty() || m_PhoneName.IsEmpty() )
		return;

	PBOOK_ITEM Item;
	memset( &Item, 0, sizeof(PBOOK_ITEM));
	ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneNum, Item.phone, sizeof(Item.phone)/sizeof(Item.phone[0]));
	ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneName, Item.name, sizeof(Item.name)/sizeof(Item.name[0]));

	//写入数据库
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	char szSql[1024];
	memset(szSql, 0, sizeof(szSql));
	if ( m_AddPhone )
	{
		sprintf(szSql, "INSERT INTO phone_book(flag, name, phone_number) VALUES(%d, '%s', '%s');", 
			3, Item.name, Item.phone);
	}
	else
	{
		sprintf(szSql, "UPDATE phone_book SET flag = %d, name = '%s', phone_number = '%s' \
					   WHERE UID IN (SELECT UID FROM phone_book ORDER BY name LIMIT %d,1);", 
			3, Item.name, Item.phone, m_nIdx);
	}
	db.execDML(szSql);
	db.close();
	OnExit();
}

//////////////////////////////////////////////////////////////////////////

void   CDlgPhoneEdit::ModifySIM(int nIdx)
{
	return;
	PBOOK_ITEM* Item = g_GsmLogic.GSMGetSIMPBookRd( nIdx );
	if ( Item )
	{
		memset( Item->name, 0, sizeof(Item->name) );
		memset( Item->phone, 0, sizeof(Item->phone) );
		ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneNum, Item->phone, sizeof(Item->phone)/sizeof(Item->phone[0]));
		ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneName, Item->name, sizeof(Item->name)/sizeof(Item->name[0]));
		g_GsmLogic.GSMUpdateSIMPBook( Item );
	}
}

//////////////////////////////////////////////////////////////////////////

void   CDlgPhoneEdit::ModifyDevice( int nIdx)
{
	logic_pbook* pbook = &( g_GsmLogic.m_pbook );
	PBOOK_ITEM* Item = pbook->get_recourd( nIdx );
	if ( Item )
	{
		memset( Item->name, 0, sizeof(Item->name) );
		memset( Item->phone, 0, sizeof(Item->phone) );
		ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneNum, Item->phone, sizeof(Item->phone)/sizeof(Item->phone[0]));
		ex_uni2char((LPTSTR)(LPCTSTR)m_PhoneName, Item->name, sizeof(Item->name)/sizeof(Item->name[0]));
		pbook->user_change( true );
	}
}

//////////////////////////////////////////////////////////////////////////

void   CDlgPhoneEdit::DelSIM(int nIdx)
{
	return;
	g_GsmLogic.GSMDelSIMPBook(nIdx);
}

//////////////////////////////////////////////////////////////////////////

void   CDlgPhoneEdit::DelDevice(int nIdx)
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	char szSql[512];
	sprintf(szSql, "DELETE FROM phone_book WHERE UID IN (SELECT UID FROM phone_book ORDER BY name LIMIT %d,1);", m_nIdx);
	db.execDML( szSql );
	db.close();
}
