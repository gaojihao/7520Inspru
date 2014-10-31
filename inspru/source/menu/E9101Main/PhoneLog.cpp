// PhoneLog.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "PhoneLog.h"
//#include "DlgDial.h"
#include "E9101MainDlg.h"
#include "DlgConfirm.h"
#include "DlgDiaIn.h"
#include "GSMDial.h"
#include "PhoneBookMng.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../SQLite3/CppSQLite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//extern HWND g_hDialOut;
extern UINT g_PhoneDialOut;
enLogType	CPhoneLog::enLog;
/////////////////////////////////////////////////////////////////////////////
// CPhoneLog dialog


CPhoneLog::CPhoneLog(CWnd* pParent /*=NULL*/)
	: CDialogBase(CPhoneLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ObArrary = NULL;
	m_Pos = 0;
	m_CurPage = 1;
	m_TotalPage = 0;
	CurPhoneLog = 0;
	m_pGSMMain = NULL;
	enLog = PLACEDCALL;
}


void CPhoneLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneLog)
	DDX_Control(pDX,IDC_BTN_EXIT, m_BtnExit);
	DDX_Control(pDX,IDC_BTN_DEL_ALL, m_BtnDelAll);
	DDX_Control(pDX,IDC_BTN_ITEM1, m_BtnPhoneLog1);
	DDX_Control(pDX,IDC_BTN_ITEM2, m_BtnPhoneLog2);
	DDX_Control(pDX,IDC_BTN_ITEM3, m_BtnPhoneLog3);
	DDX_Control(pDX,IDC_BTN_ITEM4, m_BtnPhoneLog4);
	DDX_Control(pDX,IDC_BTN_ITEM5, m_BtnPhoneLog5);
	DDX_Control(pDX,IDC_BTN_PAGEUP, m_BtnPageUp);
	DDX_Control(pDX,IDC_BTN_PAGEDOWN, m_BtnPageDown);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhoneLog, CDialogBase)
	//{{AFX_MSG_MAP(CPhoneLog)
	ON_BN_CLICKED(IDC_BTN_DEL_ALL, OnBtnDelAll)
	ON_BN_CLICKED(IDC_BTN_ITEM1, OnBtnItem1)
	ON_BN_CLICKED(IDC_BTN_ITEM2, OnBtnItem2)
	ON_BN_CLICKED(IDC_BTN_ITEM3, OnBtnItem3)
	ON_BN_CLICKED(IDC_BTN_ITEM4, OnBtnItem4)
	ON_BN_CLICKED(IDC_BTN_ITEM5, OnBtnItem5)
	ON_BN_CLICKED(IDC_BTN_PAGEUP, OnBtnPageup)
	ON_BN_CLICKED(IDC_BTN_PAGEDOWN, OnBtnPagedown)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneLog message handlers

BOOL CPhoneLog::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGdi();
	InitControl();
 	//PhoneLogMng::Instance().SetLPhoneLogState(3);	//拨出


	OnLanguageChange();
	OnSkinChange();

	//PhoneBookMng::Instance().Load();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//变换语言和单位
BOOL CPhoneLog::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_BTN_BT_DIALLOG);
	m_stTitle = str;
	return TRUE;
}

//变皮肤
BOOL CPhoneLog::OnSkinChange()
{
	return TRUE;
}


void CPhoneLog::OnBtnDelAll() 
{
	m_ObArrary = NULL;
	m_Pos = 0;
	m_CurPage = 0;
	m_TotalPage = 0;
	
	PhoneLogMng::Instance().DelAll();
	PostMessage(WM_ACTIVATE);
}

void CPhoneLog::OnBtnDelLogState(int state) 
{
	m_ObArrary = NULL;
	m_Pos = 0;
	m_CurPage = 1;
	m_TotalPage = 0;
	UINT Tag = 0;
	//PhoneLogMng::Instance().DelLPhoneLogState(state);
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	
	switch(enLog)
	{
	case PLACEDCALL:
		{
			Tag = 1;
			sprintf(szSqlBuffer, "DELETE FROM phone_log WHERE flag = '%d';", Tag);
			db.execDML(szSqlBuffer);
		}
		break;
	case RECEIVEDCALL:
		{
			Tag = 2;
			sprintf(szSqlBuffer, "DELETE FROM phone_log WHERE flag = '%d';", Tag);
			db.execDML(szSqlBuffer);
		}
		break;
	case MISSEDCALL:
		{
			Tag = 3;
			sprintf(szSqlBuffer, "DELETE FROM phone_log WHERE flag = '%d';", Tag);
			db.execDML(szSqlBuffer);
		}
		break;
	default:
		break;
	}
	
	db.close();

	PostMessage(WM_ACTIVATE);
}

