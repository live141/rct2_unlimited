/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "detour.h"
#include "page.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#if defined(__APPLE__) || defined(__linux__)
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

_detour::~_detour() {
	unhook();
}

void _detour::unhook() {
	if(_addr_tramp != NULL) {
		/* fix rel. addresses */
		opcode *op = opcode::create(_addr_tramp, _arch);
		for(uint32_t i = 0; i < _size_replaced;) {
			op->decode();
			op->copy_to(_addr_target+i);
			i += op->size();
		}
		page::free(_addr_tramp);
		_addr_tramp = NULL;
	}
}

void _detour::hook() {
	uint8_t code[8];
	_size_replaced = 0;
	/* create trampoline first, we need executable memory */
	_addr_tramp = (uint8_t*) page::alloc(_size_replaced+5);
	opcode *op = opcode::create(_addr_target+_size_replaced, _arch);
	while(_size_replaced < 5) {
		op->decode();
		op->copy_to(_addr_tramp+_size_replaced);
		_size_replaced += op->size();
	}
	
	/* create jumpback */
	code[0] = 0xE9;
	*((uint32_t*) ((uint8_t*) code+1)) = (int32_t) ((long) _addr_target + _size_replaced - ((long)_addr_tramp+_size_replaced)-5);
	memcpy((uint8_t*) _addr_tramp+_size_replaced, code, 5);

	/* replace target code */
	page::change_permissions(_addr_target, _size_replaced, PAGE_READ | PAGE_WRITE | PAGE_EXEC);
	/* NOP first */
	memset(_addr_target, 0x90, _size_replaced);
	/* jump to our function */
	code[0] = 0xE9;
	*((uint32_t*) ((uint8_t*) code+1)) = (int32_t) ((long) (_addr_new - _addr_target - 5));
	memcpy(_addr_target, code, 5);
}

void _detour::jump_to_function() {
	/* function begins at trampoline address */
	if(_addr_tramp == NULL)
		return;
#if defined(__linux__) || defined(__APPLE__)
	asm volatile("jmp *(%0)"
			:
			: "r"(_addr_tramp)
			:);
#else
		__asm {
			mov eax, _addr_tramp
			jmp eax
		}
#endif
}
