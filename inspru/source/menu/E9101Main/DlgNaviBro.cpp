// DlgNaviBro.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgNaviBro.h"

#include "../MutiLanguage/CGDICommon.h"
#include <dbt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNaviBro dialog


CDlgNaviBro::CDlgNaviBro(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgNaviBro::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNaviBro)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_DirCnt = 0;
	m_FileCnt = 0;
	m_PageCnt = 1;
	m_iCurPage = 1;
	m_iListCnt = 0;
	m_bType = TRUE;
	m_pItemList = NULL;
	m_strRootDir	= PATH_SD;
}


void CDlgNaviBro::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNaviBro)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNaviBro, CDialogBase)
	//{{AFX_MSG_MAP(CDlgNaviBro)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNaviBro message handlers

BOOL CDlgNaviBro::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	// TODO: Add extra initialization here


	InitGdi();
	InitControl();
	LoadDir(m_strRootDir);

	OnLanguageChange();
	OnSkinChange();

	return TRUE;
}

BOOL CDlgNaviBro::OnLanguageChange()
{
	m_csPath= m_strRootDir;

	CResString str;
	str.LoadString(RES_BTN_NAVI_PATH);
	m_csTitile = str;

	return TRUE;
}

//变皮肤
BOOL CDlgNaviBro::OnSkinChange()
{
	return TRUE;
}

void CDlgNaviBro::LoadDir(CString str)
{

	if( m_pItemList != NULL )
	{
		delete[] m_pItemList;
		m_pItemList = NULL;
	}

	int i;
	int less = 0;

	m_dirref.Load(str,0,_T(".exe"));

	m_DirCnt = m_dirref.ReadDirCnt();
	m_FileCnt = m_dirref.ReadFileCnt();


	m_iListCnt = ( m_DirCnt + m_FileCnt )%8;

	if( m_iListCnt > 0 )
	{
		m_iListCnt = (( m_DirCnt + m_FileCnt )/8 + 1)*8;
	}
	else
	{
		m_iListCnt = m_DirCnt + m_FileCnt;
	}

	m_PageCnt = m_iListCnt/8;
	m_iCurPage = 1;
	m_Item[0].nState = BTN_STATE_DISABLE;
	if(m_PageCnt == 1)
		m_Item[1].nState = BTN_STATE_DISABLE;
	else
		m_Item[1].nState = BTN_STATE_NORMAL;

	m_pItemList = new  ItemListInfo[m_iListCnt];
	memset(m_pItemList,0,sizeof(m_pItemList));

	int x = 0;
	for(i=0;i<m_iListCnt;i++)
	{
		if( i<m_DirCnt )
		{
			if( i != 0)
				m_pItemList[i].chChar = m_dirref.ReadOneDirName(i);
			else
				m_pItemList[i].chChar = _T("..");
			m_pItemList[i].nState =BTN_STATE_NORMAL;
			m_pItemList[i].nListType = LIST_DIR;
			//		less = i%8;
			//		m_pItemList[i].rect = m_ItemList[less].rect;
		}
		else if( i<m_DirCnt+m_FileCnt )
		{
			m_pItemList[i].chChar = m_dirref.ReadOneFileName(x);
			x++;
			m_pItemList[i].nState =BTN_STATE_NORMAL;
			m_pItemList[i].nListType = LIST_FILE;
		}
		else if( i<m_iListCnt )
		{
			m_pItemList[i].chChar = _T("");
			m_pItemList[i].nState =BTN_STATE_DISABLE;
			m_pItemList[i].nListType = LIST_NONE;
		}
	}

	for(i =0;  i < 8; i++)
	{
		m_ItemList[i].nState = m_pItemList[i].nState;
		m_ItemList[i].chChar = m_pItemList[i].chChar;
		m_ItemList[i].nListType = m_pItemList[i].nListType;
	}


}

