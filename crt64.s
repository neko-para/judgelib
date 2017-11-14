.globl _start

_start:
	call __judge_lib_init
	call main
	movq %rax, %rdi
	movq $60, %rax
	syscall
	ret
