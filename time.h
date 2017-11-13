#ifndef _TIME_H_
#define _TIME_H_

#include <stddef.h>

#define CLOCKS_PER_SEC 100

typedef long time_t;
typedef long clock_t;

#ifdef __cplusplus
extern "C" {
#endif

time_t time(time_t* loc);
clock_t clock();

#ifdef __cplusplus
}
#endif

#endif