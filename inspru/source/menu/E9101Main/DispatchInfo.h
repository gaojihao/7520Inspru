#if !defined(AFX_CDispatchInfo_H__2E0763B5_ED49_4CC3_A5A6_AD607DDFAE30__INCLUDED_)
#define AFX_CDispatchInfo_H__2E0763B5_ED49_4CC3_A5A6_AD607DDFAE30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSInfo.h : header file
//

#define TID_CLOSE_WINDOW		0x1001
#define WND_NAME_DISPATCHINFO	_T("WND_NAME_DISPATCHINFO")
#define MSG_NEW_DISPATCHINFO	(WM_USER + 0x1002)

class CDispatchInfo : public CDialogBase
{
public:
	static BOOL	m_bISspeak;
	static BOOL	m_bIsOpen;
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();

	//用户选择的当前IDX,但是显示的时候是到序
	void  SetLogicData(logic_data* pData, int nIdx, BOOL bSaved = FALSE );
	void  SetReplyBtn(BOOL bShow);
	void  UpdateRecord();
	
	// Construction
public:
	CDispatchInfo(st_sms *sms, CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_DIALOG_DISPATCH };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaMain)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	logic_data*  m_pData;
	int          m_nSelIdx;
	ItemInfo    m_Item[6];
	ItemInfo    m_Static[3];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP     m_stBtnDisableBMP;	
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	bool  m_bShowReply;
	
	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_disable_dc;
	
	CBitmap*  m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_disable_bmp;
	st_sms *m_psms;
	HANDLE m_hdtts;
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
protected:
	
	BOOL  m_bSavedMsg;
	void  OnBtnPageUp();
	void  OnBtnPageDown();
	void  OnBtnDel();
	void  SendMsg();
	void  Reply();
	int   SMSMaxNum();
	BOOL  LoadSMSContext(CString& strContext);
	
	// Generated message map functions
	//{{AFX_MSG(CMediaMain)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSINFO_H__2E0763B5_ED49_4CC3_A5A6_AD607DDFAE30__INCLUDED_)
