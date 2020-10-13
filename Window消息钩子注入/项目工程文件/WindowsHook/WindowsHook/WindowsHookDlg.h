
// WindowsHookDlg.h : 头文件
//

#pragma once


// CWindowsHookDlg 对话框
class CWindowsHookDlg : public CDialogEx
{
// 构造
public:
	CWindowsHookDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WINDOWSHOOK_DIALOG };

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
	afx_msg void OnBnClickedInject();
	BOOL InjectDllBySetWindowsHook(void);
	DWORD GetProcessIDByName(const wchar_t* pName);
	DWORD GetThreadIdByProID(ULONG32 ulTargetProcessID);

public:
	DWORD m_dwId;
	HHOOK m_hHook;
	HMODULE m_hmDll;

	afx_msg void OnBnClickedExit();
};
