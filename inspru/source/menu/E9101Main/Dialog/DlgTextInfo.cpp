// SMSInfo.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "../ExTool/ex_basics.h"
#include "DlgTextInfo.h"

#include "../DlgConfirm.h"
#include "../DlgSetPin.h"
#include "../SMSSending.h"
#include "../../imm/IMMMng.h"
#include "../../MutiLanguage/CGDICommon.h"

#include "../E9101MainDlg.h"
#include "../BrightnessSetDlg.h"
#include "../Msg/MsgQueue.h"
#include "../DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTextInfo dialog

extern BOOL g_bLight;
void TTSSpeaking(CString csText);

BOOL CDlgTextInfo::m_bIsOpen	= FALSE;
UINT CDlgTextInfo::WM_DlgTextInfo_Refresh = RegisterWindowMessage(_T("WM_DlgTextInfo_Refresh"));

CDlgTextInfo::CDlgTextInfo(int nIndexCurr, CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgTextInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRecordIndex	= nIndexCurr;//设置当前信息的index
	m_nRecordCount	= 0;
}

void CDlgTextInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTextInfo, CDialogBase)
	//{{AFX_MSG_MAP(CMediaMain)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgTextInfo::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	SetWindowText(_T("DlgTextInfo_WINDOW_NAME"));

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	ShowPage(m_nRecordIndex);

	m_bIsOpen	= TRUE;

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, DLG_WIDTH, DLG_HIGHT, SWP_SHOWWINDOW);	//置顶显示

	return TRUE;
}

//变换语言和单位
BOOL CDlgTextInfo::OnLanguageChange()//变皮肤
{
	m_csTitile = _T("中心信息");
	
	for(int i=0;i<6;i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
		m_Item[i].chChar = _T("");
	}

	m_Item[0].chChar = _T("回复");
	m_Item[1].chChar = _T("");

	return TRUE;
}

BOOL CDlgTextInfo::OnSkinChange()
{
	return TRUE;
}

void CDlgTextInfo::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	//bk_disable_dc.CreateCompatibleDC(m_pDC);

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_SMS_CENTER_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BG_SMS_CENTER_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}


void CDlgTextInfo::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	//bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);

	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	//bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CDlgTextInfo::OnExit()
{
	m_bIsOpen	= FALSE;
	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CDlgTextInfo::InitControl()
{
	//回复
	m_Item[0].rect.left		= 224;
	m_Item[0].rect.top		= 396;
	m_Item[0].rect.right	= 369;
	m_Item[0].rect.bottom	= 480;

	//转发 
	m_Item[1].rect.left		= 0;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 0;
	m_Item[1].rect.bottom	= 0;

	//删除
	m_Item[2].rect.left		= 370;
	m_Item[2].rect.top		= 396;
	m_Item[2].rect.right	= 513;
	m_Item[2].rect.bottom	= 480;

	//前一个
	m_Item[3].rect.left		= 514;
	m_Item[3].rect.top		= 396;
	m_Item[3].rect.right	= 656;
	m_Item[3].rect.bottom	= 480;

	//下一个
	m_Item[4].rect.left		= 657;
	m_Item[4].rect.top		= 396;
	m_Item[4].rect.right	= 800;
	m_Item[4].rect.bottom	= 480;

	//Exit
	m_Item[5].rect.left		= 720;
	m_Item[5].rect.top		= 0;
	m_Item[5].rect.right	= 800;
	m_Item[5].rect.bottom	= 60;

	//状态
	m_Static[0].rect.left	= 104;
	m_Static[0].rect.top	= 87;
	m_Static[0].rect.right	= 396;
	m_Static[0].rect.bottom	= 125;

	//时间
	m_Static[1].rect.left	= 475;
	m_Static[1].rect.top	= 87;
	m_Static[1].rect.right	= 767;
	m_Static[1].rect.bottom	= 125;
	
	//内容
	m_Static[2].rect.left	= 35;
	m_Static[2].rect.top	= 150;
	m_Static[2].rect.right	= 758;
	m_Static[2].rect.bottom	= 376;
}

void CDlgTextInfo::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CDlgTextInfo::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgTextInfo::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
/*
	st_SMSCenter* pMsg = g_pSMSCenter->GetRecord(m_nCurrSMSIndex);

	if( !pMsg->m_replyType )
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		m_Item[0].chChar = _T("");
	}
*/
	for(int i=0;i<6;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect, str, CGDICommon::Instance()->bigbigfont(),
			RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}

	DrawBtnText(&memDC,m_Static[0].rect,m_Static[0].chChar,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	DrawBtnText(&memDC,m_Static[1].rect,m_Static[1].chChar,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	DrawBtnText(&memDC,m_Static[2].rect,m_Static[2].chChar,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT|DT_TOP|DT_WORDBREAK|DT_EDITCONTROL);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgTextInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i = 0;
	for( i = 0; i < 6; i++ )
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

	Invalidate();
	SetCapture();
	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgTextInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<6;i++)
	{
		if(m_Item[i].nState == BTN_STATE_DOWN )
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:	Reply();		break;
			case 2:	OnBtnDel();		break;
			case 3:	OnBtnPageUp();	break;
			case 4: OnBtnPageDown();break;
			case 5: OnExit();		break;
			default:
				break;
			}
		}
	}
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void  CDlgTextInfo::OnBtnPageUp()
{
	if(m_nRecordIndex <= 0)
	{
		m_nRecordIndex = 0;
		return;
	}
	m_nRecordIndex--;

	ShowPage(m_nRecordIndex);
	Invalidate();
}

