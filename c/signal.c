#include "signal.h"
#include "syscall.h"

int raise(int sig) {
	return (int)syscall(__NR_kill, 0, sig, 0);
}