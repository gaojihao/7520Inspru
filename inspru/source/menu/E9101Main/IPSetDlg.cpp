// IPSetDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "IPSetDlg.h"
#include "..\imm\IMMMng.h"
#include "E9101MainDlg.h"
#include "../MutiLanguage/CGDICommon.h"
#include "../../SQLite3/CppSQLite3.h"
#include "../BrightnessSetDlg.h"
#include "../Msg/MsgQueue.h"

#define  SOUND_KEYCLICK    _T("\\Flashdrv Storage\\menu\\Style\\KeySound.wav\0")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIPSetDlg dialog
extern BOOL g_bLight;

CIPSetDlg::CIPSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase( CIPSetDlg::IDD, pParent )
{
	//{{AFX_DATA_INIT(CIPSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIPSetDlg, CDialog)
	//{{AFX_MSG_MAP(CIPSetDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPSetDlg message handlers

BOOL CIPSetDlg::OnInitDialog()
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

	enSel = NOTHING;

	if( m_IniFile.Load( _T("\\Flashdrv Storage\\menu\\GPRS.ini") ) )
	{
		m_IniFile.GetString(_T("Server"),_T("IP"),m_strIP);
		m_IniFile.GetString(_T("Server"),_T("Port"),m_strPort);
		m_IniFile.GetString(_T("Server"),_T("DeviceID"),m_strDeviceID);
		m_IniFile.GetString(_T("Server"),_T("Type"),m_strProtocol);
	}
	
	InitControl();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIPSetDlg::InitGdi()
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
		
		
		PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_INPUT3_N,true);
		m_stBtnNormalBMP   = pSkin->hBitmap;
		
		pSkin = g_pResMng->RequestBmp(BG_INPUT3_P, true);
		m_stBtnPressBMP = pSkin->hBitmap;
		
		
		m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
		m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
		
		ReleaseDC(m_pDC);
}

void CIPSetDlg::InitControl()
{
	int i;
	for(i=0; i<20; i++)
	{
		m_item[i].nState = BTN_STATE_NORMAL;
	}

	m_strProtocol.TrimRight();
	m_strProtocol.TrimLeft();
	if(m_strProtocol.Compare(_T("UDP")) == 0)
	{
		m_item[17].nState = BTN_STATE_DOWN;		//UDP
		m_item[18].nState = BTN_STATE_NORMAL;	//TCP
	}
	else if(m_strProtocol.Compare(_T("TCP")) == 0)
	{
		m_item[17].nState = BTN_STATE_NORMAL;
		m_item[18].nState = BTN_STATE_DOWN;
	}
	else
	{
		m_item[17].nState = BTN_STATE_NORMAL;
		m_item[18].nState = BTN_STATE_NORMAL;
	}

	//OK button.
	m_item[0].rect.left   = 557;
	m_item[0].rect.top    = 203;
	m_item[0].rect.right  = 699;
	m_item[0].rect.bottom = 268;

	//Input.

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

	//Edit area. IP.
	m_item[14].rect.left   = 144;
	m_item[14].rect.top    = 103;
	m_item[14].rect.right  = 469;
	m_item[14].rect.bottom = 155;

	//Edit area. Port.
	m_item[15].rect.left   = 144; 
	m_item[15].rect.top    = 158;
	m_item[15].rect.right  = 469;
	m_item[15].rect.bottom = 209;

	//Edit area. DeviceID.
	m_item[16].rect.left   = 144;
	m_item[16].rect.top    = 212;
	m_item[16].rect.right  = 469;
	m_item[16].rect.bottom = 264;
	
	//UDP.
	m_item[17].rect.left   = 500;
	m_item[17].rect.top    = 139;
	m_item[17].rect.right  = 577;
	m_item[17].rect.bottom = 197;

	//TCP.
	m_item[18].rect.left   = 639;
	m_item[18].rect.top    = 139;
	m_item[18].rect.right  = 716;
	m_item[18].rect.bottom = 197;

	//Exit.
	m_item[19].rect.left   = 740;
	m_item[19].rect.top    = 0;
	m_item[19].rect.right  = 800;
	m_item[19].rect.bottom = 60;

	m_csTitile = _T("IP设置");
}

