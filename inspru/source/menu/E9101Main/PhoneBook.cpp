// PhoneBook.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "PhoneBook.h"
//#include "DlgDial.h"

#include "PhoneBookMng.h"
//#include "DlgAddPhone.h"
//#include "DlgDial.h"
#include "E9101MainDlg.h"
#include "..\imm\IMMMng.h"
#include "DlgPhoneEdit.h"
#include "DlgPbookBrw.h"
#include "DlgConfirm.h"
#include "PhoneInfoMng.h"
#include "../BrightnessSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  staticListCount  5
#define  TID_READ_PBOOK    5010

extern BOOL g_bLight;

/////////////////////////////////////////////////////////////////////////////
// CPhoneBook dialog

CPhoneBook::CPhoneBook(CWnd* pParent /*=NULL*/)
	: CDialogBase(CPhoneBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ObArrary = NULL;
	m_Pos = 0;
	m_CurPage = 0;
	m_TotalPage = 0;
	CurPhoneLog = 0;
	m_PhoneNUll = TRUE;
	m_IsGetItem = FALSE;
	m_strSel = _T("");
}


void CPhoneBook::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneBook)
 	DDX_Control(pDX,IDC_BTN_EXIT, m_BtnExit);
 	DDX_Control(pDX,IDC_BTN_DEL_ALL, m_BtnDelAll);
	DDX_Control(pDX,IDC_BTN_PHONEBOOK1, m_BtnPhoneLog1);
 	DDX_Control(pDX,IDC_BTN_PHONEBOOK2, m_BtnPhoneLog2);
 	DDX_Control(pDX,IDC_BTN_PHONEBOOK3, m_BtnPhoneLog3);
 	DDX_Control(pDX,IDC_BTN_PHONEBOOK4, m_BtnPhoneLog4);
 	DDX_Control(pDX,IDC_BTN_PHONEBOOK5, m_BtnPhoneLog5);
	DDX_Control(pDX,IDC_BTN_UP, m_BtnPageUp);
	DDX_Control(pDX,IDC_BTN_DOWN, m_BtnPageDown);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhoneBook, CDialogBase)
	//{{AFX_MSG_MAP(CPhoneBook)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_DEL_ALL, OnBtnDelAll)
	ON_BN_CLICKED(IDC_BTN_PHONEBOOK1, OnBtnItem1)
	ON_BN_CLICKED(IDC_BTN_PHONEBOOK2, OnBtnItem2)
	ON_BN_CLICKED(IDC_BTN_PHONEBOOK3, OnBtnItem3)
	ON_BN_CLICKED(IDC_BTN_PHONEBOOK4, OnBtnItem4)
	ON_BN_CLICKED(IDC_BTN_PHONEBOOK5, OnBtnItem5)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnPageup)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnPagedown)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IMM_MESSAGE, OnIMMMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneLog message handlers

BOOL CPhoneBook::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGdi();
	InitControl();
	OnLanguageChange();
	OnSkinChange();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

	//变换语言和单位
BOOL CPhoneBook::OnLanguageChange()
{
	CResString str;
	str.LoadString(RES_BLUETOOTH_PHONEBOOK);
	m_stTitle = str;
	str.LoadString(RES_BUTTON_NEW);
	m_Item[6].chChar = str;


	str.LoadString(RES_BUTTON_OUT);
	m_Item[7].chChar = str;

	str.LoadString(RES_BUTTON_IN);
	m_Item[8].chChar = str;

	str.LoadString(RES_BUTTON_DEL);
	m_Item[9].chChar = str;

	return TRUE;
}

	//变皮肤
BOOL CPhoneBook::OnSkinChange()
{
	return TRUE;
}

void CPhoneBook::OnBtnExit() 
{
	EndDialog(0);
}

void CPhoneBook::OnBtnDelAll() 
{
	m_ObArrary = NULL;
	m_Pos = 0;
	m_CurPage = 0;
	m_TotalPage = 0;
	PhoneBookMng::Instance().DelAll();
	PostMessage(WM_ACTIVATE);
}

