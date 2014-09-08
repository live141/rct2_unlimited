/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

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
#endif
	section_t section_text;
	section_t section_data;
};

#endif
