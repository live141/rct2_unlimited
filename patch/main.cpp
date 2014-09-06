#if defined(linux) || defined(__APPLE__)
#include <pthread>
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

module_t g_rct2;

bool main_loop() {

	return true;
}

#if defined(linux) || defined(__APPLE__)
void* unix_thread(void *args) {
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
		g_rct2.module = GetModuleHandle("rct2.exe");
		GetModuleInformation(g_hProcess, g_rct2.module, &modinfo, sizeof(modinfo));
		g_pBaseExe = modinfo.lpBaseOfDll;
		g_dwSizeExe = modinfo.SizeOfImage;

		IMAGE_NT_HEADERS *pNtHeader = ImageNtHeader(g_rct2.module);
		/* after NT header address of section table */
		IMAGE_SECTION_HEADER *pSectionHeader = (IMAGE_SECTION_HEADER *) (pNtHeader + 1);
		for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++) {
			if( memcmp(pSectionHeader->Name, ".data", 5) == 0 ) {
				g_rct2.section_data.addr = (uint8_t*) g_rct2.module + pSectionHeader->VirtualAddress;
				g_rct2.section_data.size = pSectionHeader->Misc.VirtualSize;
			}
			else if( memcmp(pSectionHeader->Name, ".text", 5) == 0 ) {
				g_rct2.section_text.addr = (uint8_t*) g_rct2.module + pSectionHeader->VirtualAddress;
				g_rct2.section_text.size = pSectionHeader->Misc.VirtualSize;
			}
			++pSectionHeader;
		}
		

		CreateThread(NULL, NULL, win_thread, NULL, 0, &g_dwThreadId);
	}
	return true;
}
#endif
