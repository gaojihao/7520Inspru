// PWDInputDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "DlgRFID.h"

#include "../IPSetDlg.h"
#include "../GSMDialMain.h"
#include "../E9101MainDlg.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRFID dialog

extern serial_win32	g_serial_ic;

CDlgRFID::CDlgRFID(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgRFID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRFID)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTHDIC = TRUE;
	m_bIsGetICNO = FALSE;
	//memset(&m_stICCard, NULL, sizeof(m_stICCard));
}

void CDlgRFID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRFID)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgRFID, CDialog)
	//{{AFX_MSG_MAP(CDlgRFID)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRFID message handlers

BOOL CDlgRFID::OnInitDialog()
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
	ConnectCardReader(2, 9600);
	//SendCMD(1);	//蜂鸣

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgRFID::InitGdi()
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

void CDlgRFID::InitControl()
{
	int i;
	for(i=0; i<16; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}

	//OK button.
	m_item[0].rect.left		= 590;
	m_item[0].rect.top		= 357;
	m_item[0].rect.right	= 741;
	m_item[0].rect.bottom	= 447;
	m_item[0].chChar		= _T("发送");
	//Exit.
	m_item[1].rect.left   = 740;
	m_item[1].rect.top    = 0;
	m_item[1].rect.right  = 800;
	m_item[1].rect.bottom = 60;
}

