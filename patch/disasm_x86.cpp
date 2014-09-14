/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "disasm_x86.h"
#include "page.h"
#include "defines.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#if defined(__APPLE__) || defined(__linux__)
#define _XOPEN_SOURCE
#include <ucontext.h>
#if defined(__APPLE__)
#include <i386/eflags.h>
#else
#include <asm/processor-flags.h>
#endif
#endif

extern opcode_x86_t g_opcode_32_1b[];
extern opcode_x86_t g_opcode_32_2b[];
extern opcode_x86_t g_opcode_64_1b[];
extern opcode_x86_t g_opcode_64_2b[];

char g_lut_registers8[][7] = {
	"al",
	"cl",
	"dl",
	"bl",
	"ah",
	"ch",
	"dh",
	"bh",
	"r8b",
	"r9b",
	"r10b",
	"r11b",
	"r12b",
	"r13b",
	"r14b",
	"r15b"

};

char g_lut_registers16[][7] = {
	"ax",
	"cx",
	"dx",
	"bx",
	"sp",
	"bp",
	"si",
	"di",
	"r8w",
	"r9w",
	"r10w",
	"r11w",
	"r12w",
	"r13w",
	"r14w",
	"r15w"

	
};

char g_lut_registers32[][7] = {
	"eax",
	"ecx",
	"edx",
	"ebx",
	"esp",
	"ebp",
	"esi",
	"edi",
	"r8d",
	"r9d",
	"r10d",
	"r11d",
	"r12d",
	"r13d",
	"r14d",
	"r15d"

};

char g_lut_registers64[][7] = {
	"rax",
	"rcx",
	"rdx",
	"rbx",
	"rsp",
	"rbp",
	"rsi",
	"rdi",
	"r8",
	"r9",
	"r10",
	"r11",
	"r12",
	"r13",
	"r14",
	"r15"
};

char g_lut_mm[][7] = {
	"mm0",
	"mm1",
	"mm2",
	"mm3",
	"mm4",
	"mm5",
	"mm6",
	"mm7",
	"mm8",
	"mm9",
	"mm10",
	"mm11",
	"mm12",
	"mm13",
	"mm14",
	"mm15"
};

char g_lut_xmm[][7] = {
	"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7",
	"xmm8",
	"xmm9",
	"xmm10",
	"xmm11",
	"xmm12",
	"xmm13",
	"xmm14",
	"xmm15"
};

char g_lut_ymm[][7] = {
	"ymm0",
	"ymm1",
	"ymm2",
	"ymm3",
	"ymm4",
	"ymm5",
	"ymm6",
	"ymm7",
	"ymm8",
	"ymm9",
	"ymm10",
	"ymm11",
	"ymm12",
	"ymm13",
	"ymm14",
	"ymm15"
};

void opcode_x86::set_imm(int64_t val) {
	_imm = val;
	if(_operand[0]->type() == OPERAND_TYPE_REL8 || _operand[0]->type() == OPERAND_TYPE_REL32) {
		_imm = (unsigned long) val - (unsigned long) _addr - _size;
	}
	/* make page writeable */
	page::change_permissions(((uint8_t*) _addr+_offset_imm), _imm_size, PAGE_READ | PAGE_WRITE | PAGE_EXEC);
	switch(_imm_size) {
		case 1:
			*((int8_t*) ((uint8_t*) _addr+_offset_imm)) = (int8_t) _imm;
			break;
		case 2:
			*((int16_t*) ((uint8_t*) _addr+_offset_imm)) = (int16_t) _imm;
			break;
		case 4:
			*((int32_t*) ((uint8_t*) _addr+_offset_imm)) = (int32_t) _imm;
			break;
		case 8:
			*((int64_t*) ((uint8_t*) _addr+_offset_imm)) = (int64_t) _imm;
			break;
		default:
			break;
	}
}

