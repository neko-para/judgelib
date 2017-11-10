#include "stdlib.h"
#include "signal.h"
#include "sys.h"

static void (*atexit_handler)(void);

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