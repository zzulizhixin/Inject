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

//在目标进程地址空间中分配待加载的DLL路径空间
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

//把待加载的DLL路径写入目标进程空间
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

