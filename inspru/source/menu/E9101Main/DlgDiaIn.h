#if !defined(AFX_DLGDIAIN_H__78068DD4_70E8_4D51_8AB8_D747FDE84D30__INCLUDED_)
#define AFX_DLGDIAIN_H__78068DD4_70E8_4D51_8AB8_D747FDE84D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDiaIn.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgDiaIn dialog
enum BlueToothDiaInState {Start, Accept_Call, Reject_Call, End_Call};

class CDlgDiaIn : public CDialogBase
{
// Construction
public:
	CDlgDiaIn(CWnd* pParent = NULL);   // standard constructor

	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

// Dialog Data
	//{{AFX_DATA(CDlgDiaIn)
	enum { IDD = IDD_DLG_DIAL_IN };
//	CBtnOfMainMenu	m_btnExit;
//	CBtnOfMainMenu	m_btnReject;
//	CBtnOfMainMenu	m_btnAccept;
	BlueToothDiaInState m_DialInState;

//	CBtnOfMainMenu	m_btnVolUp;
//	CBtnOfMainMenu	m_btnVolDown;

//	CExStatic m_stcNum;

	//}}AFX_DATA
	BOOL m_bExit;

	static CString  m_strNumber;
	static CString  m_strName;
	static CString  m_strCityr;

	static BOOL  m_bIsAcc ;
	BOOL m_DialExtNum;
	BOOL m_EnablePress;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDiaIn)
	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual void OnExit();
//	virtual void EndDialog(int nResult)  ;

// Implementation
	protected:

	ItemInfo       m_Item[6];

	ItemInfo m_pItemCity;

	CString m_Info;

	CString		m_stTitle;
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP     m_stBtnDisBMP;
	
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

	// Generated message map functions
	//{{AFX_MSG(CDlgDiaIn)
	afx_msg void OnButtonAccept();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonReject();
	afx_msg void OnBtnExit();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnVolup();
	afx_msg void OnBtnVoldown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
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

#endif // !defined(AFX_DLGDIAIN_H__78068DD4_70E8_4D51_8AB8_D747FDE84D30__INCLUDED_)