void CPhoneBook::OnBtnItem1() 
{

}

void CPhoneBook::OnBtnItem2() 
{
	// TODO: Add your control notification handler code here
	
}

void CPhoneBook::OnBtnItem3() 
{
	// TODO: Add your control notification handler code here


}

void CPhoneBook::OnBtnItem4() 
{
	// TODO: Add your control notification handler code here

}

void CPhoneBook::OnBtnItem5() 
{
	// TODO: Add your control notification handler code here
	
}

void CPhoneBook::OnBtnPageup() 
{
	// TODO: Add your control notification handler code here

	if(m_CurPage > 0)
	{
		m_Item[10].nState = BTN_STATE_NORMAL;
		m_CurPage--;
	}
	else
	{
		m_Item[5].nState = BTN_STATE_DISABLE;
		return;
	}

	if(m_CurPage == 0)
	{
		m_Item[5].nState = BTN_STATE_DISABLE;
	}
	int i=0;
	for(i=0; i<staticListCount; i++)
	{
		ItemStrName[i] = _T("");
	}
	for(int k=0; k<staticListCount; k++)
	{
		ItemStrNum[k] = _T("");
	}

	for(int j=0;j<staticListCount;j++)
	{
		m_Item[j].nState = BTN_STATE_NORMAL;
	}
	

	CPhoneBookFile* pPhonelogItem = NULL;
	UINT TempPos = 0;


	for( i=0;i<staticListCount; i++)
	{
		//m_Pos -= 1;
		 pPhonelogItem = (CPhoneBookFile*)m_ObArrary->GetAt(m_CurPage*staticListCount+i);
		 switch(i)
		 {
		 case 0:
			 {
							ItemStrNum[0] = pPhonelogItem->m_csNum;
							ItemStrName[0] = pPhonelogItem->m_csName;
							//ItemStrCity[0] = pPhonelogItem->m_csCity;
			 }
			 break;
		 case 1:
			 {
 							ItemStrNum[1] = pPhonelogItem->m_csNum;
							ItemStrName[1] = pPhonelogItem->m_csName;
							//ItemStrCity[i] = pPhonelogItem->m_csCity;
			 }
			 break;
		 case 2:
			 {
							ItemStrNum[2] = pPhonelogItem->m_csNum;
							ItemStrName[2] = pPhonelogItem->m_csName;
							//ItemStrCity[i] = pPhonelogItem->m_csCity;
			 }
			 break;
		 case 3:
			 {
							ItemStrNum[3] = pPhonelogItem->m_csNum;
							ItemStrName[3] = pPhonelogItem->m_csName;
							//ItemStrCity[i] = pPhonelogItem->m_csCity;
			 }
			 break;
		 case 4:
			 {
							ItemStrNum[4] = pPhonelogItem->m_csNum;
 							ItemStrName[4] = pPhonelogItem->m_csName;
							//ItemStrCity[i] = pPhonelogItem->m_csCity;
			 }
			 break;
		 default:
			 break;
		 }
	 	 
	}

	Invalidate();	
}

