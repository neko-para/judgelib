.globl _start

_start:
	call __judge_lib_init
	call main
	movl %eax, %ebx
	movl $1, %eax
	int $0x80
	ret
