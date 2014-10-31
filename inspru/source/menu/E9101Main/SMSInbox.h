#if !defined(AFX_SMSINBOX_H__73A206AD_CAAF_4CDC_9796_E1302F97ACD9__INCLUDED_)
#define AFX_SMSINBOX_H__73A206AD_CAAF_4CDC_9796_E1302F97ACD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSInbox.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"



class CSMSInbox : public CDialogBase
{
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();
	
	// Construction
public:
	CSMSInbox(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_SMS_INBOX };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaMain)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	ItemInfo    m_Item[4];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP     m_stSmsFlagBmp;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CString m_csTitile;
	
	CDC  bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_sms_flag;
	
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_sms_bmp;
	
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	

	//list show
protected:
	enum  { elist_count = 6};
	UINT		m_nRecordCount;		//数据库中记录总数
	UINT		m_nTotalPage;		//总页数
	int			m_nCurPage;
	ItemInfo	m_list[elist_count];
	int			m_ItemState[elist_count]; //每个项的状态，已读和未读
	int			m_nUID[elist_count];	//每一行的UID
	void		UserClickItem(int nItem );
	void		ShowPage(int nPage);
protected:
	
	void  OnBtnPageUp();
	void  OnBtnPageDown();
	void  OnBtnDelAll();
	int   SMSMaxNum();
	
	// Generated message map functions
	//{{AFX_MSG(CMediaMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSINBOX_H__73A206AD_CAAF_4CDC_9796_E1302F97ACD9__INCLUDED_)
