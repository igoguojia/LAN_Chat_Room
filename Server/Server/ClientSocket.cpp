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
	if (m_pSerDlg)
		m_pSerDlg->ProcessData(this);
	CSocket::OnReceive(nErrorCode);
}