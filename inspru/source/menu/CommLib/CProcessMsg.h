#ifndef CPROCESSMSG_H_
#define CPROCESSMSG_H_

//�����ڶ�����
LRESULT OnComRecvMsg(WPARAM wParam, LPARAM lParam,DWORD NaviThreadId);

//������ģ����Ϣ����������
LRESULT OnE920EventMsg(WPARAM wParam, LPARAM lParam);

//��UP
void KeyUp(char cUp);

//��DOWn
void KeyDown(char cDown);

#endif