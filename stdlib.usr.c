#include "stdlib.h"
#include "signal.h"
#include "sys.h"

void abort() {
	raise(SIGABRT);
	exit(0);
}

void exit(int status) {
	_sys__exit(status);
}