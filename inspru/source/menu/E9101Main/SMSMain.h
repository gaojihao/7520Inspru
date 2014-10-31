#if !defined(AFX_MEDIASMS_H__EC00922D_D0AE_487F_BC59_8ACDED994532__INCLUDED_)
#define AFX_MEDIASMS_H__EC00922D_D0AE_487F_BC59_8ACDED994532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MediaSMS.h : header file
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMediaMain dialog

class CSMSMain : public CDialogBase
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
	CSMSMain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_SMS_MAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	ItemInfo    m_Item[5];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CString m_csTitile;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
protected:
	
	void  OnBtnNewSMS();
	void  OnBtnInBox();
	void  OnBtnOutBox();
	void  OnBtnDraft();

	BOOL  LoadSMSContext(CString& strContext);
	BOOL  LoadUserPhone(CString& strPhone);
	
	// Generated message map functions
	//{{AFX_MSG(CMediaMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_MEDIASMS_H__EC00922D_D0AE_487F_BC59_8ACDED994532__INCLUDED_)