void CDlgNaviBro::InitGdi()
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
			16,					    // nHeight
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

	bk_list_dc.CreateCompatibleDC(m_pDC);


	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(SKIN_BG_NAVIBRO_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(SKIN_BG_NAVIBRO_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 

	pSkin = g_pResMng->RequestBmp(SKIN_BG_NAVIBRO_D, true);
	m_stBtnDisableBMP = pSkin->hBitmap; 

	pSkin = g_pResMng->RequestBmp(SKIN_BTN_BROWER, true);
	m_stBtnListBMP = pSkin->hBitmap;  

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	m_pold_bk_disable_bmp    = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	m_bk_list_bmp    = bk_list_dc.SelectObject(CBitmap::FromHandle(m_stBtnListBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgNaviBro::OnExit()
{
	bk_list_dc.SelectObject(m_bk_list_bmp);
	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnListBMP);
	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_list_dc.DeleteDC();
	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();
	CDialogBase::EndDialog(0);

}

void CDlgNaviBro::InitControl()
{
	//上
	m_Item[0].rect.left		= 645;
	m_Item[0].rect.top		= 212;
	m_Item[0].rect.right	= 784;
	m_Item[0].rect.bottom	= 344;
	//下
	m_Item[1].rect.left		= 645;
	m_Item[1].rect.top		= 345;
	m_Item[1].rect.right	= 784;
	m_Item[1].rect.bottom	= 480;
	//确定
	m_Item[2].rect.left		= 644;
	m_Item[2].rect.top		= 68;
	m_Item[2].rect.right	= 790;
	m_Item[2].rect.bottom	= 135;
	//切换路径（SD卡<-->UDISK）
	m_Item[3].rect.left		= 644;
	m_Item[3].rect.top		= 136;
	m_Item[3].rect.right	= 790;
	m_Item[3].rect.bottom	= 203;
	m_Item[3].chChar		= _T("U盘");
	//Exit
	m_Item[4].rect.left		= 740;
	m_Item[4].rect.top		= 0;
	m_Item[4].rect.right	= 800;
	m_Item[4].rect.bottom	= 60;

	//文件夹和文件列表
	int i;
	for(i=0; i<8; i++)
	{
		if(i<4)
		{
			m_ItemList[i].rect.left		= 100+i*133;
			m_ItemList[i].rect.top		= 145;
			m_ItemList[i].rect.right	= 100+i*133+70;
			m_ItemList[i].rect.bottom	= 145+65;
		}
		else
		{
			m_ItemList[i].rect.left		= 100+(i-4)*133;
			m_ItemList[i].rect.top		= 285;
			m_ItemList[i].rect.right	= 100+(i-4)*133+70;
			m_ItemList[i].rect.bottom	= 285+65;
		}
	}

	for (i =2;  i < 5; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
}

void CDlgNaviBro::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgNaviBro::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgNaviBro::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
	int i;

	for( i=0;i<BTN_COUNT;i++ )
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		//Rect.top = Rect.top + 54;
		DrawBtnText(&memDC,Rect,str, CGDICommon::Instance()->smallfont());
	}

	for( i=0;i<8;i++ )
	{
		//绘制背景
		str = m_ItemList[i].chChar;
		Rect = m_ItemList[i].rect;

		if( m_ItemList[i].nState == BTN_STATE_NORMAL )
		{
			if(m_ItemList[i].nListType == LIST_DIR)
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,0,0,SRCCOPY);
			else if( m_ItemList[i].nListType == LIST_FILE )
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,70,0,SRCCOPY);

			Rect.left	-= 18;
			Rect.top	+= 65;
			Rect.right	+= 18;
			Rect.bottom	+= 50;
			DrawBtnText(&memDC,Rect,m_ItemList[i].chChar,CGDICommon::Instance()->smallfont(),
				RGB(255,255,255), DT_CENTER | DT_TOP | DT_WORDBREAK | DT_WORD_ELLIPSIS);
		}
		else if( m_ItemList[i].nState == BTN_STATE_DOWN )
		{
			if(m_ItemList[i].nListType == LIST_DIR)
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,0,65,SRCCOPY);
			else if( m_ItemList[i].nListType == LIST_FILE )
				memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,70,65,SRCCOPY);

			Rect.left	-= 18;
			Rect.top	+= 65;
			Rect.right	+= 18;
			Rect.bottom	+= 50;
			DrawBtnText(&memDC,Rect,m_ItemList[i].chChar,CGDICommon::Instance()->smallfont(),
				RGB(255,255,255), DT_CENTER | DT_TOP | DT_WORDBREAK | DT_WORD_ELLIPSIS);
		}
		else if( m_ItemList[i].nState == BTN_STATE_DISABLE && m_ItemList[i].nListType == LIST_NONE )
		{
			memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,140,0,SRCCOPY);
		}
		else if( m_ItemList[i].nState == BTN_STATE_DISABLE && m_ItemList[i].nListType == LIST_FILE )
		{
			memDC.BitBlt(Rect.left,Rect.top,Rect.Width(),Rect.Height(),&bk_list_dc,70,130,SRCCOPY);

			Rect.left	-= 18;
			Rect.top	+= 65;
			Rect.right	+= 18;
			Rect.bottom	+= 50;
			DrawBtnText(&memDC,Rect,m_ItemList[i].chChar,CGDICommon::Instance()->smallfont(),
				RGB(255,255,255), DT_CENTER | DT_TOP | DT_WORDBREAK | DT_WORD_ELLIPSIS);
		}
	}

	Rect.left	= 60;
	Rect.top	= 80;
	Rect.right	= 634;
	Rect.bottom = 116;
	DrawBtnText(&memDC,Rect,m_csPath,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT | DT_VCENTER);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_CENTER | DT_VCENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);	

}

