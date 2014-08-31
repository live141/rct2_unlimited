#include "page.h"
#include <stdint.h>
#include <iostream>

int page::_page_size = 0;

page::~page() {
	page::free(_addr);
}

page::page(size_t size) : _addr(NULL), _size(0), _flags(0) {
	_size = size/page::_page_size;
	if(size % page::_page_size)
		++_size;
	_size *= page::_page_size;
	_addr = page::alloc(size);
}

void page::init() {
	page::_page_size = getpagesize();
}

void page::change_permissions(int flags) {
	_flags = flags;
#if defined(__APPLE__) || defined(linux)
	int prot = 0;
	if(flags & PAGE_READ)
		prot |= PROT_READ;
	if(flags & PAGE_WRITE)
		prot |= PROT_WRITE;
	if(flags & PAGE_EXEC)
		prot |= PROT_EXEC;
	if(mprotect(_addr, _size, prot ) != 0) {
		std::cout << "Error: Could not change page permissions: " << errno << std::endl; 
	}
#else
	uint32_t old_prot, prot = PAGE_NOACCESS;
	if(flags & PAGE_READ && flags & PAGE_WRITE && flags & PAGE_EXEC)
		prot = PAGE_EXECUTE_READWRITE;
	else if(flags & PAGE_READ && flags & PAGE_EXEC)
		prot = PAGE_EXECUTE_READ;
	else if(flags & PAGE_READ && PAGE_WRITE)
		prot = PAGE_READWRITE;
	else if(flags & PAGE_READ)
		prot = PAGE_READONLY;
	else if(flags & PAGE_WRITE)
		prot = PAGE_WRITEONLY;
	
	if(!VirtualProtect(_addr, _size, prot, &old_prot))
		std::cout << "Error: Could not change page permissions" << std::endl; 
#endif
}

void page::change_permissions(void *addr, size_t size, int flags) {
	void *aligned_addr = (void*)((unsigned long) addr & ~(page::_page_size-1));
	size_t prot_size = ((uint64_t) addr + size - (uint64_t) aligned_addr);
	prot_size = prot_size/page::_page_size + (prot_size%page::_page_size)?1:0;
	prot_size *= page::_page_size;
#if defined(__APPLE__) || defined(linux)
	int prot = 0;
	if(flags & PAGE_READ)
		prot |= PROT_READ;
	if(flags & PAGE_WRITE)
		prot |= PROT_WRITE;
	if(flags & PAGE_EXEC)
		prot |= PROT_EXEC;
	if(mprotect(aligned_addr, prot_size, prot ) != 0) {
		std::cout << "Error: Could not change page permissions: " << errno << std::endl; 
	}
#else
	uint32_t old_prot, prot = PAGE_NOACCESS;
	if(flags & PAGE_READ && flags & PAGE_WRITE && flags & PAGE_EXEC)
		prot = PAGE_EXECUTE_READWRITE;
	else if(flags & PAGE_READ && flags & PAGE_EXEC)
		prot = PAGE_EXECUTE_READ;
	else if(flags & PAGE_READ && PAGE_WRITE)
		prot = PAGE_READWRITE;
	else if(flags & PAGE_READ)
		prot = PAGE_READONLY;
	else if(flags & PAGE_WRITE)
		prot = PAGE_WRITEONLY;
	
	if(!VirtualProtect(aligned_addr, prot_size, prot, &old_prot))
		std::cout << "Error: Could not change page permissions" << std::endl; 

#endif
}

void* page::alloc() {
	void *addr = NULL;
#if defined(__APPLE__) || defined(linux)
	if((addr = mmap(NULL, page::_page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		addr = NULL;
		std::cout << "Error: Could not map memory: " << errno << std::endl;
	}
#else
	addr = malloc(page::_page_size);
	change_permissions(addr, page::_page_size, PAGE_READ | PAGE_WRITE | PAGE_EXEC);
#endif
	return addr;
}

void* page::alloc(size_t size) {
	void *addr = NULL;
	size_t alloc_size = size/page::_page_size;
	if(size % page::_page_size)
		++alloc_size;
	alloc_size *= page::_page_size;
	
#if defined(__APPLE__) || defined(linux)
	if((addr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		std::cout << "Error: Could not map memory: " << errno << std::endl;
		return NULL;
	}
#else
	addr = malloc(alloc_size);
	change_permissions(addr, alloc_size, PAGE_READ | PAGE_WRITE | PAGE_EXEC);
#endif
	return addr;
}

void page::free(void *addr) {
	if(addr != NULL) {
#if defined(__APPLE__) || defined(linux)
		munmap(addr, page::_page_size);
#else
		free(addr);
#endif
	}
}
