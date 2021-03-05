#include "stdafx.h"
#include <afxwin.h>
#include <vector>
#include "ClientSocket.h"
#include <memory.h>
using namespace std;

extern vector<CClientSocket*> clientList;
CServerSocket::CServerSocket(void)
{
}


CServerSocket::~CServerSocket(void)
{
}

void CServerSocket::OnAccept(int nErrorCode)
{
	if(nErrorCode == 0)
	{
		if (m_pSerDlg->m_clientCnt < m_pSerDlg->m_maxClient)
		{
			CClientSocket* connectSocket=new CClientSocket;		// 创建一个连接Socket
			Accept(*connectSocket);			// connectSocket将和客户端连接
			//获得客户端IP和端口号
			connectSocket->GetPeerName(connectSocket->m_userIP,connectSocket->m_userPort);
			connectSocket->m_pSerDlg=m_pSerDlg;
			clientList.push_back(connectSocket);
			CString log;
			CTime tm=CTime::GetCurrentTime();
			log.Format(_T("【%s : %u】登录到服务器。"),connectSocket->m_userIP,connectSocket->m_userPort);
			log=tm.Format("%Y-%m-%d %X : ")+log;
			m_pSerDlg->m_ctrlChatRoomInfo.AddString(log);
			m_pSerDlg->m_clientCnt++;
			m_pSerDlg->UpdateData(FALSE);
		}
	}
	CSocket::OnAccept(nErrorCode);
}

void CServerSocket::OnReceive(int nErrorCode)
{
	
}