#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#define SIGINT 2
#define SIGILL 4
#define SIGABRT 6
#define SIGFPE 8
#define SIGSEGV 11
#define SIGTERM 15

#ifdef __cplusplus
extern "C" {
#endif

int raise(int sig);

#ifdef __cplusplus
}
#endif

#endif