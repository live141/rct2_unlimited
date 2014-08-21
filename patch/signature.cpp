#include "signature.h"
#include "addr.h"

extern module_t g_rct2;

uint8_t _compare(uint8_t *b1, uint8_t *b2, unsigned int size) {
	/* ? is wildcard */
	for(unsigned int i = 0; i < size; ++i) {
		if(b1[i] == '?')
			continue;
		if( b1[i] != b2[i] )
			break;
	}
	return (i == size)?1:0;
}

uint32_t signature::search() {
	unsigned char *sig = _sig.c_str();
	unsigned char *begin = g_rct2.section_text.addr;
	unsigned long size = g_rct2.section_text.size;
	/* don't count terminating 0x00 */
	unsigned int size_sig = _sig.len()-1;
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
