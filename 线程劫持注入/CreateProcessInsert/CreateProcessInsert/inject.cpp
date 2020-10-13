#include "stdafx.h"
#include <Windows.h>
#include "Inject.h"

//以Suspend打开目标进程
BOOL OpenTargetProcess(LPCWSTR ProcName , STARTUPINFO  &st , PROCESS_INFORMATION &pi )
{
	if( !CreateProcess(ProcName , NULL ,NULL , NULL , FALSE , CREATE_SUSPENDED , NULL , NULL , &st , &pi))
	{
		OutputDebugString(L"打开目标进程失败");
		return FALSE;
	}
	return  TRUE;
}

//提升进程权限
BOOL AdjustProcessTokenPrivilege()
{
	CString str;
	HANDLE hToken;
	if (FALSE == OpenProcessToken( GetCurrentProcess() , TOKEN_ALL_ACCESS , &hToken))
	{
		OutputDebugString(L"打开令牌失败");
		return FALSE;
	}

	//查询进程特权信息
	LUID luid;
	if(FALSE==LookupPrivilegeValue(NULL , SE_DEBUG_NAME , &luid))
	{
		OutputDebugString(L"查询进程特权信息失败");
		return FALSE;
	}

	//调整令牌访问特权
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(FALSE == AdjustTokenPrivileges(hToken , FALSE , &tkp , sizeof(tkp) , NULL , NULL))
	{
		OutputDebugString(L"调节访问令牌特权属性失败");
		return FALSE;
	}
	return TRUE;
}
//在待注入的进程中申请空间
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr)
{
	lpAddr = VirtualAllocEx(hTargetProcess , NULL , 0x1000 , MEM_COMMIT , PAGE_EXECUTE_READ);
	if(lpAddr == NULL)
	{
		OutputDebugString(L"在远程线程中内存空间申请失败");
		return FALSE;
	}
	return TRUE;
}

//将DLL路径写入申请的空间
BOOL WriteDLLToTarget(HANDLE hTargetProcess ,	LPVOID lpAddr , LPCWSTR lpBuffer)
{
	if(WriteProcessMemory(hTargetProcess ,
		lpAddr ,
		(PVOID)lpBuffer ,
		2 * (1 + lstrlen(lpBuffer)) ,
		NULL) == FALSE)
	{
		OutputDebugString(L"在远程线程中写入失败");
		return FALSE;
	}
	return TRUE;
}

//在目标进程中开辟线程
BOOL CreateThreadInTarget(HANDLE hTargetProcess , PTHREAD_START_ROUTINE pnfStartAddr , LPVOID lpAddr)
{
	HANDLE hThreadHandle;
	hThreadHandle = CreateRemoteThread(hTargetProcess , NULL , 0 , pnfStartAddr , lpAddr , 0 , NULL);
	if(hThreadHandle == NULL)
	{
		OutputDebugString(L"在目标进程中创建线程失败");
		return FALSE;
	}
	return TRUE;
}
