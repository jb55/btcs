
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "script.h"
#include "alloc.h"

extern int yyparse();
extern FILE* yyin;

struct stack reader_stack;

void yyerror(const char* s);

int main() {
  yyin = stdin;

  struct stack tmp_stack;
  alloc_arenas(0, MAX_STACK_SIZE, MAX_STACK_SIZE * MAX_STACK_SIZE);
  stack_init(&reader_stack);
  stack_init(&tmp_stack);

  do {
    yyparse();
  } while(!feof(yyin));

  script_eval(&reader_stack, &tmp_stack);
  printf("script: ");
  script_print_ops(&reader_stack);
  printf("stack:  ");
  script_print_vals(&tmp_stack);

  stack_free(&reader_stack);
  stack_free(&tmp_stack);
  free_arenas(0);

  return 0;
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}

