// ClientDlg.cpp : ʵ���ļ�
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

vector<CString> filelist;//�ļ��б�

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


// CClientDlg �Ի���



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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
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
	//((CButton *)GetDlgItem(IDC_MALE))->SetCheck(TRUE);
	//GetDlgItem(IDC_USERNAME)->SetFocus();

	m_ctrlServerIP.SetWindowTextW(m_serverIP);

	m_editFont.CreatePointFont(120, _T("����"));
	m_ctrlSendMsg.SetFont(&m_editFont); // ����������
	
	m_ctrlMsgStyle.AddString(_T("΢Ц��"));
	m_ctrlMsgStyle.AddString(_T("���ĵ�"));
	m_ctrlMsgStyle.AddString(_T("��Ц��"));
	m_ctrlMsgStyle.AddString(_T("��ŭ��"));
	m_ctrlMsgStyle.AddString(_T("���˵�"));
	m_ctrlMsgStyle.AddString(_T("�����"));
	m_ctrlMsgStyle.AddString(_T("�˸߲��ҵ�"));
	m_ctrlMsgStyle.AddString(_T("ŭ���ɶ���"));
	m_ctrlMsgStyle.AddString(_T("����������"));
	m_ctrlMsgStyle.AddString(_T("ϲЦ�տ���"));
	m_ctrlMsgStyle.AddString(_T("���������"));
	m_ctrlMsgStyle.SetCurSel(0);

	m_ctrlPeopleToTalk.AddString(_T("������"));
	m_ctrlPeopleToTalk.SetCurSel(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
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
		CBitmap bmpBackground;   

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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CClientDlg::OnBnClickedSendmsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("δ���ӵ���������"),_T("����"),MB_ICONERROR);
		return;
	}
	unsigned char buf[MAX_BUFSIZE]={0};
	CString strStyle;
	GetDlgItem(IDC_MSGSTYTLE)->GetWindowTextW(strStyle);
	if (m_ctrlPeopleToTalk.GetCurSel() == 0)	//ѡ���˵�0�Ҳ���Ƿ��͸�������
	{
		buf[0] = MESSAGE_ALL;	//������Ϣ
		
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);	//16�ֽ��û���
		memcpy(buf+17,strStyle.GetBuffer(0),strStyle.GetLength()*2);	//10�ֽ���Ϣ��ʽ
		memcpy(buf+27,m_sendMsg.GetBuffer(0),m_sendMsg.GetLength()*2);	//��Ϣ����
		m_clientSocket.Send(buf,m_sendMsg.GetLength()*2+27);	//������Ϣ
	}
	else
	{
		UpdateData();
		buf[0] = MESSAGE_ONE;	//˽����Ϣ

		memcpy(buf+1,m_strPeopleToTalk.GetBuffer(0),m_strPeopleToTalk.GetLength()*2);	//16�ֽ�Ŀ���û���
		memcpy(buf+17,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);	//16�ֽ�Դ�û���
		memcpy(buf+33,strStyle.GetBuffer(0),strStyle.GetLength()*2);	//10�ֽ���Ϣ��ʽ
		memcpy(buf+43,m_sendMsg.GetBuffer(0),m_sendMsg.GetLength()*2);	//��Ϣ����
		m_clientSocket.Send(buf,m_sendMsg.GetLength()*2+43);	//������Ϣ

		m_privateChannel+=_T("���㡿");
		m_privateChannel+=strStyle;
		m_privateChannel+=_T("* �� ��");
		m_privateChannel+=m_strPeopleToTalk;
		m_privateChannel+=_T("�� ˵��");
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
//e		pDC->SetTextColor(RGB(255,255,255));  //������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}

//����
void CClientDlg::OnBnClickedBtnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_BTN_CONNECT)->GetWindowTextW(str);
	if (str == "���ӷ�����")	//��ǰ���ڶϿ�״̬
	{
		UpdateData();
		GetDlgItem(IDC_SERVERIP)->GetWindowTextW(m_serverIP);

		/*if (m_usrName.IsEmpty())
		{
			MessageBox(_T("�������û�����"),_T("��ʾ"),MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return ;
		}*/
		if (m_serverIP.IsEmpty())
		{
			MessageBox(_T("�����������IP��"),_T("��ʾ"),MB_ICONWARNING);
			GetDlgItem(IDC_SERVERIP)->SetFocus();
			return ;
		}
		if (!m_clientSocket.Create())	//����Socket
		{
			MessageBox(_T("�����׽���ʧ�ܣ�"),_T("����"),MB_ICONERROR);
			return;
		}
		if (!m_clientSocket.Connect(m_serverIP,m_serverPort))
		{
			MessageBox(_T("���ӷ�����ʧ�ܣ����Ժ����ԣ�"),_T("����"),MB_ICONERROR);
			m_clientSocket.Close();
			return;
		}
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("�Ͽ�����"));
		//GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//�����Ϸ������󲻿ɸ����û���
		GetDlgItem(IDC_BUTTON4)->EnableWindow(true);	//���Ӻ���ϴ��ļ�
		//GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(true);	//���Ӻ���ϴ��ļ�



		//�����Ϸ��������Լ���������Ϣ���͵�������
		/*unsigned char buf[MAX_BUFSIZE];
		buf[0]=NEWCLIENT;
		m_usrNP = m_usrName+ m_usrPassword;
		//m_usrNP +=m_usrPassword;
		memcpy(buf+1, m_usrNP.GetBuffer(0), m_usrNP.GetLength()*2);
		m_clientSocket.Send(buf, m_usrNP.GetLength()*2+1);*/
	}
	else		//��ǰ��������״̬
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = EXIT;	//�Ͽ�����
		memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
		m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);		//�˳��û����û���
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
		m_ctrlUserOnline.ResetContent();	//��������û��б�
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);	//�Ͽ�����ٵ����¼
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);	//�Ͽ�������ٵ��ע��
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);	//�Ͽ��󲻿��ϴ��ļ�
		GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(false);	//�Ͽ��󲻿��ϴ��ļ�

	}
}
//ע��
void CClientDlg::OnRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("δ���ӵ���������"), _T("����"), MB_ICONERROR);
		return;
	}
	else
	{
		UpdateData();
		if (m_usrName.IsEmpty()||m_usrPassword.IsEmpty())
		{
			MessageBox(_T("�������û���/���룡"), _T("��ʾ"), MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return;
		}
		if (m_usrName.GetLength()!=8)
		{
			MessageBox(_T("�������λ�û���"), _T("��ʾ"), MB_ICONWARNING);
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
			GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//��½��󲻿ɸ����û���
		}
	}
}

