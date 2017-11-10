#include "signal.h"
#include "sys.h"

signal_handler_t signal(int sig, signal_handler_t func) {
	return _sys_signal(sig, func);
}

int raise(int sig) {
	return _sys_raise(sig);
}