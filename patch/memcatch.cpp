#include "memcatch.h"
#include "page.h"
#include "disasm_x86.h"
#include "defines.h"
#include <stdint.h>
#include <iostream>
#include <sstream>
#if defined(linux) || defined(__APPLE__)
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#define _XOPEN_SOURCE
#include <ucontext.h>
#include <i386/eflags.h>
#else
#include <Windows.h>
#endif

std::map<void*, memcatch*> memcatch::_map;

#if defined(linux) || defined(__APPLE__)
void sig_handler(int sig, siginfo_t *si, void *unused) {
	static memcatch *last_mc = NULL;
	memcatch *mem;
	machine_context_x86 context;
	memcatch_action action;
	ucontext_t *u = (ucontext_t *) unused;
	opcode_x86 op((void*) u->uc_mcontext->__ss.__rip, mode_64);
	op.decode();

	debug_printf("Received ");
	switch(sig) {
		case SIGSEGV:
			debug_printf("SIGSEGV");
			break;
		case SIGBUS:
			debug_printf("SIGBUS");
			break;
		case SIGTRAP:
			debug_printf("SIGTRAP");
			break;
	};

	if(sig == SIGTRAP) {
		if(last_mc != NULL) {
			last_mc->activate();
			last_mc = NULL;
			u->uc_mcontext->__ss.__rflags &= ~((uint64_t)EFL_TF);
		}
		return;
	}
	debug_printf(" caused by \"%s\" at 0x%llx, for ", op.expression(), u->uc_mcontext->__ss.__rip);
	/* check if we are reading, when yes then first operand is a register */
	if(op.operand(0).is_register()) {
		debug_printf("reading from");
		action = memcatch_read;
	}
	else {
		debug_printf("writing to");
		action = memcatch_write;
	}
	debug_printf(" 0x%llx\n", si->si_addr);
	
	/* get corresponding memcatch and check if we caused this signal */
	mem = memcatch::find(si->si_addr);
	/* we did not caused it */
	if(mem == NULL) {
		/* check if sideeffect of chaning page permissions */
		mem = memcatch::find_page(si->si_addr);
		if(mem != NULL) {
			//u->uc_mcontext->__ss.__rip += op.size();
			/* change permissions and trap */
			u->uc_mcontext->__ss.__rflags |= EFL_TF;
			last_mc = mem;
			mem->deactivate();
			return;
		}
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
#else
/* Windows */
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS *ExceptionInfo)
{
	static memcatch *last_mc = NULL;
	memcatch *mem;
	void *addr = NULL;
	machine_context_x86 context;
	memcatch_action action;
	PCONTEXT u = ExceptionInfo->ContextRecord;
	opcode_x86 op((void*) u->Eip, mode_32);
	op.decode();
	switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			debug_printf("SIGSEGV");
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			break;
		case EXCEPTION_BREAKPOINT:
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			break;
		case EXCEPTION_FLT_OVERFLOW:
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			break;
		case EXCEPTION_INT_OVERFLOW:
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			break;
		case EXCEPTION_SINGLE_STEP:
			debug_printf("SIGTRAP");
			break;
		case EXCEPTION_STACK_OVERFLOW:
			break;
		default:
			break;
	}

	std::cout << std::endl;

	if(ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
		if(last_mc != NULL) {
			last_mc->activate();
			last_mc = NULL;
			u->EFlags &= ~((uint64_t)0x100);
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	if(ExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) {
		//std::cout << "Error: received exception" << std::endl;
		debug_printf("Error: received exception\n");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	debug_printf(" caused by \"%s\" at 0x%x for ", op.expression(), u->Eip);
	/* check if we are reading, when yes then first operand is a register */
	if(ExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 0) {
		debug_printf("reading from");
		action = memcatch_read;
	}
	else if(ExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 1) {
		debug_printf("writing to");
		action = memcatch_write;
	}
	else {
		debug_printf("\nError: Execution prevention\n");
	}

	addr = (void*) ExceptionInfo->ExceptionRecord->ExceptionInformation[1];
	debug_printf(" 0x%x\n", addr);
	
	/* get corresponding memcatch and check if we caused this signal */
	mem = memcatch::find(addr);
	/* we did not caused it */
	if(mem == NULL) {
		/* check if sideeffect of chaning page permissions */
		mem = memcatch::find_page(addr);
		if(mem != NULL) {
			/* change permissions and trap */
			u->EFlags |= 0x100;
			last_mc = mem;
			mem->deactivate();
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}
	context.rax = (reg_t*)  &u->Eax;
	context.rbx = (reg_t*)  &u->Ebx;
	context.rcx = (reg_t*)  &u->Ecx;
	context.rdx = (reg_t*)  &u->Edx;
	context.rdi = (reg_t*)  &u->Edi;
	context.rsi = (reg_t*)  &u->Esi;
	context.rbp = (reg_t*)  &u->Ebp;
	context.rsp = (reg_t*)  &u->Esp;
	context.rip = (reg_t*)  &u->Eip;
	context.rflags = (reg_t*)  &u->EFlags;
	context.cs = (reg_t*)  &u->SegCs;
	context.fs = (reg_t*)  &u->SegFs;
	context.gs = (reg_t*)  &u->SegGs;
	mem->callback(&op, addr, action, &context);
	return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

memcatch::~memcatch() {
	memcatch::_map.erase(_addr);
}

memcatch::memcatch(void *addr, size_t size, memcatch_action type, memcatch_callback callback) : _addr(addr), _new_addr(NULL), _size(size),
	_saved_flags(0), _type(type), _callback(callback) {
	if(memcatch::find(addr) != NULL) {
		debug_printf("Error: trying to catch 0x%llx twice\n", (uint64_t) addr);
		assert(false);
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

memcatch* memcatch::find_page(void *addr) {
	memcatch::iterator it;
	for(it = memcatch::_map.begin(); it != memcatch::_map.end(); ++it) {
		/* check if is one of the pages */
		if(((uint64_t) addr & ~(page::page_size()-1)) >= ((uint64_t) it->second->addr() & ~(page::page_size()-1)) && 
			((uint64_t) addr & ~(page::page_size()-1)) <= (((uint64_t) it->second->addr()+it->second->size()) & ~(page::page_size()-1)))
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
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGTRAP, &sa, NULL) != 0)
		std::cout << "Error: Could not set SIGTRAP handler: " << errno << std::endl;

#else
	SetUnhandledExceptionFilter(windows_exception_handler);
#endif
}

void memcatch::callback(opcode_x86 *op, void *addr, memcatch_action action, machine_context_x86 *context) {
	memcatch_action action_req;
	uint64_t val;
	uint8_t reg;

	/* change page permission to allow callback to do stuff */
	deactivate();
	/* fill val with the value that the codes uses */
	/* TODO */
	if(action == memcatch_read) {
		reg = op->operand(1).base();
	}
	else if(action == memcatch_write) {
		reg = op->operand(0).base();
	}
	debug_printf("Using register: %d", reg);
	
	if(_callback != NULL)
		action_req = _callback(this, addr, action, &val);
	
	/* TODO: change register content and opcode, if patching addresses */
	if(_new_addr) {
		if(action == memcatch_read) {
			context->get(reg).set((uint64_t)_new_addr+(context->get(reg).get()-(uint64_t)_addr));
			debug_printf("Redirected: %d = 0x%llx\n", reg, context->get(reg).get());
		}
		else if(action == memcatch_write) {
			context->get(reg).set((uint64_t)_new_addr+(context->get(reg).get()-(uint64_t)_addr));

		}
	}
	else {
		context->rip->rx += op->size();
	}
	
	/* give signal causing code a value or write to memory */
	/* TODO */
	if(action == memcatch_read) {
	}
	else if(action == memcatch_write) {

	}
	activate();

}
