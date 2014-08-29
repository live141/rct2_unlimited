#include "memcatch.h"
#include "page.h"
#include "disasm_x86.h"
#include <stdint.h>
#include <iostream>
#if defined(linux) || defined(__APPLE__)
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#define _XOPEN_SOURCE
#include <ucontext.h>
#else
#endif

std::map<void*, memcatch*> memcatch::_map;

#if defined(linux) || defined(__APPLE__)
void sig_handler(int sig, siginfo_t *si, void *unused) {
	uint64_t val;
	memcatch *mem;
	memcatch_action action, action_req;
	ucontext_t *u = (ucontext_t *) unused;
	void *pc = (void*) u->uc_mcontext->__ss.__rip;
	opcode_x86 op(pc, mode_64);
	op.decode();
	std::cout << "Received SIGSEGV/SIGBUS caused by \"" << op.expression() << "\" at 0x" << std::hex << pc
		<< " for ";
	/* check if we are reading, when yes then first operand is a register */
	if(op.optype(0) == OPERAND_TYPE_REG32 || op.optype(0) == OPERAND_TYPE_REG8 || op.optype(0) == OPERAND_TYPE_AL
		|| op.optype(0) == OPERAND_TYPE_AH || op.optype(0) == OPERAND_TYPE_AX || op.optype(0) == OPERAND_TYPE_EAX
		|| op.optype(0) == OPERAND_TYPE_RAX || op.optype(0) == OPERAND_TYPE_DX || op.optype(0) == OPERAND_TYPE_EDX
		|| op.optype(0) == OPERAND_TYPE_RDX) {
		std::cout << "reading from";
		action = memcatch_read;
	}
	else {
		std::cout << "writing to";
		action = memcatch_write;
	}
	std::cout << " 0x" << si->si_addr << std::dec << std::endl;
	u->uc_mcontext->__ss.__rip = (uint64_t) pc + op.size();
	/* get corresponding memcatch and check if we caused this signal */
	mem = memcatch::find(si->si_addr);
	/* we did not caused it */
	if(mem == NULL) {
		exit(-1);
	}

	/* change page permission to allow callback to do stuff */
	mem->deactivate();
	/* fill val with the value that the codes uses */
	/* TODO */
	if(action == memcatch_read) {

	}
	else if(action == memcatch_write) {

	}
	
	action_req = mem->callback(si->si_addr, action, &val);
	
	/* give signal causing code a value or write to memory */
	/* TODO */
	if(action == memcatch_read) {
		printf("HAHAHA %d\n", *((uint8_t*)u->uc_mcontext->__ss.__rax));
		u->uc_mcontext->__ss.__rax = (u->uc_mcontext->__ss.__rax & ~0xffffffff) | *((uint8_t*)u->uc_mcontext->__ss.__rax);
	}
	else if(action == memcatch_write) {

	}
	mem->activate();
}
#endif

memcatch::~memcatch() {
	memcatch::_map.erase(_addr);
}

memcatch::memcatch(void *addr, size_t size, memcatch_action type, memcatch_callback callback) : _addr(addr), _size(size),
	_saved_flags(0), _type(type), _callback(callback) {
	if(memcatch::find(addr) != NULL) {
		std::cout << "Error: trying to catch 0x" << std::hex << addr << std::dec << "twice." << std::endl;
		exit(-1);
		return;
	}
	/* insert */
	_map[addr] = this;
}

memcatch* memcatch::find(void *addr) {
	memcatch::iterator it;
	for(it = memcatch::_map.begin(); it != memcatch::_map.end(); ++it) {
		if(addr >= it->second->addr() && (uint64_t) addr < (uint64_t) it->second->addr() + it->second->size())
			return it->second;
	}
	return NULL;
}

void memcatch::activate() {
	int prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
	switch(_type) {
		case memcatch_access:
			prot &= ~(PAGE_READ | PAGE_WRITE | PAGE_EXEC);
			break;
		case memcatch_read:
			prot &= ~(PAGE_READ);
			break;
		case memcatch_write:
			prot &= ~(PAGE_WRITE);
			break;
		case memcatch_exec:
			prot &= ~(PAGE_EXEC);
			break;
		default:
			break;
	}
	page::change_permissions(_addr, _size, prot);
}

void memcatch::deactivate() {
	page::change_permissions(_addr, _size, PAGE_READ | PAGE_WRITE | PAGE_EXEC);
}

void memcatch::init() {
#if defined(linux) || defined(__APPLE__)
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sig_handler;
	if (sigaction(SIGSEGV, &sa, NULL) != 0)
		std::cout << "Error: Could not set SIGSEGV handler: " << errno << std::endl;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGBUS, &sa, NULL) != 0)
		std::cout << "Error: Could not set SIGBUS handler: " << errno << std::endl;

#else
#error "TODO"
#endif
}

