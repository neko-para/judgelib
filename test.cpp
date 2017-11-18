#include <stdio.h>
#include <stdlib.h>

int main() {
	int sum = 0;
	int x;
	for (int i = 0; i < 5000000; ++i) {
		scanf("%i", &x);
		sum += x;
		// printf("%d ", i);
	}
	return 0;
}