void CPhoneLog::OnBtnItem1() 
{
	if(PhoneLogMng::Instance().GetCount() <= (m_Pos))
		return;

	PhoneLogMng::Instance().SetCurPhoneLogItem( m_Pos );
}

void CPhoneLog::OnBtnItem2() 
{
	if(PhoneLogMng::Instance().GetCount() <= (m_Pos + 1 ))
		return;

	PhoneLogMng::Instance().SetCurPhoneLogItem( m_Pos + 1 );	
}

void CPhoneLog::OnBtnItem3() 
{
	if(PhoneLogMng::Instance().GetCount() <= (m_Pos + 2 ))
		return;

	PhoneLogMng::Instance().SetCurPhoneLogItem( m_Pos + 2 );
}

void CPhoneLog::OnBtnItem4() 
{
	// TODO: Add your control notification handler code here
	if(PhoneLogMng::Instance().GetCount() <= (m_Pos + 3 ))
		return;

	PhoneLogMng::Instance().SetCurPhoneLogItem( m_Pos + 3 );	
}

void CPhoneLog::OnBtnItem5() 
{
	// TODO: Add your control notification handler code here
	if(PhoneLogMng::Instance().GetCount() <= (m_Pos + 4 ))
		return;

	PhoneLogMng::Instance().SetCurPhoneLogItem( m_Pos + 4 );	
}

void CPhoneLog::OnBtnPageup() 
{
// 	if(m_CurPage > 0)
// 	{
// 		m_Item[10].nState = BTN_STATE_NORMAL;
// 		m_CurPage--;
// 	}
// 	else
// 	{
// 		m_Item[5].nState = BTN_STATE_DISABLE;
// 		return;
// 	}
// 
// 	if(m_CurPage == 0)
// 	{
// 		m_Item[5].nState = BTN_STATE_DISABLE;
// 	}
// 
// 	int i=0;
// 	for(i=0; i<5; i++)
// 	{
// 		ItemStr[i] = _T("");
// 	}
// 	for(int j=0; j<5; j++)
// 	{
// 		ItemStrCity[j] = _T("");
// 	}
	if(m_CurPage > 0)
	{
		//m_Item[5].nState = BTN_STATE_DISABLE;
		m_CurPage--;
	}
	else
	{
		//m_Item[5].nState = BTN_STATE_DISABLE;
		return;
	}
	ShowPage(m_CurPage);

// 	CDialLog* pPhonelogItem = NULL;
// 	UINT TempPos = 0;
// 
// 	for( i=0;i<5; i++)
// 	{
// 		 pPhonelogItem = (CDialLog*)m_ObArrary->GetAt(m_CurPage*5+i);
// 		 if ( pPhonelogItem )
// 			 ItemStr[i] = pPhonelogItem->m_csNum; 	 
// 	}
// 	UpdateUserName();
	Invalidate();	
}

void CPhoneLog::OnBtnPagedown() 
{
// 	if(m_CurPage < m_TotalPage)
// 	{
// 		m_Item[5].nState = BTN_STATE_NORMAL;
// 		m_CurPage++;
// 	}
// 	else
// 	{
// 		m_Item[10].nState = BTN_STATE_DISABLE;
// 		return;
// 	}
// 
// 	if(m_CurPage == m_TotalPage - 1)
// 	{
// 		m_Item[10].nState = BTN_STATE_DISABLE;
// 	}
// 	int i=0;
// 	for(i=0; i<5; i++)
// 	{
// 		ItemStr[i] = _T("");
// 	}

// 	for(int j=0; j<5; j++)
// 	{
// 		ItemStrCity[j] = _T("");
// 	}
	
	if (m_CurPage >= m_nTotalPage)
	{
		//m_Item[10].nState = BTN_STATE_DISABLE;
		m_CurPage = m_nTotalPage;
	}
	else
	{
		//m_Item[10].nState = BTN_STATE_DISABLE;
		m_CurPage++;
	}
	ShowPage(m_CurPage);
// 	CDialLog* pPhoneLogItem = NULL;
// 	
// 	UINT TempPos = 0;
// 	int tempValue;
// 
// 	if(m_CurPage == m_TotalPage-1)
// 	{
// 			UINT nCount = PhoneLogMng::Instance().GetCount();
// 			//tempValue = nCount%5;
// 			tempValue = nCount - m_CurPage * 5;
// 	} 
// 	else
// 		tempValue = 5;
// 
// 	for(i=0;i<tempValue;i++)
// 	{
// 		 pPhoneLogItem = (CDialLog*)m_ObArrary->GetAt(m_CurPage*5+i);
// 		 if ( pPhoneLogItem )
// 		 {
// 			 ItemStr[i] = pPhoneLogItem->m_csNum;
// 		 }
// 	}
// 	UpdateUserName();
	Invalidate();
}

