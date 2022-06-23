%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "provolone_program.h"

	void yyerror(ProvolProgram *p, const char *s);
	int yylex(void);
%}

%parse-param {ProvolProgram *p}

%define api.value.type union
%token PROGRAM ENTRADA SAIDA ENQUANTO FACA FIM SE ENTAO SENAO '(' ')' '=' ','

%token <const char *> ID
%nterm <LinkedList *> varlist
%nterm <LinkedList *> cmds
%nterm <ProvolCmd *> cmd

%%

program		:	%empty
			|	PROGRAM ENTRADA varlist SAIDA varlist cmds FIM {
					provol_prog_add_ins(p, $3);
					provol_prog_add_outs(p, $5);
					provol_prog_add_cmds(p, $6);
				}
			;

varlist		:	ID								{ $$ = provol_var_append(NULL, $1);								}
			|	varlist ',' ID					{ $$ = provol_var_append($1, $3);								}
			;

cmds		:	cmd								{ $$ = provol_cmds_append(NULL, $1);							}
			|	cmds cmd						{ $$ = provol_cmds_append($1, $2);								}
			;

cmd			:	ENQUANTO ID FACA cmds FIM		{ $$ = provol_wloop_new(p, $2, $4);								}
			|	ID '=' ID						{ $$ = provol_assign_new(p, $1, $3);							}
			|	ID '(' ID ')'					{ $$ = provol_call_new(p, $1, $3);								}
			|	SE ID ENTAO cmds SENAO cmds FIM	{ p->is_bootstrapped = 0; $$ = provol_if_new(p, $2, $4, $6);		}
			|	SE ID ENTAO cmds FIM			{ p->is_bootstrapped = 0; $$ = provol_if_new(p, $2, $4, NULL);	}
			;

%%

#include <stdio.h>

	void yyerror(struct provol_program *p, const char *s) {
	fprintf(stderr, "%s\n", s);
}

