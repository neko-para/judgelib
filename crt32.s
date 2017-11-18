.globl _start

_start:
	call __judge_lib_init_stdlib
	call __judge_lib_init_stdio
	call main
	pushl $0
	call exit
	ret
