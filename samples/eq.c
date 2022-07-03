#include <stdio.h>
#include <stdlib.h>

void eq(unsigned int a, unsigned int b, unsigned c, unsigned int *d, unsigned int *e);

int main(int argc, char **argv) {
	unsigned int a = atoi(argv[1]);
	unsigned int b = atoi(argv[2]);
	unsigned int c = atoi(argv[3]);

	unsigned int d, e;

	eq(a, b, c, &d, &e);

	printf(	"%u * %u + %u = %u\n"
			"%u * %u - %u = %u\n",
			a, b, c, d,
			b, c, a, e);

	return 0;
}