machine_context_x86::machine_context_x86(const void *cntx) {
#if defined(__APPLE__)
/* APPLE */
	ucontext_t *u = (ucontext_t *) cntx;
#ifdef BIT_64
/* 64 bit */
	rax = (reg_t*)  &u->uc_mcontext->__ss.__rax;
	rbx = (reg_t*)  &u->uc_mcontext->__ss.__rbx;
	rcx = (reg_t*)  &u->uc_mcontext->__ss.__rcx;
	rdx = (reg_t*)  &u->uc_mcontext->__ss.__rdx;
	rdi = (reg_t*)  &u->uc_mcontext->__ss.__rdi;
	rsi = (reg_t*)  &u->uc_mcontext->__ss.__rsi;
	rbp = (reg_t*)  &u->uc_mcontext->__ss.__rbp;
	rsp = (reg_t*)  &u->uc_mcontext->__ss.__rsp;
	rip = (reg_t*)  &u->uc_mcontext->__ss.__rip;
	r8 = (reg_t*) &u->uc_mcontext->__ss.__r8;
	r9 = (reg_t*) &u->uc_mcontext->__ss.__r9;
	r10 = (reg_t*) &u->uc_mcontext->__ss.__r10;
	r11 = (reg_t*) &u->uc_mcontext->__ss.__r11;
	r12 = (reg_t*) &u->uc_mcontext->__ss.__r12;
	r13 = (reg_t*) &u->uc_mcontext->__ss.__r13;
	r14 = (reg_t*) &u->uc_mcontext->__ss.__r14;
	r15 = (reg_t*) &u->uc_mcontext->__ss.__r15;
	rflags = (reg_t*)  &u->uc_mcontext->__ss.__rflags;
	cs = (reg_t*)  &u->uc_mcontext->__ss.__cs;
	fs = (reg_t*)  &u->uc_mcontext->__ss.__fs;
	gs = (reg_t*)  &u->uc_mcontext->__ss.__gs;
#else
/* 32 bit */
	rax = (reg_t*)  &u->uc_mcontext->__ss.__eax;
	rbx = (reg_t*)  &u->uc_mcontext->__ss.__ebx;
	rcx = (reg_t*)  &u->uc_mcontext->__ss.__ecx;
	rdx = (reg_t*)  &u->uc_mcontext->__ss.__edx;
	rdi = (reg_t*)  &u->uc_mcontext->__ss.__edi;
	rsi = (reg_t*)  &u->uc_mcontext->__ss.__esi;
	rbp = (reg_t*)  &u->uc_mcontext->__ss.__ebp;
	rsp = (reg_t*)  &u->uc_mcontext->__ss.__esp;
	rip = (reg_t*)  &u->uc_mcontext->__ss.__eip;
	rflags = (reg_t*)  &u->uc_mcontext->__ss.__eflags;
	cs = (reg_t*)  &u->uc_mcontext->__ss.__cs;
	fs = (reg_t*)  &u->uc_mcontext->__ss.__fs;
	gs = (reg_t*)  &u->uc_mcontext->__ss.__gs;
#endif
#elif defined(__linux__)
/* LINUX */
	ucontext *u = (ucontext*) cntx;
#ifdef BIT_64
/* 64 bit */
	rax = (reg_t*) &u->uc_mcontext.gregs[REG_RAX];
	rbx = (reg_t*) &u->uc_mcontext.gregs[REG_RBX];
	rcx = (reg_t*) &u->uc_mcontext.gregs[REG_RCX];
	rdx = (reg_t*) &u->uc_mcontext.gregs[REG_RDX];
	rdi = (reg_t*) &u->uc_mcontext.gregs[REG_RDI];
	rsi = (reg_t*) &u->uc_mcontext.gregs[REG_RSI];
	rbp = (reg_t*) &u->uc_mcontext.gregs[REG_RBP];
	rsp = (reg_t*) &u->uc_mcontext.gregs[REG_RSP];
	rip = (reg_t*) &u->uc_mcontext.gregs[REG_RIP];
	rflags = (reg_t*) &u->uc_mcontext.gregs[REG_RFL];
	cs = (reg_t*) &u->uc_mcontext.gregs[REG_CS];
	fs = (reg_t*) &u->uc_mcontext.gregs[REG_FS];
	gs = (reg_t*) &u->uc_mcontext.gregs[REG_GS];
#else
/* 32 bit */
	rax = (reg_t*) &u->uc_mcontext.gregs[REG_EAX];
	rbx = (reg_t*) &u->uc_mcontext.gregs[REG_EBX];
	rcx = (reg_t*) &u->uc_mcontext.gregs[REG_ECX];
	rdx = (reg_t*) &u->uc_mcontext.gregs[REG_EDX];
	rdi = (reg_t*) &u->uc_mcontext.gregs[REG_EDI];
	rsi = (reg_t*) &u->uc_mcontext.gregs[REG_ESI];
	rbp = (reg_t*) &u->uc_mcontext.gregs[REG_EBP];
	rsp = (reg_t*) &u->uc_mcontext.gregs[REG_ESP];
	rip = (reg_t*) &u->uc_mcontext.gregs[REG_EIP];
	rflags = (reg_t*) &u->uc_mcontext.gregs[REG_EFL];
	cs = (reg_t*) &u->uc_mcontext.gregs[REG_CS];
	fs = (reg_t*) &u->uc_mcontext.gregs[REG_FS];
	gs = (reg_t*) &u->uc_mcontext.gregs[REG_GS];
#endif
#else
/* WINDOWS */
	PCONTEXT u = (PCONTEXT) cntx;
#ifdef BIT_64
/* 64 bit */
	rax = (reg_t*)  &u->Rax;
	rbx = (reg_t*)  &u->Rbx;
	rcx = (reg_t*)  &u->Rcx;
	rdx = (reg_t*)  &u->Rdx;
	rdi = (reg_t*)  &u->Rdi;
	rsi = (reg_t*)  &u->Rsi;
	rbp = (reg_t*)  &u->Rbp;
	rsp = (reg_t*)  &u->Rsp;
	rip = (reg_t*)  &u->Rip;
	r8 = (reg_t*)  &u->R8;
	r9 = (reg_t*)  &u->R9;
	r10 = (reg_t*)  &u->R10;
	r11 = (reg_t*)  &u->R11;
	r12 = (reg_t*)  &u->R12;
	r13 = (reg_t*)  &u->R13;
	r14 = (reg_t*)  &u->R14;
	r15 = (reg_t*)  &u->R15;
	rflags = (reg_t*)  &u->EFlags;
	cs = (reg_t*)  &u->SegCs;
	fs = (reg_t*)  &u->SegFs;
	gs = (reg_t*)  &u->SegGs;
#else
/* 32 bit */
	rax = (reg_t*)  &u->Eax;
	rbx = (reg_t*)  &u->Ebx;
	rcx = (reg_t*)  &u->Ecx;
	rdx = (reg_t*)  &u->Edx;
	rdi = (reg_t*)  &u->Edi;
	rsi = (reg_t*)  &u->Esi;
	rbp = (reg_t*)  &u->Ebp;
	rsp = (reg_t*)  &u->Esp;
	rip = (reg_t*)  &u->Eip;
	rflags = (reg_t*)  &u->EFlags;
	cs = (reg_t*)  &u->SegCs;
	fs = (reg_t*)  &u->SegFs;
	gs = (reg_t*)  &u->SegGs;
#endif
#endif
}

