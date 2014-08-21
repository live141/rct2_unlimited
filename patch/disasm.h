#ifndef _DISASM_H_
#define _DISASM_H_

#include <stdint.h>

#define TWO_BYTE 0x0f

/* Instruction prefix */
#define PREFIX_REP 0xf3
#define PREFIX_REPZ 0xf3
#define PREFIX_REPNZ 0xf2
#define PREFIX_LOCK 0xf0

/* Size prefixes */
#define PREFIX_OPERAND_SIZE 0x66
#define PREFIX_ADDR_SIZE 0x67

/* Segment override */
#define SEGMENT_OVERRIDE_CS 0x2e
#define SEGMENT_OVERRIDE_SS 0x36
#define SEGMENT_OVERRIDE_DS 0x3e
#define SEGMENT_OVERRIDE_ES 0x26
#define SEGMENT_OVERRIDE_FS 0x64
#define SEGMENT_OVERRIDE_GS 0x65
#define SEGMENT_OVERRIDE_OPERAND_SIZE 0x66 
#define SEGMENT_OVERRIDE_ADDR_SIZE 0x67

/* Mod Register */
#define MOD_REG_INDIRECT 0x00
#define MOD_REG_INDIRECT_DISP8 0x01
#define MOD_REG_INDIRECT_DISP16 0x02
#define MOD_REG_DIRECT 0x03

/* SIB */
#define SIB_SCALE_0 0x00
#define SIB_SCALE_2 0x01
#define SIB_SCALE_4 0x02
#define SIB_SCALE_8 0x03

/* Registers */
#define REG_AL 0
#define REG_CL 1
#define REG_DL 2
#define REG_BL 3
#define REG_AH 4
#define REG_CH 5
#define REG_DH 6
#define REG_BH 7

#define REG_AX 0
#define REG_CX 1
#define REG_DX 2
#define REG_BX 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7

#define REG_EAX 0
#define REG_ECX 1
#define REG_EDX 2
#define REG_EBX 3
#define REG_ESP 4
#define REG_EBP 5
#define REG_ESI 6
#define REG_EDI 7

struct opcode_t {
	uint8_t instruction_prefix, addr_size_prefix, operand_size_prefix, segment_override;
	uint8_t modrm, sib;
	uint8_t displacement, immediate;
	uint8_t type;
};

class opcode {
protected:
	uint8_t *_addr;
	uint8_t _size;
	uint8_t _scale, _idx, _base, _mod, _reg, _rm;
	int32_t _disp, _imm;

	void _decode_sib(uint8_t sib) {
		_scale = sib >> 6;
		if( _scale == SIB_SCALE_8 )
			_scale = 8;
		else
			_scale *= 2;
		_idx = (sib >> 3) & 0x07;
		_base = sib & 0x07;
	}

	void _decode_modrm(uint8_t modrm) {
		_mod = modrm >> 6;
		_reg = (modrm >> 3) & 0x07;
		_rm = modrm & 0x07;
	}

public:
	opecode(void *addr) : _addr((uint8_t*) addr), _size(0), _scale(0), _idx(0), _base(0),
				_mod(0), _reg(0), _rm(0), _disp(0), _imm(0) {}
	void decode();
};

#endif
