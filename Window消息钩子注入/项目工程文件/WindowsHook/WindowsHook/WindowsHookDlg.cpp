
// WindowsHookDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowsHook.h"
#include "WindowsHookDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindowsHookDlg 对话框



CWindowsHookDlg::CWindowsHookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowsHookDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowsHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowsHookDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INJECT, &CWindowsHookDlg::OnBnClickedInject)
	ON_BN_CLICKED(IDC_EXIT, &CWindowsHookDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CWindowsHookDlg 消息处理程序

BOOL CWindowsHookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWindowsHookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWindowsHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




DWORD CWindowsHookDlg::GetProcessIDByName(const wchar_t* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS , 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe) ; ret ; ret = Process32Next(hSnapshot , &pe))
	{
		if (wcscmp(pe.szExeFile , pName) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

DWORD CWindowsHookDlg::GetThreadIdByProID(ULONG32 ulTargetProcessID)
{	
	HANDLE Handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (Handle != INVALID_HANDLE_VALUE)	
	{		THREADENTRY32 te;		te.dwSize = sizeof(te);
	if (Thread32First(Handle, &te))
	{			
		do			
		{				
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
			{					
				if (te.th32OwnerProcessID == ulTargetProcessID)					
				{						
					HANDLE hThread = OpenThread(READ_CONTROL, FALSE, te.th32ThreadID);						
					if (!hThread)						
					{							
						OutputDebugString(_T("主线程句柄获取失败"));
					}						
					else						
					{							
						return te.th32ThreadID;						
					}					
				}				
			}			
		} while (Thread32Next(Handle, &te));		
	}	
	}	CloseHandle(Handle);	
	return 0;
}

BOOL CWindowsHookDlg::InjectDllBySetWindowsHook()
{
	HMODULE m_hmDll = LoadLibrary(_T("WindowsHookInjectDLL.dll"));
	if (m_hmDll == NULL)
	{
		OutputDebugString(_T("DLL加载失败"));
		return FALSE;
	}

	HOOKPROC sub_address = NULL;
	sub_address = (HOOKPROC)GetProcAddress(m_hmDll,"MyMessageProcess");
	if(sub_address == NULL)
	{
		OutputDebugString(_T("获取函数地址失败"));
		return FALSE;
	}

	DWORD hPid;
	hPid=GetProcessIDByName(_T("notepad++.exe"));
	if(hPid == NULL)
	{
		OutputDebugString(_T("获取目标进程PID失败"));
		return FALSE;

	}

	DWORD dwThreadID;
	dwThreadID = GetThreadIdByProID(hPid);
	if(dwThreadID == NULL)
	{
		OutputDebugString(_T("获取目标进程主线程句柄失败"));
		return FALSE;
	}

	m_hHook = SetWindowsHookEx(WH_KEYBOARD , sub_address , m_hmDll , dwThreadID);
	if(m_hHook == NULL)
	{
		OutputDebugString(_T("建立钩子失败"));
		return FALSE;
	}

	return TRUE;
}


void CWindowsHookDlg::OnBnClickedInject()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bInject;
	bInject = InjectDllBySetWindowsHook();
	if(bInject)
	{
		MessageBox(_T("注入成功"));
	}
}


void CWindowsHookDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_hHook)
		UnhookWindowsHookEx(m_hHook);

	if(m_hmDll)
		FreeLibrary(m_hmDll);

	CDialogEx::OnCancel();
}
