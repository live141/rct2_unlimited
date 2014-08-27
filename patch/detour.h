#ifndef _DETOUR_H_
#define _DETOUR_H_

#include "disasm_x86.h"
#include <vector>

class detour {
protected:
	std::vector<opcode_x86> _vec_opcode;
	uint64_t _saved_ret_addr;
	uint8_t *_addr_target;
	uint8_t *_addr_new;
	uint8_t *_addr_tramp;
	uint8_t _size_replaced;
	x86_bitmode _bitmode;

	void _change_page_permissions(void *addr);
	void* _alloc_mem(size_t size);
	void _free_mem(void *addr);

public:
	detour(void *addr_target, void *addr_new, x86_bitmode bitmode) : _saved_ret_addr(0), _addr_target((uint8_t*) addr_target), _addr_new((uint8_t*) addr_new),
	_addr_tramp(NULL), _size_replaced(0), _bitmode(bitmode) {}
	~detour();
	void hook();
	void unhook();
	void jump_to_function();

	uint8_t* trampoline() const {
		return _addr_tramp;
	}
};

#endif