void  CDlgTextInfo::OnBtnPageDown()
{
	if(m_nRecordIndex >= m_nRecordCount-1)
	{
		m_nRecordIndex = m_nRecordCount-1;
		return;
	}
	m_nRecordIndex++;

	ShowPage(m_nRecordIndex);
	Invalidate();
}

void  CDlgTextInfo::OnBtnDel()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_SEL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	if( CDlgConfirm::s_bOk )
	{
		CppSQLite3DB db;
		db.open(PATH_SQLITE_DB_808);	//打开数据库
		char szSql[512];
		sprintf(szSql, "delete from text_info where UID = %d;", m_nSMS_ID);
        db.execDML( szSql );
		db.close();

		OnExit();
	}
}

void  CDlgTextInfo::ShowPage(int nRecordIndex)
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库
	
	//查询记录总数量
	int m_nRecordCount = db.execScalar("select count(*) from text_info WHERE text_type = 0;");

	//在数据库中查询第nRecordIndex条数据
	char szSqlBuffer[512];
	sprintf(szSqlBuffer, "Select * From text_info WHERE text_type = 0 ORDER BY text_datatime DESC limit %d, 1;", nRecordIndex);
	CppSQLite3Query q = db.execQuery(szSqlBuffer);

	int nFlag = 0;

	if ( !q.eof() )
	{
		m_nSMS_ID = q.getIntField("UID");		//获取当前短信ID
		TCHAR  szBuf[300];
		m_Static[0].chChar = _T("中心信息");
		
		//短信内容
		memset(szBuf, 0, sizeof(szBuf) );
		ex_char2uni( q.fieldValue("text_content"), szBuf,300 );
		m_Static[2].chChar.Format(_T("%s"), szBuf);
		
		//时间
		m_Static[1].chChar = q.fieldValue("text_datatime");
	}
	//释放statement
	q.finalize();
	db.close();	//关闭数据库

	TTSSpeaking( m_Static[2].chChar );
}

void CDlgTextInfo::Reply()
{
	CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, MSG_TMN_TEXT_SMS, m_nSMS_ID);
	return;
}

BOOL  CDlgTextInfo::LoadSMSContext(CString& strContext)
{
	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	
	ImmReturn ret;
	IMMRESULT res;
	enSkinType enSkin = ID_SKIN2; //CSysConfig::Instance().GetSkinType();
	if(CIMMMng::Instance().Load() == TRUE)
	{
		CIMMMng::Instance().ImmSetKeySound( g_enKeySnd );
		CIMMMng::Instance().IMMInit(FourPoint3);
		if(CSysConfig::Instance().GetLanguage() == ID_ENGLISH)
			ret = CIMMMng::Instance().IMMCreateMessage(ID_ENGLISH);
		else
			ret = CIMMMng::Instance().IMMCreateMessage(ID_CHINESE);
	}
	else
	{
		MessageBox( _T("Load IMM has Failed!\n"), _T("System Tip"), MB_OK );
		return FALSE;
	}
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();
	res = CIMMMng::Instance().IMMGetResult();
	//14 确认
	if( ret == 14 )
	{
		if( res.strName.GetLength() > 0 )
		{
			strContext = res.strName;
			return TRUE;
		}
	}

	return FALSE;
}

LRESULT CDlgTextInfo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_DlgTextInfo_Refresh)
	{
		m_nRecordIndex	= wParam;	//最新的记录
		ShowPage(m_nRecordIndex);
		Invalidate();
	}

	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
