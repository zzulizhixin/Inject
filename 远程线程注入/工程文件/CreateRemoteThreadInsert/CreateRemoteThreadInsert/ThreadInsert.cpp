#include <windows.h>
#include "stdafx.h"
#include "ThreadInsert.h"

#include "tlhelp32.h"

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

//根据进程名称查找pid
DWORD GetProcessIDByName(const wchar_t* pName)
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


//打开待注入的进程
HANDLE OpenTargetProcess(DWORD dpid)
{
	HANDLE hTargetProcess;
	hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE , dpid);
	if(hTargetProcess == NULL)
	{
		OutputDebugString(L"获取目标进程句柄失败");
	}
	return hTargetProcess;
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
