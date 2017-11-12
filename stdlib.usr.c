#include "stdlib.h"
#include "signal.h"
#include "sys.h"

static void (*atexit_handler)(void);

unsigned long next = 1;
int rand() {
	next = next * 1103515245 + 12345;
	return (unsigned)(next >> 16) & 32767;
}

void srand(unsigned seed) {
	next = seed;
}

void* malloc(size_t size) {
	return _sys_malloc(size);
}

void free(void* ptr) {
	return _sys_free(ptr);
}

void* realloc(void* ptr, size_t len) {
	return _sys_realloc(ptr, len);
}

void abort() {
	raise(SIGABRT);
	exit(0);
}

int atexit(void (*func)(void)) {
	atexit_handler = func;
	return 0;
}

void exit(int status) {
	atexit_handler ? atexit_handler() : (void)0;
	_exit(status);
}

void _exit(int status) {
	_sys__exit(status);
}