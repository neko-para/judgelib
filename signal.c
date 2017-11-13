#include "signal.h"
#include "syscall.h"

signal_handler_t signal(int sig, signal_handler_t func) {
	// return (signal_handler_t)syscall(__NR_signal, sig, func, 0, 0);
}

int raise(int sig) {
	return (int)syscall(__NR_kill, syscall(__NR_getpid), sig);
}