//��¼
void CClientDlg::OnLogoIn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_clientSocket.m_hSocket == INVALID_SOCKET)
	{
		MessageBox(_T("δ���ӵ���������"), _T("����"), MB_ICONERROR);
		return;
	}
	else 
	{
		UpdateData();
		if (m_usrName.IsEmpty()||m_usrPassword.IsEmpty())
		{
			MessageBox(_T("�������û���/���룡"), _T("��ʾ"), MB_ICONWARNING);
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return;
		}
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = NEWCLIENT;
		m_usrNP = m_usrName + m_usrPassword;
		//m_usrNP +=m_usrPassword;
		memcpy(buf + 1, m_usrNP.GetBuffer(0), m_usrNP.GetLength() * 2);
		m_clientSocket.Send(buf, m_usrNP.GetLength() * 2 + 1);
		GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);	//��½��󲻿ɸ����û���
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);	//��½��󲻿��ٵ����¼
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);	//��½�󲻿����ٵ��ע��
		GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(true);	//���Ӻ���ϴ��ļ�

	}
}


void CClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(IDYES==MessageBox(_T("ȷ���˳���"),_T("��ʾ"),MB_ICONQUESTION|MB_YESNO))
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = EXIT;	//�Ͽ�����
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
	buf[0] = EXIT;	//�Ͽ�����
	memcpy(buf+1,m_usrName.GetBuffer(0),m_usrName.GetLength()*2);
	m_clientSocket.Send(buf,m_usrName.GetLength()*2+1);
	m_clientSocket.Close();
	// TODO: �ڴ˴������Ϣ����������
}
void CClientDlg::ProcessRecvData()
{
	CString str;
	unsigned char buf[MAX_BUFSIZE]={0};
	int len = m_clientSocket.Receive(buf,MAX_BUFSIZE);
	wchar_t msg[MAX_BUFSIZE]={0};//��Ϣ����
	wchar_t style[MAX_BUFSIZE]={0};//��ô˵
	wchar_t usrName[MAX_BUFSIZE]={0};//�û�����
	wchar_t srcName[16] = {0};

	//�����ļ��б�
	char CH_nameLength = NULL;
	int IN_namelength = 0;
	CString temp_File_Name;//��ʱ�ļ���string
	CString temp_File_List;

	
	srand((unsigned int)time(NULL));
	if (NEWCLIENT == buf[0])		//���û�������Ϣ
	{
		memcpy(usrName, buf + 1, len-1);		//�û���

		if (LB_ERR == m_ctrlUserOnline.FindString(-1,usrName))		//�����û�����
		{
			m_ctrlUserOnline.AddString(usrName);
		}
		if (LB_ERR == m_ctrlPeopleToTalk.FindString(-1,usrName))	//����˽�ĵ�������
		{
			m_ctrlPeopleToTalk.AddString(usrName);
		}
		str.Empty();
		str+=_T("\t\t��");
		str+=usrName;
		str+=_T("��");
		str+=enterStyle[rand()%MAX_ENTERSTYLE];
		str+=_T("�����ң���һ�ӭ��\r\n");
		m_publicChannel+=str;
	}
	else if (ONLINECLIENT == buf[0])		//��ǰ�����û���Ϣ
	{
		memcpy(usrName,buf+1,len-1);		//�û���
		if (LB_ERR == m_ctrlUserOnline.FindString(-1,usrName))
		{
			m_ctrlUserOnline.AddString(usrName);
		}
		if (LB_ERR == m_ctrlPeopleToTalk.FindString(-1,usrName))
		{
			m_ctrlPeopleToTalk.AddString(usrName);
		}
	}
	else if (MESSAGE_ALL == buf[0])		//����Ƶ������Ϣ
	{
		str.Empty();
		memcpy(usrName,buf+1,16);		//ǰʮ�����ֽڴ��û���
		memcpy(style,buf+17,10);		//ʮ���ֽڵ���Ϣ��ʽ
		memcpy(msg,buf+27,len-27);		//��Ϣ����
		str+=_T("��");
		str+=usrName;
		str+=_T("�� *");
		str+=style;
		str+=_T("* �� �������ˡ� ˵��");
		str+=msg;
		m_publicChannel+=str;
		m_publicChannel+=_T("\r\n");
	}
	else if (MESSAGE_ONE == buf[0])		//˽��Ƶ������Ϣ
	{
		wchar_t srcUsrName[16]={0},dstUsrName[16]={0};
		memcpy(dstUsrName,buf+1,16);		//Ŀ���û���
		memcpy(srcUsrName,buf+17,16);		//Դ�û���
		for (int i = 0; i < m_ctrlBlackName.GetCount(); i++)
		{
			CString str;
			m_ctrlBlackName.GetText(i, str);
			if (srcUsrName == str)	return;
		}
		memcpy(style,buf+33,10);			//��Ϣ��ʽ
		memcpy(msg,buf+43,len-43);		//��Ϣ����
		str.Empty();
		if (m_usrName == dstUsrName)		//���˷��������Ϣ
		{
			str+=_T("��");
			str+=srcUsrName;
			str+=_T("�� *");
			str+=style;
			str+=_T("* �� ���㡿 ˵��");
			str+=msg;
			m_privateChannel+=str;
			m_privateChannel+=_T("\r\n");
		}
	}
	else if (EXIT == buf[0])		//�뿪����Ϣ
	{
		memcpy(usrName,buf+1,len-1);		//�û���
		m_ctrlUserOnline.AddString(usrName);
		str.Empty();
		str+=_T("\t\t��");
		str+=usrName;
		str+=_T("��");
		str+=leaveStyle[rand()%MAX_ENTERSTYLE];
		str+=_T("�����ң�\r\n");
		m_publicChannel+=str;

		int n=m_ctrlUserOnline.FindString(-1,usrName);
		if (LB_ERR != n)
		{
			m_ctrlUserOnline.DeleteString((UINT)n);		//�������д����ɾ������ɾ��
			m_ctrlUserOnline.DeleteString((UINT)n);
		}
		n=m_ctrlPeopleToTalk.FindString(-1,usrName);
		if (LB_ERR != n)
		{
			m_ctrlPeopleToTalk.DeleteString((UINT)n);
		}
	}
	else if (SERVERCLOSE == buf[0])		//�������ر���Ϣ
	{
		MessageBox(_T("�������ѹرգ�"),_T("��ʾ"),MB_ICONINFORMATION);
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
		m_ctrlUserOnline.ResetContent();
	}
	else if (REPEATNAME == buf[0])
	{
		MessageBox(_T("�û����Ѵ��ڣ������ԣ�"),_T("��ʾ"),MB_ICONINFORMATION);
		m_clientSocket.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
		GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
		m_ctrlUserOnline.ResetContent();
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	//���ٵ����¼
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);	//�����ٵ��ע��
		GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(false);	//�����ϴ��ļ�

	}
	else if (ERROR == buf[0])
	{
	MessageBox(_T("�û��������벻��ȷ�������ԣ�"), _T("��ʾ"), MB_ICONINFORMATION);
	m_clientSocket.Close();
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
	GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
	m_ctrlUserOnline.ResetContent();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	//���ٵ����¼
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);	//�����ٵ��ע��
	GetDlgItem(IDC_DOWNLOADFILE)->EnableWindow(false);	//�Ͽ��󲻿��ϴ��ļ�

	}
	else if (HADDEFINED == buf[0])
	{
	MessageBox(_T("�û����ѱ�ע�ᣡ"), _T("��ʾ"), MB_ICONINFORMATION);
	m_clientSocket.Close();
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
	GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
	m_ctrlUserOnline.ResetContent();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	//���ٵ����¼
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);	//�����ٵ��ע��
	}
	else if (REGISTER == buf[0])
	{
	MessageBox(_T("ע��ɹ���"), _T("��ʾ"), MB_ICONINFORMATION);
	m_clientSocket.Close();
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӷ�����"));
	GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);	//�Ͽ�����Ը��û���
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
		IN_namelength = buf[1] - CH_nameLength;//�ļ�������
		wchar_t wbuf[100] = { 0 };

		memcpy(wbuf, buf + 3, IN_namelength);
		strName = wbuf;
		CFileDialog dlg(FALSE, NULL, strName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�����ļ�(*.*)||"), AfxGetMainWnd());
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



//�鿴��������
void CClientDlg::OnBnClickedBtnOtherdata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (LbnDblclkUseronline)
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = WDATA;
		memcpy(buf + 1, ot_dataName.GetBuffer(0), ot_dataName.GetLength() * 2);//16�ֽ����������û���
		memcpy(buf + 17, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);//16�ֽ������û���
		m_clientSocket.Send(buf,33);
	}
	else
		MessageBox(_T("δѡ���û�����˫��ѡ��"), _T("��ʾ"), MB_ICONWARNING);

}

