#include <signal.h>
#include "sys.h"

void (*_sys_signal(int sig, void (*func)(int)))(int) {
	return signal(sig, func);
}

int _sys_raise(int sig) {
	return kill(0, sig);
}