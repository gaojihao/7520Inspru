// PWDInputDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "DlgICLogin.h"

#include "../IPSetDlg.h"
#include "../GSMDialMain.h"
#include "../E9101MainDlg.h"
#include "../../SQLite3/CppSQLite3.h"
#include "../DlgConfirm2.h"
#include "../Msg/MsgQueue.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern serial_win32	g_serial_ic;

CDlgICLogin::CDlgICLogin(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgICLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgICLogin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTHDIC = TRUE;
	m_bIsGetICNO = FALSE;
	memset(&m_stICCard, NULL, sizeof(m_stICCard));
}

void CDlgICLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgICLogin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgICLogin, CDialog)
	//{{AFX_MSG_MAP(CDlgICLogin)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgICLogin message handlers

BOOL CDlgICLogin::OnInitDialog()
{
	CDialogBase::OnInitDialog();

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
	ConnectCardReader(CSysConfig::Instance().GetComICCard(), 38400);
	SendCMD(1);	//蜂鸣

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgICLogin::InitGdi()
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

		PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_INPUT1_N,true);
		m_stBtnNormalBMP   = pSkin->hBitmap;
		
		pSkin = g_pResMng->RequestBmp(BG_INPUT1_P, true);
		m_stBtnPressBMP = pSkin->hBitmap;
		
		
		m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
		m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
		
		ReleaseDC(m_pDC);
}

void CDlgICLogin::InitControl()
{
	int i;
	for(i=0; i<16; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}

	//OK button.
	m_item[0].rect.left		= 577;
	m_item[0].rect.top		= 169;
	m_item[0].rect.right	= 714;
	m_item[0].rect.bottom	= 231;
	//1
	m_item[1].rect.left		= 5;
	m_item[1].rect.top		= 287;
	m_item[1].rect.right	= 117;
	m_item[1].rect.bottom	= 382;
	//2
	m_item[2].rect.left		= 118;
	m_item[2].rect.top		= 287;
	m_item[2].rect.right	= 230;
	m_item[2].rect.bottom	= 382;
	//3
	m_item[3].rect.left		= 231;
	m_item[3].rect.top		= 287;
	m_item[3].rect.right	= 343;
	m_item[3].rect.bottom	= 382;
	//4
	m_item[4].rect.left		= 344;
	m_item[4].rect.top		= 287;
	m_item[4].rect.right	= 456;
	m_item[4].rect.bottom	= 382;
	//5
	m_item[5].rect.left		= 457;
	m_item[5].rect.top		= 287;
	m_item[5].rect.right	= 569;
	m_item[5].rect.bottom	= 382;
	//6
	m_item[6].rect.left		= 5;
	m_item[6].rect.top		= 383;
	m_item[6].rect.right	= 117;
	m_item[6].rect.bottom	= 480;
	//7
	m_item[7].rect.left		= 118;
	m_item[7].rect.top		= 383;
	m_item[7].rect.right	= 230;
	m_item[7].rect.bottom	= 480;
	//8
	m_item[8].rect.left		= 231;
	m_item[8].rect.top		= 383;
	m_item[8].rect.right	= 343;
	m_item[8].rect.bottom	= 480;
	//9
	m_item[9].rect.left		= 344;
	m_item[9].rect.top		= 383;
	m_item[9].rect.right	= 456;
	m_item[9].rect.bottom	= 480;
	//0
	m_item[10].rect.left	= 457;
	m_item[10].rect.top		= 383;
	m_item[10].rect.right	= 569;
	m_item[10].rect.bottom	= 480;
	//*
	m_item[11].rect.left	= 570;
	m_item[11].rect.top		= 287;
	m_item[11].rect.right	= 682;
	m_item[11].rect.bottom	= 382;
	//#
	m_item[12].rect.left	= 683;
	m_item[12].rect.top		= 287;
	m_item[12].rect.right	= 795;
	m_item[12].rect.bottom	= 382;
	//←
	m_item[13].rect.left	= 570;
	m_item[13].rect.top		= 383;
	m_item[13].rect.right	= 795;
	m_item[13].rect.bottom	= 480;
	
	//Edit area. PWD.
	m_item[14].rect.left   = 250;
	m_item[14].rect.top    = 175;
	m_item[14].rect.right  = 565;
	m_item[14].rect.bottom = 224;
	
	//Exit.
	m_item[15].rect.left   = 740;
	m_item[15].rect.top    = 0;
	m_item[15].rect.right  = 800;
	m_item[15].rect.bottom = 60;
}

