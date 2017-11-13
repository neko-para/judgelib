#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

#if _JUDGE_BIT_ == 32
#include <asm/unistd_32.h>
#ifdef __cplusplus
extern "C"
#endif
long syscall(long id, long p1, long p2, long p3);
#else
#include <asm/unistd_64.h>
#include <unistd.h>
#endif

#endif