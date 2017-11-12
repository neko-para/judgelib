#include "stdio.h"
#include "stdlib.h"

int main() {
	void* p = malloc(120);
	printf("%p\n", p);
	printf("%p\n", malloc(12));
	free(p);
	printf("%p\n", malloc(10));
	printf("%p\n", malloc(10));
}