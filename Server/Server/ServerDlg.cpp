
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "afxdialogex.h"
#include "ServerDlg.h"
#include "ClientSocket.h"
#include <vector>
#include<iostream>
using namespace std;
#pragma warning(disable:4996)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<CClientSocket*> clientList;//�ͻ����б�
vector<CString >filelist;//�ļ��б�

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg �Ի���



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, m_clientCnt(0)
	, m_listenPort(2537)
	, m_maxClient(50)
	, m_srvrName(_T(""))
	, m_srvrStatus(_T("��ֹͣ"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_serverSocket.m_pSerDlg=this;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLIENTCNT, m_clientCnt);
	DDX_Text(pDX, IDC_LISTENPORT, m_listenPort);
	DDX_Control(pDX, IDC_SERVERIP, m_ctrlServerIP);
	DDX_Text(pDX, IDC_SERVERNAME, m_srvrName);
	DDX_Text(pDX, IDC_STATUS, m_srvrStatus);
	DDX_Control(pDX, IDC_MAXCLIENT, m_ctrlMaxCnt);
	DDX_Control(pDX, IDC_CHATROOMINFO, m_ctrlChatRoomInfo);
	DDX_Control(pDX, IDC_LIST2, m_ctrlUsers);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_START_STOP, &CServerDlg::OnBnClickedBtnStartStop)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CServerDlg::OnBnClickedBtnClear)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CServerDlg::OnBnClickedBtnExport)

	ON_BN_CLICKED(IDC_BUTTON1, &CServerDlg::OnGetUsersNP)
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (!AfxSocketInit())
	{
	   AfxMessageBox(_T("Failed to Initialize Sockets"), MB_OK | MB_ICONSTOP);
	   exit(1);
	}
	char hostname[256];
	PHOSTENT hostinfo;
	CString IP;
	gethostname(hostname,sizeof(hostname));		//��ȡ������
	hostinfo=gethostbyname(hostname);			//������������ȡ����IP

	m_srvrName=hostname;	//���÷���������
	IP=inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
	m_ctrlServerIP.SetWindowTextW(IP);	//���÷�����IP
	
	m_ctrlMaxCnt.AddString(_T("50"));
	m_ctrlMaxCnt.AddString(_T("40"));
	m_ctrlMaxCnt.AddString(_T("30"));
	m_ctrlMaxCnt.AddString(_T("20"));
	m_ctrlMaxCnt.AddString(_T("10"));
	m_ctrlMaxCnt.AddString(_T("5"));
	m_ctrlMaxCnt.SetCurSel(0);
	CString folderPath = _T("�ļ�\\");
	if (!PathIsDirectory(folderPath))  // �Ƿ��������ļ���
	{
		::CreateDirectory(folderPath, 0);
	}
	m_serverSocket.m_pSerDlg=this;
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap bmpBackground,ico;   

       bmpBackground.LoadBitmap(IDB_BACKGROUND); 
       BITMAP bitmap;   
       bmpBackground.GetBitmap(&bitmap);   
       CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);   

       dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

		//CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CDialogEx::OnOK();
}


void CServerDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if(IDYES==MessageBox(_T("ȷ�Ϲرշ�������"),_T("��ʾ"),MB_ICONQUESTION|MB_YESNO))
	{
		CDialogEx::OnCancel();
	}*/
	CDialogEx::OnCancel();

}


void CServerDlg::OnBnClickedBtnStartStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_srvrStatus=="��ֹͣ")	//״̬Ϊ��ֹͣ����Ҫ���������������˿�
	{
		UpdateData();
		WCHAR str[3];
		m_ctrlMaxCnt.GetWindowTextW(str,3);//�����ͻ����� �Ǹ��������ڵ�ǰ������������str��
		m_maxClient=_ttoi(str);//��str��Stringת����int
		if (m_listenPort<1024)
		{
			MessageBox(_T("��ѡ�����1024�Ķ˿ڣ�"),_T("��ʾ"),MB_ICONWARNING);
			return;
		}
		if (!m_serverSocket.Create(m_listenPort))
		{
			MessageBox(_T("�����׽��ֳ���"),_T("����"),MB_ICONERROR);
			return;
		}
		if (!m_serverSocket.Listen())
		{
			MessageBox(_T("����ʧ�ܣ�"),_T("����"),MB_ICONERROR);
			return;
		}
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("ֹͣ"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(FALSE);
	}
	else		//״̬Ϊ���ڼ�����Ҫֹͣ����
	{
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("����"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(TRUE);
		m_clientCnt = 0;
		vector<CClientSocket *>::iterator iter;
		for (iter = clientList.begin(); iter != clientList.end(); )
		{
			unsigned char buf[2]={0};
			buf[0]=SERVERCLOSE;
			(*iter)->Send(buf,1);
			(*iter)->Close();
			delete (*iter);
			iter=clientList.erase(iter);
		}
		clientList.clear();
		m_serverSocket.Close();//�رշ�����׽���
	}

	m_srvrStatus=m_srvrStatus=="���ڼ�������"?"��ֹͣ":"���ڼ�������";
	GetDlgItem(IDC_STATUS)->SetWindowTextW(m_srvrStatus);
	UpdateData(FALSE);
}



void CServerDlg::OnBnClickedBtnClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlChatRoomInfo.ResetContent();
}


HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int CtrlID=pWnd->GetDlgCtrlID();
	switch (CtrlID)
	{
	case IDC_SERVERNAME:
	case IDC_STATUS:
	case IDC_CLIENTCNT:
		pDC->SetBkColor(RGB(255, 255, 255));
		return ::CreateSolidBrush(RGB(255, 255, 255));
	case IDC_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));  //������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}


void CServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_serverSocket.Close();
	CDialogEx::OnClose();
}