void CPhoneBook::OnBtnPagedown() 
{
	// TODO: Add your control notification handler code here

	if(m_CurPage < m_TotalPage - 1)
	{
		m_Item[5].nState = BTN_STATE_NORMAL;
		m_CurPage++;
	}
	else
	{
		m_Item[10].nState = BTN_STATE_DISABLE;
		return;
	}

	if(m_CurPage == m_TotalPage - 1)
	{
		m_Item[10].nState = BTN_STATE_DISABLE;
	}
	int i=0;
	for(i=0; i<staticListCount; i++)
	{
		ItemStrName[i] = _T("");
	}
	for(int k=0; k<staticListCount; k++)
	{
		ItemStrNum[k] = _T("");
	}
	for(int j=0;j<staticListCount;j++)
	{
		m_Item[j].nState = BTN_STATE_NORMAL;
	}
	
	
	CPhoneBookFile* pPhoneLogItem = NULL;
	
	UINT TempPos = 0;
	int tempValue;

	if(m_CurPage == m_TotalPage-1)
	{
			UINT nCount = PhoneBookMng::Instance().GetCount();
			if(nCount%staticListCount == 0)
				tempValue = staticListCount;
			else
				tempValue = nCount%staticListCount;
	}
	else
		tempValue = staticListCount;

	for(i=0;i<tempValue;i++)
	{
		 pPhoneLogItem = (CPhoneBookFile*)m_ObArrary->GetAt(m_CurPage*staticListCount+i);
		 switch(i)
		 {
		 case 0:
			 {
							ItemStrNum[0] = pPhoneLogItem->m_csNum;
							ItemStrName[0] = pPhoneLogItem->m_csName;
//							ItemStrCity[i] = pPhoneLogItem->m_csCity;
			 }
			 break;
		 case 1:
			 {
							ItemStrNum[1] = pPhoneLogItem->m_csNum;
							ItemStrName[1] = pPhoneLogItem->m_csName;
//							ItemStrCity[i] = pPhoneLogItem->m_csCity;
			 }
			 break;
		 case 2:
			 {
							ItemStrNum[2] = pPhoneLogItem->m_csNum;
							ItemStrName[2] = pPhoneLogItem->m_csName;
//							ItemStrCity[i] = pPhoneLogItem->m_csCity;
			 }
			 break;
		 case 3:
			 {
							ItemStrNum[3] = pPhoneLogItem->m_csNum;
							ItemStrName[3] = pPhoneLogItem->m_csName;
//							ItemStrCity[i] = pPhoneLogItem->m_csCity;
			 }
			 break;
		 case 4:
			 {
								ItemStrNum[4] = pPhoneLogItem->m_csNum;
								ItemStrName[4] = pPhoneLogItem->m_csName;
//								ItemStrCity[i] = pPhoneLogItem->m_csCity;
			 }
			 break;
		 default:
			 break;
		 }

	}

	 Invalidate();
}

