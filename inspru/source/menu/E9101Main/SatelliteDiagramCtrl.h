#if !defined(AFX_SATELLITEDIAGRAMCTRL_H__3D723942_6B68_4BE6_A7FF_CB7CEA7D516C__INCLUDED_)
#define AFX_SATELLITEDIAGRAMCTRL_H__3D723942_6B68_4BE6_A7FF_CB7CEA7D516C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SatelliteDiagramCtrl.h : header file
//
#include "../GSM/AlmanacDiagramCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSatelliteDiagramCtrl window
// struct SStarData // star view.
// {
// 	int		id;
// 	int		signal;
// 	int		tracked;
// 	int		horizon;
// 	int		direction;
// 	SStarData()
// 	{
// 		id			=	0;
// 		signal		=	0;
// 		tracked		=	0;
// 		horizon		=	0;
// 		direction	=	0;
// 	}
//};

class CSatelliteDiagramCtrl : public CStatic
{
// Construction
public:
	CSatelliteDiagramCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSatelliteDiagramCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSatelliteDiagramCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSatelliteDiagramCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void SetStarData(SStarData *pData, int num);

private:
	void DrawSigGrid(CDC *pDc, const CRect &rect);
	void DrawSignal(CDC *pDc, const CRect &rect);
	
	SStarData m_StarData[12];
	int	m_iStarNum;

	CFont m_TextFont;
	CPen m_LinePen;
	CPen m_SigPen;
	CPen m_WhitePen;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SATELLITEDIAGRAMCTRL_H__3D723942_6B68_4BE6_A7FF_CB7CEA7D516C__INCLUDED_)
