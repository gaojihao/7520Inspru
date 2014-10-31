// SatelliteDiagramCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SatelliteDiagramCtrl.h"
#include "../ExTool/MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Satellite_Signal_Height		15
#define Satellite_Signal_Space		1
#define Satellite_Signal_ColNum		6

/////////////////////////////////////////////////////////////////////////////
// CSatelliteDiagramCtrl

CSatelliteDiagramCtrl::CSatelliteDiagramCtrl()
{
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet	= DEFAULT_CHARSET;
	logFont.lfUnderline = FALSE;
	logFont.lfWeight = 560;
	logFont.lfHeight = 14;
	m_iStarNum = 0;
	m_TextFont.CreateFontIndirect(&logFont);

	m_LinePen.CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
	m_SigPen.CreatePen(PS_SOLID, Satellite_Signal_Height, RGB(41, 44, 123));
	m_WhitePen.CreatePen(PS_SOLID, Satellite_Signal_Height-4, RGB(255, 255, 255));
}

CSatelliteDiagramCtrl::~CSatelliteDiagramCtrl()
{
	m_LinePen.DeleteObject();
	m_WhitePen.DeleteObject();
	m_SigPen.DeleteObject();
	m_TextFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSatelliteDiagramCtrl, CStatic)
	//{{AFX_MSG_MAP(CSatelliteDiagramCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSatelliteDiagramCtrl message handlers

void CSatelliteDiagramCtrl::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect(&rect);
	CMemDC MemDc(&dc, &rect);
	
	DrawSigGrid(&MemDc, rect);
	DrawSignal(&MemDc, rect);
}

void CSatelliteDiagramCtrl::SetStarData(SStarData *pData, int num)
{
	ASSERT(pData != NULL);
	m_iStarNum = num;
	for (int i=0; i<num; i++)
	{
		m_StarData[i] = pData[i];
	}
}

void CSatelliteDiagramCtrl::DrawSigGrid(CDC *pDc, const CRect &rect)
{
	ASSERT(pDc != NULL);
	int width = rect.Width() / Satellite_Signal_ColNum + 1;
	
	CPen *OldPen = (CPen*)pDc->SelectObject(&m_LinePen);
	for (int i=-2; i<Satellite_Signal_ColNum; i++)
	{
		pDc->MoveTo(width*i, 0);
		pDc->LineTo(width*i, rect.bottom);
	}
	pDc->SelectObject(OldPen);
}

void CSatelliteDiagramCtrl::DrawSignal(CDC *pDc, const CRect &rect)
{
	ASSERT(pDc != NULL);
	int width = rect.Width() / Satellite_Signal_ColNum + 1;
		
	CPen *OldPen = (CPen*)pDc->SelectObject(&m_SigPen);
	CFont *OldFont = (CFont*)pDc->SelectObject(&m_TextFont);
	
	pDc->SetTextColor(RGB(150, 150, 150));
	int x = rect.left;
	int y = rect.top + Satellite_Signal_Space;
	
	for (int i=0; i<m_iStarNum; i++)
	{
		CRect rt;
		rt.SetRect(x, y, x+width, y+Satellite_Signal_Height);
		CString str;
		str.Format(_T("%d"), m_StarData[i].id);
		pDc->DrawText(str, &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		
		pDc->SelectObject(&m_SigPen);
		pDc->MoveTo(x+width, y+Satellite_Signal_Height/2);
		pDc->LineTo(x+width+(int)(m_StarData[i].signal*width/10), 
						y+Satellite_Signal_Height/2);

		if (m_StarData[i].tracked==0 && m_StarData[i].signal != 0)
		{

			pDc->SelectObject(&m_WhitePen);
			pDc->MoveTo(x+width+2, y+Satellite_Signal_Height/2);
			pDc->LineTo(x+width+(int)(m_StarData[i].signal*width/10)-2, 
							y+Satellite_Signal_Height/2);
		}		
		y += Satellite_Signal_Height + Satellite_Signal_Space;
	}
	pDc->SelectObject(OldPen);
	pDc->SelectObject(OldFont);
}