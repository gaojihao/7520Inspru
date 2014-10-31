#if !defined(AFX_INFOMENU_H__CFO567657HFGDGBGFCDXS9__INCLUDED_)
#define AFX_INFOMENU_H__CFO567657HFGDGBGFCDXS9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSDraft.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"
//#include "../FileOperator/CRecordSMS.cpp"

//extern CRecordSMS<>*		g_pSMSCenter;

class CDlgEventList : public CDialogBase
{
	enum { BTN_COUNT = 4 };
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	void OnExit();
	void ReleaseGdi();

	static BOOL IsOpen();

	static BOOL m_bSMSCenter;			//是否有新的中心信息
	static BOOL m_bIsOpen;
	static UINT	WM_DlgEventInfo_Refresh;

public:
	CDlgEventList(BOOL bIsModelDlg = TRUE, CWnd* pParent = NULL);

	// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_DIALOG_EVENT };
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
	ItemInfo    m_Item[BTN_COUNT];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;

	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;

	CString m_csTitile;

	CDC  bk_normal_dc;
	CDC  bk_press_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;

	BOOL	m_bIsModelDlg;	//当前对话框是否为模态对话框   TRUE:模态   FALSE:非模态

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

protected:
	enum  { elist_count = 6};

	int			m_nPageIndex;				//当前页编号：0,1,2...m_nPageCount-1
	int			m_nPageCount;				//页总数
	int			m_nRecordCount;				//数据库中记录总数
	void		ShowPage(int nPageIndex);	//显示当前页的数据

	int			m_nEvent_ID[elist_count];	//事件ID
	ItemInfo	m_list[elist_count];
	int			m_ItemState[elist_count];	//每个项的状态，已回复和未回复
	void		UserClickItem(int nItem );
	void		DrawList(CDC* pmemDC);
	void		ChangeReadStatus();
protected:

	void  OnBtnPageUp();
	void  OnBtnPageDown();
	void  OnBtnDelAll();

	// Generated message map functions
	//{{AFX_MSG(CMediaMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSCENTERLIST_H__CFBFFWE2342342CB3789__INCLUDED_)
