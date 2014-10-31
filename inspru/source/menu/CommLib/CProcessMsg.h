#ifndef CPROCESSMSG_H_
#define CPROCESSMSG_H_

//处理串口读数据
LRESULT OnComRecvMsg(WPARAM wParam, LPARAM lParam,DWORD NaviThreadId);

//处理导航模块消息，发送数据
LRESULT OnE920EventMsg(WPARAM wParam, LPARAM lParam);

//键UP
void KeyUp(char cUp);

//键DOWn
void KeyDown(char cDown);

#endif