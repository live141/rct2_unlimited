/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _PAGE_H_
#define _PAGE_H

#if defined(__linux__) || defined(__APPLE__)
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#define PAGE_READ 1
#define PAGE_WRITE 2
#define PAGE_EXEC 4

class page {
protected:
	void *_addr;
	size_t _size;
	int _flags;

	static int _page_size;
	
public:
	page();
	~page();
	void change_permissions(int flags);
	static void init();
	static void change_permissions(const void *addr, size_t size, int flags);
	static void* alloc();
	static void* alloc(size_t size);
	static void free(const void *addr);
	
	void* addr() const {
		return _addr;
	}
	
	static int page_size() {
		return page::_page_size;
	}
};

#endif
