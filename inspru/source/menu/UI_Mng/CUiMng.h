// CUiMng.h: interface for the CUiMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUIMNG_H__62BF02BC_8525_4A37_9776_A8F9D6393682__INCLUDED_)
#define AFX_CUIMNG_H__62BF02BC_8525_4A37_9776_A8F9D6393682__INCLUDED_

#include "Afxtempl.h"
#include "CResBitmap.h"
#include "CResString.h"
#include "CSkinBitmap.h"
#include "CUnitAndFormatSwitch.h"
#include "DialogBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_CLOSE_ModalDLG WM_USER + 319

class CUiMng  
{
public:
	virtual ~CUiMng();

	static CUiMng& Instance();

	UINT GetWndCount();

	void SetLanguagePath( const CString& LanguagePath);
	void SetSkinPath( const CString& SkinPath);

	//�����Թ���
	BOOL SetLanguage( enLanguage );					//��������
	enLanguage GetCurLanguage();					//ȡ��ǰ����

	//Ƥ������
	BOOL SetSkinType( enSkinType );					//����Ƥ��
	enSkinType GetCurSkin();						//ȡ��ǰƤ��

//	//ʱ��
//	BOOL SetCueTimeZone(enTimeZone);
//	enTimeZone GetCurTimeZone();

	//��λ����ʽ����
	BOOL SetTimeUnit( enTimeUnit );				//����ʱ�䵥λ
	BOOL SetDistanceUnit( enDistanceUnit );		//������̵�λ
	BOOL SetSpeedUnit( enSpeedUnit );			//�����ٶȵ�λ
	BOOL SetHoueUnit( enHourUnit );				//����ʱ�̸�ʽ
	BOOL SetLongitudeALatitudeUnit( enLongitudeALatitudeUnit );	//���þ�γ�ȸ�ʽ
	BOOL SetAngleUnit( enAngleUnit );				//���ýǶȻ��ǻ��ȵ�λ

	enDistanceUnit GetCurDistanceUnit( );			//ȡ��ǰ��̵�λ
	enSpeedUnit GetCurSpeedUnit( );					//ȡ��ǰ�ٶȵ�λ
	enTimeUnit GetCurTimeUnit( );					//ȡ��ǰʱ�̸�ʽ
	enHourUnit GetCurHoueUnit();					//ȡ��ǰʱ�䵥λ
	enLongitudeALatitudeUnit GetCurLongitudeALatitudeUnit(); //ȡ��ǰ��γ�ȸ�ʽ
	enAngleUnit GetCurAngleUnit();					//ȡ�ǶȻ��ǻ��ȵ�λ

	BOOL Attach(CDialogBase* pDialogBase);
	BOOL Detach(CDialogBase* pDialogBase);
	//Detach���д���,�ص����˵�,added by �¹���
	BOOL DetachAll();

private:
	CUiMng();
	static CUiMng _instance;

	//�Ѵ�������ָ��
	CArray<CDialogBase*,CDialogBase*&> m_WndArray;
//	CMap<enWndId,enWndId,CDialogBase*,CDialogBase*&> m_WndMap;

	
	//��ǰ����ָ��
	CDialogBase* m_curWnd;

	void ReleaseLanguage();
	void ReleaseSkin();

};

#endif // !defined(AFX_CUIMNG_H__62BF02BC_8525_4A37_9776_A8F9D6393682__INCLUDED_)
