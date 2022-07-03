#include <stdio.h>
#include <stdlib.h>

void iseven(unsigned int n, unsigned int *e);

int main(int argc, char **argv) {
	unsigned int n = atoi(argv[1]);
	unsigned int e;

	iseven(n, &e);

	printf("is %u even: %s\n", n, e ? "true" : "false");

	return 0;
}

