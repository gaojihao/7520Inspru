#if !defined(AFX_ANSWERLIST_H__CFBFFGBWRTHgefeWEWSAEGFASDFGRSTEARGB3789__INCLUDED_)
#define AFX_ANSWERLIST_H__CFBFFGBWRTHgefeWEWSAEGFASDFGRSTEARGB3789__INCLUDED_

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

class CDlgAnswerList : public CDialogBase
{
	enum { BTN_COUNT = 4 };
public:
	//�任���Ժ͵�λ
	BOOL OnLanguageChange();
	//��Ƥ��
	BOOL OnSkinChange();
	
	void OnExit();
	void ReleaseGdi();

	static BOOL IsOpen();

	static BOOL m_bSMSCenter;			//�Ƿ����µ�������Ϣ
	static BOOL m_bIsOpen;
	static UINT	WM_DlgAnswerList_Refresh;
	
public:
	CDlgAnswerList(int nIndexCurr, CWnd* pParent = NULL);
	
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
	
	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

protected:
	enum  { elist_count = 6};

	int			m_nPageIndex;				//��ǰҳ��ţ�0,1,2...m_nPageCount-1
	int			m_nPageCount;				//ҳ����
	int			m_nQuestionIndex;			//�����ţ�0,1,2...Count-1
	int			m_nRecordCount;				//���ݿ��м�¼����
	void		ShowPage(int nPageIndex);	//��ʾ��ǰҳ������

	ItemInfo	m_list[elist_count];
	int			m_nAnswerID[elist_count];	//��ID
	int			m_ItemState[elist_count];	//ÿ�����״̬���ѻظ���δ�ظ�
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
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSCENTERLIST_H__CFBFFWE2342342CB3789__INCLUDED_)
