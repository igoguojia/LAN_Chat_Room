// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include"DataDlg.h"
#include <vector>
#include "CSelect_Files.h"

#include<iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<CString> filelist;//文件列表

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


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
	, m_usrName(_T(""))
	, m_serverIP(_T("127.0.0.1"))
	, m_serverPort(2537)
	, m_sendMsg(_T(""))
	, m_publicChannel(_T(""))
	, m_privateChannel(_T(""))
	, m_strPeopleToTalk(_T(""))
	, m_usrPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_clientSocket.m_pClientDlg = this;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_USERNAME, m_usrName);
	DDX_Text(pDX, IDC_SERVERPORT, m_serverPort);
	DDV_MinMaxUInt(pDX, m_serverPort, 1024, 65535);

	DDX_Control(pDX, IDC_SERVERIP, m_ctrlServerIP);

	DDX_Text(pDX, IDC_USERNAME, m_usrName);
	DDV_MaxChars(pDX, m_usrName, 8);

	DDX_Text(pDX, IDC_EDIT1, m_usrPassword);
	DDV_MaxChars(pDX, m_usrPassword, 15);

	DDX_Text(pDX, IDC_MESSAGE, m_sendMsg);
	DDV_MaxChars(pDX, m_sendMsg, 800);

	DDX_Control(pDX, IDC_MESSAGE, m_ctrlSendMsg);

	DDX_Control(pDX, IDC_MSGSTYTLE, m_ctrlMsgStyle);

	DDX_Control(pDX, IDC_SELECTUSER, m_ctrlPeopleToTalk);
	DDX_CBString(pDX, IDC_SELECTUSER, m_strPeopleToTalk);

	DDX_Text(pDX, IDC_PUBLICAREA, m_publicChannel);
	DDX_Text(pDX, IDC_PRIVATEAREA, m_privateChannel);

	DDX_Control(pDX, IDC_USERONLINE, m_ctrlUserOnline);



	DDX_Control(pDX, IDC_BLACKLIST, m_ctrlBlackName);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_SENDMSG, &CClientDlg::OnBnClickedSendmsg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CClientDlg::OnBnClickedBtnConnect)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OTHERDATA, &CClientDlg::OnBnClickedBtnOtherdata)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnLogoIn)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnRegister)
	ON_LBN_DBLCLK(IDC_USERONLINE, &CClientDlg::OnLbnDblclkUseronline)
	ON_BN_CLICKED(IDC_BTN_MYDATA, &CClientDlg::OnMydata)
	ON_BN_CLICKED(IDC_TOBLACKLIST, &CClientDlg::OnBnClickedToBlacklist)
	ON_LBN_DBLCLK(IDC_BLACKLIST, &CClientDlg::OnLbnDblclkBlacklist)

	ON_BN_CLICKED(IDC_RESTROE, &CClientDlg::OnBnClickedRestroe)
	ON_BN_CLICKED(IDC_BUTTON3, &CClientDlg::OnShowBackGround)
	ON_BN_CLICKED(IDC_BUTTON4, &CClientDlg::OnUPFILE)
	ON_BN_CLICKED(IDC_DOWNLOADFILE, &CClientDlg::OnDOWNLOADFILE)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
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
	//((CButton *)GetDlgItem(IDC_MALE))->SetCheck(TRUE);
	//GetDlgItem(IDC_USERNAME)->SetFocus();

	m_ctrlServerIP.SetWindowTextW(m_serverIP);

	m_editFont.CreatePointFont(120, _T("黑体"));
	m_ctrlSendMsg.SetFont(&m_editFont); // 设置新字体
	
	m_ctrlMsgStyle.AddString(_T("微笑着"));
	m_ctrlMsgStyle.AddString(_T("开心地"));
	m_ctrlMsgStyle.AddString(_T("大笑着"));
	m_ctrlMsgStyle.AddString(_T("愤怒地"));
	m_ctrlMsgStyle.AddString(_T("忧伤地"));
	m_ctrlMsgStyle.AddString(_T("大哭着"));
	m_ctrlMsgStyle.AddString(_T("兴高采烈地"));
	m_ctrlMsgStyle.AddString(_T("怒不可遏地"));
	m_ctrlMsgStyle.AddString(_T("伤心欲绝地"));
	m_ctrlMsgStyle.AddString(_T("喜笑颜开地"));
	m_ctrlMsgStyle.AddString(_T("可怜兮兮地"));
	m_ctrlMsgStyle.SetCurSel(0);

	m_ctrlPeopleToTalk.AddString(_T("所有人"));
	m_ctrlPeopleToTalk.SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
		CBitmap bmpBackground;   

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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CClientDlg::OnBnClickedSendmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("未连接到服务器！"),_T("错误"),MB_ICONERROR);
		return;
	}
	unsigned char buf[MAX_BUFSIZE]={0};
	CString strStyle;
	GetDlgItem(IDC_MSGSTYTLE)->GetWindowTextW(strStyle);
	if (m_ctrlPeopleToTalk.GetCurSel() == 0)	//选中了第0项，也就是发送给所有人
	{
		buf[0] = MESSAGE_ALL;	//公共消息
		
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);	//16字节用户名
		memcpy(buf+17,strStyle.GetBuffer(0),strStyle.GetLength()*2);	//10字节消息方式
		memcpy(buf+27,m_sendMsg.GetBuffer(0),m_sendMsg.GetLength()*2);	//消息主体
		m_clientSocket.Send(buf,m_sendMsg.GetLength()*2+27);	//发送消息
	}
	else
	{
		UpdateData();
		buf[0] = MESSAGE_ONE;	//私聊消息

		memcpy(buf+1,m_strPeopleToTalk.GetBuffer(0),m_strPeopleToTalk.GetLength()*2);	//16字节目的用户名
		memcpy(buf+17,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);	//16字节源用户名
		memcpy(buf+33,strStyle.GetBuffer(0),strStyle.GetLength()*2);	//10字节消息方式
		memcpy(buf+43,m_sendMsg.GetBuffer(0),m_sendMsg.GetLength()*2);	//消息主体
		m_clientSocket.Send(buf,m_sendMsg.GetLength()*2+43);	//发送消息

		m_privateChannel+=_T("【你】");
		m_privateChannel+=strStyle;
		m_privateChannel+=_T("* 对 【");
		m_privateChannel+=m_strPeopleToTalk;
		m_privateChannel+=_T("】 说：");
		m_privateChannel+=m_sendMsg;
		m_privateChannel+=_T("\r\n");
		UpdateData(FALSE);
	}
}


