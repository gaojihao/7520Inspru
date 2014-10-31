#include <windows.h>
#include "LanguageSetType.h"
#include "../SysConfig/SysConfig.h"
#ifdef InputShl

#else

#define InputShl extern "C" __declspec(dllimport)

#endif

#define WM_IMM_MESSAGE   WM_USER + 0x1200
#define WM_IMM_DIAL_NUMBER_MESSAGE   WM_USER + 0x1201
enum ScreenSize {FourPoint3, ThreePoint5, SevenPoint0};										 //��Ļ�ߴ�
enum DisplayType {ShortMessage, TelePhone, Error};
enum ImmReturn{Cancel, Ok, Save};

typedef struct  
{
	int         nSize;
	DisplayType dt;
	union{
		struct 
		{
			CString strName;
			CString strType;
		};
		LPTSTR strMessage;
	};

}IMMRESULT;

InputShl void InitInputShl(ScreenSize nSize = FourPoint3);                //��ʼ���豸

InputShl ImmReturn ImmCreateMessage(enLanguage enlang);					  //�½�����
InputShl ImmReturn ImmEditMessage(enLanguage enlang, LPTSTR strMessage);  //�༭����

InputShl ImmReturn ImmAddPhone(enLanguage enlang);                        //�½��û�
InputShl ImmReturn ImmEditPhone(enLanguage enlang, LPTSTR);	              //�༭�û�

InputShl ImmReturn ImmAddPhoneNum(HWND hParentHwnd,HWND hWnd);             //�½��绰����
InputShl ImmReturn ImmEditPhoneNum(HWND hParentHwnd,HWND hWnd);	           //�༭�绰����
                              
InputShl IMMRESULT GetStrText();								          //ȡ�������ַ���


typedef void  (*INIT)(ScreenSize nSize /*= FourPoint3*/);					   //��ʼ���豸
typedef void  (*IMMSETKEYSOUND)(enKeySound enKey);					   //��ʼ���豸

typedef ImmReturn (*IMMCREATEMESSAGE)(enLanguage enlang);				   //�½�����
typedef ImmReturn (*IMMEDITMESSAGE)(enLanguage enlang, LPTSTR strMessage); //�༭����

typedef ImmReturn(*IMMEDITPHONE)(enLanguage enlang, LPTSTR strName, enKeySound enKey,enSkinType enSkin);       //�½��û�
typedef ImmReturn(*IMMADDPHONE)(enLanguage enLanguage);                    //�༭�û�

typedef ImmReturn(*IMMADDPHONENUM)(HWND hParentHwnd,HWND hWnd);             //�½��绰����
typedef ImmReturn(*IMMEDITPHONENUM)(HWND hParentHwnd,HWND hWnd);            //�༭�绰����

typedef IMMRESULT (*GETRESULT)();                                          //ȡ�������ַ���

