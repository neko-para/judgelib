#include "stdio.h"
#include "string.h"
#include "sys.h"

struct FILE* stdin = (struct FILE*)0;
struct FILE* stdout = (struct FILE*)1;
struct FILE* stderr = (struct FILE*)2;

int fflush(struct FILE* stream) {
	return _sys_fflush((int)stream);
}

size_t fread(void* ptr, size_t size, size_t nmemb, struct FILE* stream) {
	return _sys_fread(ptr, size, nmemb, (int)stream);
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, struct FILE* stream) {
	return _sys_fwrite(ptr, size, nmemb, (int)stream);
}

int printf(const char* format, ...) {
	va_list ap;
	int ret;
	va_start(ap, format);
	ret = vprintf(format, ap);
	va_end(ap);
	return ret;
}

int vprintf(const char* format, va_list ap) {
	
}

int sprintf(char* str, const char* format, ...) {
	va_list ap;
	int ret;
	va_start(ap, format);
	ret = vsprintf(str, format, ap);
	va_end(ap);
	return ret;
}

int vsprintf(char* str, const char* format, va_list ap) {

}

int fgetc(struct FILE* stream) {
	unsigned char ch;
	return fread(&ch, 1, 1, stream) == 1 ? ch : EOF;
}

char *fgets(char* s, int size, struct FILE* stream) {
	char* t = s;
	--size;
	while (size) {
		*s = fgetc(stream);
		if (*s == EOF) {
			*s = 0;
			return t == s ? 0 : t;
		}
		if (*s == '\n') {
			*++s = 0;
			return t;
		}
		++s;
	}
	*s = 0;
	return t;
}

int fputc(int c, struct FILE* stream) {
	return fwrite(&c, 1, 1, stream) == 1 ? c : EOF;
}

int fputs(const char* s, struct FILE* stream) {
	size_t l = strlen(s);
	return fwrite(s, 1, l, stream) == l ? 0 : EOF;
}