HBRUSH CClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int CtrlID=pWnd->GetDlgCtrlID();
	switch (CtrlID)
	{
	case IDC_PUBLICAREA:
	case IDC_PRIVATEAREA:
		pDC->SetBkColor(RGB(255, 255, 255));
		return ::CreateSolidBrush(RGB(255, 255, 255));
	case IDC_STATIC:
	//case IDC_MALE:
	//case IDC_FEMALE:
		pDC->SetBkMode(TRANSPARENT);
//e		pDC->SetTextColor(RGB(255,255,255));  //字体颜色
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}

//连接
void CClientDlg::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_BTN_CONNECT)->GetWindowTextW(str);
	if (str == "连接服务器")	//当前处于断开状态
	{
		UpdateData();
		GetDlgItem(IDC_SERVERIP)->GetWindowTextW(m_serverIP);

		/*if (m_usrName.IsEmpty())
		{
			MessageBox(_T("请输入用户名！"),_T("提示"),MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return ;
		}*/
		if (m_serverIP.IsEmpty())
		{
			MessageBox(_T("请输入服务器IP！"),_T("提示"),MB_ICONWARNING);
			GetDlgItem(IDC_SERVERIP)->SetFocus();
			return ;
		}
		if (!m_clientSocket.Create())	//建立Socket
		{
			MessageBox(_T("创建套接字失败！"),_T("错误"),MB_ICONERROR);
			return;
		}
		if (!m_clientSocket.Connect(m_serverIP,m_serverPort))
		{
			MessageBox(_T("连接服务器失败，请稍候重试！"),_T("错误"),MB_ICONERROR);
			m_clientSocket.Close();
			return;
		}
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("断开连接"));
		//GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//连接上服务器后不可更改用户名
		GetDlgItem(IDC_BUTTON4)->EnableWindow(true);	//连接后可上传文件
		//GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(true);	//连接后可上传文件



		//连接上服务器后将自己的资料信息发送到服务器
		/*unsigned char buf[MAX_BUFSIZE];
		buf[0]=NEWCLIENT;
		m_usrNP = m_usrName+ m_usrPassword;
		//m_usrNP +=m_usrPassword;
		memcpy(buf+1, m_usrNP.GetBuffer(0), m_usrNP.GetLength()*2);
		m_clientSocket.Send(buf, m_usrNP.GetLength()*2+1);*/
	}
	else		//当前处于连接状态
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = EXIT;	//断开连接
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
		m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);		//退出用户的用户名
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接服务器"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//断开后可以改用户名
		m_ctrlUserOnline.ResetContent();	//清空在线用户列表
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);	//断开后可再点击登录
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);	//断开后可以再点击注册
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);	//断开后不可上传文件
		GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(false);	//断开后不可上传文件

	}
}
//注册
void CClientDlg::OnRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("未连接到服务器！"), _T("错误"), MB_ICONERROR);
		return;
	}
	else
	{
		UpdateData();
		if (m_usrName.IsEmpty()||m_usrPassword.IsEmpty())
		{
			MessageBox(_T("请输入用户名/密码！"), _T("提示"), MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return;
		}
		if (m_usrName.GetLength()!=8)
		{
			MessageBox(_T("请输入八位用户名"), _T("提示"), MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return;
		}
		else 
		{
			unsigned char buf[MAX_BUFSIZE];
			buf[0] = REGISTER;
			m_usrNP = m_usrName + m_usrPassword;

			memcpy(buf + 1, m_usrNP.GetBuffer(0), m_usrNP.GetLength() * 2);
			m_clientSocket.Send(buf, m_usrNP.GetLength() * 2 + 1);
			GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//登陆后后不可更改用户名
		}
	}
}

//登录
void CClientDlg::OnLogoIn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("未连接到服务器！"), _T("错误"), MB_ICONERROR);
		return;
	}
	else 
	{
		UpdateData();
		if (m_usrName.IsEmpty()||m_usrPassword.IsEmpty())
		{
			MessageBox(_T("请输入用户名/密码！"), _T("提示"), MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return;
		}
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = NEWCLIENT;
		m_usrNP = m_usrName + m_usrPassword;
		//m_usrNP +=m_usrPassword;
		memcpy(buf + 1, m_usrNP.GetBuffer(0), m_usrNP.GetLength() * 2);
		m_clientSocket.Send(buf, m_usrNP.GetLength() * 2 + 1);
		GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//登陆后后不可更改用户名
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);	//登陆后后不可再点击登录
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);	//登陆后不可以再点击注册
		GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(true);	//连接后可上传文件

	}
}


void CClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDYES==MessageBox(_T("确认退出？"),_T("提示"),MB_ICONQUESTION|MB_YESNO))
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = EXIT;	//断开连接
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
		m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);
		m_clientSocket.Close();
		CDialogEx::OnClose();
	}
}
void CClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	unsigned char buf[MAX_BUFSIZE];
	buf[0] = EXIT;	//断开连接
	memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
	m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);
	m_clientSocket.Close();
	// TODO: 在此处添加消息处理程序代码
}
void CClientDlg::ProcessRecvData()
{
	CString str;
	unsigned char buf[MAX_BUFSIZE]={0};
	int len = m_clientSocket.Receive(buf,MAX_BUFSIZE);
	wchar_t msg[MAX_BUFSIZE]={0};//消息内容
	wchar_t style[MAX_BUFSIZE]={0};//怎么说
	wchar_t usrName[MAX_BUFSIZE]={0};//用户名字
	wchar_t srcName[16] = {0};

	//接收文件列表
	char CH_nameLength = NULL;
	int IN_namelength = 0;
	CString temp_File_Name;//临时文件名string
	CString temp_File_List;

	
	srand((unsigned int)time(NULL));
	if (NEWCLIENT == buf[0])		//新用户上线消息
	{
		memcpy(usrName, buf + 1, len-1);		//用户名

		if (LB_ERR == m_ctrlUserOnline.FindString(-1,usrName))		//在线用户增加
		{
			m_ctrlUserOnline.AddString(usrName);
		}
		if (LB_ERR == m_ctrlPeopleToTalk.FindString(-1,usrName))	//可以私聊的人增加
		{
			m_ctrlPeopleToTalk.AddString(usrName);
		}
		str.Empty();
		str+=_T("\t\t【");
		str+=usrName;
		str+=_T("】");
		str+=enterStyle[rand()%MAX_ENTERSTYLE];
		str+=_T("聊天室，大家欢迎！\r\n");
		m_publicChannel+=str;
	}
	else if (ONLINECLIENT == buf[0])		//当前在线用户信息
	{
		memcpy(usrName,buf+1,len-1);		//用户名
		if (LB_ERR == m_ctrlUserOnline.FindString(-1,usrName))
		{
			m_ctrlUserOnline.AddString(usrName);
		}
		if (LB_ERR == m_ctrlPeopleToTalk.FindString(-1,usrName))
		{
			m_ctrlPeopleToTalk.AddString(usrName);
		}
	}
	else if (MESSAGE_ALL == buf[0])		//公共频道的消息
	{
		str.Empty();
		memcpy(usrName,buf+1,16);		//前十六个字节存用户名
		memcpy(style,buf+17,10);		//十个字节的消息方式
		memcpy(msg,buf+27,len-27);		//消息内容
		str+=_T("【");
		str+=usrName;
		str+=_T("】 *");
		str+=style;
		str+=_T("* 对 【所有人】 说：");
		str+=msg;
		m_publicChannel+=str;
		m_publicChannel+=_T("\r\n");
	}
	else if (MESSAGE_ONE == buf[0])		//私聊频道的消息
	{
		wchar_t srcUsrName[16]={0},dstUsrName[16]={0};
		memcpy(dstUsrName,buf+1,16);		//目的用户名
		memcpy(srcUsrName,buf+17,16);		//源用户名
		for (int i = 0; i < m_ctrlBlackName.GetCount(); i++)
		{
			CString str;
			m_ctrlBlackName.GetText(i, str);
			if (srcUsrName == str)	return;
		}
		memcpy(style,buf+33,10);			//消息方式
		memcpy(msg,buf+43,len-43);		//消息内容
		str.Empty();
		if (m_usrName == dstUsrName)		//别人发给你的消息
		{
			str+=_T("【");
			str+=srcUsrName;
			str+=_T("】 *");
			str+=style;
			str+=_T("* 对 【你】 说：");
			str+=msg;
			m_privateChannel+=str;
			m_privateChannel+=_T("\r\n");
		}
	}
	else if (EXIT == buf[0])		//离开的信息
	{
		memcpy(usrName,buf+1,len-1);		//用户名
		m_ctrlUserOnline.AddString(usrName);
		str.Empty();
		str+=_T("\t\t【");
		str+=usrName;
		str+=_T("】");
		str+=leaveStyle[rand()%MAX_ENTERSTYLE];
		str+=_T("聊天室！\r\n");
		m_publicChannel+=str;

		int n=m_ctrlUserOnline.FindString(-1,usrName);
		if (LB_ERR != n)
		{
			m_ctrlUserOnline.DeleteString((UINT)n);		//这里必须写两句删除才能删除
			m_ctrlUserOnline.DeleteString((UINT)n);
		}
		n=m_ctrlPeopleToTalk.FindString(-1,usrName);
		if (LB_ERR != n)
		{
			m_ctrlPeopleToTalk.DeleteString((UINT)n);
		}
	}
	else if (SERVERCLOSE == buf[0])		//服务器关闭消息
	{
		MessageBox(_T("服务器已关闭！"),_T("提示"),MB_ICONINFORMATION);
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接服务器"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//断开后可以改用户名
		m_ctrlUserOnline.ResetContent();
	}
	else if (REPEATNAME == buf[0])
	{
		MessageBox(_T("用户名已存在，请重试！"),_T("提示"),MB_ICONINFORMATION);
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接服务器"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//断开后可以改用户名
		m_ctrlUserOnline.ResetContent();
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	//可再点击登录
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);	//可以再点击注册
		GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(false);	//不可上传文件

	}
	else if (ERROR == buf[0])
	{
	MessageBox(_T("用户名或密码不正确，请重试！"), _T("提示"), MB_ICONINFORMATION);
	m_clientSocket.Close();
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接服务器"));
	GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//断开后可以改用户名
	m_ctrlUserOnline.ResetContent();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	//可再点击登录
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);	//可以再点击注册
	GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(false);	//断开后不可上传文件

	}
	else if (HADDEFINED == buf[0])
	{
	MessageBox(_T("用户名已被注册！"), _T("提示"), MB_ICONINFORMATION);
	m_clientSocket.Close();
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接服务器"));
	GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//断开后可以改用户名
	m_ctrlUserOnline.ResetContent();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	//可再点击登录
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);	//可以再点击注册
	}
	else if (REGISTER == buf[0])
	{
	MessageBox(_T("注册成功！"), _T("提示"), MB_ICONINFORMATION);
	m_clientSocket.Close();
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接服务器"));
	GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//断开后可以改用户名
	m_ctrlUserOnline.ResetContent();
	}
	else if(WDATA==buf[0])
	{
		wchar_t otherAge[6] = { 0 }, otherSex[2] = { 0 }, otherLove[40] = { 0 };
		memcpy(srcName, buf + 1, 16);
		memcpy(otherAge, buf + 17, 6);
		memcpy(otherSex, buf + 23, 2);
		memcpy(otherLove, buf + 25, len-25);
		if (m_usrName == srcName)
		{
			ot_dataAge = otherAge;
			ot_dataSex = otherSex;
			ot_dataLove = otherLove;
			m_judgeSelf = false;
			DataDlg datadlg = new DataDlg();
			datadlg.DoModal();
		}
	
	}
	else if (WATCH_FILELIST == buf[0])
	{
		
		for (int i = 1; buf[i] != 0; i+=IN_namelength+2)
		{
			CH_nameLength = NULL;
			IN_namelength = buf[i] - CH_nameLength;
			wchar_t wbuf[100] = { 0 };
			memcpy(wbuf, buf + 1+i+1,IN_namelength);
			temp_File_Name = wbuf;
			filelist.push_back(temp_File_Name);
		}
		CSelect_Files select_file = new CSelect_Files;
		if (IDOK == select_file.DoModal())
		{
			num = select_file.CSelect_num;
			Select_File_OK = select_file.CSelect_Select_File_OK;
			filelist.clear();
		}
		else
			filelist.clear();
		if (Select_File_OK)
		{
			DownLoad();
		}

	}
	else if (DOWNLOAD_FILE == buf[0])
	{
		
		CString strPath, strName;
		CH_nameLength = NULL;
		IN_namelength = buf[1] - CH_nameLength;//文件名长度
		wchar_t wbuf[100] = { 0 };

		memcpy(wbuf, buf + 3, IN_namelength);
		strName = wbuf;
		CFileDialog dlg(FALSE, NULL, strName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("所有文件(*.*)||"), AfxGetMainWnd());
		if (dlg.DoModal() == IDOK)
		{
			strPath = dlg.GetPathName();
		}
		//strPath += strName;
		HANDLE hFile = CreateFile(strPath, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
		DWORD nBytes;
		WriteFile(hFile, buf + 3+ IN_namelength, len -1, &nBytes, 0);
		CloseHandle(hFile);
	}
	/*else if(UpDATA==buf[0])
	{
	//Sleep(100);
		wchar_t otherAge[6] = { 0 }, otherSex[2] = { 0 }, otherLove[40] = { 0 };
		memcpy(otherAge, buf + 17, 6);
		memcpy(otherSex, buf+ 23, 2);
		memcpy(otherLove, buf + 25, len-25);
		ot_dataAge = otherAge;
		ot_dataSex = otherSex;
		ot_dataLove = otherLove;
		m_judgeSelf = false;
		DataDlg datadlg = new DataDlg();
		datadlg.DoModal();

	}*/
	UpdateData(FALSE);

}



