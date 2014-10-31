#pragma once

#include "stdafx.h"
#include "../../GSM/diaodu_data.h"
#include "../../SQLite3/CppSQLite3.h"


#define TID_MSGQUE	3042


class CMsgQueue
{

typedef struct
{
	int nIndex;
	int nMsgID;
	int nlParam;
	int nwParam;
	int nMsgType;
} stMsg;

public:
	CMsgQueue(void);
	~CMsgQueue(void);

	static	CMsgQueue&	Instance();
	static	void		TimerMsgQue(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	void StartTimerCheckDB(int nTime = 5*60*1000);
	BOOL PostMsgQueue(int nMsgID, int nwParam, int nlParam, int nMsgType = 0);
public:
	static UINT m_SpareTimer;
	stMsg m_stMsgque;
private:
	static CMsgQueue _instance;
};
