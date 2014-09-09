/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _MEMCATCH_H_
#define _MEMCATCH_H_

#include "disasm.h"
#include <stdlib.h>
#include <map>
#include <limits>

class memcatch;

enum memcatch_action {
	memcatch_none = 0,
	memcatch_access,
	memcatch_read,
	memcatch_write,
	memcatch_exec,
	memcatch_overwrite
};

typedef memcatch_action (*memcatch_callback)(memcatch *mem, void *addr, memcatch_action action, uint64_t *val);

class memcatch {
protected:
	void *_addr, *_new_addr;
	size_t _size;
	unsigned long _saved_flags;
	memcatch_action _type;
	memcatch_callback _callback;

	memcatch() {}

	static std::map<void*, memcatch*> _map;

public:
	typedef std::map<void*, memcatch*>::iterator iterator;

	memcatch(void *addr, size_t size, memcatch_action type, memcatch_callback callback);
	~memcatch();
	static void init();
	void activate();
	void deactivate();
	static memcatch* find(void *addr);
	static memcatch* find_page(void *addr);
	void callback(opcode *op, void *addr, memcatch_action action, machine_context *context);

	size_t size() const {
		return _size;
	}
	
	void* addr() const {
		return _addr;
	}

	void* new_addr() const {
		return _new_addr;
	}

	void patch_addr(void *addr) {
		_new_addr = addr;
	}
	
	static iterator begin() {
		return memcatch::_map.begin();
	}
	
	static iterator end() {
		return memcatch::_map.end();
	}
};

#endif
