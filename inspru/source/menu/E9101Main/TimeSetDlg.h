#if !defined(AFX_TIMESETDLG_H__5258E269_E943_490D_A289_1FBBED0F00AB__INCLUDED_)
#define AFX_TIMESETDLG_H__5258E269_E943_490D_A289_1FBBED0F00AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeSetDlg.h : header file
//
#include "Afxtempl.h"
#include "DialogBase.h"
#include "DataFormatSetType.h"

/////////////////////////////////////////////////////////////////////////////
// CTimeSetDlg dialog

enum enTIMESET
{
	ID_TIMESET = 0,
	ID_TIMEFORMAT,
	ID_ZONESET
};


class CTimeSetDlg : public CDialogBase
{
public:
	void InitConfig();
	//变换语言和单位
	BOOL OnLanguageChange();
	//变皮肤
	BOOL OnSkinChange();
private:
	enTIMESET m_enTimeSet;

	enTimeUnit m_enTimeUnit;


	SYSTEMTIME m_systime;

	
	INT32 m_iZone;
	int m_TimeZone;
	CMap<int,int,UINT,UINT> m_ZonEexcursion;    //时区偏移

	BOOL m_bIsTimeAdjust;

// Construction
public:
	CTimeSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeSetDlg)
	enum { IDD = IDD_TIMESET_DIALOG };
	CExStatic	m_stcZoneBmp;
	CExStatic	m_stcZoneInfo;
	CExStatic	m_stcSec;
	CExStatic	m_stcMin;
	CExStatic	m_stcHour;
	CExButton	m_BtnZoneSet;
	CExButton	m_ZoneF;
	CExButton	m_BtnZoneB;
	CExButton	m_BtnTimeUnit24;
	CExButton	m_BtnTimeUnit12;
	CExButton	m_BtnOk;
	CExButton	m_BtnCancel;
	CExButton	m_BtnTimeSet;
	CExButton	m_BtnTimeFormat;
	CExButton	m_BtnMinuteSub;
	CExButton	m_BtnMinuteAdd;
	CExButton	m_BtnSecondAdd;
	CExButton	m_BtnSecondSub;
	CExButton	m_BtnHourSub;
	CExButton	m_BtnHourAdd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnTimesetOk();
	afx_msg void OnBtnTimesetBack();
	afx_msg void OnBtnTimeset();
	afx_msg void OnBtnTimeformat();
	afx_msg void OnBtnZoneset();
	afx_msg void OnBtnZoneB();
	afx_msg void OnBtnZoneF();
	afx_msg void OnBtnHourAdd();
	afx_msg void OnBtnHourSub();
	afx_msg void OnBtnMinuteAdd();
	afx_msg void OnBtnMinuteSub();
	afx_msg void OnBtnSecondAdd();
	afx_msg void OnBtnSecondSub();
//	afx_msg void OnBtnTimeunit();
	afx_msg void OnBtnTimeunit24();
	afx_msg void OnBtnTimeunit12();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMESETDLG_H__5258E269_E943_490D_A289_1FBBED0F00AB__INCLUDED_)
