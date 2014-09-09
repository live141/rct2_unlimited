/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "detour.h"
#include "page.h"
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

void detour::unhook() {
	if(_addr_tramp != NULL) {
		memcpy(_addr_target, _addr_tramp, _size_replaced);
		/* fix rel. addresses */
		opcode *op = opcode::create(_addr_target, _arch);
		for(uint32_t i = 0; i < _size_replaced;) {
			op->next();
			if( op->get_operand(0)->is_rel() ) {
				opcode *rel = opcode::create(_addr_tramp+i, _arch);
				rel->decode();
				op->set_imm(rel->immediate());
				delete rel;
			}
			i += op->size();
		}
		page::free(_addr_tramp);
		_addr_tramp = NULL;
	}

	std::vector<opcode*>::iterator it;
	for(it = _vec_opcode.begin(); it < _vec_opcode.end(); ++it) {
		delete *it;
	}
	_vec_opcode.clear();
}

void detour::hook() {
	uint8_t code[8];
	_size_replaced = 0;
	//opcode_x86 op(_addr_target, _bitmode);
	while(_size_replaced < 5) {
		opcode *op = opcode::create(_addr_target+_size_replaced, _arch);
		op->decode();
		_size_replaced += op->size();
		_vec_opcode.push_back(op);
	}
	
	/* create trampoline first, we need executable memory */
	_addr_tramp = (uint8_t*) page::alloc(_size_replaced+5);
	memcpy(_addr_tramp, _addr_target, _size_replaced);
	/* create jumpback */
	code[0] = 0xE9;
	*((uint32_t*) ((uint8_t*) code+1)) = (int32_t) ((int64_t) _addr_target + _size_replaced - ((int64_t)_addr_tramp+_size_replaced)-5);
	memcpy((uint8_t*) _addr_tramp+_size_replaced, code, 5);

	/* replace target code */
	page::change_permissions(_addr_target, _size_replaced, PAGE_READ | PAGE_WRITE | PAGE_EXEC);
	/* NOP first */
	memset(_addr_target, 0x90, _size_replaced);
	/* jump to our function */
	code[0] = 0xE9;
	*((uint32_t*) ((uint8_t*) code+1)) = (int32_t) ((int64_t) (_addr_new - _addr_target - 5));
	memcpy(_addr_target, code, 5);

	/* TODO: extend opcode in case we just have 1 or 2 bytes for rel. address */
	std::vector<opcode*>::iterator it;
	size_t size = 0;
	for(it = _vec_opcode.begin(); it < _vec_opcode.end(); ++it) {
		if((*it)->get_operand(0)->is_rel()) {
			//opcode_x86 op(_addr_tramp+size, _bitmode);
			opcode *op = opcode::create(_addr_tramp+size, _arch);
			op->decode();
			op->set_imm((*it)->immediate());
			delete op;
		}
		size += (*it)->size();
	}
}

void detour::jump_to_function() {
	/* function begins at trampoline address */
	if(_addr_tramp == NULL)
		return;
#if defined(linux) || defined(__APPLE__)
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
