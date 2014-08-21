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
module_t g_rct2;

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
		g_rct2.module = GetModuleHandle(L"rct2.exe");
		GetModuleInformation(g_hProcess, g_rtc2.module, &modinfo, sizeof(modinfo));
		g_pBaseExe = modinfo.lpBaseOfDll;
		g_dwSizeExe = modinfo.SizeOfImage;

		IMAGE_NT_HEADERS *pNtHeader = ImageNtHeader(g_rct2.module);
		/* after NT header address of section table */
		IMAGE_SECTION_HEADER *pSectionHeader = (IMAGE_SECTION_HEADER *) (pNtHeader + 1);
		for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++) {
			if( memcmp(pSectionHeader->Name, ".data", 5) == 0 ) {
				g_rct2.section_data.addr = g_rct2.module + pSectionHeader->VirtualAddress;
				g_rct2.section_data.size = pSectionHeader->Misc.VirtualSize;
			}
			else if( memcmp(pSectionHeader->Name, ".text", 5) == 0 ) {
				g_rct2.section_text.addr = g_rct2.module + pSectionHeader->VirtualAddress;
				g_rct2.section_text.size = pSectionHeader->Misc.VirtualSize;
			}
			++pSectionHeader;
		}
		

		CreateThread(NULL, NULL, loop, NULL, 0, &g_dwThreadId);
	}
	return true;
}
