#include "string.h"
#include "stdlib.h"

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

void* memmove(void* dest, const void* src, size_t n) {
	void* temp = malloc(n);
	memcpy(temp, src, n);
	memcpy(dest, temp, n);
	free(temp);
	return dest;
}

char* strcpy(char* dest, const char* src) {
	char* p = dest;
	while (*src) {
		*dest++ = *src++;
	}
	return p;
}

char* strncpy(char* dest, const char* src, size_t n) {
	char* p = dest;
	while (n--) {
		if (!(*dest++ = *src++)) {
			break;
		}
	}
	return p;
}

char* strcat(char* dest, const char* src) {
	char* p = dest;
	while (*dest) {
		++dest;
	}
	while (*src) {
		*dest++ = *src++;
	}
	return p;
}

char* strncat(char* dest, const char* src, size_t n) {
	char* p = dest;
	while (*dest) {
		++dest;
	}
	while (n--) {
		if (!(*dest++ = *src++)) {
			--dest;
			break;
		}
	}
	*dest = 0;
	return p;
}

int memcmp(const void* s1, const void* s2, size_t n) {
	size_t nn = n >> 2;
	while (nn--) {
		if (*(const unsigned*)s1 != *(const unsigned*)s2) {
			return *(const unsigned*)s1 > *(const unsigned*)s2 ? 1 : -1;
		}
		s1 += 4;
		s2 += 4;
	}
	n &= 3;
	while (n--) {
		if (*(const unsigned char*)s1 != *(const unsigned char*)s2) {
			return *(const unsigned char*)s1 > *(const unsigned char*)s2 ? 1 : -1;
		}
		++s1;
		++s2;
	}
	return 0;
}

int strcmp(const char* s1, const char* s2) {
	while (*s1 || *s2) {
		if (*s1 != *s2) {
			return *s1 > *s2 ? 1 : -1;
		}
		++s1;
		++s2;
	}
	return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
	while ((*s1 || *s2) && n--) {
		if (*s1 != *s2) {
			return *s1 > *s2 ? 1 : -1;
		}
		++s1;
		++s2;
	}
	return 0;
}

void* memchr(const void* s, int c, size_t n) {
	while (n--) {
		if (*((unsigned char*)s) == c) {
			return (void*)s;
		} else {
			++s;
		}
	}
	return NULL;
}

char* strchr(const char* s, int c) {
	while (*s != c && *s) {
		++s;
	}
	return *s == c ? (char*)s : NULL;
}

char* strrchr(const char* s, int c) {
	char* p = NULL;
	while (*s) {
		if (*s == c) {
			p = (char*)s;
		}
		++s;
	}
	return p;
}

size_t strspn(const char* s, const char* accept) {
	unsigned char mp[256] = {0};
	size_t n = 0;
	while (*accept) {
		mp[*accept++] = 1;
	}
	while (mp[*s++]) {
		++n;
	}
	return n;
}

size_t strcspn(const char* s, const char* accept) {
	unsigned char mp[256] = {0};
	size_t n = 0;
	while (*accept) {
		mp[*accept++] = 1;
	}
	while (!mp[*s++]) {
		++n;
	}
	return n;
}

char *strpbrk(const char *s, const char *accept) {
	size_t n = strcspn(s, accept);
	return s[n] ? (char*)s + n : NULL;
}

char* strstr(const char* haystack, const char* needle) {
	size_t hl = strlen(haystack);
	size_t nl = strlen(needle);
	if (nl > hl) {
		return NULL;
	}
	const char* end = haystack + hl - nl;
	for (const char* p = haystack; p != end; ++p) {
		int f = 1;
		for (size_t i = 0; i < nl; ++i) {
			if (p[i] != needle[i]) {
				f = 0;
				break;
			}
		}
		if (f) {
			return (char*)p;
		}
	}
	return NULL;
}

char* strtok(char* str, const char* delim) {
	static char* ptr;
	if (str) {
		str += strspn(str, delim);
		if (*str) {
			char* p = str;
			ptr = strpbrk(str, delim);
			if (ptr) {
				*ptr++ = 0;
			}
			return p;
		} else {
			ptr = NULL;
			return NULL;
		}
	} else {
		if (ptr) {
			ptr += strspn(ptr, delim);
			if (*ptr) {
				char* p = ptr;
				ptr = strpbrk(ptr, delim);
				if (ptr) {
					*ptr++ = 0;
				}
				return p;
			} else {
				ptr = NULL;
				return NULL;
			}
		} else {
			return NULL;
		}
	}
}

void* memset(void* s, int c, size_t n) {
	void* p = s;
	size_t nn = n >> 2;
	unsigned v = c & 255;
	v |= v << 8;
	v |= v << 16;
	while (nn--) {
		*(unsigned*)s++ = v;
	}
	n &= 3;
	while (n--) {
		*(unsigned char*)s++ = (unsigned char)v;
	}
	return p;
}

size_t strlen(const char* s) {
	size_t l = 0;
	while (*s++) {
		++l;
	}
	return l;
}