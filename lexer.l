%option noyywrap

%{
#include <stdio.h>
#include "script.h"
#include "op.h"

#define YY_DECL int yylex()

#include "parser.tab.h"

%}

%%

[ \t]	; // ignore all whitespace
\n		{return T_NEWLINE;}

\<[^\>]+\> { yylval.str = yytext; return T_EXAMPLE; }

\[[^\]]+\] { yylval.str = yytext; return T_EXAMPLE; }


[oO][pP]_ {}

[a-zA-Z0-9]+ {
  yylval.opcode = op_tokenize(yytext);
  return T_OP;
}

%%
