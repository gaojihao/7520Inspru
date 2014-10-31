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

	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	virtual void OnExit();

//	//知刷新界面
// 	BOOL ReflushData();
	//设置ID
	virtual void SetTemplateID( UINT nID );
	//取ID
	virtual UINT GetTemplateID();
	//创建和显示对话框。如果是模态对话框，返回DoModal的值。
	virtual	UINT Open();
	//关闭和销毁对话框
	virtual BOOL Close();
	//设置窗口模态或非模态TRUE模态FALSE非模态
	virtual void SetModal( BOOL bModal = FALSE);
	//判断口模态或非模态TRUE模态FALSE非模态
 	virtual BOOL GetModal();

	CDialogBase&   SkinSetResID(DWORD dwSkinID);
	CDialogBase&   SkinSetDlgStyle(DWORD dwStyle);
	CDialogBase&   SkinSetTitlePos(CPoint pt);
	

private:
	//调整各个控件的位置
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

	//对话框模式
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
