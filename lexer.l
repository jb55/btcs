%option noyywrap

%{
#include <stdio.h>
#include "script.h"

#define YY_DECL int yylex()


#include "parser.tab.h"

%}

%%

[ \t]	; // ignore all whitespace
\n		               {return T_NEWLINE;}

(op_)?[a-zA-Z0-9]+ {
  yylval = op_tokenize(yytext);
  return T_OP;
}

%%
