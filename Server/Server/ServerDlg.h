
// ServerDlg.h : 头文件
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
#define UpDATA			10//收到看资料请求，发送资料
#define BLACK			11
#define RESTORE			12
#define UPFILE			13
#define	WATCH_FILELIST	14//下载文件
#define DOWNLOAD_FILE		15

const int MAX_BUFSIZE=1024;//消息的最大字符数
// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();//原始的ok按钮函数

public:
	afx_msg void OnBnClickedCancel();//关闭按钮
	afx_msg void OnBnClickedBtnStartStop();//开启按钮
private:

	short m_listenPort;//见听端口
	CIPAddressCtrl m_ctrlServerIP;//服务器IP地址
	CString m_srvrName;			//服务器的主机名
	CString m_srvrStatus;		//服务器的状态，可以为停止或正在监听
	CComboBox m_ctrlMaxCnt;	//最大客户端数，下拉窗口
public:
	CServerSocket m_serverSocket;//
	UINT m_clientCnt;//当前客户端数
	UINT m_maxClient;//当前最大客户端数
	CListBox m_ctrlChatRoomInfo;//日志窗口
	CListBox m_ctrlUsers;//用户窗口

	afx_msg void OnBnClickedBtnClear();//清空
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//消息函数
	afx_msg void OnClose();//关闭的消息函数

	void ProcessData(CClientSocket *clientSocket);//接受发送消息给客户端的函数
	
	afx_msg void OnBnClickedBtnExport();//导出按钮
	afx_msg void OnGetUsersNP();//读取


	

};
