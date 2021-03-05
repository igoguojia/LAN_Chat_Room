
// ServerDlg.h : ͷ�ļ�
//
#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

#define EXIT			255
#define SERVERCLOSE		254
#define REPEATNAME		253
#define NEWCLIENT		1
#define ONLINECLIENT	2
#define MESSAGE_ALL		5
#define MESSAGE_ONE		6
#define ERROR			4

#define REGISTER		7
#define HADDEFINED		8
#define WDATA           9
#define UpDATA			10//�յ����������󣬷�������
#define BLACK			11
#define RESTORE			12
#define UPFILE			13
#define	WATCH_FILELIST	14//�����ļ�
#define DOWNLOAD_FILE		15

const int MAX_BUFSIZE=1024;//��Ϣ������ַ���
// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

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
	afx_msg void OnBnClickedOk();//ԭʼ��ok��ť����

public:
	afx_msg void OnBnClickedCancel();//�رհ�ť
	afx_msg void OnBnClickedBtnStartStop();//������ť
private:

	short m_listenPort;//�����˿�
	CIPAddressCtrl m_ctrlServerIP;//������IP��ַ
	CString m_srvrName;			//��������������
	CString m_srvrStatus;		//��������״̬������Ϊֹͣ�����ڼ���
	CComboBox m_ctrlMaxCnt;	//���ͻ���������������
public:
	CServerSocket m_serverSocket;//
	UINT m_clientCnt;//��ǰ�ͻ�����
	UINT m_maxClient;//��ǰ���ͻ�����
	CListBox m_ctrlChatRoomInfo;//��־����
	CListBox m_ctrlUsers;//�û�����

	afx_msg void OnBnClickedBtnClear();//���
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//��Ϣ����
	afx_msg void OnClose();//�رյ���Ϣ����

	void ProcessData(CClientSocket *clientSocket);//���ܷ�����Ϣ���ͻ��˵ĺ���
	
	afx_msg void OnBnClickedBtnExport();//������ť
	afx_msg void OnGetUsersNP();//��ȡ


	

};
