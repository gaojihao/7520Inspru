// PWDInputDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "DlgRFIDScanner.h"

#include "../IPSetDlg.h"
#include "../GSMDialMain.h"
#include "../E9101MainDlg.h"
#include "../../SQLite3/CppSQLite3.h"
#include "../../SCom/scom_rfid.h"
#include "../Msg/MsgQueue.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern serial_win32	g_serial_ic;

CDlgRFIDScanner::CDlgRFIDScanner(CWnd* pParent /*=NULL*/)
: CDialogBase(CDlgRFIDScanner::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRFIDScanner)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTHDIC = TRUE;
	//m_bIsGetICNO = FALSE;
	//memset(m_cRFID, NULL, sizeof(m_cRFID));
	//m_nRFIDCount = 0;
	m_csState		= TEXT("请扫描RFID码!");

	m_nPageIndex	= 0;
	m_nPageCount	= 1;
}

void CDlgRFIDScanner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRFIDScanner)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgRFIDScanner, CDialog)
	//{{AFX_MSG_MAP(CDlgRFIDScanner)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRFIDScanner message handlers

BOOL CDlgRFIDScanner::OnInitDialog()
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

	//OnTest();

	//添加获取RFID的函数到数组中
	scom_rfid::Instance().AddFnc(this, GetRFID);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgRFIDScanner::InitGdi()
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

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_RFID_SCAN_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_RFID_SCAN_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	ReleaseDC(m_pDC);
}

void CDlgRFIDScanner::InitControl()
{
	int i;
	for(i=0; i<enBtnCount; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}

	//发送,清除,上一页,下一页
	for(i=0; i<4; i++)
	{
		m_item[i].rect.left		= 348 + 113*i;
		m_item[i].rect.top		= 394;
		m_item[i].rect.right	= 348 + 113*i + 113;
		m_item[i].rect.bottom	= 480;
	}
	m_item[0].chChar		= _T("发送");
	m_item[1].chChar		= _T("清除");
	//Exit.
	m_item[4].rect.left   = 740;
	m_item[4].rect.top    = 0;
	m_item[4].rect.right  = 800;
	m_item[4].rect.bottom = 60;
}

void CDlgRFIDScanner::OnExit()
{
	//删除数组中的获取RFID的函数
	scom_rfid::Instance().RemoveFnc(GetRFID);

	//m_bTHDIC = FALSE;
	//if(m_hTHDICCardWatch != NULL)
	//	TerminateThread(m_hTHDICCardWatch,0);
	//Sleep(1000);
	//if( !g_serial_ic.close() )
	//	MessageBox(_T("关闭串口失败!"));
	//	bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	//	DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	//	bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialog::EndDialog(0);
}

