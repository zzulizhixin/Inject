
// CreateProcessInsertDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateProcessInsert.h"
#include "CreateProcessInsertDlg.h"
#include "afxdialogex.h"
#include "inject.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCreateProcessInsertDlg 对话框



CCreateProcessInsertDlg::CCreateProcessInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateProcessInsertDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateProcessInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateProcessInsertDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT, &CCreateProcessInsertDlg::OnBnClickedInsert)
END_MESSAGE_MAP()


// CCreateProcessInsertDlg 消息处理程序

BOOL CCreateProcessInsertDlg::OnInitDialog()
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

void CCreateProcessInsertDlg::OnPaint()
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
HCURSOR CCreateProcessInsertDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCreateProcessInsertDlg::OnBnClickedInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	PROCESS_INFORMATION stProcessInfo;                   // 存储进程信息的PROCESS_INFORMATION 结构体
	::memset(&stProcessInfo, 0 ,sizeof(stProcessInfo));  //分配结构体内存

	STARTUPINFO stStartUpInfo;                                    //进程的主窗体显示信息的STARTUPINFO结构体 
	::memset(&stStartUpInfo, 0 ,sizeof(stStartUpInfo)); //分配结构体内存
	stStartUpInfo.cb = sizeof(stStartUpInfo);

	LPCWSTR ProcName;                                          //目标进程地址名
	LPVOID    lpAddr;                                               //目标进程申请内存空间的指针
	LPCWSTR lpBuffer;                                             //dll地址
	PTHREAD_START_ROUTINE pnfStartAddr ;       //LoadLibrary地址
	
	AdjustProcessTokenPrivilege();

	ProcName = L"D:\\Program Files (x86)\\Notepad++\\notepad++.exe";
	if(	OpenTargetProcess(ProcName , stStartUpInfo , stProcessInfo ) == FALSE )
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

	HANDLE hTargetProcess= stProcessInfo.hProcess;	    //目标进程的句柄
	if( TagetAlloc(hTargetProcess , lpAddr) == FALSE )
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

	lpBuffer= TEXT("C:\\Users\\10178\\Desktop\\InjectDllFile.dll");
	if(WriteDLLToTarget(hTargetProcess, lpAddr , lpBuffer)== FALSE )
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

	HANDLE	 hTargetThread = stProcessInfo.hThread;       //目标线程的句柄
	//获取LoadLibrary地址
	pnfStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32")) , "LoadLibraryW");
	if(pnfStartAddr == NULL)
	{
		OutputDebugString(L"获取LoadLibrary地址失败");
		MessageBox(L"注入失败" , L"提示" , MB_OK);
		return;
	}
	if( CreateThreadInTarget(hTargetProcess  , pnfStartAddr , lpAddr) == FALSE)
	{
		MessageBox(L"注入失败",L"提示",MB_OK);
		return;
	}

	//恢复注入的目标线程挂起状态
	 ResumeThread(hTargetThread);

}
