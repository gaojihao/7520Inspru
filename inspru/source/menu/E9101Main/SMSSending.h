#if !defined(AFX_SMSSENDING_H__A15E34F2_EB9D_436A_AE55_C9CA0BCBE7E4__INCLUDED_)
#define AFX_SMSSENDING_H__A15E34F2_EB9D_436A_AE55_C9CA0BCBE7E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSSending.h : header file
//

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSMSSending dialog

class CSMSSending : public CDialogBase
{
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();

	void  SetMsgParam(CString& pStrPhone, CString& strMsg, BOOL bSaved = FALSE);
	void  SetDestoryHwnd( CDialogBase* hPrevHwnd ) { m_hPrevWnd = hPrevHwnd; }
	
	void  SendMsgResult(BOOL bSuc);
	void  SetCallPhone( CString& pStrPhone );
	// Construction
public:
	CSMSSending(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CSMSSending)
	enum { IDD = IDD_SMS_SENDING };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSSending)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	CString     m_strPhone;
	CString     m_strUserName;  //name
	CString     m_strSMSMsg;

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

	int    m_nListWidth[6];
	int    m_nCurIdx;
	UINT   m_nTotalIdx;
	
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

protected:

	CDialogBase*   m_hPrevWnd; //前一个窗口句柄，默认位空，如果不为空，则删除该窗口
	bool  b_HasResult;
	BOOL  m_bSaveMsg;
	// Generated message map functions
	//{{AFX_MSG(CSMSSending)
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

#endif // !defined(AFX_SMSSENDING_H__A15E34F2_EB9D_436A_AE55_C9CA0BCBE7E4__INCLUDED_)
