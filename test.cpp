#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <new>

int main() {
	// new int;
	char* str = new char[9];
	strcpy(str, "Hello");
	printf("%s", str);
}