//查看他人资料
void CClientDlg::OnBnClickedBtnOtherdata()
{
	// TODO: 在此添加控件通知处理程序代码
	if (LbnDblclkUseronline)
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = WDATA;
		memcpy(buf + 1, ot_dataName.GetBuffer(0), ot_dataName.GetLength() * 2);//16字节所看资料用户名
		memcpy(buf + 17, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);//16字节请求用户名
		m_clientSocket.Send(buf,33);
	}
	else
		MessageBox(_T("未选择用户，请双击选择！"), _T("提示"), MB_ICONWARNING);

}

//双击listbox某一行的响应函数
void CClientDlg::OnLbnDblclkUseronline()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlUserOnline.GetText(m_ctrlUserOnline.GetCurSel(), ot_dataName);//得到指定行内容，给text
	LbnDblclkUseronline = true;
}

//查看个人资料
void CClientDlg::OnMydata()
{
	// TODO: 在此添加控件通知处理程序代码
	DataDlg datadlg = new DataDlg();
	ot_dataName = m_usrName;
	m_judgeSelf = true;
	if (IDOK == datadlg.DoModal())
	{
		m_dataAge = datadlg.Data_dataAge;
		m_dataLove = datadlg.Data_dataLove;
		m_dataSex = datadlg.Data_dataSex;
	}
	unsigned char data_buf[MAX_BUFSIZE] = { 0 };
	data_buf[0] = UpDATA;	//发送资料
	memcpy(data_buf + 1, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);	//16字节上传资料的用户名	
	memcpy(data_buf + 17, m_dataAge.GetBuffer(0), m_dataAge.GetLength() * 2);	//6字年龄		
	memcpy(data_buf + 23, m_dataSex.GetBuffer(0), m_dataSex.GetLength() * 2);	//2字性别
	memcpy(data_buf + 25, m_dataLove.GetBuffer(0), m_dataLove.GetLength() * 2);	//40字节爱好
	m_clientSocket.Send(data_buf, m_dataLove.GetLength() * 2 + 25);	//发送消息

}

