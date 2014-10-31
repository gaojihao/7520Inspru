#if !defined(AFX_PBOOKMAIN_H__735FF16B_C7CF_4C22_AA6D_A54DF0C75144__INCLUDED_)
#define AFX_PBOOKMAIN_H__735FF16B_C7CF_4C22_AA6D_A54DF0C75144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PBookMain.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"

class CPBookMain : public CDialogBase
{
public:
	//�任���Ժ͵�λ
	BOOL OnLanguageChange();
	//��Ƥ��
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();
	
	// Construction
public:
	CPBookMain(CWnd* pParent = NULL);   // standard constructor
	
	BOOL   m_IsGetItem;
	CString m_strSel;

	// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_PBOOK_MAIN };
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

	ItemInfo    m_Item[5];
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
	int    m_nCurPage;
	BOOL      m_bOperation;
	CString   m_strReadTip;

	CString    m_strPhone[elist_count];
	CString    m_strName[elist_count];
	ItemInfo   m_list[elist_count];
	int        m_ItemState[elist_count]; //ÿ�����״̬,1����PBOOK,3:SIM���ϵ�PBOOK
	void    UserClickItem(int nItem );

	int			m_nPageIndex;				//��ǰҳ��ţ�0,1,2...m_nPageCount-1
	int			m_nPageCount;				//ҳ����
	int			m_nRecordCount;				//���ݿ��м�¼����
	void		ShowPage(int nPageIndex);	//��ʾ��ǰҳ������

protected:
	
	void  OnBtnPageUp();
	void  OnBtnPageDown();
	void  OnBtnDelAll();
	void  OnBtnAdd();
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

#endif // !defined(AFX_PBOOKMAIN_H__735FF16B_C7CF_4C22_AA6D_A54DF0C75144__INCLUDED_)
