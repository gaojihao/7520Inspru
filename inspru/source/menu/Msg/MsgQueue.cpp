#include "MsgQueue.h"




CMsgQueue CMsgQueue::_instance;
UINT CMsgQueue::m_SpareTimer;


CMsgQueue::CMsgQueue(void)
{
}

CMsgQueue::~CMsgQueue(void)
{
}

CMsgQueue& CMsgQueue::Instance()
{
	return _instance;
}

void CMsgQueue::TimerMsgQue(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	HWND hWnd = ::FindWindow(NULL, _T("WND_GNSSTERMINAL"));
 
	if(!(hWnd != NULL))
		return;

	CppSQLite3DB db;
	int nCount;
	char szSql[512];
	memset(szSql, 0, sizeof(szSql));

	db.open(PATH_SQLITE_DB_808);	//打开数据库
	//查询记录总数量
	nCount = db.execScalar("SELECT count(*) FROM msg_queue;");

	if (nCount == 0)
	{
		::KillTimer(NULL, m_SpareTimer);
		return;
	}

	CppSQLite3Query q = db.execQuery("SELECT * FROM msg_queue;");

	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		if ( !q.eof() )	//数据行
		{
			memset(&Instance().m_stMsgque, 0, sizeof(Instance().m_stMsgque));

			Instance().m_stMsgque.nIndex = q.getIntField("nIndex");
			Instance().m_stMsgque.nMsgID = q.getIntField("msg_ID");
			Instance().m_stMsgque.nwParam = q.getIntField("wParam");
			Instance().m_stMsgque.nlParam = q.getIntField("lParam");
			Instance().m_stMsgque.nMsgType = q.getIntField("msg_type");

			::PostMessage(hWnd, Instance().m_stMsgque.nMsgID, Instance().m_stMsgque.nwParam, Instance().m_stMsgque.nlParam);


			memset(szSql, 0, sizeof(szSql));
			sprintf(szSql, "DELETE FROM msg_queue WHERE nIndex = %d;", Instance().m_stMsgque.nIndex);
			db.execDML( szSql );

			q.nextRow();
		}
		//发送完最后一条数据后,检查数据库中是否还有数据.
		if ((nCount-i) == 1)
		{
			i = 0;
			nCount = db.execScalar("SELECT count(*) FROM msg_queue;");
		}
	}
	db.close();
	::KillTimer(NULL, m_SpareTimer);
}

BOOL CMsgQueue::PostMsgQueue(int nMsgID, int nwParam, int nlParam, int nMsgType /* = 0 */)
{
	HWND hWnd = ::FindWindow(NULL, _T("WND_GNSSTERMINAL"));
	if(hWnd != NULL)
	{
		::PostMessage(hWnd, nMsgID, nwParam, nlParam);
		return TRUE;
	}
}


void CMsgQueue::StartTimerCheckDB(int nTime)
{
	::KillTimer(NULL, m_SpareTimer);
	m_SpareTimer = ::SetTimer(NULL, TID_MSGQUE, nTime, TimerMsgQue);
}