//拉黑
void CClientDlg::OnBnClickedToBlacklist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BlackName = ot_dataName;

	if (LbnDblclkUseronline&&m_BlackName!=m_usrName)
	{
		/*unsigned char buf[MAX_BUFSIZE];
		buf[0] = BLACK;
		memcpy(buf + 1, m_BlackName.GetBuffer(0), m_BlackName.GetLength() * 2);//16字节所看资料用户名
		m_clientSocket.Send(buf, 17);*/

		m_ctrlBlackName.AddString(m_BlackName);
		int n = m_ctrlPeopleToTalk.FindString(-1, m_BlackName);
		if (LB_ERR != n)
		{
			m_ctrlPeopleToTalk.DeleteString((UINT)n);
		}
	}
	else if(!LbnDblclkUseronline)
		MessageBox(_T("未选择用户，请双击选择！"), _T("提示"), MB_ICONWARNING);
	else if (m_BlackName==m_usrName)
		MessageBox(_T("不能拉黑自己！"), _T("提示"), MB_ICONWARNING);
	/*if (LbnDblclkUseronline)
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = BLACK;
		memcpy(buf + 1, m_BalckName.GetBuffer(0), m_BalckName.GetLength() * 2);//16字节所看资料用户名
		m_clientSocket.Send(buf, 17);
		m_ctrlBlackName.AddString(m_BalckName);
	}
	else 
		MessageBox(_T("未选择用户，请双击选择！"), _T("提示"), MB_ICONWARNING);*/
	LbnDblclkUseronline = false;

}

