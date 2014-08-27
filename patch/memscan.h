#ifndef _MEMSCAN_H_
#define _MEMSCAN_H_

#include <vector>
#include <stdint.h>
#include <limits>
#include <stdlib.h>

#if defined(linux) || defined(__APPLE__)
/* SSE */
#define CHECK_BYTE(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_WORD(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_BYTE_SIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_WORD_SIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD_SIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD_SIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_BYTE_SIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_WORD_SIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD_SIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD_SIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_BYTE_UNSIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pmaxub %%xmm0, %%xmm1\n\t" \
		"pcmpeqb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_WORD_UNSIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pmaxuw %%xmm0, %%xmm1\n\t" \
		"pcmpeqw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD_UNSIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pmaxud %%xmm0, %%xmm1\n\t" \
		"pcmpeqd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD_UNSIGNED_GT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pmaxuq %%xmm0, %%xmm1\n\t" \
		"pcmpeqq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_BYTE_UNSIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pminub %%xmm0, %%xmm1\n\t" \
		"pcmpeqb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_WORD_UNSIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pminuw %%xmm0, %%xmm1\n\t" \
		"pcmpeqw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD_UNSIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pminud %%xmm0, %%xmm1\n\t" \
		"pcmpeqd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD_UNSIGNED_LT(dst,src,flag) asm volatile("movaps %2, %%xmm1\n\t" \
		"movups (%1), %%xmm0\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pminuq %%xmm0, %%xmm1\n\t" \
		"pcmpeqq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"pand %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		"notq %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

/* AVX 256 */
#define CHECK_BYTE_AVX(dst,src,flag) asm volatile("vmovaps (%2), %%ymm0\n\t" \
		"vmovups (%1), %%ymm1\n\t" \
		"prefetch 0x20(%1)\n\t" \
		"vpcmpeqb %%ymm0, %%ymm1, %%ymm1\n\t" \
		"vmovaps %%ymm1, %%ymm3\n\t" \
		"vpshufd $0xee, %%ymm1, %%ymm3\n\t" \
		"vorpd %%ymm1, %%ymm3, %%ymm3\n\t" \
		"vpextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "ymm0", "ymm1", "ymm3")

#else
/* Windows */
#error "TODO"
#endif

#define SEARCH_ALIGNED(val, begin, end, vector_cmp, cmp) do { \
			uint64_t found; \
			uint8_t *i; \
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) { \
					vector_cmp(i, val, found); \
				if( found != 0 ) { \
					/* where exactly? */ \
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) { \
						if(*((T*) ((uint8_t*)i+k)) cmp val[0]) \
							_vec_results.push_back((T*) ((uint8_t*)i+k)); \
					} \
				} \
			} \
			/* check also the rest */ \
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) { \
				if(*((T*) ((uint8_t*)i+k)) cmp val[0]) \
					_vec_results.push_back((T*) ((uint8_t*)i+k)); \
			} \
		} while(0)

#define SEARCH_UNALIGNED(val, begin, end, vector_cmp, cmp) do { \
	uint64_t found; \
	uint8_t *i; \
	for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) { \
				/* check against every possible alignment */ \
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) { \
					vector_cmp(i+j, val, found); \
					if( found != 0 ) { \
						/* where exactly? */ \
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) { \
							if(*((T*) ((uint8_t*)i+j+k)) cmp val[0]) \
								_vec_results.push_back((T*) ((uint8_t*)i+j+k)); \
						} \
					} \
				} \
			} \
			/* check also the rest */ \
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) { \
				if(*((T*) ((uint8_t*)i+k)) cmp val[0]) \
					_vec_results.push_back((T*) ((uint8_t*)i+k)); \
			} \
		} while(0)

enum search_type {
	search_equal = 0,
	search_notequal,
	search_gt,
	search_ge,
	search_lt,
	search_le,
	search_unchanged
};

template<class T>
class memscan {
protected:
#if defined(linux) || defined(__APPLE__)
	T _val[16/sizeof(T)] __attribute__((aligned(16)));
#else
	__declspec(align(16)) T _val[16/sizeof(T)];
#endif
	std::vector<T*> _vec_results;

	bool _is_signed() {
		return std::numeric_limits<T>::is_signed;
	}

public:
	typedef typename std::vector<T*>::iterator iterator;
	memscan() {}

	iterator begin() {
		return _vec_results.begin();
	}

	iterator end() {
		return _vec_results.end();
	}

