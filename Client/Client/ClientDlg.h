
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ClientSocket.h"
#include"CBackGround.h"
#include "CSelect_Files.h"
#include <vector>

const int MAX_BUFSIZE = 1024;
#define EXIT			255
#define SERVERCLOSE		254
#define REPEATNAME		253
#define NEWCLIENT		1
#define ONLINECLIENT	2
#define MESSAGE_ALL		5
#define MESSAGE_ONE		6

#define MAX_ENTERSTYLE  5
#define MAX_LEAVESTYLE  5
#define ERROR		4
#define REGISTER		7
#define HADDEFINED		8
#define WDATA           9//֪ͨ�������������ϸ��ͻ���
#define UpDATA			10//�ϴ�����
#define BLACK			11
#define RESTORE			12
#define UPFILE			13//�ϴ��ļ�
#define WATCH_FILELIST	14//�鿴�ļ��б�
#define DOWNLOAD_FILE		15

const static wchar_t enterStyle[][20]={_T("һ����Ʒ���"),
										_T("͵͵���������"),
										_T("��ҡ��ڵ��߽�"),
										_T("��������ؽ���"),
										_T("��ͥ�Ų����߽�")};
const static wchar_t leaveStyle[][20]={_T("����������뿪��"),
										_T("�������뿪��"),
										_T("��į��ת���뿪��"),
										_T("�Ų�������뿪��"),
										_T("ͷҲ���ص��뿪��")};
// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSendmsg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CString m_usrName=_T("");//�û���
							
	CString m_usrPassword;// �û�����	
	CString m_usrNP;	
	CString m_serverIP;//������IP��ַ
	CIPAddressCtrl m_ctrlServerIP;//������IP��ַ�ؼ�
	UINT m_serverPort;//�˿�ֵ
	CClientSocket m_clientSocket;
	CString m_sendMsg;//���Ϳ�
	CFont m_editFont;
	CEdit m_ctrlSendMsg;//���Ϳ�
public:
	afx_msg void OnBnClickedBtnConnect();//����
	afx_msg void OnClose();//�Ͽ�

	void ProcessRecvData();//���ͽ�����Ϣ
	CComboBox m_ctrlMsgStyle;//�㡣�������е�box
	CComboBox m_ctrlPeopleToTalk;//��˭˵box	
	CString m_strPeopleToTalk;//��˭˵��ֵ
	CString m_publicChannel;//����Ƶ��
	CString m_privateChannel;//˽��Ƶ��
	CListBox m_ctrlUserOnline;//������Աbox
	afx_msg void OnDestroy();

	
	afx_msg void OnLogoIn();//��¼
	afx_msg void OnRegister();//ע��
	//��������
	CString m_dataLove=_T("");
	CString m_dataAge=_T("");
	CString m_dataSex = _T("");
	afx_msg void OnMydata();

	afx_msg void OnLbnDblclkUseronline();//˫���û��б���Ӧ����
	
	//��������
	afx_msg void OnBnClickedBtnOtherdata();//�鿴��������
	CString ot_dataName;//Ҫ�鿴���ϵ��û�������
	CString ot_dataLove;
	CString ot_dataAge;
	CString ot_dataSex ;
	bool m_judgeSelf = true;//�Ƿ���
	bool LbnDblclkUseronline = false;//�Ƿ�ѡ���û�

	afx_msg void OnBnClickedToBlacklist();//����
	//afx_msg void OnBnClickedRestroe();//�Ƴ�������
	afx_msg void OnLbnDblclkBlacklist();//���ڴ���˫��
	CString m_BlackName;	
	CListBox m_ctrlBlackName;

	bool	DblcBlacklist = false;//�Ƿ�ѡ���û�
	int i;

	afx_msg void OnBnClickedRestroe();
	afx_msg void OnShowBackGround();
	afx_msg void OnUPFILE();
	afx_msg void OnDOWNLOADFILE();
	int num;//ѡ�е��ļ�λ��
	bool Select_File_OK = false;
	void DownLoad();

};
