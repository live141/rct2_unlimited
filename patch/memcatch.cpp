/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "memcatch.h"
#include "page.h"
#include "disasm_x86.h"
#include "defines.h"
#include <memory>
#include <stdint.h>
#include <iostream>
#include <sstream>
#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#else
#include <Windows.h>
#endif

std::map<void*, memcatch*> memcatch::_map;

#if defined(__linux__) || defined(__APPLE__)
void sig_handler(int sig, siginfo_t *si, void *unused) {
	static memcatch *last_mc = NULL;
	memcatch *mem;
	std::shared_ptr<machine_context> context(machine_context::create(unused, ARCH));
	memcatch_action action;
	std::shared_ptr<opcode> op(opcode::create((void*) context->pc(), ARCH));
	op->decode();

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
			context->clear_trapflag();
		}
		return;
	}
	debug_printf(" caused by \"%s\" at 0x%lx, for ", op->expression(), context->pc());
	/* check if we are reading, when yes then first operand is a register */
	if(op->get_operand(0)->is_register()) {
		debug_printf("reading from");
		action = memcatch_read;
	}
	else {
		debug_printf("writing to");
		action = memcatch_write;
	}
	debug_printf(" 0x%lx\n", (unsigned long) si->si_addr);
	
	/* get corresponding memcatch and check if we caused this signal */
	mem = memcatch::find(si->si_addr);
	/* we did not caused it */
	if(mem == NULL) {
		/* check if sideeffect of chaning page permissions */
		mem = memcatch::find_page(si->si_addr);
		if(mem != NULL) {
			/* change permissions and trap */
			context->set_trapflag();
			last_mc = mem;
			mem->deactivate();
			return;
		}
		exit(-1);
	}
	mem->callback(op.get(), si->si_addr, action, context.get());
}
#else
/* Windows */
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS *ExceptionInfo)
{
	static memcatch *last_mc = NULL;
	memcatch *mem;
	void *addr = NULL;
	std::shared_ptr<machine_context> context(machine_context::create(ExceptionInfo->ContextRecord, ARCH));
	memcatch_action action;
	std::shared_ptr<opcode> op(opcode::create((void*) context->pc(), ARCH));
	op->decode();
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
			context->clear_trapflag();
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	if(ExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) {
		//std::cout << "Error: received exception" << std::endl;
		debug_printf("Error: received exception\n");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	debug_printf(" caused by \"%s\" at 0x%lx for ", op->expression(), context->pc());
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
			context->set_trapflag();
			last_mc = mem;
			mem->deactivate();
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}
	mem->callback(op.get(), addr, action, context.get());
	return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

memcatch::~memcatch() {
	memcatch::_map.erase(_addr);
}

memcatch::memcatch(void *addr, size_t size, memcatch_action type, memcatch_callback callback) : _addr(addr), _new_addr(NULL), _size(size),
	_saved_flags(0), _type(type), _callback(callback) {
	if(memcatch::find(addr) != NULL) {
		debug_printf("Error: trying to catch 0x%lx twice\n", (unsigned long) addr);
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
		if(addr >= it->second->addr() && (unsigned long) addr < (unsigned long) it->second->addr() + it->second->size())
			return it->second;
	}
	return NULL;
}

memcatch* memcatch::find_page(void *addr) {
	memcatch::iterator it;
	for(it = memcatch::_map.begin(); it != memcatch::_map.end(); ++it) {
		/* check if is one of the pages */
		if(((unsigned long) addr & ~(page::page_size()-1)) >= ((unsigned long) it->second->addr() & ~(page::page_size()-1)) && 
			((unsigned long) addr & ~(page::page_size()-1)) <= (((unsigned long) it->second->addr()+it->second->size()) & ~(page::page_size()-1)))
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
#if defined(__linux__) || defined(__APPLE__)
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

void memcatch::callback(opcode *op, void *addr, memcatch_action action, machine_context *context) {
	memcatch_action action_req;
	uint64_t val;
	uint8_t reg;

	/* change page permission to allow callback to do stuff */
	deactivate();
	/* fill val with the value that the codes uses */
	/* TODO */
	if(action == memcatch_read) {
		reg = op->get_operand(1)->base();
	}
	else if(action == memcatch_write) {
		reg = op->get_operand(0)->base();
	}
	debug_printf("Using register: %d\n", reg);
	
	if(_new_addr) {
		if(action == memcatch_read) {
			context->get(reg).set((unsigned long)_new_addr+(context->get(reg).get()-(unsigned long)_addr));
		}
		else if(action == memcatch_write) {
			context->get(reg).set((unsigned long)_new_addr+(context->get(reg).get()-(unsigned long)_addr));
		}
		debug_printf("Redirected: %d = 0x%lx\n", reg, context->get(reg).get());
	}
	else {
		//context->rip->rx += op->size();
		context->set_pc(context->pc() + op->size());
	}
	
	if(_callback != NULL)
		action_req = _callback(this, addr, action, &val);
	
	/* give signal causing code a value or write to memory */
	/* TODO */
	if(action == memcatch_read) {
	}
	else if(action == memcatch_write) {

	}
	activate();

}