	size_t size() const {
		return _vec_results.size();
	}
	uint32_t search_unaligned(T val, void *begin, void *end, search_type type) {
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge) {
				_val[j] = val-1;
			}
			else if(type == search_le) {
				_val[j] = val+1;
			}
			else {
				_val[j] = val;
			}
		}

		switch(sizeof(T)) {
			case 1:
				if(type == search_equal) {
					SEARCH_UNALIGNED(_val, begin, end, CHECK_BYTE, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_BYTE_SIGNED_GT, >);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_BYTE_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_BYTE_SIGNED_LT, <);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_BYTE_UNSIGNED_LT, <);
				}
				break;
			case 2:
				if(type == search_equal) {
					SEARCH_UNALIGNED(_val, begin, end, CHECK_WORD, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_WORD_SIGNED_GT, >);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_WORD_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_WORD_SIGNED_LT, <);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_WORD_UNSIGNED_LT, <);
				}
				break;
			case 4:
				if(type == search_equal) {
					SEARCH_UNALIGNED(_val, begin, end, CHECK_DWORD, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_DWORD_SIGNED_GT, >);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_DWORD_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_DWORD_SIGNED_LT, <);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_DWORD_UNSIGNED_LT, <);
				}
				break;
			case 8:
				if(type == search_equal) {
					SEARCH_UNALIGNED(_val, begin, end, CHECK_QWORD, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_QWORD_SIGNED_GT, >);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_QWORD_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_UNALIGNED(_val, begin, end, CHECK_QWORD_SIGNED_LT, <);
					else
						SEARCH_UNALIGNED(_val, begin, end, CHECK_QWORD_UNSIGNED_LT, <);
				}
				break;
		}
		return _vec_results.size();
	}

	uint32_t search_aligned(T val, void *mem_begin, void *end, search_type type) {
		T *begin = (T*) ((unsigned long) mem_begin & ~(sizeof(T)-1));
		if((unsigned long) mem_begin & (sizeof(T)-1))
			++begin;
		
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge) {
				_val[j] = val-1;
			}
			else if(type == search_le) {
				_val[j] = val+1;
			}
			else {
				_val[j] = val;
			}
		}

		switch(sizeof(T)) {
			case 1:
				if(type == search_equal) {
					SEARCH_ALIGNED(_val, begin, end, CHECK_BYTE, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_BYTE_SIGNED_GT, >);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_BYTE_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_BYTE_SIGNED_LT, <);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_BYTE_UNSIGNED_LT, <);
				}
				break;
			case 2:
				if(type == search_equal) {
					SEARCH_ALIGNED(_val, begin, end, CHECK_WORD, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_WORD_SIGNED_GT, >);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_WORD_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_WORD_SIGNED_LT, <);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_WORD_UNSIGNED_LT, <);
				}
				break;
			case 4:
				if(type == search_equal) {
					SEARCH_ALIGNED(_val, begin, end, CHECK_DWORD, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_DWORD_SIGNED_GT, >);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_DWORD_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_DWORD_SIGNED_LT, <);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_DWORD_UNSIGNED_LT, <);
				}
				break;
			case 8:
				if(type == search_equal) {
					SEARCH_ALIGNED(_val, begin, end, CHECK_QWORD, ==);
				}
				else if(type == search_gt || type == search_ge) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_QWORD_SIGNED_GT, >);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_QWORD_UNSIGNED_GT, >);
				}
				else if(type == search_lt || type == search_le) {
					if(_is_signed())
						SEARCH_ALIGNED(_val, begin, end, CHECK_QWORD_SIGNED_LT, <);
					else
						SEARCH_ALIGNED(_val, begin, end, CHECK_QWORD_UNSIGNED_LT, <);
				}
				break;
		}
		return _vec_results.size();
	}

	uint32_t search_cont(T val, search_type type) {
		typename std::vector<T*> vec;
		typename std::vector<T*>::iterator it;
		switch(type) {
			case search_equal:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it == val )
						vec.push_back(*it);
				}
				break;
			case search_notequal:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it != val )
						vec.push_back(*it);
				}
				break;
			case search_gt:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it > val )
						vec.push_back(*it);
				}
				break;
			case search_ge:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it >= val )
						vec.push_back(*it);
				}
				break;
			case search_lt:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it < val )
						vec.push_back(*it);
				}
				break;
			case search_le:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it <= val )
						vec.push_back(*it);
				}
				break;
			case search_unchanged:
				for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
					if( **it == _val )
						vec.push_back(*it);
				}
				break;
			default:
				break;
		}

		_vec_results = vec;
	}

	uint32_t standard_search(T val, T *begin, T *end, search_type type) {
		switch(type) {
			case search_equal:
				for(T *i = begin; i < end; ++i) {
					if( *i == val )
						_vec_results.push_back(i);
				}
			break;
			case search_gt:
				for(T *i = begin; i < end; ++i) {
					if( *i > val )
						_vec_results.push_back(i);
				}
			break;
			case search_ge:
				for(T *i = begin; i < end; ++i) {
					if( *i >= val )
						_vec_results.push_back(i);
				}
			break;
			case search_lt:
				for(T *i = begin; i < end; ++i) {
					if( *i < val )
						_vec_results.push_back(i);
				}
			break;
			case search_le:
				for(T *i = begin; i < end; ++i) {
					if( *i <= val )
						_vec_results.push_back(i);
				}
			break;
		}
		return _vec_results.size();
	}
};
#endif
