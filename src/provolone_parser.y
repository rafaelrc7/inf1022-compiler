%{
	#include <stdio.h>
	#include <stdlib.h>

	int yylex(void);
%}

%define api.value.type {const char *}
%token PROGRAM ENTRADA SAIDA ENQUANTO FACA FIM ID '(' ')' '=' ','

%%

program		:	%empty
			|	ENTRADA varlist SAIDA varlist cmds FIM
			;

varlist		:	ID
			|	varlist ',' ID
			;

cmds		:	cmd
			|	cmds cmd
			;

cmd			:	ENQUANTO ID FACA cmds FIM
			|	ID '=' ID
			|	ID '(' ID ')'
			;

%%

#include <stdio.h>

void yyerror(const char *s) {
	fprintf(stderr, "%s\n", s);
}