reg machine_context_x86::get(uint8_t reg_num) {
	if(reg_num == REGISTER_EIP || reg_num == REGISTER_RIP)
		return reg(rip, MASK_REG_SIZE(reg_num));

	switch(MASK_REG(reg_num)) {
		case MASK_REG(REGISTER_RAX): return reg(rax, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RBX): return reg(rbx, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RCX): return reg(rcx, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RDX): return reg(rdx, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RDI): return reg(rdi, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RSI): return reg(rsi, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RBP): return reg(rbp, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_RSP): return reg(rsp, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R8Q): return reg(r8, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R9Q): return reg(r9, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R10Q): return reg(r10, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R11Q): return reg(r11, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R12Q): return reg(r12, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R13Q): return reg(r13, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R14Q): return reg(r14, MASK_REG_SIZE(reg_num));
		case MASK_REG(REGISTER_R15Q): return reg(r15, MASK_REG_SIZE(reg_num));
		default:
			debug_printf("Error: Invalid register number: %d\n", reg_num);
	};
}

reg machine_context_x86::get(const char *name) {
	int i, hash = 0;
	enum reg_size size = size_byte;
	char end = 0;
	for(i = 0; name[i] != 0 && i < 7; ++i) {
		hash += name[i];
	}
	end = name[i-1];

	if(end == 'b' || end == 'h' || end == 'l')
		size = size_byte;
	else if(name[0] == 'e' || end == 'd')
		size = size_dword;
	else if(name[0] != 'r' || end == 'w' )
		size = size_word;
	else
		size = size_qword;

	if(size == size_byte) {
		switch(hash) {
			case 205: return reg(rax, size_low);
			case 207: return reg(rcx, size_low);
			case 208: return reg(rdx, size_low);
			case 206: return reg(rbx, size_low);
			case 201: return reg(rax, size_high);
			case 203: return reg(rcx, size_high);
			case 204: return reg(rdx, size_high);
			case 202: return reg(rbx, size_high);
			case 268: return reg(r8, size_low);
			case 269: return reg(r9, size_low);
			case 309: return reg(r10, size_low);
			case 310: return reg(r11, size_low);
			case 311: return reg(r12, size_low);
			case 312: return reg(r13, size_low);
			case 313: return reg(r14, size_low);
			case 314: return reg(r15, size_low);
		}
	}

	if(size == size_word) {
		switch(hash) {
			case 217: return reg(rax, size_word);
			case 219: return reg(rcx, size_word);
			case 220: return (name[0] == 'd')?reg(rdx, size_word):reg(rsi, size_word);
			case 218: return reg(rbx, size_word);
			case 227: return reg(rsp, size_word);
			case 210: return reg(rbp, size_word);
			//case 220: return reg(rsi, size_word);
			case 205: return reg(rdi, size_word);
			case 289: return reg(r8, size_word);
			case 290: return reg(r9, size_word);
			case 330: return reg(r10, size_word);
			case 331: return reg(r11, size_word);
			case 332: return reg(r12, size_word);
			case 333: return reg(r13, size_word);
			case 334: return reg(r14, size_word);
			case 335: return reg(r15, size_word);
		}
	}

	if(size == size_dword) {
		switch(hash) {
			case 318: return reg(rax, size_dword);
			case 320: return reg(rcx, size_dword);
			case 321: return (name[1] == 'd')?reg(rdx, size_dword):reg(rsi, size_dword);
			case 319: return reg(rbx, size_dword);
			case 328: return reg(rsp, size_dword);
			case 311: return (name[1] == 'b')?reg(rbp, size_dword):reg(r10, size_dword);
			//case 321: return reg(rsi, size_dword);
			case 306: return reg(rdi, size_dword);
			case 270: return reg(r8, size_dword);
			case 271: return reg(r9, size_dword);
			//case 311: return reg(r10, size_dword);
			case 312: return reg(r11, size_dword);
			case 313: return reg(r12, size_dword);
			case 314: return reg(r13, size_dword);
			case 315: return reg(r14, size_dword);
			case 316: return reg(r15, size_dword);
		}
	}

	if(size == size_qword) {
		switch(hash) {
			case 331: return reg(rax, size_qword);
			case 333: return reg(rcx, size_qword);
			case 334: return (name[1] == 'd')?reg(rdx, size_qword):reg(rsi, size_qword);
			case 332: return reg(rbx, size_qword);
			case 341: return reg(rsp, size_qword);
			case 324: return reg(rbp, size_qword);
			//case 334: return reg(rsi, size_qword);
			case 319: return reg(rdi, size_qword);
			case 170: return reg(r8, size_qword);
			case 171: return reg(r9, size_qword);
			case 211: return reg(r10, size_qword);
			case 212: return reg(r11, size_qword);
			case 213: return reg(r12, size_qword);
			case 214: return reg(r13, size_qword);
			case 215: return reg(r14, size_qword);
			case 216: return reg(r15, size_qword);
		}
	}
}

void machine_context_x86::set_trapflag() {
#if defined(__APPLE__)
	set_flags(flags() | ((unsigned long)EFL_TF));
#elif defined(__linux__)
	set_flags(flags() | ((unsigned long)X86_EFLAGS_TF));
#else
/* Windows */
	set_flags(flags() | ((unsigned long)0x100));
#endif
}

void machine_context_x86::clear_trapflag() {
#if defined(__APPLE__)
	set_flags(flags() & ~((unsigned long)EFL_TF));
#elif defined(__linux__)
	set_flags(flags() & ~((unsigned long)X86_EFLAGS_TF));
#else
/* Windows */
	set_flags(flags() & ~((unsigned long)0x100));
#endif
}

void opcode_x86::_decode_modrm(uint8_t byte) {
	_mod = byte >> 6;
	_reg_ope = (byte >> 3) & 0x07;
	_rm = byte & 0x07;

	if(_prefix64 & 0x04)
		_reg_ope |= 0x08;

	/* check if next byte is sib */
	if(!_addr_size_prefix) {
		if(_mod != MOD_REG_DIRECT && _rm == 0x04) {
			_sib = 1;
		}
	}

	if(!_sib && (_prefix64 & 0x01)) {
		_rm |= 0x08;
	}
}

std::string opcode_x86::_format_modrm(uint8_t type, uint8_t i) {
	std::stringstream stream;
	stream << std::hex;
	operand_x86 *op = (operand_x86*) _operand[i];
	/* determin operand size */
	if(type == 8)
		op->set_size(1);
	else if(type == 64)
		op->set_size(8);
	else if(type == 128)
		op->set_size(16);
	else if(type == 256)
		op->set_size(32);
	else {
		if(_op_size_prefix)
			op->set_size(2);
		else if(_is_opsize64())
			op->set_size(8);
		else
			op->set_size(4);
	}
	//if(!_addr_size_prefix) {
		if(_mod == MOD_REG_DIRECT) {
			if(type == 64) {
				/* mm */
				op->set_register(_rm | REG_SIZE_64);
				return std::string(g_lut_mm[_rm]);
			}
			if(type == 128) {
				/* xmm */
				op->set_register(_rm | REG_SIZE_128);
				return std::string(g_lut_xmm[_rm]);
			}
			if(type == 256) {
				/* ymm */
				op->set_register(_rm | REG_SIZE_256);
				return std::string(g_lut_ymm[_rm]);
			}
			if(type == 8) {
				op->set_register(_rm | REG_SIZE_8);
				return std::string(g_lut_registers8[_rm]);
			}
			else if(!_is_opsize64()) {
				op->set_register(_rm | REG_SIZE_32);
				return std::string(g_lut_registers32[_rm]);
			}
			else if(_op_size_prefix) {
				op->set_register(_rm | REG_SIZE_16);
				return std::string(g_lut_registers32[_rm]);
			}
			else {
				op->set_register(_rm | REG_SIZE_64);
				return std::string(g_lut_registers64[_rm]);
			}
		}
		else {
			if(_rm == 0x04) {
				/* sib */
				if(_bitmode == mode_32 /*!_is_opsize64()*/) {
					stream << "[" << g_lut_registers32[_base];
					//op->set_register(_base | REG_SIZE_32);
					op->set_base(_base | REG_SIZE_32);
				}
				else {
					stream << "[" << g_lut_registers64[_base];
					//op->set_register(_base | REG_SIZE_64);
					op->set_base(_base | REG_SIZE_64);
				}
				if(_idx != 0x04) {
					if(_bitmode == mode_32 /*!_is_opsize64()*/) {
						stream << "+" << g_lut_registers32[_idx] << "*" << (int) _scale;
						op->set_index(_idx | REG_SIZE_32);
						op->set_scale(_scale);
					}
					else {
						stream << "+" << g_lut_registers64[_idx] << "*" << (int) _scale;
						op->set_index(_idx | REG_SIZE_64);
						op->set_scale(_scale);
					}
				}
			}
			else if(_rm == 0x05 && _mod == MOD_REG_INDIRECT) {
				/* disp32 */
				stream << "[";
				if(_bitmode == mode_64) {
					if(_is_opsize64()) {
						stream << "rip";
						op->set_base(REGISTER_RIP);
					}
					else {
						stream << "eip";
						op->set_base(REGISTER_EIP);
					}
				}
				stream << std::dec << std::showpos << _disp;
				op->set_disp(_disp);
			}
			else {
				if(_bitmode == mode_32 /*!_is_opsize64()*/) {
					stream << "[" << g_lut_registers32[_rm];
					op->set_base(_rm | REG_SIZE_32);
				}
				else {
					stream << "[" << g_lut_registers64[_rm];
					op->set_base(_rm | REG_SIZE_64);
				}
			}
			if(_mod != MOD_REG_INDIRECT) {
				/* disp */
				stream << std::dec << std::showpos << _disp << "]";
				op->set_disp(_disp);
			}
			else {
				/* no disp */
				stream << "]";
			}
		}
/*
	}
	else {

	}
*/
	return stream.str();
}


