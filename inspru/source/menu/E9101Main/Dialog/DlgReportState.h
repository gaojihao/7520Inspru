#if !defined(AFX_CDlgReportState_9DDECABA_28AE_40E9_82E3_9F05A2043CEB__INCLUDED_)
#define AFX_CDlgReportState_9DDECABA_28AE_40E9_82E3_9F05A2043CEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSNew.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSMSNew dialog

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMediaMain dialog


class CDlgReportState : public CDialogBase
{
	enum{ enCtrCount = 3 };
	enum EN_TID{
		TID_CLOSE			= 1000,
		TID_CLOSE_TIMEOUT,
		TID_SET_TOP,
		TID_TIME_OUT,
	};
public:

	static UINT	WM_DlgReportState_Refresh;

	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();

	void ReportState(WPARAM wParam);

	static BOOL	IsOpen();
	
public:
	CDlgReportState(CWnd* pParent = NULL);   // standard constructor
	// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_DLG_DRIVE_RECORD };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	//CHAR		test_buff[1024*500];
	ItemInfo    m_Item[enCtrCount];

	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CString		m_csTitile;
	CString		m_csContent;	//显示内容
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	static BOOL	m_bIsOpen;	//窗口是否已经打开
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CMediaMain)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // !defined(AFX_SMSNEW_H__9DDECABA_28AE_40E9_82E3_9F05A2043CEB__INCLUDED_)
