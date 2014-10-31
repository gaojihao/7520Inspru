#if !defined(AFX_GSMCALLING_H__DCD330EA_9F6C_4667_8EFB_9556DB8344DE__INCLUDED_)
#define AFX_GSMCALLING_H__DCD330EA_9F6C_4667_8EFB_9556DB8344DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSMCalling.h : header file
//

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"

class CGSMCalling : public CDialogBase
{
	// Construction
public:
	CGSMCalling(CWnd* pParent = NULL);   // standard constructor
	
	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();
	void  SetCallingStr(CString strNum);
	//{{AFX_DATA(CGSMCalling)
	enum { IDD = IDD_DLG_DIAL_IN };
	//}}AFX_DATA

	BOOL   m_bExit;
	static CString  m_strName;
	static CString  m_strCityr;
	static BOOL     m_bIsAcc ;
	static BOOL     m_is_accepted;

	BOOL m_DialExtNum;
	BOOL m_EnablePress;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGSMCalling)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual void OnExit();
	//	virtual void EndDialog(int nResult)  ;

protected:
	
	CString        m_strCallingNum;
	CString        m_strUserName;
	CString        m_strExtNo;

	ItemInfo    m_Item[6];
	CString		m_stTitle;
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP     m_stBtnDisBMP;
	
	HBITMAP    m_stVolBmp;
	CDC        bk_vol_dc;
	CBitmap*   m_pold_bk_vol_bmp;

	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	CFont midfont;
	
	CDC  bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_dis_dc;
	
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_dis_bmp;
	
	void InitGdi();
	//	void OnButtonDiaOut();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	void OnOpenNumber();
protected:


	int         m_nListWidth[7];
	int         m_nCurIdx;
	ItemInfo    m_TipList;
	HBITMAP     m_stBmpList;
	CBitmap*    m_pold_sending_bmp;
	CDC         bk_list_dc;

	// Generated message map functions
	//{{AFX_MSG(CGSMCalling)
	afx_msg void OnButtonAccept();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonReject();
	afx_msg void OnBtnExit();
	afx_msg void OnPaint();
	afx_msg void OnBtnVolup();
	afx_msg void OnBtnVoldown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	//afx_msg LRESULT OnBlueToothCallNumber(WPARAM, LPARAM);
	afx_msg LRESULT OnBlueToothRingInd(WPARAM, LPARAM);
	afx_msg LRESULT OnBlueToothOffHook(WPARAM, LPARAM);
	afx_msg LRESULT OnBlueToothActive(WPARAM, LPARAM);
	afx_msg LRESULT OnDialNum(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSMCALLING_H__DCD330EA_9F6C_4667_8EFB_9556DB8344DE__INCLUDED_)
