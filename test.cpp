#include <stdio.h>
#include <time.h>

int main() {
	clock_t b = clock();
	for (int i = 0; i <= 1000000; ++i) {
		printf("%d%f", 5, 6.78);
	}
	fprintf(stderr, "%f", double(clock() - b) / CLOCKS_PER_SEC);
}