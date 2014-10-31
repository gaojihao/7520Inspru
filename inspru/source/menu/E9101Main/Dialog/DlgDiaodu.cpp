
#include "stdafx.h"
#include "e9101main.h"
#include "DlgDiaodu.h"
#include "../../MutiLanguage/CGDICommon.h"

#include "DlgTextInfoList.h"
#include "DlgQuestionList.h"
#include "DlgInfoMenu.h"
#include "DlgEventList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgDiaoDu::CDlgDiaoDu(CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgDiaoDu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDiaoDu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgDiaoDu::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDiaoDu)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiaoDu, CDialogBase)
	//{{AFX_MSG_MAP(CDlgDiaoDu)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiaoDu message handlers

	//变换语言和单位
BOOL CDlgDiaoDu::OnLanguageChange()
{
	CResString str;

	m_csTitile = _T("调度信息");

	return TRUE;
}

//变皮肤
BOOL CDlgDiaoDu::OnSkinChange()
{
	return TRUE;
}

void CDlgDiaoDu::InitGdi()
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
	
	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BMP_DIAODU_INFO_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;

	pSkin = g_pResMng->RequestBmp(BMP_DIAODU_INFO_P, true);
	m_stBtnPressBMP = pSkin->hBitmap;

	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CDlgDiaoDu::OnExit()
{
	//bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	//DeleteObject(m_stBtnDisableBMP);	
	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	//bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bigfont.DeleteObject();
	smallfont.DeleteObject();

	bitmap.DeleteObject();
	memDC.DeleteDC();

	CDialogBase::EndDialog(0);
}

void CDlgDiaoDu::InitControl()
{
	for (int i =0; i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	//文本信息
	m_Item[0].rect.left		= 190;
	m_Item[0].rect.top		= 145;
	m_Item[0].rect.right	= 325;
	m_Item[0].rect.bottom	= 275;
	m_Item[0].chChar		= _T("文本信息");
	//提问应答
	m_Item[1].rect.left		= 335;
	m_Item[1].rect.top		= 145;
	m_Item[1].rect.right	= 466;
	m_Item[1].rect.bottom	= 275;
	m_Item[1].chChar		= _T("提问应答");
	//事件
	m_Item[2].rect.left		= 478;
	m_Item[2].rect.top		= 145;
	m_Item[2].rect.right	= 611;
	m_Item[2].rect.bottom	= 275;
	m_Item[2].chChar		= _T("事件");

	//Exit
	m_Item[3].rect.left		= 720;
	m_Item[3].rect.top		= 0;
	m_Item[3].rect.right	= 800;
	m_Item[3].rect.bottom	= 60;

	//信息服务
	m_Item[4].rect.left		= 62;
	m_Item[4].rect.top		= 145;
	m_Item[4].rect.right	= 167;
	m_Item[4].rect.bottom	= 275;
	m_Item[4].chChar		= _T("信息服务");

	//菜单点播
	m_Item[5].rect.left		= 635;
	m_Item[5].rect.top		= 145;
	m_Item[5].rect.right	= 739;
	m_Item[5].rect.bottom	= 275;
	m_Item[5].chChar		= _T("菜单点播");
}

void CDlgDiaoDu::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
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

void CDlgDiaoDu::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
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

void CDlgDiaoDu::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0; i<BTN_COUNT; i++)
	{
		//绘制背景
		str		= m_Item[i].chChar;
		Rect	= m_Item[i].rect;
		state	= m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		Rect.bottom += 35;
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->smallfont(),
			RGB(255,255,255),DT_CENTER|DT_BOTTOM);
	}

	DrawBtnText(&memDC, g_retTitile, m_csTitile, CGDICommon::Instance()->bigbigfont(), 
		RGB(255,255,255), DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgDiaoDu::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for(int i=0; i<BTN_COUNT; i++)
	{
		if( PtInRect(&m_Item[i].rect,point) )
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

void CDlgDiaoDu::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0; i<BTN_COUNT; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			
			switch( i )
			{
			case 0://文本
				OnTextInfo();
				break;
			case 1://提问
				OnQuestion();
				break;
			case 2://点播
				OnInfoMenu();
				break;
			case 3:
				OnExit();
				break;
			case 4://信息服务
				OnInfoServer();
				break;
			case 5:
				OnMenu();
				break;
			}
		}
	}

	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp( nFlags, point );
}

BOOL CDlgDiaoDu::OnInitDialog() 
{
	CDialogBase::OnInitDialog();

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();
	
	return TRUE;
}

//文本信息下发
VOID	CDlgDiaoDu::OnTextInfo()
{
	CDlgTextInfoList dlg;
	CDlgTextInfoList::m_iInfoType  = 0;
	dlg.DoModal();
}

//提问
VOID	CDlgDiaoDu::OnQuestion()
{
	CDlgQuestionList dlg;
	dlg.DoModal();
}
//事件
VOID CDlgDiaoDu::OnInfoMenu()
{
	CDlgEventList dlg;
	dlg.DoModal();
}

VOID CDlgDiaoDu::OnInfoServer()
{
	CDlgTextInfoList dlg;
	CDlgTextInfoList::m_iInfoType  = 1;
	dlg.DoModal();
}

VOID CDlgDiaoDu::OnMenu()
{
	CDlgInfoMenu dlg;
	dlg.DoModal();
}