
%{
#include <stdio.h>
#include "op.h"
#include "stack.h"

extern int yylex();
extern struct stack reader_stack;
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
    | T_OP { stack_push(&reader_stack, $1); }
    | T_EXAMPLE { ; }


%%
