#pragma once


// CSelect_Files 对话框

class CSelect_Files : public CDialogEx
{
	DECLARE_DYNAMIC(CSelect_Files)

public:
	CSelect_Files(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelect_Files();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOAD_FILE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox File_ListBox;
	afx_msg void OnLbnDblclkList1();
	int CSelect_num;//选中的文件位置
	bool CSelect_Select_File_OK = false;
	virtual void OnOK();
	//CClientDlg *m_pClientDlg = (CClientDlg*)this->GetParent();

};
