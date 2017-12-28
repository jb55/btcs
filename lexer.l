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

#[^\n]* { return T_COMMENT; }

[oO][pP]_ {}

\"[^\"]*\" {
  char *t = yytext + 1;
  t[strlen(t) - 1] = '\0';
  yylval.str = t;
  return T_STR;
}

0x[a-fA-F0-9]+ {
  char * t = yytext + 2;
  if (strlen(t) % 2 != 0) {
     yylval.str = "Invalid raw string, it must have even length";
     return T_ERR;
  }
  yylval.str = t;
  return T_RAW;
}

@[a-fA-F0-9]+ {
  char *t = yytext + 1;
  if (strlen(t) % 2 != 0) {
    yylval.str = "Invalid data string, it must have even length";
    return T_ERR;
  }
  yylval.str = t;
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
