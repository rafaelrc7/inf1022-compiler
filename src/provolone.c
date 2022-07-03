#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "provolone_program.h"
#include "provolone_compiler.h"
#include "provolone_parser.h"

#if YYDEBUG == 1
	extern int yydebug;
#endif

#ifdef PACKAGE_STRING
const char *argp_program_version = PACKAGE_STRING;
#else
const char *argp_program_version = "Provol 1.0";
#endif

#ifdef PACKAGE_BUGREPORT
const char *argp_program_bug_address = "<" PACKAGE_BUGREPORT ">";
#endif

static struct argp_option options[] = {
	{"output",	'o', "FILE",	0, "Output to FILE instead of stdout.", 0},
	{"tree",	't', 0,			0, "Output program tree instead of compilation.", 0},
	{"quit",	'q', 0, 		0, "Does not output result.", 0},
	{"cc",		'c', 0, 		0, "Enables compilation and set target to C. Enabled by default.", 0},
	{"keep",	'k', 0, 		0,
		"Keeps intermediate bootstrapping source code. Files will be save in current dir.", 0},
	{"debug",	'd', 0,			0, "Enable debug mode. By default enables quiet mode.", 0},
	{ NULL }
};

struct state {
	char *output_file;
	enum { COMPILE_C, TREE, QUIET, } mode;
	unsigned char keep_intermediate_files;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

#define DOC "Simple bootstrapping provol-one compiler."
#define ARGS_DOC "< [input file]"
static struct argp argp = { options, parse_opt, ARGS_DOC, DOC, NULL, NULL, NULL, };

#undef DOC
#undef ARGS_DOC

extern FILE *yyin;

int main(int argc, char **argv) {
	unsigned int count = 0;
	ProvolProgram *p;
	FILE *in, *tmp, *out;
	struct state prog_state;

	prog_state.keep_intermediate_files = 0;
	prog_state.mode = COMPILE_C;
	prog_state.output_file = NULL;

	argp_parse(&argp, argc, argv, 0, 0, &prog_state);

	in = stdin;
	out = stdout;
	while (1) {
		p = provol_prog_create();

		yyin = in;
		if (yyparse(p) != 0) {
			exit(EXIT_FAILURE);
		}
		if (in != stdin)
			fclose(in);

		if (p->is_bootstrapped) {
			break;
		} else {
			tmp = tmpfile();
			provol_pc(tmp, p);
			rewind(tmp);
			in = tmp;

			if (prog_state.mode == TREE) {
				printf("---- TREE %u ----\n", count);
				provol_prog_print_tree(p);
				printf("-----------------\n\n");
			}
			provol_prog_free(p);
		}
	}

	if (prog_state.output_file != NULL) {
		out = fopen(prog_state.output_file, "w");
		if (out == NULL) {
			perror("fopen()");
			provol_prog_free(p);
			exit(EXIT_FAILURE);
		}
	}

	if (prog_state.mode == COMPILE_C) {
		provol_cc(out, p);
	}

	provol_prog_free(p);

	return 0;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct state *prog_state = state->input;
	switch (key) {
	case 'o':
		prog_state->output_file = arg;
		break;

	case 't':
		prog_state->mode = TREE;
		break;

	case 'q':
		prog_state->mode = QUIET;
		break;

	case 'c':
		prog_state->mode = COMPILE_C;
		break;

	case 'k':
		prog_state->keep_intermediate_files = 1;
		break;

	case 'd':
#if YYDEBUG == 1
		yydebug = 1;
		prog_state->mode = QUIET;
#else
		fprintf(stderr, "parser was compiled without debug info!. Disabling debug.\n");
#endif
		break;

	case ARGP_KEY_ARG:
		return 0;

	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

