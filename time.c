#include "time.h"
#include "syscall.h"

time_t time(time_t* loc) {
	return syscall(__NR_time, (long)loc);
}

clock_t clock();