void CIPSetDlg::OnExit()
{
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

void CIPSetDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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


void CIPSetDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CIPSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);
	int i;
	for(i=0; i<20; i++)
	{
		//绘制背景
		DrawItems(&memDC, m_item[i].rect, m_item[i].nState);
	}

	CString str;
	CResString strRes;
	CRect   rcItem;

	//IP.
	rcItem = m_item[14].rect;
	rcItem.left = 65;
	//strRes.LoadString(RES_BUTTON_CENTER);
	str = _T("IP:");
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[14].rect.left + 10;
	DrawBtnText( &memDC, rcItem, m_strIP, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	//Port.
	rcItem = m_item[15].rect;
	rcItem.left = 65;
	//strRes.LoadString(RES_BUTTON_EMERGENCY);
	str = _T("Port:");
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[15].rect.left + 10;
	DrawBtnText( &memDC, rcItem, m_strPort, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	//DeviceID.
	rcItem = m_item[16].rect;
	rcItem.left = 65;
	//strRes.LoadString(RES_BUTTON_LOCAL);
	str = _T("Device:");
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem.left = m_item[16].rect.left + 10;
	DrawBtnText( &memDC, rcItem, m_strDeviceID, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	//Protocol.
	rcItem = CRect(508,110,654,143);
	str = _T("协议:");
	DrawBtnText( &memDC, rcItem, str, smallfont, RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem = m_item[17].rect;
	rcItem.left = rcItem.right + 10;
	str = _T("UDP:");
	DrawBtnText( &memDC, rcItem, str, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);
	rcItem = m_item[18].rect;
	rcItem.left = rcItem.right + 10;
	str = _T("TCP:");
	DrawBtnText( &memDC, rcItem, str, smallfont,RGB(255,255,255),DT_VCENTER|DT_LEFT);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	
	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);
	
	memDC.SelectObject(pOldBmp);
}

void CIPSetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	BOOL bRc = false;
	for(i=14; i<17; i++)
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
				enSel = NOTHING;	//Unselect.
			}
		}
	}
	for( i = 0; i < 17; i++ )
	{
		if(PtInRect(&m_item[i].rect,point))
		{
			if(m_item[i].nState == BTN_STATE_NORMAL)
			{
				m_item[i].nState = BTN_STATE_DOWN;
				//::PlaySound(SOUND_KEYCLICK, NULL, SND_ASYNC | SND_NODEFAULT );
				PlayKeySound();
			}
		}
	}
	//UDP.
	if(PtInRect(&m_item[17].rect,point))
	{
		m_item[17].nState = BTN_STATE_DOWN;
		m_item[18].nState = BTN_STATE_NORMAL;
		//::PlaySound(SOUND_KEYCLICK, NULL, SND_ASYNC | SND_NODEFAULT );
		PlayKeySound();
	}
	//TCP.
	if(PtInRect(&m_item[18].rect,point))
	{
		m_item[17].nState = BTN_STATE_NORMAL;
		m_item[18].nState = BTN_STATE_DOWN;
		//::PlaySound(SOUND_KEYCLICK, NULL, SND_ASYNC | SND_NODEFAULT );
		PlayKeySound();
	}
	
	if(PtInRect(&m_item[19].rect,point))
	{
		if(m_item[19].nState == BTN_STATE_NORMAL)
		{
			m_item[19].nState = BTN_STATE_DOWN;
			//::PlaySound(SOUND_KEYCLICK, NULL, SND_ASYNC | SND_NODEFAULT );
			PlayKeySound();
		}
	}
	
	Invalidate();
	//SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}


void CIPSetDlg::ChangeDeviceID()
{
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
        ret = CIMMMng::Instance().IMMEditPhone(ID_CHINESE,(LPTSTR)(LPCTSTR)strName, g_enKeySnd,enSkin);
	}
	
	res = CIMMMng::Instance().IMMGetResult();
	
	if(res.strName == _T(""))
	{
	}
	else
	{
		m_strDeviceID =  res.strName;
	}
	Invalidate();

	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();
}


void CIPSetDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
 
	for(i=14; i<17; i++)
	{
		if(m_item[i].nState == BTN_STATE_DOWN )
		{
			//m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 14:	enSel = IP;				break;
			case 15:	enSel = PORT;			break;
			case 16:
				{
					enSel = NOTHING;
					ChangeDeviceID();
				}
				break;
			default:							break;
			}
		}
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

	if(m_item[19].nState == BTN_STATE_DOWN )
	{
		m_item[19].nState = BTN_STATE_NORMAL;
		OnExit();
	}
	
	Invalidate();
	//ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CIPSetDlg::OnInput(int number)
{
	switch(enSel)
	{
	case IP:
		{
			if(number == 11)
			{
				m_strIP += ",";
			}
			else if(number == 12)
			{
				m_strIP += ".";
			}
			else if(number == 13)
			{
				if(m_strIP.GetLength() > 0)
				{
					m_strIP.Delete(m_strIP.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strIP += chN;
			}
			break;
		}
	case PORT:
		{
			if(number == 11)
			{
				m_strPort += ",";
			}
			else if(number == 12)
			{
				m_strPort += ".";
			}
			else if(number == 13)
			{
				if(m_strPort.GetLength() > 0)
				{
					m_strPort.Delete(m_strPort.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strPort += chN;
			}
			break;
		}
	case DEVICEID:
		{
			if(number == 11)
			{
				m_strDeviceID += ",";
			}
			else if(number == 12)
			{
				m_strDeviceID += ".";
			}
			else if(number == 13)
			{
				if(m_strDeviceID.GetLength() > 0)
				{
					m_strDeviceID.Delete(m_strDeviceID.GetLength()-1, 1);
				}
			}
			else
			{
				char chN = number + 48;
				m_strDeviceID += chN;
			}
			break;
		}
	default:
		break;
	}

	Invalidate();
}

void CIPSetDlg::OnSave()
{	
	if( !CheckIP() )
	{
		MessageBox(_T("IP或者端口错误！"));
		return;
	}

	m_IniFile.SetString(_T("Server"),_T("IP"),m_strIP);
	m_IniFile.SetString(_T("Server"),_T("Port"),m_strPort);
	m_IniFile.SetString(_T("Server"),_T("DeviceID"),m_strDeviceID);

	if(m_item[17].nState == BTN_STATE_DOWN)
		m_strProtocol = _T("UDP");
	else
		m_strProtocol = _T("TCP");

	m_IniFile.SetString(_T("Server"),_T("Type"),m_strProtocol);

	
	char szIP[64];
	memset(szIP, 0, sizeof(szIP));
	ex_uni2char(m_strIP.GetBuffer(m_strIP.GetLength()), szIP, 64);

	char szSql[512];
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	sprintf(szSql, "update msg_buffer set ip = '%s', port = %d;", 
		szIP, m_stIP_PORT.m_port );
	db.execDML(szSql);
	db.close();

/*
	cex_file file;
	if( file.is_exist(PATH_TZ_BUFFER_IP_PORT) )
		file.remove(PATH_TZ_BUFFER_IP_PORT);
	file.open(PATH_TZ_BUFFER_IP_PORT, cex_file::m_write);
	file.write(&m_stIP_PORT, sizeof(m_stIP_PORT));
	file.close();
*/
	CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, WPARAM_IP_SETTING,1);

	OnExit();
}

BOOL CIPSetDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CIPSetDlg::CheckIP()
{
	char cIP_PORT[64];
	memset(cIP_PORT, 0, sizeof(cIP_PORT));
	ex_uni2char(m_strIP.GetBuffer(m_strIP.GetLength()), cIP_PORT, 64);
	
	//IP检查
	char* pIP = cIP_PORT;
	int nIP = atoi(pIP);
	if( nIP > 0xFF)
		return FALSE;
	m_stIP_PORT.m_IP[0] = nIP;

	for(int i=0; i<3; i++)
	{
		pIP = strchr(pIP, '.');
		pIP++;
		nIP = atoi(pIP);
		if( NULL==pIP || nIP>0xFF)
			return FALSE;
		m_stIP_PORT.m_IP[i+1] = nIP;
	}

	if(strchr(pIP, '.') != NULL)
		return FALSE;

	//端口检查
	int nPort = _ttoi(m_strPort.GetBuffer(m_strPort.GetLength()));
	if(  nPort > 0xFFFF )
		return FALSE;

	m_stIP_PORT.m_port = nPort;

	return TRUE;
}
