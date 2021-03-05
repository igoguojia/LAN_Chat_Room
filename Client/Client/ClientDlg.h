
// ClientDlg.h : 头文件
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
#define WDATA           9//通知服务器发送资料给客户端
#define UpDATA			10//上传资料
#define BLACK			11
#define RESTORE			12
#define UPFILE			13//上传文件
#define WATCH_FILELIST	14//查看文件列表
#define DOWNLOAD_FILE		15

const static wchar_t enterStyle[][20]={_T("一个筋斗云翻进"),
										_T("偷偷摸摸地溜进"),
										_T("大摇大摆地走进"),
										_T("活蹦乱跳地进入"),
										_T("闲庭信步地走进")};
const static wchar_t leaveStyle[][20]={_T("恋恋不舍地离开了"),
										_T("潇洒地离开了"),
										_T("落寞地转身离开了"),
										_T("脚步轻轻地离开了"),
										_T("头也不回地离开了")};
// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSendmsg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CString m_usrName=_T("");//用户名
							
	CString m_usrPassword;// 用户密码	
	CString m_usrNP;	
	CString m_serverIP;//服务器IP地址
	CIPAddressCtrl m_ctrlServerIP;//服务器IP地址控件
	UINT m_serverPort;//端口值
	CClientSocket m_clientSocket;
	CString m_sendMsg;//发送框
	CFont m_editFont;
	CEdit m_ctrlSendMsg;//发送框
public:
	afx_msg void OnBnClickedBtnConnect();//连接
	afx_msg void OnClose();//断开

	void ProcessRecvData();//发送接收消息
	CComboBox m_ctrlMsgStyle;//你。。。对中的box
	CComboBox m_ctrlPeopleToTalk;//对谁说box	
	CString m_strPeopleToTalk;//对谁说，值
	CString m_publicChannel;//公共频道
	CString m_privateChannel;//私人频道
	CListBox m_ctrlUserOnline;//在线人员box
	afx_msg void OnDestroy();

	
	afx_msg void OnLogoIn();//登录
	afx_msg void OnRegister();//注册
	//个人资料
	CString m_dataLove=_T("");
	CString m_dataAge=_T("");
	CString m_dataSex = _T("");
	afx_msg void OnMydata();

	afx_msg void OnLbnDblclkUseronline();//双击用户列表响应函数
	
	//他人资料
	afx_msg void OnBnClickedBtnOtherdata();//查看他人资料
	CString ot_dataName;//要查看资料的用户的名字
	CString ot_dataLove;
	CString ot_dataAge;
	CString ot_dataSex ;
	bool m_judgeSelf = true;//是否本人
	bool LbnDblclkUseronline = false;//是否选中用户

	afx_msg void OnBnClickedToBlacklist();//拉黑
	//afx_msg void OnBnClickedRestroe();//移除黑名单
	afx_msg void OnLbnDblclkBlacklist();//拉黑窗口双击
	CString m_BlackName;	
	CListBox m_ctrlBlackName;

	bool	DblcBlacklist = false;//是否选中用户
	int i;

	afx_msg void OnBnClickedRestroe();
	afx_msg void OnShowBackGround();
	afx_msg void OnUPFILE();
	afx_msg void OnDOWNLOADFILE();
	int num;//选中的文件位置
	bool Select_File_OK = false;
	void DownLoad();

};
