#include <unistd.h>
#include "sys.h"

void _sys__exit(int status) {
	__asm__(
		"movl $1, %%eax;"
		"movl %0, %%ebx;"
		"int $0x80;"
		:
		: "r" (status)
	);
}