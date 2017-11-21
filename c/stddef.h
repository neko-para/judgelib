#ifndef _STDDEF_H_
#define _STDDEF_H_

typedef long ptrdiff_t;
typedef unsigned long size_t;

#define NULL ((void*)0)

#define offsetof(type, member) __builtin_offsetof(type, member)

#endif