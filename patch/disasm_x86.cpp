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
	//if(_code->type_op[0] == OPERAND_TYPE_REL8 || _code->type_op[0] == OPERAND_TYPE_REL32) {
	if(_operand[0].type() == OPERAND_TYPE_REL8 || _operand[0].type() == OPERAND_TYPE_REL32) {
		_imm = (uint64_t) val - (uint64_t) _addr - _size;
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

reg machine_context_x86::get(uint8_t reg_num) {
	if(reg_num == REG_EIP || reg_num == REG_RIP)
		return reg(rip, MASK_REG_SIZE(reg_num));

	switch(MASK_REG(reg_num)) {
		case MASK_REG(REG_RAX): return reg(rax, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RBX): return reg(rbx, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RCX): return reg(rcx, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RDX): return reg(rdx, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RDI): return reg(rdi, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RSI): return reg(rsi, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RBP): return reg(rbp, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_RSP): return reg(rsp, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R8Q): return reg(r8, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R9Q): return reg(r9, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R10Q): return reg(r10, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R11Q): return reg(r11, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R12Q): return reg(r12, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R13Q): return reg(r13, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R14Q): return reg(r14, MASK_REG_SIZE(reg_num));
		case MASK_REG(REG_R15Q): return reg(r15, MASK_REG_SIZE(reg_num));
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
	/* determin operand size */
	if(type == 8)
		_operand[i].set_size(1);
	else if(type == 64)
		_operand[i].set_size(8);
	else if(type == 128)
		_operand[i].set_size(16);
	else if(type == 256)
		_operand[i].set_size(32);
	else {
		if(_addr_size_prefix)
			_operand[i].set_size(2);
		else if(_is_opsize64())
			_operand[i].set_size(8);
		else
			_operand[i].set_size(4);
	}
	//if(!_addr_size_prefix) {
		if(_mod == MOD_REG_DIRECT) {
			if(type == 64) {
				/* mm */
				_operand[i]._register = _rm | REG_SIZE_64;
				return std::string(g_lut_mm[_rm]);
			}
			if(type == 128) {
				/* xmm */
				_operand[i]._register = _rm | REG_SIZE_128;
				return std::string(g_lut_xmm[_rm]);
			}
			if(type == 256) {
				/* ymm */
				_operand[i]._register = _rm | REG_SIZE_256;
				return std::string(g_lut_ymm[_rm]);
			}
			if(type == 8) {
				_operand[i]._register = _rm | REG_SIZE_8;
				return std::string(g_lut_registers8[_rm]);
			}
			else if(!_is_opsize64()) {
				_operand[i]._register = _rm | REG_SIZE_32;
				return std::string(g_lut_registers32[_rm]);
			}
			else {
				_operand[i]._register = _rm | REG_SIZE_64;
				return std::string(g_lut_registers64[_rm]);
			}
		}
		else {
			if(_rm == 0x04) {
				/* sib */
				if(_bitmode == mode_32 /*!_is_opsize64()*/) {
					stream << "[" << g_lut_registers32[_base];
					_operand[i]._register = _base | REG_SIZE_32;
				}
				else {
					stream << "[" << g_lut_registers64[_base];
					_operand[i]._register = _base | REG_SIZE_64;
				}
				if(_idx != 0x04) {
					if(_bitmode == mode_32 /*!_is_opsize64()*/) {
						stream << "+" << g_lut_registers32[_idx] << "*" << (int) _scale;
						_operand[i]._register = _idx | REG_SIZE_32;
					}
					else {
						stream << "+" << g_lut_registers64[_idx] << "*" << (int) _scale;
						_operand[i]._register = _idx | REG_SIZE_64;
					}
				}
			}
			else if(_rm == 0x05 && _mod == MOD_REG_INDIRECT) {
				/* disp32 */
				stream << "[";
				if(_bitmode == mode_64) {
					if(_is_opsize64()) {
						stream << "rip";
						_operand[i]._base = REG_RIP;
					}
					else {
						stream << "eip";
						_operand[i]._base = REG_EIP;
					}
				}
				stream << std::dec << std::showpos << _disp;
			}
			else {
				if(_bitmode == mode_32 /*!_is_opsize64()*/) {
					stream << "[" << g_lut_registers32[_rm];
					_operand[i]._base = _rm | REG_SIZE_32;
				}
				else {
					stream << "[" << g_lut_registers64[_rm];
					_operand[i]._base = _rm | REG_SIZE_64;
				}
			}
			if(_mod != MOD_REG_INDIRECT) {
				/* disp */
				stream << std::dec << std::showpos << _disp << "]";
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


void opcode_x86::decode(void *addr, x86_bitmode bitmode) {
	_addr = (uint8_t*) addr;
	_bitmode = bitmode;
	decode();
}

void opcode_x86::decode() {
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
		_operand[i].reset();
		_operand[i].set_type(_code->type_op[i]);
		switch(_code->type_op[i]) {
			case OPERAND_TYPE_REG64:
				stream << ", " << g_lut_registers64[_reg_ope];
				_operand[i].set_size(8);
				_operand[i]._register = _reg_ope | (REG_SIZE_64);
				break;
			case OPERAND_TYPE_REG32:
				stream << ", ";
				if(_op_size_prefix) {
					_operand[i].set_size(2);
					stream << g_lut_registers16[_reg_ope];
					_operand[i]._register = _reg_ope | (REG_SIZE_16);
				}
				else if(!_is_opsize64()) {
					stream << g_lut_registers32[_reg_ope];
					_operand[i].set_size(4);
					_operand[i]._register = _reg_ope | (REG_SIZE_32);
				}
				else {
					stream << g_lut_registers64[_reg_ope];
					_operand[i].set_size(8);
					_operand[i]._register = _reg_ope | (REG_SIZE_64);
				}
				break;
			case OPERAND_TYPE_REG8:
				stream << ", " << g_lut_registers8[_reg_ope];
				_operand[i]._register = _reg_ope | (REG_SIZE_8);
				_operand[i].set_size(1);
				break;
			case OPERAND_TYPE_IMM32:
				stream << ", 0x" << _imm;
				if(_op_size_prefix) {
					_operand[i].set_size(2);
				}
				else if(!_is_opsize64()) {
					_operand[i].set_size(4);
				}
				else {
					_operand[i].set_size(8);
				}
				break;
			case OPERAND_TYPE_IMM8:
				stream << ", 0x" << _imm;
				_operand[i].set_size(1);
				break;
			case OPERAND_TYPE_RM8:
				stream << ", " << _format_modrm(8, i);
				break;
			case OPERAND_TYPE_RM32:
				stream << ", " << _format_modrm(32, i);
				break;
			case OPERAND_TYPE_AL:
				stream << ", al";
				_operand[i].set_size(1);
				_operand[i]._register = REG_AL;
				break;
			case OPERAND_TYPE_AX:
				stream << ", ax";
				_operand[i].set_size(2);
				_operand[i]._register = REG_AX;
				break;
			case OPERAND_TYPE_EAX:
				if(!_op_size_prefix) {
					stream << ", eax";
					_operand[i].set_size(4);
					_operand[i]._register = REG_EAX;
				}
				else {
					stream << ", ax";
					_operand[i].set_size(2);
					_operand[i]._register = REG_AX;
				}
				break;
			case OPERAND_TYPE_RAX:
				if(_is_opsize64()) {
					stream << ", rax";
					_operand[i].set_size(8);
					_operand[i]._register = REG_RAX;
				}
				else if(_op_size_prefix) {
					stream << ", ax";
					_operand[i].set_size(2);
					_operand[i]._register = REG_AX;
				}
				else {
					stream << ", eax";
					_operand[i].set_size(4);
					_operand[i]._register = REG_EAX;
				}
				break;
			case OPERAND_TYPE_DX:
				stream << ", dx";
				_operand[i].set_size(2);
				_operand[i]._register = REG_DX;
				break;
			case OPERAND_TYPE_EDX:
				if(!_op_size_prefix) {
					stream << ", edx";
					_operand[i].set_size(4);
					_operand[i]._register = REG_EDX;
				}
				else {
					stream << ", dx";
					_operand[i].set_size(2);
					_operand[i]._register = REG_DX;
				}
				break;
			case OPERAND_TYPE_RDX:
				if(_is_opsize64()) {
					stream << ", rdx";
					_operand[i].set_size(8);
					_operand[i]._register = REG_RDX;
				}
				else if(_op_size_prefix) {
					stream << ", dx";
					_operand[i].set_size(2);
					_operand[i]._register = REG_DX;
				}
				else {
					stream << ", edx";
					_operand[i].set_size(4);
					_operand[i]._register = REG_EDX;
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
				break;
			case OPERAND_TYPE_MOFFSET32:
				stream << ", " << "[0x"<< _disp << "]";
				break;
			case OPERAND_TYPE_XMM:
				if(_op_size_prefix) {
					stream << ", " << g_lut_xmm[_reg_ope];
					_operand[i].set_size(32);
					_operand[i]._register = _reg_ope | REG_SIZE_128;
				}
				else {
					stream << ", " << g_lut_mm[_reg_ope];
					_operand[i].set_size(16);
					_operand[i]._register = _reg_ope | REG_SIZE_128;
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
		}
	}

	for(int i = 0; i < 1; ++i) {
		if(_operand[i].size() == 8)
			_name.append("q");
		if(_operand[i].size() == 4)
			_name.append("l");
		if(_operand[i].size() == 2)
			_name.append("w");
		if(_operand[i].size() == 1)
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
		tmp += stream.str().c_str()[i];
		if(stream.str().c_str()[i] == ',' || stream.str().c_str()[i] == '\0') {
			tmp += '\0';
			if(j != -1)
				_operand[j].set_expr(tmp);
			tmp = std::string();
			++j;
		}
	}
}

