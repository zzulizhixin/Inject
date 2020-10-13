//以Suspend打开目标进程
BOOL OpenTargetProcess(LPCWSTR ProcName , STARTUPINFO  &st , PROCESS_INFORMATION &pi );
//提升进程权限
BOOL AdjustProcessTokenPrivilege();
//在待注入的进程中申请空间
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr);
//将DLL路径写入申请的空间
BOOL WriteDLLToTarget(HANDLE hTargetProcess ,	LPVOID lpAddr , LPCWSTR lpBuffer);
//在目标进程中开辟线程
BOOL CreateThreadInTarget(HANDLE hTargetProcess , PTHREAD_START_ROUTINE pnfStartAddr , LPVOID lpAddr);
