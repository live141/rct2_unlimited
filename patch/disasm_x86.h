/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _DISASM_X86_H_
#define _DISASM_X86_H_

#include "disasm.h"
#include "instr_x86.h"
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
#define REGISTER_AL 0
#define REGISTER_CL 1
#define REGISTER_DL 2
#define REGISTER_BL 3
#define REGISTER_AH 4
#define REGISTER_CH 5
#define REGISTER_DH 6
#define REGISTER_BH 7

#define REGISTER_AX (0 | REG_SIZE_16)
#define REGISTER_CX (1 | REG_SIZE_16)
#define REGISTER_DX (2 | REG_SIZE_16)
#define REGISTER_BX (3 | REG_SIZE_16)
#define REGISTER_SP (4 | REG_SIZE_16)
#define REGISTER_BP (5 | REG_SIZE_16)
#define REGISTER_SI (6 | REG_SIZE_16)
#define REGISTER_DI (7 | REG_SIZE_16)

#define REGISTER_EAX (0 | REG_SIZE_32)
#define REGISTER_ECX (1 | REG_SIZE_32)
#define REGISTER_EDX (2 | REG_SIZE_32)
#define REGISTER_EBX (3 | REG_SIZE_32)
#define REGISTER_ESP (4 | REG_SIZE_32)
#define REGISTER_EBP (5 | REG_SIZE_32)
#define REGISTER_ESI (6 | REG_SIZE_32)
#define REGISTER_EDI (7 | REG_SIZE_32)

#define REGISTER_RAX (0 | REG_SIZE_64)
#define REGISTER_RCX (1 | REG_SIZE_64)
#define REGISTER_RDX (2 | REG_SIZE_64)
#define REGISTER_RBX (3 | REG_SIZE_64)
#define REGISTER_RSP (4 | REG_SIZE_64)
#define REGISTER_RBP (5 | REG_SIZE_64)
#define REGISTER_RSI (6 | REG_SIZE_64)
#define REGISTER_RDI (7 | REG_SIZE_64)

#define REGISTER_R8B 8
#define REGISTER_R9B 9
#define REGISTER_R10B 10
#define REGISTER_R11B 11
#define REGISTER_R12B 12
#define REGISTER_R13B 13
#define REGISTER_R14B 14
#define REGISTER_R15B 15

#define REGISTER_R8W (8 | REG_SIZE_16)
#define REGISTER_R9W (9 | REG_SIZE_16)
#define REGISTER_R10W (10 | REG_SIZE_16)
#define REGISTER_R11W (11 | REG_SIZE_16)
#define REGISTER_R12W (12 | REG_SIZE_16)
#define REGISTER_R13W (13 | REG_SIZE_16)
#define REGISTER_R14W (14 | REG_SIZE_16)
#define REGISTER_R15W (15 | REG_SIZE_16)

#define REGISTER_R8D (8 | REG_SIZE_32)
#define REGISTER_R9D (9 | REG_SIZE_32)
#define REGISTER_R10D (10 | REG_SIZE_32)
#define REGISTER_R11D (11 | REG_SIZE_32)
#define REGISTER_R12D (12 | REG_SIZE_32)
#define REGISTER_R13D (13 | REG_SIZE_32)
#define REGISTER_R14D (14 | REG_SIZE_32)
#define REGISTER_R15D (15 | REG_SIZE_32)

#define REGISTER_R8Q (8 | REG_SIZE_64)
#define REGISTER_R9Q (9 | REG_SIZE_64)
#define REGISTER_R10Q (10 | REG_SIZE_64)
#define REGISTER_R11Q (11 | REG_SIZE_64)
#define REGISTER_R12Q (12 | REG_SIZE_64)
#define REGISTER_R13Q (13 | REG_SIZE_64)
#define REGISTER_R14Q (14 | REG_SIZE_64)
#define REGISTER_R15Q (15 | REG_SIZE_64)

#define REGISTER_EIP (0xF0)
#define REGISTER_RIP (0xF1)