void CServerDlg::ProcessData(CClientSocket *clientSocket)
{
	unsigned char buf[MAX_BUFSIZE] = { 0 };
	wchar_t wbuf[MAX_BUFSIZE] = { 0 };
	wchar_t srcName[16] = { 0 }, dstName[16] = { 0 };
	int len = clientSocket->Receive(buf, MAX_BUFSIZE);
	vector<CClientSocket *>::iterator iter;
	vector<CString>::iterator file_iter;//�ļ��б�
	CString log, usrName, usrNP, usrLove, usrAge, usrSex;
	CTime tm;
	bool undefine = true;//�ж��Ƿ�ע�����������ȷ
	CString data_c_srcName;
	unsigned char data_buf[MAX_BUFSIZE] = { 0 };

	HANDLE hFile;//�ļ����	
	CString strPath;
	char CH_nameLength = NULL;
	int IN_namelength = 0;
	CString temp_File_Name;//��ʱ�ļ���string
	CString temp_File_List;

	DWORD dwSize=0;//�ļ�����
	unsigned int strName_length = 0;
	char chName_length;
	int File_num = 0;//Ҫ���ص��ļ���λ��
	char charFileLog[10] = { 0 };//��׺��
	switch (buf[0])
	{
	case NEWCLIENT:		//�յ��������û�������Ϣ��ת���������ˣ��������û�����ȫ�������û���Ϣ
		memcpy(wbuf, buf + 1, 16);		//��ȡ�û���		
		usrName = wbuf;
		memcpy(wbuf, buf + 1, len - 1);		//��ȡ�û���������
		usrNP = wbuf;
		clientSocket->m_userName = usrName;		//�����û���
		for (int i = 0; i < m_ctrlUsers.GetCount(); i++)
		{
			CString str;
			m_ctrlUsers.GetText(i, str);
			if (usrNP == str)
			{
				undefine = false;
				break;
			}
		}
		if (undefine)
		{
			tm = CTime::GetCurrentTime();
			log.Format(_T("��%s : %u����������Ͽ���"), clientSocket->m_userIP, clientSocket->m_userPort);
			log = tm.Format("%Y-%m-%d %X : ") + log;
			m_ctrlChatRoomInfo.AddString(log);

			buf[0] = ERROR;
			clientSocket->Send(buf, 1);		//�û�δע������벻��ȷ
			clientSocket->Close();
			delete clientSocket;
			clientList.pop_back();
			m_clientCnt--;
			return;
		}
		//clientSocket->m_userName = usrName;		//�����û���
		/*for (iter = clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				buf[0] = REPEATNAME;
				clientSocket->Send(buf, 1);		//�û����ظ�
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}*/
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if (*iter != NULL)
			{
				(*iter)->Send(buf, 17);		//ת����ÿ���ͻ���
				//ǰʮ�߸��ֽ�ʱ�� һ���ֽڱ�־��8�����ַ�
			}
		}
		Sleep(100);
		for (iter = clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if (*iter != NULL)
			{
				buf[0] = ONLINECLIENT;
				memcpy(wbuf, (*iter)->m_userName.GetBuffer(0), (*iter)->m_userName.GetLength() * 2);
				memcpy(buf + 1, wbuf, (*iter)->m_userName.GetLength() * 2);
				clientSocket->Send(buf, (*iter)->m_userName.GetLength() * 2 + 1); //���������û�����Ϣ�������ߵ��û�
				Sleep(100);
			}
		}
		break;
	case REGISTER://ע��
		memcpy(wbuf, buf + 1, 16);		//��ȡ�û���
		usrName = wbuf;
		memcpy(wbuf, buf + 1, len - 1);		//��ȡ�û���������
		usrNP = wbuf;
		

		clientSocket->m_usrNP = usrNP;		//�����û���������
		clientSocket->m_userName = usrName;		//�����û���
	/*	for (iter = clientList.begin(); iter < clientList.end()-1; iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				buf[0] = REPEATNAME;
				clientSocket->Send(buf, 1);		//�û����ظ�
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}*/
		for (int i = 0; i < m_ctrlUsers.GetCount(); i++)
		{
			CString str;
			m_ctrlUsers.GetText(i, str);
			str = str.Left(8);
			if (usrName == str)
			{
				undefine = false;
				buf[0] = HADDEFINED;
				clientSocket->Send(buf, 1);		//�û���ע���
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}
		if (undefine)
		{
			/*CString log;
			CTime tm = CTime::GetCurrentTime();
			log.Format(_T("�û���"), clientSocket->m_userName);
			log = tm.Format("%Y-%m-%d %X : ") + log;*/
			this->m_ctrlUsers.AddString(usrNP);
			CString strPath, strLog;
			strPath = _T("users.txt");
			CFile file(strPath, CFile::modeCreate | CFile::modeWrite);
			for (int i = 0; i < m_ctrlUsers.GetCount(); i++)
			{
				CString str;
				m_ctrlUsers.GetText(i, str);
				strLog += str;
				strLog += _T("\r\n");
			}
			WORD unicode = 0xFEFF;  //�����Ҫ��ע��������F E FF���м��и�E
			file.Write(&unicode, 2);  //�����Ҫ
			file.Write(strLog, strLog.GetLength() * 2);
			file.Close();

			tm = CTime::GetCurrentTime();
			log.Format(_T("��%s : %u����������Ͽ���"), clientSocket->m_userIP, clientSocket->m_userPort);
			log = tm.Format("%Y-%m-%d %X : ") + log;
			m_ctrlChatRoomInfo.AddString(log);

			buf[0] = REGISTER;
			clientSocket->Send(buf, 1);		//ע���û�
			clientSocket->Close();
			delete clientSocket;
			clientList.pop_back();
			m_clientCnt--;

			UpdateData(false);

			return;

		}
		break;
		//clientSocket->m_userName = usrName;		//�����û���
		/*for (iter = clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				buf[0] = REPEATNAME;
				clientSocket->Send(buf, 1);		//�û����ظ�
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}*/
	case WDATA://������������
		memcpy(dstName, buf + 1, 16);//������
		memcpy(srcName, buf + 17, 16);//Ҫ����
		data_c_srcName = srcName;
		data_buf[0] = WDATA;
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == dstName)	//�������û�
			{
				//��������
				usrAge = (*iter)->m_usrAge;
				usrSex = (*iter)->m_usrSex;
				usrLove = (*iter)->m_usrLove;
				break;
			}
		}
		memcpy(data_buf + 1, data_c_srcName.GetBuffer(0), data_c_srcName.GetLength() * 2);//16�ֽ�Ҫ�����ϵ��û���
		memcpy(data_buf + 17, usrAge.GetBuffer(0), usrAge.GetLength() * 2);//6�ֽ�����
		memcpy(data_buf + 23, usrSex.GetBuffer(0), usrSex.GetLength() * 2);//2�ֽ��Ա�
		memcpy(data_buf + 25, usrLove.GetBuffer(0), usrLove.GetLength() * 2);//����
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == srcName)	//�������û�
			{
				//��������
				(*iter)->Send(data_buf, usrLove.GetLength() * 2 + 25);	//������Ϣ
				break;
			}
		}
		break;

	case UpDATA://�ϴ�����
		memcpy(wbuf, buf + 1, 16);
		usrName = wbuf;
		memcpy(wbuf + 16, buf + 17, 6);
		usrAge = wbuf + 16;
		memcpy(wbuf + 22, buf + 23, 2);
		usrSex = wbuf + 22;
		memcpy(wbuf + 24, buf + 25, len - 25);
		usrLove = wbuf + 24;
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				//�����û��ϴ�������
				(*iter)->m_usrAge = usrAge;
				(*iter)->m_usrSex = usrSex;
				(*iter)->m_usrLove = usrLove;
				break;
			}
		}
		break;
	case MESSAGE_ALL:	//�յ����Ƿ��͸������˵�������Ϣ
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if (*iter != NULL)
			{
				(*iter)->Send(buf, len);		//ת����ÿ���ͻ���
			}
		}
		break;
	case MESSAGE_ONE:	//�յ����Ƿ��͸�ĳ���˵�������Ϣ
		memcpy(dstName, buf + 1, 16);
		memcpy(srcName, buf + 17, 16);
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == dstName)	//����Ŀ���û�
			{
				(*iter)->Send(buf, len);		//ת��
				break;
			}
		}
		break;
	case EXIT:		//�Ͽ����ӵĴ���

		tm = CTime::GetCurrentTime();
		log.Format(_T("��%s : %u����������Ͽ���"), clientSocket->m_userIP, clientSocket->m_userPort);
		log = tm.Format("%Y-%m-%d %X : ") + log;
		m_ctrlChatRoomInfo.AddString(log);
		m_clientCnt--;
		UpdateData(FALSE);
		clientSocket->Close();
		for (iter = clientList.begin(); iter != clientList.end();)
		{
			if (*iter == clientSocket)		//���ӶϿ���ɾ���׽���
			{
				iter = clientList.erase(iter);
			}
			else		//�������ͻ���ת�����ߵ���Ϣ
			{
				(*iter)->Send(buf, len);
				iter++;
			}
		}
		delete clientSocket;
		break;
	case UPFILE:
		/*CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("Text Files(*.txt)||"), AfxGetMainWnd());
		CString strPath, strLog;
		if (dlg.DoModal() == IDOK)
		{
			strPath = dlg.GetPathName();
			if (strPath.Right(4) != ".txt")
				strPath += ".txt";
		}*/
		CH_nameLength = NULL;
		IN_namelength= buf[1]-CH_nameLength;
		strPath.Empty();
		strPath += _T("�ļ�\\");

		memcpy(wbuf, buf + 2, IN_namelength);
		strPath += wbuf;

		temp_File_Name.Empty();
		temp_File_Name += buf[1];//ÿ���ļ����ĳ���
		temp_File_Name += wbuf;

		hFile = CreateFile(strPath, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
		DWORD nBytes;
		WriteFile(hFile, buf+2+IN_namelength, len-2-IN_namelength, &nBytes, 0);
		CloseHandle(hFile);

		filelist.push_back(temp_File_Name);
		break;
	case WATCH_FILELIST:
		memcpy(wbuf, buf + 1, 16);
		usrName = wbuf;
		temp_File_List.Empty();
		for (file_iter = filelist.begin(); file_iter != filelist.end();file_iter++)
		{
			temp_File_List +=*file_iter;
		}
		data_buf[0] = WATCH_FILELIST;

		memcpy(data_buf + 1, temp_File_List.GetBuffer(0), temp_File_List.GetLength() * 2+1);
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				(*iter)->Send(data_buf, temp_File_List.GetLength() * 2 + 1);
				break;
			}
		}
		break;
	case DOWNLOAD_FILE:
		CH_nameLength = NULL;
		File_num = buf[1] - CH_nameLength;

		memcpy(wbuf, buf + 2, 16);
		usrName = wbuf;

		strPath.Empty();
		strPath += _T("�ļ�\\");
		strPath += filelist[File_num].Mid(1);

		data_buf[0] = DOWNLOAD_FILE;
		//_splitpath((char*)(LPCTSTR)strPath, NULL, NULL, NULL, charFileLog);
		//File_num = strlen(charFileLog);
		//CH_nameLength = File_num;//�����׺�����ȣ�
		//data_buf[1] = CH_nameLength;
		memcpy(data_buf + 1, filelist[File_num], filelist[File_num].GetLength()*2);

		hFile = CreateFile(strPath, FILE_GENERIC_READ,            //  ���ļ�������ļ������
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,     //  ����ʽ�򿪣����������ط���Ҫ��д���ļ�
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		dwSize = GetFileSize(hFile, NULL);
		if (INVALID_HANDLE_VALUE == hFile)                              //  �ļ���ʧ�ܣ����ش���ֵ
			MessageBox(_T("���ļ�ʧ��"), _T("��ʾ"), MB_ICONWARNING);

		ReadFile(hFile, data_buf + 1+ filelist[File_num].GetLength() * 2, dwSize, &dwSize, NULL);
		CloseHandle(hFile);

		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				(*iter)->Send(data_buf, dwSize+1+ filelist[File_num].GetLength() * 2);//�ź�+��׺����+��׺��+�ļ�����
				break;
			}
		}
		break;
	default:
		break;
	}
}

