#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provolone_parser.h"

#ifdef YYDEBUG
	extern int yydebug;
#endif

int main(int argc, char **argv) {
#ifdef YYDEBUG
	if (argc > 1 && strcmp(argv[1], "--debug") == 0)
		yydebug = 1;
#endif
	return yyparse();
}

