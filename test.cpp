#include <stdio.h>
#include <time.h>

int main() {
	char buf[256];
	sprintf(buf, "%-+8.4f", 5.43);
	printf("%s", buf);
}