void CServerDlg::OnBnClickedBtnExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Text Files(*.txt)||"),AfxGetMainWnd());
	CString strPath,strLog;
	if (dlg.DoModal() == IDOK)
	{
		strPath=dlg.GetPathName();
		if (strPath.Right(4)!=".txt")
			strPath+=".txt";
	}
	CFile file(strPath,CFile::modeCreate|CFile::modeWrite);
	for (int i = 0; i < m_ctrlChatRoomInfo.GetCount(); i++)
	{
		CString str;
		m_ctrlChatRoomInfo.GetText(i,str);
		strLog+=str;
		strLog+=_T("\r\n");
	}
	WORD unicode = 0xFEFF;  //�����Ҫ��ע��������F E FF���м��и�E
	file.Write(&unicode, 2);  //�����Ҫ
	//const unsigned char LeadBytes[] = { 0xEF, 0xBB, 0xBF };//UTF-8��bom
	//const unsigned char LeadBytes[] = { 0xFE, 0xFF };	
	//file.Write(LeadBytes, sizeof(LeadBytes));
	//int i = strLog.GetLength()*2;
	//wchar_t str[i] = { 0 };
	file.Write(strLog,strLog.GetLength()*2);
	file.Close();
	MessageBox(_T("�����ɹ���"),_T("��ʾ"),MB_ICONINFORMATION);
}
//��ȡ
void CServerDlg::OnGetUsersNP()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlUsers.ResetContent();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�ı��ĵ� *.txt|*.txt|"), AfxGetMainWnd());
	CString strPath, strLog;
	CString szLine;
	//unsigned char buf[MAX_BUFSIZE] = { 0 };
	if (dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetPathName();
	}
	CStdioFile  file(strPath, CFile::modeRead);
	
	/*char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//�趨<ctpye.h>���ַ�����ʽ*/
	/*BYTE head[3];
	file.Read(head, 3);
	if (!(head[0] == 0xEF && head[1] == 0xBB && head[2] == 0xBF))
	{
		file.SeekToBegin();
	}*/
	while(file.ReadString(szLine))
	{ 
		/*ULONGLONG FileSize = szLine.GetLength();
		char* pContent = (char*)calloc(FileSize + 1, sizeof(char));
		file.Read(pContent, FileSize);
		int n = MultiByteToWideChar(CP_UTF8, 0, pContent, FileSize + 1, NULL, 0);
		wchar_t* pWideChar = (wchar_t*)calloc(n + 1, sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, pContent, FileSize + 1, pWideChar, n);
		CString strFile;
		strFile = CString(pWideChar);
		free(pContent);
		free(pWideChar);*/
		m_ctrlUsers.AddString(szLine);
	}
	/*setlocale(LC_CTYPE, old_locale);
	free(old_locale);//��ԭ�����趨*/
	file.Close();

	MessageBox(_T("��ȡ�ɹ���"), _T("��ʾ"), MB_ICONINFORMATION);

}
