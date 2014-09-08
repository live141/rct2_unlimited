/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#if defined(linux) || defined(__APPLE__)
#else
#include <Windows.h>
#include <direct.h>
#endif
#include <iostream>
#include "parameters.h"


#if defined(linux) || defined(__APPLE__)
int main(int argc, char **argv) {
	std::string target;

	std::cout << NAME << " "VERSION << " by " << AUTHOR << std::endl;
	std::cout << "Enter the name of the target: ";
	std::cin >> target;

	return 0;
}
#else
/* Windows */
int main(int argc, char **argv) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD hLibModule; 
	HANDLE hThread;
	void *pLibRemote;
	HMODULE hKernel32;
	std::string target;

	std::cout << NAME << " "VERSION << " by " << AUTHOR << std::endl;
	std::cout << "Enter the name of the target: ";
	std::cin >> target;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	if( !CreateProcess( TEXT(target.c_str()),   // No module name (use command line)
				NULL,			// Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0x00000004,     // flag: start suspended
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi )           // Pointer to PROCESS_INFORMATION structure
	  ) {
		std::cout << "Error " << GetLastError() << ": Could not launch " << target << std::endl;
	}

	/* inject our dll */
	hKernel32 = GetModuleHandle(TEXT("Kernel32"));
	pLibRemote = VirtualAllocEx( pi.hProcess, NULL, sizeof(NAME_DLL), MEM_COMMIT, PAGE_READWRITE );
	WriteProcessMemory( pi.hProcess, pLibRemote, (void *) NAME_DLL, sizeof(NAME_DLL), NULL );
	hThread = CreateRemoteThread( pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) GetProcAddress( hKernel32, "LoadLibraryA" ), pLibRemote, 0, NULL );

	WaitForSingleObject( hThread, INFINITE );
	GetExitCodeThread( hThread, &hLibModule );
	CloseHandle( hThread );
	VirtualFreeEx( pi.hProcess, pLibRemote, sizeof(NAME_DLL), MEM_RELEASE );

	/* returned from DllMain, so resume Thread */
	ResumeThread(pi.hThread);

	system("pause");
	return 0;
}
#endif
