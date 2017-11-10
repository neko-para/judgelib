#include "stdio.h"
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