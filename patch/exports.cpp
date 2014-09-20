/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "exports.h"

DLLEXP func* create_func(const void *addr, architecture arch) {
	return func::create(addr, arch);
}

DLLEXP basicblock* create_basicblock(const void *addr, basicblock *prebb) {
	return basicblock::create(addr, prebb);
}

DLLEXP opcode* create_opcode(const void *addr, architecture arch) {
	return opcode::create(addr, arch);
}
