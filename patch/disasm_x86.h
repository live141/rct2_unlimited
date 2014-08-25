#ifndef _DISASM_X86_H_
#define _DISASM_X86_H_

#include <stdint.h>
#include <string>

#define TWO_BYTE 0x0f

/* Instruction prefix */
//#define PREFIX_REP 0xf3
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
//#define SEGMENT_OVERRIDE_OPERAND_SIZE 0x66 
//#define SEGMENT_OVERRIDE_ADDR_SIZE 0x67

/* Mod Register */
#define MOD_REG_INDIRECT 0x00
#define MOD_REG_INDIRECT_DISP8 0x01
#define MOD_REG_INDIRECT_DISP32 0x02
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

#define REG_R8 0
#define REG_R9 1
#define REG_R10 2
#define REG_R11 3
#define REG_R12 4
#define REG_R13 5
#define REG_R14 6
#define REG_R15 7

/* Operand types */
#define OPERAND_TYPE_INVAL 0xff
#define OPERAND_TYPE_REG32 0
#define OPERAND_TYPE_REG8 1
#define OPERAND_TYPE_IMM32 2
#define OPERAND_TYPE_IMM8 3
#define OPERAND_TYPE_RM8 4
#define OPERAND_TYPE_RM32 5
#define OPERAND_TYPE_AL 6
#define OPERAND_TYPE_AX32 7
#define OPERAND_TYPE_REL8 8
#define OPERAND_TYPE_REL32 9
#define OPERAND_TYPE_MOFFSET8 10
#define OPERAND_TYPE_MOFFSET32 11

#define OPCODE_EXT_INVAL 0xff

struct opcode_x86_t {
	uint8_t opcode, opcode_ext;
	uint8_t size_modrm, size_sib;
	uint8_t size_displacement, size_immediate;
	uint16_t type;
	uint8_t type_op[4];
	char name[16];
};

enum x86_bitmode {
	mode_32 = 0,
	mode_64
};

class opcode_x86 {
protected:
	uint8_t *_addr;
	uint8_t _size;
	uint8_t _scale, _idx, _base, _mod, _reg_ope, _rm;
	int64_t _disp, _imm;
	uint8_t _segment, _addr_size_prefix, _op_size_prefix;
	uint8_t _sib;
	uint8_t _disp_size, _imm_size;
	uint8_t _bitmode, _prefix64;
	std::string _name, _expr;

	void _decode_modrm(uint8_t byte);
	std::string _format_modrm(uint8_t type);

	void _decode_sib(uint8_t sib) {
		_scale = sib >> 6;
		_scale = 1 << _scale;
		_idx = (sib >> 3) & 0x07;
		_base = sib & 0x07;

		if(_prefix64 & 0x02)
			_idx |= 0x08;

		if(_prefix64 & 0x01)
			_base |= 0x08;
	}

	uint8_t _is_opsize64() {
		return (_prefix64 & 0x08);
	}

	/* bitmode and addr are important, so don't allow standard constructor */
	opcode_x86() {}

public:
	opcode_x86(void *addr, x86_bitmode bitmode) : _addr((uint8_t*) addr), _size(0), _scale(0), _idx(0), _base(0),
				_mod(0), _reg_ope(0), _rm(0), _disp(0), _imm(0), _segment(0), 
				_addr_size_prefix(0), _op_size_prefix(0), _sib(0),
				_disp_size(0), _imm_size(0), _bitmode(bitmode), _prefix64(0) {}
	void decode();

	uint8_t size() const {
		return _size;
	}

	const char* name() const {
		return _name.c_str();
	}
	
	const char* expression() const {
		return _expr.c_str();
	}

	const int64_t displacement() const {
		return _disp;
	}
	
	const int64_t immediate() const {
		return _imm;
	}

	const uint8_t* addr() const {
		return _addr;
	}
};

#endif
