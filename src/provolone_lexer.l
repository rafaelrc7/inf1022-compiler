%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "provolone_parser.h"

	int yywrap(void) { return 1; }
%}

%%

[[:space:]]+				/* skip */
"Program"					{ return PROGRAM;	}
"ENTRADA"					{ return ENTRADA;	}
"SAIDA"						{ return SAIDA;		}
"ENQUANTO"					{ return ENQUANTO;	}
"FACA"						{ return FACA;		}
"FIM"						{ return FIM;		}
[()=,]						{ return yytext[0];	} /* single char tokens */
[[:alpha:]][[:alnum:]]*		{ yylval = strdup(yytext); return ID; }

%%
