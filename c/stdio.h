#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>
#include <stddef.h>

#define EOF ((int)-1)

#ifdef __cplusplus
extern "C" {
#endif

struct FILE;

#define stdin ((struct FILE*)0)
#define stdout ((struct FILE*)1)
#define stderr ((struct FILE*)2)

int fflush(struct FILE* stream);
size_t fread(void* ptr, size_t size, size_t nmemb, struct FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, struct FILE* stream);

int fscanf(struct FILE* stream, const char* format, ...);
int vfscanf(struct FILE* stream, const char* format, va_list ap);
#define scanf(...) fscanf(stdin, __VA_ARGS__)
#define vscanf(...) vfscanf(stdin, __VA_ARGS__)
int sscanf(const char* str, const char* format, ...);
int vsscanf(const char* str, const char* format, va_list ap);

int fprintf(struct FILE* stream, const char* format, ...);
int vfprintf(struct FILE* stream, const char* format, va_list ap);
#define printf(...) fprintf(stdout, __VA_ARGS__)
#define vprintf(...) vfprintf(stdout, __VA_ARGS__)
int sprintf(char* str, const char* format, ...);
int vsprintf(char* str, const char* format, va_list ap);

int fgetc(struct FILE* stream);
char* fgets(char* s, int size, struct FILE* stream);
#define getc(f) fgetc(f)
#define getchar() fgetc(stdin)
char* gets(char* s);
int fputc(int c, struct FILE* stream);
int fputs(const char* s, struct FILE* stream);
#define putc(c, f) fputc(c, f)
#define putchar(c) fputc(c, stdout);
int puts(const char* s);
int ungetc(int c, struct FILE* stream);

#ifdef __cplusplus
}
#endif

#endif