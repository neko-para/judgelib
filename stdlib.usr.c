#include "stdlib.h"
#include "signal.h"
#include "sys.h"

void abort() {
	raise(SIGABRT);
	exit(0);
}

void exit(int ret) {
	_sys_exit(0);
}