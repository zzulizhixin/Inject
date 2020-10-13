
// IMEInjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IMEInject.h"
#include "IMEInjectDlg.h"
#include "afxdialogex.h"
#include <imm.h>
#pragma comment(lib, "IMM32.LIB")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIMEInjectDlg 对话框



CIMEInjectDlg::CIMEInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIMEInjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMEInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIMEInjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMEINSERT, &CIMEInjectDlg::OnBnClickedImeinsert)
	ON_BN_CLICKED(IDC_DELETE, &CIMEInjectDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CIMEInjectDlg 消息处理程序

BOOL CIMEInjectDlg::OnInitDialog()
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

void CIMEInjectDlg::OnPaint()
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
HCURSOR CIMEInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIMEInjectDlg::OnBnClickedImeinsert()
{
	// TODO: 在此添加控件通知处理程序代码
	::SystemParametersInfo(
		SPI_GETDEFAULTINPUTLANG,
		0,
		&m_retV,
		0);
	m_hImeFile32 = ImmInstallIME(_T("IMEDLL.ime"),_T("输入法"));

	if( ImmIsIME(m_hImeFile32))
	{
		SystemParametersInfo(
			SPI_GETDEFAULTINPUTLANG,
			0,
			&m_hImeFile32,
			SPIF_SENDWININICHANGE
			);
		::MessageBox(NULL, _T("输入法加载完成") ,  _T("提示") , MB_OK);
	}
	else
	{
		::MessageBox(NULL, _T("输入法加载失败,检查1.是否是管理员权限运行,2.ime和dll文件是否拷贝到C:\Windows\SysWOW64目录下,3.运行exe目录下是否有两个文件") ,  _T("提示") ,  MB_OK);
	}
}


void CIMEInjectDlg::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	::SystemParametersInfo(
		SPI_SETDEFAULTINPUTLANG,
		0,
		&m_retV,
		SPIF_SENDWININICHANGE
		);

	if (m_hImeFile32 != NULL)
	{

		if(UnloadKeyboardLayout(m_hImeFile32))
		{
			::MessageBox (NULL , _T("输入法卸载成功") ,  _T("提示") , MB_OK);
		}
		else
		{
			::MessageBox (NULL , _T("输入法卸载失败,请手动卸载") ,  _T("提示") , MB_OK);
		}
	}
	else
	{
		::MessageBox (NULL , _T("先安装输入法") ,  _T("提示") , MB_OK);

	}
}
