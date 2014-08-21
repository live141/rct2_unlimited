#include "disasm.h"

extern opcode_t g_opcode_1b[256];
extern opcode_t g_opcode_2b[256];

void opcode::decode() {
	uint8_t *byte = _addr;
	uint8_t size = 0;

	/* check prefixes first, max 4 bytes */
	for(unsigned int i = 0; i < 4; ++i) {
		switch(*byte) {
			/* Instruction prefix */
			case PREFIX_REP:
				++size;
				break;
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
				++size;
				break;
			case PREFIX_ADDR_SIZE:
				++size;
				break;

			/* Segment override */
			case SEGMENT_OVERRIDE_CS:
				++size;
				break;
				++size;
			case SEGMENT_OVERRIDE_SS:
				++size;
				break;
			case SEGMENT_OVERRIDE_DS:
				++size;
				break;
			case SEGMENT_OVERRIDE_ES:
				++size;
				break;
			case SEGMENT_OVERRIDE_FS:
				++size;
				break;
			case SEGMENT_OVERRIDE_GS:
				++size;
				break;
			case SEGMENT_OVERRIDE_OPERAND_SIZE:
				++size;
				break;
			case SEGMENT_OVERRIDE_ADDR_SIZE:
				++size;
				break;
			/* nothing found -> no prefix */
			default:
				i = 4;
				break;
		}
		++byte;
	}

	/* check if opcode is two bytes long */
	if(*byte == TWO_BYTE) {
		++byte;
		++size;
		/* TODO: Decode */
		switch(*byte) {

		}
	}
	else {
		/* TODO: Decode */
		switch(*byte) {

		}
	}
}