/* Operand types */
#define OPERAND_TYPE_REG32 0
#define OPERAND_TYPE_REG8 1
#define OPERAND_TYPE_IMM32 2
#define OPERAND_TYPE_IMM8 3
#define OPERAND_TYPE_RM8 4
#define OPERAND_TYPE_RM32 5
#define OPERAND_TYPE_AL 6
#define OPERAND_TYPE_EAX 7
#define OPERAND_TYPE_REL8 8
#define OPERAND_TYPE_REL32 9
#define OPERAND_TYPE_MOFFSET8 10
#define OPERAND_TYPE_MOFFSET32 11
#define OPERAND_TYPE_AX 12
#define OPERAND_TYPE_RAX 13
#define OPERAND_TYPE_AH 14
#define OPERAND_TYPE_DX 15
#define OPERAND_TYPE_EDX 16
#define OPERAND_TYPE_RDX 17
#define OPERAND_TYPE_DIRECT_REGISTER 18
#define OPERAND_TYPE_REG64 19
#define OPERAND_TYPE_XMM 20
#define OPERAND_TYPE_XMMM 21
#define OPERAND_TYPE_ESI 22
#define OPERAND_TYPE_EDI 23

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

class machine_context_x86 : public machine_context{
protected:
public:
	reg_t *rax, *rbx, *rcx, *rdx, *rdi, *rsi, *rbp, *rsp;
	reg_t *r8, *r9, *r10, *r11, *r12, *r13, *r14, *r15;
	reg_t *rip, *rflags, *cs, *fs, *gs;
	
	machine_context_x86(const void *cntx);	
	virtual reg get(const char *name);
	virtual reg get(uint8_t reg_num);	
	virtual void set_trapflag();
	virtual void clear_trapflag();

	virtual unsigned long flags() const {
		return *((unsigned long*) rflags);
	}
	
	virtual unsigned long pc() const {
		return *((unsigned long*) rip);
	}

	virtual void set_flags(unsigned long flags) {
		*((unsigned long*) rflags) = flags;
	}

	virtual void set_pc(unsigned long pc) {
		*((unsigned long*) rip) = pc;
	}
};

class opcode_x86;

class operand_x86 : public operand {
protected:
	friend class opcode_x86;
	uint8_t _index, _scale;

public:
	virtual operand_x86& operator=(operand_x86& op) {
		operand::operator=(op);
		_index = op._index;
		_scale = op._scale;
		return *this;
	}

	uint8_t index() const {
		return _index;
	}

	void set_index(uint8_t idx) {
		_index = idx;
	}
	
	void set_scale(uint8_t scale) {
		_scale = scale;
	}

	uint8_t scale() const {
		return _scale;
	}

	virtual void reset() {
		operand::reset();
		_base = REGISTER_INVAL;
		_index = REGISTER_INVAL;
		_scale = 0;
	}

	virtual bool is_register() const {
		return (_type == OPERAND_TYPE_REG32 || _type == OPERAND_TYPE_REG8 || _type == OPERAND_TYPE_AL
                || _type == OPERAND_TYPE_AH || _type == OPERAND_TYPE_AX || _type == OPERAND_TYPE_EAX
                || _type == OPERAND_TYPE_RAX || _type == OPERAND_TYPE_DX || _type == OPERAND_TYPE_EDX
                || _type == OPERAND_TYPE_RDX || _type == OPERAND_TYPE_XMM);
	}

	virtual bool is_rel() const {
		return (_type == OPERAND_TYPE_REL8 || _type == OPERAND_TYPE_REL32);
	}

	virtual bool is_imm() const {
		return (_type == OPERAND_TYPE_IMM8 || _type == OPERAND_TYPE_IMM32);
	}
	
	virtual bool is_sp() const {
		return (MASK_REG(_register) == MASK_REG(REGISTER_RSP));
	}
	
	virtual bool uses_pc() const {
		return (MASK_REG(_base) == MASK_REG(REGISTER_RIP));
	}
};

