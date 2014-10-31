#include <windows.h>
#include "LanguageSetType.h"
#include "../SysConfig/SysConfig.h"
#ifdef InputShl

#else

#define InputShl extern "C" __declspec(dllimport)

#endif

#define WM_IMM_MESSAGE   WM_USER + 0x1200
#define WM_IMM_DIAL_NUMBER_MESSAGE   WM_USER + 0x1201
enum ScreenSize {FourPoint3, ThreePoint5, SevenPoint0};										 //屏幕尺寸
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

InputShl void InitInputShl(ScreenSize nSize = FourPoint3);                //初始化设备

InputShl ImmReturn ImmCreateMessage(enLanguage enlang);					  //新建短信
InputShl ImmReturn ImmEditMessage(enLanguage enlang, LPTSTR strMessage);  //编辑短信

InputShl ImmReturn ImmAddPhone(enLanguage enlang);                        //新建用户
InputShl ImmReturn ImmEditPhone(enLanguage enlang, LPTSTR);	              //编辑用户

InputShl ImmReturn ImmAddPhoneNum(HWND hParentHwnd,HWND hWnd);             //新建电话号码
InputShl ImmReturn ImmEditPhoneNum(HWND hParentHwnd,HWND hWnd);	           //编辑电话号码
                              
InputShl IMMRESULT GetStrText();								          //取得输入字符串


typedef void  (*INIT)(ScreenSize nSize /*= FourPoint3*/);					   //初始化设备
typedef void  (*IMMSETKEYSOUND)(enKeySound enKey);					   //初始化设备

typedef ImmReturn (*IMMCREATEMESSAGE)(enLanguage enlang);				   //新建短信
typedef ImmReturn (*IMMEDITMESSAGE)(enLanguage enlang, LPTSTR strMessage); //编辑短信

typedef ImmReturn(*IMMEDITPHONE)(enLanguage enlang, LPTSTR strName, enKeySound enKey,enSkinType enSkin);       //新建用户
typedef ImmReturn(*IMMADDPHONE)(enLanguage enLanguage);                    //编辑用户

typedef ImmReturn(*IMMADDPHONENUM)(HWND hParentHwnd,HWND hWnd);             //新建电话号码
typedef ImmReturn(*IMMEDITPHONENUM)(HWND hParentHwnd,HWND hWnd);            //编辑电话号码

typedef IMMRESULT (*GETRESULT)();                                          //取得输入字符串

