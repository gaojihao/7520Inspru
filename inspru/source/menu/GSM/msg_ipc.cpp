
#include "msg_ipc.h"


//////////////////////////////////////////////////////////////////////////

msg_ipc::msg_ipc()
{
	m_hwnd_svr = 0;
}

//////////////////////////////////////////////////////////////////////////

msg_ipc::~msg_ipc()
{
}

//////////////////////////////////////////////////////////////////////////

bool   msg_ipc::init_target( TCHAR* win_cls, TCHAR* win_name  )
{
	if ( win_cls == NULL && win_name == NULL )
  		return init_target(GSMSVR_CLS_NAME, WND_NAME_GSM_AT );

	for( unsigned i = 0 ; i < 10; i++ )
	{
		m_hwnd_svr = FindWindow( win_cls, win_name );
		if ( m_hwnd_svr != 0 )
			break;
		Sleep(0);
	}
	
	return ( m_hwnd_svr != 0 );
}

//////////////////////////////////////////////////////////////////////////

void msg_ipc::post_msg(unsigned n_msg,unsigned int w_param, long l_param)
{
	if ( m_hwnd_svr == NULL )
		init_target(NULL, NULL);
		 
	if ( m_hwnd_svr )
		::PostMessage(m_hwnd_svr, n_msg, w_param, l_param );
}

//////////////////////////////////////////////////////////////////////////

void msg_ipc::send_msg(unsigned n_msg,unsigned int w_param, long l_param)
{
	if ( m_hwnd_svr == NULL )
		init_target(NULL, NULL);

	if ( m_hwnd_svr )
		::SendMessage(m_hwnd_svr,n_msg, w_param, l_param );
}

//////////////////////////////////////////////////////////////////////////

void   msg_ipc::past_data(unsigned n_type, const char* p_data, int n_len )
{
	if ( m_hwnd_svr == NULL )
		init_target(NULL, NULL);

	if ( m_hwnd_svr )
	{
		char sz_data[512];
		memset(sz_data, 0, sizeof(sz_data) );
		int n_data_len = n_len;
		if ( n_data_len == -1 )
			n_data_len = strlen( p_data );
		
		n_data_len = min(n_data_len, sizeof(sz_data)/sizeof(sz_data[0]) );
		memcpy(sz_data, p_data, n_data_len );
		
		COPYDATASTRUCT  cds;
		cds.dwData = n_type;
		cds.cbData = n_data_len;
		cds.lpData = (PVOID)sz_data;
		::SendMessage( m_hwnd_svr, WM_COPYDATA, (WPARAM)m_owner, (LPARAM)&cds);
	}
}