class opcode_x86 : public opcode {
protected:
	opcode_x86_t *_code;
	uint8_t _scale, _idx, _base, _mod, _reg_ope, _rm;
	int64_t _disp;
	uint8_t _segment, _addr_size_prefix, _op_size_prefix;
	uint8_t _sib;
	uint8_t _disp_size;
	uint8_t _bitmode, _prefix64;
	uint8_t _offset_disp;

	void _decode_modrm(uint8_t byte);
	virtual void _decode();
	std::string _format_modrm(uint8_t type, uint8_t i);

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

	void _init() {
		_operand[0] = new operand_x86;
		_operand[1] = new operand_x86;
		_operand[2] = new operand_x86;
		_operand[3] = new operand_x86;
	}

	void _reset() {
		opcode::_reset();
		_scale = 0;
		_idx = 0;
		_base = 0;
		_mod = 0;
		_reg_ope = 0;
		_rm = 0;
		_disp = 0;
		_segment = 0;
		_addr_size_prefix = 0;
		_op_size_prefix = 0;
		_sib = 0;
		_prefix64 = 0;
		_disp_size = 0;
		_offset_disp = 0;
	}

public:
	opcode_x86(const void *addr, architecture arch, x86_bitmode bitmode) : opcode((uint8_t*) addr, arch), _code(NULL), _scale(0), _idx(0), _base(0),
				_mod(0), _reg_ope(0), _rm(0), _disp(0), _segment(0), 
				_addr_size_prefix(0), _op_size_prefix(0), _sib(0),
				_disp_size(0), _bitmode(bitmode), _prefix64(0),
				_offset_disp(0) {
		_init();
	}

	virtual opcode_x86& operator =(const opcode_x86 &op) {
		opcode::operator=(op);
		_code = op._code;
		_size = op._size;
		_scale = op._scale;
		_idx = op._idx;
		_base = op._base;
		_mod = op._mod;
		_reg_ope = op._reg_ope;
		_rm = op._rm;
		_disp = op._disp;
		_segment = op._segment;
		_addr_size_prefix = op._addr_size_prefix;
		_op_size_prefix = op._op_size_prefix;
		_sib = op._sib;
		_disp_size = op._disp_size;
		_bitmode = op._bitmode;
		_prefix64 = op._prefix64;
		_offset_disp = op._offset_disp;
		return *this;
	}

	virtual void copy_to(void *dest);
	virtual void set_imm(int64_t val);
	virtual void set_disp(int64_t val);

	virtual const int64_t displacement() const {
		return _disp;
	}
	
	virtual const int64_t immediate() const {
		if(_code->type_op[0] == OPERAND_TYPE_REL8 || _code->type_op[0] == OPERAND_TYPE_REL32)
			return _size + _imm + (unsigned long) _addr;
		return _imm;
	}

	const uint8_t size_disp() const {
		return _disp_size;
	}
	
	virtual bool is_cond_jump() const {
		return ((_instr >= INSTR_JO && _instr <= INSTR_JNLE) || _instr == INSTR_JCXZ || _instr == INSTR_JECXZ);
	}

	virtual bool is_jump() const {
		return (is_cond_jump() || _instr == INSTR_JMP || _instr == INSTR_JMPF);
	}

	virtual bool is_ret() const {
		return (_instr == INSTR_RETN || _instr == INSTR_RETF || _instr == INSTR_IRET
			|| _instr == INSTR_IRETD || _instr == INSTR_SYSRET);
	}

	virtual bool is_compare() const {
		return (_instr == INSTR_CMP || _instr == INSTR_CMPS || _instr == INSTR_CMPXCHG || _instr == INSTR_TEST ||
			_instr == INSTR_CMPPS || _instr == INSTR_CMPSS || _instr == INSTR_CMPPD || _instr == INSTR_CMPSD || 
			_instr == INSTR_CMPXCHG8B);
	}

	//void execute_change_result(uint64_t result, machine_context_t *context);
	//uint64_t execute(machine_context_x86 *context);
};

#endif
