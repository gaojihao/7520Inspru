#if !defined(AFX_ALMANACDIAGRAMCTRL_H__A7B369D7_0D2F_4BE6_B4C9_B9564ACF643F__INCLUDED_)
#define AFX_ALMANACDIAGRAMCTRL_H__A7B369D7_0D2F_4BE6_B4C9_B9564ACF643F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlmanacDiagramCtrl.h : header file
//
//#include "RealTimeData.h"
struct SStarData // star view.
{
	int		id;
	int		signal;
	int		tracked;
	int		horizon;
	int		direction;
	SStarData()
	{
		id			=	0;
		signal		=	0;
		tracked		=	0;
		horizon		=	0;
		direction	=	0;
	}
};
/////////////////////////////////////////////////////////////////////////////
// CAlmanacDiagramCtrl window

class CAlmanacDiagramCtrl : public CStatic
{
// Construction
public:
	CAlmanacDiagramCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlmanacDiagramCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlmanacDiagramCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAlmanacDiagramCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void SetStarData(SStarData *pData, int num);
	void SetLanguage(CString str);
	
private:
	void DrawSphereCircle(CDC *pDc, const CRect &rect);
	void DrawStarPos(CDC *pDc, const CRect &rect);
	POINT CalStarPos(SStarData star);
	
	SStarData m_StarData[20];
	int m_iStarNum;

	POINT m_centerCircle;
	int m_iRadius;

	CFont	m_TextFont;
	CFont	m_StarFont;
	CPen	m_LinePen;
	HICON	m_hSigStrong;
	HICON	m_hSigWeak;

	CString m_strDir;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALMANACDIAGRAMCTRL_H__A7B369D7_0D2F_4BE6_B4C9_B9564ACF643F__INCLUDED_)
