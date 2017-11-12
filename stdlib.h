#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>

#define RAND_MAX 32768

#ifdef __cplusplus
extern "C" {
#endif
int rand();
void srand(unsigned seed);
void* malloc(size_t size);
void free(void* ptr);
void abort() __attribute__ ((__noreturn__));
void exit(int status) __attribute__ ((__noreturn__));
void _exit(int status) __attribute__ ((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif