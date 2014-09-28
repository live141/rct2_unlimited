/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _DECOMP_H_
#define _DECOMP_H_

#include "disasm.h"
#include "graph.h"
#include <vector>
#include <map>

class basicblock : public node {
protected:
	const void *_addr;
	std::vector<opcode*> _vec_opcode;
	std::vector<basicblock*> _vec_prebb;
	basicblock *_backedge, *_bb0, *_bb1;
	bool _decoded, _is_loop, _is_ifelse;

	static std::map<const void*, basicblock*> _bbs;
	static basicblock* _find(const void *addr);

	void _add_prebb(basicblock *prebb) {
		_vec_prebb.push_back(prebb);
	}

	basicblock();
	basicblock(const void *addr, basicblock *prebb = NULL);

public:
	typedef std::map<const void*, basicblock*>::iterator iterator;
	static basicblock* create(const void *addr, basicblock *prebb = NULL);
	~basicblock();
	void decode();
	void decodeall();
	bool can_reach(basicblock *bb);
	bool can_reach_without_backedge(basicblock *bb);
	opcode* last_condition();

	void add_prebb(basicblock *prebb) {
		_vec_prebb.push_back(prebb);
	}

	std::vector<basicblock*>& vec_prebb() {
		return _vec_prebb;
	}

	basicblock* backedge() const {
		return _backedge;
	}

	basicblock* edge0() const {
		return _bb0;
	}
	
	basicblock* edge1() const {
		return _bb1;
	}

	virtual node* edge(const int n) const {
		switch(n) {
			case 0: return (node*) _bb0;
			case 1: return (node*) _bb1;
		}
		return NULL;
	}

	virtual int max_edges() const {
		return 2;
	}

	bool decoded() const {
		return _decoded;
	}

	int num_instr() const {
		return _vec_opcode.size();
	}

	opcode* last_instr() {
		if(_vec_opcode.size() == 0)
			return NULL;
		return _vec_opcode.back();
	}

	opcode* first_instr() {
		if(_vec_opcode.size() == 0)
			return NULL;
		return _vec_opcode.front();
	}

	const void* addr() const {
		return _addr;
	}

	std::vector<opcode*>& vec_opcode() {
		return _vec_opcode;
	}

	void free() {
		_bbs.erase(_bbs.find(_addr));
		delete this;
	}
};

enum code_type {
	type_loop = 0,
	type_ifelse,
	type_switch
};

struct condition_t {
	opcode *op;
	condition_type type;
};

class code_unit {
protected:
	basicblock *_bb;
	code_type _type;

	code_unit();
public:
	code_unit(basicblock *bb, const code_type type) : _bb(bb), _type(type) {}
	virtual ~code_unit();
	virtual void analyze() = 0;
	
	basicblock *bb() const {
		return _bb;
	}

	bool is_loop() const {
		return (_type == type_loop);
	}
	
	bool is_ifelse() const {
		return (_type == type_ifelse);
	}
};

class code_loop : public code_unit{
protected:
	std::vector<condition_t> _vec_condition;
	std::vector<std::string> _vec_expr;

public:
	code_loop(basicblock *bb) : code_unit(bb, type_loop) {}
	virtual ~code_loop();
	virtual void analyze();
	virtual const char* expression(const uint8_t idx);
	std::vector<condition_t>& vec_condition() {
		return _vec_condition;
	}
};

class code_ifelse : public code_unit {
protected:
	condition_t _op_condition;

public:
	code_ifelse(basicblock *bb) : code_unit(bb, type_ifelse) {}
	virtual ~code_ifelse();
	virtual void analyze();
};

class code_switch : public code_unit {
protected:

public:
	code_switch(basicblock *bb) : code_unit(bb, type_switch) {}
	virtual ~code_switch();
	virtual void analyze();
};

class code_structure {
protected:
	std::vector<code_unit*> _vec_units;
	const void *_addr;

	code_structure();
	
public:
	code_structure(const void *addr) : _addr(addr) {}
	~code_structure();
	std::vector<code_unit*>& analyze();
};

#endif
