/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _DISASM_H_
#define _DISASM_H_

#include "defines.h"
#include <stdint.h>
#include <string>

/* Registers */
#define REG_SIZE_8 0
#define REG_SIZE_16 (size_word<<4)
#define REG_SIZE_32 (size_dword<<4)
#define REG_SIZE_64 (size_qword<<4)
#define REG_SIZE_128 (size_128<<4)
#define REG_SIZE_256 (size_256<<4)

#define MASK_REG(x) (x & (0x0f))
#define MASK_REG_SIZE(x) ((reg_size)(((unsigned)x & (0xf0))>>4))

#define REG_INVAL (0xff)
#define OPERAND_TYPE_INVAL 0xff
#define INSTR_INVAL 0

union reg_t {
	int64_t rx;
	int32_t ex;
	int16_t x;
	struct {
		int8_t low;
		int8_t high;
	};
};

enum architecture {
	arch_x86 = 0,
	arch_x64,
	arch_arm,
	arch_ppc
};

enum reg_size {
	size_byte = 0,
	size_word,
	size_dword,
	size_qword,
	size_low,
	size_high,
	size_128,
	size_256
};

class reg {
protected:
	reg_t *_reg;
	reg_size _size;

	reg();	
public:
	reg(reg_t *reg, reg_size size) : _reg(reg), _size(size) {}

	int64_t get() const {
		if(_size == size_word) {
			return _reg->x;
		}
		if(_size == size_dword) {
			return _reg->ex;
		}
		if(_size == size_qword) {
			return _reg->rx;
		}
		if(_size == size_low) {
			return _reg->low;
		}
		if(_size == size_high) {
			return _reg->high;
		}
	}
	
	void set(int64_t val) {
		if(_size == size_word) {
			_reg->x = val;
			return;
		}
		if(_size == size_dword) {
			_reg->ex = val;
			return;
		}
		if(_size == size_qword) {
			_reg->rx = val;
			return;
		}
		if(_size == size_low) {
			_reg->low = val;
			return;
		}
		if(_size == size_high) {
			_reg->high = val;
			return;
		}
	}
};

class machine_context {
protected:
public:
	virtual reg get(const char *name) = 0;
	virtual reg get(uint8_t reg_num) = 0;

	virtual unsigned long flags() const = 0;
	virtual unsigned long  pc() const = 0;
	virtual void set_flags(unsigned long flags) = 0;
	virtual void set_pc(unsigned long pc) = 0;
	virtual void set_trapflag() = 0;
	virtual void clear_trapflag() = 0;
	static machine_context* create(const void *cntx, architecture arch);
};

class operand {
protected:
	std::string _expr;
	uint8_t _size;
	uint8_t _type;
	uint8_t _register, _base;

public:
	virtual bool is_register() const  = 0;
	virtual bool is_rel() const  = 0;
	virtual bool is_imm() const  = 0;
	virtual operand& operator=(operand& op) {
		_expr = op._expr;
		_size = op._size;
		_type = op._type;
		_register = op._register;
		_base = op._base;
		return *this;
	}

	virtual void reset() {
		_expr = std::string("");
		_size = 0;
		_type = OPERAND_TYPE_INVAL;
		_register = REG_INVAL;
		_base = REG_INVAL;
	}

	void set_type(uint8_t type) {
		_type = type;
	}

	void set_size(uint8_t size) {
		_size = size;
	}

	void set_expr(std::string& expr) {
		_expr = expr;
	}

	void set_register(uint8_t reg) {
		_register = reg;
	}

	void set_base(uint8_t reg) {
		_base = reg;
	}

	uint8_t base() const {
		return _base;
	}

	uint8_t type() const {
		return _type;
	}

	uint8_t size() const {
		return _size;
	}

	const char* expression() {
		return _expr.c_str();
	}
};

class opcode {
protected:
	uint8_t *_addr;
	uint8_t _size;
	int64_t _imm;
	uint8_t _imm_size;
	uint8_t _offset_imm;
	//uint8_t _op_size[4];
	operand *_operand[4];
	uint16_t _instr;
	std::string _name, _expr;

	virtual void _init() = 0;
	virtual void _reset() {
		_imm = 0;
		_imm_size = 0;
		_size = 0;
		_offset_imm = 0;
		_operand[0]->reset();
		_operand[1]->reset();
		_operand[2]->reset();
		_operand[3]->reset();
		_name = std::string();
		_expr = std::string();
		_instr = INSTR_INVAL;
	}
	opcode(const void *addr) : _addr((uint8_t*) addr), _size(0), _imm(0),
		_imm_size(0), _offset_imm(0), _instr(INSTR_INVAL) {
		_operand[0] = NULL;
		_operand[1] = NULL;
		_operand[2] = NULL;
		_operand[3] = NULL;
	}

public:
	virtual ~opcode() {
		delete _operand[0];
		delete _operand[1];
		delete _operand[2];
		delete _operand[3];
	}

	virtual opcode& operator =(const opcode &op) {
		_addr = op._addr;
		_size = op._size;
		_imm = op._imm;
		_imm_size = op._imm_size;
		_offset_imm = op._offset_imm;
		_name = op._name;
		_expr = op._expr;
		*_operand[0] = *op._operand[0];
		*_operand[1] = *op._operand[1];
		*_operand[2] = *op._operand[2];
		*_operand[3] = *op._operand[3];
		_instr = op._instr;

		return *this;
	}

	virtual void decode() = 0;
	virtual void set_imm(int64_t val) = 0;
	virtual const int64_t immediate() const = 0;
	virtual bool is_cond_jump() const  = 0;
	virtual bool is_jump() const  = 0;
	virtual bool is_ret() const  = 0;
	virtual bool is_compare() const = 0;
	static opcode* create(const void *addr, enum architecture);


	uint8_t size() const {
		return _size;
	}

	const char* name() const {
		return _name.c_str();
	}

	const uint16_t instr() const {
		return _instr;
	}
	
	const char* expression() const {
		return _expr.c_str();
	}

	const uint8_t size_imm() const {
		return _imm_size;
	}

	const uint8_t* addr() const {
		return _addr;
	}

	void next() {
		_addr += size();
		decode();
	}

	operand* get_operand(uint8_t n) {
		return _operand[n%4];
	}

	//void execute_change_result(uint64_t result, machine_context_t *context);
	//uint64_t execute(machine_context_x86 *context);
};

#endif
