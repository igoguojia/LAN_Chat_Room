#pragma once
#include "afxsock.h"
class CClientDlg;
class CClientSocket :
	public CSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);
	virtual void OnReceive(int nErrorCode);
	CClientDlg *m_pClientDlg;
};