void CDlgNaviBro::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<BTN_COUNT;i++)
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

	for(int j=0;j<8;j++)
	{
		if(PtInRect(&m_ItemList[j].rect,point))
		{
			if(m_ItemList[j].nState == BTN_STATE_NORMAL)
			{
				m_ItemList[j].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}

	Invalidate();
	SetCapture();
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgNaviBro::OnLButtonUp(UINT nFlags, CPoint point) 
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
				OnUp();
				break;
			case 1:
				OnDown();
				break;
			case 2:
				OnOK();
				break;
			case 3:
				ChangeDir();
				break;
			case 4:
				OnExit();
				break;
			default:
				break;
			}
		}

	}

	for(int j=0;j<8;j++)
	{
		if(m_ItemList[j].nState==BTN_STATE_DOWN)
		{
			m_ItemList[j].nState = BTN_STATE_NORMAL;

			if( j == 0 && m_ItemList[j].nListType == LIST_DIR && m_ItemList[j].chChar == _T("..") )
				OnUpDir();
			else
				OnBtnList(j);

		}
		else if( m_ItemList[j].nListType == LIST_FILE && m_ItemList[j].nState == BTN_STATE_DISABLE )
		{
			m_ItemList[j].nState = BTN_STATE_NORMAL;
		}

	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void CDlgNaviBro::OnUpDir()
{
	TCHAR ch = '\\';
	int pos = m_csPath.ReverseFind(ch);
	m_csPath.Delete(pos,m_csPath.GetLength()-pos);

	if( !m_csPath.Compare(_T("")) )
	{
		m_csPath = m_strRootDir;
		return;
	}
	else
	{
		LoadDir(m_csPath);
	}
}

void CDlgNaviBro::OnBtnList( int j )
{
	if( m_ItemList[j].nListType == LIST_DIR )
	{
		m_csPath += _T("\\") + m_ItemList[j].chChar;
		LoadDir(m_csPath);
	}
	else if( m_ItemList[j].nListType == LIST_FILE )
	{
		m_ItemList[j].nState = BTN_STATE_DISABLE;
	}
}

void CDlgNaviBro::OnUp()
{
	m_iCurPage--;
	m_Item[1].nState = BTN_STATE_NORMAL;

	if( m_iCurPage == 1 )
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		m_Item[1].nState = BTN_STATE_NORMAL;
	}

	int j = 0;
	for( int i = (m_iCurPage-1)*8;  i < (m_iCurPage*8); i++ )
	{
		m_ItemList[j].nState = m_pItemList[i].nState;
		m_ItemList[j].chChar = m_pItemList[i].chChar;
		m_ItemList[j].nListType = m_pItemList[i].nListType;
		j++;
	}
	Invalidate();
}

