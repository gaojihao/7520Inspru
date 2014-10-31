#if !defined(DLG_NAVI_BRO)
#define DLG_NAVI_BRO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNaviBro.h : header file
//

#include "CDirsRef.h"



typedef enum enListType
{
	LIST_DIR,
	LIST_FILE,
	LIST_NONE,

}enListType;

class ItemListInfo
{
public:
	RECT rect;
	CString chChar;
	enBtnState nState;
	enListType nListType;
};

class CDlgNaviBro : public CDialogBase
{
	enum { BTN_COUNT = 5 };
public:
	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	CDirsRef m_dirref;

	void LoadDir(CString str);

	void OnUpDir();

	void OnBtnList(int j);
	
	void OnUp();
	void OnDown();

	void OnOK();			//确认
	void ChangeDir();		//切换路径（SD卡<-->UDISK）

	void SetType( bool bType ){ m_bType = bType; }
	void SetCurPathType( int );

	virtual void OnExit();

protected:
	int m_DirCnt;
	int m_FileCnt;
	int m_iListCnt;
	int m_PageCnt;
	int m_iCurPage;

	bool m_bType;	//路径的类型 true: Navi		false: App

	ItemListInfo*    m_pItemList;

	ItemListInfo	m_ItemList[8];
	ItemInfo        m_Item[BTN_COUNT];
	HBITMAP    m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnDisableBMP;

	HBITMAP     m_stBtnListBMP;

	CDC*  m_pDC;
	CDC   memDC;
	CBitmap  bitmap;
	CFont  smallfont;
	CFont bigfont;

	CString m_csTitile;
	CString m_csPath;
	CString m_MaskDir;
	
	CDC bk_normal_dc;
	CDC  bk_press_dc;
	CDC  bk_disable_dc;

	CDC bk_list_dc;

	CBitmap* m_pold_bk_normal_bmp;
	CBitmap*  m_pold_bk_press_bmp;
	CBitmap*  m_pold_bk_disable_bmp;

	CBitmap*  m_bk_list_bmp;

	CString		m_strRootDir;	//根目录

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);

public:
	CDlgNaviBro(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNaviBro)
	enum { IDD = IDD_DLG_NAVI_BRO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNaviBro)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNaviBro)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNAVIBRO_H__922C946E_C6DE_4439_94DE_B109A97BEB9D__INCLUDED_)
