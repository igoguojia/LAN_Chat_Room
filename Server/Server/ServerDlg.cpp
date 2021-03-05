
// ServerDlg.cpp : 实现文件
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

vector<CClientSocket*> clientList;//客户端列表
vector<CString >filelist;//文件列表

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, m_clientCnt(0)
	, m_listenPort(2537)
	, m_maxClient(50)
	, m_srvrName(_T(""))
	, m_srvrStatus(_T("已停止"))
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


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (!AfxSocketInit())
	{
	   AfxMessageBox(_T("Failed to Initialize Sockets"), MB_OK | MB_ICONSTOP);
	   exit(1);
	}
	char hostname[256];
	PHOSTENT hostinfo;
	CString IP;
	gethostname(hostname,sizeof(hostname));		//获取主机名
	hostinfo=gethostbyname(hostname);			//根据主机名获取主机IP

	m_srvrName=hostname;	//设置服务器名称
	IP=inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
	m_ctrlServerIP.SetWindowTextW(IP);	//设置服务器IP
	
	m_ctrlMaxCnt.AddString(_T("50"));
	m_ctrlMaxCnt.AddString(_T("40"));
	m_ctrlMaxCnt.AddString(_T("30"));
	m_ctrlMaxCnt.AddString(_T("20"));
	m_ctrlMaxCnt.AddString(_T("10"));
	m_ctrlMaxCnt.AddString(_T("5"));
	m_ctrlMaxCnt.SetCurSel(0);
	CString folderPath = _T("文件\\");
	if (!PathIsDirectory(folderPath))  // 是否有重名文件夹
	{
		::CreateDirectory(folderPath, 0);
	}
	m_serverSocket.m_pSerDlg=this;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
}


void CServerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(IDYES==MessageBox(_T("确认关闭服务器？"),_T("提示"),MB_ICONQUESTION|MB_YESNO))
	{
		CDialogEx::OnCancel();
	}*/
	CDialogEx::OnCancel();

}


void CServerDlg::OnBnClickedBtnStartStop()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_srvrStatus=="已停止")	//状态为已停止，则要开启服务器监听端口
	{
		UpdateData();
		WCHAR str[3];
		m_ctrlMaxCnt.GetWindowTextW(str,3);//将最大客户端数 那个下拉窗口当前的数，拷贝到str中
		m_maxClient=_ttoi(str);//将str从String转换到int
		if (m_listenPort<1024)
		{
			MessageBox(_T("请选择大于1024的端口！"),_T("提示"),MB_ICONWARNING);
			return;
		}
		if (!m_serverSocket.Create(m_listenPort))
		{
			MessageBox(_T("创建套接字出错！"),_T("错误"),MB_ICONERROR);
			return;
		}
		if (!m_serverSocket.Listen())
		{
			MessageBox(_T("监听失败！"),_T("错误"),MB_ICONERROR);
			return;
		}
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("停止"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(FALSE);
	}
	else		//状态为正在监听，要停止监听
	{
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("启动"));
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
		m_serverSocket.Close();//关闭服务端套接字
	}

	m_srvrStatus=m_srvrStatus=="正在监听……"?"已停止":"正在监听……";
	GetDlgItem(IDC_STATUS)->SetWindowTextW(m_srvrStatus);
	UpdateData(FALSE);
}



void CServerDlg::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
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
		pDC->SetTextColor(RGB(0,0,0));  //字体颜色
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}


void CServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	vector<CString>::iterator file_iter;//文件列表
	CString log, usrName, usrNP, usrLove, usrAge, usrSex;
	CTime tm;
	bool undefine = true;//判断是否注册或是密码正确
	CString data_c_srcName;
	unsigned char data_buf[MAX_BUFSIZE] = { 0 };

	HANDLE hFile;//文件句柄	
	CString strPath;
	char CH_nameLength = NULL;
	int IN_namelength = 0;
	CString temp_File_Name;//临时文件名string
	CString temp_File_List;

	DWORD dwSize=0;//文件长度
	unsigned int strName_length = 0;
	char chName_length;
	int File_num = 0;//要下载的文件的位置
	char charFileLog[10] = { 0 };//后缀名
	switch (buf[0])
	{
	case NEWCLIENT:		//收到的是新用户上线消息，转发给所有人，并给新用户发送全部在线用户信息
		memcpy(wbuf, buf + 1, 16);		//获取用户名		
		usrName = wbuf;
		memcpy(wbuf, buf + 1, len - 1);		//获取用户名和密码
		usrNP = wbuf;
		clientSocket->m_userName = usrName;		//设置用户名
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
			log.Format(_T("【%s : %u】与服务器断开。"), clientSocket->m_userIP, clientSocket->m_userPort);
			log = tm.Format("%Y-%m-%d %X : ") + log;
			m_ctrlChatRoomInfo.AddString(log);

			buf[0] = ERROR;
			clientSocket->Send(buf, 1);		//用户未注册或密码不正确
			clientSocket->Close();
			delete clientSocket;
			clientList.pop_back();
			m_clientCnt--;
			return;
		}
		//clientSocket->m_userName = usrName;		//设置用户名
		/*for (iter = clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				buf[0] = REPEATNAME;
				clientSocket->Send(buf, 1);		//用户名重复
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
				(*iter)->Send(buf, 17);		//转发到每个客户端
				//前十七个字节时是 一个字节标志和8个宽字符
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
				clientSocket->Send(buf, (*iter)->m_userName.GetLength() * 2 + 1); //发送在线用户的信息给新上线的用户
				Sleep(100);
			}
		}
		break;
	case REGISTER://注册
		memcpy(wbuf, buf + 1, 16);		//获取用户名
		usrName = wbuf;
		memcpy(wbuf, buf + 1, len - 1);		//获取用户名和密码
		usrNP = wbuf;
		

		clientSocket->m_usrNP = usrNP;		//设置用户名和密码
		clientSocket->m_userName = usrName;		//设置用户名
	/*	for (iter = clientList.begin(); iter < clientList.end()-1; iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				buf[0] = REPEATNAME;
				clientSocket->Send(buf, 1);		//用户名重复
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
				clientSocket->Send(buf, 1);		//用户已注册过
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
			log.Format(_T("用户："), clientSocket->m_userName);
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
			WORD unicode = 0xFEFF;  //这句重要，注意这里是F E FF，中间有个E
			file.Write(&unicode, 2);  //这句重要
			file.Write(strLog, strLog.GetLength() * 2);
			file.Close();

			tm = CTime::GetCurrentTime();
			log.Format(_T("【%s : %u】与服务器断开。"), clientSocket->m_userIP, clientSocket->m_userPort);
			log = tm.Format("%Y-%m-%d %X : ") + log;
			m_ctrlChatRoomInfo.AddString(log);

			buf[0] = REGISTER;
			clientSocket->Send(buf, 1);		//注册用户
			clientSocket->Close();
			delete clientSocket;
			clientList.pop_back();
			m_clientCnt--;

			UpdateData(false);

			return;

		}
		break;
		//clientSocket->m_userName = usrName;		//设置用户名
		/*for (iter = clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				buf[0] = REPEATNAME;
				clientSocket->Send(buf, 1);		//用户名重复
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}*/
	case WDATA://发送他人资料
		memcpy(dstName, buf + 1, 16);//被看的
		memcpy(srcName, buf + 17, 16);//要看的
		data_c_srcName = srcName;
		data_buf[0] = WDATA;
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == dstName)	//被看的用户
			{
				//发送资料
				usrAge = (*iter)->m_usrAge;
				usrSex = (*iter)->m_usrSex;
				usrLove = (*iter)->m_usrLove;
				break;
			}
		}
		memcpy(data_buf + 1, data_c_srcName.GetBuffer(0), data_c_srcName.GetLength() * 2);//16字节要看资料的用户名
		memcpy(data_buf + 17, usrAge.GetBuffer(0), usrAge.GetLength() * 2);//6字节年龄
		memcpy(data_buf + 23, usrSex.GetBuffer(0), usrSex.GetLength() * 2);//2字节性别
		memcpy(data_buf + 25, usrLove.GetBuffer(0), usrLove.GetLength() * 2);//爱好
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == srcName)	//被看的用户
			{
				//发送资料
				(*iter)->Send(data_buf, usrLove.GetLength() * 2 + 25);	//发送消息
				break;
			}
		}
		break;

	case UpDATA://上传资料
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
				//保存用户上传的资料
				(*iter)->m_usrAge = usrAge;
				(*iter)->m_usrSex = usrSex;
				(*iter)->m_usrLove = usrLove;
				break;
			}
		}
		break;
	case MESSAGE_ALL:	//收到的是发送给所有人的聊天消息
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if (*iter != NULL)
			{
				(*iter)->Send(buf, len);		//转发到每个客户端
			}
		}
		break;
	case MESSAGE_ONE:	//收到的是发送给某个人的聊天消息
		memcpy(dstName, buf + 1, 16);
		memcpy(srcName, buf + 17, 16);
		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == dstName)	//发给目的用户
			{
				(*iter)->Send(buf, len);		//转发
				break;
			}
		}
		break;
	case EXIT:		//断开连接的代码

		tm = CTime::GetCurrentTime();
		log.Format(_T("【%s : %u】与服务器断开。"), clientSocket->m_userIP, clientSocket->m_userPort);
		log = tm.Format("%Y-%m-%d %X : ") + log;
		m_ctrlChatRoomInfo.AddString(log);
		m_clientCnt--;
		UpdateData(FALSE);
		clientSocket->Close();
		for (iter = clientList.begin(); iter != clientList.end();)
		{
			if (*iter == clientSocket)		//连接断开，删除套接字
			{
				iter = clientList.erase(iter);
			}
			else		//给其他客户端转发下线的消息
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
		strPath += _T("文件\\");

		memcpy(wbuf, buf + 2, IN_namelength);
		strPath += wbuf;

		temp_File_Name.Empty();
		temp_File_Name += buf[1];//每个文件名的长度
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
		strPath += _T("文件\\");
		strPath += filelist[File_num].Mid(1);

		data_buf[0] = DOWNLOAD_FILE;
		//_splitpath((char*)(LPCTSTR)strPath, NULL, NULL, NULL, charFileLog);
		//File_num = strlen(charFileLog);
		//CH_nameLength = File_num;//传输后缀名长度；
		//data_buf[1] = CH_nameLength;
		memcpy(data_buf + 1, filelist[File_num], filelist[File_num].GetLength()*2);

		hFile = CreateFile(strPath, FILE_GENERIC_READ,            //  打开文件，获得文件读句柄
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,     //  共享方式打开，避免其他地方需要读写此文件
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		dwSize = GetFileSize(hFile, NULL);
		if (INVALID_HANDLE_VALUE == hFile)                              //  文件打开失败，返回错误值
			MessageBox(_T("打开文件失败"), _T("提示"), MB_ICONWARNING);

		ReadFile(hFile, data_buf + 1+ filelist[File_num].GetLength() * 2, dwSize, &dwSize, NULL);
		CloseHandle(hFile);

		for (iter = clientList.begin(); iter < clientList.end(); iter++)
		{
			if ((*iter)->m_userName == usrName)
			{
				(*iter)->Send(data_buf, dwSize+1+ filelist[File_num].GetLength() * 2);//信号+后缀长度+后缀名+文件长度
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
	// TODO: 在此添加控件通知处理程序代码
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
	WORD unicode = 0xFEFF;  //这句重要，注意这里是F E FF，中间有个E
	file.Write(&unicode, 2);  //这句重要
	//const unsigned char LeadBytes[] = { 0xEF, 0xBB, 0xBF };//UTF-8带bom
	//const unsigned char LeadBytes[] = { 0xFE, 0xFF };	
	//file.Write(LeadBytes, sizeof(LeadBytes));
	//int i = strLog.GetLength()*2;
	//wchar_t str[i] = { 0 };
	file.Write(strLog,strLog.GetLength()*2);
	file.Close();
	MessageBox(_T("导出成功！"),_T("提示"),MB_ICONINFORMATION);
}
//读取
void CServerDlg::OnGetUsersNP()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlUsers.ResetContent();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文本文档 *.txt|*.txt|"), AfxGetMainWnd());
	CString strPath, strLog;
	CString szLine;
	//unsigned char buf[MAX_BUFSIZE] = { 0 };
	if (dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetPathName();
	}
	CStdioFile  file(strPath, CFile::modeRead);
	
	/*char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定<ctpye.h>中字符处理方式*/
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
	free(old_locale);//还原区域设定*/
	file.Close();

	MessageBox(_T("读取成功！"), _T("提示"), MB_ICONINFORMATION);

}
