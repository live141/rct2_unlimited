/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#define AUTHOR "Fabian Fischer"
#define VERSION "0.1"

#define NAME "WORKING TITLE"
#if defined(linux)
#define NAME_DLL "patch.so"
#elif defined(__APPLE__)
#define NAME_DLL "patch.dylib"
#else
#define NAME_DLL "patch.dll"
#endif

#endif
