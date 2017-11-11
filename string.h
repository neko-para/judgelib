#ifndef _STRING_H_
#define _STRING_H_

#ifndef _SIZE_T_DEF_
#define _SIZE_T_DEF_
typedef unsigned long size_t;
#endif

void* memcpy(void* dest, const void* src, size_t n);
size_t strlen(const char* s);

#endif