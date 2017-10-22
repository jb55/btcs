
%{

#include <stdio.h>
#include <stdlib.h>
#include "script.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;

char buffer[255];

void yyerror(const char* s);
%}

%union {
  enum opcode opcode;
  const char* str;
}

%token T_OP
%token T_NEWLINE T_QUIT T_EXAMPLE

%type<opcode> T_OP
%type<str> T_EXAMPLE

%start script

%%

script:
      | script line
;

line: T_NEWLINE
    | T_OP { printf("%s\n", op_name($1)); }
    | T_EXAMPLE { printf("ex:%s\n", $1); }

/* opcode: */
/*       | T_OP { op_add($$); } */
/*       ; */


%%

int main() {
  yyin = stdin;

  do {
    yyparse();
  } while(!feof(yyin));

  return 0;
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}
