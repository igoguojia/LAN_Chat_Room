
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

	CServerDlg* m_pSerDlg; // ������ָ��
	int m_thrdIndex; // ��CSocket�����̵߳�index������ Socket Ϊ -1
};
