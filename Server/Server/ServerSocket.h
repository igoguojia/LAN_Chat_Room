
#pragma once
#include "afxsock.h"
#include "ServerDlg.h"
class CServerDlg;

class CServerSocket :
	public CSocket
{
public:
	CServerSocket(void);
	~CServerSocket(void);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	CServerDlg* m_pSerDlg; // 主界面指针
	int m_thrdIndex; // 该CSocket所属线程的index，服务 Socket 为 -1
};
