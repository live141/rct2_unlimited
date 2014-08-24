#include "disasm_x86.h"
#include <stdio.h>
#include <sstream>

extern opcode_x86_t g_opcode_32_1b[];
extern opcode_x86_t g_opcode_32_2b[];
extern opcode_x86_t g_opcode_64_1b[];
extern opcode_x86_t g_opcode_64_2b[];

char g_lut_registers8[][4] = {
	"al",
	"cl",
	"dl",
	"bl",
	"ah",
	"ch",
	"dh",
	"bh"
};

char g_lut_registers16[][4] = {
	"ax",
	"cx",
	"dx",
	"bx",
	"sp",
	"bp",
	"si",
	"di"
};

char g_lut_registers32[][4] = {
	"eax",
	"ecx",
	"edx",
	"ebx",
	"esp",
	"ebp",
	"esi",
	"edi"
};

char g_lut_registers64[][4] = {
	"rax",
	"rcx",
	"rdx",
	"rbx",
	"rsp",
	"rbp",
	"rsi",
	"rdi"
};

void opcode_x86::_decode_modrm(uint8_t byte) {
	_mod = byte >> 6;
	_reg_ope = (byte >> 3) & 0x07;
	_rm = byte & 0x07;

	/* check if next byte is sib */
	if(!_addr_size_prefix) {
		if(_mod != MOD_REG_DIRECT && _rm == 0x04) {
			_sib = 1;
		}
	}
}

std::string opcode_x86::_format_modrm(uint8_t type) {
	std::stringstream stream;
	stream << std::hex;
	if(!_addr_size_prefix) {
		if(_mod == MOD_REG_DIRECT) {
			if(_bitmode == mode_32)
				return std::string(g_lut_registers32[_rm]);
			else
				return std::string(g_lut_registers64[_rm]);
		}
		else {
			if(_rm == 0x04) {
				/* sib */
				stream << "[" << g_lut_registers32[_base];
				if(_idx != 0x04) {
					if(_bitmode == mode_32)
						stream << "+" << g_lut_registers32[_idx] << "*" << (int) _scale;
					else
						stream << "+" << g_lut_registers64[_idx] << "*" << (int) _scale;
				}
			}
			else if(_rm == 0x05 && _mod == MOD_REG_INDIRECT) {
				/* disp32 */
				stream << "[" << (int) _disp;
			}
			else {
				if(_bitmode == mode_32)
					stream << "[" << g_lut_registers32[_rm];
				else
					stream << "[" << g_lut_registers64[_rm];
			}
			if(_mod != MOD_REG_INDIRECT) {
				/* disp */
				stream << "+" << (int) _disp << "]";
			}
			else {
				/* no disp */
				stream << "]";
			}
		}
	}
	else {

	}
	return stream.str();
}



void opcode_x86::decode() {
	opcode_x86_t *code;
	std::stringstream stream;
	uint8_t *byte = _addr;
	uint8_t size = 0;

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
			code = &g_opcode_64_2b[*byte];
		}
		else {
			code = &g_opcode_64_1b[*byte];
		}
	}
	else {
		/* check if opcode is two bytes long */
		if(*byte == TWO_BYTE) {
			++byte;
			++size;
			code = &g_opcode_32_2b[*byte];
		}
		else {
			code = &g_opcode_32_1b[*byte];
		}
	}

	/* opcode LUT is shifted, since one byte can describe mutliple opcodes
	 * -> search for real opcode
	 * -> check for opcode with respect to opcode extension in mod r/m byte */

	while(code->opcode != *byte) {
		++code;
	}
	++size;
	++byte;
	
	if(code->size_modrm == 1) {
		_decode_modrm(*byte);
		++byte;
		++size;
	}

	if(code->opcode_ext != OPCODE_EXT_INVAL) {
		/* search for opcode with opcode extension */
		while(code->opcode_ext != _reg_ope) {
			++code;
		}
	}

	_name = code->name;

	/* check for SIB byte */
	if(_sib) {
		_decode_sib(*byte);
		++byte;
		++size;
	}

#if 0
	if(/*code->size_sib == 1 &&*/ _mod != MOD_REG_DIRECT && _rm == 0x04) {
		/* check if it is only displacement */
		if((addr_size_div == 2 && _rm != 0x06) || (addr_size_div == 1 && _rm != 0x05)) {
			_decode_sib(*byte);
			++byte;
			++size;
		}
	}
#endif
	if(code->size_displacement == 1 || _mod == MOD_REG_INDIRECT_DISP8) {
		_disp = *byte;
		_disp_size = 1;
		++byte;
		++size;
	}	
	else if(code->size_displacement == 4 || _mod == MOD_REG_INDIRECT_DISP32 || (_mod == MOD_REG_INDIRECT && (_rm == 0x06 && _addr_size_prefix) || (_rm == 0x05 && !_addr_size_prefix))) {
		if(_addr_size_prefix) {
			_disp = *((int16_t*) byte);
			_disp_size = 2;
			byte += 2;
			size += 2;
		}
		else {
			_disp = *((int32_t*) byte);
			_disp_size = 4;
			byte += 4;
			size += 4;
		}
	}

	if(code->size_immediate == 1) {
		_imm = *byte;
		_imm_size = 1;
		++byte;
		++size;
	}	
	else if(code->size_immediate == 4) {
		if(_op_size_prefix) {
			_imm = *((int16_t*) byte);
			_imm_size = 2;
			byte += 2;
			size += 2;
		}
		else {
			_imm = *((int32_t*) byte);
			_imm_size = 4;
			byte += 4;
			size += 4;
		}
	}

	_size = size;

	/* put in string */
	stream << _name << std::hex;
	for(int i = 0; i < 4; ++i) {
		switch(code->type_op[i]) {
			case OPERAND_TYPE_REG32:
				stream << ", ";
				if(_op_size_prefix)
					stream << g_lut_registers16[_reg_ope];
				else {
					if(_bitmode == mode_32)
						stream << g_lut_registers32[_reg_ope];
					else
						stream << g_lut_registers64[_reg_ope];
				}
				break;
			case OPERAND_TYPE_REG8:
				stream << ", " << g_lut_registers8[_reg_ope];
				break;
			case OPERAND_TYPE_IMM32:
				stream << ", " << (int) _imm;
				break;
			case OPERAND_TYPE_IMM8:
				stream << ", " << (int) _imm;
				break;
			case OPERAND_TYPE_RM8:
				stream << ", " << _format_modrm(8);
				break;
			case OPERAND_TYPE_RM32:
				stream << ", " << _format_modrm(32);
				break;
			case OPERAND_TYPE_AL:
				stream << ", al";
				break;
			case OPERAND_TYPE_AX32:
				stream << ", ax";
				break;
			case OPERAND_TYPE_REL8:
				stream << ", " << (int) _imm;
				break;
			case OPERAND_TYPE_REL32:
				stream << ", " << (int) _imm;
				break;
			case OPERAND_TYPE_MOFFSET8:
				stream << ", [" << (int) _disp << "]";
				break;
			case OPERAND_TYPE_MOFFSET32:
				stream << ", " << "["<< (int) _disp << "]";
				break;
			default:
				break;
		}
	}

	_expr = stream.str();
}
