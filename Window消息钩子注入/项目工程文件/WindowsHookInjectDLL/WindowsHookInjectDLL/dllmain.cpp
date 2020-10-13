// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <tchar.h>
#include <windows.h>


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			MessageBox(NULL , _T("DLL加载成功") ,  _T("DLL窗口提示") , IDOK);
			break;
		} 
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{

			MessageBox(NULL , _T("DLL卸载成功") ,  _T("DLL窗口提示") , IDOK);
			break;
		}
	}
	return TRUE;
}


//LRESULT CALLBACK MsgHookProc(
//	int code,
//	WPARAM wParam,
//	LPARAM lParam
//	)
//{
//	return CallNextHookEx(g_hHook , code , wParam , lParam);
//}
//
//
//extern "C"  VOID InstallHook()
//{
//	g_hHook = SetWindowsHookEx(WH_GETMESSAGE , MsgHookProc , g_hModule , 0);
//}
//
//extern "C" VOID UnInstallHook()
//{
//	UnInstallHook();
//}



extern "C" __declspec(dllexport) LRESULT  MyMessageProcess(int Code, WPARAM wParam, LPARAM lParam)

{

	MessageBox(NULL, _T("Windows消息钩子注入成功"), _T("DLL窗口提示"), 0);
	return 0;

}