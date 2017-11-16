.globl _start

_start:
	call __judge_lib_init
	call main
	movq $0, %r8
	call exit
	ret
