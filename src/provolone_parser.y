%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "provolone_program.h"

	void yyerror(const char *s);
	int yylex(void);
%}

%parse-param {ProvolProgram *p}

%define api.value.type {const char *}
%token PROGRAM ENTRADA SAIDA ENQUANTO FACA FIM ID '(' ')' '=' ','

%%

program		:	%empty
			|	PROGRAM ENTRADA varlist SAIDA varlist cmds FIM {
					provol_prog_add_ins(p, $3);
					provol_prog_add_outs(p, $5);
					provol_prog_add_cmds(p, $6);
				}
			;

varlist		:	ID							{ $$ = provol_var_append(NULL, $1);		}
			|	varlist ',' ID				{ $$ = provol_var_append($1, $3);		}
			;

cmds		:	cmd							{ $$ = provol_cmds_append(NULL, $1);	}
			|	cmds cmd					{ $$ = provol_cmds_append($1, $2);		}
			;

cmd			:	ENQUANTO ID FACA cmds FIM	{ $$ = provol_wloop_new(p, $2, $4);		}
			|	ID '=' ID					{ $$ = provol_assign_new(p, $1, $3);	}
			|	ID '(' ID ')'				{ $$ = provol_call_new(p, $1, $3);		}
			;

%%

#include <stdio.h>

void yyerror(const char *s) {
	fprintf(stderr, "%s\n", s);
}

