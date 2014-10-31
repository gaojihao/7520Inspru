// DlgGame.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "DlgGame.h"
#include "../GSM/GSMLogic.h"
#include "../ExTool/ex_basics.h"
#include "DlgConfirm.h"
#include "SMSInfo.h"
#include "DlgGameManual.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BOOL g_bLight;
//extern CFont g_smallfont, g_bigfont;
/////////////////////////////////////////////////////////////////////////////
// CDlgGame dialog

CDlgGame::CDlgGame(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgGame::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgGame, CDialogBase)
//{{AFX_MSG_MAP(CMediaMain)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaMain message handlers

BOOL CDlgGame::OnInitDialog() 
{
	CDialogBase::OnInitDialog();
	
	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();
	
	m_iGameCount = 0;

	GetGame();

	m_nCurPage = 1;
	ShowPage( m_nCurPage );
	
	return TRUE;
}
//变换语言和单位
BOOL CDlgGame::OnLanguageChange()//变皮肤
{
	CResString str;
	
	str.LoadString(RES_BUTTON_GAMES);
	m_csTitile = str;
	
	str.LoadString(RES_SMS_READ_SIM_INFO);
	m_list[0].chChar = str;
	m_ItemState[0] = 1;
	
	m_Item[0].chChar = _T("说明");
	m_Item[1].chChar = _T("");
	m_Item[2].chChar = _T("");
	m_Item[3].chChar = _T("");
	
	return TRUE;
}

BOOL CDlgGame::OnSkinChange()
{
	return TRUE;
}

