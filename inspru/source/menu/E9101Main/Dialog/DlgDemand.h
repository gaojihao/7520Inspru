
#if !defined(AFX_Answer_H__2E0mnt56767HNFGDGJGFDNJHGHH30__INCLUDED_)
#define AFX_Answer_H__2E0mnt56767HNFGDGJGFDNJHGHH30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"
#include "../../GSM/diaodu_data.h"
//#include "../FileOperator/CRecordSMS.cpp"
#include "../../SQLite3/CppSQLite3.h"

//extern CRecordSMS<>*		g_pSMSCenter;

class CDlgDemand : public CDialogBase
{
public:
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();

	void OnExit();
	void ReleaseGdi();

	static BOOL IsOpen();

	static BOOL m_bIsOpen;
	static UINT	WM_DlgDemand_Refresh;

public:
	CDlgDemand(int nIndex, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgDemand)
	enum { IDD = IDD_DIALOG_DEMAND };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDemand)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	int			m_nInfoIndex;			//问题index
	int			m_nRecordCount;			//数据库中记录总数
	//CppSQLite3Query m_QuerySet;			//当前record
	//char		m_szSMS_ID[64];
	int			m_nSMS_ID;				//当前中心信息ID号
	int			m_nType;				//信息类型
	void		ShowPage(int nRecordIndex);
protected:
	CSMSCenter_data*  m_pData;
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

	CString m_csTitile;

	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_disable_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_disable_bmp;	
	//int		m_nCurrSMSIndex;	//当前中心信息在sms_center.sms文件中的index

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
	BOOL  LoadSMSContext(CString& strContext);

	// Generated message map functions
	//{{AFX_MSG(CDlgDemand)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Answer_H__2E0mnt56767HNFGDGJGFDNJHGHH30__INCLUDED_)
