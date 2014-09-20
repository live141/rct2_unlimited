/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <assert.h>

#define TOSTRING(x) #x

#ifdef DEBUG
#undef NDEBUG
#define debug_printf printf
#else
#define NDEBUG
#define debug_printf
#endif

#if defined(__APPLE__) || defined(linux)
#define DLLEXP extern "C"
#else
#define DLLEXP extern "C" __declspec(dllexport)
#endif

#endif
