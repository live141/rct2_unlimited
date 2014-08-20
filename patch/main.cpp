#include <Windows.h>
#include "Signature.h"
#include "Sigs.h"
#include <psapi.h>
#include <sstream>
#include "rct2.h"
#include "Memory.h"
#include "Detour.h"
#include "macros.h"

LPVOID g_pBaseDll = NULL;
DWORD g_dwSizeDll = 0;
LPVOID g_pBaseExe = NULL;
DWORD g_dwSizeExe = 0;
HANDLE g_hProcess = 0;
DWORD g_dwThreadId = 0;

DWORD WINAPI loop(void *args) {
	while(true) {

	}
	return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if( fdwReason == DLL_PROCESS_ATTACH ) {
		MODULEINFO modinfo;
		g_hProcess = GetCurrentProcess();
		GetModuleInformation(g_hProcess, GetModuleHandle(L"patch.dll"), &modinfo, sizeof(modinfo));
		g_pBaseDll = modinfo.lpBaseOfDll;
		g_dwSizeDll = modinfo.SizeOfImage;
		GetModuleInformation(g_hProcess, GetModuleHandle(L"rct2.exe"), &modinfo, sizeof(modinfo));
		g_pBaseExe = modinfo.lpBaseOfDll;
		g_dwSizeExe = modinfo.SizeOfImage;
		

		CreateThread(NULL, NULL, loop, NULL, 0, &g_dwThreadId);
	}
	return true;
}
