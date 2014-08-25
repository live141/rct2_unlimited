#ifndef _MEMSCAN_H_
#define _MEMSCAN_H_

#include <vector>
#include <stdint.h>

#if defined(linux) || defined(__APPLE__)
#define CHECK_BYTE(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups %1, %%xmm1\n\t" \
		"pcmpeqb %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"paddq %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "m"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_WORD(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups %1, %%xmm1\n\t" \
		"pcmpeqw %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"paddq %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "m"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_DWORD(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups %1, %%xmm1\n\t" \
		"pcmpeqd %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"paddq %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "m"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")

#define CHECK_QWORD(dst,src,flag) asm volatile("movaps %2, %%xmm0\n\t" \
		"movups %1, %%xmm1\n\t" \
		"pcmpeqq %%xmm0, %%xmm1\n\t" \
		"movaps %%xmm1, %%xmm3\n\t" \
		"pshufd $0xee, %%xmm1, %%xmm3\n\t" \
		"paddq %%xmm1, %%xmm3\n\t" \
		"pextrq $0, %%xmm3, %0\n\t" \
		: "=m"(flag) \
		: "m"(dst), "m"(src) \
		: "xmm0", "xmm1", "xmm3")
#else
#define CHECK_BYTE(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpeqb xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_WORD(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpeqw xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_DWORD(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpeqd xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_QWORD(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpeqq xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}
#define CHECK_BYTE_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtb xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_WORD_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtw xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_DWORD_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtd xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_QWORD_GT(dst,src,flag) __asm__ volatile { \
					movaps xmm0, src; \
					movups xmm1, dst; \
					pcmpgtq xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}
#define CHECK_BYTE_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm1, src; \
					movups xmm0, dst; \
					pcmpgtb xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_WORD_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm1, src; \
					movups xmm0, dst; \
					pcmpgtw xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_DWORD_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm1, src; \
					movups xmm0, dst; \
					pcmpgtd xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
					pextrq flag, xmm3, 0; \
				}

#define CHECK_QWORD_LE(dst,src,flag) __asm__ volatile { \
					movaps xmm1, src; \
					movups xmm0, dst; \
					pcmpgtq xmm1, xmm0; \
					movaps xmm3, xmm1; \
					pushfd xmm3, xmm1, 0xee; \
					paddq xmm3, xmm1; \
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

public:
	memscan() {}
	uint32_t search_byte(T val, T *begin, T *end, search_type type);
	uint32_t search_word(T val, T *begin, T *end, search_type type);
	uint32_t search_dword(T val, T *begin, T *end, search_type type);
	uint32_t search_qword(T val, T *begin, T *end, search_type type);


	uint32_t search_cont(search_type type, T val = 0);

	std::vector<T*>& results() {
		return _vec_results;
	}
};

#endif
