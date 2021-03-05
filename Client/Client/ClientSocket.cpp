#include "stdafx.h"
#include "ClientSocket.h"

CClientSocket::CClientSocket(void)
{
}


CClientSocket::~CClientSocket(void)
{
}

void CClientSocket::OnReceive(int nErrorCode)
{
	if (m_pClientDlg != NULL)
		m_pClientDlg->ProcessRecvData();
	CSocket::OnReceive(nErrorCode);
}
