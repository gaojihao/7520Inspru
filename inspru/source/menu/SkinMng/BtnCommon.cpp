// BtnCommon.cpp : implementation file
//

#include "stdafx.h"
#include "BtnCommon.h"
#include "CUiMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBtnCommon

CBtnCommon::CBtnCommon()
{
}

CBtnCommon::~CBtnCommon()
{
}


BEGIN_MESSAGE_MAP(CBtnCommon, CExButton)
	//{{AFX_MSG_MAP(CBtnCommon)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtnCommon message handlers

BOOL CBtnCommon::OnSkinChange()
{
	//CConfig* pConfig = AfxGetMainGroup()->GetConfig();
	//ASSERT(pConfig);
//////////////////////////////////////////////////////////////////////////
	//����Ƥ�������������ɫ(to be add because of day/night mode not finish)
	{
	
		//BYTE NorCol,SelCol,  DisCol; 
		//GetBtnCol(NorCol,SelCol,  DisCol);
		//SetBtnCol(NorCol,SelCol,  DisCol);
	}

	return TRUE;
}

BOOL CBtnCommon::OnLanguageChange()
{
	//return TRUE;
	//ASSERT(pConfig);
//////////////////////////////////////////////////////////////////////////
	//����������������Ĵ�С(��Ҫ�����ĵİ�ť�����ĵİ�ť��С�᲻ͬ)
	//enLanguage eLanguage = pConfig->GetLanguage();
	enLanguage eLanguage = ID_CHINESE;
	{
		if (ID_CHINESE == eLanguage)
		{
			//��������������
			SetBtnFontID(SF_BUTTON_NORMAL_CHINESE);
		}
		else
		{
			//������������������
			SetBtnFontID(SF_BUTTON_NORMAL);
		}

	}
 	return TRUE;
}

void CBtnCommon::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//���뷽ʽ�ǲ��ñ��,���������໯��ʱ����һ�μ���
	DWORD dwStyle;
	GetBtnStyle(dwStyle);
	dwStyle |= KDS_TEXT_VCENTER |KDS_TEXT_HCENTER;
	dwStyle &= ~KDS_TEXT_VBOTTOM; 
	SetBtnStyle(dwStyle);

	CExButton::PreSubclassWindow();
}

void CBtnCommon::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CExButton::CalcWindowRect(lpClientRect, nAdjustType);
}