void CDlgNaviBro::OnDown()
{
	m_iCurPage++;
	m_Item[0].nState = BTN_STATE_NORMAL;

	if( m_iCurPage == m_PageCnt )
	{
		m_Item[1].nState = BTN_STATE_DISABLE;
		m_Item[0].nState = BTN_STATE_NORMAL;
	}

	int j = 0;
	for( int i = (m_iCurPage-1)*8;  i < (m_iCurPage*8); i++ )
	{
		m_ItemList[j].nState = m_pItemList[i].nState;
		m_ItemList[j].chChar = m_pItemList[i].chChar;
		m_ItemList[j].nListType = m_pItemList[i].nListType;
		j++;
	}		
	Invalidate();
}

void CDlgNaviBro::OnOK()
{
	for( int i = 0;  i < 8; i++ )
	{
		if( m_ItemList[i].nListType == LIST_FILE && m_ItemList[i].nState == BTN_STATE_DISABLE )
		{
			m_csPath += _T("\\") + m_ItemList[i].chChar;

			if(m_bType)
			{
				CSysConfig::Instance().SetNaviPath( m_csPath );
			}
			else
			{
				CSysConfig::Instance().SetAppPath( m_csPath );
			}
			m_MaskDir = m_csPath.Left(m_csPath.GetLength() - m_ItemList[i].chChar.GetLength() - 1);
			m_MaskDir.Delete(0, m_MaskDir.ReverseFind('\\'));

			if(m_MaskDir != m_strRootDir)
			{
				CIniFile iniFile;
				iniFile.Load(g_csPicIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),m_MaskDir);
				iniFile.Flush();

				iniFile.Load(g_csMusicIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),m_MaskDir);
				iniFile.Flush();

				iniFile.Load(g_csMovieIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),m_MaskDir);
				iniFile.Flush();

				iniFile.Load(g_csTxtIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),m_MaskDir);
				iniFile.Flush();
			}
			else
			{
				CIniFile iniFile;
				iniFile.Load(g_csPicIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),_T(""));
				iniFile.Flush();

				iniFile.Load(g_csMusicIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),_T(""));
				iniFile.Flush();

				iniFile.Load(g_csMovieIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),_T(""));
				iniFile.Flush();

				iniFile.Load(g_csTxtIconName);
				iniFile.SetString(_T("CommonSet"),_T("NaviPath"),_T(""));
				iniFile.Flush();
			}
			EndDialog(0);
		}
	}
}

//切换路径（SD卡<-->UDISK）
void CDlgNaviBro::ChangeDir()
{
	if( m_strRootDir.CompareNoCase(PATH_SD) == 0 )
	{
		m_strRootDir		= PATH_UDISK;
		m_Item[3].chChar	= _T("U盘");
	}
	else if( m_strRootDir.CompareNoCase(PATH_UDISK) == 0 )
	{
		//如果SD卡不存在则直接退出
		if( GetFileAttributes(PATH_SD) == 0xFFFFFFFF )
			return;

		m_Item[3].chChar	= _T("SD卡");
		m_strRootDir		= PATH_SD;
	}

	m_csPath= m_strRootDir;
	LoadDir(m_strRootDir);
}

LRESULT CDlgNaviBro::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(WM_DEVICECHANGE == message)
	{
		if( DBT_DEVICEARRIVAL == wParam )	//SD卡插入
		{
			Sleep(600);
			if( m_strRootDir.CompareNoCase(PATH_SD) == 0 )
				LoadDir(m_csPath);
			Invalidate();
		}
		else if ( DBT_DEVICEREMOVECOMPLETE == wParam )//SD卡拔出
		{
			//ChangeDir();
			if( m_strRootDir.CompareNoCase(PATH_SD) == 0 )
			{
				m_strRootDir		= PATH_UDISK;
				m_Item[3].chChar	= _T("USB Disk");
			}
			m_csPath = m_strRootDir;
			LoadDir(m_csPath);
			Invalidate();
		}
	}
	else if( message == WM_CREATE_SET )
	{
		if( wParam == 0x06 )
		{
			OnExit();
		}
	}

	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
