#pragma once


// DataDlg 对话框

class DataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DataDlg)

public:
	DataDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DataDlg();
	virtual BOOL OnInitDialog();//创建窗口之初所作所为

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString Data_dataName=_T("");
	CString Data_dataLove;
	CString Data_dataAge;
	CString Data_dataSex;
	bool Data_judgeSelf;//判断是否本人资料

};
