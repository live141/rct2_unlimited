#include "detour.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#if defined(__APPLE__) || defined(linux)
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

detour::~detour() {
	unhook();
}

void* detour::_alloc_mem(size_t size) {
#if defined(__APPLE__) || defined(linux)
	void *addr = NULL;
	int page_size = getpagesize();
	if((addr = mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) != MAP_FAILED) {
		return addr;
	}
	std::cout << "Error: Could not map memory: " << errno << std::endl;
#else
#endif
}

void detour::_free_mem(void *addr) {
#if defined(__APPLE__) || defined(linux)
	int page_size = getpagesize();
	if(addr != NULL) {
		munmap(addr, page_size);
	}
#else
#endif
}

void detour::_change_page_permissions(void *addr) {
#if defined(__APPLE__) || defined(linux)
	int page_size = getpagesize();
	if(mprotect((void*)((unsigned long) addr & ~(page_size-1)), page_size, PROT_READ | PROT_EXEC | PROT_WRITE ) != 0) {
		std::cout << "Error: Could not change page permissions: " << errno << std::endl; 
	}
#else
#endif
}

void detour::unhook() {
	if(_addr_tramp != NULL) {
		memcpy(_addr_target, _addr_tramp, _size_replaced);
		_free_mem(_addr_tramp);
		_addr_tramp = NULL;
	}
}

void detour::hook_function() {
	uint8_t code[8];
	_size_replaced = 0;
	opcode_x86 op(_addr_target, mode_64);
	while(_size_replaced < 5) {
		op.next();
		_size_replaced += op.size();
	}
	
	/* create trampoline first, we need executable memory */
	_addr_tramp = (uint8_t*) _alloc_mem(_size_replaced+7);
	memcpy(_addr_tramp, _addr_target, _size_replaced);
	/* create jumpback */
	code[0] = 0xE9;
	*((uint32_t*) ((uint8_t*) code+1)) = (int32_t) ((int64_t) _addr_target + _size_replaced - ((int64_t)_addr_tramp+_size_replaced)-5);
	memcpy((uint8_t*) _addr_tramp+_size_replaced, code, 5);

	/* replace target code */
	_change_page_permissions(_addr_target);
	/* NOP first */
	memset(_addr_target, 0x90, _size_replaced);
	/* jump to our function */
	code[0] = 0xE9;
	*((uint32_t*) ((uint8_t*) code+1)) = (int32_t) ((int64_t) (_addr_new - _addr_target - 5));
	memcpy(_addr_target, code, 5);
}

