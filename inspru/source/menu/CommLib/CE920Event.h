#ifndef E920EVENT_H_
#define E920EVENT_H_

const char DLE = (char)0x5a;		////协议桢头
const char ETX = (char)0xa5;		//协议桢尾

const UINT E920BufSize = 512; //通用缓冲区长度

CONST UINT WM_E920EVENT = WM_USER + 619;	//E920事件消息

//E920协议桢结构类
class CE920Event
{
public:
	CE920Event(char cmd,UINT iDataLen = 0,char* pData = NULL)
	{
		bCmd = cmd;
		bDataLen = iDataLen;
		memset(bpData,0,E920BufSize);
		for(UINT i=0; i<iDataLen; i++)
		{
			bpData[i] = pData[i];
		}
	};

	CE920Event()	{};

	virtual ~CE920Event() {};
	
	char bCmd;
	char bDataLen;
	char bpData[E920BufSize];
};

//扫描码定义
enum LC_KEY
{
	LC_KEY_PAGE				= 0x10 ,	//VK_SPACE
	LC_KEY_MENU				= 0x11 ,	//VK_MENU
	LC_KEY_ENTER			= 0x12 ,	//VK_RETURN
	LC_KEY_ARROW_UP			= 0x13 ,	//VK_UP
	LC_KEY_ARROW_DOWN		= 0x14 ,	//VK_DOWN
	LC_KEY_ARROW_LEFT		= 0x15 ,	//VK_LEFT
	LC_KEY_ARROW_RIGHT		= 0x16 ,	//VK_RIGHT
	LC_KEY_1_ROUTE			= 0x17 ,	//VK_F1
	LC_KEY_2_RECORD			= 0x18 ,	//VK_F5
	LC_KEY_3_DET			= 0x19 ,	//VK_F2
	LC_KEY_4_VOICE			= 0x1a ,	//VK_F3
	LC_KEY_5_FIND			= 0x1b ,	//VK_F4
	LC_KEY_6_ESC			= 0x1c		//VK_ESCAPE
};

//遥控器按键定义
enum RM_KEY
{
	RM_KEY_PAGE				= 0x30 ,	//VK_SPACE
	RM_KEY_MENU				= 0x31 ,	//VK_MENU
	RM_KEY_ENTER			= 0x32 ,	//VK_RETURN
	RM_KEY_ARROW_UP			= 0x33 ,	//VK_UP
	RM_KEY_ARROW_DOWN		= 0x34 ,	//VK_DOWN
	RM_KEY_ARROW_LEFT		= 0x35 ,	//VK_LEFT
	RM_KEY_ARROW_RIGHT		= 0x36 ,	//VK_RIGHT
	RM_KEY_ROUTE			= 0x37 ,	//VK_F1
	RM_KEY_DET				= 0x38 ,	//VK_F2
	RM_KEY_VOICE			= 0x39 ,	//VK_F3
	RM_KEY_FIND				= 0x3a ,	//VK_F4
	RM_KEY_ESC				= 0x3b ,	//VK_ESCAPE
	RM_KEY_0				= 0x3c ,	//VK_NUMPAD0
	RM_KEY_1				= 0x3d ,	//VK_NUMPAD1
	RM_KEY_2				= 0x3e ,	//VK_NUMPAD2
	RM_KEY_3				= 0x3f ,	//VK_NUMPAD3
	RM_KEY_4				= 0x40 ,	//VK_NUMPAD4
	RM_KEY_5				= 0x41 ,	//VK_NUMPAD5
	RM_KEY_6				= 0x42 ,	//VK_NUMPAD6
	RM_KEY_7				= 0x43 ,	//VK_NUMPAD7
	RM_KEY_8				= 0x44 ,	//VK_NUMPAD8
	RM_KEY_9				= 0x45 ,	//VK_NUMPAD9
	RM_KEY_STAR				= 0x46 ,	//VK_MULTIPLY
	RM_KEY_SHIFT			= 0x47		//VK_ADD
};

//系统控制事件定义
enum EVENT
{
	EVENT_SHUTDOWN			= 0x60 ,	//VK_ADD
	EVENT_VIDEO				= 0x61 ,	//VK_ADD
	EVENT_VOICE				= 0x62 ,	//VK_ADD
	EVENT_INIT_OK			= 0x63 ,	//VK_ADD
	EVENT_NAV				= 0x64 ,	//VK_ADD
	EVENT_WORK				= 0x65 ,	//VK_ADD
	EVENT_LOCAL_TIME		= 0x66 ,
	EVENT_KEYDOWN			= 0x10 ,	//键按下
	EVENT_KEYUP				= 0x11		//键弹起
};


//协议类型定义
enum PROTOCOL_TYPE
{
	en_E920Event_PROTOCOL		= 0
	//Other Protocol
};
#endif
