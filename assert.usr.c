#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

int __assert__(int line, const char* file, const char* exp) {
	printf("Assertion failed: %s, file %s, line %d\n", exp, file, line);
	abort();
	return 0;
}