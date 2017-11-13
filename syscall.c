#include "syscall.h"

#if _JUDGE_BIT_ == 32

long syscall(long id, long p1, long p2, long p3) {
	long ret;
	asm __volatile__ (
		"int $0x80;"
		"movl %%eax, %0;"
		: "=r" (ret)
		: "a" (id), "b" (p1), "c" (p2), "d" (p3)
	);
	return ret;
}

#endif