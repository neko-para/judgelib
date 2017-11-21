#ifndef _ASSERT_H_
#define _ASSERT_H_

#ifdef NDEBUG
#define assert(exp) (0)
#else
#define assert(exp) \
	((exp) ? 0 : __assert__(__LINE__, __FILE__, #exp))
#endif

#ifdef __cplusplus
extern "C" {
#endif

int __assert__(int line, const char* file, const char* exp);

#ifdef __cplusplus
}
#endif

#endif