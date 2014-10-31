#if !defined(AFX_EXSTATIC_H__DA75ECAC_47C2_47D8_B538_292201A103D8__INCLUDED_)
#define AFX_EXSTATIC_H__DA75ECAC_47C2_47D8_B538_292201A103D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExStatic window
#include "SkinPub.h"
class  IResMng;

class CExStatic : public CStatic
{
// Construction
public:
	CExStatic();

// Attributes
public:

	void   SkinSetStcID(BYTE byStyle, DWORD dwSkinID, DWORD dwPntResID = SKIN_ID_NONE );
	void   SkinSetTxtAttri(BYTE byFontID, BYTE byColID, BYTE byBkCol = sc_transparent);
	void   SkinSetDigitalTime(int nSec, BOOL bFmt12 = FALSE);
	void   SkinSetCH(DWORD  dwCh);
	void   SkinSetFm(DWORD  dwFm);
	void   SkinSetNum(DWORD  dwNum);
	void   SkinSetBatterNum(DWORD dwBattery);
	void   SKinSetMapOffset(DWORD dwOffset);
	void   SkinSetPos(int nPos);
	int    SkinGetPos();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExStatic();

	// Generated message map functions
protected:
	void    SkinDrawSingleLine(HDC hDC, LPRECT lpRect, DWORD dwStyle = DT_CENTER);
	void    SkinDrawSingleLineElide(HDC hDC, LPRECT lpRect);
	void    SkinDrawMultipleText(HDC hDC, LPRECT lpRect);
	void    SkinDrawDigitalNum(HDC hDC, LPRECT lpRect);
	void    SkinDrawBattery(HDC hDC);
	void    SkinDrawDigitalTime(HDC hDC, LPRECT lpRect);
	void    SkinDrawDigitalFm(HDC hDC, LPRECT lpRect);
	void    SkinDrawDigitalCH(HDC hDC, LPRECT lpRect);
	void    SkinDrawOnlyBmp(HDC hDC, LPRECT lpRect);
	void    SkinDrawZoneMap(HDC hDC, LPRECT lpRect);
	void    SkinDrawSlider(HDC hDC, LPRECT lpRect);
	void    SkinDrawBkFromParent(HDC hDC, DWORD  dwSkinPntID);
	//{{AFX_MSG(CExStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL             m_bFmt12;
	STC_DRAW_ITEM    m_DrawItem;
	static IResMng*  s_pResMng;
	BOOL             m_bMouseDown;
	int              m_nCurPos;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXSTATIC_H__DA75ECAC_47C2_47D8_B538_292201A103D8__INCLUDED_)
