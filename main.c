
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "script.h"

extern int yyparse();
extern FILE* yyin;

struct stack reader_stack;

void yyerror(const char* s);

int main() {
  yyin = stdin;

  struct stack tmp_stack;
  stack_init(&reader_stack);
  stack_init(&tmp_stack);

  do {
    yyparse();
  } while(!feof(yyin));

  script_eval(&reader_stack, &tmp_stack);
  printf("script: ");
  script_print(&reader_stack);
  printf("stack:  ");
  script_print(&tmp_stack);

  stack_free(&reader_stack);
  stack_free(&tmp_stack);

  return 0;
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}