LRESULT CPhoneLog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( (WM_ACTIVATE == message) && (LOWORD(wParam) != WA_INACTIVE) )
	{
// 		int i=0;
// 		for(i=0; i<5; i++)
// 		{
// 			ItemStr[i] = _T("");
// 		}
// 		for( i=0; i<5; i++)
// 		{
// 			ItemStrCity[i] = _T("");
// 		}
// 		m_Pos = 0;
// 		m_CurPage = 0;
// 
// 		m_Item[5].nState = BTN_STATE_DISABLE;
// 		m_Item[10].nState = BTN_STATE_DISABLE;
// 
// 		if(!PhoneLogMng::Instance().Load())
// 		{
// 			m_Item[10].nState = BTN_STATE_DISABLE;
// 		}
// 		else
// 		{
// 			m_ObArrary = PhoneLogMng::Instance().GetPhoneLog();
// 			if(m_ObArrary==NULL)
// 			{
// 				//m_Item[10].nState = BTN_STATE_DISABLE;
// 				return FALSE;
// 			}
// 			else
// 			{
// 				UINT nCount = PhoneLogMng::Instance().GetCount();
// 				m_TotalPage = nCount/5 + ( (nCount%5)==0?0:1 );
// 				if(nCount <= 5)
// 					m_Item[10].nState = BTN_STATE_DISABLE;
// 				else
// 					m_Item[10].nState = BTN_STATE_NORMAL;
// 				
// 				CDialLog* pPhoneLogItem = NULL;
// 				
// 				UINT TempPos = 0;
// 				for(m_Pos;TempPos<5&&TempPos <PhoneLogMng::Instance().GetCount();)
// 				{
// 					 pPhoneLogItem = (CDialLog*)m_ObArrary->GetAt(TempPos);
// 					 if ( pPhoneLogItem )
// 					 {
// 						 ItemStr[TempPos] = pPhoneLogItem->m_csNum;
// 					 }	
// 	 				 TempPos++;
// 				}
// 				UpdateUserName();
// 			}
// 			m_Pos = 0;
// 		}
		m_CurPage = 1;
		ShowPage(m_CurPage);
		Invalidate();
	}

	else  if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnExit();
		}
	}
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}


void CPhoneLog::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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


void CPhoneLog::InitControl()
{
	//列表
	for (int i =0;  i < 5; i++)
	{
		m_Item[i].rect.left			= 147;
		m_Item[i].rect.top			= 111+i*64;
		m_Item[i].rect.right		= 639;
		m_Item[i].rect.bottom		= 111+i*64+64;
		m_Item[i].nState			= BTN_STATE_NORMAL;
	}
	//上一页
	m_Item[5].rect.left		= 646;
	m_Item[5].rect.top		= 125;
	m_Item[5].rect.right	= 783;
	m_Item[5].rect.bottom	= 251;
	m_Item[5].nState		= BTN_STATE_NORMAL;

	//左排按钮
	for (int j=6;  j < 10; j++)
	{
		m_Item[j].rect.left		= 0;
		m_Item[j].rect.top		= 66+(j-6)*103;
		m_Item[j].rect.right	= 112;
		m_Item[j].rect.bottom	= 66+(j-6)*103+103;
		m_Item[j].nState		= BTN_STATE_NORMAL;
	}
	//下一页
	m_Item[10].rect.left	= 646;
	m_Item[10].rect.top		= 261;
	m_Item[10].rect.right	= 783;
	m_Item[10].rect.bottom	= 387;
	m_Item[10].nState		= BTN_STATE_NORMAL;
	
	m_Item[6].nState = BTN_STATE_DISABLE;	//拨出
	m_Item[7].nState = BTN_STATE_NORMAL;	//接听
	m_Item[8].nState = BTN_STATE_NORMAL;	//未接
	
	m_Item[11].rect.left	= 735;
	m_Item[11].rect.top		= 0;
	m_Item[11].rect.right	= 800;
	m_Item[11].rect.bottom	= 60;
	m_Item[11].nState		= BTN_STATE_NORMAL;
}

void CPhoneLog::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);

	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());
	//
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
	 		_T("宋体\0"));             // lpszFacename

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	bk_disable_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_PHONELOG_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_PHONELOG_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;  

 	pSkin = g_pResMng->RequestBmp(SKIN_BG_PHONELOG_D, true);
 	m_stBtnDisableBMP = pSkin->hBitmap;  

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
 	m_pold_bk_disable_bmp    = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}


