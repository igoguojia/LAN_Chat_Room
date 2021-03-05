#pragma once


// CBackGround 对话框

class CBackGround : public CDialogEx
{
	DECLARE_DYNAMIC(CBackGround)

public:
	CBackGround(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBackGround();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
