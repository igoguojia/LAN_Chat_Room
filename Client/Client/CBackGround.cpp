// CBackGround.cpp: 实现文件
//
#include "stdafx.h"
#include "Client.h"
#include "CBackGround.h"
#include "afxdialogex.h"


// CBackGround 对话框

IMPLEMENT_DYNAMIC(CBackGround, CDialogEx)

CBackGround::CBackGround(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CBackGround::~CBackGround()
{
}

void CBackGround::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBackGround, CDialogEx)
END_MESSAGE_MAP()


// CBackGround 消息处理程序
