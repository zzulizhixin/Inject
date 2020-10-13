
// IMEInjectDlg.h : 头文件
//

#pragma once


// CIMEInjectDlg 对话框
class CIMEInjectDlg : public CDialogEx
{
// 构造
public:
	CIMEInjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMEINJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImeinsert();
	PVOID m_retV;
	HKL m_hImeFile32;
	afx_msg void OnBnClickedDelete();
};
