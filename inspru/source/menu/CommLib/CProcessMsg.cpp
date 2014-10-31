#include "stdafx.h"
#include "CProcessMsg.h"
#include "CSerial.h"
#include "CE920Event.h"
#include "CE920EventProtocol.h"
#include "CCommMng.h"
#include "Afx.h"

//键UP
void KeyUp(char cUp)
{
		switch(cUp)
		{
//扫描码
		case LC_KEY_PAGE:
			keybd_event(VK_SPACE,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_MENU:
			keybd_event(VK_APPS,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_ENTER:
			keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_ARROW_UP:
			keybd_event(VK_UP,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_ARROW_DOWN:
			keybd_event(VK_DOWN,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_ARROW_LEFT:
			keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_ARROW_RIGHT:
			keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_1_ROUTE:
			keybd_event(VK_F1,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_2_RECORD:
			keybd_event(VK_F5,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_3_DET:
			keybd_event(VK_F2,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_4_VOICE:
			keybd_event(VK_F3,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_5_FIND:
			keybd_event(VK_F4,0,KEYEVENTF_KEYUP,NULL);
			break;
		case LC_KEY_6_ESC:
			keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,NULL);
			break;
//遥控器按键
		case RM_KEY_PAGE:
			keybd_event(VK_SPACE,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_MENU:
			keybd_event(VK_APPS,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ENTER:
			keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ARROW_UP:
			keybd_event(VK_UP,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ARROW_DOWN:
			keybd_event(VK_DOWN,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ARROW_LEFT:
			keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ARROW_RIGHT:
			keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ROUTE:
			keybd_event(VK_F1,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_DET:
			keybd_event(VK_F2,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_VOICE:
			keybd_event(VK_F3,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_FIND:
			keybd_event(VK_F4,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_ESC:
			keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_0:
			keybd_event(VK_NUMPAD0,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_1:
			keybd_event(VK_NUMPAD1,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_2:
			keybd_event(VK_NUMPAD2,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_3:
			keybd_event(VK_NUMPAD3,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_4:
			keybd_event(VK_NUMPAD4,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_5:
			keybd_event(VK_NUMPAD5,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_6:
			keybd_event(VK_NUMPAD6,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_7:
			keybd_event(VK_NUMPAD7,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_8:
			keybd_event(VK_NUMPAD8,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_9:
			keybd_event(VK_NUMPAD9,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_STAR:
			keybd_event(VK_MULTIPLY,0,KEYEVENTF_KEYUP,NULL);
			break;
		case RM_KEY_SHIFT:
			keybd_event(VK_ADD,0,KEYEVENTF_KEYUP,NULL);
			break;
		default:
			break;
		}
}

//键DOWn
void KeyDown(char cDown)
{
		switch(cDown)
		{
//扫描码
		case LC_KEY_PAGE:
			keybd_event(VK_SPACE,0,0,NULL);
			break;
		case LC_KEY_MENU:
			keybd_event(VK_APPS,0,0,NULL);
			break;
		case LC_KEY_ENTER:
			keybd_event(VK_RETURN,0,0,NULL);
			break;
		case LC_KEY_ARROW_UP:
			keybd_event(VK_UP,0,0,NULL);
			break;
		case LC_KEY_ARROW_DOWN:
			keybd_event(VK_DOWN,0,0,NULL);
			break;
		case LC_KEY_ARROW_LEFT:
			keybd_event(VK_LEFT,0,0,NULL);
			break;
		case LC_KEY_ARROW_RIGHT:
			keybd_event(VK_RIGHT,0,0,NULL);
			break;
		case LC_KEY_1_ROUTE:
			keybd_event(VK_F1,0,0,NULL);
			break;
		case LC_KEY_2_RECORD:
			keybd_event(VK_F5,0,0,NULL);
			break;
		case LC_KEY_3_DET:
			keybd_event(VK_F2,0,0,NULL);
			break;
		case LC_KEY_4_VOICE:
			keybd_event(VK_F3,0,0,NULL);
			break;
		case LC_KEY_5_FIND:
			keybd_event(VK_F4,0,0,NULL);
			break;
		case LC_KEY_6_ESC:
			keybd_event(VK_ESCAPE,0,0,NULL);
			break;
//遥控器按键
		case RM_KEY_PAGE:
			keybd_event(VK_SPACE,0,0,NULL);
			break;
		case RM_KEY_MENU:
			keybd_event(VK_APPS,0,0,NULL);
			break;
		case RM_KEY_ENTER:
			keybd_event(VK_RETURN,0,0,NULL);
			break;
		case RM_KEY_ARROW_UP:
			keybd_event(VK_UP,0,0,NULL);
			break;
		case RM_KEY_ARROW_DOWN:
			keybd_event(VK_DOWN,0,0,NULL);
			break;
		case RM_KEY_ARROW_LEFT:
			keybd_event(VK_LEFT,0,0,NULL);
			break;
		case RM_KEY_ARROW_RIGHT:
			keybd_event(VK_RIGHT,0,0,NULL);
			break;
		case RM_KEY_ROUTE:
			keybd_event(VK_F1,0,0,NULL);
			break;
		case RM_KEY_DET:
			keybd_event(VK_F2,0,0,NULL);
			break;
		case RM_KEY_VOICE:
			keybd_event(VK_F3,0,0,NULL);
			break;
		case RM_KEY_FIND:
			keybd_event(VK_F4,0,0,NULL);
			break;
		case RM_KEY_ESC:
			keybd_event(VK_ESCAPE,0,0,NULL);
			break;
		case RM_KEY_0:
			keybd_event(VK_NUMPAD0,0,0,NULL);
			break;
		case RM_KEY_1:
			keybd_event(VK_NUMPAD1,0,0,NULL);
			break;
		case RM_KEY_2:
			keybd_event(VK_NUMPAD2,0,0,NULL);
			break;
		case RM_KEY_3:
			keybd_event(VK_NUMPAD3,0,0,NULL);
			break;
		case RM_KEY_4:
			keybd_event(VK_NUMPAD4,0,0,NULL);
			break;
		case RM_KEY_5:
			keybd_event(VK_NUMPAD5,0,0,NULL);
			break;
		case RM_KEY_6:
			keybd_event(VK_NUMPAD6,0,0,NULL);
			break;
		case RM_KEY_7:
			keybd_event(VK_NUMPAD7,0,0,NULL);
			break;
		case RM_KEY_8:
			keybd_event(VK_NUMPAD8,0,0,NULL);
			break;
		case RM_KEY_9:
			keybd_event(VK_NUMPAD9,0,0,NULL);
			break;
		case RM_KEY_STAR:
			keybd_event(VK_MULTIPLY,0,0,NULL);
			break;
		case RM_KEY_SHIFT:
			keybd_event(VK_ADD,0,0,NULL);
			break;
		default:
			break;
		}
}

bool g_bShutDown = false;	//标志是否正在关机

LRESULT OnComRecvMsg(WPARAM wParam, LPARAM lParam,DWORD NaviThreadId)
{
	CE920Event* pE920Event = new CE920Event;
	CCommMng& rMng = CCommMng::Instance();
	if(rMng.RecvData(pE920Event))
	{
		switch(pE920Event->bCmd)
		{
		case EVENT_KEYDOWN:
			KeyDown(*(pE920Event->bpData));
			break;
		case EVENT_KEYUP:
			KeyUp(*(pE920Event->bpData));
			break;
		case EVENT_SHUTDOWN:
			//导航模块ID
			PostThreadMessage(NaviThreadId,WM_QUIT,0x00,0x00);
			g_bShutDown = true;
			break;
		default:
			break;	
		}
	}

	if(pE920Event != NULL)
	{
		delete pE920Event;
		pE920Event = NULL;
	}
	return TRUE;
}

LRESULT OnE920EventMsg(WPARAM wParam, LPARAM lParam)
{
	char bCmd = (char)wParam;
	char bData = (char)lParam;
	CE920Event* pE920Event = NULL;
	switch(bCmd)
	{
	case EVENT_SHUTDOWN:
		if(bData == 0x01)
		{
			if (!g_bShutDown)	//不是正在关机
			{	//就是异常退出
				bCmd = EVENT_WORK;
				bData = 0x00;
			}
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
			g_bShutDown = false;
		}
		break;
	case EVENT_VIDEO:
		if(bData == 0x00)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		else if (bData == 0x01)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		break;
	case EVENT_VOICE:
		if(bData == 0x00)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		else if (bData == 0x01)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		break;
	case EVENT_INIT_OK:
		if(bData == 0x01)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		break;
	case EVENT_NAV:
		if(bData == 0x00)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		else if (bData == 0x01)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		break;
	case EVENT_WORK:
		if(bData == 0x00)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		else if (bData == 0x01)
		{
			pE920Event = new CE920Event(bCmd,1,&bData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
		}
		break;
	case EVENT_LOCAL_TIME:
		{
			CTime t = CTime::GetCurrentTime();
			struct {
				WORD year;
				BYTE month, day, hour, minute, second;
			}tData;
			tData.year = t.GetYear();
			tData.month = t.GetMonth();
			tData.day = t.GetDay();
			tData.hour = t.GetHour();
			tData.minute = t.GetMinute();
			tData.second = t.GetSecond();

			pE920Event = new CE920Event(bCmd,7,(char*)&tData);
			CCommMng& rMng = CCommMng::Instance();
			rMng.SendData(pE920Event);
			break;
		}
	default:
		break;
	}
	
	if(pE920Event != NULL)
	{
		delete pE920Event;
		pE920Event = NULL;
	}

	return TRUE;
}