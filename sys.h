#ifndef _SYS_H_
#define _SYS_H_
#include "stdarg.h"

typedef unsigned long size_t;

void _sys__exit(int status);
void (*_sys_signal(int sig, void (*func)(int)))(int);
int _sys_raise(int sig);
int _sys_fflush(int fd);
size_t _sys_fread(void* ptr, size_t size, size_t nmemb, int fd);
size_t _sys_fwrite(const void* ptr, size_t size, size_t nmemb, int fd);

#endif