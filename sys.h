#ifndef _SYS_H_
#define _SYS_H_

void _sys_exit(int ret) __attribute__ ((__noreturn__));
void (*_sys_signal(int sig, void (*func)(int)))(int);
int _sys_raise(int sig);

#endif