void CClientDlg::OnLbnDblclkBlacklist()
{
	// TODO: 在此添加控件通知处理程序代码
	i = m_ctrlBlackName.GetCurSel();
	m_ctrlBlackName.GetText(i, m_BlackName);//得到指定行内容，给text
	DblcBlacklist = true;
}
//移出黑名单
void CClientDlg::OnBnClickedRestroe()
{
	// TODO: 在此添加控件通知处理程序代码
	if (DblcBlacklist)
	{
		m_ctrlBlackName.DeleteString(i);		
		m_ctrlPeopleToTalk.AddString(m_BlackName);
		m_ctrlBlackName.SetRedraw(FALSE);      //更新内容
	}
	else
		MessageBox(_T("未选择用户，请双击选择！"), _T("提示"), MB_ICONWARNING);
	DblcBlacklist = false;
}


void CClientDlg::OnShowBackGround()
{
	// TODO: 在此添加控件通知处理程序代码
	/*GetDlgItem(IDC_USERONLINE)->ShowWindow(false);
	GetDlgItem(IDC_PUBLICAREA)->ShowWindow(false);
	GetDlgItem(IDC_PUBLICAREA)->ShowWindow(false);

	GetDlgItem(IDC_PUBLICAREA)->ShowWindow(false);
	GetDlgItem(IDC_PUBLICAREA)->ShowWindow(false);
	GetDlgItem(IDC_PUBLICAREA)->ShowWindow(false);*/
	CBackGround back = new CBackGround;
	back.DoModal();


}


