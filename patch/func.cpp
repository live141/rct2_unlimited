/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "func.h"
#include "func_x86.h"

std::map<const void*, func*> func::_map_function;

func* func::create(const void *addr, architecture arch) {
	std::map<const void*, func*>::iterator it;
	it = _map_function.find(addr);
	if(it != _map_function.end())
		return it->second;
	switch(arch) {
		case arch_x86: return new func_x86(addr, arch, mode_32);
		case arch_x64: return new func_x86(addr, arch, mode_64);
		case arch_arm: return NULL;
		case arch_ppc: return NULL;
		default: return NULL;
	};

}
