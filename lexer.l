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

@[a-fA-F0-9]+ {
  yylval.str = yytext + 1;
  return T_DATA;
}

-?[0-9]+ {
  s64 i = strtoll(yytext, NULL, 10);
  yylval.val = val_from_int(i);
  return T_VAL;
}

[a-zA-Z0-9]+ {
  yylval.opcode = op_tokenize(yytext);
  return T_OP;
}

%%
