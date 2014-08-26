#ifndef _MEMSCAN_H_
#define _MEMSCAN_H_

#include <vector>
#include <stdint.h>
#include <stdlib.h>

#if defined(linux) || defined(__APPLE__)
/* SSE */
#define CHECK_BYTE(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_WORD(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpeqq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_BYTE_GT(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_WORD_GT(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD_GT(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD_GT(dst,src,flag) asm volatile("movaps (%2), %%xmm0\n\t" \
		"movups (%1), %%xmm1\n\t" \
		"prefetch 0x10(%1)\n\t" \
		"pcmpgtq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_BYTE_LE(dst,src,flag) asm volatile("movaps (%1), %%xmm0\n\t" \
		"movups (%2), %%xmm1\n\t" \
		"prefetch 0x10(%2)\n\t" \
		"pcmpgtb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")
#define CHECK_WORD_LE(dst,src,flag) asm volatile("movaps (%1), %%xmm0\n\t" \
		"movups (%2), %%xmm1\n\t" \
		"prefetch 0x10(%2)\n\t" \
		"pcmpgtw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD_LE(dst,src,flag) asm volatile("movaps (%1), %%xmm0\n\t" \
		"movups (%2), %%xmm1\n\t" \
		"prefetch 0x10(%2)\n\t" \
		"pcmpgtd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD_LE(dst,src,flag) asm volatile("movaps (%1), %%xmm0\n\t" \
		"movups (%2), %%xmm1\n\t" \
		"prefetch 0x10(%2)\n\t" \
		"pcmpgtq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"orpd %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "r"(dst), "r"(src) \
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
#define CHECK_BYTE(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					prefetch [dst+0x10]; \
					pcmpeqb xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_WORD(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					prefetch [dst+0x10]; \
					pcmpeqw xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_DWORD(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					prefetch [dst+0x10]; \
					pcmpeqd xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_QWORD(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					prefetch [dst+0x10]; \
					pcmpeqq xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}
#define CHECK_BYTE_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtb xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_WORD_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtw xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_DWORD_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtd xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_QWORD_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtq xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}
#define CHECK_BYTE_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtb xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_WORD_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtw xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_DWORD_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtd xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_QWORD_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtq xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					orpd xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}
#endif

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

	uint32_t _search_byte(T val, T *begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_BYTE(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_BYTE_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_BYTE_LE(_val, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
	}

	uint32_t _search_word(T val, T *begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_WORD(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_WORD_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_WORD_LE(_val, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
	}

	uint32_t _search_dword(T val, T *begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_DWORD(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_DWORD_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_DWORD_LE(_val, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
		}

		return _vec_results.size();
	}

	uint32_t _search_qword(T val, T *begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_QWORD(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_QWORD_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T) && j+i+16 < (uint8_t*) end-sizeof(T); ++j) {
					CHECK_QWORD_LE(_val, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
	}

	uint32_t _search_aligned_byte(T val, T *mem_begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		T *begin = (T*) ((unsigned long) mem_begin & ~(sizeof(T)-1));
		if((unsigned long) mem_begin & (sizeof(T)-1))
			++begin;
		
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_BYTE(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) == _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_BYTE_GT(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) > _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_BYTE_LE(_val, i, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) <= _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
	}

	uint32_t _search_aligned_word(T val, T *mem_begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		T *begin = (T*) ((unsigned long) mem_begin & ~(sizeof(T)-1));
		if((unsigned long) mem_begin & (sizeof(T)-1))
			++begin;
		
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				CHECK_WORD(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) == _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				CHECK_WORD_GT(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) > _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				CHECK_WORD_LE(_val, i, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) <= _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
	}


	uint32_t _search_aligned_dword(T val, T *mem_begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		T *begin = (T*) ((unsigned long) mem_begin & ~(sizeof(T)-1));
		if((unsigned long) mem_begin & (sizeof(T)-1))
			++begin;
		
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				CHECK_DWORD(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) == _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_DWORD_GT(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) > _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_DWORD_LE(_val, i, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) <= _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
	}


	uint32_t _search_aligned_qword(T val, T *mem_begin, T *end, search_type type) {
		uint64_t found = 0;
		uint8_t *i;
		T *begin = (T*) ((unsigned long) mem_begin & ~(sizeof(T)-1));
		if((unsigned long) mem_begin & (sizeof(T)-1))
			++begin;
		
		/* move value to our aligned member variable */
		for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
			if(type == search_ge || type == search_lt) {
				_val[j] = val-1;
			}
			else {
				_val[j] = val;
			}
		}

		if(type == search_equal) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_QWORD(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) == _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) == _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_gt || type == search_ge) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_QWORD_GT(i, _val, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) > _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) > _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}
		else if(type == search_lt || type == search_le) {
			for(i = (uint8_t*) begin; i < (uint8_t*) end-15; i += 16) {
				/* check against every possible alignment */
				CHECK_QWORD_LE(_val, i, found);
				if( found != 0 ) {
					/* where exactly? */
					for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
						if(*((T*) ((uint8_t*)i+k)) <= _val[0])
							_vec_results.push_back((T*) ((uint8_t*)i+k));
					}
				}
			}
			/* check also the rest */
			for(uint8_t k = 0; k < (uint8_t*) end-i; k += sizeof(T)) {
				if(*((T*) ((uint8_t*)i+k)) <= _val[0])
					_vec_results.push_back((T*) ((uint8_t*)i+k));
			}
		}

		return _vec_results.size();
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

	uint32_t search(T val, void *begin, void *end, search_type type) {
		switch(sizeof(T)) {
			case 1:
				return _search_byte(val, (T*) begin, (T*) end, type);
				break;
			case 2:
				return _search_word(val, (T*) begin, (T*) end, type);
				break;
			case 4:
				return _search_dword(val, (T*) begin, (T*) end, type);
				break;
			case 8:
				return _search_qword(val, (T*) begin, (T*) end, type);
				break;
		}
	}

	uint32_t search_aligned(T val, void *begin, void *end, search_type type) {
		switch(sizeof(T)) {
			case 1:
				return _search_aligned_byte(val, (T*) begin, (T*) end, type);
				break;
			case 2:
				return _search_aligned_word(val, (T*) begin, (T*) end, type);
				break;
			case 4:
				return _search_aligned_dword(val, (T*) begin, (T*) end, type);
				break;
			case 8:
				return _search_aligned_qword(val, (T*) begin, (T*) end, type);
				break;
		}
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
