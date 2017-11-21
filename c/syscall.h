#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

#if _JUDGE_BIT_ == 32
#include <asm/unistd_32.h>

static long syscall(long id, long p1, long p2, long p3) {
	long ret;
	asm volatile (
		"movl %1, %%eax;"
		"movl %2, %%ebx;"
		"movl %3, %%ecx;"
		"movl %4, %%edx;"
		"int $0x80;"
		"movl %%eax, %0;"
		: "=m" (ret)
		: "m" (id), "m" (p1), "m" (p2), "m" (p3)
		: "ebx"
	);
	return ret;
}
#else
#include <asm/unistd_64.h>
#include <unistd.h>
#endif

#endif