#if !defined(AFX_GSMDIALING_H__005DF95D_2801_4F7C_A7C0_E9D3B6E003A8__INCLUDED_)
#define AFX_GSMDIALING_H__005DF95D_2801_4F7C_A7C0_E9D3B6E003A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSMDialing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGSMDialing dialog

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"

class CGSMDialing : public CDialogBase
{
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();

	void  SetCallPhone( CString& pStrPhone );
	void  SendDialResult(BOOL bSuc);

	BOOL m_bTHDDialingList;
	static DWORD THDDialingList( LPVOID lpParameter );
	HANDLE	m_hTHDDialingList;

	 
	// Construction
public:
	CGSMDialing(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CGSMDialing)
	enum { IDD = IDD_GSM_DIALING };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGSMDialing)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	CString     m_strPhone;
	CString     m_strUserName;

	ItemInfo    m_ExitItem;
	ItemInfo    m_TipItem;
	ItemInfo    m_TipList;
	
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBmpList;
	//HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC bk_list_dc;
	
	//CDC  bk_press_dc;
	
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap* m_pold_sending_bmp;
	//CBitmap*  m_pold_bk_press_bmp;
	
	int    m_nListWidth[5];
	int    m_nCurIdx;
	UINT   m_nTotalIdx;
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
protected:
	bool  b_HasResult;
	BOOL  b_DialResult;

	int   m_waiting_count;

	// Generated message map functions
	//{{AFX_MSG(CGSMDialing)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSMDIALING_H__005DF95D_2801_4F7C_A7C0_E9D3B6E003A8__INCLUDED_)