void CPhoneLog::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<12;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
	}

	CRect temprect;
	for(int j=0; j<5; j++)
	{
		temprect		= m_Item[j].rect;
		temprect.left	+= 20;
		DrawBtnText(&memDC,temprect,ItemStr[j],CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_LEFT | DT_VCENTER);

		temprect.left	+= 265;
		DrawBtnText(&memDC,temprect,m_strTime[j],CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_LEFT | DT_VCENTER);
	}

	DrawBtnText(&memDC,g_retTitile,m_stTitle,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_CENTER | DT_VCENTER);
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CPhoneLog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<12;i++)
	{
		if(PtInRect(&m_Item[i].rect,point))
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				PlayKeySound();
				m_Item[i].nState = BTN_STATE_DOWN;
			}
	}

	Invalidate();
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CPhoneLog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<12;i++)
	{
//		if(PtInRect(&m_Item[i].rect,point))
//		{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
				switch(i)
				{
				case 0:	case 1:	case 2:	case 3:	case 4:
					OnStaticList(i);
					break;
				case 5:
					OnBtnPageup(); 
					break;
				case 6:	//拨打出
					m_Item[6].nState = BTN_STATE_DISABLE;
					m_Item[7].nState = BTN_STATE_NORMAL;
					m_Item[8].nState = BTN_STATE_NORMAL;
					//PhoneLogMng::Instance().SetLPhoneLogState(3);
					enLog = PLACEDCALL;
					PostMessage(WM_ACTIVATE,WA_ACTIVE,0);
					break;
				case 7:	//已接听
					m_Item[6].nState = BTN_STATE_NORMAL;
					m_Item[7].nState = BTN_STATE_DISABLE;
					m_Item[8].nState = BTN_STATE_NORMAL;
					//PhoneLogMng::Instance().SetLPhoneLogState(2);
					enLog = RECEIVEDCALL;
					PostMessage(WM_ACTIVATE,WA_ACTIVE,0);
					break;
				case 8:	//未接听
					m_Item[6].nState = BTN_STATE_NORMAL;
					m_Item[7].nState = BTN_STATE_NORMAL;
					m_Item[8].nState = BTN_STATE_DISABLE;
					//PhoneLogMng::Instance().SetLPhoneLogState(1);
					enLog = MISSEDCALL;
					PostMessage(WM_ACTIVATE,WA_ACTIVE,0 );
					break;
				case 9:
					{
						CDlgConfirm dlg;
						CResString str;

						if( m_Item[6].nState == BTN_STATE_DISABLE )
						{
							str.LoadString(RES_BUTTON_DEL_OUT);
							CDlgConfirm::m_stTitle = str;
							dlg.DoModal();
							if( CDlgConfirm::s_bOk )
								OnBtnDelLogState(1); 
						}
						else if( m_Item[7].nState == BTN_STATE_DISABLE )
						{
							str.LoadString(RES_BUTTON_DEL_IN);
							CDlgConfirm::m_stTitle = str;
							dlg.DoModal();
							if( CDlgConfirm::s_bOk )
								OnBtnDelLogState(2); 
							
						}
						else if( m_Item[8].nState == BTN_STATE_DISABLE )
						{
							str.LoadString(RES_BUTTON_DEL_NO);
							CDlgConfirm::m_stTitle = str;
							dlg.DoModal();
							if( CDlgConfirm::s_bOk )
								OnBtnDelLogState(3); 
							
						}
						PostMessage(WM_ACTIVATE,WA_ACTIVE,0);
					}
					break;
				case 10:
					OnBtnPagedown(); 
					break;	
				case 11:
					OnExit();
					break;					
				default:
					break;
				
				}
			}
	}

	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}
void CPhoneLog::OnStaticList(UINT nId)
{
	CString str;
	if(ItemStr[nId] == _T("")) return;
	m_Pos = (m_CurPage)*5 + nId;
// 	CDialLog* pPhoneLogItem = NULL;
// 	pPhoneLogItem = (CDialLog*)m_ObArrary->GetAt(m_Pos);
	CDlgConfirm dlg;
	CResString Resstr;
	Resstr.LoadString(RES_BUTTON_CALL_PHONE);
	CDlgConfirm::m_stTitle = Resstr;
	dlg.DoModal();
	if( CDlgConfirm::s_bOk )
	{
		CGSMDial::m_UserNum = m_Item[nId].chChar;
		CGSMDial::m_bAutoDial = TRUE;
		 HWND hWnd = m_pGSMMain->GetSafeHwnd();
		//用户拨打电话消息
		::PostMessage( hWnd, 0x9876, 0,0 );
	}

}


