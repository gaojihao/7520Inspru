#if !defined(AFX_SMSDRAFT_H__CFB72DAA_91FE_4B03_9339_F9B43CCB3789__INCLUDED_)
#define AFX_SMSDRAFT_H__CFB72DAA_91FE_4B03_9339_F9B43CCB3789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSDraft.h : header file
//
#include "DialogBase.h"
#include "BtnOfMainMenu.h"
#include "MaskCtrl.h"


class CSMSDraft : public CDialogBase
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
	CSMSDraft(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CMediaMain)
	enum { IDD = IDD_SMS_DRAFT };
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
	
	ItemInfo    m_Item[4];
	HBITMAP     m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	
	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;
	
	CString m_csTitile;
	
	CDC  bk_normal_dc;
	CDC  bk_press_dc;
	
	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);
	
	
	//list show
protected:
	enum  { elist_count = 6};
	UINT		m_nRecordCount;		//���ݿ��м�¼����
	UINT		m_nTotalPage;		//��ҳ��
	int			m_nCurPage;
	ItemInfo	m_list[elist_count];
	int			m_ItemState[elist_count]; //ÿ�����״̬���Ѷ���δ��
	int			m_nUID[elist_count];	//ÿһ�е�UID
	void		UserClickItem(int nItem );
	void		ShowPage(int nPage);
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
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSDRAFT_H__CFB72DAA_91FE_4B03_9339_F9B43CCB3789__INCLUDED_)
