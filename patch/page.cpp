#include "page.h"
#include <iostream>

int page::_page_size = 0;

page::~page() {

}

page::page(void *addr, size_t size) : _addr(NULL), _size(0), _flags(0) {
	_addr = (void*) ((unsigned long) addr & ~(page::_page_size-1));
	_size = size/page::_page_size;
	if(size % page::_page_size)
		++_size;
	_size *= page::_page_size;
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
#error "TODO"
#endif
}

void page::change_permissions(void *addr, size_t size, int flags) {
#if defined(__APPLE__) || defined(linux)
	int prot = 0;
	size_t prot_size = size/page::_page_size;

	if(size % page::_page_size)
		++prot_size;
	prot_size *= page::_page_size;

	if(flags & PAGE_READ)
		prot |= PROT_READ;
	if(flags & PAGE_WRITE)
		prot |= PROT_WRITE;
	if(flags & PAGE_EXEC)
		prot |= PROT_EXEC;
	if(mprotect((void*)((unsigned long) addr & ~(page::_page_size-1)), prot_size, prot ) != 0) {
		std::cout << "Error: Could not change page permissions: " << errno << std::endl; 
	}
#else
#error "TODO"
#endif
}

void* page::alloc() {
#if defined(__APPLE__) || defined(linux)
	void *addr = NULL;
	if((addr = mmap(NULL, page::_page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		addr = NULL;
		std::cout << "Error: Could not map memory: " << errno << std::endl;
	}
	return addr;
#else
#error "TODO"
#endif

}

page* page::alloc(size_t size) {
#if defined(__APPLE__) || defined(linux)
	void *addr = NULL;
	page *p = NULL;
	size_t alloc_size = size/page::_page_size;
	if(size % page::_page_size)
		++alloc_size;
	alloc_size *= page::_page_size;
	
	if((addr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		std::cout << "Error: Could not map memory: " << errno << std::endl;
		return NULL;
	}
	
	p = new page(addr, alloc_size);
	return p;
#else
#error "TODO"
#endif

}

void page::free(void *addr) {
#if defined(__APPLE__) || defined(linux)
	if(addr != NULL) {
		munmap(addr, page::_page_size);
	}
#else
#error "TODO"
#endif
}
