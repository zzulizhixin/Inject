
// CreateRemoteThreadInsertDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateRemoteThreadInsert.h"
#include "CreateRemoteThreadInsertDlg.h"
#include "afxdialogex.h"
#include "ThreadInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCreateRemoteThreadInsertDlg 对话框



CCreateRemoteThreadInsertDlg::CCreateRemoteThreadInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateRemoteThreadInsertDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateRemoteThreadInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateRemoteThreadInsertDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT, &CCreateRemoteThreadInsertDlg::OnBnClickedInsert)
END_MESSAGE_MAP()


// CCreateRemoteThreadInsertDlg 消息处理程序

BOOL CCreateRemoteThreadInsertDlg::OnInitDialog()
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

void CCreateRemoteThreadInsertDlg::OnPaint()
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
HCURSOR CCreateRemoteThreadInsertDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCreateRemoteThreadInsertDlg::OnBnClickedInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	//HWND hwnd;//目标进程窗口句柄
	DWORD hpid;//目标进程pid
	HANDLE htargetprocess;//目标进程句柄
	LPVOID lpAddr;//目标线程申请内存空间的指针
	//	LPVOID lpBuffer;//dll地址

	AdjustProcessTokenPrivilege();
	////查找目标进程窗口
	//hwnd=::FindWindow(TEXT("Edit"),NULL);
	//if(hwnd==NULL)
	//{
	//	OutputDebugString(L"查找窗口失败");
	//	MessageBox(L"注入失败",L"提示",MB_OK);
	//	return;
	//}
	//GetWindowThreadProcessId(hwnd,&hpid);

	hpid=GetProcessIDByName(L"Calculator.exe");

	if(hpid==0)
	{
		OutputDebugString(L"获取窗口进程PID失败");
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}
	htargetprocess=OpenTargetProcess(hpid);
	if(htargetprocess==NULL)
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}
	if(TagetAlloc(htargetprocess, lpAddr) == FALSE)
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

	if(WriteDLLToTarget(htargetprocess, lpAddr , TEXT("C:\\Users\\10178\\Desktop\\InjectDllFile.dll") )== FALSE )
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

	PTHREAD_START_ROUTINE pnfStartAddr ;
	pnfStartAddr=(PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if(pnfStartAddr==NULL)
	{
		OutputDebugString(L"获取LoadLibrary地址失败");
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}
	if( CreateThreadInTarget(htargetprocess,pnfStartAddr,lpAddr) == FALSE)
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

}
