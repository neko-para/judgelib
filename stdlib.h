#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>

#define RAND_MAX 32768

typedef struct {
	int quot;
	int rem;
} div_t;

typedef struct {
	long quot;
	long rem;
} ldiv_t;

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
int abs(int n);
long labs(long n);
div_t div(int numerator, int denominator);
ldiv_t ldiv(long numerator, long denominator);
void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*, const void*));
void qsort(void* base, size_t num, size_t size, int (*compar)(const void*, const void*));
#define atof(str) ((float)strtod(str, NULL))
#define atoi(str) ((int)atoll(str))
#define atol(str) ((int)atoll(str))
long long atoll(const char* str);
#define strtof(str, end) ((float)strtold(str, end))
#define strtod(str, end) ((double)strtold(str, end))
long double strtold(const char* str, char** endptr);
#define strtol(str, end, base) ((long)strtoll(str, end, base))
long long strtoll(const char* str, char** endptr, int base);
#define strtoul(str, end, base) ((unsigned long)strtoull(str, end, base))
unsigned long long strtoull(const char* str, char** endptr, int base);
#ifdef __cplusplus
}
#endif

#endif