#include "disasm_x86.h"
#include <stdio.h>
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

reg machine_context_x86::get(const char *name) {
	int i, hash = 0;
	enum reg_size size = byte;
	char end = 0;
	for(i = 0; name[i] != 0 && i < 7; ++i) {
		hash += name[i];
	}
	end = name[i-1];

	if(end == 'b' || end == 'h' || end == 'l')
		size = byte;
	else if(name[0] == 'e' || end == 'd')
		size = dword;
	else if(name[0] != 'r' || end == 'w' )
		size = word;
	else
		size = qword;

	if(size == byte) {
		switch(hash) {
			case 205: return reg(rax, low);
			case 207: return reg(rcx, low);
			case 208: return reg(rdx, low);
			case 206: return reg(rbx, low);
			case 201: return reg(rax, high);
			case 203: return reg(rcx, high);
			case 204: return reg(rdx, high);
			case 202: return reg(rbx, high);
			case 268: return reg(r8, low);
			case 269: return reg(r9, low);
			case 309: return reg(r10, low);
			case 310: return reg(r11, low);
			case 311: return reg(r12, low);
			case 312: return reg(r13, low);
			case 313: return reg(r14, low);
			case 314: return reg(r15, low);
		}
	}

	if(size == word) {
		switch(hash) {
			case 217: return reg(rax, word);
			case 219: return reg(rcx, word);
			case 220: return (name[0] == 'd')?reg(rdx, word):reg(rsi, word);
			case 218: return reg(rbx, word);
			case 227: return reg(rsp, word);
			case 210: return reg(rbp, word);
			//case 220: return reg(rsi, word);
			case 205: return reg(rdi, word);
			case 289: return reg(r8, word);
			case 290: return reg(r9, word);
			case 330: return reg(r10, word);
			case 331: return reg(r11, word);
			case 332: return reg(r12, word);
			case 333: return reg(r13, word);
			case 334: return reg(r14, word);
			case 335: return reg(r15, word);
		}
	}

	if(size == dword) {
		switch(hash) {
			case 318: return reg(rax, dword);
			case 320: return reg(rcx, dword);
			case 321: return (name[1] == 'd')?reg(rdx, dword):reg(rsi, dword);
			case 319: return reg(rbx, dword);
			case 328: return reg(rsp, dword);
			case 311: return (name[1] == 'b')?reg(rbp, dword):reg(r10, dword);
			//case 321: return reg(rsi, dword);
			case 306: return reg(rdi, dword);
			case 270: return reg(r8, dword);
			case 271: return reg(r9, dword);
			//case 311: return reg(r10, dword);
			case 312: return reg(r11, dword);
			case 313: return reg(r12, dword);
			case 314: return reg(r13, dword);
			case 315: return reg(r14, dword);
			case 316: return reg(r15, dword);
		}
	}

	if(size == qword) {
		switch(hash) {
			case 331: return reg(rax, qword);
			case 333: return reg(rcx, qword);
			case 334: return (name[1] == 'd')?reg(rdx, qword):reg(rsi, qword);
			case 332: return reg(rbx, qword);
			case 341: return reg(rsp, qword);
			case 324: return reg(rbp, qword);
			//case 334: return reg(rsi, qword);
			case 319: return reg(rdi, qword);
			case 170: return reg(r8, qword);
			case 171: return reg(r9, qword);
			case 211: return reg(r10, qword);
			case 212: return reg(r11, qword);
			case 213: return reg(r12, qword);
			case 214: return reg(r13, qword);
			case 215: return reg(r14, qword);
			case 216: return reg(r15, qword);
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
		_op_size[i] = 1;
	else {
		if(_addr_size_prefix)
			_op_size[i] = 2;
		else if(_is_opsize64())
			_op_size[i] = 8;
		else
			_op_size[i] = 4;
	}
	//if(!_addr_size_prefix) {
		if(_mod == MOD_REG_DIRECT) {
			if(type == 8) {
				return std::string(g_lut_registers8[_rm]);
			}
			else if(!_is_opsize64()) {
				return std::string(g_lut_registers32[_rm]);
			}
			else {
				return std::string(g_lut_registers64[_rm]);
			}
		}
		else {
			if(_rm == 0x04) {
				/* sib */
				if(_bitmode == mode_32 /*!_is_opsize64()*/)
					stream << "[" << g_lut_registers32[_base];
				else
					stream << "[" << g_lut_registers64[_base];
				if(_idx != 0x04) {
					if(_bitmode == mode_32 /*!_is_opsize64()*/)
						stream << "+" << g_lut_registers32[_idx] << "*" << (int) _scale;
					else
						stream << "+" << g_lut_registers64[_idx] << "*" << (int) _scale;
				}
			}
			else if(_rm == 0x05 && _mod == MOD_REG_INDIRECT) {
				/* disp32 */
				stream << "[";
				if(_bitmode == mode_64) {
					if(_is_opsize64())
						stream << "rip";
					else
						stream << "eip";
				}
				stream << std::dec << std::showpos << _disp;
			}
			else {
				if(_bitmode == mode_32 /*!_is_opsize64()*/)
					stream << "[" << g_lut_registers32[_rm];
				else
					stream << "[" << g_lut_registers64[_rm];
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



void opcode_x86::decode() {
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
		switch(_code->type_op[i]) {
			case OPERAND_TYPE_REG64:
				stream << ", " << g_lut_registers64[_reg_ope];
				_op_size[i] = 8;
				break;
			case OPERAND_TYPE_REG32:
				stream << ", ";
				if(_op_size_prefix) {
					_op_size[i] = 2;
					stream << g_lut_registers16[_reg_ope];
				}
				else if(!_is_opsize64()) {
					stream << g_lut_registers32[_reg_ope];
					_op_size[i] = 4;
				}
				else {
					stream << g_lut_registers64[_reg_ope];
					_op_size[i] = 8;
				}
				break;
			case OPERAND_TYPE_REG8:
				stream << ", " << g_lut_registers8[_reg_ope];
				_op_size[i] = 1;
				break;
			case OPERAND_TYPE_IMM32:
				stream << ", 0x" << _imm;
				if(_op_size_prefix)
					_op_size[i] = 2;
				else if(!_is_opsize64())
					_op_size[i] = 4;
				else
					_op_size[i] = 8;
				break;
			case OPERAND_TYPE_IMM8:
				stream << ", 0x" << _imm;
				_op_size[i] = 1;
				break;
			case OPERAND_TYPE_RM8:
				stream << ", " << _format_modrm(8, i);
				break;
			case OPERAND_TYPE_RM32:
				stream << ", " << _format_modrm(32, i);
				break;
			case OPERAND_TYPE_AL:
				stream << ", al";
				_op_size[i] = 1;
				break;
			case OPERAND_TYPE_AX:
				stream << ", ax";
				_op_size[i] = 2;
				break;
			case OPERAND_TYPE_EAX:
				if(!_op_size_prefix) {
					stream << ", eax";
					_op_size[i] = 4;
				}
				else {
					stream << ", ax";
					_op_size[i] = 2;
				}
				break;
			case OPERAND_TYPE_RAX:
				if(_is_opsize64()) {
					stream << ", rax";
					_op_size[i] = 8;
				}
				else if(_op_size_prefix) {
					stream << ", ax";
					_op_size[i] = 2;
				}
				else {
					stream << ", eax";
					_op_size[i] = 4;
				}
				break;
			case OPERAND_TYPE_DX:
				stream << ", dx";
				_op_size[i] = 2;
				break;
			case OPERAND_TYPE_EDX:
				if(!_op_size_prefix) {
					stream << ", edx";
					_op_size[i] = 4;
				}
				else {
					stream << ", dx";
					_op_size[i] = 2;
				}
				break;
			case OPERAND_TYPE_RDX:
				if(_is_opsize64()) {
					stream << ", rdx";
					_op_size[i] = 8;
				}
				else if(_op_size_prefix) {
					stream << ", dx";
					_op_size[i] = 2;
				}
				else {
					stream << ", edx";
					_op_size[i] = 4;
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
			default:
				break;
		}
	}

	for(int i = 0; i < 1; ++i) {
		if(_op_size[i] == 8)
			_name.append("q");
		if(_op_size[i] == 4)
			_name.append("l");
		if(_op_size[i] == 2)
			_name.append("w");
		if(_op_size[i] == 1)
			_name.append("b");
	}

	std::transform(_name.begin(), _name.end(), _name.begin(), ::tolower);
	_expr = _name;
	_expr += stream.str().c_str()+1;
}
