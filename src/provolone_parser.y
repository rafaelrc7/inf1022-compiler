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
%token PROGRAM ENTRADA SAIDA ENQUANTO FACA REPITA VEZES FIM SE ENTAO SENAO '(' ')' '=' ',' '+' '-' '*'

%token <const char *> ID
%nterm <LinkedList *> varlist
%nterm <LinkedList *> cmds
%nterm <ProvolCmd *> cmd
%nterm <ProvolExpr *> expr

%%

program		:	%empty
			|	PROGRAM program_id in_def out_def cmds FIM { provol_prog_add_cmds(p, $5);							}
			;

program_id	:	%empty							{ provol_program_set_name(p, NULL);									}
			|	ID								{ provol_program_set_name(p, $1);									}
			;

in_def		:	ENTRADA varlist					{ provol_prog_add_ins(p, $2);										}
			;

out_def		:	SAIDA varlist					{ provol_prog_add_outs(p, $2);										}
			;

varlist		:	ID								{ $$ = provol_var_append(NULL, $1);									}
			|	varlist ',' ID					{ $$ = provol_var_append($1, $3);									}
			;

cmds		:	cmd								{ $$ = provol_cmds_append(NULL, $1);								}
			|	cmds cmd						{ $$ = provol_cmds_append($1, $2);									}
			;

cmd			:	ENQUANTO ID FACA cmds FIM		{ $$ = provol_wloop_new(p, $2, $4);									}
			|	ID '=' ID						{ $$ = provol_assign_new(p, $1, $3);								}
			|	ID '(' ID ')'					{ $$ = provol_call_new(p, $1, $3);									}
			|	SE ID ENTAO cmds SENAO cmds FIM	{ p->is_bootstrapped = 0; $$ = provol_if_new(p, $2, $4, $6);		}
			|	SE ID ENTAO cmds FIM			{ p->is_bootstrapped = 0; $$ = provol_if_new(p, $2, $4, NULL);		}
			|	REPITA ID VEZES cmds FIM		{ p->is_bootstrapped = 0; $$ = provol_doloop_new(p, $2, $4);		}
			|	ID '=' expr						{ p->is_bootstrapped = 0; $$ = provol_assign_expr_new(p, $1, $3);	}
			;

expr		:	ID '+' ID						{ p->is_bootstrapped = 0; $$ = provol_expr_new(p, $1, $3, '+');		}
			|	ID '-' ID						{ p->is_bootstrapped = 0; $$ = provol_expr_new(p, $1, $3, '-');		}
			|	ID '*' ID						{ p->is_bootstrapped = 0; $$ = provol_expr_new(p, $1, $3, '*');		}
			;

%%

#include <stdio.h>

void yyerror(struct provol_program *p, const char *s) {
	(void)(p); /* nop - supress unused parameter warning */
	fprintf(stderr, "%s\n", s);
}

