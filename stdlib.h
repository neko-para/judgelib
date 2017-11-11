#ifndef _STDLIB_H_
#define _STDLIB_H_

#define RAND_MAX 32768

#ifdef __cplusplus
extern "C" {
#endif
/* TODO: add malloc & calloc & free */
int rand();
void srand(unsigned seed);
void abort() __attribute__ ((__noreturn__));
void exit(int status) __attribute__ ((__noreturn__));
void _exit(int status) __attribute__ ((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif