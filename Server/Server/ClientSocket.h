#pragma once
#include "afxsock.h"

class CClientSocket :
	public CSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);
	virtual void OnReceive(int nErrorCode);

	CString m_userName;		//�û���
	CString m_userIP;		//�û�IP
	CString m_usrNP;		//�û���������
	UINT m_userPort;		//�û��˿�
	CString m_usrLove;
	CString m_usrAge;
	CString m_usrSex;
	CServerDlg *m_pSerDlg;	//������ָ��
};