LRESULT CPhoneBook::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnBtnExit();
		}
	}
	else if( WM_ACTIVATE == message )
	{
		for(int i=0; i<staticListCount; i++)
		{
			ItemStrName[i] = _T("");
		}
		for(int k=0; k<staticListCount; k++)
		{
			ItemStrNum[k] = _T("");
		}
	//	for( k=0; k<staticListCount; k++)
	//	{
	//		ItemStrCity[k] = _T("");
	//	return 0;
	//	}
//		m_BtnPhoneLog1.SetWindowText(_T(""));
//		m_BtnPhoneLog2.SetWindowText(_T(""));
//		m_BtnPhoneLog3.SetWindowText(_T(""));
//		m_BtnPhoneLog4.SetWindowText(_T(""));
//		m_BtnPhoneLog5.SetWindowText(_T(""));
//		m_BtnPhoneLog1.SetBtnResID(bmp_file_BtnDiaOut);
//		CDialLog* pLogItem = (CDialLog*)(PhoneLogMng::Instance().GetPhoneLog()->GetAt(0));
		
//		m_BtnPhoneLog1.SetWindowText(pLogItem->m_csName);
		m_Pos = 0;
		m_CurPage = 0;
//		m_TotalPage = 0;
		CurPhoneLog = 0;
//		m_PhoneNUll = TRUE;
//		m_BtnPageUp.EnableWindow(FALSE);
//		m_BtnPageDown.EnableWindow(TRUE);

		m_Item[5].nState = BTN_STATE_DISABLE;
		m_Item[10].nState = BTN_STATE_DISABLE;
		
		if( !PhoneBookMng::Instance().Load() )
		{
			m_Item[10].nState = BTN_STATE_DISABLE;
		}
		else
		{
			m_ObArrary = PhoneBookMng::Instance().GetPhoneBook();

			UINT nCount = PhoneBookMng::Instance().GetCount();
			m_TotalPage = nCount/staticListCount + ( (nCount%staticListCount)==0?0:1 );
			if(nCount <= 5)
				m_Item[10].nState = BTN_STATE_DISABLE;
			else
				m_Item[10].nState = BTN_STATE_NORMAL;
			CPhoneBookFile* pPhoneLogItem = NULL;
			
			UINT TempPos;

			for(TempPos=0;TempPos<staticListCount/*&&TempPos <PhoneBookMng::Instance().GetCount()*/;)
			{
				if((m_CurPage*staticListCount+TempPos) > (nCount-1))
					break;
				 pPhoneLogItem = (CPhoneBookFile*)m_ObArrary->GetAt((m_CurPage)*staticListCount+TempPos);

				 switch(TempPos)
				 {
				 case 0:
					 {
							ItemStrNum[0] = pPhoneLogItem->m_csNum;
							ItemStrName[0] = pPhoneLogItem->m_csName;
					 }
					 break;
				 case 1:
					 {
							ItemStrNum[1] = pPhoneLogItem->m_csNum;
							ItemStrName[1] = pPhoneLogItem->m_csName;
					 }
					 break;
				 case 2:
					 {
							ItemStrNum[2] = pPhoneLogItem->m_csNum;
							ItemStrName[2] = pPhoneLogItem->m_csName;
					 }
					 break;
				 case 3:
					 {
							ItemStrNum[3] = pPhoneLogItem->m_csNum;
							ItemStrName[3] = pPhoneLogItem->m_csName;
					 }
					 break;
				 case 4:
					 {
							ItemStrNum[4] = pPhoneLogItem->m_csNum;
							ItemStrName[4] = pPhoneLogItem->m_csName;
					 }
					 break;
				 default:
					 break;
				 }
	 			 TempPos++;
			}
			//m_Pos = 0;
		}
	}
	
	return CDialogBase::DefWindowProc(message, wParam, lParam);
}


void CPhoneBook::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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



void CPhoneBook::InitControl()
{
	for (int i =0;  i < 5; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
		RECT rt;
		rt.left   = 15;
		rt.top   = 46+i*43;
		rt.right = rt.left + 388;
		rt.bottom = rt.top + 43;
		m_Item[i].rect = rt;
		
	}


	for (int j=5;  j < 11; j++)
	{

		m_Item[j].nState = BTN_STATE_NORMAL;
		RECT rt;
		rt.left   = 411;
		rt.top   = 46+(j-5)*36;
		rt.right = 455;
		rt.bottom = rt.top + 36;
		m_Item[j].rect = rt;
		
	}
	
	m_Item[5].nState = BTN_STATE_DISABLE;
	m_Item[10].nState = BTN_STATE_DISABLE;

		m_Item[11].nState = BTN_STATE_NORMAL;
		RECT rt;
		rt.left   = 430;
		rt.top   = 0;
		rt.right = 480;
		rt.bottom = 40;
		m_Item[11].rect = rt;

}


void CPhoneBook::InitGdi()
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
	 		-8,					    // nHeight
	 		0,                         // nWidth
 			0,                         // nEscapement
	 		0,                         // nOrientation
	 		FW_THIN,                 // nWeight
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
// 	else if( CSysConfig::Instance().GetLanguage() == ID_PORTUGAL  )
// 	{
// 	 	smallfont.CreateFont(
// 	 		-12,					    // nHeight
// 	 		0,                         // nWidth
//  			0,                         // nEscapement
// 	 		0,                         // nOrientation
// 	 		FW_LIGHT,                 // nWeight
// 	 		FALSE,                     // bItalic
// 	 		FALSE,                     // bUnderline
// 	 		0,                         // cStrikeOut
// 	 		ANSI_CHARSET,              // nCharSet
// 	 		OUT_DEFAULT_PRECIS,        // nOutPrecision
// 	 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 	 		ANTIALIASED_QUALITY,           // nQuality
// 	 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 	 		_T("宋体\0"));             // lpszFacename
// 
//	}
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

		midfont.CreateFont(
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

		//
	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	bk_dis_dc.CreateCompatibleDC(m_pDC);
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_PHONEBOOK_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_BG_PHONEBOOK_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;  
	
 	pSkin = g_pResMng->RequestBmp(BMP_BG_PHONEBOOK_D, true);
 	m_stBtnDisableBMP = pSkin->hBitmap;  

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
 	m_pold_bk_disable_bmp    = bk_dis_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}


