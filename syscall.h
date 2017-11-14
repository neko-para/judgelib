#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

#if _JUDGE_BIT_ == 32
#include <asm/unistd_32.h>

static long syscall(long id, long p1, long p2, long p3) {
	asm volatile (
		"movl %0, %%eax;"
		"movl %1, %%ebx;"
		"movl %2, %%ecx;"
		"movl %3, %%edx;"
		"int $0x80;"
		:
		: "m" (id), "m" (p1), "m" (p2), "m" (p3)
		: "ebx"
	);
}
#else
#include <asm/unistd_64.h>
#include <unistd.h>
#endif

#endif