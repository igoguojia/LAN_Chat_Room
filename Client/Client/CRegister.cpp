// CRegister.cpp: 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "CRegister.h"
#include "afxdialogex.h"

// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

CRegister::CRegister(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, Ruser_Name(_T(""))
	, Ruser_Password(_T(""))
{

}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Ruser_Name);
	DDX_Text(pDX, IDC_EDIT2, Ruser_Password);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
END_MESSAGE_MAP()


// CRegister 消息处理程序
