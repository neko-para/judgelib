#ifndef _STDLIB_H_
#define _STDLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

void abort() __attribute__ ((__noreturn__));
void exit(int status) __attribute__ ((__noreturn__));
void _exit(int status) __attribute__ ((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif