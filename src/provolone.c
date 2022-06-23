#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provolone_program.h"
#include "provolone_parser.h"

#ifdef YYDEBUG
	extern int yydebug;
#endif

int main(int argc, char **argv) {
	ProvolProgram *p = provol_prog_create();
	if (p == NULL) {
		perror("provol_program_create()");
		exit(EXIT_FAILURE);
	}

#ifdef YYDEBUG
	if (argc > 1 && strcmp(argv[1], "--debug") == 0)
		yydebug = 1;
#endif

	yyparse(p);

	provol_prog_free(p);

	return 0;
}

