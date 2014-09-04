#include "memcatch.h"
#include "page.h"
#include "disasm_x86.h"
#include <stdint.h>
#include <iostream>
#include <sstream>
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
	memcatch *mem;
	machine_context_x86 context;
	memcatch_action action;
	ucontext_t *u = (ucontext_t *) unused;
	opcode_x86 op((void*) u->uc_mcontext->__ss.__rip, mode_64);
	op.decode();
#ifdef DEBUG
	std::cout << "Received SIGSEGV/SIGBUS caused by \"" << op.expression() << "\" at 0x" << std::hex << u->uc_mcontext->__ss.__rip
		<< " for ";
#endif
	/* check if we are reading, when yes then first operand is a register */
	if(op.is_op_register(0)) {
#ifdef DEBUG
		std::cout << "reading from";
#endif
		action = memcatch_read;
	}
	else {
#ifdef DEBUG
		std::cout << "writing to";
#endif
		action = memcatch_write;
	}
#ifdef DEBUG
	std::cout << " 0x" << si->si_addr << std::dec << std::endl;
#endif
	/* get corresponding memcatch and check if we caused this signal */
	mem = memcatch::find(si->si_addr);
	/* we did not caused it */
	if(mem == NULL) {
		exit(-1);
	}
	uint64_t *reg;
	reg_t **dst;
	reg = &u->uc_mcontext->__ss.__rax;
	dst = &context.rax;
	for(int i = 0; i < sizeof(machine_context_x86)/sizeof(uint64_t); ++i) {
		dst[i] = (reg_t*) reg+i;
	}
	mem->callback(&op, si->si_addr, action, &context);
}
#endif

memcatch::~memcatch() {
	memcatch::_map.erase(_addr);
}

memcatch::memcatch(void *addr, size_t size, memcatch_action type, memcatch_callback callback) : _addr(addr), _new_addr(NULL), _size(size),
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

memcatch_action memcatch::callback(opcode_x86 *op, void *addr, memcatch_action action, machine_context_x86 *context) {
	memcatch_action action_req;
	uint64_t val;
#if 0
	printf("%d:\n", op->size());
	for(int i = 0; i < op->size(); ++i) {
		printf("%x ", *((uint8_t*) op->addr()+i));
	}
	printf("\n");
#endif
	/* change page permission to allow callback to do stuff */
	deactivate();
	/* fill val with the value that the codes uses */
	/* TODO */
	if(action == memcatch_read) {

	}
	else if(action == memcatch_write) {

	}
	
	if(_callback != NULL)
		action_req = _callback(this, addr, action, &val);
	
	/* TODO: change register content and opcode, if patching addresses */
#if 1
	if(_new_addr) {
		if(action == memcatch_read) {
			int i, j;
			char reg[7];
			for(i = 0; op->expression()[i] != ','; ++i);
			i += 3;
			for(j = 0; op->expression()[i] != ']' && op->expression()[i] != '+' && op->expression()[i] != '*' && op->expression()[i] != '-'; ++i, ++j) {
				reg[j] = op->expression()[i];
			}
			reg[j] = 0;
#ifdef DEBUG
			std::cout << "Using register: " << reg << std::endl;
#endif
			context->get(reg).set((uint64_t)_new_addr+(context->get(reg).get()-(uint64_t)_addr));
			
		}
		else if(action == memcatch_write) {
			int i, j;
			char reg[7];
			for(i = 0; op->expression()[i] != ' '; ++i);
			i += 2;
			for(j = 0; op->expression()[i] != ']' && op->expression()[i] != '+' && op->expression()[i] != '*' && op->expression()[i] != '-'; ++i, ++j) {
				reg[j] = op->expression()[i];
			}
			reg[j] = 0;
#ifdef DEBUG
			std::cout << "Using register: " << reg << std::endl;
#endif
			context->get(reg).set((uint64_t)_new_addr+(context->get(reg).get()-(uint64_t)_addr));

		}
	}
	else {
		context->rip->rx += op->size();
	}
#else
	context->rip->rx += op->size();
#endif
	/* give signal causing code a value or write to memory */
	/* TODO */
	if(action == memcatch_read) {
	}
	else if(action == memcatch_write) {

	}
	activate();

}
