
%{
#include <stdio.h>
#include "op.h"
#include "stack.h"
#include "valstack.h"

extern int yylex();
extern struct stack g_reader_stack;
void yyerror(const char* s);

%}

%union {
  enum opcode opcode;
  s64 integer;
  struct val val;
  const char* str;
}

%token T_OP
%token T_INT
%token T_VAL
%token T_NEWLINE T_QUIT T_EXAMPLE

%type<opcode> T_OP
%type<integer> T_INT
%type<val> T_VAL
%type<str> T_EXAMPLE

%start script

%%

script:
      | script line
;

line: T_NEWLINE
    | T_INT { script_push_int(&g_reader_stack, $1); }
    | T_VAL { stack_push_val(&g_reader_stack, $1); }
    | T_OP { stack_push_op(&g_reader_stack, $1); }
    | T_EXAMPLE { ; }


%%