void CDlgICLogin::OnExit()
{
	m_bTHDIC = FALSE;
	if(m_hTHDICCardWatch != NULL)
		TerminateThread(m_hTHDICCardWatch,0);
	if(m_hTHDReadCard != NULL)
		TerminateThread(m_hTHDReadCard,0);
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

void CDlgICLogin::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CDlgICLogin::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgICLogin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	int i;
	for(i=0; i<15; i++)
	{
		//绘制背景
		DrawItems(&memDC, m_item[i].rect, m_item[i].nState);
	}

	CString str;
	CResString strRes;
	CRect   rcItem;
	rcItem = m_item[14].rect;
	rcItem.left = 60;

	str = TEXT("请输入密码：");

	DrawBtnText( &memDC, rcItem, str, bigfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[14].rect.left + 8;
	m_strPWDSHow = TEXT("");
	for(i=0; i<m_strPWDInput.GetLength(); i++)
	{
		m_strPWDSHow += TEXT("*");
	}
	DrawBtnText( &memDC, rcItem, m_strPWDSHow, bigfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	//Title.
	str = TEXT("IC卡应用");
	DrawBtnText(&memDC,g_retTitile,str,bigfont,RGB(255,255,255),DT_CENTER | DT_VCENTER);

	if(m_bIsGetICNO)	//刷卡成功
		str = TEXT("卡号:") + m_csCardNO;
	else
		str = TEXT("请刷卡!");

	if( !CSysConfig::Instance().GetICCard() )
		str = TEXT("设备未连接!");
	
	DrawBtnText( &memDC, CRect(240, 110, 576, 160), str, bigfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CDlgICLogin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i = 0;
	BOOL bRc = false;
	for(i=14; i<15; i++)
	{
		for(int j=1; j<14; j++)
		{
			bRc = bRc || PtInRect(&m_item[j].rect,point);
		}
		if(!bRc)
		{
			if(m_item[i].nState == BTN_STATE_DOWN)
			{
				m_item[i].nState = BTN_STATE_NORMAL;
				//enSel = NOTHING;	//Unselect.
			}
		}
	}
	for( i = 0; i < 16; i++ )
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

void CDlgICLogin::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
 
	if(m_item[14].nState == BTN_STATE_DOWN )
	{
		enSel = PWDINPUT;
	}
 
	for(i=0; i<14; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			m_item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:		OnSave();		break;	//OK
			case 1:		OnInput(1);		break;
			case 2:		OnInput(2);		break;
			case 3:		OnInput(3);		break;
			case 4:		OnInput(4);		break;
			case 5:		OnInput(5);		break;
			case 6:		OnInput(6);		break;
			case 7:		OnInput(7);		break;
			case 8:		OnInput(8);		break;
			case 9:		OnInput(9);		break;
			case 10:	OnInput(0);		break;
			case 11:	OnInput(11);	break;
			case 12:	OnInput(12);	break;
			case 13:	OnInput(13);	break;
			default:					break;
			}
		}
	}

	if(m_item[15].nState == BTN_STATE_DOWN )
	{
		m_item[15].nState = BTN_STATE_NORMAL;
		OnExit();
	}
	
	Invalidate();
	//ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgICLogin::OnInput(int number)
{	
	switch(enSel)
	{
	case PWDINPUT:
		if(number == 11)
		{
			//m_strPWDInput += ",";
		}
		else if(number == 12)
		{
			//m_strPWDInput += ".";
		}
		else if(number == 13)
		{
			if(m_strPWDInput.GetLength() > 0)
			{
				m_strPWDInput.Delete(m_strPWDInput.GetLength()-1, 1);
			}
		}
		else
		{
			if( m_strPWDInput.GetLength() > 5)
				return;

			char chN = number + 48;
			m_strPWDInput += chN;
		}
		break;
	default:
		break;
	}

	Invalidate();
}

void CDlgICLogin::OnSave()
{
	if( !m_bIsGetICNO )
	{
		CDlgConfirm2::m_stTitle = _T("请刷卡后再输入密码!");
		CDlgConfirm2 dlg;
		dlg.DoModal();
		return;
	}

	if( m_strPWDInput.IsEmpty() )
	{
		CDlgConfirm2::m_stTitle = _T("密码不能为空!");
		CDlgConfirm2 dlg;
		dlg.DoModal();
		return;
	}
	
	memset(m_stICCard.m_PWD, 0, sizeof(m_stICCard.m_PWD));
	ex_uni2char(m_strPWDInput.GetBuffer(m_strPWDInput.GetLength()), 
		m_stICCard.m_PWD, 
		m_strPWDInput.GetLength());
	
	char szSql[512];
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(szSql, "update msg_buffer set ic_card_ID = '%s', ic_card_PWD = '%s';", 
		m_stICCard.m_ID, m_stICCard.m_PWD );
	db.execDML(szSql);
	db.close();

	if(CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, WPARAM_IC_CARD,1))
	{
		OnExit();
	}
	
	return;
}

bool CDlgICLogin::CheckPWD()
{
	if( !m_IniFile.Load( _T("\\Flashdrv Storage\\menu\\GSM.dll") ) )
	{
		return false;
	}
	
	if(m_strFilePWD.GetLength() == 0)
		return true;
	
	if(m_strFilePWD == m_strPWDInput)
		return true;
	else
		return false;
}

BOOL CDlgICLogin::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}

bool CDlgICLogin::ConnectCardReader(int n_port, int n_baud)
{
	if ( g_serial_ic.is_open() )
		return true;

	if ( !g_serial_ic.open(n_port, n_baud))
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

	m_hTHDReadCard = CreateThread(NULL,
							0,
							THDReadCard,
							this,
							0,
							&thrd_id );
	if ( m_hTHDReadCard == 0 )
		return false;

	return true;
}

