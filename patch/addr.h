#ifndef _ADDR_H_
#define _ADDR_H_

#include <Windows.h>

struct section_t {
	unsigned long size;
	unsigned char *addr;
};

struct module_t {
	HANDLE module;
	section_t section_text;
	section_t section_data;
};

#endif
