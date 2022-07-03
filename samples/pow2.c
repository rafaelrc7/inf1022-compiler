#include <stdio.h>
#include <stdlib.h>

void pow2(unsigned int n, unsigned int *p);

int main(int argc, char **argv) {
	unsigned int n = atoi(argv[1]);
	unsigned int p;

	pow2(n, &p);

	printf("2^%u: %u\n", n, p);

	return 0;
}