void CClientDlg::OnUPFILE()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char file_buf[MAX_BUFSIZE] = { 0 };
	file_buf[0] = UPFILE;	//发送文件
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文本文档 *.txt|*.txt|图片 *.png|*.png|"), AfxGetMainWnd());
	CString strPath, strLog,strName;
	unsigned int strName_length = 0;
	if (dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetPathName();
		strName = dlg.GetFileName();
		strName_length = strName.GetLength()*2;
	}
	char chName_length = strName_length;
	file_buf[1] = chName_length;
	memcpy(file_buf + 2, strName, strName_length);

	HANDLE hFile = CreateFile(strPath, FILE_GENERIC_READ,            //  打开文件，获得文件读句柄
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,     //  共享方式打开，避免其他地方需要读写此文件
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
	DWORD dwSize = GetFileSize(hFile, NULL);

	if (INVALID_HANDLE_VALUE == hFile)                              //  文件打开失败，返回错误值
		MessageBox(_T("打开文件失败"), _T("提示"), MB_ICONWARNING);

	ReadFile(hFile, file_buf+2+strName_length, dwSize, &dwSize, NULL);
	if (dwSize == GetFileSize(hFile, NULL))
	{
		CloseHandle(hFile);
		AfxMessageBox(_T("发送完成"));
	}
	m_clientSocket.Send(file_buf, dwSize+2+strName_length);	//发送消息
}


void CClientDlg::OnDOWNLOADFILE()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char down_file_buf[MAX_BUFSIZE] = { 0 };
	down_file_buf[0] = WATCH_FILELIST;	//发送资料
	memcpy(down_file_buf + 1, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);	//16字节上传资料的用户名	
	m_clientSocket.Send(down_file_buf, m_usrName.GetLength() * 2 + 1);	//发送消息
}

void CClientDlg::DownLoad()
{
	unsigned char down_file_buf[MAX_BUFSIZE] = { 0 };
	down_file_buf[0] = DOWNLOAD_FILE;	//发送资料
	down_file_buf[1] = num;
	memcpy(down_file_buf + 2, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);	//16字节上传资料的用户名	
	m_clientSocket.Send(down_file_buf, m_usrName.GetLength() * 2 + 2);	//发送消息
}
