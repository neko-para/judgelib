#ifndef _TIME_H_
#define _TIME_H_

#include <stddef.h>

typedef long time_t;
typedef long clock_t;

time_t time(time_t* loc);
clock_t clock();

#endif