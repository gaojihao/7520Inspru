// SMSNew.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgFileDownLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog

CDlgFileDownLoad::CDlgFileDownLoad(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgFileDownLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pItemList = NULL;
	m_bIsCopyFile = FALSE;
}

void CDlgFileDownLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileDownLoad, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDlgFileDownLoad::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	LoadDir(PATH_SOURCE);
	return TRUE;
}
//变换语言和单位
BOOL CDlgFileDownLoad::OnLanguageChange()//变皮肤
{
	CResString str;
	
	//str.LoadString(RES_BUTTON_SMS_INBOX);
	m_csTitile = _T("文件下载");
	
	return TRUE;
}

BOOL CDlgFileDownLoad::OnSkinChange()
{
	return TRUE;
}


void CDlgFileDownLoad::InitGdi()
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
	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_FILE_DOWNLOAD_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_FILE_DOWNLOAD_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgFileDownLoad::ReleaseGdi()
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

void CDlgFileDownLoad::OnExit()
{
	ReleaseGdi();
	delete[] m_pItemList;
	CDialogBase::EndDialog(0);
}

void CDlgFileDownLoad::InitControl()
{
	for (int i =0;  i < enItemCount; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	//下载
	m_Item[0].rect.left		= 379;
	m_Item[0].rect.top		= 416;
	m_Item[0].rect.right	= 504;
	m_Item[0].rect.bottom	= 472;
	m_Item[0].chChar		= _T("下载文件");

	//上一页
	m_Item[1].rect.left		= 531;
	m_Item[1].rect.top		= 416;
	m_Item[1].rect.right	= 611;
	m_Item[1].rect.bottom	= 475;

	//下一页
	m_Item[2].rect.left		= 660;
	m_Item[2].rect.top		= 413;
	m_Item[2].rect.right	= 741;
	m_Item[2].rect.bottom	= 476;

	//Exit
	m_Item[3].rect.left		= 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 65;

	//文件列表
	m_ItemList[0].rect.left		= 40;
	m_ItemList[0].rect.top		= 144;
	m_ItemList[0].rect.right	= 40+700;
	m_ItemList[0].rect.bottom	= 144+48;

	m_ItemList[1].rect.left		= 40;
	m_ItemList[1].rect.top		= 201;
	m_ItemList[1].rect.right	= 40+700;
	m_ItemList[1].rect.bottom	= 201+48;

	m_ItemList[2].rect.left		= 40;
	m_ItemList[2].rect.top		= 258;
	m_ItemList[2].rect.right	= 40+700;
	m_ItemList[2].rect.bottom	= 258+48;

	m_ItemList[3].rect.left		= 40;
	m_ItemList[3].rect.top		= 314;
	m_ItemList[3].rect.right	= 40+700;
	m_ItemList[3].rect.bottom	= 314+48;

	m_ItemList[4].rect.left		= 40;
	m_ItemList[4].rect.top		= 368;
	m_ItemList[4].rect.right	= 40+700;
	m_ItemList[4].rect.bottom	= 368+37;
}

void CDlgFileDownLoad::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgFileDownLoad::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgFileDownLoad::OnPaint() 
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
	
	for(i=0;i<enItemCount;i++)
	{
		str		= m_Item[i].chChar;
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,smallfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}

	for(i=(m_nCurPage-1)*enListCount;i<m_nCurPage*enListCount;i++)
	{
		if (i > m_FileCnt-1)
			break;
		str		= m_pItemList[i].chChar;
		Rect	= m_pItemList[i].rect;
		state	= m_pItemList[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.left = Rect.left + 100;
		DrawBtnText(&memDC,Rect,str,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	}
	str.Format(_T("第 %d/%d 页"),m_nCurPage,m_nTotalPage);
	DrawBtnText(&memDC,CRect(233,412,342,468),str,bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	if (m_bIsCopyFile)
	{
		str.Format(_T("传输 %d%"),(m_nCurCopyFileIndex+1)/m_FileCnt);
		DrawBtnText(&memDC,CRect(30,412,90,468),str,bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}
	//标题
	DrawBtnText(&memDC,g_retTitile,m_csTitile,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	str = _T("选择");
	DrawBtnText(&memDC,CRect(39,91,39+83,91+45),str,bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	str = _T("文件名");
	DrawBtnText(&memDC,CRect(138,91,138+580,91+45),str,bigfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgFileDownLoad::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i=0;
	for(i=0;i<enItemCount;i++)
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
	for(i=(m_nCurPage-1)*enListCount;i<m_nCurPage*enListCount;i++)
	{
		if (i > m_FileCnt-1)
			break;
		if(PtInRect(&m_pItemList[i].rect,point))
		{
			if(m_pItemList[i].nState == BTN_STATE_NORMAL)
				m_pItemList[i].nState = BTN_STATE_DOWN;
			else
				m_pItemList[i].nState = BTN_STATE_NORMAL;
			PlayKeySound();
		}
	}
	
	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgFileDownLoad::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<enItemCount;i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0:
				//if(!m_bIsCopyFile)
				//	CreateThread(NULL, 0, THDDownLoad, this, 0, NULL);
				THDDownLoad(this);
				break;
			case 1:
				{
					OnPageUp();
				}
				break;
			case 2:
				{
					OnPageDown();
				}
				break;
			case 3: 
				OnExit();
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

void CDlgFileDownLoad::OnPageUp()
{
	m_nCurPage--;
	if (m_nCurPage == 0)
		m_nCurPage = 1;

	int j = 0;
	for( int i = (m_nCurPage-1)*enListCount;  i < (m_nCurPage*enListCount); i++ )
	{
		m_ItemList[j].nState = m_pItemList[i].nState;
		if(!m_pItemList[i].chChar.IsEmpty())
			m_ItemList[j].chChar = m_pItemList[i].chChar;
		m_ItemList[j].nListType = m_pItemList[i].nListType;
		j++;
	}
}
void CDlgFileDownLoad::OnPageDown()
{
	m_nCurPage++;
	if (m_nCurPage > m_nTotalPage)
		m_nCurPage = m_nTotalPage;

	int j = 0;
	for( int i = (m_nCurPage-1)*enListCount;  i < (m_nCurPage*enListCount); i++ )
	{
		if (i > m_FileCnt-1)
			break;
		m_ItemList[j].nState = m_pItemList[i].nState;
		m_ItemList[j].chChar = m_pItemList[i].chChar;
		m_ItemList[j].nListType = m_pItemList[i].nListType;
		j++;
	}
}

void CDlgFileDownLoad::LoadDir(CString str)
{
	if( m_pItemList != NULL )
	{
		delete[] m_pItemList;
		m_pItemList = NULL;
	}
	m_dirref.Load(str,0,_T(".vdu"));
	m_FileCnt = m_dirref.ReadFileCnt();

	if(m_FileCnt > 0)
		m_nTotalPage = (m_FileCnt%enListCount>0)?(m_FileCnt/enListCount + 1):m_FileCnt/enListCount;
	else
		m_nTotalPage = 0;
	
	m_nCurPage = 1;
	m_pItemList = new  ItemListInfo[m_FileCnt];
	memset(m_pItemList,0,sizeof(m_pItemList));

	int x = 0;
	for(int i=0;i<m_FileCnt;i++)
	{
		m_pItemList[i].chChar = m_dirref.ReadOneFileName(i);
		m_pItemList[i].nState =BTN_STATE_NORMAL;
		m_pItemList[i].nListType = LIST_FILE;
		m_pItemList[i].rect = m_ItemList[x++].rect;
		if (x == 5)
			x = 0;
	}
}

DWORD CDlgFileDownLoad::THDDownLoad(LPVOID lpParameter)
{
	CDlgFileDownLoad* pDlg = (CDlgFileDownLoad*)lpParameter;
	pDlg->m_bIsCopyFile = TRUE;
	
	CString str1,str2;
	
	for (pDlg->m_nCurCopyFileIndex = 0; pDlg->m_nCurCopyFileIndex < pDlg->m_FileCnt; pDlg->m_nCurCopyFileIndex++)
	{
		if (pDlg->m_pItemList[pDlg->m_nCurCopyFileIndex].nState == BTN_STATE_DOWN)
		{
			//str1 = PATH_SOURCE+_T("\\");
			str1 = _T("\\Flashdrv Storage\\LogoFile\\") + pDlg->m_pItemList[pDlg->m_nCurCopyFileIndex].chChar;
			str2 = _T("\\Storage Card\\") + pDlg->m_pItemList[pDlg->m_nCurCopyFileIndex].chChar;
			SetFileAttributes(str2, FILE_ATTRIBUTE_NORMAL);
			DeleteFile(str2);
			BOOL bRtn = CopyFile(str1,str2, FALSE );
			pDlg->Invalidate();
		}
	}
	pDlg->m_bIsCopyFile = FALSE;

	return 1;
}

