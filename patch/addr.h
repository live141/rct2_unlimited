#ifndef _ADDR_H_
#define _ADDR_H_

#if defined(linux) || defined(__APPLE__)
#else
#include <Windows.h>
#endif

struct section_t {
	unsigned long size;
	unsigned char *addr;
};

struct module_t {
#if defined(linux) || defined(__APPLE__)
	void *module;
#else
	HMODULE module;
	section_t section_text;
	section_t section_data;
};
#endif

#endif
