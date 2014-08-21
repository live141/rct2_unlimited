#ifndef _SIGNATURE_H_
#define _SIGNATURE_H_

#include <string>
#include <vector>
#include <stdint.h>

class signature {
protected:
	std::string _sig;
	std::vector<uint8_t*> _vec_results;

	uint8_t _compare(uint8_t *b1, uint8_t *b2, unsigned int size);

public:
	signature(const char *sig) : _sig(sig) {}
	uint32_t search();

	std::vector<uint8_t*> results() {
		return _vec_results;
	}
};

#endif