void CDlgGame::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

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
	
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_BG_GAME_LIST_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BMP_BG_GAME_LIST_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp  = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgGame::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	
	DeleteObject( m_stBtnPressBMP );
	DeleteObject( m_stBtnNormalBMP );
	
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CDlgGame::OnExit()
{
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CDlgGame::InitControl()
{
	int i;
	for ( i =0;  i < 4; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}
	
	for ( i =0;  i < elist_count; i++)
		m_list[i].nState = BTN_STATE_NORMAL;
	
	//Manual
	m_Item[0].rect.top    = 233;
	m_Item[0].rect.left   = 227;
	m_Item[0].rect.bottom = 268;
	m_Item[0].rect.right  = 302;
	//m_Item[0].chChar      = "说明";

	//3 上x:312y:231 下x:388y:268
	m_Item[1].rect.top = 231;
	m_Item[1].rect.left = 312;
	m_Item[1].rect.bottom = 268;
	m_Item[1].rect.right = 388;
	
	//4 上x:396y:231 下x:472y:268
	m_Item[2].rect.top = 231;
	m_Item[2].rect.left = 396;
	m_Item[2].rect.bottom = 268;
	m_Item[2].rect.right = 472;

	//Exit
	m_Item[3].rect.top = 0;
	m_Item[3].rect.left = 430;
	m_Item[3].rect.bottom = 40;
	m_Item[3].rect.right = 480;


	m_list[0].rect.left = 0;
	m_list[0].rect.top = 41;
	m_list[0].rect.right = 480;
	m_list[0].rect.bottom = 77;
		
	m_list[1].rect.left = 0;
	m_list[1].rect.top = 78;
	m_list[1].rect.right = 480;
	m_list[1].rect.bottom = 114;
		
	m_list[2].rect.left = 0;
	m_list[2].rect.top = 115;
	m_list[2].rect.right = 480;
	m_list[2].rect.bottom = 151;
		
	m_list[3].rect.left = 0;
	m_list[3].rect.top = 152;
	m_list[3].rect.right = 480;
	m_list[3].rect.bottom = 188;

	m_list[4].rect.left = 0;
	m_list[4].rect.top = 189;
	m_list[4].rect.right = 480;
	m_list[4].rect.bottom = 225;
}

void CDlgGame::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgGame::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgGame::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	int i=0;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	
	CString str;
	CRect Rect;
	enBtnState state;
	
	for(i=0;i<4;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect, str, smallfont,RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}
	
	for( i = 0; i < elist_count; i++ )
	{
		//DrawItems( &memDC,m_list[i].rect,m_list[i].nState);
		//CRect rt(m_list[i].rect);

		DrawBtnText( &memDC,m_list[i].rect, m_list[i].chChar, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	}

	str.Format(_T("Game"));
	DrawBtnText(&memDC,g_retTitile,str,bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgGame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	for( i = 0; i < 4; i++ )
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
	
	for( i = 0; i < elist_count; i++ )
	{
		if(PtInRect(&m_list[i].rect,point))
		{
			if(m_list[i].nState == BTN_STATE_NORMAL)
			{
				m_list[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}
	
	Invalidate();
	SetCapture();	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgGame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 	// TODO: Add your message handler code here and/or call default
	int i=0;
	for(i=0;i<4;i++)
	{
		if(m_Item[i].nState == BTN_STATE_DOWN )
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:	OnBtnManual();break;
			case 1:	OnBtnPageUp();break;
			case 2:	OnBtnPageDown();break;
			case 3: OnExit();break;
			default:
				break;
			}
		}
	}
	
	for(i=0;i<elist_count;i++)
	{
		if(m_list[i].nState == BTN_STATE_DOWN )
		{
			m_list[i].nState = BTN_STATE_NORMAL;
			UserClickItem( i );
		}
	}
	
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}


void  CDlgGame::OnBtnPageUp()
{
	m_nCurPage--;
	if ( m_nCurPage < 1 )
		m_nCurPage = 1;
	ShowPage( m_nCurPage );
	Invalidate();
}

void  CDlgGame::OnBtnPageDown()
{
	m_nCurPage++;
	int nMaxNum = m_iGameCount;
	int MaxPage = 1 + nMaxNum / elist_count;
	if ( nMaxNum > 0 )
		MaxPage = (nMaxNum - 1) / elist_count + 1;
	if ( m_nCurPage > MaxPage ) 
		m_nCurPage = MaxPage;
	
	ShowPage( m_nCurPage );
	Invalidate();
}

void  CDlgGame::OnBtnManual()
{
	CDlgGameManual dlg;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////

void    CDlgGame::UserClickItem(int nItem )
{
	int nMaxRd = m_iGameCount;
	int  nSleIdx = nItem + (m_nCurPage - 1) * elist_count;
	if ( nSleIdx >= nMaxRd )
		return;
	//nSleIdx = nMaxRd - nSleIdx - 1;
	if ( nSleIdx < m_iGameCount )
	{
		if ( nItem < elist_count) 	
			m_ItemState[nItem] = 1;
	}
	
	PROCESS_INFORMATION g_proc_info  = { 0 };
	STARTUPINFO         g_start_info = { sizeof(g_start_info) };

    BOOL bProcessState = CreateProcess(m_strPath.GetAt(nSleIdx), NULL, NULL, NULL, 
		FALSE, 0, NULL, NULL, NULL, &g_sProcessInfo );
	
	if(bProcessState)
		CreateThread(NULL, 0, CE9101MainApp::THDAppExit, NULL, 0, NULL);

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void    CDlgGame::ShowPage(int nPage)
{
	int nMaxRd = m_iGameCount;
	int i = 0;
	
	int MaxPage = 1 + nMaxRd / elist_count;
	if ( nMaxRd > 0 )
		MaxPage = ( nMaxRd - 1 ) / elist_count + 1;
	
	if ( nPage > MaxPage ) return;
	
	int nStartIdx =  ( nPage - 1 ) * elist_count;
	for( i = 0; i < elist_count; i++ )
	{
		m_ItemState[i] = 0;
		m_list[i].chChar = _T("");
		m_list[i].nState = BTN_STATE_DISABLE;
	}
	
	for( i = 0; i < elist_count; i++ )
	{
		if ( nStartIdx >= nMaxRd )
			break;
		if ( nStartIdx < m_iGameCount )
		{
			m_list[i].chChar = m_strName.GetAt(nStartIdx);
			m_list[i].nState = BTN_STATE_NORMAL;
		}else
		{
			m_list[i].chChar = m_strName.GetAt(nStartIdx);
			m_list[i].nState = BTN_STATE_NORMAL;
		}
		nStartIdx++;
	}
}

BOOL CDlgGame::PreTranslateMessage(MSG* pMsg) 
{
	return CDialogBase::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void   CDlgGame::GetGame()
{
	CString str;
	if(m_IniFile.Load( _T("\\SDMMC DISK\\Game.ini") ))
	{
		BOOL bValue = TRUE;
		for(int i=1; bValue; i++)
		{
			CString strNumber;
			CString strGameInfo;
			strNumber.Format(_T("%d"), i);
			//Get game path.
			str = _T("PATH") + strNumber;
 			bValue = m_IniFile.GetString(_T("GameInfo"), str, strGameInfo);
			if(bValue)
			{
				m_strPath.Add(strGameInfo);
			}
			//Get game name.
			str = _T("NAME") + strNumber;
			if(m_IniFile.GetString(_T("GameInfo"), str, strGameInfo))
			{
				m_strName.Add(strGameInfo);
			}
		}
		m_iGameCount = m_strPath.GetSize();
	}
}

/*
BOOL CDlgGame::Load(const CString csFileName, BOOL IsCreate)
{
	m_sFileName = csFileName;

	SetFileAttributes(m_sFileName, FILE_ATTRIBUTE_NORMAL);

	CFileException e;
	TRY
	{
		if(!IsCreate)
			m_bFileExist=m_IniFile.Open(m_sFileName, 
				CFile::modeReadWrite ,&e);
		else
			m_bFileExist=m_IniFile.Open(m_sFileName,
				CFile::modeCreate | CFile::modeReadWrite ,&e);
	}
	CATCH(CFileException,e)
	{
	//	m_IniFile.Close();
		m_bFileExist = FALSE;
		TRACE0("打开文件错误！");
		return FALSE;
	}
	END_CATCH

	if(m_bFileExist == 0)
		return FALSE;
	
	TRY
	{
		ReadIniFile();

		m_IniFile.Close();
	}
	CATCH( CException,e )
	{
		m_IniFile.Close();
		DeleteFile( csFileName );
		m_bFileExist = FALSE;
		TRACE0("打开文件错误！");
		return FALSE;
	}
	END_CATCH

	m_bFileExist = FALSE;	
	return TRUE;
}

BOOL CDlgGame::GetFileString(const CString& Section,const CString& Item, CString& Value)
{
	if(m_FileContainer.GetSize() <= 0)
		return FALSE;//文件为空，返错误

	int i = 0;
	int iFileLines = m_FileContainer.GetSize();
	CString strline,str;

	while(i<iFileLines)
	{
		strline = m_FileContainer.GetAt(i++);

		strline.TrimLeft();
		if(strline.GetAt(0)=='[')//查找Section，第一个必须为[
		{
			str=strline.Left(strline.Find(_T("]")));//去掉]右边
			str=str.Right(str.GetLength()-str.Find(_T("["))-1);//去掉[左边

			str.TrimLeft();
			str.TrimRight();

			if(Section == str)//找到Section
			{
				while(i<iFileLines)
				{
					strline = m_FileContainer.GetAt(i++);
					strline.TrimLeft();

					if(strline.GetAt(0)=='[')
						return FALSE;//如果到达下一个[]，即找不到,返回错误值
					
					str = strline.Left(strline.Find(_T("=")));//去掉=右边

					str.TrimLeft();
					str.TrimRight();

					if(Item == str)//找到Item
					{
						str=strline.Right(strline.GetLength()-strline.Find(_T("="))-1);//去掉=左边

						str.TrimLeft();
						str.TrimRight();

						Value = str;
						return TRUE;
					}
				}
				return FALSE;//找不到,返回默认值
			}

		}

	}
	return FALSE;//找不到,返回默认值
}

BOOL CDlgGame::ReadIniFile()
{
	m_iPos = 1;
	CString strline;
	
	if(m_FileContainer.GetSize() > 0)
	{
		m_FileContainer.RemoveAll();
	}
	
	m_IniFile.SeekToBegin();
	ULONG buflen = m_IniFile.GetLength() + 1;
	char* pFileBuf = new char[buflen];
	memset(pFileBuf,0,buflen);

	buflen = m_IniFile.Read(pFileBuf,buflen);


	buflen = buflen/2;

	BOOL bReadState = TRUE;
	while(bReadState)
	{
		TCHAR OutBuf[1024];
		TCHAR* pOutBuf = OutBuf;
		UINT OutLen = 0;
		bReadState = ReadOneString((TCHAR*)pFileBuf,buflen,pOutBuf,OutLen);
		if (bReadState && OutLen!=0)
		{
			pOutBuf[OutLen] = '\0';
			CString OneStr(pOutBuf);
			m_FileContainer.Add(OneStr);
		}
	}
	
	if(pFileBuf != NULL)
	{
		delete []pFileBuf;
		pFileBuf = NULL;
	}

	return TRUE;
}

BOOL CDlgGame::ReadOneString(TCHAR* pIn, UINT iInLen, TCHAR*& pOut, UINT &iOutLen)
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
*/
