%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "provolone_program.h"
	#include "provolone_parser.h"

	int yywrap(void) { return 1; }
%}

%option nounput
%option noinput

%%

[[:space:]]+				/* skip */
"PROGRAM"					{ return PROGRAM;	}
"ENTRADA"					{ return ENTRADA;	}
"SAIDA"						{ return SAIDA;		}
"ENQUANTO"					{ return ENQUANTO;	}
"FACA"						{ return FACA;		}
"REPITA"					{ return REPITA;	}
"VEZES"						{ return VEZES;		}
"FIM"						{ return FIM;		}
"SE"						{ return SE;		}
"ENTAO"						{ return ENTAO;		}
"SENAO"						{ return SENAO;		}
[()=,+*-]					{ return yytext[0];	} /* single char tokens */
[[:alpha:]][[:alnum:]]*		{ yylval.ID = strdup(yytext); return ID; }

%%