//˫��listboxĳһ�е���Ӧ����
void CClientDlg::OnLbnDblclkUseronline()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlUserOnline.GetText(m_ctrlUserOnline.GetCurSel(), ot_dataName);//�õ�ָ�������ݣ���text
	LbnDblclkUseronline = true;
}

//�鿴��������
void CClientDlg::OnMydata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	data_buf[0] = UpDATA;	//��������
	memcpy(data_buf + 1, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);	//16�ֽ��ϴ����ϵ��û���	
	memcpy(data_buf + 17, m_dataAge.GetBuffer(0), m_dataAge.GetLength() * 2);	//6������		
	memcpy(data_buf + 23, m_dataSex.GetBuffer(0), m_dataSex.GetLength() * 2);	//2���Ա�
	memcpy(data_buf + 25, m_dataLove.GetBuffer(0), m_dataLove.GetLength() * 2);	//40�ֽڰ���
	m_clientSocket.Send(data_buf, m_dataLove.GetLength() * 2 + 25);	//������Ϣ

}

//����
void CClientDlg::OnBnClickedToBlacklist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_BlackName = ot_dataName;

	if (LbnDblclkUseronline&&m_BlackName!=m_usrName)
	{
		/*unsigned char buf[MAX_BUFSIZE];
		buf[0] = BLACK;
		memcpy(buf + 1, m_BlackName.GetBuffer(0), m_BlackName.GetLength() * 2);//16�ֽ����������û���
		m_clientSocket.Send(buf, 17);*/

		m_ctrlBlackName.AddString(m_BlackName);
		int n = m_ctrlPeopleToTalk.FindString(-1, m_BlackName);
		if (LB_ERR != n)
		{
			m_ctrlPeopleToTalk.DeleteString((UINT)n);
		}
	}
	else if(!LbnDblclkUseronline)
		MessageBox(_T("δѡ���û�����˫��ѡ��"), _T("��ʾ"), MB_ICONWARNING);
	else if (m_BlackName==m_usrName)
		MessageBox(_T("���������Լ���"), _T("��ʾ"), MB_ICONWARNING);
	/*if (LbnDblclkUseronline)
	{
		unsigned char buf[MAX_BUFSIZE];
		buf[0] = BLACK;
		memcpy(buf + 1, m_BalckName.GetBuffer(0), m_BalckName.GetLength() * 2);//16�ֽ����������û���
		m_clientSocket.Send(buf, 17);
		m_ctrlBlackName.AddString(m_BalckName);
	}
	else 
		MessageBox(_T("δѡ���û�����˫��ѡ��"), _T("��ʾ"), MB_ICONWARNING);*/
	LbnDblclkUseronline = false;

}

