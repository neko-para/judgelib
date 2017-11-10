#ifndef _STDIO_H_
#define _STDIO_H_

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

#define BUFSIZ 8192


/* All file function are forbidden. */

#ifdef __cplusplus
extern "C" {
#endif

extern struct FILE* stdin;
extern struct FILE* stdout;
extern struct FILE* stderr;

int fflush(struct FILE* stream);
size_t fread(void* ptr, size_t size, size_t nmemb, struct FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, struct FILE* stream);

#ifdef __cplusplus
}
#endif

#endif