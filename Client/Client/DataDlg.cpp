// DataDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "Client.h"
#include "DataDlg.h"
#include "afxdialogex.h"
//#include"ClientDlg.h"

// DataDlg 对话框

IMPLEMENT_DYNAMIC(DataDlg, CDialogEx)

DataDlg::DataDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, Data_dataName(_T(""))
	, Data_dataLove(_T(""))
	, Data_dataAge(_T(""))
	, Data_dataSex(_T(""))
{

}
DataDlg::~DataDlg()
{
}

void DataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, Data_dataName);
	DDX_Text(pDX, IDC_EDIT1, Data_dataLove);	
	DDV_MaxChars(pDX, Data_dataLove, 20);

	DDX_Text(pDX, IDC_EDIT3, Data_dataAge);	
	DDV_MaxChars(pDX, Data_dataAge, 3);

	DDX_Text(pDX, IDC_EDIT2, Data_dataSex);
	DDV_MaxChars(pDX, Data_dataSex, 1);
	//若不是一个对一个，则会最后再跳出错误框报错字符超出

}


BEGIN_MESSAGE_MAP(DataDlg, CDialogEx)
END_MESSAGE_MAP()


// DataDlg 消息处理程序
BOOL DataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CClientDlg *m_pClientDlg = (CClientDlg*)this->GetParent();
	Data_judgeSelf = m_pClientDlg->m_judgeSelf;
	if (Data_judgeSelf)
	{
		Data_dataName = m_pClientDlg->ot_dataName;
		Data_dataLove = m_pClientDlg->m_dataLove;
		Data_dataSex = m_pClientDlg->m_dataSex;
		Data_dataAge = m_pClientDlg->m_dataAge;
		//GetDlgItem(IDC_EDIT5)->SetWindowTextW(Data_dataName);
		//GetDlgItem(IDC_EDIT1)->SetWindowTextW(Data_dataLove);	
		//GetDlgItem(IDC_EDIT3)->SetWindowTextW(Data_dataAge);
		//GetDlgItem(IDC_EDIT2)->SetWindowTextW(Data_dataSex);
		UpdateData(false);
	}
	else//非本人资料不可更改
	{
		Data_dataName = m_pClientDlg->ot_dataName;
		Data_dataLove = m_pClientDlg->ot_dataLove;
		Data_dataSex = m_pClientDlg->ot_dataSex;
		Data_dataAge = m_pClientDlg->ot_dataAge;
		UpdateData(false);

		GetDlgItem(IDC_EDIT2)->EnableWindow(false);	//不可更改性别
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);	//不可更改性别
		GetDlgItem(IDC_EDIT1)->EnableWindow(false);	//不可更改性别
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
