// DialogBase.h: interface for the CDialogBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGBASE_H__09D3EE43_E57B_438E_8761_556AFE8D521E__INCLUDED_)
#define AFX_DIALOGBASE_H__09D3EE43_E57B_438E_8761_556AFE8D521E__INCLUDED_

#include "CResString.h"
#include "CResBitMap.h"
#include "../MutiLanguage/CLanguageResDef.h"
#include "../SkinMng/CSkinResDef.h"
#include "IResMng.h"
#include "SkinPub.h"
#include "ExButton.h"
#include "../SkinMng/ExStatic.h"
#include "../SkinMng/SkinPub.h"
#include "../SkinMng/IResMng.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		DLG_WIDTH	800//480
#define		DLG_HIGHT	480//272

//#define		GET_RECT_PRE	(CRect(0,0,48,34))
//#define		GET_RECT_CUR	(CRect(48-36,34,48,34+28))
//#define		GET_RECT_TITLE	(CRect(48,0,DLG_WIDTH,15))
//#define		GET_RECT_STATUS	(CRect(48,DLG_HIGHT-15,DLG_WIDTH,DLG_HIGHT))
//#define		GET_RECT_BK		(CRect(48,15,DLG_WIDTH,DLG_HIGHT-15))
//#define		GET_RECT_BK_WITH_STATE		(CRect(48,15,DLG_WIDTH,DLG_HIGHT))
//#define		GET_RECT_SCB	(CRect(DLG_WIDTH-12,15,DLG_WIDTH-2,DLG_HIGHT-15))
//#define		GET_RECT_LIST	(CRect(51,18,DLG_WIDTH-9,DLG_HIGHT-2))


class CDialogBase : public CDialog
{
// Construction
public:
	CDialogBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);	// standard constructor
	CDialogBase();
public:
	BOOL Create( UINT nIDTemplate,CWnd* pParentWnd = NULL ); 
	virtual int DoModal( ); 
	void EndDialog(int nResult );
	virtual BOOL DestroyWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//�任���Ժ͵�λ
	virtual BOOL OnLanguageChange();
	//��Ƥ��
	virtual BOOL OnSkinChange();

	virtual void OnExit();

//	//֪ˢ�½���
// 	BOOL ReflushData();
	//����ID
	virtual void SetTemplateID( UINT nID );
	//ȡID
	virtual UINT GetTemplateID();
	//��������ʾ�Ի��������ģ̬�Ի��򣬷���DoModal��ֵ��
	virtual	UINT Open();
	//�رպ����ٶԻ���
	virtual BOOL Close();
	//���ô���ģ̬���ģ̬TRUEģ̬FALSE��ģ̬
	virtual void SetModal( BOOL bModal = FALSE);
	//�жϿ�ģ̬���ģ̬TRUEģ̬FALSE��ģ̬
 	virtual BOOL GetModal();

	CDialogBase&   SkinSetResID(DWORD dwSkinID);
	CDialogBase&   SkinSetDlgStyle(DWORD dwStyle);
	CDialogBase&   SkinSetTitlePos(CPoint pt);
	

private:
	//���������ؼ���λ��
	virtual void PositionControl(){};

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBase)
//	public:
//	virtual int DoModal();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	//virtual void PostNcDestroy();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//}}AFX_VIRTUAL

// Implementation

protected:
	HICON m_hIcon;

	//�Ի���ģʽ
	BOOL m_bModal;
	UINT	m_nIDTemplate;
	// Generated message map functions
	//{{AFX_MSG(CDialogBase)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	DLG_DRAW_ITEM     m_DlgDrawItem;
	static IResMng*   g_pResMng;
};

#endif // !defined(AFX_DIALOGBASE_H__09D3EE43_E57B_438E_8761_556AFE8D521E__INCLUDED_)
