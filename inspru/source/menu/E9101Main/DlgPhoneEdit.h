#if !defined(AFX_DLGPAIR_H__925EB066_0CF3_48DF_81111180_6E4346036C00__INCLUDED_)
#define AFX_DLGPAIR_H__925EB066_0CF3_48DF_81111180_6E4346036C00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPair.h : header file
//
#include "PhoneBookFile.h"

#include "PhoneBookMng.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPair dialog

class CDlgPhoneEdit : public CDialogBase
{
// Construction
public:
	void SetEditType(BOOL bSim, int nIdx) { m_bSIM = bSim; m_nIdx = nIdx; }
	BOOL OnLanguageChange();
	BOOL OnSkinChange();
	void OnEditPhoneNum();
	void OnEditPhoneName();
	void OnPhoneCallout();
	virtual void OnExit();

	static CString m_PhoneName;
	static CString m_PhoneNum;
	CString tempPhoneNum;
	static CString m_PhoneCity;
	static UINT m_PhonePos;
	static BOOL m_AddPhone;
	
	BOOL m_NewPhone;

	CString m_Info;
protected:

	ItemInfo    m_Item[7];
	CString		m_stTitle;
	CString     m_stInfo;
	CString     m_stName;
	CString     m_stNum;
	CString     m_stEditCall;
	CString     m_stEditDel;

	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;

	CObArray* m_ObArrary;
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;


	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);


public:
	CDlgPhoneEdit(CWnd* pParent = NULL);   // standard constructor

	void   SetPrevHwnd(CDialogBase* pDlg) { m_PrevDlg = pDlg; }

	void   DelSIM(int nIdx);
	void   DelDevice(int nIdx);
	void   ModifySIM(int nIdx);
	void   ModifyDevice(int nIdx);
	void   SaveToSIM();
	void   SaveToDevice();

// Dialog Data
	//{{AFX_DATA(CDlgPair)
	enum { IDD = IDD_DIALOG_PHONEBOOK_EDIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPair)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDialogBase*  m_PrevDlg;
	BOOL  m_bSIM;
	int   m_nIdx;
	// Generated message map functions
	//{{AFX_MSG(CDlgPair)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnIMMMsg(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAIR_H__925EB066_0CF3_48DF_8180_6E4346036C00__INCLUDED_)