DWORD CDlgICLogin::THDICCardWatch(LPVOID lpThreadParameter)
{
	CDlgICLogin* pDlg = (CDlgICLogin*)lpThreadParameter;
	char		cBuffer[100];
	while(pDlg->m_bTHDIC)
	{
		memset(cBuffer, NULL, sizeof(cBuffer));
		int nLenRead = g_serial_ic.read_data(cBuffer,20);
		if ( nLenRead == 0 )
		{
			Sleep(50);
			continue;
		}
		pDlg->parse_icdata(cBuffer, nLenRead);
	}
	return 1;
}

void CDlgICLogin::parse_icdata(char* pBuffer, int nLen)
{
	TRACE0("\n--------16进制数据-------------------------------------\n");
	for(int i=0; i<nLen; i++)
	{
		TRACE1("%02X ", (UCHAR)pBuffer[i]);
		if( (i+1)%20 == 0 && (i+1) != nLen )
			TRACE0("\n");
	}
	TRACE0("\n########GSM data end###################################\n");
	if( check_cardexist(pBuffer, nLen) )
	{
		SendCMD(3);	//读卡号
		return;
	}
	else if( check_cardnumber(pBuffer, nLen) )
	{//02 05 00 59 00 1E EC 9C	(59 00 1E EC)
		memset(&m_stICCard, NULL, sizeof(m_stICCard));
		for(int i=5; i<nLen; i++)
			sprintf(m_stICCard.m_ID, ("%03d%03d%03d%03d"), 
			(UCHAR)pBuffer[3], 
			(UCHAR)pBuffer[4], 
			(UCHAR)pBuffer[5], 
			(UCHAR)pBuffer[6]);
		m_csCardNO = m_stICCard.m_ID;
		m_bIsGetICNO = TRUE;
		Invalidate();
		return;
	}
}

void CDlgICLogin::SendCMD(int n)
{
	//02 01 00 05 00 0C F3 00 64 63
	//02 01 00 04 29 10 42 FD 00 28 
	//02 01 00 04 29 03 FC 00 28 
	char cSendBuffer[100];
	memset(cSendBuffer, NULL, sizeof(cSendBuffer));
	if(n == 1)	//beep
	{
		cSendBuffer[0] = 0x02;
		cSendBuffer[1] = 0x05;
		cSendBuffer[2] = 0x00;
		cSendBuffer[3] = 0x0C;
		cSendBuffer[4] = 0xF3;
		cSendBuffer[5] = 0x00;
		cSendBuffer[6] = 0x64;
		cSendBuffer[7] = 0x9C;
		g_serial_ic.write_data(cSendBuffer, 8);
	}
	else if(n == 2)//判断卡是否存在
	{//02 04 00 29 10 00 D4
		cSendBuffer[0] = 0x02;
		cSendBuffer[1] = 0x04;
		cSendBuffer[2] = 0x00;
		cSendBuffer[3] = 0x29;
		cSendBuffer[4] = 0x10;
		cSendBuffer[5] = 0x42;
		cSendBuffer[6] = 0x00;
		cSendBuffer[7] = 0xD4;
		g_serial_ic.write_data(cSendBuffer, 8);
	}
	else if(n == 3)//读卡号
	{//02 04 00 29 03 00 D3 
		cSendBuffer[0] = 0x02;
		cSendBuffer[1] = 0x04;
		cSendBuffer[2] = 0x00;
		cSendBuffer[3] = 0x29;
		cSendBuffer[4] = 0x03;
		cSendBuffer[5] = 0x00;
		cSendBuffer[6] = 0xD3;
		g_serial_ic.write_data(cSendBuffer, 7);
	}
}

DWORD CDlgICLogin::THDReadCard(LPVOID lpThreadParameter)
{
	CDlgICLogin* pDlg = (CDlgICLogin*)lpThreadParameter;
	while(pDlg->m_bTHDIC)
	{
		Sleep(2*1000);
		pDlg->SendCMD(2);	//检测卡是否存在
	}

	return 1;
}

bool CDlgICLogin::check_cardexist(char* pBuffer, int nLen)
{
	//YES:02 00 01 03 00 00 04 04 
	//NO:02 00 01 03 0B 00 00 0B 
	if( (UCHAR)pBuffer[0] == 0x02
		&& (UCHAR)pBuffer[1] == 0x03
		&& (UCHAR)pBuffer[2] == 0x00
		&& (UCHAR)pBuffer[3] == 0x00
		&& (UCHAR)pBuffer[4] == 0x04
		&& (UCHAR)pBuffer[5] == 0xFB
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
bool CDlgICLogin::check_cardnumber(char* pBuffer, int nLen)
{
	//02 05 00 59 00 1E EC 9C	(59 00 1E EC)
	if( (UCHAR)pBuffer[0] == 0x02
		&& (UCHAR)pBuffer[1] == 0x05
		&& (UCHAR)pBuffer[2] == 0x00
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