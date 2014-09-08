/*
* WORKING TITLE
* Copyright (c) 2014 Fabian Fischer
*/

#include "signature.h"
#include "addr.h"

extern module_t g_rct2;

uint8_t signature::_compare(uint8_t *b1, uint8_t *b2, unsigned int size) {
	/* ? is wildcard */
	unsigned int i;
	for(i = 0; i < size; ++i) {
		if(b1[i] == '?')
			continue;
		if( b1[i] != b2[i] )
			break;
	}
	return (i == size)?1:0;
}

uint32_t signature::search() {
	uint8_t *sig = (uint8_t*) _sig.c_str();
	uint8_t *begin = g_rct2.section_text.addr;
	unsigned long size = g_rct2.section_text.size;
	/* don't count terminating 0x00 */
	unsigned int size_sig = _sig.length()-1;
	/* adjust size with signature length */
	size = size - size_sig + 1;

	for(unsigned long i = 0; i < size; ++i) {
		if( _compare(sig, begin, size_sig) ) {
			_vec_results.push_back(begin);
		}
		++begin;
	}
	return _vec_results.size();
}