void CPhoneBook::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<11;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
	}


	CRect temprect;
	int j=0;
	for(j=0; j<5; j++)
	{
		temprect.top = m_Item[j].rect.top + 5;
		temprect.left = m_Item[j].rect.left + 15;
		temprect.right = m_Item[j].rect.right;
		temprect.bottom = m_Item[j].rect.bottom;
		DrawBtnText(&memDC,temprect,ItemStrName[j],midfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);

		temprect.top = m_Item[j].rect.top + 5;
		temprect.left = m_Item[j].rect.left + 215;
		temprect.right = m_Item[j].rect.right -15;
		temprect.bottom = m_Item[j].rect.bottom;
		DrawBtnText(&memDC,temprect,ItemStrNum[j],midfont,RGB(255,255,255),DT_RIGHT | DT_VCENTER);

	}

	for( j=6; j<10; j++ )
	{
		temprect = m_Item[j].rect;
		DrawBtnText(&memDC,temprect,m_Item[j].chChar,smallfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	}
	
	DrawBtnText(&memDC,g_retTitile,m_stTitle,bigfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

	
	// Do not call CDialogBase::OnPaint() for painting messages
}

void CPhoneBook::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CPhoneBook::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int j=0; j<5; j++)
	{	
//		if(PtInRect(&m_Item[j].rect,point))	
//		{
			if(m_Item[j].nState==BTN_STATE_DOWN)
			{
				m_Item[j].nState = BTN_STATE_NORMAL;
				switch(j)
					{

					case 0:
						m_CurListPos = 0;
						OnStaticList(0);
						break;
					case 1:
						m_CurListPos = 1;
						OnStaticList(1);
						break;
					case 2:
						m_CurListPos = 2;
						OnStaticList(2);
						break;
					case 3:
						m_CurListPos = 3;
						OnStaticList(3);
						break;
 					case 4:
 						m_CurListPos = 3;
						OnStaticList(4);
						break;
					}
			}
//		}

	}

	for(int i=5;i<12;i++)
	{
//		if(PtInRect(&m_Item[i].rect,point) /*&& m_Item[i].nState==BTN_STATE_DOWN*/)
//		{
			if(m_Item[i].nState==BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
			//m_Item[i].nState = BTN_STATE_NORMAL;
				switch(i)
				{
				case 5:
					OnBtnPageup(); 
					break;
				case 6:
					OnAddPhone();
					break;
				case 7:
					OnOutBook();
					break;
				case 8:
					OnInBook();
					break;
				case 9:
					{
						CResString str;
						str.LoadString( RES_BUTTON_DEL_ALL_PB );
						
						CDlgConfirm::m_stTitle = str;
						CDlgConfirm dlg;
						dlg.DoModal();

						if( CDlgConfirm::s_bOk )
						{
							OnBtnDelAll(); 
						}
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
//		}
	}

	
	Invalidate();
	CDialogBase::OnLButtonUp(nFlags, point);
}
void CPhoneBook::OnStaticList(UINT nId)
{
//	CString str;
	m_PhoneNUll = FALSE;
	if(ItemStrName[nId] == _T("") && ItemStrNum[nId] == _T(""))
		m_PhoneNUll = TRUE;

	m_Pos = m_CurPage*staticListCount + nId;

	switch(nId)
	{
		case 0:
			m_Item[1].nState = BTN_STATE_NORMAL;
			m_Item[2].nState = BTN_STATE_NORMAL;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[4].nState = BTN_STATE_NORMAL;
			break;

		case 1:
			m_Item[0].nState = BTN_STATE_NORMAL;
			m_Item[2].nState = BTN_STATE_NORMAL;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[4].nState = BTN_STATE_NORMAL;
			break;

		case 2:
			m_Item[0].nState = BTN_STATE_NORMAL;
			m_Item[1].nState = BTN_STATE_NORMAL;
			m_Item[3].nState = BTN_STATE_NORMAL;
			m_Item[4].nState = BTN_STATE_NORMAL;
			break;
		case 3:
			m_Item[0].nState = BTN_STATE_NORMAL;
			m_Item[1].nState = BTN_STATE_NORMAL;
			m_Item[2].nState = BTN_STATE_NORMAL;
			m_Item[4].nState = BTN_STATE_NORMAL;
			break;
 		case 4:
 			m_Item[0].nState = BTN_STATE_NORMAL;
 			m_Item[1].nState = BTN_STATE_NORMAL;
 			m_Item[2].nState = BTN_STATE_NORMAL;
 			m_Item[3].nState = BTN_STATE_NORMAL;
		break;

		default:
			break;
	}

	Invalidate();



	if(m_PhoneNUll)
		return;
	if ( m_IsGetItem )
	{
		m_strSel = ItemStrNum[nId];
		OnExit();
	}
	else
	{
		CDlgPhoneEdit dlg;
		dlg.SetPrevHwnd(this);
		CDlgPhoneEdit::m_PhoneName = ItemStrName[nId];
		CDlgPhoneEdit::m_PhoneNum = ItemStrNum[nId];
		//	CDlgPhoneEdit::m_PhoneCity = ItemStrCity[nId];
		CDlgPhoneEdit::m_PhonePos = m_Pos;
		CDlgPhoneEdit::m_AddPhone = FALSE;
		dlg.DoModal();
	}


}
void CPhoneBook::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_dis_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CPhoneBook::OnOutBook()
{
	CResString str;
	str.LoadString( RES_BUTTON_OUT );
	CResString str1;
	str1.LoadString(RES_BLUETOOTH_PHONEBOOK);

	CDlgConfirm::m_stTitle = str+str1;
	
	CDlgConfirm dlg;
	dlg.DoModal();

	CFile PBookFile;
	char strnum[MAX_PATH];
	char Tail[2]= {0x0d,0x0a};
	CString name;
	CString num;
	CString city;
	if( CDlgConfirm::s_bOk )
	{
		::DeleteFile( _T("\\SDMMC DISK\\PhoneBook.txt") );
		if( !PBookFile.Open(_T("\\SDMMC DISK\\PhoneBook.txt"),
				CFile::modeCreate | CFile::modeReadWrite ,NULL) )
				return;

		CPhoneBookFile* PhoneBook = NULL;
		for ( int i =0; i<PhoneBookMng::Instance().GetCount();i++ )
		{
			PhoneBook =  PhoneBookMng::Instance().GetPhoneBookItem( i );
			
			wcstombs(strnum, (LPCTSTR)PhoneBook->m_csName, PhoneBook->m_csName.GetLength()*2);
			strnum[PhoneBook->m_csName.GetLength()*2] = '\0';
			PBookFile.Write(strnum,strlen(strnum));
			PBookFile.Write("  ",2);
			wcstombs(strnum, (LPCTSTR)PhoneBook->m_csNum, PhoneBook->m_csNum.GetLength()*2);
			strnum[PhoneBook->m_csNum.GetLength()*2] = '\0';
			PBookFile.Write(strnum,strlen(strnum));
			PBookFile.Write(Tail,2);
		}
		PBookFile.Close();
	}
}

void CPhoneBook::OnBtnEditPhoneName() 
{
	if(m_PhoneNUll)
		return;
	tempItemStr = ItemStrName[m_CurListPos];

	ImmReturn ret;
	IMMRESULT res;
	CString strName;
	strName = _T("");

	enSkinType enSkin = CSysConfig::Instance().GetSkinType();
	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	
	if(CIMMMng::Instance().Load() == TRUE)
	{
		CIMMMng::Instance().ImmSetKeySound( g_enKeySnd );
		CIMMMng::Instance().IMMInit(FourPoint3);
        ret = CIMMMng::Instance().IMMEditPhone(ID_ENGLISH,(LPTSTR)(LPCTSTR)strName, g_enKeySnd,enSkin);
	}

	res = CIMMMng::Instance().IMMGetResult();


	if(res.strName == _T(""))
	{

	}
	else
	{
		ItemStrName[m_CurListPos] = res.strName;	
		CPhoneBookFile* pPhoneBookItem = NULL;

		PhoneBookMng::Instance().Load();
		pPhoneBookItem = (CPhoneBookFile*)m_ObArrary->GetAt(m_Pos); 			

 		pPhoneBookItem->m_csName = ItemStrName[m_CurListPos];

 		PhoneBookMng::Instance().SetCurPhoneBookItem(m_Pos);			
 		PhoneBookMng::Instance().ReSetCurItem(pPhoneBookItem);
 		PhoneBookMng::Instance().Write();
		PostMessage(WM_ACTIVATE);
	}
	Invalidate();
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();
}

LRESULT CPhoneBook::OnIMMMsg(WPARAM wParam , LPARAM lParam)
{
	CString str;
	str.Format(_T("%d"), wParam);

	switch(wParam)
	{
		case 0:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 1:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 2:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 3:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 4:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 5:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 6:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 7:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 8:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 9:
			ItemStrNum[m_CurListPos] += str;
			break;
		case 10:
			ItemStrNum[m_CurListPos] += _T("*");
			break;
		case 11:
			ItemStrNum[m_CurListPos] += _T("#");
			break;
		case 12:
			ItemStrNum[m_CurListPos] = ItemStrNum[m_CurListPos].Left(ItemStrNum[m_CurListPos].GetLength()-1);
			break;
		case 13:
			//ItemStrNum[m_CurListPos] = tempItemStr;//cancel
			break;
		case 14:
			CPhoneBookFile* pPhoneBookItem = NULL;

			PhoneBookMng::Instance().Load();
			pPhoneBookItem = (CPhoneBookFile*)m_ObArrary->GetAt(m_Pos); 			

 			pPhoneBookItem->m_csNum = ItemStrNum[m_CurListPos];

 			PhoneBookMng::Instance().SetCurPhoneBookItem(m_Pos);			
 			PhoneBookMng::Instance().ReSetCurItem(pPhoneBookItem);
 			PhoneBookMng::Instance().Write();

			PostMessage(WM_ACTIVATE);
			//ItemStr[m_CurListPos]  += _T("ok");
			break;
	}

	Invalidate();
	return 1;
}

void CPhoneBook::OnInBook() 
{
	CDlgPbookBrw dlg;
	dlg.DoModal();
	if( CDlgPbookBrw::s_bOk )
	{
		CResString str;
		str.LoadString(RES_BUTTON_IN);
		CResString str1;
		str1.LoadString(RES_BLUETOOTH_PHONEBOOK);

		CDlgConfirm dlg;
		CDlgConfirm::m_stTitle = str + str1;
		dlg.DoModal();

		if( CDlgConfirm::s_bOk )
		{
			ReadIniFile( CDlgPbookBrw::s_csPath );
			PostMessage(WM_ACTIVATE);
		}
	}
 }
 
void CPhoneBook::OnAddPhone()
{
	CDlgPhoneEdit dlg;
	dlg.SetPrevHwnd(this);
	CDlgPhoneEdit::m_PhoneName = _T("");
	CDlgPhoneEdit::m_PhoneNum = _T("");
	CDlgPhoneEdit::m_PhoneCity = _T("");
	CDlgPhoneEdit::m_AddPhone = TRUE;
	dlg.DoModal();
}

void CPhoneBook::OnExit()
{
	bk_dis_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_dis_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();
	midfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	if ( m_IsGetItem )
		CDialogBase::EndDialog(IDOK);
	else
		CDialogBase::EndDialog(IDCANCEL);

}
BOOL CPhoneBook::ReadOneString(char* pIn, ULONGLONG iInLen, char*& pOut, ULONGLONG &iOutLen)
{

	if(m_iPos >= iInLen)
		return FALSE;

	while(pIn[m_iPos] == 0x000d && pIn[m_iPos+1] == 0x000a)
		m_iPos += 2;

	iOutLen = 0;


	while(m_iPos < iInLen)
	{

		if(pIn[m_iPos] != 0x000d)
		{
			pOut[iOutLen] = pIn[m_iPos];
			iOutLen++;
			m_iPos++;
		}
		else
		{
			m_iPos = m_iPos+2;
			break;
		}
	}
	return TRUE;
}

BOOL CPhoneBook::ReadIniFile( CString path )
{
	PhoneBookMng::Instance().DelAll();

	SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);
	BOOL bFileExist = FALSE;

	CFileException e;
	TRY
	{
		bFileExist=m_IniFile.Open(path, 
				CFile::modeReadWrite ,&e);

	}
	CATCH(CFileException,e)
	{
		bFileExist = FALSE;
		TRACE0("打开文件错误！");
		return FALSE;
	}
	END_CATCH

	if(bFileExist == 0)
		return FALSE;

	m_iPos = 0;
	CString strline;

	m_IniFile.SeekToBegin();
	ULONGLONG buflen = m_IniFile.GetLength() + 1;
	if((buflen-1) == 0)
		return FALSE;
	char* pFileBuf = new char[buflen];
	memset(pFileBuf,0,buflen);

	buflen = m_IniFile.Read(pFileBuf,buflen);


	buflen = buflen;

	BOOL bReadState = TRUE;
	int pos = 0;
	CString Num;
	CString City;
	CString OneStr;
	char* OutBuf = new char[buflen];
	char* pOutBuf = OutBuf;
	ULONGLONG OutLen = 0;
	CPhoneBookFile pBook;


	while(bReadState)
	{

		bReadState = ReadOneString(pFileBuf,buflen,pOutBuf,OutLen);
		if (bReadState && OutLen!=0)
		{
			PhoneBookMng::Instance().Load();	
			pOutBuf[OutLen] = '\0';
			OneStr = pOutBuf;
			Num = OneStr;
			pos = OneStr.Find(' ',0);
			OneStr.Delete( 0, pos);
			OneStr.TrimLeft();
			OneStr.TrimRight();
			Num.Delete( pos ,Num.GetLength() - pos);
			Num.TrimLeft();
			Num.TrimRight();
			City = PhoneInfoMng::Instance().FindPhoneNum(OneStr);
			pBook.m_csName = Num;
			pBook.m_csNum = OneStr;
			pBook.m_csCity = City;

			PhoneBookMng::Instance().AddPhone( &pBook );

		}
	}

	if(OutBuf != NULL)
	{
		delete []OutBuf;
		OutBuf = NULL;
	}
	
	if(pFileBuf != NULL)
	{
		delete []pFileBuf;
		pFileBuf = NULL;
	}

	m_IniFile.Close();

	return TRUE;
}

void CPhoneBook::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TID_READ_PBOOK )
	{
		KillTimer( TID_READ_PBOOK );
		g_GsmLogic.GSMReadSIMPBook();
	}
	CDialogBase::OnTimer(nIDEvent);
}

BOOL CPhoneBook::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == UMSG_GSM_CMD )
	{
		if ( pMsg->wParam ==  GSM_SIM_PBOOK_READ )
		{
			Invalidate();
		}
	}
	return CDialogBase::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////

int   CPhoneBook::GetMaxRecord()
{
	return 1;
}