void opcode_x86::_decode() {
	if(_addr == NULL)
		return;
	std::stringstream stream;
	uint8_t *byte = _addr;
	uint8_t size = 0;
	_reset();
	
	/* check prefixes first, max 4 bytes */
	for(unsigned int i = 0; i < 4; ++i) {
		switch(*byte) {
			/* Instruction prefix */
			/*
			case PREFIX_REP:
				++size;
				break;
			*/
			case PREFIX_REPZ:
				++size;
				break;
			case PREFIX_REPNZ:
				++size;
				break;
			case PREFIX_LOCK:
				++size;
				break;

			/* Size prefixes */
			case PREFIX_OPERAND_SIZE:
				_op_size_prefix = *byte;
				++size;
				break;
			case PREFIX_ADDR_SIZE:
				_addr_size_prefix = *byte;
				++size;
				break;

			/* Segment override */
			case SEGMENT_OVERRIDE_CS:
				_segment = *byte;
				++size;
				break;
				++size;
			case SEGMENT_OVERRIDE_SS:
				_segment = *byte;
				++size;
				break;
			case SEGMENT_OVERRIDE_DS:
				_segment = *byte;
				++size;
				break;
			case SEGMENT_OVERRIDE_ES:
				_segment = *byte;
				++size;
				break;
			case SEGMENT_OVERRIDE_FS:
				_segment = *byte;
				++size;
				break;
			case SEGMENT_OVERRIDE_GS:
				_segment = *byte;
				++size;
				break;
			/*
			case SEGMENT_OVERRIDE_OPERAND_SIZE:
				++size;
				break;
			case SEGMENT_OVERRIDE_ADDR_SIZE:
				++size;
				break;
			*/
			/* nothing found -> no prefix */
			default:
				i = 4;
				--byte;
				break;
		}
		++byte;
	}

	if(_bitmode == mode_64) {
		/* check for PREFIX */
		if(*byte >= 0x40 && *byte <= 0x4f) {
			_prefix64 = *byte;
			++byte;
			++size;
		}
		
		/* check if opcode is two bytes long */
		if(*byte == TWO_BYTE) {
			++byte;
			++size;
			_code = &g_opcode_64_2b[*byte];
		}
		else {
			_code = &g_opcode_64_1b[*byte];
		}
	}
	else {
		/* check if opcode is two bytes long */
		if(*byte == TWO_BYTE) {
			++byte;
			++size;
			_code = &g_opcode_32_2b[*byte];
		}
		else {
			_code = &g_opcode_32_1b[*byte];
		}
	}

	/* opcode LUT is shifted, since one byte can describe mutliple opcodes
	 * -> search for real opcode
	 * -> check for opcode with respect to opcode extension in mod r/m byte */

	while(_code->opcode != *byte) {
		++_code;
	}
	++size;
	++byte;

	if(_code->size_modrm == 1) {
		_decode_modrm(*byte);
		++byte;
		++size;
	}

	if(_code->opcode_ext != OPCODE_EXT_INVAL) {
		/* search for opcode with opcode extension */
		while(_code->opcode_ext != _reg_ope) {
			++_code;
		}
		if(_code->size_modrm != 1) {
			/* 64bit prefix rex.b specifies extended register */
			if(_prefix64 & 0x01)
				_reg_ope |= 0x08;
		}
	}

	_name = _code->name;
	_instr = _code->type;

	/* check for SIB byte */
	if(_sib) {
		_decode_sib(*byte);
		++byte;
		++size;
	}

	if(_code->size_displacement == 1 || _mod == MOD_REG_INDIRECT_DISP8) {
		_disp = *((int8_t*) byte);
		_disp_size = 1;
		_offset_disp = size;
		++byte;
		++size;
	}	
	else if(_code->size_displacement == 4 || _mod == MOD_REG_INDIRECT_DISP32 || (_mod == MOD_REG_INDIRECT && ((_rm == 0x06 && _addr_size_prefix) || (_rm == 0x05 && !_addr_size_prefix)))) {
		if(_addr_size_prefix) {
			_disp = *((int16_t*) byte);
			_disp_size = 2;
			_offset_disp = size;
			byte += 2;
			size += 2;
		}
		else {
			_disp = *((int32_t*) byte);
			_disp_size = 4;
			_offset_disp = size;
			byte += 4;
			size += 4;
		}
	}

	if(_code->size_immediate == 1) {
		_imm = *((int8_t*) byte);
		_imm_size = 1;
		_offset_imm = size;
		++byte;
		++size;
	}	
	else if(_code->size_immediate == 4) {
		if(_op_size_prefix) {
			_imm = *((int16_t*) byte);
			_imm_size = 2;
		}
		else if(!_is_opsize64()) {
			_imm = *((int32_t*) byte);
			_imm_size = 4;
		}
		else {
			_imm = *((int64_t*) byte);
			_imm_size = 8;
		}
		_offset_imm = size;
		byte += _imm_size;
		size += _imm_size;
	}

	_size = size;

	/* put in string */
	//stream << _name << std::hex;
	stream << std::hex;
	for(int i = 0; i < 4; ++i) {
		_operand[i]->reset();
		_operand[i]->set_type(_code->type_op[i]);
		switch(_code->type_op[i]) {
			case OPERAND_TYPE_REG64:
				stream << ", " << g_lut_registers64[_reg_ope];
				_operand[i]->set_size(8);
				_operand[i]->set_register(_reg_ope | (REG_SIZE_64));
				break;
			case OPERAND_TYPE_REG32:
				stream << ", ";
				if(_op_size_prefix) {
					_operand[i]->set_size(2);
					stream << g_lut_registers16[_reg_ope];
					_operand[i]->set_register(_reg_ope | (REG_SIZE_16));
				}
				else if(!_is_opsize64()) {
					stream << g_lut_registers32[_reg_ope];
					_operand[i]->set_size(4);
					_operand[i]->set_register(_reg_ope | (REG_SIZE_32));
				}
				else {
					stream << g_lut_registers64[_reg_ope];
					_operand[i]->set_size(8);
					_operand[i]->set_register(_reg_ope | (REG_SIZE_64));
				}
				break;
			case OPERAND_TYPE_REG8:
				stream << ", " << g_lut_registers8[_reg_ope];
				_operand[i]->set_register(_reg_ope | (REG_SIZE_8));
				_operand[i]->set_size(1);
				break;
			case OPERAND_TYPE_IMM32:
				stream << ", 0x" << _imm;
				if(_op_size_prefix) {
					_operand[i]->set_size(2);
				}
				else if(!_is_opsize64()) {
					_operand[i]->set_size(4);
				}
				else {
					_operand[i]->set_size(8);
				}
				break;
			case OPERAND_TYPE_IMM8:
				stream << ", 0x" << _imm;
				_operand[i]->set_size(1);
				break;
			case OPERAND_TYPE_RM8:
				stream << ", " << _format_modrm(8, i);
				break;
			case OPERAND_TYPE_RM32:
				stream << ", " << _format_modrm(32, i);
				break;
			case OPERAND_TYPE_AL:
				stream << ", al";
				_operand[i]->set_size(1);
				_operand[i]->set_register(REGISTER_AL);
				break;
			case OPERAND_TYPE_AX:
				stream << ", ax";
				_operand[i]->set_size(2);
				_operand[i]->set_register(REGISTER_AX);
				break;
			case OPERAND_TYPE_EAX:
				if(!_op_size_prefix) {
					stream << ", eax";
					_operand[i]->set_size(4);
					_operand[i]->set_register(REGISTER_EAX);
				}
				else {
					stream << ", ax";
					_operand[i]->set_size(2);
					_operand[i]->set_register(REGISTER_AX);
				}
				break;
			case OPERAND_TYPE_RAX:
				if(_is_opsize64()) {
					stream << ", rax";
					_operand[i]->set_size(8);
					_operand[i]->set_register(REGISTER_RAX);
				}
				else if(_op_size_prefix) {
					stream << ", ax";
					_operand[i]->set_size(2);
					_operand[i]->set_register(REGISTER_AX);
				}
				else {
					stream << ", eax";
					_operand[i]->set_size(4);
					_operand[i]->set_register(REGISTER_EAX);
				}
				break;
			case OPERAND_TYPE_DX:
				stream << ", dx";
				_operand[i]->set_size(2);
				_operand[i]->set_register(REGISTER_DX);
				break;
			case OPERAND_TYPE_EDX:
				if(!_op_size_prefix) {
					stream << ", edx";
					_operand[i]->set_size(4);
					_operand[i]->set_register(REGISTER_EDX);
				}
				else {
					stream << ", dx";
					_operand[i]->set_size(2);
					_operand[i]->set_register(REGISTER_DX);
				}
				break;
			case OPERAND_TYPE_RDX:
				if(_is_opsize64()) {
					stream << ", rdx";
					_operand[i]->set_size(8);
					_operand[i]->set_register(REGISTER_RDX);
				}
				else if(_op_size_prefix) {
					stream << ", dx";
					_operand[i]->set_size(2);
					_operand[i]->set_register(REGISTER_DX);
				}
				else {
					stream << ", edx";
					_operand[i]->set_size(4);
					_operand[i]->set_register(REGISTER_EDX);
				}
				break;
			case OPERAND_TYPE_REL8:
				stream << ", 0x" << immediate();
				break;
			case OPERAND_TYPE_REL32:
				stream << ", 0x" << immediate();
				break;
			case OPERAND_TYPE_MOFFSET8:
				stream << ", [0x" << _disp << "]";
				_operand[i]->set_disp(_disp);
				break;
			case OPERAND_TYPE_MOFFSET32:
				stream << ", " << "[0x"<< _disp << "]";
				_operand[i]->set_disp(_disp);
				break;
			case OPERAND_TYPE_XMM:
				if(_op_size_prefix) {
					stream << ", " << g_lut_xmm[_reg_ope];
					_operand[i]->set_size(32);
					_operand[i]->set_register(_reg_ope | REG_SIZE_128);
				}
				else {
					stream << ", " << g_lut_mm[_reg_ope];
					_operand[i]->set_size(16);
					_operand[i]->set_register(_reg_ope | REG_SIZE_128);
				}
				break;
			case OPERAND_TYPE_XMMM:
				if(_op_size_prefix) {
					stream << ", " << _format_modrm(128, i);
				}
				else {
					stream << ", " << _format_modrm(64, i);
				}
				break;
			default:
				break;

			case OPERAND_TYPE_ESI:
				if(_bitmode == mode_64) {
					stream << ", " << "[rsi]";
					//_operand[i]->set_register(REGISTER_RSI);
					_operand[i]->set_base(REGISTER_RSI);
				}
				else {
					stream << ", " << "[esi]";
					//_operand[i]->set_register(REGISTER_ESI);
					_operand[i]->set_base(REGISTER_ESI);
				}
				break;
			case OPERAND_TYPE_EDI:
				if(_bitmode == mode_64) {
					stream << ", " << "[rdi]";
					//_operand[i]->set_register(REGISTER_RDI);
					_operand[i]->set_base(REGISTER_RDI);
				}
				else {
					stream << ", " << "[edi]";
					//_operand[i]->set_register(REGISTER_EDI);
					_operand[i]->set_base(REGISTER_EDI);
				}
				break;
		}
	}

	for(int i = 0; i < 1; ++i) {
		if(_operand[i]->size() == 8)
			_name.append("q");
		if(_operand[i]->size() == 4)
			_name.append("l");
		if(_operand[i]->size() == 2)
			_name.append("w");
		if(_operand[i]->size() == 1)
			_name.append("b");
	}

	std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);
	_expr = _name;
	_expr += stream.str().c_str()+1;

	/* set operand expressions */
	std::string tmp;
	/* do not forget last operand, terminated by 0x00 */
	for(int i = 0, j = -1; i < stream.str().size()+1; ++i) {
		if(stream.str().c_str()[i] == ' ')
			continue;
		if(stream.str().c_str()[i] == ',' || stream.str().c_str()[i] == '\0') {
			//tmp += '\0';
			if(j != -1)
				_operand[j]->set_expr(tmp);
			tmp = std::string();
			++j;
		}
		else
			tmp += stream.str().c_str()[i];
	}
}

