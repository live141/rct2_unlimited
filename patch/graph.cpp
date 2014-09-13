/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "graph.h"

node* dfs::next() {
	_prev = _next;
	_visited.push_back(_next);

	if(_next == NULL)
		return NULL;

	/* push not visited sons to stack */
	for(int i = 0; i < _max_edges; ++i) {
		node *n = _next->edge(i);
		if(n != NULL && find(_visited.begin(), _visited.end(), n) == _visited.end())
			_stack.push_back(n);
	}
	if(_stack.size() > 0) {
		_next = _stack.back();
		_stack.pop_back();
	}
	else
		_next = NULL;
	return _prev;
}
