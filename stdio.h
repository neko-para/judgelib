#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

#ifndef NULL
#ifndef __cplusplus
#define NULL ((void*)0)
#else
#define NULL (0)
#endif
#endif

#ifndef _SIZE_T_DEF_
#define _SIZE_T_DEF_
typedef unsigned long size_t;
#endif

#define EOF ((int)-1)

#ifdef __cplusplus
extern "C" {
#endif

extern struct FILE* stdin;
extern struct FILE* stdout;
extern struct FILE* stderr;

int fflush(struct FILE* stream);
size_t fread(void* ptr, size_t size, size_t nmemb, struct FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, struct FILE* stream);

int scanf(const char* format, ...);
int sscanf(const char* str, const char* format, ...);

int printf(const char* format, ...);
int vprintf(const char* format, va_list ap);
int sprintf(char* str, const char* format, ...);
int vsprintf(char* str, const char* format, va_list ap);

int fgetc(struct FILE* stream);
char *fgets(char* s, int size, struct FILE* stream);
int fputc(int c, struct FILE* stream);
int fputs(const char* s, struct FILE* stream);


#ifdef __cplusplus
}
#endif

#endif