#include "string.h"

void* memcpy(void* dest, const void* src, size_t n) {
	size_t nn = n >> 2;
	void* ret = dest;
	while (nn--) {
		*(unsigned*)dest++ = *(const unsigned*)src++;
	}
	n &= 3;
	while (n--) {
		*(unsigned char*)dest++ = *(const unsigned char*)src++;
	}
	return ret;
}

size_t strlen(const char* s) {
	size_t l = 0;
	while (*s++) {
		++l;
	}
	return l;
}