// AlmanacDiagramCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AlmanacDiagramCtrl.h"
#include "../ExTool/MemDC.h"
#include "e9101main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Circle_top	20
#define PI			3.1415926
#define Icon_Size	24

/////////////////////////////////////////////////////////////////////////////
// CAlmanacDiagramCtrl
//************************************
// Method:    CAlmanacDiagramCtrl
// FullName:  CAlmanacDiagramCtrl::CAlmanacDiagramCtrl
// Access:    public 
// Returns:   
// Qualifier:
//************************************

CAlmanacDiagramCtrl::CAlmanacDiagramCtrl()
{
	CString str = _T("ESWN");
	SetLanguage(str);

	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet	= DEFAULT_CHARSET;
	logFont.lfUnderline = FALSE;
	logFont.lfHeight = 15;
	m_TextFont.CreateFontIndirect(&logFont);
	logFont.lfHeight = 12;
	m_StarFont.CreateFontIndirect(&logFont);
	m_LinePen.CreatePen(PS_SOLID, 2, RGB(200, 200, 200));

	m_hSigStrong = AfxGetApp()->LoadIcon(IDI_ICON_GPS_STRONG);
	m_hSigWeak = AfxGetApp()->LoadIcon(IDI_ICON_GPS_WEAK);
}

CAlmanacDiagramCtrl::~CAlmanacDiagramCtrl()
{
	m_TextFont.DeleteObject();
	m_StarFont.DeleteObject();
	m_LinePen.DeleteObject();
	DestroyIcon(m_hSigStrong);
	DestroyIcon(m_hSigWeak);
}


BEGIN_MESSAGE_MAP(CAlmanacDiagramCtrl, CStatic)
	//{{AFX_MSG_MAP(CAlmanacDiagramCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlmanacDiagramCtrl message handlers

void CAlmanacDiagramCtrl::OnPaint() 
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);
	CMemDC MemDc(&dc, &rect);
	rect.InflateRect(0,0,0,0);

	DrawSphereCircle(&MemDc, rect);
	DrawStarPos(&MemDc, rect);
}

void CAlmanacDiagramCtrl::SetStarData(SStarData *pData, int num)
{
	ASSERT(pData != NULL);
	m_iStarNum = num;
	for (int i=0; i<num; i++)
	{
		m_StarData[i] = pData[i];
	}
}

void CAlmanacDiagramCtrl::DrawSphereCircle(CDC *pDc, const CRect &rect)
{
	ASSERT(pDc != NULL);

	int width = rect.Width() / 5;
	
	m_iRadius = 2 * width;
	m_centerCircle.x = rect.Width() / 2;
	m_centerCircle.y = Circle_top + m_iRadius;
	
	CPen *OldPen = pDc->SelectObject(&m_LinePen);

	CRect rtCircle(m_centerCircle.x-m_iRadius, m_centerCircle.y-m_iRadius, 
				   m_centerCircle.x+m_iRadius, m_centerCircle.y+m_iRadius);
	pDc->Ellipse(&rtCircle);

	rtCircle.DeflateRect(rtCircle.Width()/4, rtCircle.Height()/4);
	pDc->Ellipse(&rtCircle);
	
	pDc->SetPixel(m_centerCircle, RGB(0, 0, 0));

	pDc->SelectObject(OldPen);
}

void CAlmanacDiagramCtrl::DrawStarPos(CDC *pDc, const CRect &rect)
{
	ASSERT(pDc != NULL);
	if (pDc==NULL || m_strDir.GetLength()!=4)
	{
		return;
	}
	CFont *OldFont = (CFont*)pDc->SelectObject(&m_TextFont);
	for (int j=90; j<=360; j+=90)
	{
		POINT pt;
		pt.x = m_centerCircle.x + m_iRadius * sin(j*PI/180);
		pt.y = m_centerCircle.y - m_iRadius * cos(j*PI/180);
		CRect rt;
		rt.SetRect(pt.x-7, pt.y-7, pt.x+7, pt.y+7);
		CString str;
		str = m_strDir.Mid(j/90-1, 1);
		pDc->DrawText(str, &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}

	pDc->SetBkMode(TRANSPARENT);  
	pDc->SelectObject(&m_StarFont);
	for (int i=0; i<m_iStarNum; i++)
	{
		POINT pt = CalStarPos(m_StarData[i]);
		if (m_StarData[i].tracked == 1)
		{
			pDc->DrawIcon(pt.x-Icon_Size/2, pt.y-Icon_Size/2, m_hSigStrong);
		}
		else
		{
			pDc->DrawIcon(pt.x-Icon_Size/2, pt.y-Icon_Size/2, m_hSigWeak);
		}
		CRect rt(pt.x-Icon_Size/2, pt.y-Icon_Size/2, 
				 pt.x+Icon_Size/2, pt.y+Icon_Size/2);
		CString str;
		str.Format(_T("%d"), m_StarData[i].id);
		pDc->DrawText(str, &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}
	pDc->SelectObject(OldFont);
}

POINT CAlmanacDiagramCtrl::CalStarPos(SStarData star)
{
	POINT pt;
	pt.x = (LONG)(m_centerCircle.x + (m_iRadius * (90-star.horizon) / 90)
			* sin(star.direction / 180.0 * PI));   
	pt.y = (LONG)(m_centerCircle.y - (m_iRadius * (90-star.horizon) / 90)
			* cos(star.direction / 180.0 * PI));
	return pt;
}

void CAlmanacDiagramCtrl::SetLanguage(CString str)
{
	m_strDir = str;
}