void CClientDlg::OnLbnDblclkBlacklist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	i = m_ctrlBlackName.GetCurSel();
	m_ctrlBlackName.GetText(i, m_BlackName);//�õ�ָ�������ݣ���text
	DblcBlacklist = true;
}
//�Ƴ�������
void CClientDlg::OnBnClickedRestroe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (DblcBlacklist)
	{
		m_ctrlBlackName.DeleteString(i);		
		m_ctrlPeopleToTalk.AddString(m_BlackName);
		m_ctrlBlackName.SetRedraw(FALSE);      //��������
	}
	else
		MessageBox(_T("δѡ���û�����˫��ѡ��"), _T("��ʾ"), MB_ICONWARNING);
	DblcBlacklist = false;
}


void CClientDlg::OnShowBackGround()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char file_buf[MAX_BUFSIZE] = { 0 };
	file_buf[0] = UPFILE;	//�����ļ�
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�ı��ĵ� *.txt|*.txt|ͼƬ *.png|*.png|"), AfxGetMainWnd());
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

	HANDLE hFile = CreateFile(strPath, FILE_GENERIC_READ,            //  ���ļ�������ļ������
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,     //  ����ʽ�򿪣����������ط���Ҫ��д���ļ�
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
	DWORD dwSize = GetFileSize(hFile, NULL);

	if (INVALID_HANDLE_VALUE == hFile)                              //  �ļ���ʧ�ܣ����ش���ֵ
		MessageBox(_T("���ļ�ʧ��"), _T("��ʾ"), MB_ICONWARNING);

	ReadFile(hFile, file_buf+2+strName_length, dwSize, &dwSize, NULL);
	if (dwSize == GetFileSize(hFile, NULL))
	{
		CloseHandle(hFile);
		AfxMessageBox(_T("�������"));
	}
	m_clientSocket.Send(file_buf, dwSize+2+strName_length);	//������Ϣ
}


void CClientDlg::OnDOWNLOADFILE()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char down_file_buf[MAX_BUFSIZE] = { 0 };
	down_file_buf[0] = WATCH_FILELIST;	//��������
	memcpy(down_file_buf + 1, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);	//16�ֽ��ϴ����ϵ��û���	
	m_clientSocket.Send(down_file_buf, m_usrName.GetLength() * 2 + 1);	//������Ϣ
}

void CClientDlg::DownLoad()
{
	unsigned char down_file_buf[MAX_BUFSIZE] = { 0 };
	down_file_buf[0] = DOWNLOAD_FILE;	//��������
	down_file_buf[1] = num;
	memcpy(down_file_buf + 2, m_usrName.GetBuffer(0), m_usrName.GetLength() * 2);	//16�ֽ��ϴ����ϵ��û���	
	m_clientSocket.Send(down_file_buf, m_usrName.GetLength() * 2 + 2);	//������Ϣ
}
