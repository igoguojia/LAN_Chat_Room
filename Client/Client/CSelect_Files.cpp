// CSelect_Files.cpp: 实现文件
//
#include "stdafx.h"
#include "Client.h"
#include "CSelect_Files.h"
#include "afxdialogex.h"
#include <vector>
#include <memory.h>

using namespace std;

// CSelect_Files 对话框
IMPLEMENT_DYNAMIC(CSelect_Files, CDialogEx)

extern vector <CString> filelist;//文件列表

CSelect_Files::CSelect_Files(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DOWNLOAD_FILE, pParent)
{

}

CSelect_Files::~CSelect_Files()
{
}

void CSelect_Files::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, File_ListBox);
}


BEGIN_MESSAGE_MAP(CSelect_Files, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &CSelect_Files::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CSelect_Files 消息处理程序


BOOL CSelect_Files::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化	

	 vector<CString> ::iterator iter;//文件列表

	 File_ListBox.ResetContent();	//清空在线用户列表
	for (iter = filelist.begin(); iter < filelist.end(); iter++)
	{
		File_ListBox.AddString(*iter);
	}
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSelect_Files::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	CSelect_num = File_ListBox.GetCurSel();
	CSelect_Select_File_OK = true;

}


void CSelect_Files::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!CSelect_Select_File_OK&&File_ListBox.GetCount()!=0)
	{
		MessageBox(_T("未选择文件，请双击选择！"), _T("提示"), MB_ICONWARNING);
		return;
	}
	CDialogEx::OnOK();
}
