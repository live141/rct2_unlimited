/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <algorithm>

class node {
public:
	virtual node* edge(const int n) const = 0;
	virtual int max_edges() const = 0;
};

class dfs {
protected:
	std::vector<node*> _visited, _stack;
	node *_start, *_next, *_prev;
	int _max_edges;

	dfs() {}
public:
	dfs(node *start) : _start(start), _next(start), _max_edges() {
		_max_edges = start->max_edges();
	}

	node* next();
	
	void reset() {
		_next = _start;
		_visited.clear();
		_stack.clear();
	}

	bool is_reachable(node *n) {
		node *k;
		reset();
		while((k = next()) != NULL) {
			if(k == n)
				return true;
		}
		return false;
	}
};

#endif
