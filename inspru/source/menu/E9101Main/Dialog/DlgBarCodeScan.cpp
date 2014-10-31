// PWDInputDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "DlgBarCodeScan.h"

#include "../IPSetDlg.h"
#include "../GSMDialMain.h"
#include "../E9101MainDlg.h"
#include "../../SQLite3/CppSQLite3.h"
#include "../Msg/MsgQueue.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern serial_win32	g_serial_ic;

CDlgBarCodeScan::CDlgBarCodeScan(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgBarCodeScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBarCodeScan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTHDIC = TRUE;
	m_bIsGetICNO = FALSE;
	memset(m_cBarCode, NULL, sizeof(m_cBarCode));
	m_nBarCodeCount = 0;
	m_csState = TEXT("请扫描条形码!");
}

void CDlgBarCodeScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarCodeScan)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgBarCodeScan, CDialog)
	//{{AFX_MSG_MAP(CDlgBarCodeScan)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarCodeScan message handlers

BOOL CDlgBarCodeScan::OnInitDialog()
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

	enSel = PWDINPUT;
	ConnectCardReader(CSysConfig::Instance().GetComScanner(), 9600);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgBarCodeScan::InitGdi()
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

		PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_BARCODESCAN_N,true);
		m_stBtnNormalBMP   = pSkin->hBitmap;
		
		pSkin = g_pResMng->RequestBmp(BG_BARCODESCAN_P, true);
		m_stBtnPressBMP = pSkin->hBitmap;
		
		
		m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
		m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
		
		ReleaseDC(m_pDC);
}

void CDlgBarCodeScan::InitControl()
{
	int i;
	for(i=0; i<enBtnCount; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}

	//发送
	m_item[0].rect.left		= 512;
	m_item[0].rect.top		= 397;
	m_item[0].rect.right	= 656;
	m_item[0].rect.bottom	= 480;
	m_item[0].chChar		= _T("发送");
	//清除
	m_item[1].rect.left		= 657;
	m_item[1].rect.top		= 397;
	m_item[1].rect.right	= 800;
	m_item[1].rect.bottom	= 480;
	m_item[1].chChar		= _T("清除");
	//Exit.
	m_item[2].rect.left   = 740;
	m_item[2].rect.top    = 0;
	m_item[2].rect.right  = 800;
	m_item[2].rect.bottom = 60;
}

