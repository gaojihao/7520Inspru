
#ifndef   _MACROCODE_H
#define   _MACROCODE_H

/*++
  ====+ (C) Copyright 2006.  eCompass Corp.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $public\MacroCode.h

 $Abstract:
      
		This public header file specifies all structures, macro, global
		
	  variables and function prototypes ,which presents functions to 
	
  	 be used  in project EurpNav in the WINCE platform.

 $Reversion History:

         If this module does work properly, it is created by LiuHL, if not,

	  i don't know who writes these codes. 

        (1) LiuHL  Create file on 05/08/2006

 $Notes:
		
	    Increment this if a change has a global effects.
--*/



#define     PATH_CONFIG_USER          _T("UserData")
#define     PATH_CONFIG_RES           _T("SysRes")
//
//  define macro for user show & hide dialog
//
#define     MCR_ShowDlgInTop( nID ) \
	{ \
		CDialogBase* p =  CUiMng::Instance().GetWindowPtr( nID ); \
		p->Open(); \
	   if ( p && ::IsWindow(p->GetSafeHwnd()) ) \
	   {	\
		 p->ShowWindow( SW_SHOW ); \
		 ::SetWindowPos(p->GetSafeHwnd(),HWND_TOP,0,0,0,0,SWP_NOSIZE); \
		}	\
	} \


#define     MCR_HideDlg( nID ) \
	{ \
		CDialogBase* p =  CUiMng::Instance().GetWindowPtr( nID ); \
	   if ( p && ::IsWindow(p->GetSafeHwnd()) ) \
			p->ShowWindow( SW_HIDE ); \
	} \

#define  MCR_ShowInputCtrl(t, rtn) \
	{\
		CDialogBase * pBase = CUiMng::Instance().GetWindowPtr(IDD_PANEL_320X240);\
		pBase->SetModal(TRUE);  \
		CInputPanelSh * pDlg = (CInputPanelSh*)pBase;  \
		pDlg->SetStringSrc(t); \
		rtn = pBase->Open();   \
		CUiMng::Instance().ReleaseWindow(IDD_PANEL_320X240); \
	} \
 
//参数：cursel是enInputDataType类型枚举值
#define  MCR_ShowInputCtrl_CurSel(t, cursel, rtn) \
	{\
		CDialogBase * pBase = CUiMng::Instance().GetWindowPtr(IDD_PANEL_320X240);\
		pBase->SetModal(TRUE);  \
		CInputPanelSh * pDlg = (CInputPanelSh*)pBase;  \
		pDlg->SetStringSrc(t); \
		pDlg->SetDataType(cursel); \
		rtn = pBase->Open();   \
		CUiMng::Instance().ReleaseWindow(IDD_PANEL_320X240); \
	} \

#define  MCR_SysTipOK(t)	\
	{\
		CDlgSysTip * pDlg = (CDlgSysTip*)CUiMng::Instance().GetWindowPtr(IDD_SYSTIP_DLG);\
		pDlg->SetModal(TRUE);  \
		pDlg->SetSysTip(t,TRUE); \
		pDlg->Open();   \
		CUiMng::Instance().ReleaseWindow(IDD_SYSTIP_DLG); \
	} \

#define  MCR_SysTipIsOK(t, rtn ) \
	{\
		CDlgSysTip * pDlg = (CDlgSysTip*)CUiMng::Instance().GetWindowPtr(IDD_SYSTIP_DLG);\
		pDlg->SetModal(TRUE);  \
		pDlg->SetSysTip(t,FALSE); \
		rtn = pDlg->Open();   \
		CUiMng::Instance().ReleaseWindow(IDD_SYSTIP_DLG); \
	} \

#define  MCR_SysTipIsCheck(bS, t, tCheck, rtn, bCheck ) \
	{\
		CDlgSysTipNavi * pDlg = (CDlgSysTipNavi*)CUiMng::Instance().GetWindowPtr(IDD_SYSTIP_NAVI_DLG);\
		pDlg->SetModal(TRUE);  \
		pDlg->SetSysTip(t,tCheck,bCheck); \
		pDlg->SetbCalSuccess(bS); \
		rtn = pDlg->Open();   \
		CUiMng::Instance().ReleaseWindow(IDD_SYSTIP_NAVI_DLG); \
	} \

#define     MCR_SendMsg2Frame(uMsg)  ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,(uMsg),0,0)
#define     MCR_PostMsg2Frame(uMsg)  ::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,(uMsg),0,0)





#if defined(_WATCH_SYSTEM)

#define   WACHING_MEMORY_SYSTEM  \
	{\
		CSysWatch * pWatch =  CSysWatch::Instance(); \
		pWatch->WatchSyBeforeStartup(); \
		pWatch->StartTimerForWathcing(); \
	} \

#define   WATCH_MEMORY_START \
	{\
		CSysWatch * pWatch =  CSysWatch::Instance(); \
		pWatch->WatchStart(); \
	} \

#define   WATCH_MEMORY_STOP \
	{\
		CSysWatch * pWatch =  CSysWatch::Instance();  \
		pWatch->WatchStop(); \
	} \	

#define   WATCH_TIME_START \
	{\
		CSysWatch * pWatch =  CSysWatch::Instance(); \
		pWatch->TimeStart(); \
	} \

#define   WATCH_TIME_STOP \
	{\
		CSysWatch * pWatch =  CSysWatch::Instance(); \
		pWatch->TimeElapsed(); \
	} \


#else // ndef (_WATCH_SYSTEM)

#define   WACHING_MEMORY_SYSTEM		((void)0)
#define   WATCH_MEMORY_START		((void)0)
#define   WATCH_MEMORY_STOP			((void)0)
#define   WATCH_TIME_START			((void)0)
#define   WATCH_TIME_STOP			((void)0)

#endif  // (_WATCH_SYSTEM)


//////////////////////////////////////////////////////////////////////////

#define   MC_CLEAR_THREAD_MOUSEEVENT \
	{ \
		MSG msg; \
		while (PeekMessage(&msg,NULL, \
			WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)) \
		{ \
			; \
		} \
	} 

#define  MC_CLEAR_WND_MOUSEEVENT(hWnd ) \
	{ \
		MSG msg; \
		while (PeekMessage(&msg,(hWnd), \
			WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)) \
		{ \
			; \
		} \
	} 


//////////////////////////////////////////////////////////////////////////


#endif  //_MACROCODE_H