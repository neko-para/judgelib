#include <stdlib.h>
#include <new>

void* operator new(size_t size) {
	return malloc(size);
}

void* operator new(size_t size, void* ptr) {
	return ptr;
}

void* operator new[](size_t size) {
	return malloc(size);
}

void* operator new[](size_t size, void* ptr) {
	return ptr;
}

void operator delete(void* ptr) {
	free(ptr);
}

void operator delete[](void* ptr) {
	free(ptr);
}