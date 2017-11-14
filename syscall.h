#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

#if _JUDGE_BIT_ == 32
#include <asm/unistd_32.h>
#else
#include <asm/unistd_64.h>
#endif
#include <unistd.h>

#endif