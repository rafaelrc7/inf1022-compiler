#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provolone_program.h"
#include "provolone_compiler.h"
#include "provolone_parser.h"

#ifdef YYDEBUG
	extern int yydebug;
#endif

extern FILE *yyin;

int main(int argc, char **argv) {
	unsigned int count = 0;
	ProvolProgram *p;
	FILE *in, *tmp, *out;

#ifdef YYDEBUG
	if (argc > 1 && strcmp(argv[1], "--debug") == 0)
		yydebug = 1;
#endif

	in = stdin;
	out = stdout;
	while (1) {
		p = provol_prog_create();

		yyin = in;
		yyparse(p);
		provol_prog_print_tree(p);
		if (in != stdin)
			fclose(in);

		if (p->is_bootstrapped) {
			break;
		} else {
			tmp = tmpfile();
			provol_pc(tmp, p);
			rewind(tmp);
			in = tmp;
			provol_prog_free(p);
		}
	}

	provol_cc(out, p);

	provol_prog_free(p);

	return 0;
}

