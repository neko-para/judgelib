#include "time.h"
#include "syscall.h"

time_t time(time_t* loc) {
	return syscall(__NR_time, (long)loc, 0, 0);
}

clock_t clock() {
	long dat[4];
	syscall(__NR_times, (long)dat, 0, 0);
	return dat[0] + dat[1];
}