void CDlgRFID::OnExit()
{
	m_bTHDIC = FALSE;
	//if( !g_serial_ic.close() )
	//	MessageBox(_T("关闭串口失败!"));
	if(m_hTHDICCardWatch != NULL)
		TerminateThread(m_hTHDICCardWatch, 0);

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

void CDlgRFID::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgRFID::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgRFID::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	int i;
	for(i=0; i<2; i++)
	{
		//绘制背景
		DrawItems(&memDC,m_item[i].rect,m_item[i].nState);
		DrawBtnText(&memDC,m_item[i].rect,m_item[i].chChar,bigfont,RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}

	CString str;
	CResString strRes;
	CRect   rcItem;
	//Title.
	str = TEXT("RFID");
	DrawBtnText(&memDC,g_retTitile,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);

	//扫描状态
	//DrawBtnText( &memDC, CRect(536, 142, 789, 198), m_csState, bigfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	//显示RFID
	CRect rect(304,184,696,249);
	DrawBtnText(&memDC, rect,m_csCardNO,bigfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);
	str = _T("卡号:");
	DrawBtnText(&memDC, CRect(104,184,304,249),str,bigfont,RGB(255,255,255),DT_RIGHT | DT_VCENTER);
	//DrawBarCode(&memDC);
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgRFID::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i = 0;
	for( i = 0; i < 2; i++ )
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

void CDlgRFID::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int i;
	for(i=0; i<2; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			m_item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:		
				OnSave();		
				break;	//OK
			case 1:
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

void CDlgRFID::OnSave()
{
	cex_file file;
	if( file.is_exist(PATH_TZ_BUFFER_IC_CARD) )
		file.remove(PATH_TZ_BUFFER_IC_CARD);
	file.open(PATH_TZ_BUFFER_IC_CARD, cex_file::m_write);
	file.write(&m_stICCard, sizeof(m_stICCard));
	file.close();

	HWND hWnd = ::FindWindow(NULL, _T("TZ_GPS_VEHICLE"));
	if(hWnd != NULL)
		::PostMessage(hWnd, WM_COMMAND_TIANZE, WPARAM_IC_CARD, 1);
	else
		MessageBox(_T("找不到窗口：一体机"));
	
	return;
//
//	if(CheckPWD())
//	{
//	}
//	else
//	{
//		if ( ID_ENGLISH == CSysConfig::Instance().GetLanguage() )
//			MessageBox(_T("Password error!"), _T("WARNING"), MB_OK);
//		else
//			MessageBox(_T("密码错误!"), _T("WARNING"), MB_OK);
//		//m_strPWDInput = TEXT("");
//	}
}
//
//bool CDlgRFID::CheckPWD()
//{
//	if( !m_IniFile.Load( "\\Flashdrv Storage\\menu\\GSM.dll" ) )
//	{
//		return false;
//	}
//	
//	if(m_strFilePWD.GetLength() == 0)
//		return true;
//	
//	if(m_strFilePWD == m_strPWDInput)
//		return true;
//	else
//		return false;
//}

BOOL CDlgRFID::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}

bool CDlgRFID::ConnectCardReader(int n_port, int n_baud)
{
//	if ( g_serial_ic.is_open() )
//		return true;
//
//	if ( !g_serial_ic.open( n_port, n_baud ))
//	{
//		CString str;
//		str.Format(_T("打开IC卡读卡器串口失败:com%d,%d"), n_port, n_baud);
//		MessageBox(str);
//		return false;
//	}

	if( !g_serial_ic.change_baud(n_baud) )
		MessageBox(_T("改变波特率失败!"));
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

DWORD CDlgRFID::THDICCardWatch(LPVOID lpThreadParameter)
{
	CDlgRFID* pDlg = (CDlgRFID*)lpThreadParameter;
	char		cBuffer[100];
	while(pDlg->m_bTHDIC)
	{
		memset(cBuffer, NULL, sizeof(cBuffer));
		int nLenRead = g_serial_ic.read_data(cBuffer,100);
		if ( nLenRead == 0 )
		{
			Sleep(50);
			continue;
		}
		pDlg->parse_icdata(cBuffer, nLenRead);
	}
	return 1;
}

void CDlgRFID::parse_icdata(char* pBuffer, int nLen)
{
	TRACE0("\n--------16进制数据-------------------------------------\n");
	for(int i=0; i<nLen; i++)
	{
		TRACE1("%02X ", (UCHAR)pBuffer[i]);
		if( (i+1)%20 == 0 && (i+1) != nLen )
			TRACE0("\n");
	}
	TRACE0("\n########RFID data end###################################\n");
	if( check_cardnumber(pBuffer, nLen) )
	{
		//CC FF FF 01 08 0D E2 00 10 66 66 13 00 95 20 70 3C 81 6D
		//CC FF FF 01 08 0D E2 00 10 66 66 13 00 85 12 80 92 BE E8
		//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 (index)
		char cCardNum[16];
		memset(&m_stICCard, NULL, sizeof(m_stICCard));
		//for(int i=5; i<nLen; i++)
		sprintf(m_stICCard.m_ID, ("%03d%03d%03d%03d"), 
			(UCHAR)pBuffer[14], 
			(UCHAR)pBuffer[15], 
			(UCHAR)pBuffer[16], 
			(UCHAR)pBuffer[17]);
		m_csCardNO = m_stICCard.m_ID;
		m_bIsGetICNO = TRUE;
		Invalidate();
		return;
	}
}

void CDlgRFID::SendCMD(int n)
{
	//02 01 00 05 00 0C F3 00 64 63
	//02 01 00 04 29 10 42 FD 00 28 
	//02 01 00 04 29 03 FC 00 28 
	char cSendBuffer[100];
	memset(cSendBuffer, NULL, sizeof(cSendBuffer));
	if(n == 1)	//beep
	{
		cSendBuffer[0] = 0x02;
		cSendBuffer[1] = 0x01;
		cSendBuffer[2] = 0x00;
		cSendBuffer[3] = 0x05;
		cSendBuffer[4] = 0x00;
		cSendBuffer[5] = 0x0C;
		cSendBuffer[6] = 0xF3;
		cSendBuffer[7] = 0x00;
		cSendBuffer[8] = 0x64;
		cSendBuffer[9] = 0x63;
		g_serial_ic.write_data(cSendBuffer, 10);
	}
	else if(n == 2)//判断卡是否存在
	{//02 01 00 04 29 10 42 FD 00 28 
		cSendBuffer[0] = 0x02;
		cSendBuffer[1] = 0x01;
		cSendBuffer[2] = 0x00;
		cSendBuffer[3] = 0x04;
		cSendBuffer[4] = 0x29;
		cSendBuffer[5] = 0x10;
		cSendBuffer[6] = 0x42;
		cSendBuffer[7] = 0xFD;
		cSendBuffer[8] = 0x00;
		cSendBuffer[9] = 0x28;
		g_serial_ic.write_data(cSendBuffer, 10);
	}
	else if(n == 3)//读卡号
	{//02 01 00 04 29 03 FC 00 28 
		cSendBuffer[0] = 0x02;
		cSendBuffer[1] = 0x01;
		cSendBuffer[2] = 0x00;
		cSendBuffer[3] = 0x04;
		cSendBuffer[4] = 0x29;
		cSendBuffer[5] = 0x03;
		cSendBuffer[6] = 0xFC;
		cSendBuffer[7] = 0x00;
		cSendBuffer[8] = 0x28;
		g_serial_ic.write_data(cSendBuffer, 9);
	}
	else if(n == 4)	//获取序列号
	{
		cSendBuffer[0]=0x7C;
		cSendBuffer[1]=0xAA;
		cSendBuffer[2]=0xBB;
		cSendBuffer[3]=0x80;
		cSendBuffer[4]=0x20;
		cSendBuffer[5]=CheckSum((PUCHAR)cSendBuffer, 5);
		g_serial_ic.write_data(cSendBuffer, 6);
	}
	else if(n == 5)//识别卡
	{
		cSendBuffer[0]=0x7C;
		cSendBuffer[1]=65535%256;
		cSendBuffer[2]=65535/256;
		cSendBuffer[3]=0x01;
		cSendBuffer[4]=0x08;
		cSendBuffer[5]=CheckSum((PUCHAR)cSendBuffer,5);
		g_serial_ic.write_data(cSendBuffer, 6);
	}
}

DWORD CDlgRFID::THDReadCard(LPVOID lpThreadParameter)
{
	CDlgRFID* pDlg = (CDlgRFID*)lpThreadParameter;
	while(pDlg->m_bTHDIC)
	{
		Sleep(3*1000);
		//pDlg->SendCMD(2);	//检测卡是否存在
	}

	return 1;
}

bool CDlgRFID::check_cardexist(char* pBuffer, int nLen)
{
	//YES:02 00 01 03 00 00 04 04 
	//NO:02 00 01 03 0B 00 00 0B 
	if( pBuffer[0] == 0x02
		&& pBuffer[1] == 0x00
		&& pBuffer[2] == 0x01
		&& pBuffer[3] == 0x03
		&& pBuffer[4] == 0x00
		&& pBuffer[5] == 0x00
		&& pBuffer[6] == 0x04
		&& pBuffer[7] == 0x04
		)
	{
		return true;
	}
	else
	{
		//m_bIsGetICNO = FALSE;
		//Invalidate();
		return false;
	}
}
bool CDlgRFID::check_cardnumber(char* pBuffer, int nLen)
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

int CDlgRFID::CheckSum(unsigned char * uBuff,int uBuffLen)
{
    int i;
    int Sum_check;
    Sum_check = 0;
    for(i=0 ; i < uBuffLen ; i++)
	{
        Sum_check = Sum_check + uBuff[i];
        if (Sum_check > 255 ) Sum_check = Sum_check - 256;
    }
    Sum_check = 256 - Sum_check;
    return Sum_check;
}

void CDlgRFID::DrawBarCode(CDC* pmemDC)
{
	CString str;
	CRect rect(70,80,750,115);
	DrawBtnText(pmemDC, rect,m_csCardNO,smallfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);
	/*for(int i=0; i<m_nBarCodeCount; i++)
	{
		str = &m_cBarCode[i][0];
		str = str.Left(str.GetLength()-1);
		DrawBtnText(pmemDC, rect,str,smallfont,RGB(255,255,255),DT_LEFT | DT_VCENTER);
		rect.OffsetRect(0,35);
	}*/
}