void CDlgBarCodeScan::OnExit()
{
	m_bTHDIC = FALSE;
	if(m_hTHDICCardWatch != NULL)
		TerminateThread(m_hTHDICCardWatch,0);
	Sleep(1000);
	if( !g_serial_ic.close() )
		MessageBox(_T("关闭串口失败!"));
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

void CDlgBarCodeScan::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgBarCodeScan::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgBarCodeScan::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	int i;
	for(i=0; i<3; i++)
	{
		//绘制背景
		DrawItems(&memDC,m_item[i].rect,m_item[i].nState);
		DrawBtnText(&memDC,m_item[i].rect,m_item[i].chChar,bigfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}

	CString str;
	CResString strRes;
	CRect   rcItem;
	rcItem = m_item[14].rect;
	rcItem.left = 60;

	//Title.
	str = TEXT("条形码扫描");

	DrawBtnText(&memDC,g_retTitile,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);
	
	//扫描状态
	if( !CSysConfig::Instance().GetICCard() )
		m_csState = TEXT("设备未连接!");
	DrawBtnText( &memDC, CRect(50, 397, 500, 480), m_csState, bigfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	//显示已扫描条形码
	DrawBarCode(&memDC);
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgBarCodeScan::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CDlgBarCodeScan::OnLButtonUp(UINT nFlags, CPoint point) 
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
			case 0:		
				OnBtnSend();		
				break;
			case 1:
				OnBtnClear();	
				break;
			case 2:
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

void CDlgBarCodeScan::OnBtnSend()
{
	if(0 == m_nBarCodeCount)
	{
		m_csState = TEXT("没有扫描到条形码数据!");
		Invalidate();
		return;
	}
	
	//以','隔开m_cRFID数组中的rfid号，写入数据库中
	char	szBarCode[2048];
	int		nOffset = 0;
	memset(szBarCode, NULL, sizeof(szBarCode));
	for(int i=0; i<m_nBarCodeCount; i++)
	{
		memcpy(&szBarCode[nOffset], &m_cBarCode[i], strlen(m_cBarCode[i]));
		nOffset += strlen(m_cBarCode[i]);
		if(i < m_nBarCodeCount-1)	//最后面不需要逗号
			szBarCode[nOffset++] = ',';		//用逗号隔开
	}

	char szSql[512];
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(szSql, "update msg_buffer set bar_code = '%s';", szBarCode);
	db.execDML(szSql);
	db.close();
	/*
	cex_file file;
	if( file.is_exist(PATH_TZ_BUFFER_BAR_CODE) )
		file.remove(PATH_TZ_BUFFER_BAR_CODE);
	file.open(PATH_TZ_BUFFER_BAR_CODE, cex_file::m_write);
	for(int i=0; i<m_nBarCodeCount; i++)
	{
		file.write(&m_cBarCode[i], 256);
	}
	file.close();
	*/

	if(CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, WPARAM_BAR_CODE_CMD,1))
	{
		m_csState = TEXT("成功发送到服务器!");
		memset(m_cBarCode, NULL, sizeof(m_cBarCode));
		m_nBarCodeCount = 0;
		Invalidate();
	}
	
	return;
}

void CDlgBarCodeScan::OnBtnClear()
{
	memset(m_cBarCode, NULL, sizeof(m_cBarCode));
	m_nBarCodeCount = 0;
	m_csState = TEXT("全部清除!");
}

BOOL CDlgBarCodeScan::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}

bool CDlgBarCodeScan::ConnectCardReader(int n_port, int n_baud)
{
	if ( g_serial_ic.is_open() )
		return true;

	if ( !g_serial_ic.open( n_port, n_baud ))
	{
		CString str;
		str.Format(_T("打开IC卡读卡器串口失败:com%d,%d"), n_port, n_baud);
		MessageBox(str);
		return false;
	}
	//if( !g_serial_ic.change_baud(n_baud) )
	//	MessageBox(_T("改变波特率失败!"));

	unsigned long  thrd_id;
	m_hTHDICCardWatch = CreateThread(NULL,
							0,
							THDICCardWatch,
							this,
							0,
							&thrd_id );
	if ( m_hTHDICCardWatch == 0 )
		return false;

	return true;
}

DWORD CDlgBarCodeScan::THDICCardWatch(LPVOID lpThreadParameter)
{
	CDlgBarCodeScan* pDlg = (CDlgBarCodeScan*)lpThreadParameter;
	char		cBuffer[256];
	int			nOffset = 0;
	while(pDlg->m_bTHDIC)
	{
		while(true)
		{
			int nLenRead = g_serial_ic.read_data(cBuffer+nOffset,1);
			if ( nLenRead == 0 )
			{
				Sleep(50);
				continue;
			}
			if(cBuffer[nOffset] == 0x0D		//二维条形码结尾:0x0D0A
				|| cBuffer[nOffset] == 0x0A)
				break;
			else
				nOffset++;
		}
		if(nOffset != 0)
			pDlg->parse_icdata(cBuffer, nOffset);
		nOffset = 0;
	}
	return 1;
}

void CDlgBarCodeScan::parse_icdata(char* pBuffer, int nLen)
{
	int i;
	TRACE0("\n>>>>>>>>>>Receive Serial COM Data:>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	for(i=0; i<nLen; i++)
	{
		TRACE1("%c", pBuffer[i]);
	}
	TRACE0("\n--------16进制数据-------------------------------------\n");
	for(i=0; i<nLen; i++)
	{
		TRACE1("%02X ", (UCHAR)pBuffer[i]);
		if( (i+1)%20 == 0 && (i+1) != nLen )
			TRACE0("\n");
	}
	TRACE0("\n>>>>>>>>>>end>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

	for(i=0; i<m_nBarCodeCount; i++)
	{
		if( strncmp(pBuffer, m_cBarCode[i], nLen) == 0 )
		{
			m_csState = _T("重复扫描");
			Invalidate();
			return;
		}
	}
	if(m_nBarCodeCount < 10)
	{//加入条形码数组
		memcpy(&m_cBarCode[i], pBuffer, nLen);

		m_nBarCodeCount++;
		m_csState = _T("扫描成功");
	}
	else
	{
		m_csState = _T("已扫描10条,请发送!");
	}
	Invalidate();
}

void CDlgBarCodeScan::DrawBarCode(CDC* pmemDC)
{
	CString str;
	CRect rect(50,70,750,95);
	CRect rect2;
	for(int i=0; i<m_nBarCodeCount; i++)
	{
		str = &m_cBarCode[i][0];
		str = str.Left(str.GetLength());
		DrawBtnText(pmemDC, rect, str,smallfont,RGB(255,255,255), DT_LEFT | DT_VCENTER);
		
		str.Format(_T("%d. "), i+1);
		rect2 = rect;
		rect2.left	= 0;
		rect2.right = 50;
		DrawBtnText(pmemDC, rect2,str,smallfont,RGB(255,255,255), DT_CENTER | DT_VCENTER);
		rect.OffsetRect(0,25);
	}
}
