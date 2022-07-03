#include <stdio.h>
#include <stdlib.h>

void fib(unsigned int n, unsigned int *f);

int main(int argc, char **argv) {
	unsigned int n = atoi(argv[1]);
	unsigned int f;

	fib(n, &f);

	printf("%unth fib: %u\n", n, f);

	return 0;
}

