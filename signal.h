#ifndef _SIGNAL_H_
#define _SIGNAL_H_

typedef int sig_atomic_t;
typedef void (*signal_handler_t)(int param);

#define SIGINT		 2
#define SIGILL		 4
#define SIGABRT		 6
#define SIGFPE		 8
#define SIGSEGV		11
#define SIGTERM		15

#define SIG_DFL	((signal_handler_t)0)
#define SIG_IGN	((signal_handler_t)1)

#ifdef __cplusplus
extern "C" {
#endif

signal_handler_t signal(int sig, signal_handler_t func);
int raise(int sig);

#ifdef __cplusplus
}
#endif

#endif