void CDlgRFIDScanner::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgRFIDScanner::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgRFIDScanner::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
	int i;
	for(i=0; i<enBtnCount; i++)
	{
		//绘制背景
		str		= m_item[i].chChar;
		Rect	= m_item[i].rect;
		state	= m_item[i].nState;
		DrawItems(&memDC,m_item[i].rect,m_item[i].nState);
		DrawBtnText(&memDC,m_item[i].rect,m_item[i].chChar,bigfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}

	//Title.
	str = TEXT("RFID");
	DrawBtnText(&memDC,g_retTitile,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);


	if( !CSysConfig::Instance().GetICCard() )
		str = TEXT("设备未连接!");
	//显示状态信息
	DrawBtnText( &memDC, CRect(50, 397, 500, 480), m_csState, bigfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	//显示页码
	str.Format(_T("%d/%d"), m_nPageIndex+1, m_nPageCount);
	DrawBtnText(&memDC, CRect(240,430,348,480),str,smallfont,RGB(255,255,255),DT_RIGHT | DT_BOTTOM);
	//显示已扫描RFID码
	DrawRFID(&memDC);
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgRFIDScanner::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	for( i = 0; i < enBtnCount; i++ )
	{
		if(PtInRect(&m_item[i].rect,point))
		{
			if(m_item[i].nState == BTN_STATE_NORMAL)
			{
				m_item[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}

	Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgRFIDScanner::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
	for(i=0; i<enBtnCount; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			m_item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0://发送
				//OnTest();
				OnBtnSend();		
				break;
			case 1://清除
				OnBtnClear();	
				break;
			case 2://上一页
				OnBtnChangePage(-1);
				break;
			case 3://下一页
				OnBtnChangePage(1);
				break;
			case 4:
				OnExit();
				break;
			default:
				break;
			}
		}
	}

	Invalidate();
	//ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgRFIDScanner::OnBtnSend()
{
	if(0 == m_vtRFID.size())
	{
		m_csState = TEXT("没有扫描到RFID数据!");
		Invalidate();
		return;
	}
	//以','隔开m_cRFID数组中的rfid号，写入数据库中
	const DWORD dwLength = m_vtRFID.size() * sizeof(RFID_INFO);

	//CString str,str2;
	//str.Format(_T("dwLength:%d"), dwLength);
	//MessageBox(str);

	char*	pszRFID = new char[dwLength];
	memset(pszRFID, NULL, sizeof(dwLength));
	int		nOffset = 0;
	for(ITER_RFID iter=m_vtRFID.begin(); iter!=m_vtRFID.end(); iter++)
	{
		//str2 = iter->szRFID;
		//str.Format(_T("nOffset:%d,RFID:%s"), nOffset, str2);
		//MessageBox(str);

		memcpy(&pszRFID[nOffset], iter->szRFID, strlen(iter->szRFID));
		nOffset += strlen(iter->szRFID);
		if(iter < m_vtRFID.end()-1)				//最后面不需要逗号
			pszRFID[nOffset++] = ',';			//用逗号隔开
	}

	char* pszSql = new char[dwLength+40];
	memset(pszSql, NULL, sizeof(dwLength+40));
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(pszSql, "update msg_buffer set rfid = '%s';", pszRFID);
	db.execDML(pszSql);
	db.close();

	delete[] pszRFID;
	delete[] pszSql;
	pszRFID = NULL;
	pszSql = NULL;

	if(CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, WPARAM_SEND_RFID_CMD,1))
	{
		m_csState = TEXT("成功发送到服务器!");
		m_vtRFID.clear();
		m_nPageIndex = 0;
		m_nPageCount = 1;
		Invalidate();
	}

	return;
}

BOOL CDlgRFIDScanner::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}

bool CDlgRFIDScanner::check_cardnumber(char* pBuffer, int nLen)
{
	//CC FF FF 01 08 0D E2 00 10 66 66 13 00 95 20 70 3C 81 6D 
	//CC FF FF 01 08 0D E2 00 10 66 66 13 00 85 12 80 92 BE E8 
	if( (UCHAR)pBuffer[0] == 0xcc
		&& (UCHAR)pBuffer[1] == 0xFF
		&& (UCHAR)pBuffer[2] == 0xFF
		&& (UCHAR)pBuffer[3] == 0x01
		&& (UCHAR)pBuffer[4] == 0x08
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//改变页数
void CDlgRFIDScanner::OnBtnChangePage(int nDiffer)
{
	m_nPageIndex += nDiffer;
	if(m_nPageIndex < 0)
		m_nPageIndex = 0;
	else if(m_nPageIndex > m_nPageCount-1)
		m_nPageIndex = m_nPageCount-1;
}

void CDlgRFIDScanner::OnBtnClear()
{
	//memset(m_cRFID, NULL, sizeof(m_cRFID));
	//m_nRFIDCount = 0;
	m_vtRFID.clear();
	m_csState = TEXT("全部清除!");

	m_nPageIndex = 0;
	m_nPageCount = 1;
}

void CDlgRFIDScanner::DrawRFID(CDC* pmemDC)
{
	CString str,strTemp;
	int j,k;
	int nRFIDIndex;
	CRect rect;
	//第0列～第enCountColumn-1列
	for(j=0; j<enCountColumn; j++)
	{
		//第0行～第enCountRow-1列
		for(k=0; k<enCountRow; k++)
		{
			rect.left	= 160*j;
			rect.top	= 100 + 25*k;
			rect.right	= 160*j + 160;
			rect.bottom	= 100 + 25*k + 25;

			nRFIDIndex	= enCountColumn*enCountRow*m_nPageIndex + enCountRow*j + k;
			if(nRFIDIndex < (int)m_vtRFID.size())
			{
				strTemp		= m_vtRFID[nRFIDIndex].szRFID;
				str.Format(_T("%4d. %s"), nRFIDIndex+1, strTemp);
				DrawBtnText(pmemDC, rect, str,smallfont,RGB(255,255,255), DT_LEFT | DT_VCENTER);
			}
		}
	}
}

void CDlgRFIDScanner::GetRFID(PVOID pCWnd, char* pBuffer, int nLen)
{
	if(pCWnd == NULL)
		return;

	//TRACE1("", pBuffer);

	RFID_INFO stRFIDInfo;
	memset(&stRFIDInfo, NULL, sizeof(RFID_INFO));
	strncpy(stRFIDInfo.szRFID, pBuffer, sizeof(RFID_INFO));

	CDlgRFIDScanner* pDlg = (CDlgRFIDScanner*)pCWnd;
	int i;
	for(i=0; i<(int)pDlg->m_vtRFID.size(); i++)
	{
		if( strcmp(pBuffer, pDlg->m_vtRFID[i].szRFID) == 0 )
			return;
	}
	pDlg->m_vtRFID.push_back(stRFIDInfo);
	pDlg->m_csState = _T("扫描成功");
	//计算页数
	//pDlg->m_nPageCount = ( pDlg->m_vtRFID.size()==0 ) ? 1 : 
	//	(pDlg->m_vtRFID.size()-1)/(enCountColumn*enCountRow) + 1;
	//计算总页数
	if(pDlg->m_vtRFID.size() > 0)
		pDlg->m_nPageCount = (pDlg->m_vtRFID.size()-1)/(enCountColumn*enCountRow)+1;
	else
		pDlg->m_nPageCount = 1;

	pDlg->Invalidate();
}

void CDlgRFIDScanner::OnTest()
{
	RFID_INFO stRFIDInfo;
	for(int i=0; i<50; i++)
	{
		sprintf(stRFIDInfo.szRFID, "%010d", i+10);
		m_vtRFID.push_back(stRFIDInfo);
	}	
	//计算总页数
	if(m_vtRFID.size() > 0)
		m_nPageCount = (m_vtRFID.size()-1)/(enCountColumn*enCountRow)+1;
	else
		m_nPageCount = 1;
}