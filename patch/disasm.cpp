/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "disasm.h"
#include "disasm_x86.h"

opcode* opcode::create(const void *addr, architecture arch) {
	switch(arch) {
		case arch_x86: return new opcode_x86(addr, mode_32);
		case arch_x64: return new opcode_x86(addr, mode_64);
		case arch_arm: return NULL;
		case arch_ppc: return NULL;
		default: return NULL;
	};
}

machine_context* machine_context::create(const void *cntx, architecture arch) {
	switch(arch) {
		case arch_x86: return new machine_context_x86(cntx);
		case arch_x64: return new machine_context_x86(cntx);
		case arch_arm: return NULL;
		case arch_ppc: return NULL;
		default: return NULL;
	};
}
