/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#if defined(__linux__) || defined(__APPLE__)
#include <pthread.h>
#else
#include <Windows.h>
#include <Dbghelp.h>
#include <psapi.h>
#endif
#include "signature.h"
#include <sstream>
#include "defines.h"
#include "memscan.h"
#include "detour.h"
#include "addr.h"
#include "exports.h"

module_t g_target;

bool main_loop() {

	return true;
}

#if defined(__linux__) || defined(__APPLE__)
void* unix_thread(void *args) {
	patch_init();
	while(main_loop());
	return 0;
}

extern "C" void patch_main() {
	pthread_t tid;
	pthread_create(&tid, NULL, &unix_thread, NULL);
}
#else
/* Windows */
LPVOID g_pBaseDll = NULL;
DWORD g_dwSizeDll = 0;
LPVOID g_pBaseExe = NULL;
DWORD g_dwSizeExe = 0;
HANDLE g_hProcess = 0;
DWORD g_dwThreadId = 0;

DWORD WINAPI win_thread(void *args) {
	patch_init();
	while(main_loop());
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if( fdwReason == DLL_PROCESS_ATTACH ) {
		MODULEINFO modinfo;
		g_hProcess = GetCurrentProcess();
		GetModuleInformation(g_hProcess, GetModuleHandle("patch.dll"), &modinfo, sizeof(modinfo));
		g_pBaseDll = modinfo.lpBaseOfDll;
		g_dwSizeDll = modinfo.SizeOfImage;
		g_target.module = GetModuleHandle("rct2.exe");
		GetModuleInformation(g_hProcess, g_target.module, &modinfo, sizeof(modinfo));
		g_pBaseExe = modinfo.lpBaseOfDll;
		g_dwSizeExe = modinfo.SizeOfImage;

		IMAGE_NT_HEADERS *pNtHeader = ImageNtHeader(g_target.module);
		/* after NT header address of section table */
		IMAGE_SECTION_HEADER *pSectionHeader = (IMAGE_SECTION_HEADER *) (pNtHeader + 1);
		for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++) {
			if( memcmp(pSectionHeader->Name, ".data", 5) == 0 ) {
				g_target.section_data.addr = (uint8_t*) g_target.module + pSectionHeader->VirtualAddress;
				g_target.section_data.size = pSectionHeader->Misc.VirtualSize;
			}
			else if( memcmp(pSectionHeader->Name, ".text", 5) == 0 ) {
				g_target.section_text.addr = (uint8_t*) g_target.module + pSectionHeader->VirtualAddress;
				g_target.section_text.size = pSectionHeader->Misc.VirtualSize;
			}
			++pSectionHeader;
		}
		
		CreateThread(NULL, NULL, win_thread, NULL, 0, &g_dwThreadId);
	}
	return true;
}
#endif
