/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _DETOUR_H_
#define _DETOUR_H_

#include "disasm.h"
#include <vector>

class detour {
protected:
	std::vector<opcode*> _vec_opcode;
	uint64_t _saved_ret_addr;
	uint8_t *_addr_target;
	uint8_t *_addr_new;
	uint8_t *_addr_tramp;
	uint8_t _size_replaced;
	const architecture _arch;

public:
	detour(const void *addr_target, const void *addr_new, const architecture arch) : _saved_ret_addr(0),
		_addr_target((uint8_t*) addr_target), _addr_new((uint8_t*) addr_new),
		_addr_tramp(NULL), _size_replaced(0), _arch(arch) {}
	~detour();
	void hook();
	void unhook();
	void jump_to_function();

	const uint8_t* trampoline() const {
		return _addr_tramp;
	}
};

#endif
