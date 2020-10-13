//以Suspend打开目标进程
BOOL OpenTargetProcess(LPCWSTR ProcName , STARTUPINFO  &st , PROCESS_INFORMATION &pi );
//在目标进程地址空间中分配待加载的DLL路径空间
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr);
//把待加载的DLL路径写入目标进程空间
BOOL WriteDLLToTarget(HANDLE hTargetProcess ,	LPVOID lpAddr , LPCWSTR lpBuffer);
