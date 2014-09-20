/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _DETOUR_H_
#define _DETOUR_H_

#include "disasm.h"
#include <vector>

class _detour {
protected:
	std::vector<opcode*> _vec_opcode;
	uint64_t _saved_ret_addr;
	uint8_t *_addr_target;
	uint8_t *_addr_new;
	uint8_t *_addr_tramp;
	uint8_t _size_replaced;
	const architecture _arch;

public:
	_detour(const void *addr_target, const void *addr_new, const architecture arch) : _saved_ret_addr(0),
		_addr_target((uint8_t*) addr_target), _addr_new((uint8_t*) addr_new),
		_addr_tramp(NULL), _size_replaced(0), _arch(arch) {}
	~_detour();
	void hook();
	void unhook();
	void jump_to_function();

	const uint8_t* trampoline() const {
		return _addr_tramp;
	}
};

template<typename F, typename... args>
class detour {
protected:
	F _f;
	_detour _det;

public:
	detour(void *addr, void *new_addr, architecture arch) : _det(addr, new_addr, arch), _f((F) NULL) {
	}

	typename std::result_of<F(args...)>::type call_original(args&&... a)
	{
		return _f(std::forward<args>(a)...);
	}

	void hook() {
		_det.hook();
		_f = (F) _det.trampoline();
	}

	void unhook() {
		_f = (F) NULL;
		_det.unhook();
	}

};
#endif
