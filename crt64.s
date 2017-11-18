.globl _start

_start:
	call __judge_lib_init_stdlib
	call __judge_lib_init_stdio
	call main
	movq $0, %r8
	call exit
	ret
