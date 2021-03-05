#pragma once
#include "afxsock.h"

class CClientSocket :
	public CSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);
	virtual void OnReceive(int nErrorCode);

	CString m_userName;		//用户名
	CString m_userIP;		//用户IP
	CString m_usrNP;		//用户名和密码
	UINT m_userPort;		//用户端口
	CString m_usrLove;
	CString m_usrAge;
	CString m_usrSex;
	CServerDlg *m_pSerDlg;	//主界面指针
};

