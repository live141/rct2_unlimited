#ifndef _PAGE_H_
#define _PAGE_H

#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

#define PAGE_READ 1
#define PAGE_WRITE 2
#define PAGE_EXEC 4

class page {
protected:
	void *_addr;
	size_t _size;
	int _flags;

	static int _page_size;
	page();
	
public:
	page(void *addr, size_t size);
	~page();
	void change_permissions(int flags);
	static void init();
	static void change_permissions(void *addr, size_t size, int flags);
	static void* alloc();
	static void* alloc(size_t size);
	static void free(void *addr);
	
	void* addr() const {
		return _addr;
	}
	
	static int page_size() {
		return page::_page_size;
	}
};

#endif
