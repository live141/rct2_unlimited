#include "disasm.h"
#include <stdio.h>

extern opcode_t g_opcode_1b[256];
extern opcode_t g_opcode_2b[256];

void opcode::decode() {
	opcode_t *code;
	uint8_t *byte = _addr;
	uint8_t size = 0;
	uint8_t op_size_div = 1;

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
				op_size_div = 2;
				++size;
				break;
			case PREFIX_ADDR_SIZE:
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

	/* check if opcode is two bytes long */
	if(*byte == TWO_BYTE) {
		++byte;
		++size;
		code = &g_opcode_2b[*byte];
	}
	else {
		code = &g_opcode_1b[*byte];
	}

	/* opcode LUT is shifted, since one byte can describe mutliple opcodes
	 * -> search for real opcode
	 * -> check for opcode with respect to opcode extension in mod r/m byte */

	while(code->opcode != *byte) {
		++code;
	}
	++size;
	++byte;

	_name = code->name;

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

	//printf("FOUND %u\n", code->type);

	/* check for SIB byte */
	if(code->size_sib == 1) {
		_decode_sib(*byte);
		++byte;
		++size;
	}

	if(code->size_immediate == 1) {
		_imm = *byte;
		++byte;
		++size;
	}	
	else if(code->size_immediate == 4) {
		if(op_size_div == 2) {
			_imm = *((uint16_t*) byte);
		}
		else {
			_imm = *((uint32_t*) byte);
		}
		byte += code->size_immediate/op_size_div;
		size += code->size_immediate/op_size_div;
	}

	if(code->size_displacement == 1) {
		_disp = *byte;
		++byte;
		++size;
	}	
	else if(code->size_displacement == 4) {
		if(op_size_div == 2) {
			_disp = *((uint16_t*) byte);
		}
		else {
			_disp = *((uint32_t*) byte);
		}
		byte += code->size_displacement/op_size_div;
		size += code->size_displacement/op_size_div;
	}
	_size = size;
}