void CPhoneLog::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
{
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

void CPhoneLog::OnExit()
{
	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	PhoneBookMng::Instance().Write();
	g_GsmLogic.m_pbook.save_data( PBOOK_PATH_DEVICE );
	CDialogBase::EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////

void CPhoneLog::UpdateUserName()
{
	if ( !g_GsmLogic.m_pbook.is_open() )
		g_GsmLogic.m_pbook.load_data( PBOOK_PATH_DEVICE );

	//通过电话簿查找用户姓名
	CPhoneBookFile* pFile = NULL;
	for( int i = 0; i < 5; i++ )
	{
		if ( !g_GsmLogic.SchPBook( ItemStr[i], ItemStrCity[i] ) )
			ItemStrCity[i] = _T("");
	}
}

void CPhoneLog::ShowPage(int nPageIndex)
{
	CppSQLite3DB db_808;
	db_808.open(PATH_SQLITE_DB_808);	//打开数据库

	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	CppSQLite3Query q, p;
	CppSQLite3Table t;

	switch(enLog)
	{
	case PLACEDCALL:
		{
			//查询记录总数量
			t = db_808.getTable("select * from phone_log where flag = 1;");
			m_nRecordCount  = t.numRows();
			m_nTag = 1;

			//在数据库中查询第nPageIndex页的SDCount条数据
			sprintf(szSqlBuffer, "SELECT * FROM phone_log where UID and flag = %d ORDER BY log_datatime DESC LIMIT %d, %d;", m_nTag, (nPageIndex-1)*5, 5);
			q = db_808.execQuery(szSqlBuffer);
		}
		break;
	case RECEIVEDCALL:
		{
			//查询记录总数量
			t = db_808.getTable("select * from phone_log where flag = 2;");
			m_nRecordCount  = t.numRows();
			m_nTag = 2;

			//在数据库中查询第nPageIndex页的SDCount条数据
			sprintf(szSqlBuffer, "SELECT * FROM phone_log where UID and flag = %d ORDER BY log_datatime DESC LIMIT %d, %d;", m_nTag, (nPageIndex-1)*5, 5);
			q = db_808.execQuery(szSqlBuffer);
		}
		break;
	case MISSEDCALL:
		{
			//查询记录总数量
			t = db_808.getTable("select * from phone_log where flag = 3;");
			m_nRecordCount  = t.numRows();
			m_nTag = 3;

			//在数据库中查询第nPageIndex页的SDCount条数据
			sprintf(szSqlBuffer, "SELECT * FROM phone_log where UID and flag = %d ORDER BY log_datatime DESC LIMIT %d, %d;", m_nTag, (nPageIndex-1)*5, 5);
			q = db_808.execQuery(szSqlBuffer);
		}
		break;
	default:
		break;
	}
	//计算总页数
	if(m_nRecordCount > 0)
		m_nTotalPage = (m_nRecordCount-1)/5+1;
	else
		m_nTotalPage = 1;

	for(int k=0; k<5; k++)
	{
		m_strTimeLen[k]  = _T("");
		m_strTime[k]	 = _T("");
		ItemStr[k]       = _T("");
		m_Item[k].chChar = _T("");
	}

	for( int i=0; i<5; i++ )
	{	
		if ( !q.eof() )	//数据行
		{
			const char* number = q.fieldValue("phone_number");		//号码
			m_Item[i].chChar = number;
			ItemStr[i] = m_Item[i].chChar;
			//m_strPhoneNum[i] = m_ItemRecord[i][1].chChar;

			sprintf(szSqlBuffer, "SELECT * FROM phone_book where phone_number = '%s' ;", number);
			p = db_808.execQuery(szSqlBuffer);
			m_strName[i] = p.fieldValue("name");		//名字
			if (m_strName[i] != _T(""))
			{
				ItemStr[i] = m_strName[i];
			}
			
			m_strTime[i] = q.fieldValue("log_datatime");
			m_strTimeLen[i] = q.fieldValue("call_duration");
			//m_strName[i] = m_ItemRecord[i][0].chChar;

			m_Item[i].nState = BTN_STATE_NORMAL;
			q.nextRow();
		}
		else //空白行
		{
			m_strTimeLen[i]		= _T("");
			m_strTime[i]		= _T("");
			ItemStr[i]          = _T("");
			m_Item[i].chChar	= _T("");
			m_Item[i].nState	= BTN_STATE_NORMAL;
		}
	}

	q.finalize(); //释放statement
	db_808.close();
}