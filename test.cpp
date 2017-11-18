#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>

int main() {
	struct stat st;
	fstat(0, &st);
	printf("%ld %ld", sizeof(struct stat), offsetof(struct stat, st_size));
}