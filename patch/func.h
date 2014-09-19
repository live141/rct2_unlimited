/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _FUNC_H_
#define _FUNC_H_

#include "disasm.h"
#include "decomp.h"
#include <vector>
#include <map>

class func {
protected:
	std::vector<const void*> _vec_subroutine;
	std::vector<operand*> _vec_parameter;
	const void *_addr;
	basicblock *_bb;
	architecture _arch;

	static std::map<const void*, func*> _map_function;
	func() {}
	func(const void *addr, architecture arch) : _addr(addr), _bb(NULL), _arch(arch) { _map_function[addr] = this; }
public:
	static func* create(const void *addr, architecture arch);
	virtual void analyze() = 0;

	void free() {
		_map_function.erase(_map_function.find(_addr));
		delete this;
	}

	const std::vector<operand*>& vec_parameter() const {
		return _vec_parameter;
	}

	unsigned int num_parameters() const {
		return _vec_parameter.size();
	}
};

#endif
