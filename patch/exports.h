/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _EXPORTS_H_
#define _EXPORTS_H_

#include "defines.h"
#include "func.h"
#include "decomp.h"
#include "disasm.h"

DLLEXP func* create_func(const void *addr, architecture arch);
DLLEXP basicblock* create_basicblock(const void *addr, basicblock *prebb = NULL);
DLLEXP opcode* create_opcode(const void *addr, architecture arch);

#endif
