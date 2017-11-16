.globl _start

_start:
	call __judge_lib_init
	call main
	pushl $0
	call exit
	ret
